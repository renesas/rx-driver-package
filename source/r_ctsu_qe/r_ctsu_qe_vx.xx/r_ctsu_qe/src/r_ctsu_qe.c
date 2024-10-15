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
* Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
************************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_ctsu_qe.c
* Description  : This file implements the QE CTSU API
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#ifdef QE_TOUCH_CONFIGURATION
 #include "qe_touch_define.h"
 #include "qe_touch_config.h"
#endif
#include "r_ctsu_qe_if.h"
#include "r_ctsu_qe_pinset.h"
#if (CTSU_CFG_DTC_SUPPORT_ENABLE == 1)
#include "r_dtc_rx_if.h"
#endif
#if ((CTSU_CFG_DIAG_SUPPORT_ENABLE == 1) || (CTSU_CFG_CALIB_RTRIM_SUPPORT==1))
 #if (BSP_FEATURE_CTSU_VERSION == 2)
 #include "r_s12ad_rx_if.h"
 #endif
#endif

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** "CTSU" in ASCII, used to determine if device is open. */
#define CTSU_OPEN                            (0x43545355U)

/* Macro definitions for register setting */
#define CTSU_PON_OFF                         (0)    // CTSU hardware macro power off
#define CTSU_PON_ON                          (1)    // CTSU hardware macro power on
#define CTSU_CSW_OFF                         (0)    // Capacitance switch turned off
#define CTSU_CSW_ON                          (1)    // Capacitance switch turned on

#define CTSU_CR1_MODIFY_BIT                  (0xC8) // MD1, MD0, ATUNE1
#define CTSU_SOVF                            (0x20) // Overflow bit
#define CTSU_CORRECTION_AVERAGE              (32)
#define CTSU_SHIFT_AMOUNT                    (15)

#define CTSU_PCLKB_FREQ_MHZ                  (1000000)
#define CTSU_PCLKB_FREQ_RANGE1               (32)
#define CTSU_PCLKB_FREQ_RANGE2               (64)
#define CTSU_PCLKB_FREQ_RANGE3               (128)
#define CTSU_WAIT_TIME                       (500)

/* Macro definitions for initial offset tuning */
#define CTSU_TUNING_MAX                      (0x03FF)
#define CTSU_TUNING_MIN                      (0x0000)
#define CTSU_TUNING_VALUE_SELF               (15360)
#define CTSU_TUNING_VALUE_MUTUAL             (10240)
#define CTSU_TUNING_OT_COUNT                 (25)

#define CTSU_CFG_DECIMAL_POINT               (16)

#if (BSP_FEATURE_CTSU_VERSION == 2)
 #define CTSU_SST_RECOMMEND                  (0x1F) // The recommend value of SST
 #define CTSU_SST_RECOMMEND_CURRENT          (0x3F) // The recommend value of SST with current
 #define CTSU_SNUM_RECOMMEND                 (0x07) // The value of SNUM should be fixed
 #define CTSU_SNUM_MAX                       (0xFF) // The maximum value of SNUM
 #define CTSU_ICOMP0                         (0x80) // ICOMP0 bit
 #define CTSU_ICOMP1                         (0x40) // ICOMP1 bit
 #define CTSU_ICOMPRST                       (0x20) // ICOMPRST bit
 #define CTSU_CR0_MODIFY_BIT                 (0xC0) // TXVSEL
 #define CTSU_CR2_MODIFY_BIT                 (0x33) // POSEL, ATUNE2, MD2
 #define CTSU_SUADJ_MAX                      (0xFF) // The maximum value of SUADJx
 #define CTSU_SUADJ_SSCNT_ADJ                (0x20) // The value of Adjusting SCADJx by SSCNT
 #define CTSU_MUTUAL_BUF_SIZE                (CTSU_CFG_NUM_SUMULTI * 2)
 #define CTSU_SUMULTI_BASE                   (0x3F) // SUCLK base value (32MHz)

/* Macro definitions for correction */
 #if (CTSU_CFG_LOW_VOLTAGE_MODE == 0)
  #define CTSU_CORRECTION_STD_VAL            (19200) // 20UC standard value
  #define CTSU_CORRECTION_STD_UNIT           (1920)  // 2UC value
  #define CTSU_CORRECTION_STD_EXREG          (14400) // External registance standard value
  #define CTSU_CORRECTION_OFFSET_UNIT        (120)   // (7680 / 64)
 #else
  #define CTSU_CORRECTION_STD_VAL            (15360) // 20UC standard value * 0.8
  #define CTSU_CORRECTION_STD_UNIT           (1536)  // 2UC value * 0.8
  #define CTSU_CORRECTION_STD_EXREG          (11520) // External registance standard value
  #define CTSU_CORRECTION_OFFSET_UNIT        (96)    // (7680 / 64) * 0.8
 #endif
 #define CTSU_CORRECTION_SUMULTI             (0x20)  // SUMULTI step
 #define CTSU_CORRECTION_TRIMB_MAX           (0xFF)
 #define CTSU_CORRECTION_TRIMB_SIGN_BIT      (0x80)
 #define CTSU_CORRECTION_RTRIM_THRESHOLD1    (0xA0)
 #define CTSU_CORRECTION_RTRIM_THRESHOLD2    (0x50)
 #define CTSU_CORRECTION_TRIMB_THRESHOLD1    (0xC0)
 #define CTSU_CORRECTION_TRIMB_THRESHOLD2    (0x3F)
 #define CTSU_CORRECTION_BIT16               (0x10000)
 #define CTSU_CORRECTION_BIT10               (0x0400)
 #define CTSU_CORRECTION_BIT9                (0x0200)
 #define CTSU_CORRECTION_BIT8                (0x0100)
 #define CTSU_CORRECTION_BIT7                (0x0080)

 #if (CTSU_CFG_NUM_CFC != 0)
  #define CTSU_CORRCFC_CENTER_POINT          ((CTSU_CORRCFC_POINT_NUM - 1) / 2) // number of center point
  #define CTSU_CORRCFC_TS_MAX                (36)                               // Maximum number of TS terminal
  #define CTSU_CORRCFC_SHIFT8                (8)                                // Definition of 8bit shift
 #endif

 #if (CTSU_CFG_CALIB_RTRIM_SUPPORT == 1)
  #if (CTSU_CFG_LOW_VOLTAGE_MODE == 0)
   #define CTSU_CALIB_REF                    ((6144000 * 10) / CTSU_CFG_VCC_MV) // 1.5V Reference value (4096 * 1500 * 10)
  #else
   #define CTSU_CALIB_REF                    ((4915200 * 10) / CTSU_CFG_VCC_MV) // 1.2V Reference value (4096 * 1200 * 10)
  #endif
  #define CTSU_CALIB_AVERAGE_TIME            (64)                               // ADC average time
  #define CTSU_CALIB_THRESHOLD               ((0x1000 * 4) / CTSU_CFG_VCC_MV)   // RTRIM calib threshold
  #define CTSU_CALIB_CTSUSO                  (0x3C0)                            // 150uA offset
  #define CTSU_CALIB_ADSSTRL                 (0x3F)                             // Sampling time
 #endif

#endif
#if (BSP_FEATURE_CTSU_VERSION == 1)
 #define CTSU_SST_RECOMMEND                  (0x10) // The value of SST should be fixed to 00010000b
 #define CTSU_SNUM_MAX                       (0x3F) // The maximum value of SNUM
 #define CTSU_SDPA_MAX                       (0x1F) // The maximum value of SDPA
 #define CTSU_PRRATIO_RECOMMEND              (3)    // Recommended setting value
 #define CTSU_PRMODE_62_PULSES               (2)    // 62 pulses (recommended setting value)
 #define CTSU_SOFF_ON                        (0)    // High-pass noise reduction function turned on
 #define CTSU_SSMOD                          (0)    // The value of SSMOD should be fixed to 00b
 #define CTSU_SSCNT                          (3)    // The value of SSCNT should be fixed to 11b
 #define CTSU_RICOA_RECOMMEND                (0x0F) // Recommended setting value
 #define CTSU_ICOG_100                       (0)    // ICOG = 100%
 #define CTSU_ICOG_66                        (1)    // ICOG = 66%
 #define CTSU_ICOG_50                        (2)    // ICOG = 50%
 #define CTSU_ICOG_40                        (3)    // ICOG = 40%
 #define CTSU_MUTUAL_BUF_SIZE                (1)

/* Macro definitions for correction */
 #if (CTSU_CFG_MCU_PROCESS_MF3 == 1)
  #if (CTSU_CFG_LOW_VOLTAGE_MODE == 0)
   #define CTSU_CORRECTION_1ST_STD_VAL       (40960UL)       // ICOG = 66%
   #define CTSU_CORRECTION_2ND_STD_VAL       (24824)         // ICOG = 40%, (x = 40960 * 40 / 66)
  #else
   #define CTSU_CORRECTION_1ST_STD_VAL       (32768UL)       // ICOG = 66%
   #define CTSU_CORRECTION_2ND_STD_VAL       (19859)         // ICOG = 40%, (x = 40960 * 40 / 66)
  #endif
  #define CTSU_WAFER_PARAMETER               (0.96523525)
  #define CTSU_ICOG_RECOMMEND                (CTSU_ICOG_66)  // Recommended setting value
 #endif
 #if (CTSU_CFG_MCU_PROCESS_40N_PHASE2 == 1)
  #define CTSU_CORRECTION_1ST_STD_VAL        (27306UL)       // ICOG = 66%, (x = 40960 * 66 / 100)
  #define CTSU_CORRECTION_2ND_STD_VAL        (16384)         // ICOG = 40%, (x = 40960 * 40 / 100)
  #define CTSU_WAFER_PARAMETER               (1)
  #define CTSU_ICOG_RECOMMEND                (CTSU_ICOG_100) // Recommended setting value
 #endif
#endif

#if (BSP_FEATURE_CTSU_VERSION == 1)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)

  #define CTSU_DIAG_DAC_1UC             (0x10)  // 0x10 for so dac value
  #define CTSU_DIAG_DAC_2UC             (0x20)  // 0x20 for so dac value
  #define CTSU_DIAG_DAC_4UC             (0x40)  // 0x40 for so dac value
  #define CTSU_DIAG_DAC_8UC             (0x80)  // 0x80 for so dac value
  #define CTSU_DIAG_DAC_16UC            (0x100) // 0x100 for so dac value

  #define CTSU_DIAG_DAC_SO_MAX          (0x3FF) // so dac max

  #define CTSU_DIAG_TSOC_BIT            (0x80)                     // TSOC bit 1
  #define CTSU_DIAG_CLKSEL1_BIT         (0x40)                     // clksel bit 1
  #define CTSU_DIAG_DAC_INIT_VALUE      (241)   // SO value of dac test
  #define CTSU_DIAG_DAC_TARGET_VALUE    (15360) // 6UC value dac test target
  #define CTSU_DIAG_DAC_START_VALUE     (0x100) // so value dac test tuning
  #define CTSU_DAC_TEST_ATUNE1          (0x08)  // ATUNE1 bit 1
 #endif
#endif

#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
  #define CTSU_DIAG_ADSSTRL             (0xff)   // ADC scan ADSSTAL register
  #define CTSU_DIAG_DAC_INIT_0          (0x0)    // so dac Lower current source0 init
  #define CTSU_DIAG_DAC_INIT_1          (0x100)  // so dac Lower current source1 init
  #define CTSU_DIAG_DAC_INIT_2          (0x200)  // so dac Lower current source2 init
  #define CTSU_DIAG_DAC_INIT_3          (0x300)  // so dac Lower current source3 init

  #define CTSU_DIAG_DAC_DATA_MAX_0      (0x0ff)  // so dac Lower current source0 max num
  #define CTSU_DIAG_DAC_DATA_MAX_1      (0x1ff)  // so dac Lower current source1 max num
  #define CTSU_DIAG_DAC_DATA_MAX_2      (0x2ff)  // so dac Lower current source2 max num
  #define CTSU_DIAG_DAC_DATA_MAX_3      (0x3ff)  // so dac Lower current source3 max num

  #define CTSU_DIAG_CURRENT_CLIB_REG    (0x818)  // current test calib register

  #define CTSU_DIAG_DELAY_MS            (5)      // delay time (ms)
  #define CTSU_DIAG_SUCLK0_REG1         (0xff00) // cco gain test SUCLK0
  #define CTSU_DIAG_SUCLK0_REG2         (0x20)   // cco gain test SUCLK0
  #define CTSU_DIAG_STCLK_FREQ          (500)    // stclk freqency (Hz)
  #define CTSU_DIAG_LVM_MASK            (0x400)  // LVmode mask
  #define CTSU_DIAG_CFC_SDPA_REG        (0x3F)   // cfc scan sdpa
  #define CTSU_DIAG_CHACA_TSMAX         (32)     // ts max chaca byte

  #define CTSU_DIAG_DAC_0BIT            (0x1)    // 0x1 for SO register
  #define CTSU_DIAG_DAC_1BIT            (0x2)    // 0x2 for SO register
  #define CTSU_DIAG_DAC_2BIT            (0x4)    // 0x4 for SO register
  #define CTSU_DIAG_DAC_3BIT            (0x8)    // 0x8 for SO register
  #define CTSU_DIAG_DAC_4BIT            (0x10)   // 0x10 for SO register
  #define CTSU_DIAG_DAC_5BIT            (0x20)   // 0x20 for SO register
  #define CTSU_DIAG_DAC_6BIT            (0x40)   // 0x40 for SO register
  #define CTSU_DIAG_DAC_7BIT            (0x80)   // 0x80 for SO register

 #endif
#endif

#if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
 #define CTSU_AUTO_JUDGE_BLOCK_SIZE     (5)  // Block size of automatic judgement register to be transferred by DTC
 #define CTSU_AUTO_CTSUSO_BLOCK_SIZE    (1)  // Block size of CTSUSO register to be transferred by DTC for automatic judgement(same for CTSUSCNT register)
 #define CTSU_AUTO_FINAL_JUDGE_BIT      (4)  // Definition for bit-shifting the final judgement bit of the automatic judgement result register
 #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
 #define CTSU_AUTO_MCACT_BLOCK_SIZE     (1)  // Block size of CTSUMCACT register to be transferred by DTC
 #endif
#endif

#if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 1)
 #define CTSU_AUTO_DECIMAL_POINT_NUM    (12)     // Number of bits after the decimal point of the sensor counter auto correction table register
#endif

#if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
 #define CTSU_MULTICLOCK_AUTO_CORRECTION_BASE_VALUE (7680)
#endif

#if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
 #define CTSU_SERIAL_TUNING_ELEMENT     (1)  // To avoid reading outside the array in the case of mutual 1 x N configuration
#else
 #define CTSU_SERIAL_TUNING_ELEMENT     (0)  // No additional buffer required
#endif

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/
typedef struct st_ctsu_correction_calc
{
    ctsu_range_t range;
    uint16_t     snum;
    uint16_t     sdpa;
    uint16_t     cfc;
    ctsu_md_t    md;
} ctsu_correction_calc_t;

typedef struct st_ctsu_correction_multi
{
    uint16_t pri[CTSU_CFG_NUM_SUMULTI];
    uint16_t snd[CTSU_CFG_NUM_SUMULTI];
    uint32_t offset[CTSU_CFG_NUM_SUMULTI];
    uint8_t  selected_freq;
} ctsu_correction_multi_t;

#if (BSP_FEATURE_CTSU_VERSION == 1)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
typedef struct st_ctsu_diag_save_reg
{
    uint8_t  ctsucr0;
    uint8_t  ctsucr1;
    uint8_t  ctsusdprs;
    uint8_t  ctsusst;
    uint8_t  ctsuchac0;
    uint8_t  ctsuchac1;
    uint8_t  ctsuchac2;
    uint8_t  ctsuchtrc0;
    uint8_t  ctsuchtrc1;
    uint8_t  ctsuchtrc2;
    uint8_t  ctsudclkc;
    uint16_t ctsuerrs;
} ctsu_diag_save_reg_t;
 #endif
#endif

#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
typedef struct st_ctsu_diag_save_reg
{
    uint32_t ctsucra;
    uint32_t ctsucrb;
    uint32_t ctsuchaca;
    uint32_t ctsuchacb;
    uint32_t ctsuchtrca;
    uint32_t ctsuchtrcb;
    uint32_t ctsumch;
    uint32_t ctsucalib;
    uint32_t ctsusuclka;
    uint32_t ctsusuclkb;
} ctsu_diag_save_reg_t;
 #endif
#endif

#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
#elif defined(__CCRL__) || defined(__ICCRL__) || defined(__RL78__)
#else
 #if defined(__ARMCC_VERSION) || defined(__ICCARM__)
typedef void (BSP_CMSE_NONSECURE_CALL * ctsu_prv_ns_callback)(ctsu_callback_args_t * p_args);
 #elif defined(__GNUC__)
typedef BSP_CMSE_NONSECURE_CALL void (*volatile ctsu_prv_ns_callback)(ctsu_callback_args_t * p_args);
 #endif
#endif

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
#if (CTSU_CFG_DTC_SUPPORT_ENABLE == 1)
static fsp_err_t ctsu_transfer_open(ctsu_instance_ctrl_t * const p_instance_ctrl);
static fsp_err_t ctsu_transfer_configure(ctsu_instance_ctrl_t * const p_instance_ctrl);
 #if (BSP_FEATURE_CTSU_VERSION == 2)
static dtc_err_t ctsu_transfer_normal (ctsu_instance_ctrl_t * const p_instance_ctrl);
static dtc_err_t ctsu_transfer_normal_ctsuwr (ctsu_instance_ctrl_t * const p_instance_ctrl);
static dtc_err_t ctsu_transfer_normal_ctsurd (ctsu_instance_ctrl_t * const p_instance_ctrl);
  #if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
static dtc_err_t ctsu_transfer_autojudge (ctsu_instance_ctrl_t * const p_instance_ctrl);
static dtc_err_t ctsu_transfer_autojudge_ctsuwr (ctsu_instance_ctrl_t * const p_instance_ctrl);
static dtc_err_t ctsu_transfer_autojudge_ctsurd (ctsu_instance_ctrl_t * const p_instance_ctrl);
static void ctsu_transfer_autojudge_value_set (uint8_t array_number, dtc_repeat_block_side_t set_repeat_block_side, uint32_t set_transfer_count);
static void ctsu_transfer_autojudge_repeat_set (uint8_t array_number, dtc_chain_transfer_t set_chain_transfer_enable, dtc_repeat_block_side_t set_repeat_block_side);
  #endif
  #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
static void ctsu_transfer_mcact_set (uint8_t array_number, uint32_t set_transfer_count);
  #endif
 #endif
 #if (BSP_FEATURE_CTSU_VERSION == 1)
static dtc_err_t ctsu_transfer_ctsu1 (ctsu_instance_ctrl_t * const p_instance_ctrl);
static dtc_err_t ctsu_transfer_ctsu1_ctsuwr (ctsu_instance_ctrl_t * const p_instance_ctrl);
static dtc_err_t ctsu_transfer_ctsu1_ctsurd (ctsu_instance_ctrl_t * const p_instance_ctrl);
 #endif
static void ctsu_transfer_ctsuso_set (uint8_t array_number, dtc_chain_transfer_t set_chain_transfer_enable, dtc_chain_transfer_mode_t set_chain_transfer_mode, uint32_t set_transfer_count);
static void ctsu_transfer_ctsuscnt_set (uint8_t array_number, dtc_chain_transfer_t set_chain_transfer_enable, dtc_chain_transfer_mode_t set_chain_transfer_mode, uint32_t set_transfer_count);
static void ctsu_transfer_address_set (uint8_t array_number, uint32_t * p_set_source_addr, uint32_t * p_set_dest_addr);
static fsp_err_t ctsu_transfer_close(ctsu_instance_ctrl_t * const p_instance_ctrl);
#endif
static void ctsu_initial_offset_tuning(ctsu_instance_ctrl_t * const p_instance_ctrl);
static void ctsu_moving_average(ctsu_data_t * p_average, uint16_t new_data, uint16_t average_num);
static void ctsu_end_interrupt(ctsu_instance_ctrl_t * const p_instance_ctrl);
static void ctsu_correction_process(ctsu_instance_ctrl_t * const p_instance_ctrl);
static void ctsu_correction_measurement(ctsu_instance_ctrl_t * const p_instance_ctrl, uint16_t * data);
#if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 0)
static void ctsu_correction_calc(uint16_t * correction_data, uint16_t raw_data, ctsu_correction_calc_t * p_calc);
#endif
static void ctsu_correction_exec(ctsu_instance_ctrl_t * const p_instance_ctrl);
#if (BSP_FEATURE_CTSU_VERSION == 1)
static void ctsu_correction_ctsu1_exec(ctsu_instance_ctrl_t * const p_instance_ctrl);
#endif
#if (BSP_FEATURE_CTSU_VERSION == 2)
static void ctsu_correction_ctsu2_exec(ctsu_instance_ctrl_t * const p_instance_ctrl);
#endif

#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 0)
static void ctsu_correction_freq(ctsu_correction_multi_t * p_multi, uint16_t * p_pri, uint16_t * p_snd);
 #endif
static void ctsu_correction_multi(ctsu_correction_multi_t * p_multi, uint16_t * p_pri, uint16_t * p_snd);

 #if (CTSU_CFG_TEMP_CORRECTION_SUPPORT == 1)
static void      ctsu_correction_scan_start(void);
static fsp_err_t ctsu_correction_data_get(ctsu_instance_ctrl_t * const p_instance_ctrl, uint16_t * p_data);

  #if (CTSU_CFG_CALIB_RTRIM_SUPPORT == 1)
static fsp_err_t ctsu_correction_calib_rtrim(ctsu_instance_ctrl_t * const p_instance_ctrl, uint16_t * p_data);

  #endif
 #endif
 #if (CTSU_CFG_NUM_CFC != 0)
static void ctsu_corrcfc_process(ctsu_instance_ctrl_t * const p_instance_ctrl);
static void ctsu_corrcfc_measurement(ctsu_instance_ctrl_t * const p_instance_ctrl, uint16_t * data, uint8_t point_num);

 #endif

 #if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 1)
static void ctsu_auto_correction_register_set (ctsu_instance_ctrl_t * const p_instance_ctrl);
 #endif

 #if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
static void ctsu_auto_judge_threshold_calc (ctsu_instance_ctrl_t * const p_instance_ctrl);
 #endif

 #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
static void ctsu_multiclock_auto_correction_register_set (ctsu_instance_ctrl_t * const p_instance_ctrl);
 #endif
#endif



#if (BSP_FEATURE_CTSU_VERSION == 1)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
static void ctsu_diag_dac_initial_tuning(void);

static void ctsu_diag_ldo_over_voltage_scan_start(void);
static void ctsu_diag_oscillator_high_scan_start(void);
static void ctsu_diag_oscillator_low_scan_start(void);
static void ctsu_diag_sscg_scan_start(void);
static void ctsu_diag_dac_scan_start(ctsu_instance_ctrl_t * const p_instance_ctrl);

static void ctsu_diag_ldo_over_voltage_data_get(void);
static void ctsu_diag_oscillator_high_data_get(void);
static void ctsu_diag_oscillator_low_data_get(void);
static void ctsu_diag_sscg_data_get(void);
static void ctsu_diag_dac_data_get(void);

static fsp_err_t ctsu_diag_ldo_over_voltage_result(void);
static fsp_err_t ctsu_diag_oscillator_high_result(void);
static fsp_err_t ctsu_diag_oscillator_low_result(void);
static fsp_err_t ctsu_diag_sscg_result(void);
static fsp_err_t ctsu_diag_dac_result(void);

static void      ctsu_diag_scan_start1(ctsu_instance_ctrl_t * const p_instance_ctrl);
static fsp_err_t ctsu_diag_data_get1(void);

 #endif
#endif

#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)

static void ctsu_diag_regi_store2(void);
static void ctsu_diag_regi_restore2(void);

static fsp_err_t ctsu_diag_output_voltage_scan_start (ctsu_instance_ctrl_t * const p_instance_ctrl);
static fsp_err_t ctsu_diag_output_voltage_result(void);

static void      ctsu_diag_over_voltage_scan_start(void);
static fsp_err_t ctsu_diag_over_voltage_result(void);

static void      ctsu_diag_over_current_scan_start(void);
static fsp_err_t ctsu_diag_over_current_result(void);

static void      ctsu_diag_load_resistance_scan_start(void);
static void      ctsu_diag_load_resistance_data_get(void);
static fsp_err_t ctsu_diag_load_resistance_result(void);

static void      ctsu_diag_current_source_scan_start(void);
static void      ctsu_diag_current_source_data_get(void);
static fsp_err_t ctsu_diag_current_source_result(void);

static void      ctsu_diag_cco_gain_scan_start(void);
static void      ctsu_diag_cco_gain_data_get(void);
static fsp_err_t ctsu_diag_cco_gain_result(void);

static void      ctsu_diag_clock_recovery_scan_start(void);
static void      ctsu_diag_clock_recovery_data_get(void);
static fsp_err_t ctsu_diag_clock_recovery_result(void);

  #if (CTSU_CFG_NUM_CFC != 0)
static fsp_err_t ctsu_diag_cfc_gain_result(void);
static void      ctsu_diag_cfc_gain_scan_start(void);
static void      ctsu_diag_cfc_gain_data_get(void);

  #endif

static fsp_err_t ctsu_diag_scan_start2(ctsu_instance_ctrl_t * const p_instance_ctrl);
static fsp_err_t ctsu_diag_data_get2(uint16_t * p_data);

 #endif
#endif

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

static uint16_t      g_ctsu_element_index = 0;
static uint8_t       g_ctsu_element_complete_flag[CTSU_CFG_NUM_SELF_ELEMENTS + CTSU_CFG_NUM_MUTUAL_ELEMENTS];
#if (BSP_FEATURE_CTSU_VERSION == 2)
static uint8_t       g_ctsu_frequency_complete_flag[CTSU_CFG_NUM_SELF_ELEMENTS + CTSU_CFG_NUM_MUTUAL_ELEMENTS];
#endif
static int32_t       g_ctsu_tuning_diff[CTSU_CFG_NUM_SELF_ELEMENTS + CTSU_CFG_NUM_MUTUAL_ELEMENTS];
static ctsu_ctsuwr_t g_ctsu_ctsuwr[(CTSU_CFG_NUM_SELF_ELEMENTS + CTSU_CFG_NUM_MUTUAL_ELEMENTS +
                                    CTSU_SERIAL_TUNING_ELEMENT) * CTSU_CFG_NUM_SUMULTI];
#if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
static uint16_t        g_ctsu_self_element_index = 0;
static ctsu_self_buf_t g_ctsu_self_raw[CTSU_CFG_NUM_SELF_ELEMENTS * CTSU_CFG_NUM_SUMULTI];
static uint16_t        g_ctsu_self_corr[CTSU_CFG_NUM_SELF_ELEMENTS * CTSU_CFG_NUM_SUMULTI];
static ctsu_data_t     g_ctsu_self_data[CTSU_CFG_NUM_SELF_ELEMENTS * CTSU_MAJORITY_MODE_ELEMENTS];
#endif
#if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
static uint16_t          g_ctsu_mutual_element_index = 0;
static ctsu_mutual_buf_t g_ctsu_mutual_raw[CTSU_CFG_NUM_MUTUAL_ELEMENTS * CTSU_MUTUAL_BUF_SIZE];
static uint16_t          g_ctsu_mutual_pri_corr[CTSU_CFG_NUM_MUTUAL_ELEMENTS * CTSU_CFG_NUM_SUMULTI];
static uint16_t          g_ctsu_mutual_snd_corr[CTSU_CFG_NUM_MUTUAL_ELEMENTS * CTSU_CFG_NUM_SUMULTI];
static ctsu_data_t       g_ctsu_mutual_pri_data[CTSU_CFG_NUM_MUTUAL_ELEMENTS * CTSU_MAJORITY_MODE_ELEMENTS];
static ctsu_data_t       g_ctsu_mutual_snd_data[CTSU_CFG_NUM_MUTUAL_ELEMENTS * CTSU_MAJORITY_MODE_ELEMENTS];
#endif
static ctsu_correction_info_t g_ctsu_correction_info;

#if (CTSU_CFG_DTC_SUPPORT_ENABLE == 1)
static dtc_cmd_arg_t                g_ctsu_dtc_cmd_arg;         /* DTC command argument */
 #if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
  #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
static dtc_transfer_data_cfg_t      g_ctsu_dtc_info[12];         /* table of DTC setting information */
static dtc_transfer_data_t          g_transfer_data_tx[12];
  #else
static dtc_transfer_data_cfg_t      g_ctsu_dtc_info[6];         /* table of DTC setting information */
static dtc_transfer_data_t          g_transfer_data_tx[6];
  #endif
static dtc_transfer_data_t          g_transfer_data_rx[6];
 #else
  #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
static dtc_transfer_data_cfg_t      g_ctsu_dtc_info[2];         /* table of DTC setting information */
static dtc_transfer_data_t          g_transfer_data_tx[2];
  #else
static dtc_transfer_data_cfg_t      g_ctsu_dtc_info[1];         /* table of DTC setting information */
static dtc_transfer_data_t          g_transfer_data_tx[1];
  #endif
static dtc_transfer_data_t          g_transfer_data_rx[1];
 #endif
#endif

#if (BSP_FEATURE_CTSU_VERSION == 1)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
static ctsu_diag_info_t     g_ctsu_diag_info;
static ctsu_diag_save_reg_t g_ctsu_diag_reg;
 #endif
#endif

#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_NUM_CFC != 0)
static ctsu_corrcfc_info_t g_ctsu_corrcfc_info;
 #endif

 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
static ctsu_diag_info_t     g_ctsu_diag_info;
static ctsu_diag_save_reg_t g_ctsu_diag_reg;
 #endif
 #if (CTSU_CFG_TEMP_CORRECTION_SUPPORT == 1)
static uint32_t g_ctsu_temp_reg_ctsucra;
 #endif
 #if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
static ctsu_auto_judge_t g_ctsu_auto_judge[(CTSU_CFG_NUM_SELF_ELEMENTS + CTSU_CFG_NUM_MUTUAL_ELEMENTS) * CTSU_MAJORITY_MODE_ELEMENTS];
 #endif
 #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
static uint32_t g_ctsu_mcact1[(CTSU_CFG_NUM_SELF_ELEMENTS + CTSU_CFG_NUM_MUTUAL_ELEMENTS) * CTSU_CFG_NUM_SUMULTI];
static uint32_t g_ctsu_mcact2[(CTSU_CFG_NUM_SELF_ELEMENTS + CTSU_CFG_NUM_MUTUAL_ELEMENTS) * CTSU_CFG_NUM_SUMULTI];
 #endif

 #if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
uint8_t g_ctsu_selected_freq_self[CTSU_CFG_NUM_SELF_ELEMENTS];
 #endif
 #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
uint8_t g_ctsu_selected_freq_mutual[CTSU_CFG_NUM_MUTUAL_ELEMENTS];
 #endif

#endif


#if (BSP_FEATURE_CTSU_VERSION == 1)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
static const uint16_t dac_oscil_table[6][2] =
{
    {CTSU_CFG_DIAG_DAC1_MAX, CTSU_CFG_DIAG_DAC1_MIN},
    {CTSU_CFG_DIAG_DAC2_MAX, CTSU_CFG_DIAG_DAC2_MIN},
    {CTSU_CFG_DIAG_DAC3_MAX, CTSU_CFG_DIAG_DAC3_MIN},
    {CTSU_CFG_DIAG_DAC4_MAX, CTSU_CFG_DIAG_DAC4_MIN},
    {CTSU_CFG_DIAG_DAC5_MAX, CTSU_CFG_DIAG_DAC5_MIN},
    {CTSU_CFG_DIAG_DAC6_MAX, CTSU_CFG_DIAG_DAC6_MIN},
};
 #endif
#endif

#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
static const uint16_t cco_gain_table[12][2] =
{
    {CTSU_CFG_DIAG_DAC1_MIN,  CTSU_CFG_DIAG_DAC1_MAX   },
    {CTSU_CFG_DIAG_DAC2_MIN,  CTSU_CFG_DIAG_DAC2_MAX   },
    {CTSU_CFG_DIAG_DAC3_MIN,  CTSU_CFG_DIAG_DAC3_MAX   },
    {CTSU_CFG_DIAG_DAC4_MIN,  CTSU_CFG_DIAG_DAC4_MAX   },
    {CTSU_CFG_DIAG_DAC5_MIN,  CTSU_CFG_DIAG_DAC5_MAX   },
    {CTSU_CFG_DIAG_DAC6_MIN,  CTSU_CFG_DIAG_DAC6_MAX   },
    {CTSU_CFG_DIAG_DAC7_MIN,  CTSU_CFG_DIAG_DAC7_MAX   },
    {CTSU_CFG_DIAG_DAC8_MIN,  CTSU_CFG_DIAG_DAC8_MAX   },
    {CTSU_CFG_DIAG_DAC9_MIN,  CTSU_CFG_DIAG_DAC9_MAX   },
    {CTSU_CFG_DIAG_DAC10_MIN, CTSU_CFG_DIAG_DAC10_MAX  },
    {CTSU_CFG_DIAG_DAC11_MIN, CTSU_CFG_DIAG_DAC11_MAX  },
    {CTSU_CFG_DIAG_DAC12_MIN, CTSU_CFG_DIAG_DAC12_MAX  },
};

static const uint16_t cco_gain_diff_table[11][2] =
{
    {CTSU_CFG_DIAG_DAC1_2_DIFF_MIN,   CTSU_CFG_DIAG_DAC1_2_DIFF_MAX      },
    {CTSU_CFG_DIAG_DAC2_3_DIFF_MIN,   CTSU_CFG_DIAG_DAC2_3_DIFF_MAX      },
    {CTSU_CFG_DIAG_DAC3_4_DIFF_MIN,   CTSU_CFG_DIAG_DAC3_4_DIFF_MAX      },
    {CTSU_CFG_DIAG_DAC4_5_DIFF_MIN,   CTSU_CFG_DIAG_DAC4_5_DIFF_MAX      },
    {CTSU_CFG_DIAG_DAC5_6_DIFF_MIN,   CTSU_CFG_DIAG_DAC5_6_DIFF_MAX      },
    {CTSU_CFG_DIAG_DAC6_7_DIFF_MIN,   CTSU_CFG_DIAG_DAC6_7_DIFF_MAX      },
    {CTSU_CFG_DIAG_DAC7_8_DIFF_MIN,   CTSU_CFG_DIAG_DAC7_8_DIFF_MAX      },
    {CTSU_CFG_DIAG_DAC8_9_DIFF_MIN,   CTSU_CFG_DIAG_DAC8_9_DIFF_MAX      },
    {CTSU_CFG_DIAG_DAC9_10_DIFF_MIN,  CTSU_CFG_DIAG_DAC9_10_DIFF_MAX     },
    {CTSU_CFG_DIAG_DAC10_11_DIFF_MIN, CTSU_CFG_DIAG_DAC10_11_DIFF_MAX    },
    {CTSU_CFG_DIAG_DAC11_12_DIFF_MIN, CTSU_CFG_DIAG_DAC11_12_DIFF_MAX    },
};
 #endif
#endif

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
const ctsu_api_t g_ctsu_on_ctsu =
{
    .open            = R_CTSU_Open,
    .scanStart       = R_CTSU_ScanStart,
    .dataGet         = R_CTSU_DataGet,
    .scanStop        = R_CTSU_ScanStop,
    .diagnosis       = R_CTSU_Diagnosis,
    .close           = R_CTSU_Close,
    .callbackSet     = R_CTSU_CallbackSet,
    .specificDataGet = R_CTSU_SpecificDataGet,
    .dataInsert      = R_CTSU_DataInsert,
    .offsetTuning    = R_CTSU_OffsetTuning,
};

ctsu_instance_ctrl_t * gp_ctsu_isr_context;

/*******************************************************************************************************************//**
 * @addtogroup CTSU
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Opens and configures the CTSU driver module. Implements @ref ctsu_api_t::open.
 *
 * Example:
 * @snippet r_ctsu_example.c R_CTSU_Open
 *
 * @retval FSP_SUCCESS              CTSU successfully configured.
 * @retval FSP_ERR_ASSERTION        Null pointer, or one or more configuration options is invalid.
 * @retval FSP_ERR_ALREADY_OPEN     Module is already open.  This module can only be opened once.
 * @retval FSP_ERR_INVALID_ARGUMENT Configuration parameter error.
 *
 * @note In the first Open, measurement for correction works, and it takes several tens of milliseconds.
 **********************************************************************************************************************/
fsp_err_t R_CTSU_Open (ctsu_ctrl_t * const p_ctrl, ctsu_cfg_t const * const p_cfg)
{
    ctsu_instance_ctrl_t     * p_instance_ctrl = (ctsu_instance_ctrl_t *) p_ctrl;
    fsp_err_t                  err             = FSP_SUCCESS;
    uint16_t                   element_id;
    const ctsu_element_cfg_t * element_cfgs;
#if (BSP_FEATURE_CTSU_VERSION == 2)
    uint16_t i;
    uint32_t pclkb_mhz;
    uint16_t suadj[3];
#endif

#if (CTSU_CFG_PARAM_CHECKING_ENABLE == 1)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_cfg);
#endif
    FSP_ERROR_RETURN(CTSU_OPEN != p_instance_ctrl->open, FSP_ERR_ALREADY_OPEN);

    /* Check element number */
#if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
    if (CTSU_MODE_SELF_MULTI_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_cfg->md))
    {
        FSP_ERROR_RETURN(CTSU_CFG_NUM_SELF_ELEMENTS >=
                         (uint8_t) (g_ctsu_self_element_index + p_cfg->num_rx),
                         FSP_ERR_INVALID_ARGUMENT);
    }
#endif
#if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
    if (CTSU_MODE_MUTUAL_FULL_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_cfg->md))
    {
        FSP_ERROR_RETURN(CTSU_CFG_NUM_MUTUAL_ELEMENTS >=
                         (uint8_t) (g_ctsu_mutual_element_index + (p_cfg->num_rx * p_cfg->num_tx)),
                         FSP_ERR_INVALID_ARGUMENT);
    }
#endif
    p_instance_ctrl->state = CTSU_STATE_INIT;

    /* Save configurations. */
    p_instance_ctrl->p_ctsu_cfg = p_cfg;

    /* Initialize driver control structure (address setting) */
#if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
    if (CTSU_MODE_SELF_MULTI_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_cfg->md))
    {
        p_instance_ctrl->p_self_raw   = &g_ctsu_self_raw[g_ctsu_self_element_index * CTSU_CFG_NUM_SUMULTI];
        p_instance_ctrl->p_self_corr  = &g_ctsu_self_corr[g_ctsu_self_element_index * CTSU_CFG_NUM_SUMULTI];
        p_instance_ctrl->p_self_data  = &g_ctsu_self_data[g_ctsu_self_element_index * CTSU_MAJORITY_MODE_ELEMENTS];
        p_instance_ctrl->num_elements = p_cfg->num_rx;
 #if (BSP_FEATURE_CTSU_VERSION == 2)
        p_instance_ctrl->p_selected_freq_self = &g_ctsu_selected_freq_self[g_ctsu_self_element_index];
 #endif
        g_ctsu_self_element_index        = (uint8_t) (g_ctsu_self_element_index + p_instance_ctrl->num_elements);
        p_instance_ctrl->self_elem_index = g_ctsu_self_element_index;

 #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
        p_instance_ctrl->p_mutual_raw      = &g_ctsu_mutual_raw[0];
        p_instance_ctrl->p_mutual_pri_corr = &g_ctsu_mutual_pri_corr[0];
        p_instance_ctrl->p_mutual_snd_corr = &g_ctsu_mutual_snd_corr[0];
        p_instance_ctrl->p_mutual_pri_data = &g_ctsu_mutual_pri_data[0];
        p_instance_ctrl->p_mutual_snd_data = &g_ctsu_mutual_snd_data[0];
 #endif
    }
#endif
#if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
    if (CTSU_MODE_MUTUAL_FULL_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_cfg->md))
    {
        p_instance_ctrl->p_mutual_raw      = &g_ctsu_mutual_raw[g_ctsu_mutual_element_index * CTSU_MUTUAL_BUF_SIZE];
        p_instance_ctrl->p_mutual_pri_corr =
            &g_ctsu_mutual_pri_corr[g_ctsu_mutual_element_index * CTSU_CFG_NUM_SUMULTI];
        p_instance_ctrl->p_mutual_snd_corr =
            &g_ctsu_mutual_snd_corr[g_ctsu_mutual_element_index * CTSU_CFG_NUM_SUMULTI];
        p_instance_ctrl->p_mutual_pri_data =
            &g_ctsu_mutual_pri_data[g_ctsu_mutual_element_index * CTSU_MAJORITY_MODE_ELEMENTS];
        p_instance_ctrl->p_mutual_snd_data =
            &g_ctsu_mutual_snd_data[g_ctsu_mutual_element_index * CTSU_MAJORITY_MODE_ELEMENTS];
        p_instance_ctrl->num_elements = (uint8_t) (p_cfg->num_rx * p_cfg->num_tx);
 #if (BSP_FEATURE_CTSU_VERSION == 2)
        p_instance_ctrl->p_selected_freq_mutual = &g_ctsu_selected_freq_mutual[g_ctsu_mutual_element_index * 2];
 #endif
        g_ctsu_mutual_element_index        = (uint8_t) (g_ctsu_mutual_element_index + p_instance_ctrl->num_elements);
        p_instance_ctrl->mutual_elem_index = g_ctsu_mutual_element_index;

 #if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
        p_instance_ctrl->p_self_raw  = &g_ctsu_self_raw[0];
        p_instance_ctrl->p_self_corr = &g_ctsu_self_corr[0];
        p_instance_ctrl->p_self_data = &g_ctsu_self_data[0];
 #endif
    }
#endif

    p_instance_ctrl->p_element_complete_flag = &g_ctsu_element_complete_flag[g_ctsu_element_index];
#if (BSP_FEATURE_CTSU_VERSION == 2)
    p_instance_ctrl->p_frequency_complete_flag = &g_ctsu_frequency_complete_flag[g_ctsu_element_index];
#endif
    p_instance_ctrl->p_tuning_diff  = &g_ctsu_tuning_diff[g_ctsu_element_index];
    p_instance_ctrl->p_ctsuwr       = &g_ctsu_ctsuwr[g_ctsu_element_index * CTSU_CFG_NUM_SUMULTI];
#if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
    /* Secure DTC transfer area for automatic judgement, store start address and transfer count */
    p_instance_ctrl->p_auto_judge             = &g_ctsu_auto_judge[g_ctsu_element_index * CTSU_MAJORITY_MODE_ELEMENTS];
    p_instance_ctrl->adress_auto_judge        = (uint32_t)p_instance_ctrl->p_auto_judge;
    p_instance_ctrl->adress_ctsuwr            = (uint32_t)p_instance_ctrl->p_ctsuwr;
    p_instance_ctrl->adress_self_raw          = (uint32_t)p_instance_ctrl->p_self_raw;
 #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
    p_instance_ctrl->adress_mutual_raw        = (uint32_t)p_instance_ctrl->p_mutual_raw;
 #endif
 #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
    if (1 == p_instance_ctrl->p_ctsu_cfg->majirimd)
    {
        p_instance_ctrl->count_auto_judge         = (uint32_t)((CTSU_AUTO_JUDGE_BLOCK_SIZE << 24) | (CTSU_AUTO_JUDGE_BLOCK_SIZE << 16) | (p_instance_ctrl->num_elements));
        p_instance_ctrl->count_ctsuwr_self_mutual = (uint32_t)((CTSU_AUTO_CTSUSO_BLOCK_SIZE << 24) | (CTSU_AUTO_CTSUSO_BLOCK_SIZE << 16) | (p_instance_ctrl->num_elements));
        p_instance_ctrl->adress_mcact1            = (uint32_t)p_instance_ctrl->p_mcact1;
        p_instance_ctrl->count_mcact1             = (uint32_t)((CTSU_AUTO_MCACT_BLOCK_SIZE << 24) | (CTSU_AUTO_MCACT_BLOCK_SIZE << 16) | (p_instance_ctrl->num_elements));
        p_instance_ctrl->adress_mcact2            = (uint32_t)p_instance_ctrl->p_mcact2;
        p_instance_ctrl->count_mcact2             = (uint32_t)((CTSU_AUTO_MCACT_BLOCK_SIZE << 24) | (CTSU_AUTO_MCACT_BLOCK_SIZE << 16) | (p_instance_ctrl->num_elements));
    }
    else
 #endif
    {
        p_instance_ctrl->count_auto_judge         = (uint32_t)((CTSU_AUTO_JUDGE_BLOCK_SIZE << 24) | (CTSU_AUTO_JUDGE_BLOCK_SIZE << 16) | (p_instance_ctrl->num_elements * CTSU_CFG_NUM_SUMULTI));
        p_instance_ctrl->count_ctsuwr_self_mutual = (uint32_t)((CTSU_AUTO_CTSUSO_BLOCK_SIZE << 24) | (CTSU_AUTO_CTSUSO_BLOCK_SIZE << 16) | (p_instance_ctrl->num_elements * CTSU_CFG_NUM_SUMULTI));
    }

    /* Copy to ctrl variable for setting change for automatic judgement */
    p_instance_ctrl->ajmmat                   = p_cfg->ajmmat;
    p_instance_ctrl->ajbmat                   = p_cfg->ajbmat;
#endif
#if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
    p_instance_ctrl->p_mcact1   = &g_ctsu_mcact1[g_ctsu_element_index * CTSU_CFG_NUM_SUMULTI];
    p_instance_ctrl->p_mcact2   = &g_ctsu_mcact2[g_ctsu_element_index * CTSU_CFG_NUM_SUMULTI];
    p_instance_ctrl->mcact_flag = 0;
#endif
    g_ctsu_element_index            = (uint8_t) (g_ctsu_element_index + p_instance_ctrl->num_elements);
    p_instance_ctrl->ctsu_elem_index = g_ctsu_element_index;

    /* Set Value */
    p_instance_ctrl->cap                = p_cfg->cap;
    p_instance_ctrl->num_moving_average = p_cfg->num_moving_average;
    p_instance_ctrl->average            = 0;
    if (true == p_cfg->tunning_enable)
    {
        p_instance_ctrl->tuning = CTSU_TUNING_INCOMPLETE;
    }
    else
    {
        p_instance_ctrl->tuning = CTSU_TUNING_COMPLETE;
    }

#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_NUM_CFC != 0)
    if (CTSU_MODE_MUTUAL_CFC_SCAN == p_cfg->md)
    {
        p_instance_ctrl->tuning = CTSU_TUNING_COMPLETE;
    }
 #endif

    if (CTSU_MODE_CURRENT_SCAN == p_cfg->md)
    {
        p_instance_ctrl->tuning = CTSU_TUNING_COMPLETE;
    }
#endif

#if (BSP_FEATURE_CTSU_VERSION == 1)
    p_instance_ctrl->ctsucr1 = (uint8_t) (p_cfg->atune1 << 3);
#endif
#if (BSP_FEATURE_CTSU_VERSION == 2)
    p_instance_ctrl->ctsucr1 = (uint8_t) ((p_cfg->atune12 & 0x01) << 3);
#endif
    p_instance_ctrl->ctsucr1 |= (uint8_t) (p_cfg->md << 6);

    p_instance_ctrl->txvsel  = p_instance_ctrl->p_ctsu_cfg->txvsel;
    p_instance_ctrl->txvsel2 = p_instance_ctrl->p_ctsu_cfg->txvsel2;

    p_instance_ctrl->ctsuchac0 = p_instance_ctrl->p_ctsu_cfg->ctsuchac0;
    p_instance_ctrl->ctsuchac1 = p_instance_ctrl->p_ctsu_cfg->ctsuchac1;
    p_instance_ctrl->ctsuchac2 = p_instance_ctrl->p_ctsu_cfg->ctsuchac2;
    p_instance_ctrl->ctsuchac3 = p_instance_ctrl->p_ctsu_cfg->ctsuchac3;
    p_instance_ctrl->ctsuchac4 = p_instance_ctrl->p_ctsu_cfg->ctsuchac4;

    p_instance_ctrl->ctsuchtrc0 = p_instance_ctrl->p_ctsu_cfg->ctsuchtrc0;
    p_instance_ctrl->ctsuchtrc1 = p_instance_ctrl->p_ctsu_cfg->ctsuchtrc1;
    p_instance_ctrl->ctsuchtrc2 = p_instance_ctrl->p_ctsu_cfg->ctsuchtrc2;
    p_instance_ctrl->ctsuchtrc3 = p_instance_ctrl->p_ctsu_cfg->ctsuchtrc3;
    p_instance_ctrl->ctsuchtrc4 = p_instance_ctrl->p_ctsu_cfg->ctsuchtrc4;

    p_instance_ctrl->md = p_instance_ctrl->p_ctsu_cfg->md;

#if (BSP_FEATURE_CTSU_VERSION == 2)
    p_instance_ctrl->ctsucr2  = (uint8_t) (p_cfg->atune12 & 0x02);
    p_instance_ctrl->ctsucr2 |= (uint8_t) ((p_cfg->md & 0x04) >> 2);
    p_instance_ctrl->ctsucr2 |= (uint8_t) (p_cfg->posel << 4);

    if (CTSU_ATUNE12_80UA == p_cfg->atune12)
    {
        p_instance_ctrl->range = CTSU_RANGE_80UA;
    }
    else if (CTSU_ATUNE12_40UA == p_cfg->atune12)
    {
        p_instance_ctrl->range = CTSU_RANGE_40UA;
    }
    else if (CTSU_ATUNE12_20UA == p_cfg->atune12)
    {
        p_instance_ctrl->range = CTSU_RANGE_20UA;
    }
    else if (CTSU_ATUNE12_160UA == p_cfg->atune12)
    {
        p_instance_ctrl->range = CTSU_RANGE_160UA;
    }
    else
    {
    }

 #if (CTSU_CFG_MULTIPLE_ELECTRODE_CONNECTION_ENABLE == 1)
    /* Copy the variables used for the MEC function to the ctrl instance */
    p_instance_ctrl->tsod = p_instance_ctrl->p_ctsu_cfg->tsod;
    p_instance_ctrl->mec_ts = p_instance_ctrl->p_ctsu_cfg->mec_ts;
    p_instance_ctrl->mec_shield_ts = p_instance_ctrl->p_ctsu_cfg->mec_shield_ts;
 #endif
#endif

    for (element_id = 0; element_id < p_instance_ctrl->num_elements; element_id++)
    {
        p_instance_ctrl->p_element_complete_flag[element_id] = 0;
#if (BSP_FEATURE_CTSU_VERSION == 2)
        p_instance_ctrl->p_frequency_complete_flag[element_id] = 0;
#endif
        p_instance_ctrl->p_tuning_diff[element_id]  = 0;
        element_cfgs = (p_cfg->p_elements + element_id);
#if (BSP_FEATURE_CTSU_VERSION == 2)
        if (CTSU_MODE_CURRENT_SCAN == p_cfg->md)
        {
            /* Current scan does not run multiple frequency */
            p_instance_ctrl->p_ctsuwr[element_id].ctsuso =
                (uint32_t) (((uint32_t) element_cfgs->sdpa << (uint32_t) 24) |
                            ((uint32_t) element_cfgs->snum << (uint32_t) 10) | element_cfgs->so);
        }
        else
        {
            if (CTSU_MODE_MUTUAL_CFC_SCAN != p_cfg->md)
            {
                p_instance_ctrl->p_ctsuwr[element_id * CTSU_CFG_NUM_SUMULTI].ctsuso =
                    (uint32_t) (((uint32_t) element_cfgs->sdpa << (uint32_t) 24) |
                                ((uint32_t) element_cfgs->snum << (uint32_t) 10) | element_cfgs->so);
            }

 #if (CTSU_CFG_NUM_CFC != 0)
            else
            {
                /* CFC scan does not use CTSUSO */
                p_instance_ctrl->p_ctsuwr[element_id * CTSU_CFG_NUM_SUMULTI].ctsuso =
                    (uint32_t) (((uint32_t) element_cfgs->sdpa << (uint32_t) 24) |
                                ((uint32_t) element_cfgs->snum << (uint32_t) 10));
            }
 #endif

            for (i = 1; i < CTSU_CFG_NUM_SUMULTI; i++)
            {
                p_instance_ctrl->p_ctsuwr[(element_id * CTSU_CFG_NUM_SUMULTI) + i].ctsuso =
                    p_instance_ctrl->p_ctsuwr[element_id * CTSU_CFG_NUM_SUMULTI].ctsuso;
            }
        }
#endif
#if (BSP_FEATURE_CTSU_VERSION == 1)
        p_instance_ctrl->p_ctsuwr[element_id].ctsussc = (uint16_t) (element_cfgs->ssdiv << 8);
        p_instance_ctrl->p_ctsuwr[element_id].ctsuso0 = (uint16_t) ((element_cfgs->snum << 10) | element_cfgs->so);
        p_instance_ctrl->p_ctsuwr[element_id].ctsuso1 =
            (uint16_t) ((CTSU_ICOG_RECOMMEND << 13) | (element_cfgs->sdpa << 8) | CTSU_RICOA_RECOMMEND);
#endif
    }

    /* Enable interrupts for write, read, & scan done */
    IR(CTSU,CTSUWR)= 0;
    IR(CTSU,CTSURD)= 0;
    IR(CTSU,CTSUFN)= 0;
    IPR(CTSU,CTSUWR)= CTSU_CFG_INTCTSUWR_PRIORITY_LEVEL;
    IPR(CTSU,CTSURD)= CTSU_CFG_INTCTSURD_PRIORITY_LEVEL;
    IPR(CTSU,CTSUFN)= CTSU_CFG_INTCTSUFN_PRIORITY_LEVEL;
    R_BSP_InterruptRequestEnable(VECT(CTSU,CTSUWR));
    R_BSP_InterruptRequestEnable(VECT(CTSU,CTSURD));
    R_BSP_InterruptRequestEnable(VECT(CTSU,CTSUFN));

    gp_ctsu_isr_context = p_instance_ctrl;
    
    p_instance_ctrl->interrupt_reverse_flag = 0;

    if (p_instance_ctrl->num_elements == g_ctsu_element_index)
    {
        /* TSCAP discharge process */
        R_Set_CTSU_TSCAP_Discharge();
        R_BSP_SoftwareDelay(30,BSP_DELAY_MICROSECS);
        R_Set_CTSU_TSCAP_Charge();

        /* Get CTSU out of stop state (supply power/clock) */
        R_BSP_RegisterProtectDisable (BSP_REG_PROTECT_LPC_CGC_SWR);
        MSTP(CTSU) = 0;
        R_BSP_RegisterProtectEnable (BSP_REG_PROTECT_LPC_CGC_SWR);

#if (CTSU_CFG_DTC_SUPPORT_ENABLE == 1)
        err = ctsu_transfer_open(p_instance_ctrl);
#endif
#if (BSP_FEATURE_CTSU_VERSION == 2)
        CTSU.CTSUCRA.BIT.ATUNE0 = CTSU_CFG_LOW_VOLTAGE_MODE;
        CTSU.CTSUCRA.BIT.PUMPON = 1;

        pclkb_mhz = BSP_PCLKB_HZ / CTSU_PCLKB_FREQ_MHZ;

        if (CTSU_PCLKB_FREQ_RANGE1 >= pclkb_mhz)
        {
            CTSU.CTSUCRA.BIT.CLK   = 0;
            CTSU.CTSUCRA.BIT.STCLK = (uint8_t) (pclkb_mhz - 1);
        }
        else if ((CTSU_PCLKB_FREQ_RANGE1 < pclkb_mhz) && (CTSU_PCLKB_FREQ_RANGE2 >= pclkb_mhz))
        {
            CTSU.CTSUCRA.BIT.CLK   = 1;
            CTSU.CTSUCRA.BIT.STCLK = (uint8_t) ((pclkb_mhz >> 1) - 1);
        }
        else if ((CTSU_PCLKB_FREQ_RANGE2 < pclkb_mhz) && (CTSU_PCLKB_FREQ_RANGE3 >= pclkb_mhz))
        {
            CTSU.CTSUCRA.BIT.CLK   = 2;
            CTSU.CTSUCRA.BIT.STCLK = (uint8_t) ((pclkb_mhz >> 2) - 1);
        }
        else
        {
            CTSU.CTSUCRA.BIT.CLK   = 3;
            CTSU.CTSUCRA.BIT.STCLK = (uint8_t) ((pclkb_mhz >> 3) - 1);
        }
#endif
#if (BSP_FEATURE_CTSU_VERSION == 1)

        /* Set power on */
        CTSU.CTSUCR1.BYTE = ((CTSU_CFG_LOW_VOLTAGE_MODE << 2) | (CTSU_CSW_ON << 1) | CTSU_PON_ON);

        /* Synchronous Noise Reduction Setting */
        CTSU.CTSUSDPRS.BYTE = ((CTSU_SOFF_ON << 6) | (CTSU_PRMODE_62_PULSES << 4) | CTSU_PRRATIO_RECOMMEND);

        /* High Pass Noise Reduction- ALWAYS 0x30 as per HW Manual */
        CTSU.CTSUDCLKC.BYTE = ((CTSU_SSCNT << 4) | CTSU_SSMOD);

        /* Sensor Stabilization- ALWAYS 0x10 as per HW Manual */
        CTSU.CTSUSST.BYTE = (CTSU_SST_RECOMMEND);

        if (CTSU_CORRECTION_INIT == g_ctsu_correction_info.status)
        {
            ctsu_correction_process(p_instance_ctrl);
        }

        /* Since CLK is rewritten by correction, set here. */
        CTSU.CTSUCR1.BYTE |= (uint8_t) (CTSU_CFG_PCLK_DIVISION << 4);
#endif
    }

#if (BSP_FEATURE_CTSU_VERSION == 2)
    if (CTSU_MODE_MUTUAL_CFC_SCAN != p_cfg->md)
    {
        /* Set power on */
        CTSU.CTSUCRA.BIT.CSW = CTSU_CSW_ON;
        CTSU.CTSUCRA.BIT.PON = CTSU_PON_ON;
        R_BSP_SoftwareDelay(30, BSP_DELAY_MICROSECS);
    }

 #if (CTSU_CFG_NUM_CFC != 0)
    else
    {
        R_CTSU->CTSUCALIB_b.CFCRDMD = 1;
        R_CTSU->CTSUCRA_b.CFCON     = 1;
        R_BSP_SoftwareDelay(30, BSP_DELAY_UNITS_MICROSECONDS);
    }
 #endif

    /* High resolution pulse mode  */
    CTSU.CTSUCRA.BIT.SDPSEL    = 1;
    CTSU.CTSUCRA.BIT.PCSEL     = 1;
    CTSU.CTSUCRB.BIT.SST       = CTSU_SST_RECOMMEND;
    CTSU.CTSUCALIB.BIT.CCOCLK  = 0;
    CTSU.CTSUCALIB.BIT.SUCLKEN = 0;

    if (CTSU_MODE_MUTUAL_CFC_SCAN != p_cfg->md)
    {
 #if (CTSU_CFG_TEMP_CORRECTION_SUPPORT == 1)
        if (CTSU_MODE_CORRECTION_SCAN == p_cfg->md)
        {
            g_ctsu_correction_info.scan_index = CTSU_CORRECTION_POINT_NUM;
        }
 #endif
        if (CTSU_CORRECTION_INIT == g_ctsu_correction_info.status)
        {
            ctsu_correction_process(p_instance_ctrl);
        }
    }

 #if (CTSU_CFG_NUM_CFC != 0)
    else
    {
        ctsu_corrcfc_process(p_instance_ctrl);
    }
    p_instance_ctrl->p_corrcfc_info = &g_ctsu_corrcfc_info;
 #endif

    CTSU.CTSUCRA.BIT.LOAD      = 1;
    CTSU.CTSUCRB.BIT.SSCNT      = 1;
    CTSU.CTSUCALIB.BIT.SUCARRY  = 0;
    CTSU.CTSUCALIB.BIT.CCOCALIB = 0;
    CTSU.CTSUCALIB.BIT.CCOCLK   = 1;
    CTSU.CTSUCALIB.BIT.TSOC     = 0;
    CTSU.CTSUCALIB.BIT.SUCLKEN  = 1;

    /* Read SUADJD byte */
    suadj[0] = (uint16_t) ((CTSU.CTSUTRIMA.BIT.SUADJD * (CTSU_CFG_SUMULTI0 + 1)) / (CTSU_SUMULTI_BASE + 1));

    /* Adjust multi freq */
    suadj[1] = (uint16_t) ((suadj[0] * (CTSU_CFG_SUMULTI1 + 1)) / (CTSU_CFG_SUMULTI0 + 1));
    suadj[2] = (uint16_t) ((suadj[0] * (CTSU_CFG_SUMULTI2 + 1)) / (CTSU_CFG_SUMULTI0 + 1));

    /* Adjust SSCNT setting */
    suadj[0] = (uint16_t) (suadj[0] - (CTSU_SUADJ_SSCNT_ADJ * CTSU.CTSUCRB.BIT.SSCNT));
    suadj[1] = (uint16_t) (suadj[1] - (CTSU_SUADJ_SSCNT_ADJ * CTSU.CTSUCRB.BIT.SSCNT));
    suadj[2] = (uint16_t) (suadj[2] - (CTSU_SUADJ_SSCNT_ADJ * CTSU.CTSUCRB.BIT.SSCNT));

    /* Set CTSUSUCLK register */
    CTSU.CTSUCRA.BIT.SDPSEL = 0;
    CTSU.CTSUSUCLKA.BIT.SUADJ0   = (uint8_t) suadj[0];
    CTSU.CTSUSUCLKA.BIT.SUMULTI0 = CTSU_CFG_SUMULTI0;

    CTSU.CTSUSUCLKA.BIT.SUADJ1   = (uint8_t) suadj[1];
    CTSU.CTSUSUCLKA.BIT.SUMULTI1 = CTSU_CFG_SUMULTI1;

    CTSU.CTSUSUCLKB.BIT.SUADJ2   = (uint8_t) suadj[2];
    CTSU.CTSUSUCLKB.BIT.SUMULTI2 = CTSU_CFG_SUMULTI2;

    CTSU.CTSUCRA.BIT.SDPSEL = 1;

 #if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
    if (1 == p_instance_ctrl->p_ctsu_cfg->ajfen)
    {
        /* Fixed parameter setting for automatic judgement */
        CTSU.CTSUAJCR.BIT.TLOT   = p_instance_ctrl->p_ctsu_cfg->tlot;
        CTSU.CTSUAJCR.BIT.THOT   = p_instance_ctrl->p_ctsu_cfg->thot;

        p_instance_ctrl->blini_flag = 1;
    }
 #endif
#endif

#if (BSP_FEATURE_CTSU_VERSION == 1)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
    if (CTSU_MODE_DIAGNOSIS_SCAN == p_instance_ctrl->md)
    {
       /* Initialize diagnosis information */
        p_instance_ctrl->p_diag_info    = &g_ctsu_diag_info;
        g_ctsu_diag_info.state          = CTSU_DIAG_INIT;
        g_ctsu_diag_info.ctsuwr.ctsussc = 0;
        g_ctsu_diag_info.ctsuwr.ctsuso0 = 0;
        g_ctsu_diag_info.ctsuwr.ctsuso1 = 0;
        g_ctsu_diag_info.loop_count     = 0;
        g_ctsu_diag_info.scanbuf.sen    = 0;
        g_ctsu_diag_info.scanbuf.ref    = 0;
        g_ctsu_diag_info.correct_data   = 0;
        g_ctsu_diag_info.icomp          = 0;
        g_ctsu_diag_info.cco_high       = 0;
        g_ctsu_diag_info.cco_low        = 0;
        g_ctsu_diag_info.sscg           = 0;
        g_ctsu_diag_info.dac_cnt[0]     = 0;
        g_ctsu_diag_info.dac_cnt[1]     = 0;
        g_ctsu_diag_info.dac_cnt[2]     = 0;
        g_ctsu_diag_info.dac_cnt[3]     = 0;
        g_ctsu_diag_info.dac_cnt[4]     = 0;
        g_ctsu_diag_info.dac_cnt[5]     = 0;
        g_ctsu_diag_info.so0_4uc_val    = 0;
        g_ctsu_diag_info.dac_init       = 0;
        g_ctsu_diag_info.tuning         = CTSU_TUNING_INCOMPLETE;
        g_ctsu_diag_info.tuning_diff    = 0;
    }
 #endif
#endif

#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
    if (CTSU_MODE_DIAGNOSIS_SCAN == p_instance_ctrl->md)
    {
        p_instance_ctrl->p_diag_info = &g_ctsu_diag_info;
        g_ctsu_diag_info.lvmode      = CTSU_CFG_LOW_VOLTAGE_MODE;
        g_ctsu_diag_info.state       = CTSU_DIAG_INIT;
    }
    else
    {
        /* Once chac is set, it will not be set after that */
        if ((0 == g_ctsu_diag_info.chaca) && (0 == g_ctsu_diag_info.chacb))
        {
            /* From the CHAC information configured in the normal measurement,                                  */
            /* find the minimum TS pin on the CTSU peripheral for the OverCuurent test and clock recovery test. */

            /* Get CHACA register info */
            ((uint8_t *) &(g_ctsu_diag_info.chaca))[0] = p_instance_ctrl->p_ctsu_cfg->ctsuchac0;
            ((uint8_t *) &(g_ctsu_diag_info.chaca))[1] = p_instance_ctrl->p_ctsu_cfg->ctsuchac1;
            ((uint8_t *) &(g_ctsu_diag_info.chaca))[2] = p_instance_ctrl->p_ctsu_cfg->ctsuchac2;
            ((uint8_t *) &(g_ctsu_diag_info.chaca))[3] = p_instance_ctrl->p_ctsu_cfg->ctsuchac3;

            /* Get CHACB register info */
            g_ctsu_diag_info.chacb = (uint32_t) (p_instance_ctrl->p_ctsu_cfg->ctsuchac4);

            /* Get the number of measurable elements from enabled CHACA */
            if (0 != g_ctsu_diag_info.chaca)
            {
                /* Get the number of measurable elements from enabled CHACA */
                for (i = 0; i < CTSU_DIAG_CHACA_TSMAX; i++)
                {
                    if ((g_ctsu_diag_info.chaca >> i) & (uint32_t) 0x00000001)
                    {
                        g_ctsu_diag_info.chaca = (uint32_t) (0x00000001 << i);
                        g_ctsu_diag_info.chacb = 0;
                        break;
                    }
                }
            }
            else                       /* Check the measurable elements from enabled CHACB. */
            {
                for (i = 0; i < 8; i++)
                {
                    /* Get the number of measurable elements from enabled CHACB */
                    if ((g_ctsu_diag_info.chacb >> i) & (uint32_t) 0x00000001)
                    {
                        g_ctsu_diag_info.chaca = 0;
                        g_ctsu_diag_info.chacb = (uint32_t) (0x00000001 << i);
                        break;
                    }
                }
            }
        }
    }
 #endif
#endif

    p_instance_ctrl->p_correction_info = &g_ctsu_correction_info;
    p_instance_ctrl->rd_index          = 0;
    p_instance_ctrl->wr_index          = 0;
    p_instance_ctrl->state             = CTSU_STATE_IDLE;

    p_instance_ctrl->p_callback        = p_cfg->p_callback;
    p_instance_ctrl->p_context         = p_cfg->p_context;
    p_instance_ctrl->p_callback_memory = NULL;

    p_instance_ctrl->serial_tuning_enable     = 0;
    p_instance_ctrl->serial_tuning_mutual_cnt = 0;

#if defined(CTSU_CFG_TARGET_VALUE_QE_SUPPORT)
    p_instance_ctrl->tuning_self_target_value   = p_cfg->tuning_self_target_value;
    p_instance_ctrl->tuning_mutual_target_value = p_cfg->tuning_mutual_target_value;
#else
    p_instance_ctrl->tuning_self_target_value   = CTSU_TUNING_VALUE_SELF;
    p_instance_ctrl->tuning_mutual_target_value = CTSU_TUNING_VALUE_MUTUAL;
#endif

    /* Mark driver as open */
    p_instance_ctrl->open = CTSU_OPEN;

    return err;
}

/*******************************************************************************************************************//**
 * @brief This function should be called each time a periodic timer expires.
 * If initial offset tuning is enabled, The first several calls are used to tuning for the sensors.
 * Before starting the next scan, first get the data with R_CTSU_DataGet().
 * If a different control block scan should be run, check the scan is complete before executing.
 * Implements @ref ctsu_api_t::scanStart.
 *
 * Example:
 * @snippet r_ctsu_example.c R_CTSU_ScanStart
 *
 * @retval FSP_SUCCESS              CTSU successfully configured.
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 * @retval FSP_ERR_CTSU_SCANNING    Scanning this instance or other.
 * @retval FSP_ERR_CTSU_NOT_GET_DATA    The previous data has not been retrieved by DataGet.
 **********************************************************************************************************************/
fsp_err_t R_CTSU_ScanStart (ctsu_ctrl_t * const p_ctrl)
{
    fsp_err_t              err             = FSP_SUCCESS;
    ctsu_instance_ctrl_t * p_instance_ctrl = (ctsu_instance_ctrl_t *) p_ctrl;
#if (BSP_FEATURE_CTSU_VERSION == 1)
    uint8_t                temp;
#endif
#if (CTSU_CFG_PARAM_CHECKING_ENABLE == 1)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN(CTSU_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    if (CTSU_CAP_SOFTWARE == p_instance_ctrl->cap)
    {
#if (BSP_FEATURE_CTSU_VERSION == 2)
        /* Can be checked if the previous measurement was a software trigger */
        if (0 == (0x02 & CTSU.CTSUCRA.LONG))
        {
            FSP_ERROR_RETURN(0x01 != (CTSU.CTSUCRA.LONG & 0x01), FSP_ERR_CTSU_SCANNING);
        }
#endif
#if (BSP_FEATURE_CTSU_VERSION == 1)
        /* Can be checked if the previous measurement was a software trigger */
        if (0 == (0x02 & CTSU.CTSUCR0.BYTE))
        {
            FSP_ERROR_RETURN(0x01 != (CTSU.CTSUCR0.BYTE & 0x01), FSP_ERR_CTSU_SCANNING);
        }
#endif
    }

    FSP_ERROR_RETURN(CTSU_STATE_SCANNED != p_instance_ctrl->state, FSP_ERR_CTSU_NOT_GET_DATA);

    gp_ctsu_isr_context = p_instance_ctrl;

#if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
    if (((1 == p_instance_ctrl->p_ctsu_cfg->ajfen) && (1 == p_instance_ctrl->p_ctsu_cfg->majirimd)) ||
        ((0 == p_instance_ctrl->p_ctsu_cfg->ajfen) && (0 == p_instance_ctrl->p_ctsu_cfg->majority_mode)))
    {
        if (0 == p_instance_ctrl->mcact_flag)
        {
            if (CTSU_TUNING_COMPLETE == p_instance_ctrl->tuning)
            {
                ctsu_multiclock_auto_correction_register_set(p_instance_ctrl);
                p_instance_ctrl->mcact_flag = 1;
                CTSU.CTSUOPT.BIT.MCACFEN    = 1;
                CTSU.CTSUOPT.BIT.MAJIRIMD   = p_instance_ctrl->p_ctsu_cfg->majirimd;
            }
            else
            {
                CTSU.CTSUOPT.BIT.MAJIRIMD = 0;
                CTSU.CTSUOPT.BIT.MCACFEN  = 0;
            }
        }
        else
        {
            CTSU.CTSUOPT.BIT.MCACFEN  = 1;
            CTSU.CTSUOPT.BIT.MAJIRIMD = p_instance_ctrl->p_ctsu_cfg->majirimd;
        }
    }
    else
    {
        CTSU.CTSUOPT.BIT.MAJIRIMD = 0;
        CTSU.CTSUOPT.BIT.MCACFEN = 0;
    }
#endif

#if (CTSU_CFG_DTC_SUPPORT_ENABLE == 1)
    err = ctsu_transfer_configure(p_instance_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif


#if (BSP_FEATURE_CTSU_VERSION == 2)

    if (CTSU_CAP_SOFTWARE == p_instance_ctrl->cap)
    {
        /* specify Software trigger usage */
        CTSU.CTSUCRA.BIT.CAP = 0;
        CTSU.CTSUCRA.BIT.SNZ = 0;
    }
    else
    {
        /* specify external trigger usage and enable power saving */
        CTSU.CTSUCRA.BIT.CAP = 1;
        CTSU.CTSUCRA.BIT.SNZ = 1;
        CTSU.CTSUCRA.BIT.STRT = 0; // To write CTSUCR1
    }

    /* Write CTSU Control Register 1 and save mode */

    if ((CTSU_MODE_CORRECTION_SCAN != p_instance_ctrl->md) &&
        (CTSU_MODE_DIAGNOSIS_SCAN != p_instance_ctrl->md))
    {
        CTSU.CTSUCRA.BIT.TXVSEL = (p_instance_ctrl->p_ctsu_cfg->txvsel << 1 ) | p_instance_ctrl->p_ctsu_cfg->txvsel2;

        CTSU.CTSUCRA.BIT.ATUNE1 = ((p_instance_ctrl->ctsucr1 >> 3) & 0x01);
        CTSU.CTSUCRA.BIT.MD0 = ((p_instance_ctrl->ctsucr1 >> 6) & 0x01);
        CTSU.CTSUCRA.BIT.MD1 = ((p_instance_ctrl->ctsucr1 >> 7) & 0x01);
        CTSU.CTSUCRA.BIT.ATUNE2 = ((p_instance_ctrl->ctsucr2 >> 1) & 0x01);
        CTSU.CTSUCRA.BIT.POSEL = ((p_instance_ctrl->ctsucr2 >> 4) & 0x03);
        if (CTSU_MODE_CURRENT_SCAN == p_instance_ctrl->md)
        {
            CTSU.CTSUCRB.BIT.SST    = CTSU_SST_RECOMMEND_CURRENT;
            CTSU.CTSUCRA.BIT.DCMODE = 1;
            CTSU.CTSUCRA.BIT.DCBACK = 1;
            CTSU.CTSUMCH.BIT.MCA0   = 1;
            CTSU.CTSUMCH.BIT.MCA1   = 0;
            CTSU.CTSUMCH.BIT.MCA2   = 0;
            CTSU.CTSUMCH.BIT.MCA3   = 0;
        }
        else
        {
            CTSU.CTSUCRB.BIT.SST    = CTSU_SST_RECOMMEND;
            CTSU.CTSUCRA.BIT.DCMODE = 0;
            CTSU.CTSUCRA.BIT.DCBACK = 0;
            CTSU.CTSUMCH.BIT.MCA0   = 1;
 #if (CTSU_CFG_NUM_SUMULTI >= 2)
            CTSU.CTSUMCH.BIT.MCA1 = 1;
 #endif
 #if (CTSU_CFG_NUM_SUMULTI >= 3)
            CTSU.CTSUMCH.BIT.MCA2 = 1;
 #endif
        }

        /* Write Channel setting */
        CTSU.CTSUCHACA.LONG  = (uint32_t)((p_instance_ctrl->ctsuchac3 << 24) |
                                          (p_instance_ctrl->ctsuchac2 << 16) |
                                          (p_instance_ctrl->ctsuchac1 << 8 ) |
                                          (p_instance_ctrl->ctsuchac0));
        CTSU.CTSUCHACB.LONG  = (uint32_t)(p_instance_ctrl->ctsuchac4);

        CTSU.CTSUCHTRCA.LONG  = (uint32_t)((p_instance_ctrl->ctsuchtrc3 << 24) |
                                           (p_instance_ctrl->ctsuchtrc2 << 16) |
                                           (p_instance_ctrl->ctsuchtrc1 << 8 ) |
                                           (p_instance_ctrl->ctsuchtrc0));
        CTSU.CTSUCHTRCB.LONG  = (uint32_t)(p_instance_ctrl->ctsuchtrc4);

    }

 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
    if (CTSU_MODE_DIAGNOSIS_SCAN == p_instance_ctrl->md)
    {
        err = (fsp_err_t)ctsu_diag_scan_start2(p_instance_ctrl);
    }
 #endif
 #if (CTSU_CFG_TEMP_CORRECTION_SUPPORT == 1)
    if (CTSU_MODE_CORRECTION_SCAN == p_instance_ctrl->md)
    {
        CTSU.CTSUCRB.BIT.SST = CTSU_SST_RECOMMEND;
        ctsu_correction_scan_start();
  #if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 1)
        /* Disable automatic correction during temperature correction */
        CTSU.CTSUOPT.BIT.CCOCFEN = 0;
  #endif
    }
 #endif

 #if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
    if (CTSU_TUNING_COMPLETE == p_instance_ctrl->tuning)
    {
        /* Enable / disable switching of automatic judgement */
        CTSU.CTSUOPT.BIT.MTUCFEN  = p_instance_ctrl->p_ctsu_cfg->mtucfen;
        CTSU.CTSUOPT.BIT.AJFEN    = p_instance_ctrl->p_ctsu_cfg->ajfen;

        if (1 == p_instance_ctrl->p_ctsu_cfg->ajfen)
        {
            /* Setting the number of moving averages and the number of baseline averages for automatic judgement */
            CTSU.CTSUAJCR.BIT.AJMMAT = p_instance_ctrl->ajmmat;
            CTSU.CTSUAJCR.BIT.AJBMAT = p_instance_ctrl->ajbmat;
            CTSU.CTSUAJCR.BIT.JC     = p_instance_ctrl->p_ctsu_cfg->jc;
            CTSU.CTSUAJRR.BIT.TJR0   = 0;
            CTSU.CTSUAJRR.BIT.TJR1   = 0;
            CTSU.CTSUAJRR.BIT.TJR2   = 0;
        }
    }

    if (1 == p_instance_ctrl->blini_flag)
    {
        /* Set blini to 1 at the first judgement of automatic judgement */
        CTSU.CTSUAJCR.BIT.BLINI = 1;
    }
 #endif

 #if (CTSU_CFG_MULTIPLE_ELECTRODE_CONNECTION_ENABLE == 1)
    /* Parameter setting for multiple electrode connection */
    CTSU.CTSUCALIB.BIT.TSOD   = p_instance_ctrl->tsod;
    CTSU.CTSUCALIB.BIT.IOCSEL = 0;
    if (1 == p_instance_ctrl->tsod)
    {
        /* When using MEC, MD0 bit is set to single scan mode. */
        CTSU.CTSUCRA.BIT.MD0  = 0;
        CTSU.CTSUMCH.BIT.MCH0 = p_instance_ctrl->mec_ts;

        /* Set MCH1 when using both MEC and Active Shield.*/
        if((0 != p_instance_ctrl->ctsuchtrc0)
                || (0 != p_instance_ctrl->ctsuchtrc1)
                || (0 != p_instance_ctrl->ctsuchtrc2)
                || (0 != p_instance_ctrl->ctsuchtrc3)
                || (0 != p_instance_ctrl->ctsuchtrc4))
        {
            CTSU.CTSUMCH.BIT.MCH1 = p_instance_ctrl->mec_shield_ts;
        }
    }
 #endif

    p_instance_ctrl->state = CTSU_STATE_SCANNING;

    /* Set CTSU_STRT bit to start scan */
    CTSU.CTSUCRA.BIT.STRT = 1;
#endif
#if (BSP_FEATURE_CTSU_VERSION == 1)

    if (CTSU_CAP_SOFTWARE == p_instance_ctrl->cap)
    {
        /* specify Software trigger usage */
        CTSU.CTSUCR0.BIT.CTSUCAP = 0;
        CTSU.CTSUCR0.BIT.CTSUSNZ = 0;
    }
    else
    {
        /* specify external trigger usage and enable power saving */
        CTSU.CTSUCR0.BIT.CTSUCAP = 1;
        CTSU.CTSUCR0.BIT.CTSUSNZ = 1;
        CTSU.CTSUCR0.BIT.CTSUSTRT = 0; // To write CTSUCR1
    }


    if (CTSU_MODE_DIAGNOSIS_SCAN != p_instance_ctrl->md)
    {
        temp            = (uint8_t) (CTSU.CTSUCR1.BYTE & ~(CTSU_CR1_MODIFY_BIT));
        CTSU.CTSUCR1.BYTE = (uint8_t) (temp | (p_instance_ctrl->ctsucr1 & CTSU_CR1_MODIFY_BIT)); // MD1, MD0, ATUNE1

 #if BSP_FEATURE_CTSU_HAS_TXVSEL
        CTSU.CTSUCR0.BIT.CTSUTXVSEL = p_instance_ctrl->p_ctsu_cfg->txvsel;
 #endif

        /* Write Channel setting */
        CTSU.CTSUCHAC0.BYTE = p_instance_ctrl->ctsuchac0;
        CTSU.CTSUCHAC1.BYTE = p_instance_ctrl->ctsuchac1;
 #if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 3)
        CTSU.CTSUCHAC2.BYTE = p_instance_ctrl->ctsuchac2;
 #endif
 #if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 4)
        CTSU.CTSUCHAC3.BYTE = p_instance_ctrl->ctsuchac3;
 #endif
 #if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 5)
        CTSU.CTSUCHAC4.BYTE = p_instance_ctrl->ctsuchac4;
 #endif

 #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
        CTSU.CTSUCHTRC0.BYTE = p_instance_ctrl->ctsuchtrc0;
        CTSU.CTSUCHTRC1.BYTE = p_instance_ctrl->ctsuchtrc1;
  #if (BSP_FEATURE_CTSU_CTSUCHTRC_REGISTER_COUNT >= 3)
        CTSU.CTSUCHTRC2.BYTE = p_instance_ctrl->ctsuchtrc2;
  #endif
  #if (BSP_FEATURE_CTSU_CTSUCHTRC_REGISTER_COUNT >= 4)
        CTSU.CTSUCHTRC3.BYTE = p_instance_ctrl->ctsuchtrc3;
  #endif
  #if (BSP_FEATURE_CTSU_CTSUCHTRC_REGISTER_COUNT >= 5)
        CTSU.CTSUCHTRC4.BYTE = p_instance_ctrl->ctsuchtrc4;
  #endif
 #endif
    }

 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
    if (CTSU_MODE_DIAGNOSIS_SCAN == p_instance_ctrl->md)
    {
        ctsu_diag_scan_start1(p_instance_ctrl);
    }
 #endif

    p_instance_ctrl->state = CTSU_STATE_SCANNING;

    /* Set CTSU_STRT bit to start scan */
    CTSU.CTSUCR0.BIT.CTSUSTRT = 1;
#endif



    return err;
}

/*******************************************************************************************************************//**
 * @brief This function gets the sensor values as scanned by the CTSU.
 * If initial offset tuning is enabled, The first several calls are used to tuning for the sensors.
 * Implements @ref ctsu_api_t::dataGet.
 *
 * Example:
 * @snippet r_ctsu_example.c R_CTSU_DataGet
 *
 * @retval FSP_SUCCESS              CTSU successfully configured.
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 * @retval FSP_ERR_CTSU_SCANNING    Scanning this instance.
 * @retval FSP_ERR_CTSU_INCOMPLETE_TUNING      Incomplete initial offset tuning.
 * @retval FSP_ERR_CTSU_DIAG_NOT_YET      Diagnosis of data collected no yet.
 * @retval FSP_ERR_INVALID_MODE           The mode of automatic judgement on is invalid.
 * @retval FSP_ERR_ABORTED                Operate error of Diagnosis ADC data collection ,since ADC use other
 **********************************************************************************************************************/
fsp_err_t R_CTSU_DataGet (ctsu_ctrl_t * const p_ctrl, uint16_t * p_data)
{
    fsp_err_t              err             = FSP_SUCCESS;
    ctsu_instance_ctrl_t * p_instance_ctrl = (ctsu_instance_ctrl_t *) p_ctrl;
    uint16_t               element_id;
    uint16_t               majority_mode_id;

#if (CTSU_CFG_PARAM_CHECKING_ENABLE == 1)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_data);
    FSP_ERROR_RETURN(CTSU_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    FSP_ERROR_RETURN(CTSU_STATE_SCANNING != p_instance_ctrl->state, FSP_ERR_CTSU_SCANNING);
    FSP_ERROR_RETURN(0 == p_instance_ctrl->p_ctsu_cfg->ajfen, FSP_ERR_INVALID_MODE);

#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_TEMP_CORRECTION_SUPPORT == 1)
    if (CTSU_MODE_CORRECTION_SCAN == p_instance_ctrl->md)
    {
        err = ctsu_correction_data_get(p_instance_ctrl, p_data);
  #if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 1)
        /* Enable automatic correction after temperature correction is completed */
        CTSU.CTSUOPT.BIT.CCOCFEN = 1;
  #endif
        p_instance_ctrl->state = CTSU_STATE_IDLE;

        return err;
    }
 #endif

 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
    if (CTSU_MODE_DIAGNOSIS_SCAN == p_instance_ctrl->md)
    {
        err = ctsu_diag_data_get2(p_data);
        p_instance_ctrl->state = CTSU_STATE_IDLE;
        if (FSP_ERR_CTSU_DIAG_NOT_YET == err)
        {
            err = FSP_ERR_CTSU_DIAG_NOT_YET;
        }
        else if (FSP_ERR_ABORTED == err)
        {
            err = FSP_ERR_ABORTED;
        }
        else
        {
            err = FSP_SUCCESS;
        }

        return err;
    }
 #endif
#endif

#if (BSP_FEATURE_CTSU_VERSION == 1)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
    if (CTSU_MODE_DIAGNOSIS_SCAN == p_instance_ctrl->md)
    {
        err = ctsu_diag_data_get1();
        p_instance_ctrl->state = CTSU_STATE_IDLE;
        if (FSP_ERR_CTSU_DIAG_NOT_YET == err)
        {
            err = FSP_ERR_CTSU_DIAG_NOT_YET;
        }
        else
        {
            err = FSP_SUCCESS;
        }

        return err;
    }
 #endif
#endif

    if (CTSU_STATE_SCANNED == p_instance_ctrl->state)
    {
        if (CTSU_TUNING_COMPLETE == p_instance_ctrl->tuning)
        {
            if (p_instance_ctrl->average == p_instance_ctrl->num_moving_average)
            {
                /* Do nothing */
            }
            else if (p_instance_ctrl->average < p_instance_ctrl->num_moving_average)
            {
                (p_instance_ctrl->average)++;
            }
            else
            {
                p_instance_ctrl->average = p_instance_ctrl->num_moving_average;
            }
        }

        ctsu_correction_exec(p_instance_ctrl);

        if (CTSU_TUNING_INCOMPLETE == p_instance_ctrl->tuning)
        {
            if ((CTSU_MODE_SELF_MULTI_SCAN == p_instance_ctrl->md) ||
                (CTSU_MODE_MUTUAL_FULL_SCAN == p_instance_ctrl->md))
            {
                ctsu_initial_offset_tuning(p_instance_ctrl);
            }
        }
        p_instance_ctrl->state = CTSU_STATE_IDLE;
    }

    FSP_ERROR_RETURN(0 < p_instance_ctrl->average, FSP_ERR_CTSU_INCOMPLETE_TUNING);

#if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
    if (CTSU_MODE_SELF_MULTI_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_instance_ctrl->md))
    {
        for (element_id = 0; element_id < p_instance_ctrl->num_elements; element_id++)
        {
            for (majority_mode_id = 0; majority_mode_id < CTSU_MAJORITY_MODE_ELEMENTS; majority_mode_id++)
            {
                *p_data =
                    (p_instance_ctrl->p_self_data + element_id * CTSU_MAJORITY_MODE_ELEMENTS +
                     majority_mode_id)->int_data;
                p_data++;
            }
        }
    }
#endif
#if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
    if (CTSU_MODE_MUTUAL_FULL_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_instance_ctrl->md))
    {
        if (true == p_instance_ctrl->serial_tuning_enable)
        {
            /* Serial Tuning Phase1 */
            if ((0 == p_instance_ctrl->ctsuchtrc0) && (0 == p_instance_ctrl->ctsuchtrc1) &&
                (0 == p_instance_ctrl->ctsuchtrc2) && (0 == p_instance_ctrl->ctsuchtrc3) &&
                (0 == p_instance_ctrl->ctsuchtrc4))
            {
                return FSP_SUCCESS;
            }
        }
        
        for (element_id = 0; element_id < p_instance_ctrl->num_elements; element_id++)
        {
            for (majority_mode_id = 0; majority_mode_id < CTSU_MAJORITY_MODE_ELEMENTS; majority_mode_id++)
            {
                *p_data =
                    (p_instance_ctrl->p_mutual_pri_data + element_id * CTSU_MAJORITY_MODE_ELEMENTS +
                     majority_mode_id)->int_data;
                p_data++;
                *p_data =
                    (p_instance_ctrl->p_mutual_snd_data + element_id * CTSU_MAJORITY_MODE_ELEMENTS +
                     majority_mode_id)->int_data;
                p_data++;
            }
        }
    }
#endif

    return err;
}

/*******************************************************************************************************************//**
 * @brief This function gets the result of automatic judgement button. Call after the scan is completed.
 * The result is 64-bit bitmaps and is stored in order of TS number for specified ctsu control.
 * After the initial judgement, the baseline initialization bit is set and the automatic judgement threshold is set.
 * This function is called only for automatic judgement.
 * Implements @ref ctsu_api_t::autoJudgementDataGet.
 *
 * Example:
 * @snippet r_ctsu_example.c R_CTSU_AutoJudgementDataGet
 *
 * @retval FSP_SUCCESS              CTSU successfully configured.
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 * @retval FSP_ERR_CTSU_SCANNING    Scanning this instance.
 * @retval FSP_ERR_INVALID_MODE     The mode of automatic judgement off is invalid.
 **********************************************************************************************************************/
fsp_err_t R_CTSU_AutoJudgementDataGet (ctsu_ctrl_t * const p_ctrl, uint64_t * p_button_status)
{
    fsp_err_t              err             = FSP_SUCCESS;
    ctsu_instance_ctrl_t * p_instance_ctrl = (ctsu_instance_ctrl_t *) p_ctrl;
#if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
    uint16_t               element_id;
#endif

#if (CTSU_CFG_PARAM_CHECKING_ENABLE == 1)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_button_status);
    FSP_ERROR_RETURN(CTSU_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    FSP_ERROR_RETURN(CTSU_STATE_SCANNING != p_instance_ctrl->state, FSP_ERR_CTSU_SCANNING);
    FSP_ERROR_RETURN(1 == p_instance_ctrl->p_ctsu_cfg->ajfen, FSP_ERR_INVALID_MODE);

#if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
    if (1 == p_instance_ctrl->blini_flag)
    {
        p_instance_ctrl->blini_flag = 0;

        /* After the initial judgement of automatic judgement, set the baseline initialization bit to 0. */
        CTSU.CTSUAJCR.BIT.BLINI = 0;

        ctsu_auto_judge_threshold_calc (p_instance_ctrl);
    }

    if (CTSU_STATE_SCANNED == p_instance_ctrl->state)
    {
        for (element_id = 0; element_id < p_instance_ctrl->num_elements; element_id++)
        {
 #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
            if (1 == p_instance_ctrl->p_ctsu_cfg->majirimd)
            {
                /* Reflect the final result bit of automatic judgement in buttun_status */
                if (1 == (((p_instance_ctrl->p_auto_judge[element_id].ajrr) >> CTSU_AUTO_FINAL_JUDGE_BIT) & 0x1))
                {
                    *p_button_status |= ((uint64_t) 1 << element_id);
                }
                else
                {
                    *p_button_status &= ~((uint64_t) 1 << element_id);
                }
            }
            else
 #endif
            {
                /* Reflect the final result bit of automatic judgement in buttun_status */
                /* Since the final result of the automatic judgement is output to the FJR bit of the final multi-clock, */
                /* the result of the final multi-clock is taken out.                                                   */
                if (1 == (((p_instance_ctrl->p_auto_judge[(element_id * CTSU_MAJORITY_MODE_ELEMENTS) + (CTSU_MAJORITY_MODE_ELEMENTS - 1)].ajrr) >> CTSU_AUTO_FINAL_JUDGE_BIT) & 0x1))
                {
                    *p_button_status |= ((uint64_t) 1 << element_id);
                }
                else
                {
                    *p_button_status &= ~((uint64_t) 1 << element_id);
                }
            }
        }

        p_instance_ctrl->state = CTSU_STATE_IDLE;
    }
#endif
    return err;
}

/*******************************************************************************************************************//**
 * @brief This function tunes the offset register(SO). Call after the measurement is completed.
 * If the return value is FSP_ERR_CTSU_INCOMPLETE_TUNING, tuning is not complete.
 * Execute the measurement and this function call routine until the return value becomes FSP_SUCCESS.
 * It is recommended to run this routine after R_CTSU_Open().
 * It can be recalled and tuned again.
 * When the automatic judgement is enabled, after the offset tuning is completed,the baseline initialization bit flag is set.
 * Implements @ref ctsu_api_t::offsetTuning.
 *
 * Example:
 * @snippet r_ctsu_example.c R_CTSU_OffsetTuning
 *
 * @retval FSP_SUCCESS              CTSU successfully configured.
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 * @retval FSP_ERR_CTSU_SCANNING    Scanning this instance.
 * @retval FSP_ERR_CTSU_INCOMPLETE_TUNING      Incomplete initial offset tuning.
 **********************************************************************************************************************/
fsp_err_t R_CTSU_OffsetTuning (ctsu_ctrl_t * const p_ctrl)
{
    fsp_err_t              err             = FSP_SUCCESS;
    ctsu_instance_ctrl_t * p_instance_ctrl = (ctsu_instance_ctrl_t *) p_ctrl;
    uint16_t               element_id;
#if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
    uint16_t               i;
#endif

#if (CTSU_CFG_PARAM_CHECKING_ENABLE == 1)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN(CTSU_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    FSP_ERROR_RETURN(CTSU_STATE_SCANNING != p_instance_ctrl->state, FSP_ERR_CTSU_SCANNING);

    if (CTSU_TUNING_COMPLETE == p_instance_ctrl->tuning)
    {
        for (element_id = 0; element_id < p_instance_ctrl->num_elements; element_id++)
        {
            /* Counter clear for re-offset tuning */
            *(p_instance_ctrl->p_element_complete_flag + element_id) = 0;
#if (BSP_FEATURE_CTSU_VERSION == 2)
            *(p_instance_ctrl->p_frequency_complete_flag + element_id) = 0;
#endif
            *(p_instance_ctrl->p_tuning_diff + element_id)  = 0;
        }
    }

    p_instance_ctrl->tuning = CTSU_TUNING_INCOMPLETE;

    if (CTSU_STATE_SCANNED == p_instance_ctrl->state)
    {
        if (0 == p_instance_ctrl->p_ctsu_cfg->ajfen)
        {
            ctsu_correction_exec(p_instance_ctrl);
        }
#if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
        else
        {
            CTSU.CTSUOPT.BIT.AJFEN = 0;
            CTSU.CTSUOPT.BIT.MTUCFEN = 0;

            for (element_id = 0; element_id < p_instance_ctrl->num_elements; element_id++)
            {
                for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
                {
                    if (CTSU_MODE_SELF_MULTI_SCAN == p_instance_ctrl->md)
                    {
                        p_instance_ctrl->p_self_corr[(element_id * CTSU_CFG_NUM_SUMULTI) + i] = p_instance_ctrl->p_self_raw[(element_id * CTSU_CFG_NUM_SUMULTI) + i];
                    }
 #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
                    else
                    {
                        p_instance_ctrl->p_mutual_pri_corr[(element_id * CTSU_CFG_NUM_SUMULTI) + i] = p_instance_ctrl->p_mutual_raw[(element_id * CTSU_MUTUAL_BUF_SIZE) + (i * 2)];
                    }
 #endif
 #if (CTSU_CFG_MAJORITY_MODE & CTSU_JUDGEMENT_MAJORITY_MODE)
                    /* Initial value setting of DTC transfer information for automatic judgement */
                    p_instance_ctrl->p_auto_judge[(element_id * CTSU_MAJORITY_MODE_ELEMENTS) + i].ajthr   = 0;
                    p_instance_ctrl->p_auto_judge[(element_id * CTSU_MAJORITY_MODE_ELEMENTS) + i].ajmmar  = 0;
                    p_instance_ctrl->p_auto_judge[(element_id * CTSU_MAJORITY_MODE_ELEMENTS) + i].ajblact = 0;
                    p_instance_ctrl->p_auto_judge[(element_id * CTSU_MAJORITY_MODE_ELEMENTS) + i].ajblar  = 0;
                    p_instance_ctrl->p_auto_judge[(element_id * CTSU_MAJORITY_MODE_ELEMENTS) + i].ajrr    = 0;
 #endif
                }
 #if ((CTSU_CFG_MAJORITY_MODE & CTSU_JUDGEMENT_MAJORITY_MODE) == 0)
                p_instance_ctrl->p_auto_judge[element_id].ajthr   = 0;
                p_instance_ctrl->p_auto_judge[element_id].ajmmar  = 0;
                p_instance_ctrl->p_auto_judge[element_id].ajblact = 0;
                p_instance_ctrl->p_auto_judge[element_id].ajblar  = 0;
                p_instance_ctrl->p_auto_judge[element_id].ajrr    = 0;
 #endif
            }
        }
#endif
        if (CTSU_TUNING_INCOMPLETE == p_instance_ctrl->tuning)
        {
            if ((CTSU_MODE_SELF_MULTI_SCAN == p_instance_ctrl->md) ||
                (CTSU_MODE_MUTUAL_FULL_SCAN == p_instance_ctrl->md))
            {
                ctsu_initial_offset_tuning(p_instance_ctrl);
            }
        }
#if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
        if (1 == p_instance_ctrl->p_ctsu_cfg->ajfen)
        {
            if (CTSU_TUNING_COMPLETE == p_instance_ctrl->tuning)
            {
                /* Initialization of baseline mean for automatic judgement */
                p_instance_ctrl->blini_flag = 1;
            }
        }
#endif
#if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
        p_instance_ctrl->mcact_flag = 0;
#endif
        p_instance_ctrl->state = CTSU_STATE_IDLE;
    }

    FSP_ERROR_RETURN(CTSU_TUNING_COMPLETE == p_instance_ctrl->tuning, FSP_ERR_CTSU_INCOMPLETE_TUNING);

    return err;
}

/*******************************************************************************************************************//**
 * @brief This function scan stops the sensor as scanning by the CTSU.
 * Implements @ref ctsu_api_t::scanStop.
 * @retval FSP_SUCCESS              CTSU successfully scan stop.
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 **********************************************************************************************************************/
fsp_err_t R_CTSU_ScanStop (ctsu_ctrl_t * const p_ctrl)
{
    fsp_err_t              err             = FSP_SUCCESS;
    ctsu_instance_ctrl_t * p_instance_ctrl = (ctsu_instance_ctrl_t *) p_ctrl;
#if (CTSU_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(CTSU_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    if (CTSU_STATE_SCANNING == p_instance_ctrl->state)
    {
#if (BSP_FEATURE_CTSU_VERSION == 2)
        CTSU.CTSUCRA.LONG ^= 0x11;
#else
        CTSU.CTSUCR0.BYTE ^= 0x11;
#endif

        IR(CTSU,CTSUWR)= 0;
        IR(CTSU,CTSURD)= 0;
        IR(CTSU,CTSUFN)= 0;

        p_instance_ctrl->state    = CTSU_STATE_IDLE;
        p_instance_ctrl->wr_index = 0;
        p_instance_ctrl->rd_index = 0;
    }

    return err;
}

/*******************************************************************************************************************//**
 * Updates the user callback and has option of providing memory for callback structure.
 * Implements ctsu_api_t::callbackSet
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 * @retval  FSP_ERR_NO_CALLBACK_MEMORY   p_callback is non-secure and p_callback_memory is either secure or NULL.
 **********************************************************************************************************************/
fsp_err_t R_CTSU_CallbackSet (ctsu_ctrl_t * const          p_api_ctrl,
                              void (                     * p_callback)(ctsu_callback_args_t *),
                              void const * const           p_context,
                              ctsu_callback_args_t * const p_callback_memory)
{
    ctsu_instance_ctrl_t * p_ctrl = (ctsu_instance_ctrl_t *) p_api_ctrl;

#if (CTSU_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_callback);
    FSP_ERROR_RETURN(CTSU_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

#if BSP_TZ_SECURE_BUILD

    /* Get security state of p_callback */
    bool callback_is_secure =
        (NULL == cmse_check_address_range((void *) p_callback, sizeof(void *), CMSE_AU_NONSECURE));

 #if CTSU_CFG_PARAM_CHECKING_ENABLE

    /* In secure projects, p_callback_memory must be provided in non-secure space if p_callback is non-secure */
    ctsu_callback_args_t * const p_callback_memory_checked = cmse_check_pointed_object(p_callback_memory,
                                                                                       CMSE_AU_NONSECURE);
    FSP_ERROR_RETURN(callback_is_secure || (NULL != p_callback_memory_checked), FSP_ERR_NO_CALLBACK_MEMORY);
 #endif
#endif

    /* Store callback and context */
#if BSP_TZ_SECURE_BUILD
    p_ctrl->p_callback = callback_is_secure ? p_callback :
                         (void (*)(ctsu_callback_args_t *))cmse_nsfptr_create(p_callback);
#else
    p_ctrl->p_callback = p_callback;
#endif
    p_ctrl->p_context         = p_context;
    p_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Disables specified CTSU control block. Implements @ref ctsu_api_t::close.
 *
 * @retval FSP_SUCCESS              CTSU successfully configured.
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 **********************************************************************************************************************/
fsp_err_t R_CTSU_Close (ctsu_ctrl_t * const p_ctrl)
{
    fsp_err_t              err             = FSP_SUCCESS;
    ctsu_instance_ctrl_t * p_instance_ctrl = (ctsu_instance_ctrl_t *) p_ctrl;

    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN(CTSU_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    g_ctsu_element_index = (uint8_t) (g_ctsu_element_index - p_instance_ctrl->num_elements);
#if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
    if (CTSU_MODE_SELF_MULTI_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_instance_ctrl->md))
    {
        g_ctsu_self_element_index = (uint8_t) (g_ctsu_self_element_index - p_instance_ctrl->num_elements);
    }
#endif
#if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
    if (CTSU_MODE_MUTUAL_FULL_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_instance_ctrl->md))
    {
        g_ctsu_mutual_element_index = (uint8_t) (g_ctsu_mutual_element_index - p_instance_ctrl->num_elements);
    }
#endif
    if (0 == g_ctsu_element_index)
    {
#if (CTSU_CFG_DTC_SUPPORT_ENABLE == 1)
        err = ctsu_transfer_close(p_instance_ctrl);
#endif
#if (BSP_FEATURE_CTSU_VERSION == 2)
        CTSU.CTSUCRA.BIT.STRT = 0;
#else
        CTSU.CTSUCR0.BIT.CTSUSTRT = 0;
#endif

        /* Get CTSU out of stop state (supply power/clock) */
        R_BSP_InterruptRequestDisable(VECT(CTSU,CTSUWR));
        R_BSP_InterruptRequestDisable(VECT(CTSU,CTSURD));
        R_BSP_InterruptRequestDisable(VECT(CTSU,CTSUFN));

        R_BSP_RegisterProtectDisable (BSP_REG_PROTECT_LPC_CGC_SWR);
        MSTP(CTSU) = 1;
        R_BSP_RegisterProtectEnable (BSP_REG_PROTECT_LPC_CGC_SWR);
    }

    p_instance_ctrl->state = CTSU_STATE_INIT;
    p_instance_ctrl->open  = false;

    return err;
}

/*******************************************************************************************************************//**
 * @brief This function gets the sensor specific data values as scanned by the CTSU.
 * Call this function after calling the R_CTSU_DataGet() function.
 *
 * By setting the third argument to CTSU_SPECIFIC_RAW_DATA,
 * RAW data can be output from the second argument.
 *
 * By setting the third argument to CTSU_SPECIFIC_CCO_CORRECTION_DATA,
 * the cco corrected data can be output from the second argument.
 *
 * By setting the third argument to CTSU_SPECIFIC_CORRECTION_DATA,
 * the frequency corrected data can be output from the second argument.
 *
 * By setting the third argument to CTSU_SPECIFIC_SELECTED_FREQ,
 * Get bitmap of the frequency values used in majority decision from the second argument.(CTSU2 Only)
 * The bitmap is shown as follows.
 *
 * | 2bit                | 1bit                | 0bit                |
 * |---------------------|---------------------|---------------------|
 * | 3rd frequency value | 2nd frequency value | 1st frequency value |
 *
 * Implements @ref ctsu_api_t::specificDataGet.
 *
 * Example:
 * @snippet r_ctsu_example.c R_CTSU_SpecificDataGet
 *
 *
 * @retval FSP_SUCCESS              CTSU successfully configured.
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 * @retval FSP_ERR_CTSU_SCANNING    Scanning this instance.
 * @retval FSP_ERR_CTSU_INCOMPLETE_TUNING      Incomplete initial offset tuning.
 * @retval FSP_ERR_NOT_ENABLED      CTSU_SPECIFIC_SELECTED_FREQ is not enabled in CTSU1.(CTSU2 Only)
 **********************************************************************************************************************/
fsp_err_t R_CTSU_SpecificDataGet (ctsu_ctrl_t * const       p_ctrl,
                                  uint16_t                * p_specific_data,
                                  ctsu_specific_data_type_t specific_data_type)
{
    fsp_err_t              err = FSP_SUCCESS;
    uint16_t               element_id;
    uint16_t               i;
    ctsu_instance_ctrl_t * p_instance_ctrl = (ctsu_instance_ctrl_t *) p_ctrl;
#if (BSP_FEATURE_CTSU_VERSION == 2)
    uint32_t                ctsuso;
    ctsu_correction_multi_t multi;
    uint32_t                snum;
    int32_t                 offset_unit;
    uint16_t                corr_pri[CTSU_CFG_NUM_SUMULTI];
 #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
    uint16_t corr_snd[CTSU_CFG_NUM_SUMULTI];
 #endif
    for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
    {
        corr_pri[i] = 0;
 #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
        corr_snd[i] = 0;
 #endif
    }
#endif

#if (CTSU_CFG_PARAM_CHECKING_ENABLE == 1)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_specific_data);
    FSP_ERROR_RETURN(CTSU_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    FSP_ERROR_RETURN(CTSU_STATE_SCANNING != p_instance_ctrl->state, FSP_ERR_CTSU_SCANNING);
    FSP_ERROR_RETURN(CTSU_TUNING_INCOMPLETE != p_instance_ctrl->tuning, FSP_ERR_CTSU_INCOMPLETE_TUNING);

#if (BSP_FEATURE_CTSU_VERSION == 1)
    FSP_ERROR_RETURN(CTSU_SPECIFIC_SELECTED_FREQ != specific_data_type, FSP_ERR_NOT_ENABLED);
#endif
#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_MAJORITY_MODE & CTSU_JUDGEMENT_MAJORITY_MODE)
    FSP_ERROR_RETURN(CTSU_SPECIFIC_SELECTED_FREQ != specific_data_type, FSP_ERR_NOT_ENABLED);
    FSP_ERROR_RETURN(CTSU_SPECIFIC_CORRECTION_DATA != specific_data_type, FSP_ERR_NOT_ENABLED);
 #endif
 #if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 1)
    FSP_ERROR_RETURN(CTSU_SPECIFIC_RAW_DATA != specific_data_type, FSP_ERR_NOT_ENABLED);
 #endif
#endif

#if (CTSU_CFG_MAJORITY_MODE & (CTSU_JUDGEMENT_MAJORITY_MODE | CTSU_VALUE_MAJORITY_MODE))
    if (p_instance_ctrl->p_ctsu_cfg->majority_mode == 1)
    {
        FSP_ERROR_RETURN(CTSU_SPECIFIC_CORRECTION_DATA != specific_data_type, FSP_ERR_NOT_ENABLED);
    }
#endif

#if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
    if (1 == p_instance_ctrl->p_ctsu_cfg->ajfen)
    {
        if (1 == p_instance_ctrl->p_ctsu_cfg->majirimd)
        {
            FSP_ERROR_RETURN(CTSU_SPECIFIC_CCO_CORRECTION_DATA != specific_data_type, FSP_ERR_NOT_ENABLED);
            FSP_ERROR_RETURN(CTSU_SPECIFIC_CORRECTION_DATA != specific_data_type, FSP_ERR_NOT_ENABLED);
            FSP_ERROR_RETURN(CTSU_SPECIFIC_SELECTED_FREQ != specific_data_type, FSP_ERR_NOT_ENABLED);
        }
    }
    else
    {
        if (0 == p_instance_ctrl->p_ctsu_cfg->majority_mode)
        {
            FSP_ERROR_RETURN(CTSU_SPECIFIC_CCO_CORRECTION_DATA != specific_data_type, FSP_ERR_NOT_ENABLED);
        }
    }
#endif

    if (1 == p_instance_ctrl->p_ctsu_cfg->ajfen)
    {
        if (0 == p_instance_ctrl->p_ctsu_cfg->majirimd)
        {
            FSP_ERROR_RETURN(CTSU_SPECIFIC_CORRECTION_DATA != specific_data_type, FSP_ERR_NOT_ENABLED);
            FSP_ERROR_RETURN(CTSU_SPECIFIC_SELECTED_FREQ != specific_data_type, FSP_ERR_NOT_ENABLED);
        }
    }

    if (CTSU_SPECIFIC_RAW_DATA == specific_data_type)
    {
#if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
        if (CTSU_MODE_SELF_MULTI_SCAN == p_instance_ctrl->md)
        {
            for (element_id = 0; element_id < p_instance_ctrl->num_elements; element_id++)
            {
 #if (BSP_FEATURE_CTSU_VERSION == 1)
                *p_specific_data = (p_instance_ctrl->p_self_raw + element_id)->sen;
                p_specific_data++;
 #endif
 #if (BSP_FEATURE_CTSU_VERSION == 2)
                for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
                {
                    *p_specific_data = *(p_instance_ctrl->p_self_raw + (element_id * CTSU_CFG_NUM_SUMULTI) + i);
                    p_specific_data++;
                }
 #endif
            }
        }
#endif
#if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
        if (CTSU_MODE_MUTUAL_FULL_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_instance_ctrl->md))
        {
            for (element_id = 0; element_id < p_instance_ctrl->num_elements; element_id++)
            {
 #if (BSP_FEATURE_CTSU_VERSION == 1)
                *p_specific_data = (p_instance_ctrl->p_mutual_raw + element_id)->pri_sen;
                p_specific_data++;
                *p_specific_data = (p_instance_ctrl->p_mutual_raw + element_id)->snd_sen;
                p_specific_data++;
 #endif
 #if (BSP_FEATURE_CTSU_VERSION == 2)
                for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
                {
                    *p_specific_data =
                        *(p_instance_ctrl->p_mutual_raw + (element_id * CTSU_CFG_NUM_SUMULTI * 2) + (i * 2));
                    p_specific_data++;
                    *p_specific_data =
                        *(p_instance_ctrl->p_mutual_raw + (element_id * CTSU_CFG_NUM_SUMULTI * 2) + (i * 2) + 1);
                    p_specific_data++;
                }
 #endif
            }
        }
#endif
    }
    else if (CTSU_SPECIFIC_CCO_CORRECTION_DATA == specific_data_type)
    {
#if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
        if (CTSU_MODE_SELF_MULTI_SCAN == p_instance_ctrl->md)
        {
            for (element_id = 0; element_id < (p_instance_ctrl->num_elements); element_id++)
            {
                for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
                {
 #if (BSP_FEATURE_CTSU_VERSION == 2)
  #if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
                    if (1 == p_instance_ctrl->p_ctsu_cfg->ajfen)
                    {
                        *p_specific_data = *(p_instance_ctrl->p_self_raw + (element_id * CTSU_CFG_NUM_SUMULTI) + i);
                        p_specific_data++;
                    }
                    else
  #endif
 #endif
                    {
                        *p_specific_data = *(p_instance_ctrl->p_self_corr + (element_id * CTSU_CFG_NUM_SUMULTI) + i);
                        p_specific_data++;
                    }
                }
            }
        }
#endif
#if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
        if (CTSU_MODE_MUTUAL_FULL_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_instance_ctrl->md))
        {
            for (element_id = 0; element_id < (p_instance_ctrl->num_elements); element_id++)
            {
                for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
                {
 #if (BSP_FEATURE_CTSU_VERSION == 2)
  #if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
                    if (1 == p_instance_ctrl->p_ctsu_cfg->ajfen)
                    {
                        *p_specific_data = *(p_instance_ctrl->p_mutual_raw + ((element_id * CTSU_MUTUAL_BUF_SIZE) + (i * 2)));
                        p_specific_data++;
                        *p_specific_data = *(p_instance_ctrl->p_mutual_raw + ((element_id * CTSU_MUTUAL_BUF_SIZE) + (i * 2) + 1));
                        p_specific_data++;
                    }
                    else
  #endif
 #endif
                    {
                        *p_specific_data = *(p_instance_ctrl->p_mutual_pri_corr + (element_id * CTSU_CFG_NUM_SUMULTI) + i);
                        p_specific_data++;
                        *p_specific_data = *(p_instance_ctrl->p_mutual_snd_corr + (element_id * CTSU_CFG_NUM_SUMULTI) + i);
                        p_specific_data++;
                    }
                }
            }
        }
#endif
    }

#if (BSP_FEATURE_CTSU_VERSION == 2)
    else if (CTSU_SPECIFIC_CORRECTION_DATA == specific_data_type)
    {
 #if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
        if (CTSU_MODE_SELF_MULTI_SCAN == p_instance_ctrl->md)
        {
            for (element_id = 0; element_id < (p_instance_ctrl->num_elements); element_id++)
            {
                for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
                {
                    ctsuso =
                        (p_instance_ctrl->p_ctsuwr[(element_id * CTSU_CFG_NUM_SUMULTI) + i].ctsuso & CTSU_TUNING_MAX);
                    snum = (p_instance_ctrl->p_ctsuwr[(element_id * CTSU_CFG_NUM_SUMULTI)].ctsuso >> 10) &
                           CTSU_SNUM_MAX;
                    offset_unit =
                        (int32_t) ((CTSU_CORRECTION_OFFSET_UNIT * (snum + 1)) / (CTSU_SNUM_RECOMMEND + 1));
                    multi.offset[i] = (ctsuso * (uint32_t) (offset_unit >> (p_instance_ctrl->range)));

                    multi.pri[i] = *(p_instance_ctrl->p_self_corr + (element_id * CTSU_CFG_NUM_SUMULTI) + i);
                    multi.snd[i] = 0;
                }

  #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 0)
                ctsu_correction_freq(&multi, corr_pri, NULL);
  #else
                for (i = 1; i < CTSU_CFG_NUM_SUMULTI; i++)
                {
                    corr_pri[i] = multi.pri[i];
                }
  #endif

                *p_specific_data = *(p_instance_ctrl->p_self_corr + (element_id * CTSU_CFG_NUM_SUMULTI));
                p_specific_data++;

                for (i = 1; i < CTSU_CFG_NUM_SUMULTI; i++)
                {
                    *p_specific_data = corr_pri[i];
                    p_specific_data++;
                }
            }
        }
 #endif
 #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
        if (CTSU_MODE_MUTUAL_FULL_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_instance_ctrl->md))
        {
            for (element_id = 0; element_id < (p_instance_ctrl->num_elements); element_id++)
            {
                for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
                {
                    ctsuso =
                        (p_instance_ctrl->p_ctsuwr[(element_id * CTSU_CFG_NUM_SUMULTI) + i].ctsuso & CTSU_TUNING_MAX);
                    snum = (p_instance_ctrl->p_ctsuwr[(element_id * CTSU_CFG_NUM_SUMULTI)].ctsuso >> 10) &
                           CTSU_SNUM_MAX;
                    offset_unit =
                        (int32_t) ((CTSU_CORRECTION_OFFSET_UNIT * (snum + 1)) / (CTSU_SNUM_RECOMMEND + 1));
                    multi.offset[i] = (ctsuso * (uint32_t) (offset_unit >> (p_instance_ctrl->range)));

                    multi.pri[i] = *(p_instance_ctrl->p_mutual_pri_corr + (element_id * CTSU_CFG_NUM_SUMULTI) + i);
                    multi.snd[i] = *(p_instance_ctrl->p_mutual_snd_corr + (element_id * CTSU_CFG_NUM_SUMULTI) + i);
                }

  #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 0)
                ctsu_correction_freq(&multi, corr_pri, corr_snd);
  #else
                for (i = 1; i < CTSU_CFG_NUM_SUMULTI; i++)
                {
                    corr_pri[i] = multi.pri[i];
                    corr_snd[i] = multi.snd[i];
                }
  #endif

                *p_specific_data = *(p_instance_ctrl->p_mutual_pri_corr + (element_id * CTSU_CFG_NUM_SUMULTI));
                p_specific_data++;
                *p_specific_data = *(p_instance_ctrl->p_mutual_snd_corr + (element_id * CTSU_CFG_NUM_SUMULTI));
                p_specific_data++;

                for (i = 1; i < CTSU_CFG_NUM_SUMULTI; i++)
                {
                    *p_specific_data = corr_pri[i];
                    p_specific_data++;
                    *p_specific_data = corr_snd[i];
                    p_specific_data++;
                }
            }
        }
 #endif
    }
    else if (CTSU_SPECIFIC_SELECTED_FREQ == specific_data_type)
    {
 #if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
        if (CTSU_MODE_SELF_MULTI_SCAN == p_instance_ctrl->md)
        {
            for (element_id = 0; element_id < (p_instance_ctrl->num_elements); element_id++)
            {
                *p_specific_data = *(p_instance_ctrl->p_selected_freq_self + element_id);
                p_specific_data++;
            }
        }
 #endif
 #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
        if (CTSU_MODE_MUTUAL_FULL_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_instance_ctrl->md))
        {
            for (element_id = 0; element_id < (p_instance_ctrl->num_elements); element_id++)
            {
                *p_specific_data = *(p_instance_ctrl->p_selected_freq_mutual + element_id);
                p_specific_data++;
            }
        }
 #endif
    }
#endif
    else
    {
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief This function inserts the value of the second argument as the measurement result value.
 * Call this function after calling the R_CTSU_DataInsert() function.
 * Implements @ref ctsu_api_t::dataInsert.
 *
 * Example:
 * @snippet r_ctsu_example.c R_CTSU_DataInsert
 *
 * @retval FSP_SUCCESS              CTSU successfully configured.
 * @retval FSP_ERR_ASSERTION        Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN         Module is not open.
 * @retval FSP_ERR_CTSU_SCANNING    Scanning this instance.
 * @retval FSP_ERR_CTSU_INCOMPLETE_TUNING      Incomplete initial offset tuning.
 **********************************************************************************************************************/
fsp_err_t R_CTSU_DataInsert (ctsu_ctrl_t * const p_ctrl, uint16_t * p_insert_data)
{
    fsp_err_t              err             = FSP_SUCCESS;
    ctsu_instance_ctrl_t * p_instance_ctrl = (ctsu_instance_ctrl_t *) p_ctrl;
    uint16_t               element_id;
    uint16_t               majority_mode_elem_id;
#if (CTSU_CFG_PARAM_CHECKING_ENABLE == 1)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_insert_data);
    FSP_ERROR_RETURN(CTSU_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    FSP_ERROR_RETURN(CTSU_STATE_SCANNING != p_instance_ctrl->state, FSP_ERR_CTSU_SCANNING);
    FSP_ERROR_RETURN(CTSU_TUNING_INCOMPLETE != p_instance_ctrl->tuning, FSP_ERR_CTSU_INCOMPLETE_TUNING);

#if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
    if (CTSU_MODE_SELF_MULTI_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_instance_ctrl->md))
    {
        /* Data output */
        for (element_id = 0; element_id < p_instance_ctrl->num_elements; element_id++)
        {
            for (majority_mode_elem_id = 0;
                 majority_mode_elem_id < CTSU_MAJORITY_MODE_ELEMENTS;
                 majority_mode_elem_id++)
            {
                (p_instance_ctrl->p_self_data + (element_id * CTSU_MAJORITY_MODE_ELEMENTS) +
                 majority_mode_elem_id)->int_data = *p_insert_data;
                p_insert_data++;
            }
        }
    }
#endif
#if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
    if (CTSU_MODE_MUTUAL_FULL_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_instance_ctrl->md))
    {
        for (element_id = 0; element_id < p_instance_ctrl->num_elements; element_id++)
        {
            for (majority_mode_elem_id = 0;
                 majority_mode_elem_id < CTSU_MAJORITY_MODE_ELEMENTS;
                 majority_mode_elem_id++)
            {
                (p_instance_ctrl->p_mutual_pri_data + (element_id * CTSU_MAJORITY_MODE_ELEMENTS) +
                 majority_mode_elem_id)->int_data = *p_insert_data;
                p_insert_data++;
                (p_instance_ctrl->p_mutual_snd_data + (element_id * CTSU_MAJORITY_MODE_ELEMENTS) +
                 majority_mode_elem_id)->int_data = *p_insert_data;
                p_insert_data++;
            }
        }
    }
#endif

    return err;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup CTSU)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Internal ctsu private function.
 **********************************************************************************************************************/

#if (CTSU_CFG_DTC_SUPPORT_ENABLE == 1)
/***********************************************************************************************************************
 * ctsu_transfer_open
 ***********************************************************************************************************************/
fsp_err_t ctsu_transfer_open (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    R_DTC_Open();

    R_DTC_Control(DTC_CMD_DTC_START, NULL, NULL);

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * ctsu_transfer_close
 ***********************************************************************************************************************/
fsp_err_t ctsu_transfer_close (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    R_DTC_Close();

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * ctsu_transfer_configure
 ***********************************************************************************************************************/
fsp_err_t ctsu_transfer_configure (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    dtc_err_t dtc_err;

 #if (BSP_FEATURE_CTSU_VERSION == 2)
    /* After the initial offset tuning is completed, set the DTC for automatic judgement.                              */
    /* In addition, the method for which automatic judgement is disabled does not set the DTC for automatic judgement. */
    /* Alternatively, DTC for automatic judgment will not be set during correction measurement.                        */
    if ((0 == p_instance_ctrl->p_ctsu_cfg->ajfen) || (CTSU_CORRECTION_RUN == g_ctsu_correction_info.status) || (CTSU_TUNING_INCOMPLETE == p_instance_ctrl->tuning))
    {
        dtc_err = ctsu_transfer_normal(p_instance_ctrl);
        if (dtc_err != DTC_SUCCESS)
        {
            return FSP_ERR_ASSERTION;
        }
    }
  #if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
    else    /* DTC setting for automatic judgement */
    {
        dtc_err = ctsu_transfer_autojudge(p_instance_ctrl);
        if (dtc_err != DTC_SUCCESS)
        {
            return FSP_ERR_ASSERTION;
        }
    }
  #endif
 #endif

 #if (BSP_FEATURE_CTSU_VERSION == 1)
    dtc_err = ctsu_transfer_ctsu1(p_instance_ctrl);
    if (dtc_err != DTC_SUCCESS)
    {
        return FSP_ERR_ASSERTION;
    }
 #endif

    return FSP_SUCCESS;
}

 #if (BSP_FEATURE_CTSU_VERSION == 2)
/***********************************************************************************************************************
 * ctsu_transfer_normal
 ***********************************************************************************************************************/
dtc_err_t ctsu_transfer_normal (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    dtc_err_t dtc_err;

    dtc_err = ctsu_transfer_normal_ctsuwr(p_instance_ctrl);

    dtc_err = ctsu_transfer_normal_ctsurd(p_instance_ctrl);

    return dtc_err;
}

/***********************************************************************************************************************
 * ctsu_transfer_normal_ctsuwr
 ***********************************************************************************************************************/
dtc_err_t ctsu_transfer_normal_ctsuwr (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    dtc_err_t dtc_err;
    uint8_t   ien_bk;
    uint32_t  calc_transfer_count;

    calc_transfer_count = p_instance_ctrl->num_elements * CTSU_CFG_NUM_SUMULTI;

    g_ctsu_dtc_cmd_arg.act_src = (dtc_activation_source_t)IR_CTSU_CTSUWR;

    R_DTC_Control(DTC_CMD_ACT_SRC_DISABLE, NULL, &g_ctsu_dtc_cmd_arg);

    ctsu_transfer_ctsuso_set(0, DTC_CHAIN_TRANSFER_DISABLE, DTC_CHAIN_TRANSFER_CONTINUOUSLY, calc_transfer_count);
    ctsu_transfer_address_set(0, (uint32_t *)p_instance_ctrl->p_ctsuwr, (uint32_t *)&(CTSU.CTSUSO.LONG));

    if (CTSU_CORRECTION_RUN == g_ctsu_correction_info.status)
    {
        g_ctsu_dtc_info[0].transfer_count   = 1;
        g_ctsu_dtc_info[0].source_addr      = (uint32_t)&(g_ctsu_correction_info.ctsuwr);
    }

  #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
    else if (CTSU_MODE_DIAGNOSIS_SCAN == p_instance_ctrl->md)
    {
        if (CTSU_DIAG_CLOCK_RECOVERY == g_ctsu_diag_info.state)
        {
            g_ctsu_dtc_info[0].src_addr_mode      = DTC_SRC_ADDR_FIXED;
            g_ctsu_dtc_info[0].transfer_count     = 3;
        }
        else
        {
            g_ctsu_dtc_info[0].src_addr_mode      = DTC_SRC_ADDR_INCR;
            g_ctsu_dtc_info[0].transfer_count     = 1;
        }

        g_ctsu_dtc_info[0].source_addr            = (uint32_t)&(g_ctsu_diag_info.ctsuwr);
    }
  #endif
    else if (CTSU_MODE_CURRENT_SCAN == p_instance_ctrl->md)
    {
        g_ctsu_dtc_info[0].transfer_count     = p_instance_ctrl->num_elements;
    }
    else
    {
  #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
        if ((0 == p_instance_ctrl->p_ctsu_cfg->majority_mode) && (CTSU_TUNING_INCOMPLETE != p_instance_ctrl->tuning))
        {
            ctsu_transfer_mcact_set(0, calc_transfer_count);
            ctsu_transfer_address_set(0, (uint32_t *)p_instance_ctrl->p_mcact1, (uint32_t *)&(CTSU.CTSUMCACT1.LONG));

            ctsu_transfer_ctsuso_set(1, DTC_CHAIN_TRANSFER_DISABLE, DTC_CHAIN_TRANSFER_CONTINUOUSLY, calc_transfer_count);
            ctsu_transfer_address_set(1, (uint32_t *)p_instance_ctrl->p_ctsuwr, (uint32_t *)&(CTSU.CTSUSO.LONG));

            ien_bk = IEN(CTSU,CTSUWR);
            IEN(CTSU,CTSUWR) = 0U;

            dtc_err = R_DTC_Create(g_ctsu_dtc_cmd_arg.act_src, g_transfer_data_tx, g_ctsu_dtc_info, 1);

            IEN(CTSU,CTSUWR) = ien_bk;

            R_DTC_Control(DTC_CMD_ACT_SRC_ENABLE, NULL, &g_ctsu_dtc_cmd_arg);

            return dtc_err;
        }
  #endif
    }

    ien_bk = IEN(CTSU,CTSUWR);
    IEN(CTSU,CTSUWR) = 0U;

    dtc_err = R_DTC_Create(g_ctsu_dtc_cmd_arg.act_src, g_transfer_data_tx, g_ctsu_dtc_info, 0);

    IEN(CTSU,CTSUWR) = ien_bk;

    R_DTC_Control(DTC_CMD_ACT_SRC_ENABLE, NULL, &g_ctsu_dtc_cmd_arg);

    return dtc_err;
}

/***********************************************************************************************************************
 * ctsu_transfer_normal_ctsurd
 ***********************************************************************************************************************/
dtc_err_t ctsu_transfer_normal_ctsurd (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    dtc_err_t dtc_err;
    uint8_t   ien_bk;
    uint32_t  calc_transfer_count;

    calc_transfer_count = p_instance_ctrl->num_elements * CTSU_CFG_NUM_SUMULTI;

    g_ctsu_dtc_cmd_arg.act_src = (dtc_activation_source_t)IR_CTSU_CTSURD;

    R_DTC_Control(DTC_CMD_ACT_SRC_DISABLE, NULL, &g_ctsu_dtc_cmd_arg);

    ctsu_transfer_ctsuscnt_set(0, DTC_CHAIN_TRANSFER_DISABLE, DTC_CHAIN_TRANSFER_CONTINUOUSLY, calc_transfer_count);
    ctsu_transfer_address_set(0, (uint32_t *)&(CTSU.CTSUSCNT.LONG), (uint32_t *)p_instance_ctrl->p_self_raw);

    if (CTSU_CORRECTION_RUN == g_ctsu_correction_info.status)
    {
        g_ctsu_dtc_info[0].transfer_count   = 1;
        g_ctsu_dtc_info[0].dest_addr        = (uint32_t)&g_ctsu_correction_info.scanbuf;
    }

  #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
    else if (CTSU_MODE_DIAGNOSIS_SCAN == p_instance_ctrl->md)
    {
        g_ctsu_dtc_info[0].data_size        = DTC_DATA_SIZE_LWORD;
        g_ctsu_dtc_info[0].block_size       = 1;
        if (CTSU_DIAG_CLOCK_RECOVERY == g_ctsu_diag_info.state)
        {
            g_ctsu_dtc_info[0].dest_addr_mode    = DTC_DES_ADDR_INCR;
            g_ctsu_dtc_info[0].transfer_count    = 3;
        }
        else
        {
            g_ctsu_dtc_info[0].transfer_count    = 1;
        }

        g_ctsu_dtc_info[0].dest_addr        = (uint32_t)&(g_ctsu_diag_info.ctsuscnt[0]);
        g_ctsu_dtc_info[0].source_addr      = (uint32_t)&(CTSU.CTSUSCNT.LONG);
    }
  #endif
    else
    {
        if (CTSU_MODE_CURRENT_SCAN == p_instance_ctrl->md)
        {
            g_ctsu_dtc_info[0].transfer_count    = p_instance_ctrl->num_elements;
        }
  #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
        if (CTSU_MODE_MUTUAL_FULL_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_instance_ctrl->md))
        {
            g_ctsu_dtc_info[0].dest_addr        = (uint32_t)p_instance_ctrl->p_mutual_raw;
            g_ctsu_dtc_info[0].transfer_count   = (uint16_t) (g_ctsu_dtc_info[0].transfer_count * 2);
        }
  #endif
    }

    ien_bk = IEN(CTSU,CTSUWR);
    IEN(CTSU,CTSUWR) = 0U;

    dtc_err = R_DTC_Create(g_ctsu_dtc_cmd_arg.act_src, g_transfer_data_rx, g_ctsu_dtc_info, 0);

    IEN(CTSU,CTSUWR) = ien_bk;

    R_DTC_Control(DTC_CMD_ACT_SRC_ENABLE, NULL, &g_ctsu_dtc_cmd_arg);

    return dtc_err;
}

  #if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
/***********************************************************************************************************************
 * ctsu_transfer_autojudge
 ***********************************************************************************************************************/
dtc_err_t ctsu_transfer_autojudge (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    dtc_err_t dtc_err;

    dtc_err = ctsu_transfer_autojudge_ctsuwr(p_instance_ctrl);

    dtc_err = ctsu_transfer_autojudge_ctsurd(p_instance_ctrl);

    return dtc_err;
}

/***********************************************************************************************************************
 * ctsu_transfer_autojudge_ctsuwr
 ***********************************************************************************************************************/
dtc_err_t ctsu_transfer_autojudge_ctsuwr (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    dtc_err_t dtc_err;
    uint8_t   ien_bk;
    uint32_t  calc_transfer_count;

    g_ctsu_dtc_cmd_arg.act_src = (dtc_activation_source_t)IR_CTSU_CTSUWR;

    R_DTC_Control(DTC_CMD_ACT_SRC_DISABLE, NULL, &g_ctsu_dtc_cmd_arg);

   #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
    if (1 == p_instance_ctrl->p_ctsu_cfg->majirimd)
    {
        calc_transfer_count = p_instance_ctrl->num_elements;

        /* For automatic judgement register transfer */
        ctsu_transfer_autojudge_value_set(0, DTC_REPEAT_BLOCK_DESTINATION, calc_transfer_count);
        ctsu_transfer_address_set(0, (uint32_t *)p_instance_ctrl->p_auto_judge, (uint32_t *)&(CTSU.CTSUAJTHR.LONG));

        /* For CTSUMCACT1 register transfer */
        ctsu_transfer_mcact_set(1, calc_transfer_count);
        ctsu_transfer_address_set(1, (uint32_t *)p_instance_ctrl->p_mcact1, (uint32_t *)&(CTSU.CTSUMCACT1.LONG));

        /* For CTSUMCACT2 register transfer */
        ctsu_transfer_mcact_set(2, calc_transfer_count);
        ctsu_transfer_address_set(2, (uint32_t *)p_instance_ctrl->p_mcact2, (uint32_t *)&(CTSU.CTSUMCACT2.LONG));

        /* For CTSUSO register transfer */
        ctsu_transfer_ctsuso_set(3, DTC_CHAIN_TRANSFER_ENABLE, DTC_CHAIN_TRANSFER_NORMAL, calc_transfer_count);
        ctsu_transfer_address_set(3, (uint32_t *)p_instance_ctrl->p_ctsuwr, (uint32_t *)&(CTSU.CTSUSO.LONG));

        /* For transfer of the start address of variables for automatic judgement */
        ctsu_transfer_autojudge_repeat_set(4, DTC_CHAIN_TRANSFER_ENABLE, DTC_REPEAT_BLOCK_DESTINATION);
        ctsu_transfer_address_set(4, (uint32_t *)&p_instance_ctrl->adress_auto_judge, (uint32_t *)&g_transfer_data_tx[0].lw2);

        /* Number of transfers in the automatic judgement register For forwarding */
        ctsu_transfer_autojudge_repeat_set(5, DTC_CHAIN_TRANSFER_ENABLE, DTC_REPEAT_BLOCK_DESTINATION);
        ctsu_transfer_address_set(5, (uint32_t *)&p_instance_ctrl->count_auto_judge, (uint32_t *)&g_transfer_data_tx[0].lw4);

        /* For transfer of the start address of variables for CTSUMCACT1 */
        ctsu_transfer_autojudge_repeat_set(6, DTC_CHAIN_TRANSFER_ENABLE, DTC_REPEAT_BLOCK_DESTINATION);
        ctsu_transfer_address_set(6, (uint32_t *)&p_instance_ctrl->adress_mcact1, (uint32_t *)&g_transfer_data_tx[1].lw2);

        /* Number of transfers of CTSUMCACT1 register For forwarding */
        ctsu_transfer_autojudge_repeat_set(7, DTC_CHAIN_TRANSFER_ENABLE, DTC_REPEAT_BLOCK_DESTINATION);
        ctsu_transfer_address_set(7, (uint32_t *)&p_instance_ctrl->count_mcact1, (uint32_t *)&g_transfer_data_tx[1].lw4);

        /* For transfer of the start address of variables for CTSUMCACT2 */
        ctsu_transfer_autojudge_repeat_set(8, DTC_CHAIN_TRANSFER_ENABLE, DTC_REPEAT_BLOCK_DESTINATION);
        ctsu_transfer_address_set(8, (uint32_t *)&p_instance_ctrl->adress_mcact2, (uint32_t *)&g_transfer_data_tx[2].lw2);

        /* Number of transfers of CTSUMCACT2 register For forwarding */
        ctsu_transfer_autojudge_repeat_set(9, DTC_CHAIN_TRANSFER_ENABLE, DTC_REPEAT_BLOCK_DESTINATION);
        ctsu_transfer_address_set(9, (uint32_t *)&p_instance_ctrl->count_mcact2, (uint32_t *)&g_transfer_data_tx[2].lw4);

        /* For transfer of the start address of variables for CTSUSO */
        ctsu_transfer_autojudge_repeat_set(10, DTC_CHAIN_TRANSFER_ENABLE, DTC_REPEAT_BLOCK_DESTINATION);
        ctsu_transfer_address_set(10, (uint32_t *)&p_instance_ctrl->adress_ctsuwr, (uint32_t *)&g_transfer_data_tx[3].lw2);

        /* Number of transfers of CTSUSO register For forwarding */
        ctsu_transfer_autojudge_repeat_set(11, DTC_CHAIN_TRANSFER_DISABLE, DTC_REPEAT_BLOCK_DESTINATION);
        ctsu_transfer_address_set(11, (uint32_t *)&p_instance_ctrl->count_ctsuwr_self_mutual, (uint32_t *)&g_transfer_data_tx[3].lw4);

        ien_bk = IEN(CTSU,CTSUWR);
        IEN(CTSU,CTSUWR) = 0U;

        dtc_err = R_DTC_Create(g_ctsu_dtc_cmd_arg.act_src, g_transfer_data_tx, g_ctsu_dtc_info, 11);
    }
    else
   #endif
    {
        calc_transfer_count = p_instance_ctrl->num_elements * CTSU_CFG_NUM_SUMULTI;

        /* For automatic judgement register transfer */
        ctsu_transfer_autojudge_value_set(0, DTC_REPEAT_BLOCK_DESTINATION, calc_transfer_count);
        ctsu_transfer_address_set(0, (uint32_t *)p_instance_ctrl->p_auto_judge, (uint32_t *)&(CTSU.CTSUAJTHR.LONG));

        /* For CTSUSO register transfer */
        ctsu_transfer_ctsuso_set(1, DTC_CHAIN_TRANSFER_ENABLE, DTC_CHAIN_TRANSFER_NORMAL, calc_transfer_count);
        ctsu_transfer_address_set(1, (uint32_t *)p_instance_ctrl->p_ctsuwr, (uint32_t *)&(CTSU.CTSUSO.LONG));

        /* For transfer of the start address of variables for automatic judgement */
        ctsu_transfer_autojudge_repeat_set(2, DTC_CHAIN_TRANSFER_ENABLE, DTC_REPEAT_BLOCK_DESTINATION);
        ctsu_transfer_address_set(2, (uint32_t *)&p_instance_ctrl->adress_auto_judge, (uint32_t *)&g_transfer_data_tx[0].lw2);

        /* Number of transfers in the automatic judgement register For forwarding */
        ctsu_transfer_autojudge_repeat_set(3, DTC_CHAIN_TRANSFER_ENABLE, DTC_REPEAT_BLOCK_DESTINATION);
        ctsu_transfer_address_set(3, (uint32_t *)&p_instance_ctrl->count_auto_judge, (uint32_t *)&g_transfer_data_tx[0].lw4);

        /* For transfer of the start address of variables for CTSUSO */
        ctsu_transfer_autojudge_repeat_set(4, DTC_CHAIN_TRANSFER_ENABLE, DTC_REPEAT_BLOCK_DESTINATION);
        ctsu_transfer_address_set(4, (uint32_t *)&p_instance_ctrl->adress_ctsuwr, (uint32_t *)&g_transfer_data_tx[1].lw2);

        /* Number of transfers of CTSUSO register For forwarding */
        ctsu_transfer_autojudge_repeat_set(5, DTC_CHAIN_TRANSFER_DISABLE, DTC_REPEAT_BLOCK_DESTINATION);
        ctsu_transfer_address_set(5, (uint32_t *)&p_instance_ctrl->count_ctsuwr_self_mutual, (uint32_t *)&g_transfer_data_tx[1].lw4);

        ien_bk = IEN(CTSU,CTSUWR);
        IEN(CTSU,CTSUWR) = 0U;

        dtc_err = R_DTC_Create(g_ctsu_dtc_cmd_arg.act_src, g_transfer_data_tx, g_ctsu_dtc_info, 5);
    }

    IEN(CTSU,CTSUWR) = ien_bk;

    R_DTC_Control(DTC_CMD_ACT_SRC_ENABLE, NULL, &g_ctsu_dtc_cmd_arg);

    return dtc_err;
}

/***********************************************************************************************************************
 * ctsu_transfer_autojudge_ctsurd
 ***********************************************************************************************************************/
dtc_err_t ctsu_transfer_autojudge_ctsurd (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    dtc_err_t dtc_err;
    uint8_t   ien_bk;
    uint32_t  calc_transfer_count;

    g_ctsu_dtc_cmd_arg.act_src = (dtc_activation_source_t)IR_CTSU_CTSURD;

    R_DTC_Control(DTC_CMD_ACT_SRC_DISABLE, NULL, &g_ctsu_dtc_cmd_arg);

   #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
    if (1 == p_instance_ctrl->p_ctsu_cfg->majirimd)
    {
        calc_transfer_count = p_instance_ctrl->num_elements;
    }
    else
   #endif
    {
        calc_transfer_count = p_instance_ctrl->num_elements * CTSU_CFG_NUM_SUMULTI;
    }

    /* For automatic judgement register transfer */
    ctsu_transfer_autojudge_value_set(0, DTC_REPEAT_BLOCK_SOURCE, calc_transfer_count);
    ctsu_transfer_address_set(0, (uint32_t *)&(CTSU.CTSUAJTHR.LONG), (uint32_t *)p_instance_ctrl->p_auto_judge);

    /* For CTSUSCNT register transfer */
    ctsu_transfer_ctsuscnt_set(1, DTC_CHAIN_TRANSFER_ENABLE, DTC_CHAIN_TRANSFER_NORMAL, calc_transfer_count);
    ctsu_transfer_address_set(1, (uint32_t *)&(CTSU.CTSUSCNT.LONG), (uint32_t *)p_instance_ctrl->p_self_raw);
   #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
    if (CTSU_MODE_MUTUAL_FULL_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_instance_ctrl->md))
    {
        g_ctsu_dtc_info[1].dest_addr        = (uint32_t)p_instance_ctrl->p_mutual_raw;
    }
   #endif

    /* For transfer of the start address of variables for automatic judgement */
    ctsu_transfer_autojudge_repeat_set(2, DTC_CHAIN_TRANSFER_ENABLE, DTC_REPEAT_BLOCK_SOURCE);
    ctsu_transfer_address_set(2, (uint32_t *)&p_instance_ctrl->adress_auto_judge, (uint32_t *)&g_transfer_data_rx[0].lw3);

    /* Number of transfers in the automatic judgement register For forwarding */
    ctsu_transfer_autojudge_repeat_set(3, DTC_CHAIN_TRANSFER_ENABLE, DTC_REPEAT_BLOCK_SOURCE);
    ctsu_transfer_address_set(3, (uint32_t *)&p_instance_ctrl->count_auto_judge, (uint32_t *)&g_transfer_data_rx[0].lw4);

    /* For transfer of the start address of variables for CTSUSCNT */
    ctsu_transfer_autojudge_repeat_set(4, DTC_CHAIN_TRANSFER_ENABLE, DTC_REPEAT_BLOCK_SOURCE);
    ctsu_transfer_address_set(4, (uint32_t *)&p_instance_ctrl->adress_self_raw, (uint32_t *)&g_transfer_data_rx[1].lw3);
   #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
    if (CTSU_MODE_MUTUAL_FULL_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_instance_ctrl->md))
    {
        g_ctsu_dtc_info[4].source_addr        = (uint32_t)&p_instance_ctrl->adress_mutual_raw;
    }
   #endif

    /* Number of transfers of CTSUSCNT register For forwarding */
    ctsu_transfer_autojudge_repeat_set(5, DTC_CHAIN_TRANSFER_DISABLE, DTC_REPEAT_BLOCK_SOURCE);
    ctsu_transfer_address_set(5, (uint32_t *)&p_instance_ctrl->count_ctsuwr_self_mutual, (uint32_t *)&g_transfer_data_rx[1].lw4);

    ien_bk = IEN(CTSU,CTSUWR);
    IEN(CTSU,CTSUWR) = 0U;

    dtc_err = R_DTC_Create(g_ctsu_dtc_cmd_arg.act_src, g_transfer_data_rx, g_ctsu_dtc_info, 5);

    IEN(CTSU,CTSUWR) = ien_bk;

    R_DTC_Control(DTC_CMD_ACT_SRC_ENABLE, NULL, &g_ctsu_dtc_cmd_arg);

    return dtc_err;
}
  #endif
 #endif

 #if (BSP_FEATURE_CTSU_VERSION == 1)
/***********************************************************************************************************************
 * ctsu_transfer_ctsu1
 ***********************************************************************************************************************/
dtc_err_t ctsu_transfer_ctsu1 (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    dtc_err_t dtc_err;

    dtc_err = ctsu_transfer_ctsu1_ctsuwr(p_instance_ctrl);

    dtc_err = ctsu_transfer_ctsu1_ctsurd(p_instance_ctrl);

    return dtc_err;
}

/***********************************************************************************************************************
 * ctsu_transfer_ctsu1_ctsuwr
 ***********************************************************************************************************************/
dtc_err_t ctsu_transfer_ctsu1_ctsuwr (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    dtc_err_t dtc_err;
    uint8_t   ien_bk;
    uint32_t  calc_transfer_count;

    calc_transfer_count = p_instance_ctrl->num_elements * CTSU_CFG_NUM_SUMULTI;

    g_ctsu_dtc_cmd_arg.act_src = (dtc_activation_source_t)IR_CTSU_CTSUWR;

    R_DTC_Control(DTC_CMD_ACT_SRC_DISABLE, NULL, &g_ctsu_dtc_cmd_arg);

    ctsu_transfer_ctsuso_set(0, DTC_CHAIN_TRANSFER_DISABLE, DTC_CHAIN_TRANSFER_CONTINUOUSLY, calc_transfer_count);
    ctsu_transfer_address_set(0, (uint32_t *)p_instance_ctrl->p_ctsuwr, (uint32_t *)&(CTSU.CTSUSSC));

    if (CTSU_CORRECTION_RUN == g_ctsu_correction_info.status)
    {
        g_ctsu_dtc_info[0].transfer_count   = 1;
        g_ctsu_dtc_info[0].source_addr      = (uint32_t)&(g_ctsu_correction_info.ctsuwr);
    }

  #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
    else if (CTSU_MODE_DIAGNOSIS_SCAN == p_instance_ctrl->md)
    {
        g_ctsu_dtc_info[0].transfer_count   = 1;
        g_ctsu_dtc_info[0].source_addr      = (uint32_t)&(g_ctsu_diag_info.ctsuwr);
    }
  #endif
    else if ((CTSU_CORRECTION_RUN != g_ctsu_correction_info.status) &&
             (CTSU_MODE_DIAGNOSIS_SCAN != p_instance_ctrl->md))
    {

  #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
        if (CTSU_MODE_MUTUAL_FULL_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_instance_ctrl->md))
        {
            if (true == p_instance_ctrl->serial_tuning_enable)
            {
                if (0 == ((p_instance_ctrl->ctsucr1 >> 7) & 0x01))
                {
                    g_ctsu_dtc_info[0].transfer_count    = (uint16_t) (p_instance_ctrl->num_elements * 2);
                }
            }
        }
  #endif
    }
    else
    {
    }

    ien_bk = IEN(CTSU,CTSUWR);
    IEN(CTSU,CTSUWR) = 0U;

    dtc_err = R_DTC_Create(g_ctsu_dtc_cmd_arg.act_src, g_transfer_data_tx, g_ctsu_dtc_info, 0);

    IEN(CTSU,CTSUWR) = ien_bk;

    R_DTC_Control(DTC_CMD_ACT_SRC_ENABLE, NULL, &g_ctsu_dtc_cmd_arg);

    return dtc_err;
}

/***********************************************************************************************************************
 * ctsu_transfer_ctsu1_ctsurd
 ***********************************************************************************************************************/
dtc_err_t ctsu_transfer_ctsu1_ctsurd (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    dtc_err_t dtc_err;
    uint8_t   ien_bk;
    uint32_t  calc_transfer_count;

    calc_transfer_count = p_instance_ctrl->num_elements * CTSU_CFG_NUM_SUMULTI;

    g_ctsu_dtc_cmd_arg.act_src = (dtc_activation_source_t)IR_CTSU_CTSURD;

    R_DTC_Control(DTC_CMD_ACT_SRC_DISABLE, NULL, &g_ctsu_dtc_cmd_arg);

    ctsu_transfer_ctsuscnt_set(0, DTC_CHAIN_TRANSFER_DISABLE, DTC_CHAIN_TRANSFER_CONTINUOUSLY, calc_transfer_count);
    ctsu_transfer_address_set(0, (uint32_t *)&(CTSU.CTSUSC.WORD), (uint32_t *)p_instance_ctrl->p_self_raw);

    if (CTSU_CORRECTION_RUN == g_ctsu_correction_info.status)
    {
        g_ctsu_dtc_info[0].transfer_count = 1;
        g_ctsu_dtc_info[0].dest_addr      = (uint32_t)&g_ctsu_correction_info.scanbuf;
    }

  #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
    else if (CTSU_MODE_DIAGNOSIS_SCAN == p_instance_ctrl->md)
    {
        g_ctsu_dtc_info[0].transfer_count    = 1;
        g_ctsu_dtc_info[0].dest_addr        = (uint32_t)&g_ctsu_diag_info.scanbuf;
    }
  #endif
    else if ((CTSU_CORRECTION_RUN != g_ctsu_correction_info.status) &&
             (CTSU_MODE_DIAGNOSIS_SCAN != p_instance_ctrl->md))
    {
  #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
        if (CTSU_MODE_MUTUAL_FULL_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_instance_ctrl->md))
        {
            g_ctsu_dtc_info[0].dest_addr        = (uint32_t)p_instance_ctrl->p_mutual_raw;
            g_ctsu_dtc_info[0].transfer_count    = (uint16_t) (g_ctsu_dtc_info[0].transfer_count * 2);

            if (true == p_instance_ctrl->serial_tuning_enable)
            {
                if (0 == ((p_instance_ctrl->ctsucr1 >> 7) & 0x01))
                {
                    g_ctsu_dtc_info[0].transfer_count    = (uint16_t) (p_instance_ctrl->num_elements * 2);
                }
            }
        }
  #endif
    }
    else
    {
    }

    ien_bk = IEN(CTSU,CTSUWR);
    IEN(CTSU,CTSUWR) = 0U;

    dtc_err = R_DTC_Create(g_ctsu_dtc_cmd_arg.act_src, g_transfer_data_rx, g_ctsu_dtc_info, 0);

    IEN(CTSU,CTSUWR) = ien_bk;

    R_DTC_Control(DTC_CMD_ACT_SRC_ENABLE, NULL, &g_ctsu_dtc_cmd_arg);

    return dtc_err;
}
 #endif

/***********************************************************************************************************************
 * ctsu_transfer_ctsuso_set
 ***********************************************************************************************************************/
void ctsu_transfer_ctsuso_set (uint8_t array_number, dtc_chain_transfer_t set_chain_transfer_enable, dtc_chain_transfer_mode_t set_chain_transfer_mode, uint32_t set_transfer_count)
{
    g_ctsu_dtc_info[array_number].chain_transfer_enable = set_chain_transfer_enable;
    g_ctsu_dtc_info[array_number].chain_transfer_mode   = set_chain_transfer_mode;
    g_ctsu_dtc_info[array_number].src_addr_mode         = DTC_SRC_ADDR_INCR;
 #if (BSP_FEATURE_CTSU_VERSION == 2)
    g_ctsu_dtc_info[array_number].dest_addr_mode        = DTC_DES_ADDR_FIXED;
 #endif
 #if (BSP_FEATURE_CTSU_VERSION == 1)
    g_ctsu_dtc_info[array_number].dest_addr_mode        = DTC_DES_ADDR_INCR;
 #endif
    g_ctsu_dtc_info[array_number].response_interrupt    = DTC_INTERRUPT_AFTER_ALL_COMPLETE;
    g_ctsu_dtc_info[array_number].transfer_mode         = DTC_TRANSFER_MODE_BLOCK;
    g_ctsu_dtc_info[array_number].repeat_block_side     = DTC_REPEAT_BLOCK_DESTINATION;
 #if (BSP_FEATURE_CTSU_VERSION == 2)
    g_ctsu_dtc_info[array_number].data_size             = DTC_DATA_SIZE_LWORD;
 #endif
 #if (BSP_FEATURE_CTSU_VERSION == 1)
    g_ctsu_dtc_info[array_number].data_size             = DTC_DATA_SIZE_WORD;
 #endif
 #if (BSP_FEATURE_CTSU_VERSION == 2)
    g_ctsu_dtc_info[array_number].block_size            = 1;
 #endif
 #if (BSP_FEATURE_CTSU_VERSION == 1)
    g_ctsu_dtc_info[array_number].block_size            = 3;
 #endif
    g_ctsu_dtc_info[array_number].transfer_count        = set_transfer_count;
}

/***********************************************************************************************************************
 * ctsu_transfer_ctsuscnt_set
 ***********************************************************************************************************************/
void ctsu_transfer_ctsuscnt_set (uint8_t array_number, dtc_chain_transfer_t set_chain_transfer_enable, dtc_chain_transfer_mode_t set_chain_transfer_mode, uint32_t set_transfer_count)
{
    g_ctsu_dtc_info[array_number].chain_transfer_enable = set_chain_transfer_enable;
    g_ctsu_dtc_info[array_number].chain_transfer_mode   = set_chain_transfer_mode;
 #if (BSP_FEATURE_CTSU_VERSION == 2)
    g_ctsu_dtc_info[array_number].src_addr_mode         = DTC_SRC_ADDR_FIXED;
 #endif
 #if (BSP_FEATURE_CTSU_VERSION == 1)
    g_ctsu_dtc_info[array_number].src_addr_mode         = DTC_SRC_ADDR_INCR;
 #endif
    g_ctsu_dtc_info[array_number].dest_addr_mode        = DTC_DES_ADDR_INCR;
    g_ctsu_dtc_info[array_number].response_interrupt    = DTC_INTERRUPT_AFTER_ALL_COMPLETE;
    g_ctsu_dtc_info[array_number].transfer_mode         = DTC_TRANSFER_MODE_BLOCK;
    g_ctsu_dtc_info[array_number].repeat_block_side     = DTC_REPEAT_BLOCK_SOURCE;
    g_ctsu_dtc_info[array_number].data_size             = DTC_DATA_SIZE_WORD;
 #if (BSP_FEATURE_CTSU_VERSION == 2)
    g_ctsu_dtc_info[array_number].block_size            = 1;
 #endif
 #if (BSP_FEATURE_CTSU_VERSION == 1)
    g_ctsu_dtc_info[array_number].block_size            = 2;
 #endif
    g_ctsu_dtc_info[array_number].transfer_count        = set_transfer_count;
}

/***********************************************************************************************************************
 * ctsu_transfer_address_set
 ***********************************************************************************************************************/
void ctsu_transfer_address_set (uint8_t array_number, uint32_t * p_set_source_addr, uint32_t * p_set_dest_addr)
{
    g_ctsu_dtc_info[array_number].source_addr = (uint32_t)p_set_source_addr;
    g_ctsu_dtc_info[array_number].dest_addr   = (uint32_t)p_set_dest_addr;
}

 #if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
/***********************************************************************************************************************
* ctsu_transfer_autojudge_value_set
***********************************************************************************************************************/
void ctsu_transfer_autojudge_value_set (uint8_t array_number, dtc_repeat_block_side_t set_repeat_block_side, uint32_t set_transfer_count)
{
   g_ctsu_dtc_info[array_number].chain_transfer_enable = DTC_CHAIN_TRANSFER_ENABLE;
   g_ctsu_dtc_info[array_number].chain_transfer_mode   = DTC_CHAIN_TRANSFER_CONTINUOUSLY;
   g_ctsu_dtc_info[array_number].src_addr_mode         = DTC_SRC_ADDR_INCR;
   g_ctsu_dtc_info[array_number].dest_addr_mode        = DTC_DES_ADDR_INCR;
   g_ctsu_dtc_info[array_number].response_interrupt    = DTC_INTERRUPT_AFTER_ALL_COMPLETE;
   g_ctsu_dtc_info[array_number].transfer_mode         = DTC_TRANSFER_MODE_BLOCK;
   g_ctsu_dtc_info[array_number].repeat_block_side     = set_repeat_block_side;
   g_ctsu_dtc_info[array_number].data_size             = DTC_DATA_SIZE_LWORD;
   g_ctsu_dtc_info[array_number].block_size            = 5;
   g_ctsu_dtc_info[array_number].transfer_count        = set_transfer_count;
}

/***********************************************************************************************************************
 * ctsu_transfer_autojudge_repeat_set
 ***********************************************************************************************************************/
void ctsu_transfer_autojudge_repeat_set (uint8_t array_number, dtc_chain_transfer_t set_chain_transfer_enable, dtc_repeat_block_side_t set_repeat_block_side)
{
    g_ctsu_dtc_info[array_number].chain_transfer_enable = set_chain_transfer_enable;
    g_ctsu_dtc_info[array_number].chain_transfer_mode   = DTC_CHAIN_TRANSFER_CONTINUOUSLY;
    g_ctsu_dtc_info[array_number].src_addr_mode         = DTC_SRC_ADDR_FIXED;
    g_ctsu_dtc_info[array_number].dest_addr_mode        = DTC_DES_ADDR_FIXED;
    g_ctsu_dtc_info[array_number].response_interrupt    = DTC_INTERRUPT_AFTER_ALL_COMPLETE;
    g_ctsu_dtc_info[array_number].transfer_mode         = DTC_TRANSFER_MODE_REPEAT;
    g_ctsu_dtc_info[array_number].repeat_block_side     = set_repeat_block_side;
    g_ctsu_dtc_info[array_number].data_size             = DTC_DATA_SIZE_LWORD;
    g_ctsu_dtc_info[array_number].block_size            = 1;
    g_ctsu_dtc_info[array_number].transfer_count        = 1;
}
 #endif

 #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
/***********************************************************************************************************************
 * ctsu_transfer_mcact_set
 ***********************************************************************************************************************/
void ctsu_transfer_mcact_set (uint8_t array_number, uint32_t set_transfer_count)
{
    g_ctsu_dtc_info[array_number].chain_transfer_enable = DTC_CHAIN_TRANSFER_ENABLE;
    g_ctsu_dtc_info[array_number].chain_transfer_mode   = DTC_CHAIN_TRANSFER_CONTINUOUSLY;
    g_ctsu_dtc_info[array_number].src_addr_mode         = DTC_SRC_ADDR_INCR;
    g_ctsu_dtc_info[array_number].dest_addr_mode        = DTC_DES_ADDR_FIXED;
    g_ctsu_dtc_info[array_number].response_interrupt    = DTC_INTERRUPT_AFTER_ALL_COMPLETE;
    g_ctsu_dtc_info[array_number].transfer_mode         = DTC_TRANSFER_MODE_BLOCK;
    g_ctsu_dtc_info[array_number].repeat_block_side     = DTC_REPEAT_BLOCK_DESTINATION;
    g_ctsu_dtc_info[array_number].data_size             = DTC_DATA_SIZE_LWORD;
    g_ctsu_dtc_info[array_number].block_size            = 1;
    g_ctsu_dtc_info[array_number].transfer_count        = set_transfer_count;
}
 #endif
#endif

/***********************************************************************************************************************
 * ctsu_initial_offset_tuning
 ***********************************************************************************************************************/
void ctsu_initial_offset_tuning (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    uint16_t element_id;
    int32_t  diff          = 0;
    uint32_t complete_flag = 0;
    uint32_t num_complete  = 0;
    uint16_t target_val;
#if (BSP_FEATURE_CTSU_VERSION == 1)
    uint16_t               ctsuso;
    ctsu_correction_calc_t calc;
#endif
#if (BSP_FEATURE_CTSU_VERSION == 2)
    uint16_t i;
    uint16_t element_top;
    uint16_t corr_data[CTSU_CFG_NUM_SUMULTI];
    int32_t  ctsuso;
    uint32_t snum;
    int32_t  offset_unit;
#endif

    /* element_id through each element for control block */
    for (element_id = 0; element_id < p_instance_ctrl->num_elements; element_id++)
    {
        if (0 == *(p_instance_ctrl->p_element_complete_flag + element_id))
        {
#if (BSP_FEATURE_CTSU_VERSION == 1)
            if (CTSU_MODE_SELF_MULTI_SCAN == p_instance_ctrl->md)
            {
                target_val = (p_instance_ctrl->tuning_self_target_value);
            }
            else
            {
                target_val = (p_instance_ctrl->tuning_mutual_target_value);
            }

            calc.snum  = (p_instance_ctrl->p_ctsuwr[element_id].ctsuso0 >> 10) & CTSU_SNUM_MAX;
            calc.sdpa  = (p_instance_ctrl->p_ctsuwr[element_id].ctsuso1 >> 8) & CTSU_SDPA_MAX;
            target_val = (uint16_t) (target_val *
                                     (uint32_t) ((calc.snum + 1) * (calc.sdpa + 1)) /
                                     g_ctsu_correction_info.ctsu_clock);
 #if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
            if (CTSU_MODE_SELF_MULTI_SCAN == p_instance_ctrl->md)
            {
                diff = (int32_t)(p_instance_ctrl->p_self_data + element_id)->int_data - (int32_t)target_val;
            }
 #endif
 #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
            if (CTSU_MODE_MUTUAL_FULL_SCAN == p_instance_ctrl->md)
            {
                diff = (int32_t)(p_instance_ctrl->p_mutual_pri_data + element_id)->int_data - (int32_t)target_val;
            }
 #endif
            ctsuso = (p_instance_ctrl->p_ctsuwr[element_id].ctsuso0 & CTSU_TUNING_MAX);
            if (0 < diff)
            {
                if (*(p_instance_ctrl->p_tuning_diff + element_id) < 0)
                {
                    if ((-diff) > *(p_instance_ctrl->p_tuning_diff + element_id))
                    {
                        ctsuso++;      ///< Decrease count
                    }

                    complete_flag = 1;
                }
                else
                {
                    if (CTSU_TUNING_MAX == ctsuso) /* CTSUSO limit check    */
                    {
                        complete_flag = 1;
                    }
                    else
                    {
                        ctsuso++;                                                ///< Decrease count
                        (*(p_instance_ctrl->p_tuning_diff + element_id)) = diff; ///< Plus
                    }
                }
            }
            else if (0 == diff)
            {
                complete_flag = 1;
            }
            else
            {
                if (*(p_instance_ctrl->p_tuning_diff + element_id) > 0)
                {
                    if ((-diff) > *(p_instance_ctrl->p_tuning_diff + element_id))
                    {
                        ctsuso--;      ///< Increase count
                    }

                    complete_flag = 1;
                }
                else
                {
                    if (CTSU_TUNING_MIN == ctsuso) /* CTSUSO limit check    */
                    {
                        complete_flag = 1;
                    }
                    else
                    {
                        ctsuso--;                                                ///< Increase count
                        (*(p_instance_ctrl->p_tuning_diff + element_id)) = diff; ///< Minus
                    }
                }
            }

            p_instance_ctrl->p_ctsuwr[element_id].ctsuso0 &= (uint16_t) (~CTSU_TUNING_MAX);
            p_instance_ctrl->p_ctsuwr[element_id].ctsuso0 |= ctsuso;
#endif
#if (BSP_FEATURE_CTSU_VERSION == 2)
            element_top = (uint16_t) (element_id * CTSU_CFG_NUM_SUMULTI);
            for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
            {
                /* Adjust only frequencies for which offset tuning is not completed */
                if (0 == (p_instance_ctrl->p_frequency_complete_flag[element_id] & (1 << i)))
                {
                    if (CTSU_MODE_SELF_MULTI_SCAN == p_instance_ctrl->md)
                    {
                        target_val = (p_instance_ctrl->tuning_self_target_value / 2);
                    }
                    else
                    {
                        target_val = (p_instance_ctrl->tuning_mutual_target_value / 2);
                    }

                    if (CTSU_MODE_SELF_MULTI_SCAN == p_instance_ctrl->md)
                    {
                        corr_data[i] = p_instance_ctrl->p_self_corr[element_top + i];
                    }
                    else
                    {
                        corr_data[i] = p_instance_ctrl->p_mutual_pri_corr[element_top + i];
                    }

                    snum = (p_instance_ctrl->p_ctsuwr[(element_id * CTSU_CFG_NUM_SUMULTI)].ctsuso >> 10) &
                           CTSU_SNUM_MAX;
                    target_val  = (uint16_t) (((uint32_t) target_val * (snum + 1)) / (CTSU_SNUM_RECOMMEND + 1));
                    offset_unit = (int32_t) ((CTSU_CORRECTION_OFFSET_UNIT * (snum + 1)) / (CTSU_SNUM_RECOMMEND + 1));

                    /* Calculate CTSUSO equivalent difference between current value and target value */
                    diff = (int32_t) ((int32_t) corr_data[i] - (int32_t) target_val) /
                           (offset_unit >> p_instance_ctrl->range);

                    ctsuso  = (int32_t) (p_instance_ctrl->p_ctsuwr[element_top + i].ctsuso & CTSU_TUNING_MAX);
                    ctsuso += diff;

                    /* If the CTSUSO exceeds the minimum value or the maximum value, tuning complete */
                    if (ctsuso < 0)
                    {
                        ctsuso = 0;
                        p_instance_ctrl->p_frequency_complete_flag[element_id] += (uint8_t) (1 << i);
                    }
                    else if (ctsuso > CTSU_TUNING_MAX)
                    {
                        ctsuso = CTSU_TUNING_MAX;
                        p_instance_ctrl->p_frequency_complete_flag[element_id] += (uint8_t) (1 << i);
                    }
                    else
                    {
                        /* If the difference is large, tuning value may not be able to match, so create the next opportunity */
                        if (0 == diff)
                        {
                            p_instance_ctrl->p_frequency_complete_flag[element_id] += (uint8_t) (1 << i);
                        }
                    }

                    /* Set the result of the calculated CTSUSO */
                    p_instance_ctrl->p_ctsuwr[element_top + i].ctsuso &= (uint32_t) (~CTSU_TUNING_MAX);
                    p_instance_ctrl->p_ctsuwr[element_top + i].ctsuso |= (uint32_t) ctsuso;
                }

                /* Add completion status for each frequency */
                complete_flag += ((p_instance_ctrl->p_frequency_complete_flag[element_id] >> i) & 1);
            }
#endif
        }
        else
        {
            complete_flag = CTSU_CFG_NUM_SUMULTI;
        }

        if (CTSU_CFG_NUM_SUMULTI == complete_flag)
        {
            num_complete++;
            *(p_instance_ctrl->p_element_complete_flag + element_id) = 1;
        }

        complete_flag = 0;
    }

    if (num_complete == p_instance_ctrl->num_elements)
    {
        p_instance_ctrl->tuning = CTSU_TUNING_COMPLETE;
    }
}

/***********************************************************************************************************************
 * ctsu_moving_average
 ***********************************************************************************************************************/
void ctsu_moving_average (ctsu_data_t * p_average, uint16_t new_data, uint16_t average_num)
{
    uint32_t work;

    work  = (uint32_t)(((uint32_t) p_average->int_data << CTSU_CFG_DECIMAL_POINT) + p_average->decimal_point_data);
    work -= (uint32_t)(work / average_num);
    work += (uint32_t)(((uint32_t) new_data << CTSU_CFG_DECIMAL_POINT) / average_num);

    p_average->int_data           = (uint16_t)(work >> CTSU_CFG_DECIMAL_POINT);
    p_average->decimal_point_data = (uint16_t)(work & 0x0000FFFF);
}

/***********************************************************************************************************************
 * CTSUWR interrupt handler. This service routine sets the tuning for the next element to be scanned by hardware.
 ***********************************************************************************************************************/
R_BSP_PRAGMA_INTERRUPT(ctsu_ctsuwr_isr, VECT(CTSU,CTSUWR))
R_BSP_ATTRIB_INTERRUPT void ctsu_ctsuwr_isr(void);
R_BSP_ATTRIB_INTERRUPT void ctsu_ctsuwr_isr(void)
{
#if (CTSU_CFG_DTC_SUPPORT_ENABLE == 0)
    ctsu_instance_ctrl_t * p_instance_ctrl = gp_ctsu_isr_context;

    /* Write settings for current element */
    if (CTSU_CORRECTION_RUN == g_ctsu_correction_info.status)
    {
 #if (BSP_FEATURE_CTSU_VERSION == 2)
        CTSU.CTSUSO.LONG = g_ctsu_correction_info.ctsuwr.ctsuso;
 #endif
 #if (BSP_FEATURE_CTSU_VERSION == 1)
        CTSU.CTSUSSC.WORD = g_ctsu_correction_info.ctsuwr.ctsussc;
        CTSU.CTSUSO0.WORD = g_ctsu_correction_info.ctsuwr.ctsuso0;
        CTSU.CTSUSO1.WORD = g_ctsu_correction_info.ctsuwr.ctsuso1;
 #endif
    }

 #if (BSP_FEATURE_CTSU_VERSION == 2)
  #if (CTSU_CFG_NUM_CFC != 0)
    else if ((CTSU_CORRECTION_RUN == g_ctsu_corrcfc_info.status))
    {
        R_CTSU->CTSUSO = g_ctsu_corrcfc_info.ctsuwr.ctsuso;
    }
  #endif
 #endif
    else if (CTSU_MODE_DIAGNOSIS_SCAN == p_instance_ctrl->md)
    {
 #if (BSP_FEATURE_CTSU_VERSION == 2)
  #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
        CTSU.CTSUSO.LONG = g_ctsu_diag_info.ctsuwr.ctsuso;
        p_instance_ctrl->wr_index++;
  #endif
 #endif
 #if (BSP_FEATURE_CTSU_VERSION == 1)
  #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
        CTSU.CTSUSSC.WORD = g_ctsu_diag_info.ctsuwr.ctsussc;
        CTSU.CTSUSO0.WORD = g_ctsu_diag_info.ctsuwr.ctsuso0;
        CTSU.CTSUSO1.WORD = g_ctsu_diag_info.ctsuwr.ctsuso1;
  #endif
 #endif
    }
    else
    {
 #if (BSP_FEATURE_CTSU_VERSION == 2)
        CTSU.CTSUSO.LONG = p_instance_ctrl->p_ctsuwr[p_instance_ctrl->wr_index].ctsuso;
 #endif
 #if (BSP_FEATURE_CTSU_VERSION == 1)
        CTSU.CTSUSSC.WORD = p_instance_ctrl->p_ctsuwr[p_instance_ctrl->wr_index].ctsussc;
        CTSU.CTSUSO0.WORD = p_instance_ctrl->p_ctsuwr[p_instance_ctrl->wr_index].ctsuso0;
        CTSU.CTSUSO1.WORD = p_instance_ctrl->p_ctsuwr[p_instance_ctrl->wr_index].ctsuso1;
 #endif
        p_instance_ctrl->wr_index++;
    }
#endif
}

/***********************************************************************************************************************
 * CTSURD interrupt handler. This service routine reads the sensor count and reference counter for
 * the current element and places the value in the scan data buffer. Note that the reference counter
 * does not work properly but is saved anyway for backward compatibility and potential future use.
 * Additionally, the SC register cannot be read again until RC is read.
 ***********************************************************************************************************************/
R_BSP_PRAGMA_INTERRUPT(ctsu_ctsurd_isr, VECT(CTSU,CTSURD))
R_BSP_ATTRIB_INTERRUPT void ctsu_ctsurd_isr(void);
R_BSP_ATTRIB_INTERRUPT void ctsu_ctsurd_isr(void)
{
#if (CTSU_CFG_DTC_SUPPORT_ENABLE == 0)
    ctsu_instance_ctrl_t * p_instance_ctrl = gp_ctsu_isr_context;

    /* read current channel/element value */
    /* Store the reference counter for possible future use. Register must be read or scan will hang. */

 #if (BSP_FEATURE_CTSU_VERSION == 1)
    if (CTSU_CORRECTION_RUN == g_ctsu_correction_info.status)
    {
        g_ctsu_correction_info.scanbuf.sen = CTSU.CTSUSC.WORD;
        g_ctsu_correction_info.scanbuf.ref = CTSU.CTSURC.WORD;
    }

  #if (BSP_FEATURE_CTSU_VERSION == 1)
   #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
    else if (CTSU_MODE_DIAGNOSIS_SCAN == p_instance_ctrl->md)
    {
        g_ctsu_diag_info.scanbuf.sen = CTSU.CTSUSC.WORD;
        g_ctsu_diag_info.scanbuf.ref = CTSU.CTSURC.WORD;
    }
   #endif
  #endif
    else
    {
  #if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
        if (CTSU_MODE_SELF_MULTI_SCAN == p_instance_ctrl->md)
        {
            (p_instance_ctrl->p_self_raw + p_instance_ctrl->rd_index)->sen = CTSU.CTSUSC.WORD;
            (p_instance_ctrl->p_self_raw + p_instance_ctrl->rd_index)->ref = CTSU.CTSURC.WORD;
            p_instance_ctrl->rd_index++;
        }
  #endif
  #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
        if (CTSU_MODE_MUTUAL_FULL_SCAN == p_instance_ctrl->md)
        {
            if (false == p_instance_ctrl->serial_tuning_enable)
            {
                if (1 == CTSU.CTSUST.BIT.CTSUPS)
                {
                    (p_instance_ctrl->p_mutual_raw + p_instance_ctrl->rd_index)->pri_sen = CTSU.CTSUSC.WORD;
                    (p_instance_ctrl->p_mutual_raw + p_instance_ctrl->rd_index)->pri_ref = CTSU.CTSURC.WORD;
                }
                else
                {
                    (p_instance_ctrl->p_mutual_raw + p_instance_ctrl->rd_index)->snd_sen = CTSU.CTSUSC.WORD;
                    (p_instance_ctrl->p_mutual_raw + p_instance_ctrl->rd_index)->snd_ref = CTSU.CTSURC.WORD;
                    p_instance_ctrl->rd_index++;
                }
            }
            else
            {
                p_instance_ctrl->serial_tuning_mutual_cnt++;
                if (p_instance_ctrl->serial_tuning_mutual_cnt % 2)
                {
                    (p_instance_ctrl->p_mutual_raw + p_instance_ctrl->rd_index)->pri_sen = CTSU.CTSUSC.WORD;
                    (p_instance_ctrl->p_mutual_raw + p_instance_ctrl->rd_index)->pri_ref = CTSU.CTSURC.WORD;
                }
                else
                {
                    (p_instance_ctrl->p_mutual_raw + p_instance_ctrl->rd_index)->snd_sen = CTSU.CTSUSC.WORD;
                    (p_instance_ctrl->p_mutual_raw + p_instance_ctrl->rd_index)->snd_ref = CTSU.CTSURC.WORD;
                    p_instance_ctrl->rd_index++;
                }
            }
        }
  #endif
    }
 #endif

 #if (BSP_FEATURE_CTSU_VERSION == 2)
  #if (CTSU_CFG_NUM_CFC != 0)
    uint16_t i;
  #endif
    if (CTSU_CORRECTION_RUN == g_ctsu_correction_info.status)
    {
        g_ctsu_correction_info.scanbuf = CTSU.CTSUSCNT.BIT.SC;
    }

  #if (CTSU_CFG_NUM_CFC != 0)
    else if (CTSU_CORRECTION_RUN == g_ctsu_corrcfc_info.status)
    {
        for (i = 0; i < g_ctsu_corrcfc_info.num_ts; i++)
        {
            g_ctsu_corrcfc_info.scanbuf[i] = R_CTSU->CTSUCFCCNT_b.CFCCNT;
        }
    }
  #endif
  #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
    else if (CTSU_MODE_DIAGNOSIS_SCAN == p_instance_ctrl->md)
    {
        if (CTSU_DIAG_CFC == g_ctsu_diag_info.state)
        {
/*            g_ctsu_diag_info.ctsuscnt[p_instance_ctrl->rd_index] = R_CTSU->CTSUCFCCNT; */
            p_instance_ctrl->rd_index++;
        }
        else
        {
            g_ctsu_diag_info.ctsuscnt[p_instance_ctrl->rd_index] = CTSU.CTSUSCNT.LONG;
            p_instance_ctrl->rd_index++;
        }
    }
  #endif
    else
    {
  #if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
        if (CTSU_MODE_SELF_MULTI_SCAN == (CTSU_MODE_MUTUAL_FULL_SCAN & p_instance_ctrl->md))
        {
            p_instance_ctrl->p_self_raw[p_instance_ctrl->rd_index] = CTSU.CTSUSCNT.BIT.SC;
            p_instance_ctrl->rd_index++;

            /* Interim */
            if (0 == p_instance_ctrl->rd_index % 3)
            {
                CTSU.CTSUSR.BIT.MFC = 0;
            }
        }
  #endif
  #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
        if (CTSU_MODE_MUTUAL_FULL_SCAN == p_instance_ctrl->md)
        {
            p_instance_ctrl->p_mutual_raw[p_instance_ctrl->rd_index] = CTSU.CTSUSCNT.BIT.SC;
            p_instance_ctrl->rd_index++;
        }

   #if (CTSU_CFG_NUM_CFC != 0)
        if (CTSU_MODE_MUTUAL_CFC_SCAN == p_instance_ctrl->md)
        {
            for (i = 0; i < p_instance_ctrl->p_ctsu_cfg->num_rx; i++)
            {
                p_instance_ctrl->p_mutual_raw[p_instance_ctrl->rd_index] = R_CTSU->CTSUCFCCNT_b.CFCCNT;
                p_instance_ctrl->rd_index++;
            }
        }
   #endif
  #endif
    }
 #endif
    if (1 == p_instance_ctrl->interrupt_reverse_flag)
    {
        p_instance_ctrl->interrupt_reverse_flag = 0;

        ctsu_end_interrupt(p_instance_ctrl);
    }
#endif
}

/***********************************************************************************************************************
 * CTSUFN interrupt handler. This service routine occurs when all elements have been scanned (finished).
 * The user's callback function is called if available.
 ***********************************************************************************************************************/
R_BSP_PRAGMA_INTERRUPT(ctsu_ctsuend_isr, VECT(CTSU,CTSUFN))
R_BSP_ATTRIB_INTERRUPT void ctsu_ctsuend_isr(void);
R_BSP_ATTRIB_INTERRUPT void ctsu_ctsuend_isr(void)
{
    ctsu_instance_ctrl_t * p_instance_ctrl = gp_ctsu_isr_context;
    uint16_t               rd_index;

    /* In CTSU1 and CTSU2 self-capacity and current measurement mode and diagnostic mode, */
    /* rd_index has the same value as wr_index.                                           */
    rd_index = p_instance_ctrl->rd_index;

#if (BSP_FEATURE_CTSU_VERSION == 1)
    if (CTSU_MODE_MUTUAL_FULL_SCAN == p_instance_ctrl->md)
    {
        if (true == p_instance_ctrl->serial_tuning_enable)
        {
            if (0 == ((p_instance_ctrl->ctsucr1 >> 7) & 0x01))
            {
                rd_index = (uint16_t) (p_instance_ctrl->serial_tuning_mutual_cnt);
            }
        }
    }
#endif

#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
    if (CTSU_MODE_MUTUAL_FULL_SCAN == p_instance_ctrl->md)
    {
        if (true == p_instance_ctrl->serial_tuning_enable)
        {
            if (0 == ((p_instance_ctrl->ctsucr1 >> 7) & 0x01))
            {
                rd_index = p_instance_ctrl->rd_index;
            }
            else
            {
                rd_index = p_instance_ctrl->rd_index / 2;
            }
        }
        else
        {
            /* In the mutual capacity of CTSU2, the value of rd_index is twice the value of wr_index. */
            rd_index = p_instance_ctrl->rd_index / 2;
        }
    }

  #if (CTSU_CFG_NUM_CFC != 0)
    if (CTSU_MODE_MUTUAL_CFC_SCAN == p_instance_ctrl->md)
    {
        if (true == p_instance_ctrl->serial_tuning_enable)
        {
            if (0 == ((p_instance_ctrl->ctsucr1 >> 7) & 0x01))
            {
                rd_index = p_instance_ctrl->rd_index;
            }
            else
            {
                rd_index = (uint16_t) (p_instance_ctrl->rd_index / (p_instance_ctrl->p_ctsu_cfg->num_rx * 2));
            }
        }
        else
        {
            /* In the CFC mutual capacity of CTSU2, rd_index is twice the number of RX terminals in each wr_index. */
            rd_index = (uint16_t) (p_instance_ctrl->rd_index / (p_instance_ctrl->p_ctsu_cfg->num_rx * 2));
        }
    }
  #endif
 #endif
#endif

    /* Countermeasure for the problem that RD interrupt and FN interrupt are reversed. */
    if (rd_index != p_instance_ctrl->wr_index)
    {
        p_instance_ctrl->interrupt_reverse_flag = 1;

        return;
    }

    ctsu_end_interrupt(p_instance_ctrl);
}

void ctsu_end_interrupt (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    ctsu_callback_args_t args;

    /* Store callback arguments in memory provided by user if available.  This allows callback arguments to be
     * stored in non-secure memory so they can be accessed by a non-secure callback function. */
    ctsu_callback_args_t * p_args = p_instance_ctrl->p_callback_memory;
    if (NULL == p_args)
    {
        /* Store on stack */
        p_args = &args;
    }
    else
    {
        /* Save current arguments on the stack in case this is a nested interrupt. */
        args = *p_args;
    }

#if (BSP_FEATURE_CTSU_VERSION == 1)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
    if (CTSU_DIAG_OVER_VOLTAGE == g_ctsu_diag_info.state)
    {
        if (CTSU.CTSUERRS.BIT.CTSUICOMP == 1)
        {
            g_ctsu_diag_info.icomp = 1;
        }
    }
 #endif
#endif

#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
    if (CTSU_MODE_DIAGNOSIS_SCAN == p_instance_ctrl->md)
    {
        if (CTSU_DIAG_OVER_CURRENT == g_ctsu_diag_info.state)
        {
            if (CTSU.CTSUSR.BIT.ICOMP1 == 1)
            {
                g_ctsu_diag_info.icomp1_value = CTSU.CTSUSR.BIT.ICOMP1;
            }
        }
    }
 #endif
#endif

    p_args->event = CTSU_EVENT_SCAN_COMPLETE;

#if (BSP_FEATURE_CTSU_VERSION == 2)
    if (CTSU.CTSUSR.BIT.SCOVF)
    {
        p_args->event  |= CTSU_EVENT_OVERFLOW;
        CTSU.CTSUSR.BIT.SCOVF = 0;
    }

    if ((1 == CTSU.CTSUSR.BIT.ICOMP0) || (1 == CTSU.CTSUSR.BIT.ICOMP1))
    {
        if (CTSU.CTSUSR.BIT.ICOMP0)
        {
            p_args->event |= CTSU_EVENT_ICOMP;
        }
        if (CTSU.CTSUSR.BIT.ICOMP1)
        {
            p_args->event |= CTSU_EVENT_ICOMP1;
        }

        CTSU.CTSUSR.BIT.ICOMPRST = 1;
    }
#endif
#if (BSP_FEATURE_CTSU_VERSION == 1)

    if (CTSU.CTSUST.BIT.CTSUSOVF)
    {
        p_args->event  |= CTSU_EVENT_OVERFLOW;
        CTSU.CTSUST.BIT.CTSUSOVF = 0;
    }

    if (CTSU.CTSUERRS.BIT.CTSUICOMP == 1)
    {
        CTSU.CTSUCR1.BYTE &= (uint8_t) (~0x01);
        R_BSP_NOP();
        R_BSP_NOP();
        CTSU.CTSUCR1.BYTE |= 0x01;
        p_args->event   |= CTSU_EVENT_ICOMP;
    }
#endif
#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_TEMP_CORRECTION_SUPPORT == 1)
    if (CTSU_CORRECTION_RUN != g_ctsu_correction_info.status)
    {
        if (CTSU_MODE_CORRECTION_SCAN == p_instance_ctrl->md)
        {
            CTSU.CTSUCRA.BIT.SDPSEL      = 0;
            CTSU.CTSUSUCLKA.BIT.SUADJ0   = g_ctsu_correction_info.suadj0;
            CTSU.CTSUSUCLKA.BIT.SUMULTI0 = CTSU_CFG_SUMULTI0;
            CTSU.CTSUCRB.BIT.SSCNT       = 1;
            CTSU.CTSUCALIB.BIT.SUCARRY   = 0;
            CTSU.CTSUCALIB.BIT.CCOCALIB  = 0;
            CTSU.CTSUCALIB.BIT.CCOCLK    = 1;
            CTSU.CTSUCALIB.BIT.TSOC      = 0;
            CTSU.CTSUCRA.BIT.SDPSEL      = 1;
        }
    }
 #endif
#endif

#if (CTSU_CFG_MULTIPLE_ELECTRODE_CONNECTION_ENABLE == 1)
    if ((1 == p_instance_ctrl->tsod) && (CTSU_CAP_EXTERNAL == p_instance_ctrl->cap))
    {
        /* When using MEC, MD0 bit is set to single scan mode. */
        CTSU.CTSUMCH.BIT.MCH0 = p_instance_ctrl->mec_ts;

        /* Set MCH1 when using both MEC and Active Shield.*/
        if((0 != p_instance_ctrl->ctsuchtrc0)
                || (0 != p_instance_ctrl->ctsuchtrc1)
                || (0 != p_instance_ctrl->ctsuchtrc2)
                || (0 != p_instance_ctrl->ctsuchtrc3)
                || (0 != p_instance_ctrl->ctsuchtrc4))
        {
            CTSU.CTSUMCH.BIT.MCH1 = p_instance_ctrl->mec_shield_ts;
        }
    }
#endif

    p_instance_ctrl->state        = CTSU_STATE_SCANNED;
    p_instance_ctrl->error_status = args.event;
    p_args->p_context             = p_instance_ctrl->p_context;

    /* If a callback was provided, call it with the argument */
    if (NULL != p_instance_ctrl->p_callback)
    {
#if BSP_TZ_SECURE_BUILD

        /* p_callback can point to a secure function or a non-secure function. */
        if (!cmse_is_nsfptr(p_instance_ctrl->p_callback))
        {
            /* If p_callback is secure, then the project does not need to change security state. */
            p_instance_ctrl->p_callback(p_args);
        }
        else
        {
            /* If p_callback is Non-secure, then the project must change to Non-secure state in order to call the callback. */
            ctsu_prv_ns_callback p_callback = (ctsu_prv_ns_callback) (p_instance_ctrl->p_callback);
            p_callback(p_args);
        }

#else

        /* If the project is not Trustzone Secure, then it will never need to change security state in order to call the callback. */
        p_instance_ctrl->p_callback(p_args);
#endif
    }

    if (NULL != p_instance_ctrl->p_callback_memory)
    {
        /* Restore callback memory in case this is a nested interrupt. */
        *p_instance_ctrl->p_callback_memory = args;
    }

    /* reset indexes */
    p_instance_ctrl->wr_index = 0;
    p_instance_ctrl->rd_index = 0;
#if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
    p_instance_ctrl->serial_tuning_mutual_cnt = 0;
#endif
}

/***********************************************************************************************************************
 * ctsu_correction_process
 ***********************************************************************************************************************/
void ctsu_correction_process (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
#if (BSP_FEATURE_CTSU_VERSION == 2)
    uint32_t i;
    uint32_t j;
    uint16_t error_registance[CTSU_RANGE_NUM];
    uint32_t * p_trimb = (uint32_t *)(0x007fc3a8);
    uint32_t trimb;
    uint8_t  trimb_byte;
    uint8_t  rtrim;
    uint16_t base_value;
    uint16_t base_conv_dac;
    uint32_t ref_include_error;
    int32_t  x0;
    int32_t  x1;
    int32_t  y0;

    g_ctsu_correction_info.status = CTSU_CORRECTION_RUN;

    CTSU.CTSUCRA.BIT.MD0  = 1;
    CTSU.CTSUCRA.BIT.MD1  = 0;
    CTSU.CTSUMCH.BIT.MCA0 = 1;
    CTSU.CTSUMCH.BIT.MCA1 = 0;
    CTSU.CTSUMCH.BIT.MCA2 = 0;
    CTSU.CTSUMCH.BIT.MCA3 = 0;
    CTSU.CTSUCRA.BIT.LOAD = 1;

    /* Setting time of measurement */
    g_ctsu_correction_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10);

    /* Step1 : Get resistance value from TRIMB register. */
    trimb = * p_trimb;
    rtrim = CTSU.CTSUTRIMA.BIT.RTRIM;
    for (i = 0; i < CTSU_RANGE_NUM; i++)
    {
        trimb_byte          = (trimb >> (i * 8)) & CTSU_CORRECTION_TRIMB_MAX;
        error_registance[i] = (uint16_t) (trimb_byte & (uint8_t) (~CTSU_CORRECTION_TRIMB_SIGN_BIT));
        if (trimb_byte & CTSU_CORRECTION_TRIMB_SIGN_BIT)
        {
            if ((rtrim > CTSU_CORRECTION_RTRIM_THRESHOLD1) && (trimb_byte >= CTSU_CORRECTION_TRIMB_THRESHOLD1))
            {
                /* Minus2 : bit8(0.50) set 1 */
                error_registance[i] = (error_registance[i] | CTSU_CORRECTION_BIT8);
            }
            else
            {
                /* Plus   : bit9(1.00) set 1 */
                error_registance[i] = (error_registance[i] | CTSU_CORRECTION_BIT9);
            }
        }
        else
        {
            if ((rtrim < CTSU_CORRECTION_RTRIM_THRESHOLD2) && (trimb_byte <= CTSU_CORRECTION_TRIMB_THRESHOLD2))
            {
                /* Plus2  : bit7(0.25) and bit9(1.00) set 1 */
                error_registance[i] = (error_registance[i] | CTSU_CORRECTION_BIT7 | CTSU_CORRECTION_BIT9);
            }
            else
            {
                /* Minus : bit8(0.50) and bit7(0.25) set 1 */
                error_registance[i] = (error_registance[i] | CTSU_CORRECTION_BIT7 | CTSU_CORRECTION_BIT8);
            }
        }

        /* 6-bit left shift, 512 to 32768 */
        error_registance[i] = (uint16_t) (error_registance[i] << (CTSU_SHIFT_AMOUNT - 9));

 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
        g_ctsu_diag_info.error_registance[i] = error_registance[i];
 #endif
    }

    /* Step2-a : Measure the current input to the ICO by passing current through the internal resistance */
    /*           in each range. The theoretical value of the current is 12.5uA. */
    CTSU.CTSUCHACA.LONG        = 1;
    CTSU.CTSUCHACB.LONG        = 0;
    CTSU.CTSUCALIB.BIT.TSOC = 1;
    for (i = 0; i < CTSU_RANGE_NUM; i++)
    {
        if (CTSU_RANGE_20UA == i)
        {
            CTSU.CTSUCRA.BIT.ATUNE1 = 0;
            CTSU.CTSUCRA.BIT.ATUNE2 = 1;
        }
        else if (CTSU_RANGE_40UA == i)
        {
            CTSU.CTSUCRA.BIT.ATUNE1 = 1;
            CTSU.CTSUCRA.BIT.ATUNE2 = 0;
        }
        else if (CTSU_RANGE_80UA == i)
        {
            CTSU.CTSUCRA.BIT.ATUNE1 = 0;
            CTSU.CTSUCRA.BIT.ATUNE2 = 0;
        }
        else
        {
            CTSU.CTSUCRA.BIT.ATUNE1 = 1;
            CTSU.CTSUCRA.BIT.ATUNE2 = 1;
        }

        CTSU.CTSUCRA.BIT.LOAD = 3;
        ctsu_correction_measurement(p_instance_ctrl, &g_ctsu_correction_info.base_value[i]);
        CTSU.CTSUCRA.BIT.LOAD = 3;
    }

    /* Step3 : Measure by inputting each constant current from internal DAC to ICO. */
    CTSU.CTSUCRB.BIT.SSCNT      = 0;
    CTSU.CTSUCALIB.BIT.CCOCLK   = 0;
    CTSU.CTSUCALIB.BIT.CCOCALIB = 1;

    /* 1.25uA * (j + 1), SUCARRY is required for greater than 10uA */
    for (j = 0; j < CTSU_CORRECTION_POINT_NUM; j++)
    {
        CTSU.CTSUCRA.BIT.SDPSEL = 0;
        if (8 > j)
        {
            CTSU.CTSUSUCLKA.BIT.SUADJ0 = (uint32_t) (((j + 1) * CTSU_CORRECTION_SUMULTI) - 1);
        }
        else
        {
            CTSU.CTSUCRB.BIT.SSCNT     = 3;
            CTSU.CTSUCALIB.BIT.SUCARRY = 1;
            CTSU.CTSUSUCLKA.BIT.SUADJ0 = (uint32_t) (((j - 3) * CTSU_CORRECTION_SUMULTI) - 1);
        }

        CTSU.CTSUCRA.BIT.SDPSEL = 1;
        ctsu_correction_measurement(p_instance_ctrl, &g_ctsu_correction_info.dac_value[j]);
    }

    /* Step4 : Calculate the coefficient between step2 and step3. */
    for (i = 0; i < CTSU_RANGE_NUM; i++)
    {
        /* Linear interpolation calculation */
        base_value = g_ctsu_correction_info.base_value[i];
        j          = 1;
        while (1)
        {
            if ((base_value < g_ctsu_correction_info.dac_value[j]) || ((CTSU_CORRECTION_POINT_NUM - 1) == j))
            {
                y0 = (uint16_t) (CTSU_CORRECTION_STD_UNIT * (j + 1));
                x0 = g_ctsu_correction_info.dac_value[j];
                x1 = g_ctsu_correction_info.dac_value[j - 1];
                break;
            }

            j++;
        }

        base_conv_dac = (uint16_t) (y0 - ((CTSU_CORRECTION_STD_UNIT * (x0 - base_value)) / (x0 - x1)));

        /* Error rate calculation */
        ref_include_error = (uint32_t) (CTSU_CORRECTION_STD_VAL * (CTSU_CORRECTION_BIT16 - error_registance[i]));
        g_ctsu_correction_info.error_rate[i] = (uint16_t) (ref_include_error / base_conv_dac);

        for (j = 0; j < CTSU_CORRECTION_POINT_NUM; j++)
        {
            g_ctsu_correction_info.ref_value[i][j] =
                (uint16_t) ((CTSU_CORRECTION_STD_UNIT * (j + 1) * g_ctsu_correction_info.error_rate[i]) >>
                            CTSU_SHIFT_AMOUNT);
        }
    }

    for (i = 0; i < CTSU_RANGE_NUM - 1; i++)
    {
        g_ctsu_correction_info.range_ratio[i] =
            (uint16_t) (((uint32_t) g_ctsu_correction_info.error_rate[i] << CTSU_SHIFT_AMOUNT) /
                        g_ctsu_correction_info.error_rate[CTSU_RANGE_160UA]);
    }

 #if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 1)
    ctsu_auto_correction_register_set (p_instance_ctrl);

    /* Sensor counter auto correction enabled */
    CTSU.CTSUOPT.BIT.CCOCFEN = 1;
 #endif
    g_ctsu_correction_info.status = CTSU_CORRECTION_COMPLETE;
#endif
#if (BSP_FEATURE_CTSU_VERSION == 1)
    uint16_t second_std_val;
    uint32_t ctsu_sdpa;
    uint32_t pclkb_mhz;

 #if BSP_FEATURE_CTSU_HAS_TRMR
    uint8_t ctsutrimr_def;
 #endif

    g_ctsu_correction_info.status = CTSU_CORRECTION_RUN;

    pclkb_mhz = BSP_PCLKB_HZ / CTSU_PCLKB_FREQ_MHZ;
    if (CTSU_PCLKB_FREQ_RANGE1 >= pclkb_mhz)
    {
        CTSU.CTSUCR1.BYTE |= (0 << 4);
        ctsu_sdpa        = pclkb_mhz - 1;
    }
    else if ((CTSU_PCLKB_FREQ_RANGE1 < pclkb_mhz) && (CTSU_PCLKB_FREQ_RANGE2 >= pclkb_mhz))
    {
        CTSU.CTSUCR1.BYTE |= (1 << 4);
        ctsu_sdpa        = (pclkb_mhz / 2) - 1;
    }
    else
    {
        CTSU.CTSUCR1.BYTE |= (2 << 4);
        ctsu_sdpa        = (pclkb_mhz / 4) - 1;
    }

    g_ctsu_correction_info.ctsu_clock = pclkb_mhz >> CTSU_CFG_PCLK_DIVISION;

    CTSU.CTSUCR1.BYTE    |= (CTSU_MODE_SELF_MULTI_SCAN << 6);
    CTSU.CTSUCHAC0.BYTE = 0x01;

    g_ctsu_correction_info.ctsuwr.ctsussc = (CTSU_SSDIV_0500 << 8);
    g_ctsu_correction_info.ctsuwr.ctsuso0 = 0x0000;

    /* Set CTSUSO1 */
    g_ctsu_correction_info.ctsuwr.ctsuso1 = (uint16_t) ((CTSU_ICOG_66 << 13) | (ctsu_sdpa << 8) | CTSU_RICOA_RECOMMEND);

    /* Correction measurement setting */
    CTSU.CTSUERRS.BIT.CTSUSPMD = 2;
    CTSU.CTSUERRS.BIT.CTSUTSOC = 1;
    R_BSP_SoftwareDelay(CTSU_WAIT_TIME, BSP_DELAY_MICROSECS);

    /* First value measurement */
    ctsu_correction_measurement(p_instance_ctrl, &g_ctsu_correction_info.first_val);

    /* Second standard value create */
 #if BSP_FEATURE_CTSU_HAS_TRMR
    uint32_t work;

    /* Current trimming value storage */
    ctsutrimr_def = CTSU.CTSUTRMR;

    /*                       ctsutrimr_def + 273            ((ctsutrimr_def + 273) * 2560 * 128)      */
    /*    second_std_val  =  ------------------- * 40960 =  ------------------------------------ + 64 */
    /*                               528                                   33 * 128                   */

    work           = (ctsutrimr_def + 273) * 9930 + 64;
    second_std_val = (uint16_t) (work >> 7);

    /* 0xFF set in the current trimming register */
    CTSU.CTSUTRMR = 0xFF;
 #else
    second_std_val = (uint16_t) (CTSU_CORRECTION_2ND_STD_VAL * CTSU_WAFER_PARAMETER);
    g_ctsu_correction_info.ctsuwr.ctsuso1 |= (uint16_t) (CTSU_ICOG_40 << 13); /* ICO gain  66% -> 40% */
 #endif

    /* Second value measurement */
    ctsu_correction_measurement(p_instance_ctrl, &g_ctsu_correction_info.second_val);
 #if BSP_FEATURE_CTSU_HAS_TRMR

    /* Return the current trimming register to the initial value */
    CTSU.CTSUTRMR = ctsutrimr_def;
 #endif

    /* Normal measurement setting */
    CTSU.CTSUERRS.BIT.CTSUTSOC = 0;
    CTSU.CTSUERRS.BIT.CTSUSPMD = 0;

    R_BSP_SoftwareDelay(CTSU_WAIT_TIME, BSP_DELAY_MICROSECS);

    if ((0 != g_ctsu_correction_info.first_val) && (0 != g_ctsu_correction_info.second_val))
    {
        if (g_ctsu_correction_info.second_val < g_ctsu_correction_info.first_val)
        {
            /* 1st coefficient create */
            g_ctsu_correction_info.first_coefficient = (CTSU_CORRECTION_1ST_STD_VAL << CTSU_SHIFT_AMOUNT) /
                                                       g_ctsu_correction_info.first_val;

            /* 2nd coefficient create */
            g_ctsu_correction_info.second_coefficient = ((uint32_t) second_std_val << CTSU_SHIFT_AMOUNT) /
                                                        g_ctsu_correction_info.second_val;

            g_ctsu_correction_info.status = CTSU_CORRECTION_COMPLETE;
        }
        else
        {
            g_ctsu_correction_info.status = CTSU_CORRECTION_ERROR;
        }
    }
    else
    {
        g_ctsu_correction_info.status = CTSU_CORRECTION_ERROR;
    }
#endif
}

/***********************************************************************************************************************
 * ctsu_correction_measurement
 ***********************************************************************************************************************/
void ctsu_correction_measurement (ctsu_instance_ctrl_t * const p_instance_ctrl, uint16_t * data)
{
    uint16_t i;
    uint32_t sum = 0;

    for (i = 0; i < CTSU_CORRECTION_AVERAGE; i++)
    {
#if (CTSU_CFG_DTC_SUPPORT_ENABLE == 1)
        ctsu_transfer_configure(p_instance_ctrl);
#else
        FSP_PARAMETER_NOT_USED(p_instance_ctrl);
#endif
        p_instance_ctrl->state = CTSU_STATE_SCANNING;
#if (BSP_FEATURE_CTSU_VERSION == 2)
        CTSU.CTSUCRA.LONG       |= 0x01;
        while (p_instance_ctrl->state != CTSU_STATE_SCANNED)
        {
        }

        sum += g_ctsu_correction_info.scanbuf;
#endif
#if (BSP_FEATURE_CTSU_VERSION == 1)
        CTSU.CTSUCR0.BYTE       |= 0x01;
        while (p_instance_ctrl->state != CTSU_STATE_SCANNED)
        {
        }
        sum += g_ctsu_correction_info.scanbuf.sen;
#endif
    }

    *data = (uint16_t) (sum / CTSU_CORRECTION_AVERAGE);
}

#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_TEMP_CORRECTION_SUPPORT == 1)

/***********************************************************************************************************************
 * ctsu_correction_scan_start
 ***********************************************************************************************************************/
void ctsu_correction_scan_start (void)
{
    g_ctsu_temp_reg_ctsucra = CTSU.CTSUCRA.LONG;

    CTSU.CTSUCRA.BIT.MD0  = 1;
    CTSU.CTSUCRA.BIT.MD1  = 0;
    CTSU.CTSUMCH.BIT.MCA0 = 1;
    CTSU.CTSUMCH.BIT.MCA1 = 0;
    CTSU.CTSUMCH.BIT.MCA2 = 0;
    CTSU.CTSUMCH.BIT.MCA3 = 0;

    /* Setting time of measurement */
    g_ctsu_correction_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10);
    g_ctsu_correction_info.suadj0        = (uint8_t) (CTSU.CTSUSUCLKA.BIT.SUADJ0 & CTSU_SUADJ_MAX);

    if (g_ctsu_correction_info.scan_index < CTSU_CORRECTION_POINT_NUM)
    {
        /* Dummy setting */
        CTSU.CTSUCHACA.LONG = 1;
        CTSU.CTSUCHACB.LONG = 0;
        CTSU.CTSUCHTRCA.LONG = 0;
        CTSU.CTSUCHTRCB.LONG = 0;

        /* Step3 : Measure by inputting each constant current from internal DAC to ICO. */
        CTSU.CTSUCRB.BIT.SSCNT      = 0;
        CTSU.CTSUCALIB.BIT.CCOCLK   = 0;
        CTSU.CTSUCALIB.BIT.CCOCALIB = 1;
        CTSU.CTSUCALIB.BIT.TSOC     = 1;

        CTSU.CTSUCRA.BIT.SDPSEL = 0;
        if (8 > g_ctsu_correction_info.scan_index)
        {
            CTSU.CTSUSUCLKA.BIT.SUADJ0 = (uint16_t) (((g_ctsu_correction_info.scan_index + 1) * CTSU_CORRECTION_SUMULTI) - 1);
        }
        else
        {
            /* SUCARRY is required for greater than 10uA */
            CTSU.CTSUCRB.BIT.SSCNT     = 3;
            CTSU.CTSUCALIB.BIT.SUCARRY = 1;
            CTSU.CTSUSUCLKA.BIT.SUADJ0          =
                (uint16_t) (((g_ctsu_correction_info.scan_index - 3) * CTSU_CORRECTION_SUMULTI) - 1);
        }

        CTSU.CTSUCRA.BIT.SDPSEL = 1;
    }
    else
    {
        /* Step2-b : Measure the current input to the ICO by passing current through the external resistance. */
        /*           The theoretical value of the current is 9.375uA. */
        CTSU.CTSUCRA.BIT.ATUNE1 = 1;
        CTSU.CTSUCRA.BIT.ATUNE2 = 1;
  #if (CTSU_CFG_TEMP_CORRECTION_TS < 32)
        CTSU.CTSUCHACA.LONG = (uint32_t) (1 << CTSU_CFG_TEMP_CORRECTION_TS);
        CTSU.CTSUCHACB.LONG = 0;
  #else
        CTSU.CTSUCHACA.LONG = 0;
        CTSU.CTSUCHACB.LONG = (uint32_t) (1 << (CTSU_CFG_TEMP_CORRECTION_TS - 32));
  #endif
        CTSU.CTSUCRA.BIT.DCMODE = 1;
        CTSU.CTSUCRA.BIT.DCBACK = 1;
        CTSU.CTSUSO.BIT.SO      = 0;
    }
}

/***********************************************************************************************************************
 * ctsu_correction_data_get
 ***********************************************************************************************************************/
fsp_err_t ctsu_correction_data_get (ctsu_instance_ctrl_t * const p_instance_ctrl, uint16_t * p_data)
{
    uint32_t i;
    uint32_t j;
    uint16_t base_value;
    uint16_t base_conv_dac;
    int32_t  x0;
    int32_t  x1;
    int32_t  y0;
    ctsu_data_t temp_avg_data;
    fsp_err_t err = FSP_SUCCESS;

    if (g_ctsu_correction_info.scan_index < CTSU_CORRECTION_POINT_NUM)
    {
        temp_avg_data.int_data = g_ctsu_correction_info.dac_value[g_ctsu_correction_info.scan_index];
        ctsu_moving_average(&temp_avg_data,
                            *p_instance_ctrl->p_self_raw,
                            4);
        g_ctsu_correction_info.dac_value[g_ctsu_correction_info.scan_index] = temp_avg_data.int_data;
        g_ctsu_correction_info.scan_index++;
    }
    else
    {
        if (0 == g_ctsu_correction_info.ex_base_value)
        {
            g_ctsu_correction_info.ex_base_value  = *p_instance_ctrl->p_self_raw;
            g_ctsu_correction_info.update_counter = CTSU_CFG_TEMP_CORRECTION_TIME;
        }
        else
        {
            temp_avg_data.int_data = g_ctsu_correction_info.ex_base_value;
            ctsu_moving_average(&temp_avg_data, *p_instance_ctrl->p_self_raw,4);
            g_ctsu_correction_info.ex_base_value = temp_avg_data.int_data;
        }

        g_ctsu_correction_info.scan_index = 0;
        g_ctsu_correction_info.update_counter++;
    }

    /* Step4 : Calculate the coefficient between step2 and step3. */
    if (g_ctsu_correction_info.update_counter > CTSU_CFG_TEMP_CORRECTION_TIME)
    {
        /* Linear interpolation calculation */
        base_value = g_ctsu_correction_info.ex_base_value;
        j          = 1;
        while (1)
        {
            if ((base_value < g_ctsu_correction_info.dac_value[j]) || ((CTSU_CORRECTION_POINT_NUM - 1) == j))
            {
                y0 = (uint16_t) (CTSU_CORRECTION_STD_UNIT * (j + 1));
                x0 = g_ctsu_correction_info.dac_value[j];
                x1 = g_ctsu_correction_info.dac_value[j - 1];
                break;
            }

            j++;
        }

        base_conv_dac = (uint16_t) (y0 - ((CTSU_CORRECTION_STD_UNIT * (x0 - base_value)) / (x0 - x1)));

        /* Error rate calculation */
        g_ctsu_correction_info.error_rate[CTSU_RANGE_160UA] =
            (uint16_t) (((uint32_t) CTSU_CORRECTION_STD_EXREG << (uint32_t) CTSU_SHIFT_AMOUNT) / base_conv_dac);

        for (j = 0; j < CTSU_CORRECTION_POINT_NUM; j++)
        {
            g_ctsu_correction_info.ref_value[CTSU_RANGE_160UA][j] =
                (uint16_t) ((CTSU_CORRECTION_STD_UNIT * (j + 1) *
                             (uint32_t) g_ctsu_correction_info.error_rate[CTSU_RANGE_160UA]) >> CTSU_SHIFT_AMOUNT);
            for (i = 0; i < CTSU_RANGE_NUM - 1; i++)
            {
                g_ctsu_correction_info.ref_value[i][j] =
                    (uint16_t) (((uint32_t) g_ctsu_correction_info.ref_value[CTSU_RANGE_160UA][j] *
                                 (uint32_t) g_ctsu_correction_info.range_ratio[i]) >> CTSU_SHIFT_AMOUNT);
            }
        }

  #if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 1)
        /* Initialization of sensor counter auto correction table register number */
        CTSU.CTSUOPT.BIT.SCACTB = 0;

        ctsu_auto_correction_register_set(p_instance_ctrl);
  #endif

        g_ctsu_correction_info.update_counter = 0;
  #if (CTSU_CFG_CALIB_RTRIM_SUPPORT == 1)
        err = ctsu_correction_calib_rtrim(p_instance_ctrl, p_data);
        if (FSP_ERR_ALREADY_OPEN != err)
        {
            R_ADC_Close(0);
        }
        if (FSP_SUCCESS != err)
        {
            err = FSP_ERR_ABORTED;
        }
  #endif
    }
    else
    {
        /* Indicates that ADC measurement was not performed. */
        *p_data = CTSU_COUNT_MAX;
    }

    CTSU.CTSUCRA.LONG = g_ctsu_temp_reg_ctsucra;
#if (CTSU_CFG_TEMP_CORRECTION_TS < 32)
    CTSU.CTSUCHACA.LONG &= (uint32_t) ~(1 << CTSU_CFG_TEMP_CORRECTION_TS);
#else
    CTSU.CTSUCHACB.LONG &= (uint32_t) ~(1 << (CTSU_CFG_TEMP_CORRECTION_TS - 32));
#endif

    return err;
}

  #if (CTSU_CFG_CALIB_RTRIM_SUPPORT == 1)

/***********************************************************************************************************************
 * ctsu_correction_calib_rtrim
 ***********************************************************************************************************************/
fsp_err_t ctsu_correction_calib_rtrim (ctsu_instance_ctrl_t * const p_instance_ctrl, uint16_t * p_data)
{
    uint16_t               i;
    uint16_t               adctdr_result;
    uint16_t               adctdr_ave;
    uint32_t               adctdr_sum;
    int16_t                diff;
    int16_t                dir  = 0;
    uint16_t               comp = 0;
    adc_cfg_t              open_cfg;
    adc_ch_cfg_t           ctrl_cfg;
    fsp_err_t              err = FSP_SUCCESS;
    adc_err_t              poll;

    /* TSCAP AD/C AN008 Setting */
    CTSU.CTSUADCC.LONG = 1;

    /* Setting for A/D convert mode */
    open_cfg.conv_speed = ADC_CONVERT_SPEED_DEFAULT;
    open_cfg.alignment = ADC_ALIGN_RIGHT;
    open_cfg.add_cnt = ADC_ADD_OFF;                 /* addition is turned off for chans/sensors */
    open_cfg.clearing = ADC_CLEAR_AFTER_READ_OFF;
    open_cfg.trigger = ADC_TRIG_SOFTWARE;           /* Software trigger */
    open_cfg.trigger_groupb = ADC_TRIG_NONE;        /* Trigger none when without group mode */
    open_cfg.priority = 0;                          /* S12ADIO no interrupt(polling) */
    open_cfg.priority_groupb = 0;                   /* S12GBADI no interrupt */
    err = (fsp_err_t) R_ADC_Open (0, ADC_MODE_SS_ONE_CH, &open_cfg, FIT_NO_FUNC);
    if (ADC_ERR_AD_NOT_CLOSED == (adc_err_t) err)
    {
        return FSP_ERR_ALREADY_OPEN;
    }
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* choice A/D port(allow A/D convert) */
    ctrl_cfg.chan_mask = ADC_MASK_CH8;
    ctrl_cfg.chan_mask_groupb = ADC_MASK_GROUPB_OFF;
    ctrl_cfg.priority_groupa = ADC_GRPA_PRIORITY_OFF;
    ctrl_cfg.diag_method = ADC_DIAG_OFF;
    ctrl_cfg.add_mask = ADC_MASK_ADD_OFF;
    ctrl_cfg.signal_elc = ADC_ELC_ALL_SCANS_DONE;
    err = (fsp_err_t) R_ADC_Control (0, ADC_CMD_ENABLE_CHANS, &ctrl_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Self single scan mode */
    CTSU.CTSUCRA.BIT.LOAD = 1;
    CTSU.CTSUCRA.BIT.MD0  = 0;
    CTSU.CTSUCRA.BIT.MD1  = 0;

    /* Set Channel */
   #if (CTSU_CFG_TEMP_CORRECTION_TS < 32)
    CTSU.CTSUCHACA.LONG  |= (uint32_t) (1 << CTSU_CFG_TEMP_CORRECTION_TS);
    CTSU.CTSUCHTRCA.LONG &= (uint32_t) ~(1 << CTSU_CFG_TEMP_CORRECTION_TS);
   #else
    CTSU.CTSUCHACB.LONG  |= (uint32_t) (1 << (CTSU_CFG_TEMP_CORRECTION_TS - 32));
    CTSU.CTSUCHTRCB.LONG &= (uint32_t) ~(1 << (CTSU_CFG_TEMP_CORRECTION_TS - 32));
   #endif
    CTSU.CTSUMCH.BIT.MCH0 = CTSU_CFG_TEMP_CORRECTION_TS;

    /* 150uA current measurement */
    CTSU.CTSUCRA.BIT.DCMODE = 1;
    CTSU.CTSUCRA.BIT.DCBACK = 1;
    CTSU.CTSUSO.BIT.SO      = CTSU_CALIB_CTSUSO;
    CTSU.CTSUCRA.BIT.CSW    = 0;
    CTSU.CTSUCALIB.BIT.DRV  = 1;

    /* ADC scan */
    while (!comp)
    {
        adctdr_sum = 0;
        for (i = 0; i < CTSU_CALIB_AVERAGE_TIME; i++)
        {
            /* Software trigger start scan */
            err = (fsp_err_t) R_ADC_Control (0, ADC_CMD_SCAN_NOW, FIT_NO_PTR);
            FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

            /* Polling for scan complete */
            do
            {
                poll = R_ADC_Control (0, ADC_CMD_CHECK_SCAN_DONE, FIT_NO_PTR);
            } while (ADC_ERR_SCAN_NOT_DONE == poll);

            /* Read A/D data then scan normal end */
            err = (fsp_err_t) R_ADC_Read (0, ADC_REG_CH8, &adctdr_result);
            FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
            adctdr_sum += adctdr_result;
        }

        adctdr_ave = (uint16_t) ((adctdr_sum * 10) / CTSU_CALIB_AVERAGE_TIME);
        diff       = (int16_t) ((adctdr_ave - CTSU_CALIB_REF) / 10);

        /* The change unit of the voltage by the RTRIM register is about 4mV (4096 * 4) = 16.384 */
        if (diff > CTSU_CALIB_THRESHOLD)
        {
            if (0 <= dir)
            {
                CTSU.CTSUTRIMA.BIT.RTRIM = (uint8_t) (CTSU.CTSUTRIMA.BIT.RTRIM + 1);
                dir = 1;
            }
            else
            {
                comp = 1;
            }
        }
        else if (diff < -(CTSU_CALIB_THRESHOLD))
        {
            if (0 >= dir)
            {
                CTSU.CTSUTRIMA.BIT.RTRIM = (uint8_t) (CTSU.CTSUTRIMA.BIT.RTRIM - 1);
                dir = -1;
            }
            else
            {
                comp = 1;
            }
        }
        else
        {
            comp = 1;
        }
    }

    /* Restore register settings */
    CTSU.CTSUCALIB.BIT.DRV  = 0;
    CTSU.CTSUCRA.BIT.CSW    = 1;
    CTSU.CTSUCRA.BIT.DCMODE = 0;
    CTSU.CTSUCRA.BIT.DCBACK = 0;

    /* Indicates that ADC measurement was performed  */
    *p_data = CTSU.CTSUTRIMA.BIT.RTRIM;

    /* Close ADC for CTSU TSCAP */
    R_ADC_Close(0);

    return err;
}

  #endif
 #endif
#endif

#if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 0)
/***********************************************************************************************************************
 * ctsu_correction_calc
 ***********************************************************************************************************************/
void ctsu_correction_calc (uint16_t * correction_data, uint16_t raw_data, ctsu_correction_calc_t * p_calc)
{
    uint32_t answer;
    int32_t  cmp_data;
    uint8_t  calc_flag = 0;
#if (BSP_FEATURE_CTSU_VERSION == 1)
    uint16_t diff_val;
    int32_t  diff_coefficient;
    int32_t  mul_diffcoff_diff1valsval;
    uint32_t mul_coff1val_diffcorr;
    uint16_t coefficient;
#endif
#if (BSP_FEATURE_CTSU_VERSION == 2)
    int64_t  y0 = 0;
    int64_t  y1 = 0;
    int64_t  x0 = 0;
    int64_t  x1 = 0;
    uint16_t i;
#endif

#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_NUM_CFC != 0)
    if ((CTSU_CORRECTION_COMPLETE == g_ctsu_correction_info.status) ||
        (CTSU_CORRECTION_COMPLETE == g_ctsu_corrcfc_info.status))
    {
        calc_flag = 1;
    }

 #else
    if (CTSU_CORRECTION_COMPLETE == g_ctsu_correction_info.status)
    {
        calc_flag = 1;
    }
 #endif
#endif
#if (BSP_FEATURE_CTSU_VERSION == 1)
    if (CTSU_CORRECTION_COMPLETE == g_ctsu_correction_info.status)
    {
        calc_flag = 1;
    }
#endif

    if (calc_flag)
    {
#if (BSP_FEATURE_CTSU_VERSION == 1)

        /* Since the correction coefficient table is created with the recommended measurement time, */
        /* If the measurement time is different, adjust the value level. */
        cmp_data = (int32_t) ((raw_data * g_ctsu_correction_info.ctsu_clock) /
                              (uint32_t) ((p_calc->snum + 1) * (p_calc->sdpa + 1)));

        /*               g_mul_coff1val_diffcorr - g_diff_cofficient * (g_ctsu_correction_info.first_val - raw_data) */
        /*  coefficient= ------------------------------------------------------------------------------------------  */
        /*                                      g_diff_correct_val                                                   */
        /*                                                                                                           */

        diff_val = (uint16_t) (g_ctsu_correction_info.first_val - g_ctsu_correction_info.second_val);

        /* Get multiplication of g_ctsu_correction_info.first_coefficient and difference of Correction value */
        mul_coff1val_diffcorr = g_ctsu_correction_info.first_coefficient * diff_val;

        /* Get difference of Correction coefficient */
        diff_coefficient =
            (int32_t) (g_ctsu_correction_info.first_coefficient - g_ctsu_correction_info.second_coefficient);

        /* Get multiplication of  g_diff_cofficient  and (g_ctsu_correction_info.first_val - raw_data_coff) */
        mul_diffcoff_diff1valsval = (diff_coefficient * (int32_t) (g_ctsu_correction_info.first_val - cmp_data));

        /* Get correction coefficient of scan data */
        coefficient = (uint16_t) (((int32_t) mul_coff1val_diffcorr - mul_diffcoff_diff1valsval) / diff_val);

        /* Get output count data */
        answer = (uint32_t) (((uint32_t) raw_data * (uint32_t) coefficient) >> CTSU_SHIFT_AMOUNT);
#endif
#if (BSP_FEATURE_CTSU_VERSION == 2)

        /* Since the correction coefficient table is created with the recommended measurement time, */
        /* If the measurement time is different, adjust the value level. */
        if (CTSU_SNUM_RECOMMEND == p_calc->snum)
        {
            cmp_data = raw_data;
        }
        else
        {
            cmp_data = (int32_t) (((int32_t) raw_data * (CTSU_SNUM_RECOMMEND + 1)) / (p_calc->snum + 1));
        }

        /* y = y0 + (y1 - y0) * (x - x0) / (x1 - x0);    [y=coefficient, x=value] */
        if (CTSU_MODE_MUTUAL_CFC_SCAN != p_calc->md)
        {
            i = 0;
            while (1)
            {
                if ((cmp_data < g_ctsu_correction_info.dac_value[i]) || ((CTSU_CORRECTION_POINT_NUM - 1) == i))
                {
                    y0 = g_ctsu_correction_info.ref_value[p_calc->range][i];
                    x0 = g_ctsu_correction_info.dac_value[i];
                    if (0 == i)
                    {
                        x1 = 0;
                        y1 = 0;
                    }
                    else
                    {
                        x1 = g_ctsu_correction_info.dac_value[i - 1];
                        y1 = g_ctsu_correction_info.ref_value[p_calc->range][i - 1];
                    }

                    break;
                }

                i++;
            }
        }

 #if (CTSU_CFG_NUM_CFC != 0)
        else
        {
            i = 0;
            while (1)
            {
                if ((cmp_data < g_ctsu_corrcfc_info.dac_value[p_calc->cfc][i]) || ((CTSU_CORRCFC_POINT_NUM - 1) == i))
                {
                    y0 = g_ctsu_corrcfc_info.ref_value[p_calc->cfc][i];
                    x0 = g_ctsu_corrcfc_info.dac_value[p_calc->cfc][i];
                    if (0 == i)
                    {
                        x1 = 0;
                        y1 = 0;
                    }
                    else
                    {
                        x1 = g_ctsu_corrcfc_info.dac_value[p_calc->cfc][i - 1];
                        y1 = g_ctsu_corrcfc_info.ref_value[p_calc->cfc][i - 1];
                    }

                    break;
                }

                i++;
            }
        }
 #endif
        if (CTSU_SNUM_RECOMMEND != p_calc->snum)
        {
            x0 = (int64_t)((x0 * (p_calc->snum + 1)) / (CTSU_SNUM_RECOMMEND + 1));
            y0 = (int64_t)((y0 * (p_calc->snum + 1)) / (CTSU_SNUM_RECOMMEND + 1));
            x1 = (int64_t)((x1 * (p_calc->snum + 1)) / (CTSU_SNUM_RECOMMEND + 1));
            y1 = (int64_t)((y1 * (p_calc->snum + 1)) / (CTSU_SNUM_RECOMMEND + 1));
        }

        answer = (uint32_t) (y0 - (((y0 - y1) * (x0 - raw_data)) / (x0 - x1)));

#endif

        /* Value Overflow Check */
        if (CTSU_COUNT_MAX < answer)
        {
            *correction_data = CTSU_COUNT_MAX;
        }
        else
        {
            *correction_data = (uint16_t) answer;
        }
    }
    else
    {
        *correction_data = raw_data;
    }
}
#endif

/***********************************************************************************************************************
 * ctsu_correction_exec
 ***********************************************************************************************************************/
void ctsu_correction_exec (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
#if (BSP_FEATURE_CTSU_VERSION == 1)
    ctsu_correction_ctsu1_exec(p_instance_ctrl);
#endif
#if (BSP_FEATURE_CTSU_VERSION == 2)
    ctsu_correction_ctsu2_exec(p_instance_ctrl);
#endif
}

#if (BSP_FEATURE_CTSU_VERSION == 1)
void ctsu_correction_ctsu1_exec (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    uint16_t element_id;

    ctsu_correction_calc_t calc;
 #if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
    ctsu_data_t * p_self_data;
    ctsu_data_t   average_self = {0, 0};
 #endif
 #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
    ctsu_data_t * p_pri_data;
    ctsu_data_t * p_snd_data;
    ctsu_data_t   average_pri = {0, 0};
    ctsu_data_t   average_snd = {0, 0};
 #endif

    for (element_id = 0; element_id < p_instance_ctrl->num_elements; element_id++)
    {
        calc.snum = (p_instance_ctrl->p_ctsuwr[element_id].ctsuso0 >> 10) & CTSU_SNUM_MAX;
        calc.sdpa = (p_instance_ctrl->p_ctsuwr[element_id].ctsuso1 >> 8) & CTSU_SDPA_MAX;
 #if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
        if (CTSU_MODE_SELF_MULTI_SCAN == p_instance_ctrl->md)
        {
            p_self_data  = (p_instance_ctrl->p_self_data + element_id);
            average_self = *p_self_data;
            ctsu_correction_calc((p_instance_ctrl->p_self_corr + element_id),
                                 (p_instance_ctrl->p_self_raw + element_id)->sen,
                                 &calc);
            p_self_data->int_data = *(p_instance_ctrl->p_self_corr + element_id);
            if (1 < p_instance_ctrl->average)
            {
                ctsu_moving_average(&average_self, p_self_data->int_data, p_instance_ctrl->average);
                *p_self_data = average_self;
            }
        }
 #endif
 #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
        p_pri_data = (p_instance_ctrl->p_mutual_pri_data + element_id);
        p_snd_data = (p_instance_ctrl->p_mutual_snd_data + element_id);
        if (CTSU_MODE_MUTUAL_FULL_SCAN == p_instance_ctrl->md)
        {
            average_pri = *p_pri_data;
            average_snd = *p_snd_data;
            ctsu_correction_calc((p_instance_ctrl->p_mutual_pri_corr + element_id),
                                 (p_instance_ctrl->p_mutual_raw + element_id)->pri_sen, &calc);
            ctsu_correction_calc((p_instance_ctrl->p_mutual_snd_corr + element_id),
                                 (p_instance_ctrl->p_mutual_raw + element_id)->snd_sen, &calc);
            p_pri_data->int_data = *(p_instance_ctrl->p_mutual_pri_corr + element_id);
            p_snd_data->int_data = *(p_instance_ctrl->p_mutual_snd_corr + element_id);

            if (1 < p_instance_ctrl->average)
            {
                ctsu_moving_average(&average_pri, p_pri_data->int_data, p_instance_ctrl->average);
                ctsu_moving_average(&average_snd, p_snd_data->int_data, p_instance_ctrl->average);
                *p_pri_data = average_pri;
                *p_snd_data = average_snd;
            }
        }
 #endif
    }
}

#endif

#if (BSP_FEATURE_CTSU_VERSION == 2)
void ctsu_correction_ctsu2_exec (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    uint16_t                element_id;
    ctsu_correction_calc_t  calc;
    uint16_t                i;
    uint32_t                ctsuso;
    uint32_t                snum;
    int32_t                 offset_unit;
    uint16_t                majority_mode_element_num;
    ctsu_correction_multi_t multi;

 #if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
    ctsu_data_t * p_self_data;
    ctsu_data_t   average_self;
 #endif
 #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
    ctsu_data_t * p_pri_data;
    ctsu_data_t * p_snd_data;
    ctsu_data_t   average_pri;
    ctsu_data_t   average_snd;

  #if (CTSU_CFG_NUM_CFC != 0)
    uint8_t  ts_id;
    uint8_t  table_id;
    uint8_t  cfc_ts_table[CTSU_CFG_NUM_CFC];
    uint16_t offset = 0;
    uint16_t cfc_id = 0;
    uint16_t num_rx = 0;

    /* By inspection, calc.cfc is only used when calc.md == CTSU_MODE_MUTUAL_CFC_SCAN. It is initialized in this case.
     * However, GCC expects calc.cfc to be initialized before calling ctsu_correction_calc regardless of calc.md to
     * avoid the warning -Werror=maybe-uninitialized. */
    calc.cfc = 0;

    /* Create CFC-Rx table in ascending order at this instance */
    for (ts_id = 0; ts_id < CTSU_CORRCFC_TS_MAX; ts_id++)
    {
        if (1 == ((p_instance_ctrl->cfc_rx_bitmap >> ts_id) & 1))
        {
            for (table_id = 0; table_id < CTSU_CFG_NUM_CFC; table_id++)
            {
                if (g_ctsu_corrcfc_info.ts_table[table_id] == ts_id)
                {
                    cfc_ts_table[cfc_id] = table_id;
                    cfc_id++;
                }
            }
        }
    }
  #endif
 #endif
 #if (CTSU_CFG_MAJORITY_MODE & CTSU_JUDGEMENT_MAJORITY_MODE)
    if (1 == p_instance_ctrl->p_ctsu_cfg->majority_mode)
    {
        majority_mode_element_num = CTSU_MAJORITY_MODE_ELEMENTS;
    }
    else
 #endif
    {
        majority_mode_element_num = 1;
    }

    calc.range = p_instance_ctrl->range;
    calc.md    = p_instance_ctrl->md;

    for (element_id = 0; element_id < p_instance_ctrl->num_elements; element_id++)
    {
        calc.snum = (p_instance_ctrl->p_ctsuwr[(element_id * CTSU_CFG_NUM_SUMULTI)].ctsuso >> 10) & CTSU_SNUM_MAX;
        for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
        {
            ctsuso =
                (p_instance_ctrl->p_ctsuwr[(element_id * CTSU_CFG_NUM_SUMULTI) + i].ctsuso & CTSU_TUNING_MAX);
            snum = (p_instance_ctrl->p_ctsuwr[(element_id * CTSU_CFG_NUM_SUMULTI)].ctsuso >> 10) &
                   CTSU_SNUM_MAX;
            offset_unit = ((int32_t) (CTSU_CORRECTION_OFFSET_UNIT * (snum + 1))) /
                          ((int32_t) (CTSU_SNUM_RECOMMEND + 1));
            multi.offset[i] = (ctsuso * (uint32_t) (offset_unit >> calc.range));
        }

        if (CTSU_MODE_SELF_MULTI_SCAN == p_instance_ctrl->md)
        {
 #if (CTSU_CFG_NUM_SELF_ELEMENTS != 0)
            for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
            {
  #if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 0)
                ctsu_correction_calc(&p_instance_ctrl->p_self_corr[(element_id * CTSU_CFG_NUM_SUMULTI) + i],
                                     p_instance_ctrl->p_self_raw[(element_id * CTSU_CFG_NUM_SUMULTI) + i],
                                     &calc);
  #else
                p_instance_ctrl->p_self_corr[(element_id * CTSU_CFG_NUM_SUMULTI) + i] =
                    p_instance_ctrl->p_self_raw[(element_id * CTSU_CFG_NUM_SUMULTI) + i];
  #endif

                multi.pri[i] = p_instance_ctrl->p_self_corr[(element_id * CTSU_CFG_NUM_SUMULTI) + i];

                multi.snd[i] = 0;
            }

            for (i = 0; i < majority_mode_element_num; i++)
            {
                p_self_data = (p_instance_ctrl->p_self_data + (element_id * majority_mode_element_num) + i);

                if (0 == p_instance_ctrl->average)
                {
                    /* Store corrected data in p_pri_data[i] for initial offset tuning */
                }
                else
                {
                    /* Store last moving averaged data */
                    average_self.int_data           = p_self_data->int_data;
                    average_self.decimal_point_data = p_self_data->decimal_point_data;
  #if (CTSU_CFG_MAJORITY_MODE & CTSU_JUDGEMENT_MAJORITY_MODE)
                    if (p_instance_ctrl->p_ctsu_cfg->majority_mode == 1)
                    {
                        /* Skip the ctsu_correction_multi at Software JMM */
                        p_self_data->int_data = multi.pri[i];
                    }
                    else
  #endif
                    {
                        /* Matching values */
  #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 0)
                        ctsu_correction_freq(&multi, multi.pri, NULL);
  #endif
                        ctsu_correction_multi(&multi, &(p_self_data->int_data), NULL);
                        *(p_instance_ctrl->p_selected_freq_self + element_id) = multi.selected_freq;
                    }

                    /* Update moving averaged data */
                    ctsu_moving_average(&average_self, p_self_data->int_data, p_instance_ctrl->average);
                    *p_self_data = average_self;
                }
            }
        }
        else if (CTSU_MODE_CURRENT_SCAN == p_instance_ctrl->md)
        {
            p_self_data = (p_instance_ctrl->p_self_data + (element_id * CTSU_CFG_NUM_SUMULTI));

            /* Store last moving averaged data */
            average_self = *p_self_data;

            /* Correction */
  #if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 0)
            ctsu_correction_calc(&(p_self_data->int_data), p_instance_ctrl->p_self_raw[element_id], &calc);
  #else
            p_self_data->int_data =p_instance_ctrl->p_self_raw[element_id];
  #endif

            /* Update moving averaged data */
            if (1 < p_instance_ctrl->average)
            {
                ctsu_moving_average(&average_self, p_self_data->int_data, p_instance_ctrl->average);
                *p_self_data = average_self;
            }
 #endif
        }
        else if (CTSU_MODE_MUTUAL_FULL_SCAN == p_instance_ctrl->md)
        {
 #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
            for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
            {
                if (true == p_instance_ctrl->serial_tuning_enable)
                {
                    if ((0 == p_instance_ctrl->ctsuchtrc0) && (0 == p_instance_ctrl->ctsuchtrc1) &&
                        (0 == p_instance_ctrl->ctsuchtrc2) && (0 == p_instance_ctrl->ctsuchtrc3) &&
                        (0 == p_instance_ctrl->ctsuchtrc4))
                    {
                        /* Serial tuning Phase1 */
  #if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 0)
                        ctsu_correction_calc(&p_instance_ctrl->p_mutual_pri_corr[element_id],
                                     p_instance_ctrl->p_mutual_raw[element_id * CTSU_CFG_NUM_SUMULTI],
                                     &calc);
  #else
                        p_instance_ctrl->p_mutual_pri_corr[element_id] =
                            p_instance_ctrl->p_mutual_raw[element_id * CTSU_CFG_NUM_SUMULTI];
  #endif
                        break;
                    }
                }
  #if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 0)
                ctsu_correction_calc(&p_instance_ctrl->p_mutual_pri_corr[(element_id * CTSU_CFG_NUM_SUMULTI) + i],
                                     p_instance_ctrl->p_mutual_raw[(element_id * CTSU_MUTUAL_BUF_SIZE) + (i * 2)],
                                     &calc);
  #else
                p_instance_ctrl->p_mutual_pri_corr[(element_id * CTSU_CFG_NUM_SUMULTI) + i] =
                    p_instance_ctrl->p_mutual_raw[(element_id * CTSU_MUTUAL_BUF_SIZE) + (i * 2)];
  #endif

                multi.pri[i] = p_instance_ctrl->p_mutual_pri_corr[(element_id * CTSU_CFG_NUM_SUMULTI) + i];

  #if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 0)
                ctsu_correction_calc(&p_instance_ctrl->p_mutual_snd_corr[(element_id * CTSU_CFG_NUM_SUMULTI) + i],
                                     p_instance_ctrl->p_mutual_raw[(element_id * CTSU_MUTUAL_BUF_SIZE) + (i * 2) + 1],
                                     &calc);
  #else
                p_instance_ctrl->p_mutual_snd_corr[(element_id * CTSU_CFG_NUM_SUMULTI) + i] =
                    p_instance_ctrl->p_mutual_raw[(element_id * CTSU_MUTUAL_BUF_SIZE) + (i * 2) + 1];
  #endif

                multi.snd[i] = p_instance_ctrl->p_mutual_snd_corr[(element_id * CTSU_CFG_NUM_SUMULTI) + i];
            }

            for (i = 0; i < majority_mode_element_num; i++)
            {
                if (true == p_instance_ctrl->serial_tuning_enable)
                {
                    /* Serial Tuning Phase1 */
                    if ((0 == p_instance_ctrl->ctsuchtrc0) && (0 == p_instance_ctrl->ctsuchtrc1) &&
                        (0 == p_instance_ctrl->ctsuchtrc2) && (0 == p_instance_ctrl->ctsuchtrc3) &&
                        (0 == p_instance_ctrl->ctsuchtrc4))
                    {
                        break;
                    }
                }
                p_pri_data = (p_instance_ctrl->p_mutual_pri_data + (element_id * majority_mode_element_num) + i);
                p_snd_data = (p_instance_ctrl->p_mutual_snd_data + (element_id * majority_mode_element_num) + i);

                if (0 == p_instance_ctrl->average)
                {
                    /* Store corrected data in p_pri_data[i] for initial offset tuning */
                }
                else
                {
                    /* Store last moving averaged data */
                    average_pri = *p_pri_data;
                    average_snd = *p_snd_data;
  #if (CTSU_CFG_MAJORITY_MODE & CTSU_JUDGEMENT_MAJORITY_MODE)
                    if (p_instance_ctrl->p_ctsu_cfg->majority_mode == 1)
                    {
                        /* Skip the ctsu_correction_multi at Software JMM */
                        p_pri_data->int_data = multi.pri[i];
                        p_snd_data->int_data = multi.snd[i];
                    }
                    else
  #endif
                    {
                        /* Matching values */
  #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 0)
                        ctsu_correction_freq(&multi, multi.pri, multi.snd);
  #endif
                        ctsu_correction_multi(&multi, &(p_pri_data->int_data), &(p_snd_data->int_data));
                        *(p_instance_ctrl->p_selected_freq_mutual + element_id) = multi.selected_freq;
                    }

                    /* Update moving averaged data */
                    ctsu_moving_average(&average_pri, p_pri_data->int_data, p_instance_ctrl->average);
                    *p_pri_data = average_pri;
                    ctsu_moving_average(&average_snd, p_snd_data->int_data, p_instance_ctrl->average);
                    *p_snd_data = average_snd;
                }
            }
 #endif
        }
        else if (CTSU_MODE_MUTUAL_CFC_SCAN == p_instance_ctrl->md)
        {
 #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
  #if (CTSU_CFG_NUM_CFC != 0)
            if (CTSU_MODE_MUTUAL_CFC_SCAN == p_instance_ctrl->md)
            {
                num_rx = p_instance_ctrl->p_ctsu_cfg->num_rx;
                cfc_id = (uint16_t) (element_id % num_rx);
                if (0 == cfc_id)
                {
                    if (0 == element_id)
                    {
                        offset = 0;
                    }
                    else
                    {
                        offset = (uint16_t) (offset + (num_rx * CTSU_MUTUAL_BUF_SIZE));
                    }
                }
            }

            calc.cfc = cfc_ts_table[cfc_id];

            for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
            {
                ctsu_correction_calc(&p_instance_ctrl->p_mutual_pri_corr[(element_id * CTSU_CFG_NUM_SUMULTI) + i],
                                     p_instance_ctrl->p_mutual_raw[offset + cfc_id + (num_rx * i * 2)],
                                     &calc);

                multi.pri[i] = p_instance_ctrl->p_mutual_pri_corr[(element_id * CTSU_CFG_NUM_SUMULTI) + i];

                ctsu_correction_calc(&p_instance_ctrl->p_mutual_snd_corr[(element_id * CTSU_CFG_NUM_SUMULTI) + i],
                                     p_instance_ctrl->p_mutual_raw[offset + cfc_id + (num_rx * i * 2) + num_rx],
                                     &calc);

                multi.snd[i] = p_instance_ctrl->p_mutual_snd_corr[(element_id * CTSU_CFG_NUM_SUMULTI) + i];
            }

            p_pri_data = (p_instance_ctrl->p_mutual_pri_data + element_id);
            p_snd_data = (p_instance_ctrl->p_mutual_snd_data + element_id);

            if (0 == p_instance_ctrl->average)
            {
                /* Store corrected data in p_pri_data[i] for initial offset tuning */
                for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
                {
                    p_pri_data[i].int_data = multi.pri[i];
                }
            }
            else
            {
                /* Store last moving averaged data */
                average_pri = *p_pri_data;
                average_snd = *p_snd_data;

                /* Matching values */
                ctsu_correction_fleq(&multi, multi.pri, multi.snd);
                ctsu_correction_multi(&multi, &(p_pri_data->int_data), &(p_snd_data->int_data));
                *(p_instance_ctrl->p_selected_freq_mutual + element_id) = multi.selected_freq;

                /* Update moving averaged data */
                ctsu_moving_average(&average_pri, p_pri_data->int_data, p_instance_ctrl->average);
                *p_pri_data = average_pri;
                ctsu_moving_average(&average_snd, p_snd_data->int_data, p_instance_ctrl->average);
                *p_snd_data = average_snd;
            }
  #endif
 #endif
        }
        else
        {
        }
    }
}

#endif
#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 0)
/***********************************************************************************************************************
 * ctsu_correction_freq
 ***********************************************************************************************************************/
void ctsu_correction_freq (ctsu_correction_multi_t * p_multi, uint16_t * p_pri, uint16_t * p_snd)
{
    uint32_t i;
    int32_t  sumulti[CTSU_CFG_NUM_SUMULTI];
    int32_t  pri_calc[CTSU_CFG_NUM_SUMULTI];
    int32_t  snd_calc[CTSU_CFG_NUM_SUMULTI];
    int32_t  pri_total;
    int32_t  snd_total;

    sumulti[0] = CTSU_CFG_SUMULTI0 + 1;
 #if CTSU_CFG_NUM_SUMULTI >= 2
    sumulti[1] = CTSU_CFG_SUMULTI1 + 1;
 #endif
 #if CTSU_CFG_NUM_SUMULTI >= 3
    sumulti[2] = CTSU_CFG_SUMULTI2 + 1;
 #endif

    for (i = 1; i < CTSU_CFG_NUM_SUMULTI; i++)
    {
        pri_total   = (int32_t) (p_multi->pri[i] + p_multi->offset[i]);
        pri_calc[i] = (int32_t) (((pri_total * sumulti[0]) / sumulti[i]) - (int32_t) p_multi->offset[0]);
        p_pri[i]    = (uint16_t) pri_calc[i];
    }

    if (NULL == p_snd)
    {
        for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
        {
            p_multi->snd[i] = 0;
        }
    }
    else
    {
        for (i = 1; i < CTSU_CFG_NUM_SUMULTI; i++)
        {
            snd_total   = (int32_t) (p_multi->snd[i] + p_multi->offset[i]);
            snd_calc[i] = (int32_t) (((snd_total * sumulti[0]) / sumulti[i]) - (int32_t) p_multi->offset[0]);
            p_snd[i]    = (uint16_t) snd_calc[i];
        }
    }
}
 #endif

/***********************************************************************************************************************
 * ctsu_correction_multi
 ***********************************************************************************************************************/
void ctsu_correction_multi (ctsu_correction_multi_t * p_multi, uint16_t * p_pri, uint16_t * p_snd)
{
    int32_t  add_pri;
    int32_t  add_snd;
 #if CTSU_CFG_NUM_SUMULTI >= 3
    uint32_t i;
    int32_t  pri_calc[CTSU_CFG_NUM_SUMULTI];
    int32_t  snd_calc[CTSU_CFG_NUM_SUMULTI];
    int32_t  diff[CTSU_CFG_NUM_SUMULTI];
 #endif

 #if CTSU_CFG_NUM_SUMULTI == 1
    add_pri = p_multi->pri[0];
    add_snd = p_multi->snd[0];
 #endif
 #if CTSU_CFG_NUM_SUMULTI == 2
    add_pri = p_multi->pri[0] + p_multi->pri[1];
    add_snd = p_multi->snd[0] + p_multi->snd[1];
 #endif
 #if CTSU_CFG_NUM_SUMULTI >= 3
    for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
    {
        pri_calc[i] = (int32_t) p_multi->pri[i];
        if (NULL == p_snd)
        {
            snd_calc[i] = 0;
        }
        else
        {
            snd_calc[i] = (int32_t) p_multi->snd[i];
        }
    }

    diff[0] = (snd_calc[1] - pri_calc[1]) - (snd_calc[0] - pri_calc[0]);
    diff[1] = (snd_calc[2] - pri_calc[2]) - (snd_calc[0] - pri_calc[0]);
    diff[2] = (snd_calc[2] - pri_calc[2]) - (snd_calc[1] - pri_calc[1]);

    for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
    {
        if (diff[i] < 0)
        {
            diff[i] = -diff[i];
        }
    }

    /* Normally select freq0 and freq1 addition */
    /* If the following conditions are false, it is estimated to be a noise environment. */
    /* Compare with the combination with the other frequency difference (including margin). */
    if ((diff[0] < (diff[1] * 2)) && (diff[0] < ((diff[2] * 3) / 2)))
    {
        add_pri = pri_calc[0] + pri_calc[1];
        add_snd = snd_calc[0] + snd_calc[1];

        p_multi->selected_freq = 0x3;
    }
    else
    {
        if (diff[1] < diff[2])
        {
            add_pri = pri_calc[0] + pri_calc[2];
            add_snd = snd_calc[0] + snd_calc[2];

            p_multi->selected_freq = 0x5;
        }
        else
        {
            add_pri = pri_calc[1] + pri_calc[2];
            add_snd = snd_calc[1] + snd_calc[2];

            p_multi->selected_freq = 0x6;
        }
    }
 #endif

    if (CTSU_COUNT_MAX < add_pri)
    {
        *p_pri = CTSU_COUNT_MAX;
    }
    else
    {
        *p_pri = (uint16_t) add_pri;
    }

    if (NULL != p_snd)
    {
        if (CTSU_COUNT_MAX < add_snd)
        {
            *p_snd = CTSU_COUNT_MAX;
        }
        else
        {
            *p_snd = (uint16_t) add_snd;
        }
    }
}

 #if (CTSU_CFG_NUM_CFC != 0)

/***********************************************************************************************************************
 * ctsu_corrcfc_process
 ***********************************************************************************************************************/
void ctsu_corrcfc_process (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    uint8_t            i;
    uint8_t            j;
    uint8_t            ts_id;
    uint8_t            index;
    uint64_t           new_bitmap;
    ctsu_cfg_t const * p_cfg;
    uint16_t           base_value;
    uint16_t           base_conv_dac;
    int32_t            x0;
    int32_t            x1;
    int32_t            y0;

    g_ctsu_corrcfc_info.status = CTSU_CORRECTION_RUN;

    p_cfg = p_instance_ctrl->p_ctsu_cfg;
    index = g_ctsu_corrcfc_info.index;

    /* Get CFC-Rx bitmap at this instance. */
    p_instance_ctrl->cfc_rx_bitmap =
        ((uint64_t) (p_cfg->ctsuchac0 & (~p_cfg->ctsuchtrc0)) |
         ((uint64_t) (p_cfg->ctsuchac1 & (~p_cfg->ctsuchtrc1)) << (CTSU_CORRCFC_SHIFT8)) |
         ((uint64_t) (p_cfg->ctsuchac2 & (~p_cfg->ctsuchtrc2)) << (CTSU_CORRCFC_SHIFT8 * 2)) |
         ((uint64_t) (p_cfg->ctsuchac3 & (~p_cfg->ctsuchtrc3)) << (CTSU_CORRCFC_SHIFT8 * 3)) |
         ((uint64_t) (p_cfg->ctsuchac4 & (~p_cfg->ctsuchtrc4)) << (CTSU_CORRCFC_SHIFT8 * 4)));

    /* Get RX bitmap at this correction. */
    new_bitmap = p_instance_ctrl->cfc_rx_bitmap & (~g_ctsu_corrcfc_info.stored_rx_bitmap);

    /* Get TS id and number of TS in this instance. */
    g_ctsu_corrcfc_info.num_ts = 0;
    for (ts_id = 0; ts_id < CTSU_CORRCFC_TS_MAX; ts_id++)
    {
        if (1 == ((new_bitmap >> ts_id) & 1))
        {
            g_ctsu_corrcfc_info.ts_table[index + g_ctsu_corrcfc_info.num_ts] = ts_id;
            g_ctsu_corrcfc_info.num_ts++;
        }
    }

    /* Add the bitmap of this instance to the entire CFC correction information. */
    g_ctsu_corrcfc_info.stored_rx_bitmap |= p_instance_ctrl->cfc_rx_bitmap;

    /* Write Channel setting */
    R_CTSU->CTSUCHAC0 = (uint8_t) (new_bitmap);
    R_CTSU->CTSUCHAC1 = (uint8_t) (new_bitmap >> (CTSU_CORRCFC_SHIFT8));
    R_CTSU->CTSUCHAC2 = (uint8_t) (new_bitmap >> (CTSU_CORRCFC_SHIFT8 * 2));
    R_CTSU->CTSUCHAC3 = (uint8_t) (new_bitmap >> (CTSU_CORRCFC_SHIFT8 * 3));
    R_CTSU->CTSUCHAC4 = (uint8_t) (new_bitmap >> (CTSU_CORRCFC_SHIFT8 * 4));

    /* Set register of CFC self mode */
    R_CTSU->CTSUCRA_b.MD0  = 1;
    R_CTSU->CTSUCRA_b.MD1  = 0;
    R_CTSU->CTSUCRA_b.MD2  = 1;
    R_CTSU->CTSUMCH_b.MCA0 = 1;
    R_CTSU->CTSUMCH_b.MCA1 = 0;
    R_CTSU->CTSUMCH_b.MCA2 = 0;
    R_CTSU->CTSUMCH_b.MCA3 = 0;

    /* Set time of measurement */
    g_ctsu_corrcfc_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10);

    /* Step1 : Measure the current input to the CFC-ICO by passing bias current. */
    /*         The theoretical value of the current is 6uA. */
    ctsu_corrcfc_measurement(p_instance_ctrl, &g_ctsu_corrcfc_info.base_value[index], 1);

    /* Step2 : Measure by inputting each constant current from internal DAC to CFC-ICO. */
    R_CTSU->CTSUCRB_b.SSCNT     = 0;
    R_CTSU->CTSUCRA_b.LOAD      = 1;
    R_CTSU->CTSUCALIB_b.CCOCLK  = 0;
    R_CTSU->CTSUCALIB_b.CFCMODE = 1;

    for (j = 0; j < CTSU_CORRCFC_POINT_NUM; j++)
    {
        R_CTSU->CTSUCRA_b.SDPSEL = 0;
        R_CTSU->CTSUSUCLK0       = (uint16_t) (((j + CTSU_CORRCFC_CENTER_POINT) * CTSU_CORRECTION_SUMULTI) - 1);
        R_CTSU->CTSUCRA_b.SDPSEL = 1;

        ctsu_corrcfc_measurement(p_instance_ctrl, &g_ctsu_corrcfc_info.dac_value[index][j], CTSU_CORRCFC_POINT_NUM);
    }

    /* Reset register */
    R_CTSU->CTSUCALIB_b.CFCMODE = 0;
    R_CTSU->CTSUCALIB_b.CCOCLK  = 1;

    /* Step3 : Calculate the error between step1 and step2. */
    for (i = index; i < (index + g_ctsu_corrcfc_info.num_ts); i++)
    {
        /* Linear interpolation calculation */
        base_value = g_ctsu_corrcfc_info.base_value[i];
        j          = 1;
        while (1)
        {
            if ((base_value < g_ctsu_corrcfc_info.dac_value[index][j]) || ((CTSU_CORRCFC_POINT_NUM - 1) == j))
            {
                y0 = (uint16_t) (CTSU_CORRECTION_STD_UNIT * (j + CTSU_CORRCFC_CENTER_POINT));
                x0 = g_ctsu_corrcfc_info.dac_value[index][j];
                x1 = g_ctsu_corrcfc_info.dac_value[index][j - 1];
                break;
            }

            j++;
        }

        base_conv_dac = (uint16_t) (y0 - ((CTSU_CORRECTION_STD_UNIT * (x0 - base_value)) / (x0 - x1)));

        /* Error rate calculation */
        g_ctsu_corrcfc_info.error_rate[i] =
            (uint16_t) (((CTSU_CORRECTION_STD_UNIT * 4) << CTSU_SHIFT_AMOUNT) / base_conv_dac);

        for (j = 0; j < CTSU_CORRCFC_POINT_NUM; j++)
        {
            g_ctsu_corrcfc_info.ref_value[i][j] =
                (uint16_t) ((CTSU_CORRECTION_STD_UNIT * (j + CTSU_CORRCFC_CENTER_POINT) *
                             g_ctsu_corrcfc_info.error_rate[i]) >> CTSU_SHIFT_AMOUNT);
        }
    }

    g_ctsu_corrcfc_info.index = (uint8_t) (index + g_ctsu_corrcfc_info.num_ts);

    g_ctsu_corrcfc_info.status = CTSU_CORRECTION_COMPLETE;
}

/***********************************************************************************************************************
 * ctsu_corrcfc_measurement
 ***********************************************************************************************************************/
void ctsu_corrcfc_measurement (ctsu_instance_ctrl_t * const p_instance_ctrl, uint16_t * data, uint8_t point_num)
{
    uint16_t i;
    uint8_t  cfc_id;
    uint32_t sum[CTSU_CFG_NUM_CFC];

    /* initialize sum array */
    for (cfc_id = 0; cfc_id < CTSU_CFG_NUM_CFC; cfc_id++)
    {
        sum[cfc_id] = 0;
    }

    for (i = 0; i < CTSU_CORRECTION_AVERAGE; i++)
    {
  #if (CTSU_CFG_DTC_SUPPORT_ENABLE == 1)
        ctsu_transfer_configure(p_instance_ctrl);
  #else
        FSP_PARAMETER_NOT_USED(p_instance_ctrl);
  #endif
        p_instance_ctrl->state = CTSU_STATE_SCANNING;
        R_CTSU->CTSUCR0       |= 0x01;
        while (p_instance_ctrl->state != CTSU_STATE_SCANNED)
        {
        }

        /* Get sum */
        for (cfc_id = 0; cfc_id < CTSU_CFG_NUM_CFC; cfc_id++)
        {
            sum[cfc_id] += g_ctsu_corrcfc_info.scanbuf[cfc_id];
        }
    }

    /* Average sum data */
    for (cfc_id = 0; cfc_id < g_ctsu_corrcfc_info.num_ts; cfc_id++)
    {
        *(data + (cfc_id * point_num)) = (uint16_t) (sum[cfc_id] / CTSU_CORRECTION_AVERAGE);
    }
}

 #endif

 #if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 1)
/***********************************************************************************************************************
 * ctsu_auto_correction_register_set
 ***********************************************************************************************************************/
void ctsu_auto_correction_register_set (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    uint32_t j;
    uint32_t corr_original;
    uint8_t  corr_integer;
    uint16_t corr_decimal;
    uint16_t corr_dac;

    /* Enter the correction factor and dac_value in CTSUSCNTACT for 12 correction tables */
    for (j = 0; j < CTSU_CORRECTION_POINT_NUM; j++)
    {
        /* The correction coefficient is shifted to the right by 12 bits after the decimal point and treated as an integer. */
        corr_original = ((uint32_t)g_ctsu_correction_info.ref_value[p_instance_ctrl->range][j] << CTSU_AUTO_DECIMAL_POINT_NUM) / g_ctsu_correction_info.dac_value[j];
        /* Extract an integer of the correction factor */
        corr_integer = (corr_original >> CTSU_AUTO_DECIMAL_POINT_NUM) & 0xF;
        /* Extract a decimal of the correction factor */
        corr_decimal = corr_original & 0xFFF;

        corr_dac = g_ctsu_correction_info.dac_value[j];

        /* Table 0 is set to 0x0000 and table 11 is set to 0xFFFF */
        if (0 == j)
        {
            corr_dac = 0x0000;
        }
        else if ((CTSU_CORRECTION_POINT_NUM - 1) == j)
        {
            corr_dac = CTSU_COUNT_MAX;
        }

        CTSU.CTSUSCNTACT.LONG = (corr_dac << 16) | (corr_integer << 12) | (corr_decimal);
    }
}
 #endif

 #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
/***********************************************************************************************************************
* ctsu_multiclock_auto_correction_register_set
***********************************************************************************************************************/
void ctsu_multiclock_auto_correction_register_set (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    uint16_t element_id;
    uint16_t mostly_constant;
    int32_t  mcact_coef_int32_1;
    int16_t  mcact_coef_int16_1;
    int32_t  mcact_coef_int32_2;
    int16_t  mcact_coef_int16_2;

    for (element_id = 0; element_id < p_instance_ctrl->num_elements; element_id++)
    {
        mostly_constant = CTSU_MULTICLOCK_AUTO_CORRECTION_BASE_VALUE * ((p_instance_ctrl->p_ctsu_cfg->p_elements[element_id].snum + 1) / (CTSU_SNUM_RECOMMEND + 1));

        mcact_coef_int32_1 = (((CTSU_CFG_SUMULTI1 + 1) * (p_instance_ctrl->p_ctsuwr[(element_id * CTSU_CFG_NUM_SUMULTI) + 1].ctsuso & CTSU_TUNING_MAX) / (CTSU_CFG_SUMULTI0 + 1)) - (p_instance_ctrl->p_ctsuwr[(element_id * CTSU_CFG_NUM_SUMULTI)].ctsuso & CTSU_TUNING_MAX)) * mostly_constant;
        mcact_coef_int16_1 = (mcact_coef_int32_1 >> p_instance_ctrl->range) >> 10;
        mcact_coef_int16_1 = -(mcact_coef_int16_1);

        mcact_coef_int32_2 = (((CTSU_CFG_SUMULTI2 + 1) * (p_instance_ctrl->p_ctsuwr[(element_id * CTSU_CFG_NUM_SUMULTI) + 2].ctsuso & CTSU_TUNING_MAX) / (CTSU_CFG_SUMULTI0 + 1)) - (p_instance_ctrl->p_ctsuwr[(element_id * CTSU_CFG_NUM_SUMULTI)].ctsuso & CTSU_TUNING_MAX)) * mostly_constant;
        mcact_coef_int16_2 = (mcact_coef_int32_2 >> p_instance_ctrl->range) >> 10;
        mcact_coef_int16_2 = -(mcact_coef_int16_2);

        if (0 == p_instance_ctrl->p_ctsu_cfg->ajfen)
        {
            p_instance_ctrl->p_mcact1[(element_id * CTSU_CFG_NUM_SUMULTI) + 0] = 0;
            p_instance_ctrl->p_mcact1[(element_id * CTSU_CFG_NUM_SUMULTI) + 1] = ((uint32_t)mcact_coef_int16_1 << 16) | (p_instance_ctrl->p_ctsuwr[(element_id * CTSU_CFG_NUM_SUMULTI) + 1].ctsuso & CTSU_TUNING_MAX);
            p_instance_ctrl->p_mcact1[(element_id * CTSU_CFG_NUM_SUMULTI) + 2] = ((uint32_t)mcact_coef_int16_2 << 16) | (p_instance_ctrl->p_ctsuwr[(element_id * CTSU_CFG_NUM_SUMULTI) + 2].ctsuso & CTSU_TUNING_MAX);
        }
        else
        {
            p_instance_ctrl->p_mcact1[element_id] = ((uint32_t)mcact_coef_int16_1 << 16) | (p_instance_ctrl->p_ctsuwr[(element_id * CTSU_CFG_NUM_SUMULTI) + 1].ctsuso & CTSU_TUNING_MAX);
            p_instance_ctrl->p_mcact2[element_id] = ((uint32_t)mcact_coef_int16_2 << 16) | (p_instance_ctrl->p_ctsuwr[(element_id * CTSU_CFG_NUM_SUMULTI) + 2].ctsuso & CTSU_TUNING_MAX);
        }
    }

    /* When using HW VMM, pack and re-store the first frequency settings. The second and third frequencies use mcact. */
    if (1 == p_instance_ctrl->p_ctsu_cfg->ajfen)
    {
        for (element_id = 1; element_id < p_instance_ctrl->num_elements; element_id++)
        {
            p_instance_ctrl->p_ctsuwr[element_id].ctsuso = p_instance_ctrl->p_ctsuwr[element_id * CTSU_CFG_NUM_SUMULTI].ctsuso;
        }
    }
}
 #endif

 #if (CTSU_CFG_AUTO_JUDGE_ENABLE == 1)
/***********************************************************************************************************************
 * ctsu_auto_judge_threshold_calc
 ***********************************************************************************************************************/
void ctsu_auto_judge_threshold_calc (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    uint8_t  element_id;
    uint8_t  i;
  #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
    int16_t  ajthr_h;
    int16_t  ajthr_l;
  #endif

    for (element_id = 0; element_id < p_instance_ctrl->num_elements; element_id++)
    {
  #if (CTSU_CFG_AUTO_MULTI_CLOCK_CORRECTION_ENABLE == 1)
        if (1 == p_instance_ctrl->p_ctsu_cfg->majirimd)
        {
   #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
            if(CTSU_MODE_SELF_MULTI_SCAN == p_instance_ctrl->p_ctsu_cfg->md)
            {
   #endif
                p_instance_ctrl->p_auto_judge[element_id].ajthr = ((uint32_t)p_instance_ctrl->p_ctsu_cfg->p_ctsu_auto_buttons[element_id].threshold << 16) |
                        (p_instance_ctrl->p_ctsu_cfg->p_ctsu_auto_buttons[element_id].threshold - p_instance_ctrl->p_ctsu_cfg->p_ctsu_auto_buttons[element_id].hysteresis);
   #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
            }
            else
            {
                ajthr_h = -(p_instance_ctrl->p_ctsu_cfg->p_ctsu_auto_buttons[element_id].threshold) + p_instance_ctrl->p_ctsu_cfg->p_ctsu_auto_buttons[element_id].hysteresis;
                ajthr_l = -(p_instance_ctrl->p_ctsu_cfg->p_ctsu_auto_buttons[element_id].threshold);
                p_instance_ctrl->p_auto_judge[element_id].ajthr = ((uint32_t)ajthr_h << 16) | (ajthr_l & 0x0000FFFF);
            }
   #endif
        }
        else
  #endif
        {
            for (i = 0; i < CTSU_CFG_NUM_SUMULTI; i++)
            {
  #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
                if(CTSU_MODE_SELF_MULTI_SCAN == p_instance_ctrl->p_ctsu_cfg->md)
                {
  #endif
                    p_instance_ctrl->p_auto_judge[(element_id * CTSU_MAJORITY_MODE_ELEMENTS) + i].ajthr =
                        ((uint32_t)p_instance_ctrl->p_ctsu_cfg->p_ctsu_auto_buttons[(element_id * CTSU_MAJORITY_MODE_ELEMENTS) + i].threshold << 16) |
                        (p_instance_ctrl->p_ctsu_cfg->p_ctsu_auto_buttons[(element_id * CTSU_MAJORITY_MODE_ELEMENTS) + i].threshold -
                        p_instance_ctrl->p_ctsu_cfg->p_ctsu_auto_buttons[(element_id * CTSU_MAJORITY_MODE_ELEMENTS) + i].hysteresis);
  #if (CTSU_CFG_NUM_MUTUAL_ELEMENTS != 0)
                }
                else
                {
                    ajthr_h = -(p_instance_ctrl->p_ctsu_cfg->p_ctsu_auto_buttons[(element_id * CTSU_MAJORITY_MODE_ELEMENTS) + i].threshold) +
                            p_instance_ctrl->p_ctsu_cfg->p_ctsu_auto_buttons[(element_id * CTSU_MAJORITY_MODE_ELEMENTS) + i].hysteresis;
                    ajthr_l = -(p_instance_ctrl->p_ctsu_cfg->p_ctsu_auto_buttons[(element_id * CTSU_MAJORITY_MODE_ELEMENTS) + i].threshold);
                    p_instance_ctrl->p_auto_judge[(element_id * CTSU_MAJORITY_MODE_ELEMENTS) + i].ajthr =
                              ((uint32_t)ajthr_h << 16) | (ajthr_l & 0x0000FFFF);
                }
  #endif
            }
        }
    }
}
 #endif

#endif

/*******************************************************************************************************************//**
 * @brief Diagnosis the CTSU peripheral.
 * Implements @ref ctsu_api_t::diagnosis.
 *
 *
 * Example:
 * @snippet r_ctsu_example.c R_CTSU_Diagnosis
 *
 * @retval FSP_SUCCESS                             CTSU successfully configured.
 * @retval FSP_ERR_ASSERTION                       Null pointer passed as a parameter.
 * @retval FSP_ERR_NOT_OPEN                        Module is not open.
 * @retval FSP_ERR_CTSU_NOT_GET_DATA               The previous data has not been retrieved by DataGet.
 * @retval FSP_ERR_CTSU_DIAG_LDO_OVER_VOLTAGE      Diagnosis of LDO over voltage failed.
 * @retval FSP_ERR_CTSU_DIAG_CCO_HIGH              Diagnosis of CCO into 19.2uA failed.
 * @retval FSP_ERR_CTSU_DIAG_CCO_LOW               Diagnosis of CCO into 2.4uA failed.
 * @retval FSP_ERR_CTSU_DIAG_SSCG                  Diagnosis of SSCG frequency failed.
 * @retval FSP_ERR_CTSU_DIAG_DAC                   Diagnosis of non-touch count value failed.
 * @retval FSP_ERR_CTSU_DIAG_OUTPUT_VOLTAGE        Diagnosis of LDO output voltage failed.
 * @retval FSP_ERR_CTSU_DIAG_OVER_VOLTAGE          Diagnosis of over voltage detection circuit failed.
 * @retval FSP_ERR_CTSU_DIAG_OVER_CURRENT          Diagnosis of over current detection circuit failed.
 * @retval FSP_ERR_CTSU_DIAG_LOAD_RESISTANCE       Diagnosis of LDO internal resistance value failed.
 * @retval FSP_ERR_CTSU_DIAG_CURRENT_SOURCE        Diagnosis of LDO internal resistance value failed.
 * @retval FSP_ERR_CTSU_DIAG_SENSCLK_GAIN          Diagnosis of SENSCLK frequency gain failed.
 * @retval FSP_ERR_CTSU_DIAG_SUCLK_GAIN            Diagnosis of SUCLK frequency gain failed.
 * @retval FSP_ERR_CTSU_DIAG_CLOCK_RECOVERY        Diagnosis of SUCLK clock recovery function failed.
 * @retval FSP_ERR_CTSU_DIAG_CFC_GAIN              Diagnosis of CFC oscillator gain failed.
 **********************************************************************************************************************/

fsp_err_t R_CTSU_Diagnosis (ctsu_ctrl_t * const p_ctrl)
{
#if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
    fsp_err_t diag_err;
#endif
    ctsu_instance_ctrl_t * p_instance_ctrl = (ctsu_instance_ctrl_t *) p_ctrl;

#if (CTSU_CFG_PARAM_CHECKING_ENABLE == 1)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN(CTSU_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    FSP_ERROR_RETURN(CTSU_STATE_SCANNED != p_instance_ctrl->state, FSP_ERR_CTSU_NOT_GET_DATA);

#if (BSP_FEATURE_CTSU_VERSION == 1)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
    if (CTSU_DIAG_COMPLETE == g_ctsu_diag_info.state)
    {
        diag_err = ctsu_diag_ldo_over_voltage_result();
        if (FSP_SUCCESS != diag_err)
        {
            g_ctsu_diag_info.state = CTSU_DIAG_INIT;

            return FSP_ERR_CTSU_DIAG_LDO_OVER_VOLTAGE;
        }

        diag_err = ctsu_diag_oscillator_high_result();
        if (FSP_SUCCESS != diag_err)
        {
            g_ctsu_diag_info.state = CTSU_DIAG_INIT;

            return FSP_ERR_CTSU_DIAG_CCO_HIGH;
        }

        diag_err = ctsu_diag_oscillator_low_result();
        if (FSP_SUCCESS != diag_err)
        {
            g_ctsu_diag_info.state = CTSU_DIAG_INIT;

            return FSP_ERR_CTSU_DIAG_CCO_LOW;
        }

        diag_err = ctsu_diag_sscg_result();
        if (FSP_SUCCESS != diag_err)
        {
            g_ctsu_diag_info.state = CTSU_DIAG_INIT;

            return FSP_ERR_CTSU_DIAG_SSCG;
        }

        diag_err = ctsu_diag_dac_result();
        if (FSP_SUCCESS != diag_err)
        {
            g_ctsu_diag_info.state = CTSU_DIAG_INIT;

            return FSP_ERR_CTSU_DIAG_DAC;
        }

        g_ctsu_diag_info.state = CTSU_DIAG_INIT;
    }
 #endif
#endif

#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
    if (CTSU_DIAG_COMPLETE == g_ctsu_diag_info.state)
    {
        ctsu_diag_regi_store2();

        g_ctsu_diag_info.state = CTSU_DIAG_OUTPUT_VOLTAGE;
        diag_err               = ctsu_diag_output_voltage_result();
        if (FSP_SUCCESS != diag_err)
        {
            ctsu_diag_regi_restore2();
            g_ctsu_diag_info.state = CTSU_DIAG_INIT;

            return FSP_ERR_CTSU_DIAG_OUTPUT_VOLTAGE;
        }

        g_ctsu_diag_info.state = CTSU_DIAG_OVER_VOLTAGE;
        diag_err               = ctsu_diag_over_voltage_result();
        if (FSP_SUCCESS != diag_err)
        {
            ctsu_diag_regi_restore2();
            g_ctsu_diag_info.state = CTSU_DIAG_INIT;

            return FSP_ERR_CTSU_DIAG_OVER_VOLTAGE;
        }

        g_ctsu_diag_info.state = CTSU_DIAG_OVER_CURRENT;
        diag_err               = ctsu_diag_over_current_result();
        if (FSP_SUCCESS != diag_err)
        {
            ctsu_diag_regi_restore2();
            g_ctsu_diag_info.state = CTSU_DIAG_INIT;

            return FSP_ERR_CTSU_DIAG_OVER_CURRENT;
        }

        g_ctsu_diag_info.state = CTSU_DIAG_LOAD_RESISTANCE;
        diag_err               = ctsu_diag_load_resistance_result();
        if (FSP_SUCCESS != diag_err)
        {
            ctsu_diag_regi_restore2();
            g_ctsu_diag_info.state = CTSU_DIAG_INIT;

            return FSP_ERR_CTSU_DIAG_LOAD_RESISTANCE;
        }

        g_ctsu_diag_info.state = CTSU_DIAG_CURRENT_SOURCE;
        diag_err               = ctsu_diag_current_source_result();
        if (FSP_SUCCESS != diag_err)
        {
            ctsu_diag_regi_restore2();
            g_ctsu_diag_info.state = CTSU_DIAG_INIT;

            return FSP_ERR_CTSU_DIAG_CURRENT_SOURCE;
        }

        g_ctsu_diag_info.state = CTSU_DIAG_SENSCLK;
        diag_err               = ctsu_diag_cco_gain_result();
        if (FSP_SUCCESS != diag_err)
        {
            ctsu_diag_regi_restore2();
            g_ctsu_diag_info.state = CTSU_DIAG_INIT;

            return FSP_ERR_CTSU_DIAG_SENSCLK_GAIN;
        }

        g_ctsu_diag_info.state = CTSU_DIAG_SUCLK;
        diag_err               = ctsu_diag_cco_gain_result();
        if (FSP_SUCCESS != diag_err)
        {
            ctsu_diag_regi_restore2();
            g_ctsu_diag_info.state = CTSU_DIAG_INIT;

            return FSP_ERR_CTSU_DIAG_SUCLK_GAIN;
        }

        g_ctsu_diag_info.state = CTSU_DIAG_CLOCK_RECOVERY;
        diag_err               = ctsu_diag_clock_recovery_result();
        if (FSP_SUCCESS != diag_err)
        {
            ctsu_diag_regi_restore2();
            g_ctsu_diag_info.state = CTSU_DIAG_INIT;

            return FSP_ERR_CTSU_DIAG_CLOCK_RECOVERY;
        }

  #if (CTSU_CFG_NUM_CFC != 0)
        g_ctsu_diag_info.state = CTSU_DIAG_CFC;
        diag_err               = ctsu_diag_cfc_gain_result();
        if (FSP_SUCCESS != diag_err)
        {
            ctsu_diag_regi_restore2();
            g_ctsu_diag_info.state = CTSU_DIAG_INIT;

            return FSP_ERR_CTSU_DIAG_CFC_GAIN;
        }
  #endif

        ctsu_diag_regi_restore2();
        g_ctsu_diag_info.state = CTSU_DIAG_INIT;
    }
 #endif
#endif

    /* if all checks passed to this point, return success */
    return FSP_SUCCESS;
}

#if (BSP_FEATURE_CTSU_VERSION == 1)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)

/***********************************************************************************************************************
 * ctsu_diag_scan_start1
 ***********************************************************************************************************************/
static void ctsu_diag_scan_start1 (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    if (CTSU_DIAG_INIT == g_ctsu_diag_info.state)
    {
        g_ctsu_diag_info.state = CTSU_DIAG_OVER_VOLTAGE;
    }

    /* ctsu normal scan register save */
    g_ctsu_diag_reg.ctsucr0    = CTSU.CTSUCR0.BYTE;
    g_ctsu_diag_reg.ctsucr1    = CTSU.CTSUCR1.BYTE;
    g_ctsu_diag_reg.ctsusdprs  = CTSU.CTSUSDPRS.BYTE;
    g_ctsu_diag_reg.ctsusst    = CTSU.CTSUSST.BYTE;
    g_ctsu_diag_reg.ctsuchac0  = CTSU.CTSUCHAC0.BYTE;
    g_ctsu_diag_reg.ctsuchac1  = CTSU.CTSUCHAC1.BYTE;
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 3)
    g_ctsu_diag_reg.ctsuchac2  = CTSU.CTSUCHAC2.BYTE;
#endif
    g_ctsu_diag_reg.ctsuchtrc0 = CTSU.CTSUCHTRC0.BYTE;
    g_ctsu_diag_reg.ctsuchtrc1 = CTSU.CTSUCHTRC1.BYTE;
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 3)
    g_ctsu_diag_reg.ctsuchtrc2 = CTSU.CTSUCHTRC2.BYTE;
#endif
    g_ctsu_diag_reg.ctsudclkc  = CTSU.CTSUDCLKC.BYTE;
    g_ctsu_diag_reg.ctsuerrs   = CTSU.CTSUERRS.WORD;

    /* scan register setting */
    if (CTSU_DIAG_OVER_VOLTAGE == g_ctsu_diag_info.state)
    {
        ctsu_diag_ldo_over_voltage_scan_start();
    }

    if (CTSU_DIAG_CCO_HIGH == g_ctsu_diag_info.state)
    {
        ctsu_diag_oscillator_high_scan_start();
    }

    if (CTSU_DIAG_CCO_LOW == g_ctsu_diag_info.state)
    {
        ctsu_diag_oscillator_low_scan_start();
    }

    if (CTSU_DIAG_SSCG == g_ctsu_diag_info.state)
    {
        ctsu_diag_sscg_scan_start();
    }

    if (CTSU_DIAG_DAC == g_ctsu_diag_info.state)
    {
        ctsu_diag_dac_scan_start(p_instance_ctrl);
    }
}

/***********************************************************************************************************************
 * ctsu_diag_data_get1
 ***********************************************************************************************************************/
static fsp_err_t ctsu_diag_data_get1 (void)
{
    fsp_err_t err;

    /* data get */
    if (CTSU_DIAG_OVER_VOLTAGE == g_ctsu_diag_info.state)
    {
        ctsu_diag_ldo_over_voltage_data_get();

        g_ctsu_diag_info.state = CTSU_DIAG_CCO_HIGH;
    }
    else if (CTSU_DIAG_CCO_HIGH == g_ctsu_diag_info.state)
    {
        ctsu_diag_oscillator_high_data_get();

        g_ctsu_diag_info.state = CTSU_DIAG_CCO_LOW;
    }
    else if (CTSU_DIAG_CCO_LOW == g_ctsu_diag_info.state)
    {
        ctsu_diag_oscillator_low_data_get();

        g_ctsu_diag_info.state = CTSU_DIAG_SSCG;
    }
    else if (CTSU_DIAG_SSCG == g_ctsu_diag_info.state)
    {
        ctsu_diag_sscg_data_get();

        g_ctsu_diag_info.state = CTSU_DIAG_DAC;
    }
    else if (CTSU_DIAG_DAC == g_ctsu_diag_info.state)
    {
        ctsu_diag_dac_data_get();
        if (CTSU_TUNING_INCOMPLETE == g_ctsu_diag_info.tuning)
        {
            g_ctsu_diag_info.state = CTSU_DIAG_DAC;
        }
        else
        {
            g_ctsu_diag_info.loop_count++;
            if (6 <= g_ctsu_diag_info.loop_count)
            {
                g_ctsu_diag_info.state      = CTSU_DIAG_COMPLETE;
                g_ctsu_diag_info.loop_count = 0;
            }
        }
    }
    else
    {
    }

    /* register restore */
    CTSU.CTSUCR0.BYTE      = g_ctsu_diag_reg.ctsucr0;
    CTSU.CTSUCR1.BYTE      = g_ctsu_diag_reg.ctsucr1;
    CTSU.CTSUSDPRS.BYTE    = g_ctsu_diag_reg.ctsusdprs;
    CTSU.CTSUSST.BYTE      = g_ctsu_diag_reg.ctsusst;
    CTSU.CTSUCHAC0.BYTE    = g_ctsu_diag_reg.ctsuchac0;
    CTSU.CTSUCHAC1.BYTE    = g_ctsu_diag_reg.ctsuchac1;
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 3)
    CTSU.CTSUCHAC2.BYTE    = g_ctsu_diag_reg.ctsuchac2;
#endif
    CTSU.CTSUCHTRC0.BYTE   = g_ctsu_diag_reg.ctsuchtrc0;
    CTSU.CTSUCHTRC1.BYTE   = g_ctsu_diag_reg.ctsuchtrc1;
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 3)
    CTSU.CTSUCHTRC2.BYTE   = g_ctsu_diag_reg.ctsuchtrc2;
#endif
    CTSU.CTSUDCLKC.BYTE    = g_ctsu_diag_reg.ctsudclkc;
    CTSU.CTSUERRS.WORD = 0;

    if (CTSU_DIAG_COMPLETE == g_ctsu_diag_info.state)
    {
        err = FSP_SUCCESS;
    }
    else
    {
        err = FSP_ERR_CTSU_DIAG_NOT_YET;
    }

    return err;
}

static void ctsu_diag_ldo_over_voltage_scan_start (void)
{
    uint32_t pclkb_mhz;
    uint32_t ctsu_sdpa;

    /* Set power on */
    CTSU.CTSUCR1.BYTE = ((CTSU_CFG_LOW_VOLTAGE_MODE << 2) | (CTSU_CSW_ON << 1) | CTSU_PON_ON);

    /* Synchronous Noise Reduction Setting */
    CTSU.CTSUSDPRS.BYTE = ((CTSU_SOFF_ON << 6) | (CTSU_PRMODE_62_PULSES << 4) | CTSU_PRRATIO_RECOMMEND);

    /* High Pass Noise Reduction- ALWAYS 0x30 as per HW Manual */
    CTSU.CTSUDCLKC.BYTE = ((CTSU_SSCNT << 4) | CTSU_SSMOD);

    /* Sensor Stabilization- ALWAYS 0x10 as per HW Manual */
    CTSU.CTSUSST.BYTE = (CTSU_SST_RECOMMEND);

    /* Since CLK is rewritten by correction, set here. */
    CTSU.CTSUCR1.BYTE |= (uint8_t) (CTSU_CFG_PCLK_DIVISION << 4);

    pclkb_mhz = BSP_PCLKB_HZ / CTSU_PCLKB_FREQ_MHZ;
    if (CTSU_PCLKB_FREQ_RANGE1 >= pclkb_mhz)
    {
        CTSU.CTSUCR1.BYTE |= (0 << 4);
        ctsu_sdpa          = pclkb_mhz - 1;
    }
    else if ((CTSU_PCLKB_FREQ_RANGE1 < pclkb_mhz) && (CTSU_PCLKB_FREQ_RANGE2 >= pclkb_mhz))
    {
        CTSU.CTSUCR1.BYTE |= (1 << 4);
        ctsu_sdpa          = (pclkb_mhz / 2) - 1;
    }
    else
    {
        CTSU.CTSUCR1.BYTE |= (2 << 4);
        ctsu_sdpa          = (pclkb_mhz / 4) - 1;
    }

    CTSU.CTSUCR1.BYTE   |= (CTSU_MODE_SELF_MULTI_SCAN << 6);
    CTSU.CTSUCHAC0.BYTE  = 0x01;
    CTSU.CTSUCHAC1.BYTE  = 0x00;
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 3)
    CTSU.CTSUCHAC2.BYTE  = 0x00;
#endif
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 4)
    CTSU.CTSUCHAC3.BYTE  = 0x00;
#endif
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 5)
    CTSU.CTSUCHAC4.BYTE  = 0x00;
#endif
    CTSU.CTSUCHTRC0.BYTE = 0x00;
    CTSU.CTSUCHTRC1.BYTE = 0x00;
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 3)
    CTSU.CTSUCHTRC2.BYTE = 0x00;
#endif
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 4)
    CTSU.CTSUCHTRC3.BYTE = 0x00;
#endif
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 5)
    CTSU.CTSUCHTRC4.BYTE = 0x00;
#endif

    /* Correction measurement setting */
    CTSU.CTSUERRS.BIT.CTSUTSOC = 1;
    g_ctsu_diag_info.icomp = 0;

    g_ctsu_diag_info.ctsuwr.ctsussc = (CTSU_SSDIV_0500 << 8);
    g_ctsu_diag_info.ctsuwr.ctsuso0 = CTSU_DIAG_DAC_SO_MAX;
    g_ctsu_diag_info.ctsuwr.ctsuso1 = (uint16_t) ((CTSU_ICOG_66 << 13) | (ctsu_sdpa << 8) | CTSU_RICOA_RECOMMEND);
}

static void ctsu_diag_ldo_over_voltage_data_get (void)
{
    /* Nothing */
}

static fsp_err_t ctsu_diag_ldo_over_voltage_result (void)
{
    if (1 != g_ctsu_diag_info.icomp)
    {
        return FSP_ERR_CTSU_DIAG_LDO_OVER_VOLTAGE;
    }

    /* if all checks passed to this point, return success */
    return FSP_SUCCESS;
}

static void ctsu_diag_oscillator_high_scan_start (void)
{
    uint32_t ctsu_sdpa;
    uint32_t pclkb_mhz;

    /* Set power on */
    CTSU.CTSUCR1.BYTE = ((CTSU_CFG_LOW_VOLTAGE_MODE << 2) | (CTSU_CSW_ON << 1) | CTSU_PON_ON);

    /* Synchronous Noise Reduction Setting */
    CTSU.CTSUSDPRS.BYTE = ((CTSU_SOFF_ON << 6) | (CTSU_PRMODE_62_PULSES << 4) | CTSU_PRRATIO_RECOMMEND);

    /* High Pass Noise Reduction- ALWAYS 0x30 as per HW Manual */
    CTSU.CTSUDCLKC.BYTE = ((CTSU_SSCNT << 4) | CTSU_SSMOD);

    /* Sensor Stabilization- ALWAYS 0x10 as per HW Manual */
    CTSU.CTSUSST.BYTE = (CTSU_SST_RECOMMEND);

    /* Since CLK is rewritten by correction, set here. */
    CTSU.CTSUCR1.BYTE |= (uint8_t) (CTSU_CFG_PCLK_DIVISION << 4);

    pclkb_mhz = BSP_PCLKB_HZ / CTSU_PCLKB_FREQ_MHZ;
    if (CTSU_PCLKB_FREQ_RANGE1 >= pclkb_mhz)
    {
        CTSU.CTSUCR1.BYTE |= (0 << 4);
        ctsu_sdpa          = pclkb_mhz - 1;
    }
    else if ((CTSU_PCLKB_FREQ_RANGE1 < pclkb_mhz) && (CTSU_PCLKB_FREQ_RANGE2 >= pclkb_mhz))
    {
        CTSU.CTSUCR1.BYTE |= (1 << 4);
        ctsu_sdpa          = (pclkb_mhz / 2) - 1;
    }
    else
    {
        CTSU.CTSUCR1.BYTE |= (2 << 4);
        ctsu_sdpa          = (pclkb_mhz / 4) - 1;
    }

    CTSU.CTSUCR1.BYTE   |= (CTSU_MODE_SELF_MULTI_SCAN << 6);
    CTSU.CTSUCHAC0.BYTE  = 0x01;
    CTSU.CTSUCHAC1.BYTE  = 0x00;
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 3)
    CTSU.CTSUCHAC2.BYTE  = 0x00;
#endif
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 4)
    CTSU.CTSUCHAC3.BYTE  = 0x00;
#endif
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 5)
    CTSU.CTSUCHAC4.BYTE  = 0x00;
#endif
    CTSU.CTSUCHTRC0.BYTE = 0x00;
    CTSU.CTSUCHTRC1.BYTE = 0x00;
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 3)
    CTSU.CTSUCHTRC2.BYTE = 0x00;
#endif
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 4)
    CTSU.CTSUCHTRC3.BYTE = 0x00;
#endif
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 5)
    CTSU.CTSUCHTRC4.BYTE = 0x00;
#endif

    CTSU.CTSUSO0.WORD = 0;

    g_ctsu_diag_info.ctsuwr.ctsussc = (CTSU_SSDIV_0500 << 8);
    g_ctsu_diag_info.ctsuwr.ctsuso0 = 0x0000;
    g_ctsu_diag_info.ctsuwr.ctsuso1 = (uint16_t) ((CTSU_ICOG_66 << 13) | (ctsu_sdpa << 8) | CTSU_RICOA_RECOMMEND);

    /* Correction measurement setting */
    CTSU.CTSUERRS.BIT.CTSUSPMD = 2;   // 0x82
    CTSU.CTSUERRS.BIT.CTSUTSOC = 1;
    R_BSP_SoftwareDelay(10, BSP_DELAY_MILLISECS);
}

static void ctsu_diag_oscillator_high_data_get (void)
{
    g_ctsu_diag_info.cco_high = g_ctsu_diag_info.scanbuf.sen;
}

static fsp_err_t ctsu_diag_oscillator_high_result (void)
{
    if ((g_ctsu_diag_info.cco_high < CTSU_CFG_DIAG_CCO_HIGH_MAX) &&
        (g_ctsu_diag_info.cco_high > CTSU_CFG_DIAG_CCO_HIGH_MIN))
    {
    }
    else
    {
        return FSP_ERR_CTSU_DIAG_CCO_HIGH;
    }

    /* if all checks passed to this point, return success */
    return FSP_SUCCESS;
}

static void ctsu_diag_oscillator_low_scan_start (void)
{
    uint32_t ctsu_sdpa;
    uint32_t pclkb_mhz;

    /* Set power on */
    CTSU.CTSUCR1.BYTE = ((CTSU_CFG_LOW_VOLTAGE_MODE << 2) | (CTSU_CSW_ON << 1) | CTSU_PON_ON);

    /* Synchronous Noise Reduction Setting */
    CTSU.CTSUSDPRS.BYTE = ((CTSU_SOFF_ON << 6) | (CTSU_PRMODE_62_PULSES << 4) | CTSU_PRRATIO_RECOMMEND);

    /* High Pass Noise Reduction- ALWAYS 0x30 as per HW Manual */
    CTSU.CTSUDCLKC.BYTE = ((CTSU_SSCNT << 4) | CTSU_SSMOD);

    /* Sensor Stabilization- ALWAYS 0x10 as per HW Manual */
    CTSU.CTSUSST.BYTE = (CTSU_SST_RECOMMEND);

    /* Since CLK is rewritten by correction, set here. */
    CTSU.CTSUCR1.BYTE |= (uint8_t) (CTSU_CFG_PCLK_DIVISION << 4);

    pclkb_mhz = BSP_PCLKB_HZ / CTSU_PCLKB_FREQ_MHZ;
    if (CTSU_PCLKB_FREQ_RANGE1 >= pclkb_mhz)
    {
        CTSU.CTSUCR1.BYTE |= (0 << 4);
        ctsu_sdpa          = pclkb_mhz - 1;
    }
    else if ((CTSU_PCLKB_FREQ_RANGE1 < pclkb_mhz) && (CTSU_PCLKB_FREQ_RANGE2 >= pclkb_mhz))
    {
        CTSU.CTSUCR1.BYTE |= (1 << 4);
        ctsu_sdpa          = (pclkb_mhz / 2) - 1;
    }
    else
    {
        CTSU.CTSUCR1.BYTE |= (2 << 4);
        ctsu_sdpa          = (pclkb_mhz / 4) - 1;
    }

    CTSU.CTSUCR1.BYTE   |= (CTSU_MODE_SELF_MULTI_SCAN << 6);
    CTSU.CTSUCHAC0.BYTE  = 0x01;
    CTSU.CTSUCHAC1.BYTE  = 0x00;
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 3)
    CTSU.CTSUCHAC2.BYTE  = 0x00;
#endif
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 4)
    CTSU.CTSUCHAC3.BYTE  = 0x00;
#endif
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 5)
    CTSU.CTSUCHAC4.BYTE  = 0x00;
#endif
    CTSU.CTSUCHTRC0.BYTE = 0x00;
    CTSU.CTSUCHTRC1.BYTE = 0x00;
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 3)
    CTSU.CTSUCHTRC2.BYTE = 0x00;
#endif
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 4)
    CTSU.CTSUCHTRC3.BYTE = 0x00;
#endif
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 5)
    CTSU.CTSUCHTRC4.BYTE = 0x00;
#endif

    CTSU.CTSUSO0.WORD = 0;

    g_ctsu_diag_info.ctsuwr.ctsussc = (CTSU_SSDIV_0500 << 8);
    g_ctsu_diag_info.ctsuwr.ctsuso0 = 0x0000;
    g_ctsu_diag_info.ctsuwr.ctsuso1 = (uint16_t) ((CTSU_ICOG_66 << 13) | (ctsu_sdpa << 8) | CTSU_RICOA_RECOMMEND);

    /* Correction measurement setting */
    CTSU.CTSUERRS.BIT.CTSUSPMD = 0;   // 0x80
    CTSU.CTSUERRS.BIT.CTSUTSOC = 1;
    R_BSP_SoftwareDelay(10, BSP_DELAY_MILLISECS);
}

static void ctsu_diag_oscillator_low_data_get (void)
{
    g_ctsu_diag_info.cco_low = g_ctsu_diag_info.scanbuf.sen;
}

static fsp_err_t ctsu_diag_oscillator_low_result (void)
{
    if ((g_ctsu_diag_info.cco_low < CTSU_CFG_DIAG_CCO_LOW_MAX) &&
        (g_ctsu_diag_info.cco_low > CTSU_CFG_DIAG_CCO_LOW_MIN))
    {
    }
    else
    {
        return FSP_ERR_CTSU_DIAG_CCO_LOW;
    }

    /* if all checks passed to this point, return success */
    return FSP_SUCCESS;
}

static void ctsu_diag_sscg_scan_start (void)
{
    uint32_t ctsu_sdpa;
    uint32_t pclkb_mhz;

    /* Set power on */
    CTSU.CTSUCR1.BYTE = ((CTSU_CFG_LOW_VOLTAGE_MODE << 2) | (CTSU_CSW_ON << 1) | CTSU_PON_ON);

    /* Synchronous Noise Reduction Setting */
    CTSU.CTSUSDPRS.BYTE = ((CTSU_SOFF_ON << 6) | (CTSU_PRMODE_62_PULSES << 4) | CTSU_PRRATIO_RECOMMEND);

    /* High Pass Noise Reduction- ALWAYS 0x30 as per HW Manual */
    CTSU.CTSUDCLKC.BYTE = ((CTSU_SSCNT << 4) | CTSU_SSMOD);

    /* Sensor Stabilization- ALWAYS 0x10 as per HW Manual */
    CTSU.CTSUSST.BYTE = (CTSU_SST_RECOMMEND);

    /* Since CLK is rewritten by correction, set here. */
    CTSU.CTSUCR1.BYTE |= (uint8_t) (CTSU_CFG_PCLK_DIVISION << 4);

    pclkb_mhz = BSP_PCLKB_HZ / CTSU_PCLKB_FREQ_MHZ;
    if (CTSU_PCLKB_FREQ_RANGE1 >= pclkb_mhz)
    {
        CTSU.CTSUCR1.BYTE |= (0 << 4);
        ctsu_sdpa          = pclkb_mhz - 1;
    }
    else if ((CTSU_PCLKB_FREQ_RANGE1 < pclkb_mhz) && (CTSU_PCLKB_FREQ_RANGE2 >= pclkb_mhz))
    {
        CTSU.CTSUCR1.BYTE |= (1 << 4);
        ctsu_sdpa          = (pclkb_mhz / 2) - 1;
    }
    else
    {
        CTSU.CTSUCR1.BYTE |= (2 << 4);
        ctsu_sdpa          = (pclkb_mhz / 4) - 1;
    }

    CTSU.CTSUCR1.BYTE |= (CTSU_MODE_SELF_MULTI_SCAN << 6);

    CTSU.CTSUCHAC0.BYTE  = 0x01;
    CTSU.CTSUCHAC1.BYTE  = 0x00;
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 3)
    CTSU.CTSUCHAC2.BYTE  = 0x00;
#endif
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 4)
    CTSU.CTSUCHAC3.BYTE  = 0x00;
#endif
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 5)
    CTSU.CTSUCHAC4.BYTE  = 0x00;
#endif
    CTSU.CTSUCHTRC0.BYTE = 0x00;
    CTSU.CTSUCHTRC1.BYTE = 0x00;
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 3)
    CTSU.CTSUCHTRC2.BYTE = 0x00;
#endif
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 4)
    CTSU.CTSUCHTRC3.BYTE = 0x00;
#endif
#if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 5)
    CTSU.CTSUCHTRC4.BYTE = 0x00;
#endif

    CTSU.CTSUSO0.WORD = 0;

    g_ctsu_diag_info.ctsuwr.ctsussc = (CTSU_SSDIV_0500 << 8);
    g_ctsu_diag_info.ctsuwr.ctsuso0 = 0x0000;
    g_ctsu_diag_info.ctsuwr.ctsuso1 = (uint16_t) ((CTSU_ICOG_66 << 13) | (ctsu_sdpa << 8) | CTSU_RICOA_RECOMMEND);

    /* Correction measurement setting */
    CTSU.CTSUERRS.BIT.CTSUSPMD = 0;   // 0x80
    CTSU.CTSUERRS.BIT.CTSUTSOC = 1;

    CTSU.CTSUERRS.WORD = CTSU_DIAG_TSOC_BIT | CTSU_DIAG_CLKSEL1_BIT;
}

static void ctsu_diag_sscg_data_get (void)
{
    g_ctsu_diag_info.sscg = g_ctsu_diag_info.scanbuf.ref;
}

static fsp_err_t ctsu_diag_sscg_result (void)
{
    if ((g_ctsu_diag_info.sscg > CTSU_CFG_DIAG_SSCG_MAX) || (g_ctsu_diag_info.sscg < CTSU_CFG_DIAG_SSCG_MIN))
    {
        return FSP_ERR_CTSU_DIAG_SSCG;
    }

    /* if all checks passed to this point, return success */
    return FSP_SUCCESS;
}

static void ctsu_diag_dac_initial_tuning (void)
{
    int32_t  diff          = 0;
    uint32_t complete_flag = 0;
    uint16_t ctsuso;

    diff = g_ctsu_diag_info.correct_data - CTSU_DIAG_DAC_TARGET_VALUE;

    ctsuso = g_ctsu_diag_info.ctsuwr.ctsuso0 & CTSU_TUNING_MAX;
    if (0 < diff)
    {
        if (g_ctsu_diag_info.tuning_diff < 0)
        {
            if ((-diff) > g_ctsu_diag_info.tuning_diff)
            {
                ctsuso++;
            }

            complete_flag = 1;
        }
        else
        {
            if (CTSU_TUNING_MAX == ctsuso)
            {
                complete_flag = 1;
            }
            else
            {
                ctsuso++;
                g_ctsu_diag_info.tuning_diff = diff;
            }
        }
    }
    else if (0 == diff)
    {
        complete_flag = 1;
    }
    else
    {
        if (g_ctsu_diag_info.tuning_diff > 0)
        {
            if ((-diff) > g_ctsu_diag_info.tuning_diff)
            {
                ctsuso--;
            }

            complete_flag = 1;
        }
        else
        {
            complete_flag = 1;
        }
    }

    g_ctsu_diag_info.ctsuwr.ctsuso0 &= (uint16_t) (~CTSU_TUNING_MAX);
    g_ctsu_diag_info.ctsuwr.ctsuso0 |= ctsuso;

    if (CTSU_CFG_NUM_SUMULTI == complete_flag)
    {
        g_ctsu_diag_info.tuning_diff = 0;
        g_ctsu_diag_info.so0_4uc_val = ctsuso;
        g_ctsu_diag_info.dac_init    = 3;
        g_ctsu_diag_info.tuning      = CTSU_TUNING_COMPLETE;
        g_ctsu_diag_info.loop_count  = 0;
        g_ctsu_diag_info.dac_cnt[0]  = g_ctsu_diag_info.correct_data;
    }
}

static void ctsu_diag_dac_scan_start (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    uint8_t temp;

    /* Set power on */
    CTSU.CTSUCR1.BYTE = ((CTSU_CFG_LOW_VOLTAGE_MODE << 2) | (CTSU_CSW_ON << 1) | CTSU_PON_ON);

    /* Synchronous Noise Reduction Setting */
    CTSU.CTSUSDPRS.BYTE = ((CTSU_SOFF_ON << 6) | (CTSU_PRMODE_62_PULSES << 4) | CTSU_PRRATIO_RECOMMEND);

    /* High Pass Noise Reduction- ALWAYS 0x30 as per HW Manual */
    CTSU.CTSUDCLKC.BYTE = ((CTSU_SSCNT << 4) | CTSU_SSMOD);

    /* Sensor Stabilization- ALWAYS 0x10 as per HW Manual */
    CTSU.CTSUSST.BYTE = (CTSU_SST_RECOMMEND);

    /* Since CLK is rewritten by correction, set here. */
    CTSU.CTSUCR1.BYTE |= (uint8_t) (CTSU_CFG_PCLK_DIVISION << 4);

    temp            = (uint8_t) (CTSU.CTSUCR1.BYTE & ~(CTSU_CR1_MODIFY_BIT));
    CTSU.CTSUCR1.BYTE = (uint8_t) (temp | (p_instance_ctrl->ctsucr1 & CTSU_CR1_MODIFY_BIT) | CTSU_DAC_TEST_ATUNE1); // MD1, MD0, ATUNE1=1
  #if BSP_FEATURE_CTSU_HAS_TXVSEL
    CTSU.CTSUCR0.BIT.CTSUTXVSEL = p_instance_ctrl->p_ctsu_cfg->txvsel;
  #endif

    /* Write Channel setting */
  #if (CTSU_CFG_DIAG_DAC_TS >= 0) && (CTSU_CFG_DIAG_DAC_TS < 8)
    CTSU.CTSUCHAC0.BYTE = 1 << CTSU_CFG_DIAG_DAC_TS;
  #else
    CTSU.CTSUCHAC0.BYTE = 0;
  #endif
  #if (CTSU_CFG_DIAG_DAC_TS >= 8) && (CTSU_CFG_DIAG_DAC_TS < 16)
    CTSU.CTSUCHAC1.BYTE = 1 << (CTSU_CFG_DIAG_DAC_TS - 8);
  #else
    CTSU.CTSUCHAC1.BYTE = 0;
  #endif
  #if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 3)
  #if (CTSU_CFG_DIAG_DAC_TS >= 16) && (CTSU_CFG_DIAG_DAC_TS < 24)
    CTSU.CTSUCHAC2.BYTE = 1 << (CTSU_CFG_DIAG_DAC_TS - 16);
  #else
    CTSU.CTSUCHAC2.BYTE = 0;
  #endif
  #endif
  #if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 4)
  #if (CTSU_CFG_DIAG_DAC_TS >= 24) && (CTSU_CFG_DIAG_DAC_TS < 32)
    CTSU.CTSUCHAC3.BYTE = 1 << (CTSU_CFG_DIAG_DAC_TS - 24);
  #else
    CTSU.CTSUCHAC3.BYTE = 0;
   #endif
  #endif
  #if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 5)
  #if (CTSU_CFG_DIAG_DAC_TS >= 32)
    CTSU.CTSUCHAC4.BYTE  = 1 << (CTSU_CFG_DIAG_DAC_TS - 32);
  #else
    CTSU.CTSUCHAC4.BYTE  = 0;
   #endif
  #endif
    CTSU.CTSUCHTRC0.BYTE = 0;
    CTSU.CTSUCHTRC1.BYTE = 0;
   #if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 3)
    CTSU.CTSUCHTRC2.BYTE = 0;
   #endif
   #if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 4)
    CTSU.CTSUCHTRC3.BYTE = 0;
   #endif
   #if (BSP_FEATURE_CTSU_CTSUCHAC_REGISTER_COUNT >= 5)
    CTSU.CTSUCHTRC4.BYTE = 0;
   #endif

    g_ctsu_diag_info.ctsuwr.ctsussc = (uint16_t) (CTSU_SSDIV_1330 << 8);
    g_ctsu_diag_info.ctsuwr.ctsuso1 = (uint16_t) ((CTSU_ICOG_RECOMMEND << 13) | (7 << 8) | CTSU_RICOA_RECOMMEND);

    if (g_ctsu_diag_info.dac_init > 2)
    {
        /* Apply DAC current */
        if (0 == g_ctsu_diag_info.loop_count)
        {
            g_ctsu_diag_info.ctsuwr.ctsuso0 = (uint16_t) ((3 << 10) | g_ctsu_diag_info.so0_4uc_val);
        }
        else if (1 == g_ctsu_diag_info.loop_count)
        {
            g_ctsu_diag_info.ctsuwr.ctsuso0 =
                (uint16_t) ((3 << 10) | (g_ctsu_diag_info.so0_4uc_val - CTSU_DIAG_DAC_1UC));
        }
        else if (2 == g_ctsu_diag_info.loop_count)
        {
            g_ctsu_diag_info.ctsuwr.ctsuso0 =
                (uint16_t) ((3 << 10) | (g_ctsu_diag_info.so0_4uc_val - CTSU_DIAG_DAC_2UC));
        }
        else if (3 == g_ctsu_diag_info.loop_count)
        {
            g_ctsu_diag_info.ctsuwr.ctsuso0 =
                (uint16_t) ((3 << 10) | (g_ctsu_diag_info.so0_4uc_val - CTSU_DIAG_DAC_4UC));
        }
        else if (4 == g_ctsu_diag_info.loop_count)
        {
            g_ctsu_diag_info.ctsuwr.ctsuso0 =
                (uint16_t) ((3 << 10) | (g_ctsu_diag_info.so0_4uc_val - CTSU_DIAG_DAC_8UC));
        }
        else if (5 == g_ctsu_diag_info.loop_count)
        {
            g_ctsu_diag_info.ctsuwr.ctsuso0 =
                (uint16_t) ((3 << 10) | (g_ctsu_diag_info.so0_4uc_val - CTSU_DIAG_DAC_16UC));
        }
        else
        {
        }
    }

    if (g_ctsu_diag_info.dac_init == 0)
    {
        g_ctsu_diag_info.dac_init       = 1;
        g_ctsu_diag_info.so0_4uc_val    = 0;
        g_ctsu_diag_info.tuning_diff    = 0;
        g_ctsu_diag_info.ctsuwr.ctsuso0 = (3 << 10) + CTSU_DIAG_DAC_START_VALUE;
    }
}

static void ctsu_diag_dac_data_get (void)
{
    ctsu_correction_calc_t calc;

    calc.snum = (g_ctsu_diag_info.ctsuwr.ctsuso0 >> 10) & CTSU_SNUM_MAX;

    if (CTSU_DIAG_DAC == g_ctsu_diag_info.state)
    {
        calc.snum = 3;
    }

    calc.sdpa = (g_ctsu_diag_info.ctsuwr.ctsuso1 >> 8) & CTSU_SDPA_MAX;

    /* Correction process */
    ctsu_correction_calc(&g_ctsu_diag_info.correct_data, g_ctsu_diag_info.scanbuf.sen, &calc);

    if (g_ctsu_diag_info.tuning == CTSU_TUNING_COMPLETE)
    {
        g_ctsu_diag_info.dac_cnt[g_ctsu_diag_info.loop_count] = g_ctsu_diag_info.correct_data;
    }
    else
    {
        ctsu_diag_dac_initial_tuning();
    }
}

static fsp_err_t ctsu_diag_dac_result (void)
{
    uint8_t k;
    if (g_ctsu_diag_info.tuning == CTSU_TUNING_COMPLETE)
    {
        for (k = 0; k < 6; k++)
        {
            if ((g_ctsu_diag_info.dac_cnt[k] > dac_oscil_table[k][0]) ||
                (g_ctsu_diag_info.dac_cnt[k] < dac_oscil_table[k][1]))
            {
                return FSP_ERR_CTSU_DIAG_DAC;
            }
        }
    }

    /* if all checks passed to this point, return success */
    return FSP_SUCCESS;
}

 #endif
#endif

#if (BSP_FEATURE_CTSU_VERSION == 2)
 #if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)

/***********************************************************************************************************************
 * ctsu_diag_scan_start2
 ***********************************************************************************************************************/
static fsp_err_t ctsu_diag_scan_start2 (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    fsp_err_t   err = FSP_SUCCESS;

    /* initial state change*/
    if (CTSU_DIAG_INIT == g_ctsu_diag_info.state)
    {
        g_ctsu_diag_info.state = CTSU_DIAG_OUTPUT_VOLTAGE;
    }

    /* ctsu normal scan register save */
    ctsu_diag_regi_store2();

    /* scan register setting */
    if (CTSU_DIAG_OUTPUT_VOLTAGE == g_ctsu_diag_info.state)
    {
        err = ctsu_diag_output_voltage_scan_start(p_instance_ctrl);
        if (FSP_SUCCESS == err)
        {
            g_ctsu_diag_info.state = CTSU_DIAG_OVER_VOLTAGE;
            err = FSP_SUCCESS;
        }
        else
        {
            /* if ADC is already opened */
            if (FSP_ERR_ALREADY_OPEN != err)
            {
                R_ADC_Close(0);
            }
            err = FSP_SUCCESS;

            g_ctsu_diag_info.state = CTSU_DIAG_OUTPUT_VOLTAGE;
            CTSU.CTSUCRA.BIT.ATUNE1 = 0;
            CTSU.CTSUCRA.BIT.ATUNE2 = 0;
            CTSU.CTSUCRA.BIT.LOAD   = 0;
            CTSU.CTSUMCH.BIT.MCA0   = 1;
            CTSU.CTSUMCH.BIT.MCA1   = 0;
            CTSU.CTSUMCH.BIT.MCA2   = 0;
            CTSU.CTSUMCH.BIT.MCA3   = 0;
            CTSU.CTSUCHACA.LONG     = g_ctsu_diag_info.chaca;
            CTSU.CTSUCHACB.LONG     = g_ctsu_diag_info.chacb;
            CTSU.CTSUCHTRCA.LONG    = 0;
            CTSU.CTSUCHTRCB.LONG    = 0;
        }
    }

    if (CTSU_DIAG_OVER_VOLTAGE == g_ctsu_diag_info.state)
    {
        ctsu_diag_over_voltage_scan_start();
        g_ctsu_diag_info.state = CTSU_DIAG_OVER_CURRENT;
    }

    if (CTSU_DIAG_OVER_CURRENT == g_ctsu_diag_info.state)
    {
        ctsu_diag_over_current_scan_start();
    }

    if (CTSU_DIAG_LOAD_RESISTANCE == g_ctsu_diag_info.state)
    {
        ctsu_diag_load_resistance_scan_start();
    }

    if (CTSU_DIAG_CURRENT_SOURCE == g_ctsu_diag_info.state)
    {
        ctsu_diag_current_source_scan_start();
    }

    if (CTSU_DIAG_SENSCLK == g_ctsu_diag_info.state)
    {
        ctsu_diag_cco_gain_scan_start();
    }

    if (CTSU_DIAG_SUCLK == g_ctsu_diag_info.state)
    {
        ctsu_diag_cco_gain_scan_start();
    }

    if (CTSU_DIAG_CLOCK_RECOVERY == g_ctsu_diag_info.state)
    {
        ctsu_diag_clock_recovery_scan_start();
    }

  #if (CTSU_CFG_NUM_CFC != 0)
    if (CTSU_DIAG_CFC == g_ctsu_diag_info.state)
    {
        ctsu_diag_cfc_gain_scan_start();
    }
  #endif

    return err;
}

/***********************************************************************************************************************
 * ctsu_diag_data_get2
 ***********************************************************************************************************************/
static fsp_err_t ctsu_diag_data_get2 (uint16_t * p_data)
{
    fsp_err_t err;

    if (CTSU_DIAG_LOAD_RESISTANCE == g_ctsu_diag_info.state)
    {
        ctsu_diag_load_resistance_data_get();
    }

    if (CTSU_DIAG_CURRENT_SOURCE == g_ctsu_diag_info.state)
    {
        ctsu_diag_current_source_data_get();
    }

    if (CTSU_DIAG_SENSCLK == g_ctsu_diag_info.state)
    {
        ctsu_diag_cco_gain_data_get();
    }

    if (CTSU_DIAG_SUCLK == g_ctsu_diag_info.state)
    {
        ctsu_diag_cco_gain_data_get();
    }

    if (CTSU_DIAG_CLOCK_RECOVERY == g_ctsu_diag_info.state)
    {
        ctsu_diag_clock_recovery_data_get();
    }

  #if (CTSU_CFG_NUM_CFC != 0)
    if (CTSU_DIAG_CFC == g_ctsu_diag_info.state)
    {
        ctsu_diag_cfc_gain_data_get();
    }
  #endif

    /* DIagnosis state transition */
    if (CTSU_DIAG_OVER_CURRENT == g_ctsu_diag_info.state)
    {
        g_ctsu_diag_info.loop_count = 0;
        g_ctsu_diag_info.state      = CTSU_DIAG_LOAD_RESISTANCE;
    }
    else if ((CTSU_DIAG_LOAD_RESISTANCE == g_ctsu_diag_info.state) && (CTSU_RANGE_NUM <= g_ctsu_diag_info.loop_count))
    {
        g_ctsu_diag_info.loop_count = 0;
        g_ctsu_diag_info.state      = CTSU_DIAG_CURRENT_SOURCE;
    }
    else if ((CTSU_DIAG_CURRENT_SOURCE == g_ctsu_diag_info.state) &&
             ((CTSU_DIAG_HIGH_CURRENT_SOURCE + CTSU_DIAG_LOW_CURRENT_SOURCE) <= g_ctsu_diag_info.loop_count))
    {
        g_ctsu_diag_info.loop_count = 0;
        g_ctsu_diag_info.state      = CTSU_DIAG_SENSCLK;
    }
    else if ((CTSU_DIAG_SENSCLK == g_ctsu_diag_info.state) &&
             (CTSU_CORRECTION_POINT_NUM <= g_ctsu_diag_info.loop_count))
    {
        g_ctsu_diag_info.loop_count = 0;
        g_ctsu_diag_info.state      = CTSU_DIAG_SUCLK;
    }
    else if ((CTSU_DIAG_SUCLK == g_ctsu_diag_info.state) && (CTSU_CORRECTION_POINT_NUM <= g_ctsu_diag_info.loop_count))
    {
        g_ctsu_diag_info.loop_count = 0;
        g_ctsu_diag_info.state      = CTSU_DIAG_CLOCK_RECOVERY;
    }
    else if (CTSU_DIAG_CLOCK_RECOVERY == g_ctsu_diag_info.state)
    {
  #if (CTSU_CFG_NUM_CFC != 0)
        g_ctsu_diag_info.state = CTSU_DIAG_CFC;
  #else
        g_ctsu_diag_info.state = CTSU_DIAG_COMPLETE;
  #endif
    }

  #if (CTSU_CFG_NUM_CFC != 0)
    else if ((CTSU_DIAG_CFC == g_ctsu_diag_info.state) && (CTSU_CORRCFC_POINT_NUM <= g_ctsu_diag_info.loop_count))
    {
        g_ctsu_diag_info.loop_count = 0;
        g_ctsu_diag_info.state      = CTSU_DIAG_COMPLETE;
    }
  #endif
    else
    {
    }

    if ((CTSU_DIAG_OUTPUT_VOLTAGE <= g_ctsu_diag_info.state) && (CTSU_DIAG_OVER_CURRENT >= g_ctsu_diag_info.state))
    {
        *p_data = g_ctsu_diag_info.output_voltage_cnt[0];
    }
    else
    {
        /* Indicates that ADC measurement was not performed. */
        *p_data = CTSU_COUNT_MAX;
    }

    /* register restore */
    ctsu_diag_regi_restore2();

    if (CTSU_DIAG_COMPLETE == g_ctsu_diag_info.state)
    {
        err = FSP_SUCCESS;
    }
    else if (CTSU_DIAG_OUTPUT_VOLTAGE == g_ctsu_diag_info.state)
    {
        err = FSP_ERR_ABORTED;
        g_ctsu_diag_info.state = CTSU_DIAG_INIT;
    }
    else
    {
        err = FSP_ERR_CTSU_DIAG_NOT_YET;
    }

    return err;
}

static void ctsu_diag_regi_store2 (void)
{
    g_ctsu_diag_reg.ctsucra    = CTSU.CTSUCRA.LONG;
    g_ctsu_diag_reg.ctsucrb    = CTSU.CTSUCRB.LONG;
    g_ctsu_diag_reg.ctsuchaca  = CTSU.CTSUCHACA.LONG;
    g_ctsu_diag_reg.ctsuchacb  = CTSU.CTSUCHACB.LONG;
    g_ctsu_diag_reg.ctsuchtrca = CTSU.CTSUCHTRCA.LONG;
    g_ctsu_diag_reg.ctsuchtrcb = CTSU.CTSUCHTRCB.LONG;
    g_ctsu_diag_reg.ctsumch    = CTSU.CTSUMCH.LONG;
    g_ctsu_diag_reg.ctsucalib  = CTSU.CTSUCALIB.LONG;
    g_ctsu_diag_reg.ctsusuclka = CTSU.CTSUSUCLKA.LONG;
    g_ctsu_diag_reg.ctsusuclkb = CTSU.CTSUSUCLKB.LONG;
}

static void ctsu_diag_regi_restore2 (void)
{
    /* register restore */
    CTSU.CTSUCRA.LONG    = g_ctsu_diag_reg.ctsucra;
    CTSU.CTSUCRB.LONG    = g_ctsu_diag_reg.ctsucrb;
    CTSU.CTSUCHACA.LONG  = g_ctsu_diag_reg.ctsuchaca;
    CTSU.CTSUCHACB.LONG  = g_ctsu_diag_reg.ctsuchacb;
    CTSU.CTSUCHTRCA.LONG = g_ctsu_diag_reg.ctsuchtrca;
    CTSU.CTSUCHTRCB.LONG = g_ctsu_diag_reg.ctsuchtrcb;
    CTSU.CTSUMCH.LONG    = g_ctsu_diag_reg.ctsumch;
    CTSU.CTSUCALIB.LONG  = g_ctsu_diag_reg.ctsucalib;
    CTSU.CTSUSUCLKA.LONG = g_ctsu_diag_reg.ctsusuclka;
    CTSU.CTSUSUCLKB.LONG = g_ctsu_diag_reg.ctsusuclkb;
}

static fsp_err_t ctsu_diag_output_voltage_scan_start (ctsu_instance_ctrl_t * const p_instance_ctrl)
{
    uint8_t                k;
    adc_cfg_t              open_cfg;
    adc_ch_cfg_t           ctrl_cfg;
    fsp_err_t              err;
    adc_err_t              poll;

    /* TSCAP AD/C AN008 Setting */
    CTSU.CTSUADCC.LONG = 1;

    /* Setting for A/D convert mode */
    open_cfg.conv_speed = ADC_CONVERT_SPEED_DEFAULT;
    open_cfg.alignment = ADC_ALIGN_RIGHT;
    open_cfg.add_cnt = ADC_ADD_OFF;                 /* addition is turned off for chans/sensors */
    open_cfg.clearing = ADC_CLEAR_AFTER_READ_OFF;
    open_cfg.trigger = ADC_TRIG_SOFTWARE;           /* Software trigger */
    open_cfg.trigger_groupb = ADC_TRIG_NONE;        /* Trigger none when without group mode */
    open_cfg.priority = 0;                          /* S12ADIO no interrupt(polling) */
    open_cfg.priority_groupb = 0;                   /* S12GBADI no interrupt */
    err = (fsp_err_t) R_ADC_Open (0, ADC_MODE_SS_ONE_CH, &open_cfg, FIT_NO_FUNC);
    if (ADC_ERR_AD_NOT_CLOSED == (adc_err_t) err)
    {
        return FSP_ERR_ALREADY_OPEN;
    }
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* choice A/D port(allow A/D convert) */
    ctrl_cfg.chan_mask = ADC_MASK_CH8;
    ctrl_cfg.chan_mask_groupb = ADC_MASK_GROUPB_OFF;
    ctrl_cfg.priority_groupa = ADC_GRPA_PRIORITY_OFF;
    ctrl_cfg.diag_method = ADC_DIAG_OFF;
    ctrl_cfg.add_mask = ADC_MASK_ADD_OFF;
    ctrl_cfg.signal_elc = ADC_ELC_ALL_SCANS_DONE;
    err = (fsp_err_t) R_ADC_Control (0, ADC_CMD_ENABLE_CHANS, &ctrl_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* CTSU setting */
    CTSU.CTSUCRA.BIT.PUMPON = 1;
    CTSU.CTSUCRA.BIT.CSW    = 1;
    CTSU.CTSUCRA.BIT.PON    = 1;
    CTSU.CTSUCALIB.LONG     = 0;

    /* Self single scan mode */
    CTSU.CTSUCRA.BIT.MD0 = 0;
    CTSU.CTSUCRA.BIT.MD1 = 0;
    CTSU.CTSUSO.LONG     = CTSU_DIAG_DAC_INIT_0;
    CTSU.CTSUSO.LONG     = CTSU_DIAG_DAC_INIT_1;
    CTSU.CTSUSO.LONG     = CTSU_DIAG_DAC_INIT_2;
    CTSU.CTSUSO.LONG     = CTSU_DIAG_DAC_INIT_3;

    /* LDO setting */
    CTSU.CTSUCALIB.BIT.DRV = 0;

    for (k = 0; k < 8; k++)
    {
        switch (k)
        {
            case 0:
            {
                /*select normal mode */
                CTSU.CTSUCRA.BIT.LOAD = 0;

                /* LDO Setting (20uA)*/
                CTSU.CTSUCRA.BIT.ATUNE1 = 0;
                CTSU.CTSUCRA.BIT.ATUNE2 = 1;
                break;
            }

            case 1:
            {
                /*select normal mode */
                CTSU.CTSUCRA.BIT.LOAD = 0;

                /* LDO Setting (40uA) */
                CTSU.CTSUCRA.BIT.ATUNE1 = 1;
                CTSU.CTSUCRA.BIT.ATUNE2 = 0;
                break;
            }

            case 2:
            {
                /*select normal mode */
                CTSU.CTSUCRA.BIT.LOAD = 0;

                /* LDO Setting (80uA) */
                CTSU.CTSUCRA.BIT.ATUNE1 = 0;
                CTSU.CTSUCRA.BIT.ATUNE2 = 0;
                break;
            }

            case 3:
            {
                /*select normal mode */
                CTSU.CTSUCRA.BIT.LOAD = 0;

                /* LDO Setting (160uA) */
                CTSU.CTSUCRA.BIT.ATUNE1 = 1;
                CTSU.CTSUCRA.BIT.ATUNE2 = 1;
                break;
            }

            case 4:
            {
                /*select normal mode */
                CTSU.CTSUCRA.BIT.LOAD = 1;
                CTSU.CTSUCRA.BIT.LOAD = 3;

                /* LDO Setting (20uA) */
                CTSU.CTSUCRA.BIT.ATUNE1 = 0;
                CTSU.CTSUCRA.BIT.ATUNE2 = 1;
                break;
            }

            case 5:
            {
                /* select Resistive load mode */
                CTSU.CTSUCRA.BIT.LOAD = 1;
                CTSU.CTSUCRA.BIT.LOAD = 3;

                /* LDO Setting (40uA) */
                CTSU.CTSUCRA.BIT.ATUNE1 = 1;
                CTSU.CTSUCRA.BIT.ATUNE2 = 0;
                break;
            }

            case 6:
            {
                /* select Resistive load mode */
                CTSU.CTSUCRA.BIT.LOAD = 1;
                CTSU.CTSUCRA.BIT.LOAD = 3;

                /* LDO Setting (80uA) */
                CTSU.CTSUCRA.BIT.ATUNE1 = 0;
                CTSU.CTSUCRA.BIT.ATUNE2 = 0;
                break;
            }

            case 7:
            {
                /* select Resistive load mode */
                CTSU.CTSUCRA.BIT.LOAD = 1;
                CTSU.CTSUCRA.BIT.LOAD = 3;

                /* LDO Setting (160uA) */
                CTSU.CTSUCRA.BIT.ATUNE1 = 1;
                CTSU.CTSUCRA.BIT.ATUNE2 = 1;
                break;
            }

            default:
            {
                break;
            }
        }

        CTSU.CTSUCALIB.BIT.DRV = 1;

        /* Software trigger start scan */
        err = (fsp_err_t) R_ADC_Control (0, ADC_CMD_SCAN_NOW, FIT_NO_PTR);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

        /* Polling for scan complete */
        do
        {
            poll = R_ADC_Control (0, ADC_CMD_CHECK_SCAN_DONE, FIT_NO_PTR);
        } while (ADC_ERR_SCAN_NOT_DONE == poll);

        /* Read A/D data then scan normal end */
        err = (fsp_err_t) R_ADC_Read (0, ADC_REG_CH8, &g_ctsu_diag_info.output_voltage_cnt[k]);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
    R_ADC_Close(0);

    return err;
}

static fsp_err_t ctsu_diag_output_voltage_result (void)
{
    uint8_t k;

    for (k = 0; k < 8; k++)
    {
        if ((g_ctsu_diag_info.output_voltage_cnt[k] <= CTSU_DIAG_TSCAP_RANGE_LOW) ||
            (g_ctsu_diag_info.output_voltage_cnt[k] >= CTSU_DIAG_TSCAP_RANGE_HIGH))
        {
            return FSP_ERR_CTSU_DIAG_OUTPUT_VOLTAGE;
        }
    }

    /* if all checks passed to this point, return success */
    return FSP_SUCCESS;
}

static void ctsu_diag_over_voltage_scan_start (void)
{
    g_ctsu_diag_info.icomp0_value = 1;

    CTSU.CTSUCRA.BIT.PUMPON  = 1;
    CTSU.CTSUCRA.BIT.CSW     = 1;
    CTSU.CTSUCRA.BIT.PON     = 1;
    CTSU.CTSUCALIB.LONG      = 0;
    CTSU.CTSUCALIB.BIT.DRV   = 1;
    CTSU.CTSUCALIB.BIT.DCOFF = 1;

    CTSU.CTSUCRA.BIT.LOAD   = 1;
    CTSU.CTSUCRA.BIT.ATUNE1 = 1;
    CTSU.CTSUCRA.BIT.ATUNE2 = 0;
    CTSU.CTSUCRA.BIT.LOAD   = 3;

    CTSU.CTSUSR.BIT.ICOMPRST = 1;
    CTSU.CTSUCALIB.LONG      = CTSU_DIAG_CURRENT_CLIB_REG;
    CTSU.CTSUSO.LONG         = 0;
    CTSU.CTSUSO.LONG         = 0;

    R_BSP_SoftwareDelay(1, BSP_DELAY_MILLISECS);

    if (CTSU.CTSUSR.BIT.ICOMP0 != 0)
    {
        g_ctsu_diag_info.icomp0_value = 0;
    }

    CTSU.CTSUCRA.BIT.LOAD   = 1;
    CTSU.CTSUCRA.BIT.ATUNE1 = 1;
    CTSU.CTSUCRA.BIT.ATUNE2 = 1;
    CTSU.CTSUCRA.BIT.LOAD   = 3;

    CTSU.CTSUCALIB.LONG = CTSU_DIAG_CURRENT_CLIB_REG;

    CTSU.CTSUCRA.BIT.LOAD = 2;
    CTSU.CTSUSO.LONG      = CTSU_DIAG_DAC_DATA_MAX_2;
    CTSU.CTSUSO.LONG      = CTSU_DIAG_DAC_DATA_MAX_3;

    R_BSP_SoftwareDelay(1, BSP_DELAY_MILLISECS);

    if (CTSU.CTSUSR.BIT.ICOMP0 != 1)
    {
        g_ctsu_diag_info.icomp0_value = 0;
    }

    CTSU.CTSUSR.BIT.ICOMPRST = 1;

    R_BSP_SoftwareDelay(1, BSP_DELAY_MILLISECS);

    if (CTSU.CTSUSR.BIT.ICOMP0 != 1)
    {
        g_ctsu_diag_info.icomp0_value = 0;
    }

    /* 30kohm setting */
    CTSU.CTSUCRA.BIT.ATUNE1 = 1;
    CTSU.CTSUCRA.BIT.ATUNE2 = 0;
    CTSU.CTSUCRA.BIT.LOAD   = 1;
    CTSU.CTSUCALIB.LONG     = CTSU_DIAG_CURRENT_CLIB_REG;

    /* 0uA setting */
    CTSU.CTSUSO.LONG = 0x00000000;
    CTSU.CTSUSO.LONG = 0x00000000;

    R_BSP_SoftwareDelay(1, BSP_DELAY_MILLISECS);

    if (CTSU.CTSUSR.BIT.ICOMP0 != 1)
    {
        g_ctsu_diag_info.icomp0_value = 0;
    }

    CTSU.CTSUSR.BIT.ICOMPRST = 1;

    R_BSP_SoftwareDelay(1, BSP_DELAY_MILLISECS);

    if (CTSU.CTSUSR.BIT.ICOMP0 != 0)
    {
        g_ctsu_diag_info.icomp0_value = 0;
    }
}

static fsp_err_t ctsu_diag_over_voltage_result (void)
{
    if (0 == g_ctsu_diag_info.icomp0_value)
    {
        return FSP_ERR_CTSU_DIAG_OVER_VOLTAGE;
    }

    /* if all checks passed to this point, return success */
    return FSP_SUCCESS;
}

static void ctsu_diag_over_current_scan_start (void)
{
    CTSU.CTSUCALIB.LONG      = 0;
    CTSU.CTSUCALIB.BIT.DRV   = 0;
    CTSU.CTSUCALIB.BIT.DCOFF = 0;
    CTSU.CTSUCRA.BIT.ATUNE1  = 0;
    CTSU.CTSUCRA.BIT.ATUNE2  = 0;
    CTSU.CTSUCRA.BIT.LOAD    = 0;
    CTSU.CTSUSR.BIT.ICOMPRST = 1;

    CTSU.CTSUMCH.BIT.MCA0 = 1;
    CTSU.CTSUMCH.BIT.MCA1 = 0;
    CTSU.CTSUMCH.BIT.MCA2 = 0;
    CTSU.CTSUMCH.BIT.MCA3 = 0;

    CTSU.CTSUCRA.BIT.MD0 = 1;
    CTSU.CTSUCRA.BIT.MD1 = 0;


    CTSU.CTSUCHACA.LONG  = g_ctsu_diag_info.chaca;
    CTSU.CTSUCHACB.LONG  = g_ctsu_diag_info.chacb;
    CTSU.CTSUCHTRCA.LONG = 0;
    CTSU.CTSUCHTRCB.LONG = 0;

    CTSU.CTSUCRA.BIT.CSW = 0;
    CTSU.CTSUCRA.BIT.PON = 0;

    /* TSCAP discharge process */
    R_Set_CTSU_TSCAP_Discharge();
    R_BSP_SoftwareDelay(10,BSP_DELAY_MICROSECS);
    R_Set_CTSU_TSCAP_Charge();

    CTSU.CTSUCRA.BIT.CSW = 1;
    CTSU.CTSUCRA.BIT.PON = 1;
}

static fsp_err_t ctsu_diag_over_current_result (void)
{
    if (g_ctsu_diag_info.icomp1_value == 0)
    {
        return FSP_ERR_CTSU_DIAG_OVER_CURRENT;
    }

    /* if all checks passed to this point, return success */
    return FSP_SUCCESS;
}

static void ctsu_diag_load_resistance_scan_start (void)
{
    /* CTSU setting */
    CTSU.CTSUCRA.BIT.PUMPON = 1;
    CTSU.CTSUCRA.BIT.CSW    = 1;
    CTSU.CTSUCRA.BIT.PON    = 1;

    CTSU.CTSUCRA.BIT.SDPSEL    = 1;
    CTSU.CTSUCRA.BIT.PCSEL     = 1;
    CTSU.CTSUCRB.BIT.SST       = CTSU_SST_RECOMMEND;
    CTSU.CTSUCALIB.BIT.CCOCLK  = 0;
    CTSU.CTSUCALIB.BIT.SUCLKEN = 0;

    CTSU.CTSUCRA.BIT.MD0  = 1;
    CTSU.CTSUCRA.BIT.MD1  = 0;
    CTSU.CTSUMCH.BIT.MCA0 = 1;
    CTSU.CTSUMCH.BIT.MCA1 = 0;
    CTSU.CTSUMCH.BIT.MCA2 = 0;
    CTSU.CTSUMCH.BIT.MCA3 = 0;
    CTSU.CTSUCRA.BIT.LOAD = 1;

    /* Setting time of measurement */
    g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10);

    CTSU.CTSUCHACA.LONG  = 1;
    CTSU.CTSUCHACB.LONG  = 0;
    CTSU.CTSUCHTRCA.LONG = 0;
    CTSU.CTSUCHTRCB.LONG = 0;

    CTSU.CTSUCALIB.BIT.TSOC = 1;

    switch (g_ctsu_diag_info.loop_count)
    {
        case 0:
        {
            CTSU.CTSUCRA.BIT.ATUNE1 = 1;
            CTSU.CTSUCRA.BIT.ATUNE2 = 1; // 7.5Kohm 160uA
            break;
        }

        case 1:
        {
            CTSU.CTSUCRA.BIT.ATUNE1 = 0;
            CTSU.CTSUCRA.BIT.ATUNE2 = 0; // 15Kohm 80uA
            break;
        }

        case 2:
        {
            CTSU.CTSUCRA.BIT.ATUNE1 = 1;
            CTSU.CTSUCRA.BIT.ATUNE2 = 0; // 30Kohm 40uA
            break;
        }

        case 3:
        {
            CTSU.CTSUCRA.BIT.ATUNE1 = 0;
            CTSU.CTSUCRA.BIT.ATUNE2 = 1; // 60Kohm 20uA
            break;
        }

        default:
        {
            break;
        }
    }

    CTSU.CTSUCRA.BIT.LOAD = 3;
}

static void ctsu_diag_load_resistance_data_get (void)
{
    g_ctsu_diag_info.load_resistance[g_ctsu_diag_info.loop_count] = (uint16_t) g_ctsu_diag_info.ctsuscnt[0];

    g_ctsu_diag_info.load_resistance[g_ctsu_diag_info.loop_count] =
        (uint16_t) ((((uint32_t) g_ctsu_diag_info.load_resistance[g_ctsu_diag_info.loop_count]) *
                     g_ctsu_diag_info.error_registance[3 - g_ctsu_diag_info.loop_count]) >> (CTSU_SHIFT_AMOUNT));

#if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 0)
    ctsu_correction_calc_t calc;
    calc.snum  = CTSU_SNUM_RECOMMEND;
    calc.range = CTSU_RANGE_40UA;
    calc.md    = CTSU_MODE_DIAGNOSIS_SCAN;
    ctsu_correction_calc(&g_ctsu_diag_info.load_resistance[g_ctsu_diag_info.loop_count],
                         g_ctsu_diag_info.load_resistance[g_ctsu_diag_info.loop_count],
                         &calc);
#endif
    g_ctsu_diag_info.loop_count++;
}

static fsp_err_t ctsu_diag_load_resistance_result (void)
{
    uint8_t k;

    for (k = 0; k < 4; k++)
    {
        if ((g_ctsu_diag_info.load_resistance[k] > CTSU_CFG_DIAG_LOAD_REISTER_MIN) &&
            (g_ctsu_diag_info.load_resistance[k] < CTSU_CFG_DIAG_LOAD_REISTER_MAX))
        {
        }
        else
        {
            return FSP_ERR_CTSU_DIAG_LOAD_RESISTANCE;
        }
    }

    /* if all checks passed to this point, return success */
    return FSP_SUCCESS;
}

static void ctsu_diag_current_source_scan_start (void)
{
    /* CTSU setting */
    CTSU.CTSUCRA.BIT.PUMPON = 1;
    CTSU.CTSUCRA.BIT.CSW    = 1;
    CTSU.CTSUCRA.BIT.PON    = 1;

    CTSU.CTSUCRA.BIT.SDPSEL    = 1;
    CTSU.CTSUCRA.BIT.PCSEL     = 1;
    CTSU.CTSUCRB.BIT.SST       = CTSU_SST_RECOMMEND;
    CTSU.CTSUCALIB.BIT.CCOCLK  = 0;
    CTSU.CTSUCALIB.BIT.SUCLKEN = 0;

    CTSU.CTSUCRA.BIT.MD0  = 1;
    CTSU.CTSUCRA.BIT.MD1  = 0;
    CTSU.CTSUMCH.BIT.MCA0 = 1;
    CTSU.CTSUMCH.BIT.MCA1 = 0;
    CTSU.CTSUMCH.BIT.MCA2 = 0;
    CTSU.CTSUMCH.BIT.MCA3 = 0;

    CTSU.CTSUCHACA.LONG  = 1;
    CTSU.CTSUCHACB.LONG  = 0;
    CTSU.CTSUCHTRCA.LONG = 0;
    CTSU.CTSUCHTRCB.LONG = 0;

    CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_0;
    CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_1;
    CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_2;
    CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_3;

    /* LDO Nch setting (40uAmode) */
    CTSU.CTSUCRA.BIT.LOAD   = 1;
    CTSU.CTSUCALIB.BIT.TSOC = 1;
    CTSU.CTSUCRA.BIT.ATUNE1 = 1;
    CTSU.CTSUCRA.BIT.ATUNE2 = 0;
    CTSU.CTSUCRA.BIT.LOAD   = 3;

    /* DAC initial setting */
    CTSU.CTSUCALIB.BIT.DACMSEL  = 1;
    CTSU.CTSUCALIB.BIT.DACCARRY = 1;

    if (15 >= g_ctsu_diag_info.loop_count)
    {
        /* Upper Current source setting (10uA) */
        CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_0;
        CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_1;

        switch (g_ctsu_diag_info.loop_count)
        {
            case 0:
            {
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_2 + CTSU_DIAG_DAC_0BIT;
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_3;

                /* Setting time of measurement */
                g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10) | CTSU_DIAG_DAC_INIT_3;
                break;
            }

            case 1:
            {
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_2 + CTSU_DIAG_DAC_1BIT;
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_3;
                g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10) | CTSU_DIAG_DAC_INIT_3;
                break;
            }

            case 2:
            {
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_2 + CTSU_DIAG_DAC_2BIT;
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_3;
                g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10) | CTSU_DIAG_DAC_INIT_3;
                break;
            }

            case 3:
            {
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_2 + CTSU_DIAG_DAC_3BIT;
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_3;
                g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10) | CTSU_DIAG_DAC_INIT_3;
                break;
            }

            case 4:
            {
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_2 + CTSU_DIAG_DAC_4BIT;
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_3;
                g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10) | CTSU_DIAG_DAC_INIT_3;
                break;
            }

            case 5:
            {
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_2 + CTSU_DIAG_DAC_5BIT;
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_3;
                g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10) | CTSU_DIAG_DAC_INIT_3;
                break;
            }

            case 6:
            {
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_2 + CTSU_DIAG_DAC_6BIT;
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_3;
                g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10) | CTSU_DIAG_DAC_INIT_3;
                break;
            }

            case 7:
            {
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_2 + CTSU_DIAG_DAC_7BIT;
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_3;
                g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10) | CTSU_DIAG_DAC_INIT_3;
                break;
            }

            case 8:
            {
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_2;
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_3 + CTSU_DIAG_DAC_0BIT;
                g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10) |
                                                 (CTSU_DIAG_DAC_INIT_3 + CTSU_DIAG_DAC_0BIT);
                break;
            }

            case 9:
            {
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_2;
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_3 + CTSU_DIAG_DAC_1BIT;
                g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10) |
                                                 (CTSU_DIAG_DAC_INIT_3 + CTSU_DIAG_DAC_1BIT);
                break;
            }

            case 10:
            {
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_2;
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_3 + CTSU_DIAG_DAC_2BIT;
                g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10) |
                                                 (CTSU_DIAG_DAC_INIT_3 + CTSU_DIAG_DAC_2BIT);
                break;
            }

            case 11:
            {
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_2;
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_3 + CTSU_DIAG_DAC_3BIT;
                g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10) |
                                                 (CTSU_DIAG_DAC_INIT_3 + CTSU_DIAG_DAC_3BIT);
                break;
            }

            case 12:
            {
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_2;
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_3 + CTSU_DIAG_DAC_4BIT;
                g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10) |
                                                 (CTSU_DIAG_DAC_INIT_3 + CTSU_DIAG_DAC_4BIT);
                break;
            }

            case 13:
            {
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_2;
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_3 + CTSU_DIAG_DAC_5BIT;
                g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10) |
                                                 (CTSU_DIAG_DAC_INIT_3 + CTSU_DIAG_DAC_5BIT);
                break;
            }

            case 14:
            {
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_2;
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_3 + CTSU_DIAG_DAC_6BIT;
                g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10) |
                                                 (CTSU_DIAG_DAC_INIT_3 + CTSU_DIAG_DAC_6BIT);
                break;
            }

            case 15:
            {
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_2;
                CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_3 + CTSU_DIAG_DAC_7BIT;
                g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10) |
                                                 (CTSU_DIAG_DAC_INIT_3 + CTSU_DIAG_DAC_7BIT);
                break;
            }
        }
    }

    if (16 <= g_ctsu_diag_info.loop_count)
    {
        /* Lower Current source setting (10uA) */
        switch (g_ctsu_diag_info.loop_count)
        {
            case 16:
            {
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_DATA_MAX_0;
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_1;
                break;
            }

            case 17:
            {
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_DATA_MAX_0 - CTSU_DIAG_DAC_0BIT;
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_1 + CTSU_DIAG_DAC_0BIT;
                break;
            }

            case 18:
            {
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_DATA_MAX_0 - (CTSU_DIAG_DAC_0BIT + CTSU_DIAG_DAC_1BIT);
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_1 + (CTSU_DIAG_DAC_0BIT + CTSU_DIAG_DAC_1BIT);
                break;
            }

            case 19:
            {
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_DATA_MAX_0 - (CTSU_DIAG_DAC_1BIT + CTSU_DIAG_DAC_2BIT);
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_1 + (CTSU_DIAG_DAC_0BIT + CTSU_DIAG_DAC_1BIT);
                break;
            }

            case 20:
            {
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_DATA_MAX_0 - (CTSU_DIAG_DAC_2BIT + CTSU_DIAG_DAC_3BIT);
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_1 + (CTSU_DIAG_DAC_0BIT + CTSU_DIAG_DAC_1BIT);
                break;
            }

            case 21:
            {
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_DATA_MAX_0 - (CTSU_DIAG_DAC_3BIT + CTSU_DIAG_DAC_4BIT);
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_1 + (CTSU_DIAG_DAC_0BIT + CTSU_DIAG_DAC_1BIT);
                break;
            }

            case 22:
            {
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_DATA_MAX_0 - (CTSU_DIAG_DAC_4BIT + CTSU_DIAG_DAC_5BIT);
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_1 + (CTSU_DIAG_DAC_0BIT + CTSU_DIAG_DAC_1BIT);
                break;
            }

            case 23:
            {
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_DATA_MAX_0 - (CTSU_DIAG_DAC_5BIT + CTSU_DIAG_DAC_6BIT);
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_1 + (CTSU_DIAG_DAC_0BIT + CTSU_DIAG_DAC_1BIT);
                break;
            }

            case 24:
            {
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_DATA_MAX_0 - (CTSU_DIAG_DAC_6BIT + CTSU_DIAG_DAC_7BIT);
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_1 + (CTSU_DIAG_DAC_0BIT + CTSU_DIAG_DAC_1BIT);
                break;
            }

            case 25:
            {
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_DATA_MAX_0 - CTSU_DIAG_DAC_7BIT;
                CTSU.CTSUSO.LONG = CTSU_DIAG_DAC_INIT_1 + CTSU_DIAG_DAC_1BIT;
                break;
            }
        }

        CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_2;
        CTSU.CTSUSO.LONG                 = CTSU_DIAG_DAC_INIT_3;
        g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10) | CTSU_DIAG_DAC_INIT_3;
    }
}

static void ctsu_diag_current_source_data_get (void)
{
    g_ctsu_diag_info.current_source[g_ctsu_diag_info.loop_count] = (uint16_t) g_ctsu_diag_info.ctsuscnt[0];

    g_ctsu_diag_info.current_source[g_ctsu_diag_info.loop_count] =
        (uint16_t) ((((uint32_t) g_ctsu_diag_info.current_source[g_ctsu_diag_info.loop_count]) *
                     g_ctsu_diag_info.error_registance[2]) >> (CTSU_SHIFT_AMOUNT));

#if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 0)
    ctsu_correction_calc_t calc;
    calc.snum  = CTSU_SNUM_RECOMMEND;
    calc.range = CTSU_RANGE_40UA;
    calc.md    = CTSU_MODE_DIAGNOSIS_SCAN;
    ctsu_correction_calc(&g_ctsu_diag_info.current_source[g_ctsu_diag_info.loop_count],
                         g_ctsu_diag_info.current_source[g_ctsu_diag_info.loop_count],
                         &calc);
#endif

    g_ctsu_diag_info.loop_count++;
}

static fsp_err_t ctsu_diag_current_source_result (void)
{
    uint8_t  k;
    uint16_t current_source_diff;

    for (k = 0; k < (CTSU_DIAG_HIGH_CURRENT_SOURCE + CTSU_DIAG_LOW_CURRENT_SOURCE); k++)
    {
        current_source_diff = (uint16_t) (g_ctsu_diag_info.load_resistance[2] - g_ctsu_diag_info.current_source[k]);
        if ((current_source_diff < CTSU_CFG_DIAG_CURRENT_SOURCE_DIFF_MAX) &&
            (current_source_diff > CTSU_CFG_DIAG_CURRENT_SOURCE_DIFF_MIN))
        {
        }
        else
        {
            return FSP_ERR_CTSU_DIAG_CURRENT_SOURCE;
        }
    }

    /* if all checks passed to this point, return success */
    return FSP_SUCCESS;
}

static void ctsu_diag_cco_gain_scan_start (void)
{
    /* CTSU Setting */
    CTSU.CTSUCRA.BIT.MD0    = 0;
    CTSU.CTSUCRA.BIT.MD1    = 0;
    CTSU.CTSUCRA.BIT.LOAD   = 1;
    CTSU.CTSUCRA.BIT.PON    = 1;
    CTSU.CTSUCRA.BIT.CSW    = 1;
    CTSU.CTSUCRA.BIT.PUMPON = 1;

    CTSU.CTSUMCH.BIT.MCA0 = 1;
    CTSU.CTSUMCH.BIT.MCA1 = 0;
    CTSU.CTSUMCH.BIT.MCA2 = 0;
    CTSU.CTSUMCH.BIT.MCA3 = 0;

    CTSU.CTSUCRB.BIT.SSMOD = 4;
    CTSU.CTSUCRB.BIT.SSCNT = 0;
    CTSU.CTSUCRB.BIT.SST   = CTSU_SST_RECOMMEND;

    CTSU.CTSUCRA.BIT.PCSEL  = 1;
    CTSU.CTSUCRA.BIT.SDPSEL = 1;

    CTSU.CTSUCALIB.LONG        = 0;
    CTSU.CTSUCALIB.BIT.SUCARRY = 0;

#if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 1)
   /* Sensor counter auto correction enabled */
   CTSU.CTSUOPT.BIT.CCOCFEN = 0;
#endif

    if (CTSU_DIAG_SENSCLK == g_ctsu_diag_info.state)
    {
        CTSU.CTSUCALIB.BIT.CCOCALIB = 1;
    }

    if (CTSU_DIAG_SUCLK == g_ctsu_diag_info.state)
    {
        CTSU.CTSUCALIB.BIT.CCOCALIB = 0;
    }

    CTSU.CTSUCALIB.BIT.CCOCLK = 0;

    CTSU.CTSUCHACA.LONG  = 1;
    CTSU.CTSUCHACB.LONG  = 0;
    CTSU.CTSUCHTRCA.LONG = 0;
    CTSU.CTSUCHTRCB.LONG = 0;

    /* Setting time of measurement */
    g_ctsu_diag_info.ctsuwr.ctsuso = (CTSU_SNUM_RECOMMEND << 10);

    CTSU.CTSUMCH.BIT.MCH0 = 0;

    if (g_ctsu_diag_info.loop_count < 8)
    {
        CTSU.CTSUSUCLKA.BIT.SUADJ0   = (0x1f + (g_ctsu_diag_info.loop_count * CTSU_DIAG_SUCLK0_REG2));
    }
    else if ((8 <= g_ctsu_diag_info.loop_count) && (g_ctsu_diag_info.loop_count < 11))
    {
        CTSU.CTSUSUCLKA.BIT.SUADJ0   = (0x1f + (7 * CTSU_DIAG_SUCLK0_REG2));
        if (8 == g_ctsu_diag_info.loop_count)
        {
            CTSU.CTSUCRB.BIT.SSCNT = 0;
        }
        else if (9 == g_ctsu_diag_info.loop_count)
        {
            CTSU.CTSUCRB.BIT.SSCNT = 1;
        }
        else if (10 == g_ctsu_diag_info.loop_count)
        {
            CTSU.CTSUCRB.BIT.SSCNT = 2;
        }
        else
        {
        }

        CTSU.CTSUCALIB.BIT.SUCARRY = 0;
        if (CTSU_DIAG_SENSCLK == g_ctsu_diag_info.state)
        {
            CTSU.CTSUCALIB.BIT.CCOCALIB = 1;
        }

        if (CTSU_DIAG_SUCLK == g_ctsu_diag_info.state)
        {
            CTSU.CTSUCALIB.BIT.CCOCALIB = 0;
        }

        CTSU.CTSUCALIB.BIT.CCOCLK = 0;
        CTSU.CTSUCRB.BIT.SSMOD    = 1;
        CTSU.CTSUCRB.BIT.SST      = CTSU_SST_RECOMMEND_CURRENT;
        if (8 == g_ctsu_diag_info.loop_count)
        {
            CTSU.CTSUCRB.BIT.SSCNT = 1;
        }
        else if (9 == g_ctsu_diag_info.loop_count)
        {
            CTSU.CTSUCRB.BIT.SSCNT = 2;
        }
        else if (10 == g_ctsu_diag_info.loop_count)
        {
            CTSU.CTSUCRB.BIT.SSCNT = 3;
        }
        else
        {
        }
    }
    else if (11 == g_ctsu_diag_info.loop_count)
    {
        CTSU.CTSUSUCLKA.BIT.SUADJ0   = (0x1f + (7 * CTSU_DIAG_SUCLK0_REG2));

        CTSU.CTSUCRB.BIT.SSCNT     = 2;
        CTSU.CTSUCALIB.BIT.SUCARRY = 1;
        if (CTSU_DIAG_SENSCLK == g_ctsu_diag_info.state)
        {
            CTSU.CTSUCALIB.BIT.CCOCALIB = 1;
        }

        if (CTSU_DIAG_SUCLK == g_ctsu_diag_info.state)
        {
            CTSU.CTSUCALIB.BIT.CCOCALIB = 0;
        }

        CTSU.CTSUCALIB.BIT.CCOCLK = 0;
        CTSU.CTSUCRB.BIT.SSMOD    = 1;
        CTSU.CTSUCRB.BIT.SST      = CTSU_SST_RECOMMEND_CURRENT;
        CTSU.CTSUCRB.BIT.SSCNT    = 3;
    }
    else
    {
    }
}

static void ctsu_diag_cco_gain_data_get (void)
{
    if (CTSU_DIAG_SENSCLK == g_ctsu_diag_info.state)
    {
        g_ctsu_diag_info.sensclk_cnt[g_ctsu_diag_info.loop_count] = (uint16_t) g_ctsu_diag_info.ctsuscnt[0];
    }

    if (CTSU_DIAG_SUCLK == g_ctsu_diag_info.state)
    {
        g_ctsu_diag_info.suclk_cnt[g_ctsu_diag_info.loop_count] = (uint16_t) (g_ctsu_diag_info.ctsuscnt[0] >> 16);
    }

    g_ctsu_diag_info.loop_count++;

#if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 1)
   /* Sensor counter auto correction enabled */
   CTSU.CTSUOPT.BIT.CCOCFEN = 1;
#endif

}

static fsp_err_t ctsu_diag_cco_gain_result (void)
{
    uint32_t          k;
    volatile uint16_t cco_gain_data;
    uint16_t          cco_gain_data_pre;
    uint16_t          cco_gain_diff;
    uint16_t          cco_gain_max;
    uint16_t          cco_gain_min;
    uint16_t          cco_gain_diff_max;
    uint16_t          cco_gain_diff_min;

    /* read counter data */
    for (k = 0; k < CTSU_CORRECTION_POINT_NUM; k++)
    {
        if (CTSU_DIAG_SENSCLK == g_ctsu_diag_info.state)
        {
            cco_gain_data = g_ctsu_diag_info.sensclk_cnt[k];
        }

        if (CTSU_DIAG_SUCLK == g_ctsu_diag_info.state)
        {
            cco_gain_data = g_ctsu_diag_info.suclk_cnt[k];
        }

        cco_gain_max = cco_gain_table[k][1];
        cco_gain_min = cco_gain_table[k][0];

        /* check measurement result */
        if ((cco_gain_data <= cco_gain_max) && (cco_gain_data >= cco_gain_min))
        {
        }
        else
        {
            if (CTSU_DIAG_SENSCLK == g_ctsu_diag_info.state)
            {
                return FSP_ERR_CTSU_DIAG_SENSCLK_GAIN;
            }

            if (CTSU_DIAG_SUCLK == g_ctsu_diag_info.state)
            {
                return FSP_ERR_CTSU_DIAG_SUCLK_GAIN;
            }
        }

        if (0 < k)
        {
            cco_gain_diff_max = cco_gain_diff_table[k - 1][1];
            cco_gain_diff_min = cco_gain_diff_table[k - 1][0];

            if (cco_gain_data > cco_gain_data_pre)
            {
                cco_gain_diff = (uint16_t) (cco_gain_data - cco_gain_data_pre);
            }
            else
            {
                cco_gain_diff = (uint16_t) (cco_gain_data_pre - cco_gain_data);
            }

            /* check measurement result */
            if ((cco_gain_diff > cco_gain_diff_max) || (cco_gain_diff < cco_gain_diff_min))
            {
                if (CTSU_DIAG_SENSCLK == g_ctsu_diag_info.state)
                {
                    return FSP_ERR_CTSU_DIAG_SENSCLK_GAIN;
                }

                if (CTSU_DIAG_SUCLK == g_ctsu_diag_info.state)
                {
                    return FSP_ERR_CTSU_DIAG_SUCLK_GAIN;
                }
            }
        }

        cco_gain_data_pre = cco_gain_data;
    }

    /* if all checks passed to this point, return success */
    return FSP_SUCCESS;
}

static void ctsu_diag_clock_recovery_scan_start (void)
{
    CTSU.CTSUCRA.LONG           = g_ctsu_diag_reg.ctsucra;
    CTSU.CTSUCRA.BIT.MD0        = 1;
    CTSU.CTSUCRA.BIT.MD1        = 0;
    CTSU.CTSUMCH.LONG           = g_ctsu_diag_reg.ctsumch;
    CTSU.CTSUCRB.LONG           = g_ctsu_diag_reg.ctsucrb;
    CTSU.CTSUCRA.BIT.LOAD       = 1;
    CTSU.CTSUCRB.BIT.SSCNT      = 1;
    CTSU.CTSUCALIB.BIT.SUCARRY  = 0;
    CTSU.CTSUCALIB.BIT.CCOCALIB = 0;
    CTSU.CTSUCALIB.BIT.CCOCLK   = 1;
    CTSU.CTSUCALIB.BIT.TSOC     = 0;
    CTSU.CTSUCALIB.BIT.SUCLKEN  = 1;

    CTSU.CTSUCRA.BIT.SDPSEL = 0;
    CTSU.CTSUSUCLKA.LONG    = g_ctsu_diag_reg.ctsusuclka;
    CTSU.CTSUSUCLKB.LONG    = g_ctsu_diag_reg.ctsusuclkb;
    CTSU.CTSUCRA.BIT.SDPSEL = 1;

    CTSU.CTSUMCH.BIT.MCA0 = 1;
    CTSU.CTSUMCH.BIT.MCA1 = 1;
    CTSU.CTSUMCH.BIT.MCA2 = 1;
    CTSU.CTSUMCH.BIT.MCA3 = 0;

    CTSU.CTSUCHACA.LONG  = g_ctsu_diag_info.chaca;
    CTSU.CTSUCHACB.LONG  = g_ctsu_diag_info.chacb;
    CTSU.CTSUCHTRCA.LONG = 0;
    CTSU.CTSUCHTRCB.LONG = 0;

#if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 1)
   /* Sensor counter auto correction enabled */
   CTSU.CTSUOPT.BIT.CCOCFEN = 0;
#endif

    /* Setting time of measurement */

    g_ctsu_diag_info.ctsuwr.ctsuso = (uint32_t) ((0xf << 24) | (CTSU_SNUM_RECOMMEND << 10));
}

static void ctsu_diag_clock_recovery_data_get (void)
{
    uint8_t i;
    for (i = 0; i < 3; i++)
    {
        g_ctsu_diag_info.suclk_count_clk_recv[i] = (uint16_t) (g_ctsu_diag_info.ctsuscnt[i] >> 16);
    }

#if (CTSU_CFG_AUTO_CORRECTION_ENABLE == 1)
   /* Sensor counter auto correction enabled */
   CTSU.CTSUOPT.BIT.CCOCFEN = 1;
#endif
}

static fsp_err_t ctsu_diag_clock_recovery_result (void)
{
    uint8_t         i;
    uint32_t        suclk[3];
    static uint32_t suclk_cnt[3];

    suclk[0] = (CTSU_DIAG_STCLK_FREQ * (CTSU_CFG_SUMULTI0 + 1));
    suclk[1] = (CTSU_DIAG_STCLK_FREQ * (CTSU_CFG_SUMULTI1 + 1));
    suclk[2] = (CTSU_DIAG_STCLK_FREQ * (CTSU_CFG_SUMULTI2 + 1));

    for (i = 0; i < 3; i++)
    {
        suclk_cnt[i % 3] = ((suclk[i % 3] * (CTSU_SNUM_RECOMMEND + 1)) / CTSU_DIAG_STCLK_FREQ) * 8 * 2;

        /* suclk check */
        if ((suclk_cnt[i % 3] + CTSU_CFG_DIAG_CLOCK_RECOV_RANGE) < g_ctsu_diag_info.suclk_count_clk_recv[i])
        {
            return FSP_ERR_CTSU_DIAG_CLOCK_RECOVERY;
        }

        if ((suclk_cnt[i % 3] - CTSU_CFG_DIAG_CLOCK_RECOV_RANGE) > g_ctsu_diag_info.suclk_count_clk_recv[i])
        {
            return FSP_ERR_CTSU_DIAG_CLOCK_RECOVERY;
        }
    }

    /* if all checks passed to this point, return success */
    return FSP_SUCCESS;
}

  #if (CTSU_CFG_NUM_CFC != 0)
static void ctsu_diag_cfc_gain_scan_start (void)
{
    /* CTSU Setting */
    CTSU.CTSUCRA.BIT.MD0    = 1;
    CTSU.CTSUCRA.BIT.MD1    = 0;
    CTSU.CTSUMCH.BIT.MCA0   = 1;
    CTSU.CTSUMCH.BIT.MCA1   = 0;
    CTSU.CTSUMCH.BIT.MCA2   = 0;
    CTSU.CTSUMCH.BIT.MCA3   = 0;
    CTSU.CTSUCRA.BIT.LOAD   = 1;
    CTSU.CTSUCRA.BIT.CFCON  = 1;
    CTSU.CTSUCRA.BIT.PUMPON = 1;
    CTSU.CTSUCRA.BIT.TXVSEL = 1;

    CTSU.CTSUCRB.BIT.SSMOD = 0;
    CTSU.CTSUCRB.BIT.SSCNT = 0;
    CTSU.CTSUCRB.BIT.SST   = 0x1F;

    CTSU.CTSUCRA.BIT.PCSEL  = 1;
    CTSU.CTSUCRA.BIT.SDPSEL = 1;

    CTSU.CTSUCALIB            = 0;
    CTSU.CTSUCALIB.BIT.SUCARRY  = 0;
    CTSU.CTSUCALIB.BIT.CCOCALIB = 0;
    CTSU.CTSUCALIB.BIT.CCOCLK   = 0;
    CTSU.CTSUCALIB.BIT.CFCMODE  = 1;
    CTSU.CTSUCALIB.BIT.CFCRDMD  = 1;

    if (CTSU_DIAG_CHACA_TSMAX > g_ctsu_corrcfc_info.ts_table[0])
    {
        CTSU.CTSUCHACA.LONG = (uint32_t) (1 << g_ctsu_corrcfc_info.ts_table[0]);
        CTSU.CTSUCHACB.LONG = 0;
    }
    else
    {
        CTSU.CTSUCHACA.LONG = 0;
        CTSU.CTSUCHACB.LONG = (uint32_t) (1 << (g_ctsu_corrcfc_info.ts_table[0] - CTSU_DIAG_CHACA_TSMAX));
    }

    CTSU.CTSUCHTRC0 = 0;
    CTSU.CTSUCHTRC1 = 0;
    CTSU.CTSUCHTRC2 = 0;
    CTSU.CTSUCHTRC3 = 0;
    CTSU.CTSUCHTRC4 = 0;

    CTSU.CTSUMCH.BIT.MCH0 = 0;

    CTSU.CTSUCRA.BIT.SDPSEL = 0;
    CTSU.CTSUSUCLK0       =
        (uint16_t) (((g_ctsu_diag_info.loop_count + CTSU_CORRCFC_CENTER_POINT) * CTSU_CORRECTION_SUMULTI) - 1);
    CTSU.CTSUCRA.BIT.SDPSEL = 1;

    g_ctsu_diag_info.ctsuwr.ctsuso = (uint32_t) (CTSU_SNUM_RECOMMEND << 10);
}

static void ctsu_diag_cfc_gain_data_get (void)
{
    g_ctsu_diag_info.cfc_cnt[g_ctsu_diag_info.loop_count] = (uint16_t) g_ctsu_diag_info.ctsuscnt[0];

    g_ctsu_diag_info.loop_count++;
}

static fsp_err_t ctsu_diag_cfc_gain_result (void)
{
    uint8_t k;

    for (k = 0; k < (CTSU_CORRCFC_POINT_NUM - 1); k++)
    {
        if (g_ctsu_diag_info.cfc_cnt[k + 1] > g_ctsu_diag_info.cfc_cnt[k])
        {
            /* PASS */
        }
        else
        {

            /* FAIL */
            return FSP_ERR_CTSU_DIAG_CFC_GAIN;
        }
    }

    return FSP_SUCCESS;
}

  #endif
 #endif
#endif
