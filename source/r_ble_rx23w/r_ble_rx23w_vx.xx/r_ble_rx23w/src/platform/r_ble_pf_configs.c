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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_ble_pf_configs.c
* Description  : The Link Layer & Host Stack parameters for the BLE FIT module configurations.
* History      : DD.MM.YYYY Version Description           
*              : 23.08.2019 1.00    First Release
***********************************************************************************************************************/

#include <stdio.h>
#ifdef NO_USE_BSP
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#else /* NO_USE_BSP */
#include "platform.h"
#endif /* NO_USE_BSP */
#include "r_ble_rx23w_if.h"
#include "r_ble_rx23w_config.h"
#include "r_ble_pf_config_private.h"

#ifndef DISBALE_BLE_SECTION
#if defined(__CCRX__)
#pragma section P BLE_P
#pragma section C BLE_C
#pragma section D BLE_D
#pragma section B BLE_B
#endif /* defined(__CCRX__) */
#endif /* DISBALE_BLE_SECTION */

#ifndef NO_USE_BSP
#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module for Rev.5.00 or higher."
#endif /* R_BSP_VERSION_MAJOR < 5 */
#endif /* NO_USE_BSP */

BLE_SECTION_C(4) const st_ble_rf_notify_t g_ble_rf_notify = {
    .enable = BLE_EVENT_NOTIFY_ENABLE_VAL,

    #if ((BLE_EVENT_NOTIFY_ENABLE_VAL & BLE_EVENT_NOTIFY_START_MASK) != 0)
    .start_cb = r_ble_rf_notify_event_start,
    #endif /* ((BLE_EVENT_NOTIFY_ENABLE_VAL & BLE_EVENT_NOTIFY_START_MASK) != 0) */
    #if ((BLE_EVENT_NOTIFY_ENABLE_VAL & BLE_EVENT_NOTIFY_CLOSE_MASK) != 0)
    .close_cb = r_ble_rf_notify_event_close,
    #endif
    #if ((BLE_EVENT_NOTIFY_ENABLE_VAL & BLE_EVENT_NOTIFY_DS_MASK) != 0)
    .dsleep_cb = r_ble_rf_notify_deep_sleep,
    #endif
};

BLE_SECTION_C(1) const uint8_t  g_ble_dbg_pub_addr[6]  = BLE_CFG_RF_DBG_PUB_ADDR;
BLE_SECTION_C(1) const uint8_t  g_ble_dbg_rand_addr[6] = BLE_CFG_RF_DBG_RAND_ADDR;

/****************************/
/*** Memory customization ***/
/****************************/
#if (BLE_CFG_RF_CONN_MAX >= 1) && (BLE_CFG_RF_CONN_MAX <= 7)
BLE_SECTION_C(2) const uint16_t g_ble_conn_max = BLE_CFG_RF_CONN_MAX;
#else
#error "error: Maximum number of connections setting (BLE_CFG_RF_CONN_MAX)"
#endif

#if (BLE_CFG_RF_CONN_DATA_MAX >= 27) && (BLE_CFG_RF_CONN_DATA_MAX <= 251)
BLE_SECTION_C(2) const uint16_t g_ble_conn_data_max = BLE_CFG_RF_CONN_DATA_MAX;
#else
#error "error: Maximum connection data length (BLE_CFG_RF_CONN_DATA_MAX)"
#endif

#if (BLE_CFG_LIB_TYPE == 0)
#if (BLE_CFG_RF_ADV_DATA_MAX >= 31) && (BLE_CFG_RF_ADV_DATA_MAX <= 1650)
BLE_SECTION_C(2) const uint16_t g_ble_adv_data_max = BLE_CFG_RF_ADV_DATA_MAX;
#else
#error "error: Maximum advertising data length (BLE_CFG_RF_ADV_DATA_MAX)"
#endif

#if (BLE_CFG_RF_ADV_SET_MAX >= 1) && (BLE_CFG_RF_ADV_SET_MAX <= 4)
BLE_SECTION_C(2) const uint16_t g_ble_adv_set_max = BLE_CFG_RF_ADV_SET_MAX;
#else
#error "error: Maximum advertising set number (BLE_CFG_RF_ADV_SET_MAX)"
#endif

#if (BLE_CFG_RF_SYNC_SET_MAX >= 1) && (BLE_CFG_RF_SYNC_SET_MAX <= 2)
BLE_SECTION_C(2) const uint16_t g_ble_sync_set_max = BLE_CFG_RF_SYNC_SET_MAX;
#else
#error "error: Maximum create sync number (BLE_CFG_RF_SYNC_SET_MAX)"
#endif
#endif /* (BLE_CFG_LIB_TYPE == 0) */

/******************************************/
/**** LL management data area (2bytes) ****/
/******************************************/
#if (BLE_CFG_LIB_TYPE != 0)
#define BLE_CNTL_DATA_MIN           (392)
#define BLE_CNTL_DATA_CONN          (65)
#define BLE_CNTL_DATA_ADV           (0)
#define BLE_CNTL_DATA_SYNC          (0)
#else /* (BLE_CFG_LIB_TYPE != 0) */
#define BLE_CNTL_DATA_MIN           (279)
#define BLE_CNTL_DATA_CONN          (65)
#define BLE_CNTL_DATA_ADV           (78)
#define BLE_CNTL_DATA_SYNC          (33)
#endif /* (BLE_CFG_LIB_TYPE != 0) */
#define BLE_CNTL_DATA_MAX                           \
(                                                   \
    (BLE_CNTL_DATA_MIN) +                           \
    (BLE_CNTL_DATA_CONN * BLE_CFG_RF_CONN_MAX) +    \
    (BLE_CNTL_DATA_ADV  * BLE_CFG_RF_ADV_SET_MAX) + \
    (BLE_CNTL_DATA_SYNC * BLE_CFG_RF_SYNC_SET_MAX) +\
    (0)                                             \
)

/******************************************/
/**** BLE stack event heap area (1byte)****/
/******************************************/
#ifdef ENABLE_HCI_MODE
#define BLE_HOST_HEAP_MIN         (0)
#else /* ENABLE_HCI_MODE */
#define BLE_HOST_HEAP_MIN         (3032)
#endif /* ENABLE_HCI_MODE */
#if (BLE_CFG_LIB_TYPE != 0)
#define BLE_CNTL_HEAP_MIN         (88)
#define BLE_CNTL_HEAP_EVENT       (720)
#else /* (BLE_CFG_LIB_TYPE != 0) */
#define BLE_CNTL_HEAP_MIN         (280)
#define BLE_CNTL_HEAP_EVENT       (3784)
#endif /* (BLE_CFG_LIB_TYPE != 0) */
#define BLE_CNTL_HEAP_CONN        (388)
#define _ALIGN_4BYTE(base)        ((((base)+3)>>2)<<2)
#define BLE_CNTL_HEAP_TX_DATA     (_ALIGN_4BYTE(BLE_CFG_RF_CONN_DATA_MAX+4)+20)
#define BLE_CNTL_HEAP_RX_DATA     (_ALIGN_4BYTE(BLE_CFG_RF_CONN_DATA_MAX+8)+4)
#define BLE_CNTL_HEAP_TX2_DATA    (BLE_CFG_RF_CONN_DATA_MAX+8)
#define BLE_CNTL_TXRX_MAX         (4)

#if (BLE_CFG_LIB_TYPE != 0)
#define BLE_CNTL_ADV_DATA_MAX     (0)
#else /* (BLE_CFG_LIB_TYPE != 0) */
#define BLE_ADV_DATA_BLOCKS_LIMIT (36)
#define BLE_ADV_DATA_BLOCKS       ((((BLE_CFG_RF_ADV_DATA_MAX + 251)/252) * BLE_CFG_RF_ADV_SET_MAX) * 2)
#if (BLE_ADV_DATA_BLOCKS > BLE_ADV_DATA_BLOCKS_LIMIT)
#define BLE_CNTL_ADV_DATA_MAX     (BLE_ADV_DATA_BLOCKS_LIMIT * 256)
#else /* (BLE_ADV_DATA_BLOCKS > BLE_ADV_DATA_BLOCKS_LIMIT) */
#define BLE_CNTL_ADV_DATA_MAX     (BLE_ADV_DATA_BLOCKS * 256)
#endif /* (BLE_ADV_DATA_BLOCKS > BLE_ADV_DATA_BLOCKS_LIMIT) */
#endif /* (BLE_CFG_LIB_TYPE != 0) */

#define BLE_CNTL_HEAP_MAX                           \
(                                                   \
    (BLE_CNTL_HEAP_MIN) +                           \
    (BLE_HOST_HEAP_MIN) +                           \
    (BLE_CNTL_HEAP_EVENT) +                         \
    (BLE_CNTL_HEAP_CONN * BLE_CFG_RF_CONN_MAX) +    \
    (BLE_CNTL_HEAP_TX_DATA * BLE_CNTL_TXRX_MAX) +   \
    (BLE_CNTL_HEAP_RX_DATA * BLE_CNTL_TXRX_MAX) +   \
    (BLE_CNTL_ADV_DATA_MAX) +                       \
    (0)                                             \
)

/******************************************/
/**** LL connection entry area (1byte) ****/
/******************************************/
#if (BLE_CFG_LIB_TYPE == 1)
#define BLE_CNTL_CONN_ENT         (328)
#elif (BLE_CFG_LIB_TYPE == 2)
#define BLE_CNTL_CONN_ENT         (316)
#else /* (BLE_CFG_LIB_TYPE == x) */
#define BLE_CNTL_CONN_ENT         (336)
#endif /* (BLE_CFG_LIB_TYPE == x) */
#define BLE_CNTL_CONN_ENT_MAX                       \
(                                                   \
    (BLE_CNTL_CONN_ENT * BLE_CFG_RF_CONN_MAX) +     \
    (0)                                             \
)

/******************************************/
/**** LL Advertising set area (1byte)  ****/
/******************************************/
#define BLE_CNTL_ADV_SET          (152)
#define BLE_CNTL_ADV_SET_MAX                        \
(                                                   \
    (BLE_CNTL_ADV_SET * BLE_CFG_RF_ADV_SET_MAX) +   \
    (0)                                             \
)

BLE_SECTION_B(2) uint16_t g_ble_cntl_data[BLE_CNTL_DATA_MAX];
BLE_SECTION_B(4) uint32_t g_ble_cntl_heap[(BLE_CNTL_HEAP_MAX+3)/4];
BLE_SECTION_B(4) uint32_t g_ble_cntl_heap2[(BLE_CNTL_HEAP_TX2_DATA+3)/4];
BLE_SECTION_B(4) uint32_t g_ble_cntl_conn_ent[(BLE_CNTL_CONN_ENT_MAX+3)/4];
#if (BLE_CFG_LIB_TYPE == 0)
BLE_SECTION_B(4) uint32_t g_ble_cntl_adv_set[(BLE_CNTL_ADV_SET_MAX+3)/4];
BLE_SECTION_C(2) const uint16_t g_ble_adv_block         = (uint16_t)(BLE_CNTL_ADV_DATA_MAX/256);
#endif /* (BLE_CFG_LIB_TYPE == 0) */

#ifdef NO_USE_BSP
const uint16_t g_ble_main_clk_khz      = (uint16_t)BLE_CFG_MCU_MAIN_CLK_KHZ;
const ble_mcu_clock_change_cb_t g_ble_mcu_clock_change_fp = NULL;
#else /* NO_USE_BSP */
#include "r_bsp_config.h"
#if (BSP_CFG_CLKOUT_RF_MAIN == 1) && (BSP_CFG_XTAL_HZ == 4000000) && (BLE_CFG_RF_CLKOUT_EN == 5) 
extern void R_BSP_ConfigClockSetting(void);
BLE_SECTION_C(2) const uint16_t g_ble_main_clk_khz      = (uint16_t)(BSP_CFG_XTAL_HZ/1000);
BLE_SECTION_C(4) const ble_mcu_clock_change_cb_t g_ble_mcu_clock_change_fp = R_BSP_ConfigClockSetting;
#elif (BSP_CFG_CLKOUT_RF_MAIN == 0)
BLE_SECTION_C(2) const uint16_t g_ble_main_clk_khz      = (uint16_t)BLE_CFG_MCU_MAIN_CLK_KHZ;
BLE_SECTION_C(4) const ble_mcu_clock_change_cb_t g_ble_mcu_clock_change_fp = NULL;
#else /* (BSP_CFG_CLKOUT_RF_MAIN == 1) && (BSP_CFG_XTAL_HZ == 4000000) && (BLE_CFG_RF_CLKOUT_EN == 5) */
#error "error: When BSP_CFG_CLKOUT_RF_MAIN is (1), needs to set BSP_CFG_XTAL_HZ (4000000) and BLE_CFG_RF_CLVAL (5)."
#endif /* (BSP_CFG_CLKOUT_RF_MAIN == 1) && (BSP_CFG_XTAL_HZ == 4000000) && (BLE_CFG_RF_CLKOUT_EN == 5)  */
#endif /* NO_USE_BSP */

#if (BLE_CFG_DEV_DATA_CF_BLOCK >= 0) && (BLE_CFG_DEV_DATA_CF_BLOCK <= 255)
BLE_SECTION_C(4) const uint32_t  g_ble_dev_data_cf_addr = BLE_DEV_DATA_CF_ADDR;
#else /* (BLE_CFG_DEV_DATA_CF_BLOCK >= 0) && (BLE_CFG_DEV_DATA_CF_BLOCK <= 255) */
BLE_SECTION_C(4) const uint32_t  g_ble_dev_data_cf_addr = 0U;
#endif /* (BLE_CFG_DEV_DATA_CF_BLOCK >= 0) && (BLE_CFG_DEV_DATA_CF_BLOCK <= 255) */

#if ((BLE_CFG_DEV_DATA_DF_BLOCK  >= 0) && (BLE_CFG_DEV_DATA_DF_BLOCK  <= 7)) && \
    ((BLE_CFG_SECD_DATA_DF_BLOCK >= 0) && (BLE_CFG_SECD_DATA_DF_BLOCK <= 7)) && \
    (BLE_CFG_DEV_DATA_DF_BLOCK == BLE_CFG_SECD_DATA_DF_BLOCK)
#error "error: The same block number is specified for BLE_CFG_DEV_DATA_DF_BLOCK and BLE_CFG_SECD_DATA_DF_BLOCK"
#endif /* (BLE_CFG_DEV_DATA_DF_BLOCK == BLE_CFG_SECD_DATA_DF_BLOCK) */

#if (BLE_CFG_DEV_DATA_DF_BLOCK >= 0) && (BLE_CFG_DEV_DATA_DF_BLOCK <= 7)
BLE_SECTION_C(4) const uint32_t  g_ble_dev_data_df_addr = BLE_DEV_DATA_DF_ADDR;
#else /* (BLE_CFG_DEV_DATA_DF_BLOCK >= 0) && (BLE_CFG_DEV_DATA_DF_BLOCK <= 7) */
BLE_SECTION_C(4) const uint32_t  g_ble_dev_data_df_addr = 0U;
#endif /* (BLE_CFG_DEV_DATA_DF_BLOCK >= 0) && (BLE_CFG_DEV_DATA_DF_BLOCK <= 7) */

BLE_SECTION_C(1) const uint8_t g_ble_rf_config[] =
{
/***************************************/
/**** CLVAL setting                 ****/
/***************************************/
#if (BLE_CFG_RF_CLVAL >= 0) && (BLE_CFG_RF_CLVAL <= 15)
    (BLE_CFG_RF_CLVAL << 0) |
#else  /* BLE_CFG_RF_CLVAL */
    #error "error: CLVAL setting (BLE_CFG_RF_CLVAL)"
#endif /* BLE_CFG_RF_CLVAL */
    0x00, /* base value */

/***************************************/
/**** RF Slow Clock setting         ****/
/***************************************/
#if (BLE_CFG_RF_EXT32K_EN >= 0) && (BLE_CFG_RF_EXT32K_EN <= 1)
    /**** External 32kHz setting ****/
    (BLE_CFG_RF_EXT32K_EN << 0) |
#else  /* BLE_CFG_RF_EXT32K_EN */
    #error "error: RF Slow Clock source setting (BLE_CFG_RF_EXT32K_EN)"
#endif /* BLE_CFG_RF_EXT32K_EN */

#if (BLE_CFG_RF_MCU_CLKOUT_FREQ >= 0) && (BLE_CFG_RF_MCU_CLKOUT_FREQ <= 1)
    /**** MCU CLKOUT setting ****/
    (BLE_CFG_RF_MCU_CLKOUT_FREQ << 1) |
#else  /* BLE_CFG_RF_MCU_CLKOUT_FREQ */
    #error "error: RF Slow Clock frequency setting (BLE_CFG_RF_MCU_CLKOUT_FREQ)"
#endif /* BLE_CFG_RF_MCU_CLKOUT_FREQ */

#if (BLE_CFG_RF_MCU_CLKOUT_PORT >= 0) && (BLE_CFG_RF_MCU_CLKOUT_PORT <= 1) 
    /**** RF_LOCO setting ****/
    (BLE_CFG_RF_MCU_CLKOUT_PORT << 2) |
#else  /* BLE_RF_CONF_RF_LOCO */
    #error "error: MCU CLKOUT port setting (BLE_CFG_RF_MCU_CLKOUT_PORT)"
#endif /* BLE_RF_CONF_RF_LOCO */

/**** Sleep Clock Accuracy (SCA) setting ****/
#if   (BLE_CFG_RF_SCA>=251) && (BLE_CFG_RF_SCA<=500)
    (0x00 << 4) |
#elif (BLE_CFG_RF_SCA>=151) && (BLE_CFG_RF_SCA<=250)
    (0x01 << 4) |
#elif (BLE_CFG_RF_SCA>=101) && (BLE_CFG_RF_SCA<=150)
    (0x02 << 4) |
#elif (BLE_CFG_RF_SCA>=76 ) && (BLE_CFG_RF_SCA<=100)
    (0x03 << 4) |
#elif (BLE_CFG_RF_SCA>=51 ) && (BLE_CFG_RF_SCA<=75 )
    (0x04 << 4) |
#elif (BLE_CFG_RF_SCA>=31 ) && (BLE_CFG_RF_SCA<=50 )
    (0x05 << 4) |
#elif (BLE_CFG_RF_SCA>=21 ) && (BLE_CFG_RF_SCA<=30 )
    (0x06 << 4) |
#elif (BLE_CFG_RF_SCA>=0  ) && (BLE_CFG_RF_SCA<=20 )
    (0x07 << 4) |
#else /* BLE_CFG_RF_SCA */
    #error "error: Sleep Clock Accuracy (SCA) setting (BLE_CFG_RF_SCA)"
#endif /* BLE_CFG_RF_SCA */
    0x00, /* base value */

/***************************************/
/**** Tx Power setting              ****/
/***************************************/
#if (BLE_CFG_RF_DEF_TX_POW >= 0) && (BLE_CFG_RF_DEF_TX_POW <= 2)
    /**** Defalut Tx Power Setting ****/
    (BLE_CFG_RF_DEF_TX_POW << 0) | 
#else /* BLE_CFG_RF_DEF_TX_POW */
    #error "error: default Tx Power setting (BLE_CFG_RF_DEF_TX_POW)"
#endif /* BLE_CFG_RF_DEF_TX_POW */
    0x00, /* base value */

/***************************************/
/**** RF option setting             ****/
/***************************************/
#if (BLE_CFG_RF_DDC_EN >= 0) && (BLE_CFG_RF_DDC_EN <= 1 )

    /**** DC-DC converter setting ****/
    (BLE_CFG_RF_DDC_EN << 0) |

#else  /* BLE_CFG_RF_DDC_EN */
    #error "error: DC-DC converter setting (BLE_CFG_RF_DDC_EN)"
#endif /* BLE_CFG_RF_DDC_EN */
#if (BLE_CFG_RF_MAX_TX_POW >= 0) && (BLE_CFG_RF_MAX_TX_POW <= 1)
    /**** Max Tx Power Setting ****/
    (BLE_CFG_RF_MAX_TX_POW << 1) |
#else /* BLE_CFG_RF_MAX_TX_POW */
    #error "error: Max Tx Power setting (BLE_CFG_RF_MAX_TX_POW)"
#endif /* BLE_CFG_RF_MAX_TX_POW */
#if (BLE_CFG_RF_CLKOUT_EN >= 0) && (BLE_CFG_RF_CLKOUT_EN <= 7)
    /**** RF clock output settng ****/
    (BLE_CFG_RF_CLKOUT_EN << 4) |
#else /* BLE_CFG_RF_CLKOUT_EN  */
    #error "error: RF clock output settng (BLE_CFG_RF_CLKOUT_EN)"
#endif /* BLE_CFG_RF_CLKOUT_EN */

    0x00 /* base value */
};

/***************************************/
/**** Host Stack settings           ****/
/***************************************/
#ifndef ENABLE_HCI_MODE
#define BLE_HOST_L2_SIG_TBL_LEN                  24
#define BLE_HOST_L2_CH_PARAM_TBL_LEN              2
#define BLE_HOST_HCI_REM_TBL_LEN                  6
#define BLE_HOST_SMP_CONFIG_LEN                 108
#define BLE_HOST_GAP_CONN_TBL_LEN                12
#define BLE_HOST_DEV_Q_TBL_LEN                   14
#define BLE_HOST_ATT_CONN_TBL_LEN                16
#define BLE_HOST_GATTS_CNF_TBL_LEN                2

BLE_SECTION_B(4) uint32_t g_ble_host_dev_q_tbl[(BLE_CFG_RF_CONN_MAX * BLE_HOST_DEV_Q_TBL_LEN + 3)/4];
BLE_SECTION_B(4) uint32_t g_ble_host_hci_rem_tbl[(BLE_CFG_RF_CONN_MAX * BLE_HOST_HCI_REM_TBL_LEN + 3)/4];
BLE_SECTION_B(4) uint32_t g_ble_host_l2_sig_tbl[(BLE_CFG_RF_CONN_MAX * BLE_HOST_L2_SIG_TBL_LEN + 3)/4];
BLE_SECTION_B(4) uint32_t g_ble_host_l2_ch_param_tbl[(BLE_CFG_RF_CONN_MAX * BLE_HOST_L2_CH_PARAM_TBL_LEN + 3)/4];
BLE_SECTION_B(4) uint32_t g_ble_host_smp_config_tbl[(BLE_CFG_RF_CONN_MAX * BLE_HOST_SMP_CONFIG_LEN + 3)/4];
BLE_SECTION_B(4) uint32_t g_ble_host_att_conn_tbl[(BLE_CFG_RF_CONN_MAX * BLE_HOST_ATT_CONN_TBL_LEN + 3)/4];
BLE_SECTION_B(4) uint32_t g_ble_host_gap_conn_tbl[(BLE_CFG_RF_CONN_MAX * BLE_HOST_GAP_CONN_TBL_LEN + 3)/4];
BLE_SECTION_B(4) uint32_t g_ble_host_gatts_cnf_tbl[(BLE_CFG_RF_CONN_MAX * BLE_HOST_GATTS_CNF_TBL_LEN + 3)/4];

BLE_SECTION_C(4) const uint32_t g_p_ble_host_config_tbls[] = {
    (uint32_t)g_ble_host_dev_q_tbl,
    (uint32_t)g_ble_host_hci_rem_tbl,
    (uint32_t)g_ble_host_l2_sig_tbl,
    (uint32_t)g_ble_host_l2_ch_param_tbl,
    (uint32_t)g_ble_host_smp_config_tbl,
    (uint32_t)g_ble_host_att_conn_tbl,
    (uint32_t)g_ble_host_gap_conn_tbl,
    (uint32_t)g_ble_host_gatts_cnf_tbl
};

BLE_SECTION_P void ble_host_conn_config(uint32_t ** pp_host_conn_config_table)
{
    * pp_host_conn_config_table = (uint32_t*)g_p_ble_host_config_tbls;
}
#endif /* !ENABLE_HCI_MODE */


#ifndef DISBALE_BLE_SECTION
#if defined(__CCRX__)
#pragma section
#endif /* defined(__CCRX__) */
#endif /* DISBALE_BLE_SECTION */
