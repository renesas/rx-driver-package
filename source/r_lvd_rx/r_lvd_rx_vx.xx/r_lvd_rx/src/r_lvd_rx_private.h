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
* Copyright (C) 2016-2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_lvd_rx_private.h
* Description  : Functions for using LVD on RX devices.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 15.06.2016 2.00     First Release
*              : 01.10.2016 2.10     Added RX65N support.
*              :                     Deleted Tool-Chain version.
*              : 19.12.2016 2.20     Added support for RX24U, RX24T(512KB).
*              :                     Deleted unnecessary header information.
*              : 09.06.2017 2.30     Added support for RX130-512K, RX65N-2M.
*              : 28.09.2018 2.40     Added support for RX66T.
*              : 01.02.2019 2.50     Added support for RX72T, RX65N-64pin.
*              : 30.12.2019 3.40     Added support for RX66N, RX72N.
                                     Added define for LVD_GROUP_INT_ICUD.
***********************************************************************************************************************/
#ifndef LVD_PRIVATE_HEADER_FILE
#define LVD_PRIVATE_HEADER_FILE

/***********************************************************************************************************************
Includes <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"

/* Public interface header file for this package. */
#include "r_lvd_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/* The number of the channels to support in this API. */
#ifndef  BSP_MCU_RX23W
#define LVD_CHANNEL_NUM                         (LVD_CHANNEL_INVALID)
#endif

#if defined(BSP_MCU_RX23W)
#define LVD_CHANNEL_NUM                         (LVD_CHANNEL_2)
#endif

#define LVD_DEFAULT_VALUE                       (0xFFFF)
#define LVD_INVALID_VALUE                       (0xFFFE)

/* These value are used for LVD_CFG_CHANNEL_n_USED and LVD_CFG_DIGITAL_FILTER_CHANNEL_n. */
#define LVD_DISABLE                             (0)
#define LVD_ENABLE                              (1)
#define LVD_ENABLE_INVALID                      (2)

/* These value are used for LVD_CFG_VDET_TARGET_CHANNEL_n. */
#define LVD_VDET_TARGET_VCC                     (0)
#define LVD_VDET_TARGET_CMPA                    (1)
#define LVD_VDET_TARGET_INVALID                 (2)

/* These value are used for LVD_CFG_ACTION_CHANNEL_n. */
#define LVD_ACTION_RESET                        (0)
#define LVD_ACTION_NMI                          (1)
#define LVD_ACTION_MI                           (2)
#define LVD_ACTION_NONE                         (3)
#define LVD_ACTION_INVALID                      (4)

/* These value are used for LVD_CFG_STABILIZATION_CHANNEL_n. */
#define LVD_STAB_RECOVARY                       (0)
#define LVD_STAB_RESET_ASSERT                   (1)
#define LVD_STAB_INVALID                        (2)

/* These value are used for LVD_GROUP_SETUP. */
#define LVD_GROUP_SETUP_LVDA_1                  (0x101)
#define LVD_GROUP_SETUP_LVDA_2                  (0x102)
#define LVD_GROUP_SETUP_LVDAa_1                 (0x111)
#define LVD_GROUP_SETUP_LVDAb_1                 (0x121)

/* These value are used for LVD_GROUP_INT. */
#define LVD_GROUP_INT_ICUb                      (0x020)
#define LVD_GROUP_INT_ICUA                      (0x100)
#define LVD_GROUP_INT_ICUB                      (0x200)
#define LVD_GROUP_INT_ICUD                      (0x300)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Structure body of the configuration option. */
typedef struct
{
    uint16_t    lvd_ch_used;
    uint16_t    lvd_target;
    uint16_t    lvd_voltage_level_value;
    uint16_t    lvd_dfilter;
    uint16_t    lvd_dfilter_div_value;
    uint32_t    lvd_delay_dfilter_enable;
    uint16_t    lvd_action;
    uint16_t    lvd_int_prio;
    uint16_t    lvd_reset_negate;
}lvd_cfg_opt_t;

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

/* Error check functions */
extern lvd_err_t lvd_hw_check_param_ch(lvd_channel_t ch, lvd_cfg_opt_t const *p_cfg_opt);
extern lvd_err_t lvd_hw_check_ptr(void const *p_ptr);
extern lvd_err_t lvd_hw_check_already_open(bool b_open_flag);
extern lvd_err_t lvd_hw_check_not_opened(bool b_open_flag);
extern lvd_err_t lvd_hw_check_getstatus(void);
extern lvd_err_t lvd_hw_check_clearstatus(void);
extern lvd_err_t lvd_hw_check_param_open(lvd_channel_t ch, 
                                         lvd_config_t const *p_cfg, 
                                         void (*p_cb)(void *), 
                                         lvd_cfg_opt_t const *p_cfg_opt);
extern lvd_err_t lvd_hw_check_loco_limitation(lvd_cfg_opt_t const *p_cfg_opt);

/* Setup functions */
extern void lvd_hw_clear_lvd_status(lvd_channel_t ch);
extern void lvd_hw_get_lvd_status(lvd_channel_t ch, lvd_status_position_t *p_pos, lvd_status_cross_t *p_cross);
extern void lvd_hw_set_level(lvd_channel_t ch, uint16_t level_value);
extern void lvd_hw_set_trigger(lvd_channel_t ch, lvd_trigger_t trigger);
extern void lvd_hw_set_target(lvd_channel_t ch, uint16_t target);
extern void lvd_hw_select_reset(lvd_channel_t ch);
extern void lvd_hw_setup_reset(lvd_channel_t ch, uint16_t lvd_reset_negate);
extern void lvd_hw_select_int(lvd_channel_t ch);
extern void lvd_hw_select_mi(lvd_channel_t ch);
extern void lvd_hw_select_nmi(lvd_channel_t ch);
extern void lvd_hw_setup_dfilter(lvd_channel_t ch, uint16_t clock_value);
extern void lvd_hw_get_circuit_enable(lvd_channel_t ch, bool *b_penable_flag);
extern void lvd_hw_get_reset_int_enable(lvd_channel_t ch, bool *b_penable_flag);
extern void lvd_hw_get_dfilter_enable(lvd_channel_t ch, bool *b_penable_flag);

/* Enable functions */
extern void lvd_hw_enable_output(lvd_channel_t ch, bool b_enable_flag);
extern void lvd_hw_enable_circuit(lvd_channel_t ch, bool b_enable_flag);
extern void lvd_hw_enable_reset_int(lvd_channel_t ch, bool b_enable_flag);
extern void lvd_hw_enable_dfilter(lvd_channel_t ch, bool enable_flag);
extern void lvd_hw_enable_mi(lvd_channel_t ch, uint16_t prio, bool b_enable_flag);
extern void lvd_hw_enable_nmi(lvd_channel_t ch, bool b_enable_flag);

/* Dummy read functions */
extern void lvd_hw_dummy_read_dfilter(lvd_channel_t ch);
extern void lvd_hw_dummy_read_circuit(lvd_channel_t ch);
extern void lvd_hw_dummy_read_output(lvd_channel_t ch);

/* Except LVD functions */
extern void lvd_hw_set_mi_cb(lvd_channel_t ch, void (*p_cb)(void *));
extern void lvd_hw_set_nmi_cb(lvd_channel_t ch, void (*p_cb)(void *));
extern void lvd_hw_enable_reg_protect(bool b_enable_flag);
extern void lvd_hw_wait_delay(uint32_t usec);
extern void lvd_hw_wait_delay_loco(uint32_t loco_cycle);

#endif/* LVD_PRIVATE_HEADER_FILE */

/* End of File */

