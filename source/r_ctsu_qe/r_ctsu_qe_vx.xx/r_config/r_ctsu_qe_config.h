/* Generated configuration header file - do not edit */
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
 * File Name    : r_ctsu_qe_config.h
 * Description  : Configures the QE CTSU API module.
 ***********************************************************************************************************************/
#ifndef CTSU_CONFIG_HEADER_FILE
#define CTSU_CONFIG_HEADER_FILE


/***********************************************************************************************************************
 Configuration Options
 ***********************************************************************************************************************/
/* SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING
 * Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting
 * Setting to 1 includes parameter checking; 0 compiles out parameter checking
 */
#define CTSU_CFG_PARAM_CHECKING_ENABLE      (BSP_CFG_PARAM_CHECKING_ENABLE)

/* Data transfer of INTCTSUWR and INTCTSURD. To use DTC, set this to 1. */
#define CTSU_CFG_DTC_SUPPORT_ENABLE         (0)

/* Overall automatic judgement setting. To use, set this to 1. */
#define CTSU_CFG_AUTO_JUDGE_ENABLE          (0)

/* Interrupt priority level */
#define CTSU_CFG_INTCTSUWR_PRIORITY_LEVEL   (2)
#define CTSU_CFG_INTCTSURD_PRIORITY_LEVEL   (2)
#define CTSU_CFG_INTCTSUFN_PRIORITY_LEVEL   (2)

/* The following configurations are set according to the touch interface configuration,
 * so they are not set in Smart Configurator.
 * Set when using QE for Capacitive Touch. In that case, QE_TOUCH_CONFIGURATION is defined in the project,
 * the definition in r_ctsu_config.h is invalid, and instead it is defined in qe_touch_define.h.
 */

#ifndef QE_TOUCH_CONFIGURATION
#if(TOUCH_CFG_SERIAL_TUNING_SUPPORT == 1)
#define CTSU_CFG_NUM_SELF_ELEMENTS                (35)
#define CTSU_CFG_NUM_MUTUAL_ELEMENTS              (255)
#else
#define CTSU_CFG_NUM_SELF_ELEMENTS                (1)
#define CTSU_CFG_NUM_MUTUAL_ELEMENTS              (0)
#endif
#define CTSU_CFG_LOW_VOLTAGE_MODE                 (0)
#define CTSU_CFG_PCLK_DIVISION                    (0)
#define CTSU_CFG_TSCAP_PORT                       (0xFFFF)
#define CTSU_CFG_VCC_MV                           (5000)
#define CTSU_CFG_DIAG_SUPPORT_ENABLE              (0)
#if (BSP_MCU_RX140)
#define CTSU_CFG_NUM_CFC                          (0)
#define CTSU_CFG_NUM_CFC_TX                       (0)
#if(TOUCH_CFG_SERIAL_TUNING_SUPPORT == 1)
#define CTSU_CFG_NUM_SUMULTI                      (1)
#else
#define CTSU_CFG_NUM_SUMULTI                      (3)
#endif
#define CTSU_CFG_SUMULTI0                         (0x3F)
#define CTSU_CFG_SUMULTI1                         (0x36)
#define CTSU_CFG_SUMULTI2                         (0x48)
#define CTSU_CFG_TEMP_CORRECTION_SUPPORT          (0)
#define CTSU_CFG_TEMP_CORRECTION_TS               (0)
#define CTSU_CFG_TEMP_CORRECTION_TIME             (0)
#define CTSU_CFG_CALIB_RTRIM_SUPPORT              (0)
#else
#define CTSU_CFG_NUM_SUMULTI                      (1)
#endif
#endif

#if (CTSU_CFG_DIAG_SUPPORT_ENABLE == 1)
#if  (defined(BSP_MCU_RX113) || defined(BSP_MCU_RX130) || defined(BSP_MCU_RX23_ALL))
#if (CTSU_CFG_LOW_VOLTAGE_MODE == 0)
#define CTSU_CFG_DIAG_CCO_HIGH_MAX                (54888)
#define CTSU_CFG_DIAG_CCO_HIGH_MIN                (29062)
#define CTSU_CFG_DIAG_CCO_LOW_MAX                 (3269)
#define CTSU_CFG_DIAG_CCO_LOW_MIN                 (705)
#define CTSU_CFG_DIAG_SSCG_MAX                    (21813)
#define CTSU_CFG_DIAG_SSCG_MIN                    (11782)
#define CTSU_CFG_DIAG_DAC1_MAX                    (20422)
#define CTSU_CFG_DIAG_DAC2_MAX                    (21532)
#define CTSU_CFG_DIAG_DAC3_MAX                    (23015)
#define CTSU_CFG_DIAG_DAC4_MAX                    (25923)
#define CTSU_CFG_DIAG_DAC5_MAX                    (32016)
#define CTSU_CFG_DIAG_DAC6_MAX                    (45795)
#define CTSU_CFG_DIAG_DAC1_MIN                    (12492)
#define CTSU_CFG_DIAG_DAC2_MIN                    (14053)
#define CTSU_CFG_DIAG_DAC3_MIN                    (15322)
#define CTSU_CFG_DIAG_DAC4_MIN                    (18070)
#define CTSU_CFG_DIAG_DAC5_MIN                    (23262)
#define CTSU_CFG_DIAG_DAC6_MIN                    (33446)
#else
#define CTSU_CFG_DIAG_CCO_HIGH_MAX                (43910)
#define CTSU_CFG_DIAG_CCO_HIGH_MIN                (23249)
#define CTSU_CFG_DIAG_CCO_LOW_MAX                 (2615)
#define CTSU_CFG_DIAG_CCO_LOW_MIN                 (564)
#define CTSU_CFG_DIAG_SSCG_MAX                    (21813)
#define CTSU_CFG_DIAG_SSCG_MIN                    (11782)
#define CTSU_CFG_DIAG_DAC1_MAX                    (16599)
#define CTSU_CFG_DIAG_DAC2_MAX                    (17226)
#define CTSU_CFG_DIAG_DAC3_MAX                    (18412)
#define CTSU_CFG_DIAG_DAC4_MAX                    (20738)
#define CTSU_CFG_DIAG_DAC5_MAX                    (25613)
#define CTSU_CFG_DIAG_DAC6_MAX                    (36636)
#define CTSU_CFG_DIAG_DAC1_MIN                    (9994)
#define CTSU_CFG_DIAG_DAC2_MIN                    (11242)
#define CTSU_CFG_DIAG_DAC3_MIN                    (12258)
#define CTSU_CFG_DIAG_DAC4_MIN                    (14456)
#define CTSU_CFG_DIAG_DAC5_MIN                    (18610)
#define CTSU_CFG_DIAG_DAC6_MIN                    (26757)
#endif
#endif
#if  defined(BSP_MCU_RX671)
#define CTSU_CFG_DIAG_CCO_HIGH_MAX                (36873)
#define CTSU_CFG_DIAG_CCO_HIGH_MIN                (24433)
#define CTSU_CFG_DIAG_CCO_LOW_MAX                 (1781)
#define CTSU_CFG_DIAG_CCO_LOW_MIN                 (105)
#define CTSU_CFG_DIAG_SSCG_MAX                    (17795)
#define CTSU_CFG_DIAG_SSCG_MIN                    (9610)
#define CTSU_CFG_DIAG_DAC1_MAX                    (20422)
#define CTSU_CFG_DIAG_DAC2_MAX                    (21532)
#define CTSU_CFG_DIAG_DAC3_MAX                    (23015)
#define CTSU_CFG_DIAG_DAC4_MAX                    (25923)
#define CTSU_CFG_DIAG_DAC5_MAX                    (32016)
#define CTSU_CFG_DIAG_DAC6_MAX                    (45795)
#define CTSU_CFG_DIAG_DAC1_MIN                    (12492)
#define CTSU_CFG_DIAG_DAC2_MIN                    (14053)
#define CTSU_CFG_DIAG_DAC3_MIN                    (15322)
#define CTSU_CFG_DIAG_DAC4_MIN                    (18070)
#define CTSU_CFG_DIAG_DAC5_MIN                    (23262)
#define CTSU_CFG_DIAG_DAC6_MIN                    (33446)
#endif

#if defined(BSP_MCU_RX140)
#define CTSU_DIAG_TSCAP_RANGE_LOW                 (1050)
#define CTSU_DIAG_TSCAP_RANGE_HIGH                (1419)

#define CTSU_CFG_DIAG_LOAD_REISTER_MIN            (11951)
#define CTSU_CFG_DIAG_LOAD_REISTER_MAX            (23957)

#define CTSU_CFG_DIAG_CURRENT_SOURCE_DIFF_MIN     (2890)
#define CTSU_CFG_DIAG_CURRENT_SOURCE_DIFF_MAX     (5703)

#if (CTSU_CFG_LOW_VOLTAGE_MODE == 0)
#define CTSU_CFG_DIAG_DAC1_MIN                    (1486)
#define CTSU_CFG_DIAG_DAC2_MIN                    (2890)
#define CTSU_CFG_DIAG_DAC3_MIN                    (4202)
#define CTSU_CFG_DIAG_DAC4_MIN                    (5448)
#define CTSU_CFG_DIAG_DAC5_MIN                    (6641)
#define CTSU_CFG_DIAG_DAC6_MIN                    (7787)
#define CTSU_CFG_DIAG_DAC7_MIN                    (8891)
#define CTSU_CFG_DIAG_DAC8_MIN                    (9954)
#define CTSU_CFG_DIAG_DAC9_MIN                    (10974)
#define CTSU_CFG_DIAG_DAC10_MIN                   (11951)
#define CTSU_CFG_DIAG_DAC11_MIN                   (12880)
#define CTSU_CFG_DIAG_DAC12_MIN                   (13761)

#define CTSU_CFG_DIAG_DAC1_MAX                    (3057)
#define CTSU_CFG_DIAG_DAC2_MAX                    (5703)
#define CTSU_CFG_DIAG_DAC3_MAX                    (8292)
#define CTSU_CFG_DIAG_DAC4_MAX                    (10838)
#define CTSU_CFG_DIAG_DAC5_MAX                    (13269)
#define CTSU_CFG_DIAG_DAC6_MAX                    (15593)
#define CTSU_CFG_DIAG_DAC7_MAX                    (17816)
#define CTSU_CFG_DIAG_DAC8_MAX                    (19948)
#define CTSU_CFG_DIAG_DAC9_MAX                    (21992)
#define CTSU_CFG_DIAG_DAC10_MAX                   (23957)
#define CTSU_CFG_DIAG_DAC11_MAX                   (25843)
#define CTSU_CFG_DIAG_DAC12_MAX                   (27651)

#define CTSU_CFG_DIAG_DAC1_2_DIFF_MIN             (1384)
#define CTSU_CFG_DIAG_DAC2_3_DIFF_MIN             (1293)
#define CTSU_CFG_DIAG_DAC3_4_DIFF_MIN             (1228)
#define CTSU_CFG_DIAG_DAC4_5_DIFF_MIN             (1176)
#define CTSU_CFG_DIAG_DAC5_6_DIFF_MIN             (1129)
#define CTSU_CFG_DIAG_DAC6_7_DIFF_MIN             (1083)
#define CTSU_CFG_DIAG_DAC7_8_DIFF_MIN             (1037)
#define CTSU_CFG_DIAG_DAC8_9_DIFF_MIN             (987)
#define CTSU_CFG_DIAG_DAC9_10_DIFF_MIN            (929)
#define CTSU_CFG_DIAG_DAC10_11_DIFF_MIN           (867)
#define CTSU_CFG_DIAG_DAC11_12_DIFF_MIN           (807)

#define CTSU_CFG_DIAG_DAC1_2_DIFF_MAX             (2781)
#define CTSU_CFG_DIAG_DAC2_3_DIFF_MAX             (2678)
#define CTSU_CFG_DIAG_DAC3_4_DIFF_MAX             (2567)
#define CTSU_CFG_DIAG_DAC4_5_DIFF_MAX             (2458)
#define CTSU_CFG_DIAG_DAC5_6_DIFF_MAX             (2355)
#define CTSU_CFG_DIAG_DAC6_7_DIFF_MAX             (2255)
#define CTSU_CFG_DIAG_DAC7_8_DIFF_MAX             (2166)
#define CTSU_CFG_DIAG_DAC8_9_DIFF_MAX             (2077)
#define CTSU_CFG_DIAG_DAC9_10_DIFF_MAX            (1998)
#define CTSU_CFG_DIAG_DAC10_11_DIFF_MAX           (1921)
#define CTSU_CFG_DIAG_DAC11_12_DIFF_MAX           (1841)
#else

#define CTSU_CFG_DIAG_DAC1_MIN                    (1039)
#define CTSU_CFG_DIAG_DAC2_MIN                    (2113)
#define CTSU_CFG_DIAG_DAC3_MIN                    (3130)
#define CTSU_CFG_DIAG_DAC4_MIN                    (4108)
#define CTSU_CFG_DIAG_DAC5_MIN                    (5049)
#define CTSU_CFG_DIAG_DAC6_MIN                    (5961)
#define CTSU_CFG_DIAG_DAC7_MIN                    (6848)
#define CTSU_CFG_DIAG_DAC8_MIN                    (7713)
#define CTSU_CFG_DIAG_DAC9_MIN                    (8554)
#define CTSU_CFG_DIAG_DAC10_MIN                   (9374)
#define CTSU_CFG_DIAG_DAC11_MIN                   (10176)
#define CTSU_CFG_DIAG_DAC12_MIN                   (10945)

#define CTSU_CFG_DIAG_DAC1_MAX                    (2627)
#define CTSU_CFG_DIAG_DAC2_MAX                    (4899)
#define CTSU_CFG_DIAG_DAC3_MAX                    (6974)
#define CTSU_CFG_DIAG_DAC4_MAX                    (8907)
#define CTSU_CFG_DIAG_DAC5_MAX                    (10729)
#define CTSU_CFG_DIAG_DAC6_MAX                    (12476)
#define CTSU_CFG_DIAG_DAC7_MAX                    (14314)
#define CTSU_CFG_DIAG_DAC8_MAX                    (16089)
#define CTSU_CFG_DIAG_DAC9_MAX                    (17802)
#define CTSU_CFG_DIAG_DAC10_MAX                   (19462)
#define CTSU_CFG_DIAG_DAC11_MAX                   (21067)
#define CTSU_CFG_DIAG_DAC12_MAX                   (22624)

#define CTSU_CFG_DIAG_DAC1_2_DIFF_MIN             (1081)
#define CTSU_CFG_DIAG_DAC2_3_DIFF_MIN             (1013)
#define CTSU_CFG_DIAG_DAC3_4_DIFF_MIN             (967)
#define CTSU_CFG_DIAG_DAC4_5_DIFF_MIN             (929)
#define CTSU_CFG_DIAG_DAC5_6_DIFF_MIN             (899)
#define CTSU_CFG_DIAG_DAC6_7_DIFF_MIN             (867)
#define CTSU_CFG_DIAG_DAC7_8_DIFF_MIN             (844)
#define CTSU_CFG_DIAG_DAC8_9_DIFF_MIN             (821)
#define CTSU_CFG_DIAG_DAC9_10_DIFF_MIN            (794)
#define CTSU_CFG_DIAG_DAC10_11_DIFF_MIN           (762)
#define CTSU_CFG_DIAG_DAC11_12_DIFF_MIN           (733)

#define CTSU_CFG_DIAG_DAC1_2_DIFF_MAX             (2273)
#define CTSU_CFG_DIAG_DAC2_3_DIFF_MAX             (2118)
#define CTSU_CFG_DIAG_DAC3_4_DIFF_MAX             (2044)
#define CTSU_CFG_DIAG_DAC4_5_DIFF_MAX             (1975)
#define CTSU_CFG_DIAG_DAC5_6_DIFF_MAX             (1909)
#define CTSU_CFG_DIAG_DAC6_7_DIFF_MAX             (1846)
#define CTSU_CFG_DIAG_DAC7_8_DIFF_MAX             (1786)
#define CTSU_CFG_DIAG_DAC8_9_DIFF_MAX             (1731)
#define CTSU_CFG_DIAG_DAC9_10_DIFF_MAX            (1679)
#define CTSU_CFG_DIAG_DAC10_11_DIFF_MAX           (1632)
#define CTSU_CFG_DIAG_DAC11_12_DIFF_MAX           (1581)
#endif
#define CTSU_CFG_DIAG_CLOCK_RECOV_RANGE           (20)
#endif
#endif


#endif /* TOUCH_CONFIG_HEADER_FILE */
