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
 * File Name    : r_ble_abs_api.c
 * Description  : Functions for BLE Abstraction API. 
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * History      : DD.MM.YYYY Version  Description
 *              : 23.08.2019 1.00     First Release
 **********************************************************************************************************************/

/***********************************************************************************************************************
  Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "r_ble_abs_api.h"
#include "sec_data/r_ble_sec_data.h"

#include "timer/r_ble_timer.h"

#if (BLE_CFG_ABS_API_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0)

/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/
/**********************************/
/*  Define for advertising handle */
/**********************************/
/* Advertising Handle for Legacy Advertising. */
#define BLE_ABS_LEGACY_HDL                                              (0x00)

/* Advertising Handle for Extended Advertising. */
#define BLE_ABS_EXT_HDL                                                 (0x01)

/* Advertising Handle for Non-Connectable Advertising. */
#define BLE_ABS_NON_CONN_HDL                                            (0x02)

/* Advertising Handle for Periodic Advertising. */
#define BLE_ABS_PERD_HDL                                                (0x03)

/* Advertising Handle for Legacy Advertising */
#define BLE_ABS_COMMON_HDL                                              (0x00)

/**********************************/
/*  Define for advertising status */
/**********************************/
/*  set fast advertising parameters */
#define BLE_ABS_ADV_STATUS_PARAM_FAST                                   (0x00000001)
/*  set slow advertising parameters */
#define BLE_ABS_ADV_STATUS_PARAM_SLOW                                   (0x00000002)
/*  set advertising data */
#define BLE_ABS_ADV_STATUS_ADV_DATA                                     (0x00000010)
/*  set scan response data */
#define BLE_ABS_ADV_STATUS_SRES_DATA                                    (0x00000020)
/*  set periodic advertising data */
#define BLE_ABS_ADV_STATUS_PERD_DATA                                    (0x00000040)
/*  start fast advertising */
#define BLE_ABS_ADV_STATUS_ADV_FAST_START                               (0x00000100)
/*  start slow advertising */
#define BLE_ABS_ADV_STATUS_ADV_SLOW_START                               (0x00000200)
/*  set periodic advertising parameters */
#define BLE_ABS_ADV_STATUS_PERD_PARAM                                   (0x00001000)
/*  start periodic advertising */
#define BLE_ABS_ADV_STATUS_PERD_START                                   (0x00010000)
/*  set legacy adv for legacy advertising  */
#define BLE_ABS_ADV_COMM_LEG                                            (0x00100000)
/*  set non-connectable adv for legacy advertising  */
#define BLE_ABS_ADV_COMM_NON                                            (0x00200000)
/*  set non-connectable adv for legacy advertising  */
#define BLE_ABS_ADV_COMM_TO                                             (0x01000000)

/**********************************/
/*  Define for scan status        */
/**********************************/
/*  start fast scan */
#define BLE_ABS_SCAN_STATUS_FAST_START                                  (0x00000001)
/*  start slow scan */
#define BLE_ABS_SCAN_STATUS_SLOW_START                                  (0x00000002)
/*  maximum filter data length */
#define BLE_ABS_SCAN_MAX_FILT_LENT                                      (0x10)

/**********************************/
/*  Define for privacy status     */
/**********************************/
/*  create irk */
#define BLE_ABS_PV_STATUS_CREATE_IRK                                    (0x00000001)
/*  add irk to resolving list */
#define BLE_ABS_PV_STATUS_ADD_RSLV                                      (0x00000002)
/*  set privacy mode */
#define BLE_ABS_PV_STATUS_SET_MODE                                      (0x00000004)
/*  enable resolvable private address function */
#define BLE_ABS_PV_STATUS_EN_RPA                                        (0x00000008)

/**********************************/
/*  Define for create connection  */
/**********************************/
/*  scan interval for connection request with 1M & 2M PHY */
#define BLE_ABS_CONN_SC_INTV_FAST                                       (0x0060)
/*  scan window for connection request with 1M & 2M PHY */
#define BLE_ABS_CONN_SC_WINDOW_FAST                                     (0x0030)
/*  scan interval for connection request with coded PHY */
#define BLE_ABS_CONN_SC_INTV_SLOW                                       (0x0180)
/*  scan window for connection request with coded PHY */
#define BLE_ABS_CONN_SC_WINDOW_SLOW                                     (0x0090)

/*  minimum advertising data length */
#define BLE_ABS_LEGACY_ADV_DATA_LEN                                     (31)
#define BLE_ABS_CONN_EXT_ADV_DATA_LEN                                   (229)


/***********************************************************************************************************************
 Local Typedef definitions
 **********************************************************************************************************************/
/*  advertising set parameters structure */
typedef struct 
{
    union 
    {
        st_ble_abs_legacy_adv_param_t legacy; /* Legacy advertising parameters. */
        st_ble_abs_ext_adv_param_t ext; /* Extended advertising parameters. */
        st_ble_abs_non_conn_adv_param_t non_conn; /* Non-Connectable advertising parameters. */
        st_ble_abs_perd_adv_param_t perd; /* Periodic advertising parameters. */
    } param;  /* Advertising parameters. */

    uint32_t status; /* Advertising status. */

    st_ble_dev_addr_t rem_addr; /* Remote device address for direct advertising. */

} st_abs_adv_param_t;

/*  scan parameters structure */
typedef struct 
{
    st_ble_abs_scan_param_t param; /* Scan parameters. */
    st_ble_abs_scan_phy_param_t phy_1M; /* 1M phy parameters for scan. */
    st_ble_abs_scan_phy_param_t phy_coded; /* Coded phy parameters for scan. */
    uint32_t status; /* Scan status. */
} st_abs_scan_param_t;

/***********************************************************************************************************************
 Private global variables and functions
 **********************************************************************************************************************/
static ble_gap_app_cb_t gs_abs_gap_cb = NULL; /* GAP callback function */
static ble_vs_app_cb_t  gs_abs_vs_cb = NULL; /* Vendor specific callback function */
static uint32_t gs_conn_t_hdl; /* Cancel a request for connection timer. */
#if (BLE_CFG_LIB_TYPE != 0)
static uint32_t gs_adv_t_hdl; /* Advertising timer for legacy advertising */
static uint32_t gs_scan_t_hdl; /* Scaning timer for legacy scaning */
#endif /* (BLE_CFG_LIB_TYPE != 0) */

static st_abs_adv_param_t gs_adv_sets[BLE_MAX_NO_OF_ADV_SETS_SUPPORTED]; /* Advertising set information. */
static st_abs_scan_param_t gs_abs_scan; /* Scan information. */
static st_ble_dev_addr_t gs_loc_bd_addr; /* Local device address. */
static uint8_t gs_privacy_mode = BLE_GAP_NET_PRIV_MODE; /* Privacy mode. */
static uint32_t gs_set_privacy_status = 0; /* Local privacy status. */

static ble_status_t set_pair_param(st_ble_abs_pairing_param_t * p_pairing_param);
static ble_status_t conv_legacy_adv_param(st_ble_abs_legacy_adv_param_t * p_param, 
                                          st_ble_gap_adv_param_t * p_adv_param);
#if (BLE_CFG_LIB_TYPE == 0)
static ble_status_t conv_ext_adv_param(st_ble_abs_ext_adv_param_t * p_param, 
                                       st_ble_gap_adv_param_t * p_adv_param);
#endif /* (BLE_CFG_LIB_TYPE == 0) */
static ble_status_t conv_non_conn_adv_param(st_ble_abs_non_conn_adv_param_t * p_param, 
                                            st_ble_gap_adv_param_t * p_adv_param,
                                            uint8_t adv_hdl);
static ble_status_t abs_adv_rept_hdlr(st_ble_evt_data_t * p_event_data);
static ble_status_t check_scan_phy_param(st_ble_abs_scan_phy_param_t * p_scan_phy);
static ble_status_t set_scan_param(st_ble_abs_scan_param_t * p_scan_param);

static void abs_gapcb(uint16_t event_type, ble_status_t event_result, st_ble_evt_data_t * p_event_data);
static void abs_vscb(uint16_t event_type, ble_status_t event_result, st_ble_vs_evt_data_t * p_event_data);
static void set_abs_cb(ble_gap_app_cb_t cb, ble_vs_app_cb_t vs_cb);
static void set_adv_status(uint8_t adv_hdl, uint32_t set, uint32_t clear);
static void set_adv_param(void * p_adv_param, uint8_t adv_hdl);
static void cancel_conn_func(uint32_t hdl);
#if (BLE_CFG_LIB_TYPE != 0)
static void adv_to_func(uint32_t hdl);
static void ads_scan_on_hdlr(void);
#endif /* (BLE_CFG_LIB_TYPE != 0) */

static void set_scan_status(uint32_t set, uint32_t clear);
static void update_data_status(uint32_t status, uint8_t * p_data, uint16_t data_len, uint8_t adv_hdl);
static void ads_conn_ind_hdlr(void);
static void ads_scan_to_hdlr(uint32_t hdl);
static void ads_scan_off_hdlr(void);
static void abs_loc_ver_hdlr(st_ble_evt_data_t * p_event_data);
static void abs_adv_param_set_hdlr(st_ble_evt_data_t * p_event_data);
static void abs_adv_data_set_hdlr(st_ble_evt_data_t * p_event_data);
static void abs_perd_param_hdlr(void);
static void abs_adv_off_hdlr(st_ble_evt_data_t * p_event_data);
static void abs_conf_rslv_hdlr(st_ble_evt_data_t * p_event_data);
static void abs_rand_hdlr(st_ble_vs_evt_data_t * p_event_data);
static void set_irk_to_rslv(uint8_t * p_lc_irk);
static void adv_start(uint8_t adv_hdl);
static void adv_set_data(uint8_t adv_hdl, uint8_t data_type);
static void set_legacy_sres_data(void);
static void conv_scan_phy_param(st_ble_abs_scan_phy_param_t * p_abs_phy, 
                         st_ble_gap_scan_phy_param_t * p_gap_phy, 
                         st_ble_gap_scan_on_t * p_scan_enable);
static void set_conn_param(st_ble_abs_conn_phy_param_t * p_abs_conn_param, 
                           st_ble_gap_conn_phy_param_t * p_conn_phy_param,
                           st_ble_gap_conn_param_t * p_conn_param);
static void conv_scan_param(st_ble_gap_scan_param_t * p_scan_param, 
                            st_ble_gap_scan_on_t * p_scan_enable, 
                            uint32_t status);
static void set_conn_adv_intv(st_ble_gap_adv_param_t * p_adv_param, 
                                 uint32_t fast_adv_intv, 
                                 uint32_t slow_adv_intv, 
                                 uint16_t fast_period);



/***********************************************************************************************************************
 Exported global functions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Function Name: R_BLE_ABS_Reset
 * Description  : BLE is reset with this function.  The process is carried out in the following order.
 *              : R_BLE_Close() -> R_BLE_GAP_Terminate() -> R_BLE_Open() -> R_BLE_SetEvent().
 *              : The init_cb callback initializes the others (Host Stack, timer, etc...).
 * Arguments    : ble_event_cb_t init_cb                      ; callback functions 
 * Return Value : none
 **********************************************************************************************************************/
void R_BLE_ABS_Reset(ble_event_cb_t init_cb)
{
    R_BLE_Close();

    R_BLE_GAP_Terminate();

    R_BLE_Open();

    if(init_cb)
    {
        R_BLE_SetEvent(init_cb);
    }
}

/***********************************************************************************************************************
 * Function Name: R_BLE_ABS_Init
 * Description  : Host stack is initialized with this function. Before using All the R_BLE APIs, 
 *              : it's necessary to call this function. A callback functions are registered with this function. 
 *              : In order to receive the GAP, GATT, Vendor specific event, 
 *              : it's necessary to register a callback function.
 *              : The result of this API call is notified in BLE_GAP_EVENT_STACK_ON event.
 * Arguments    : st_ble_abs_init_param_t * p_init_param      ; callback functions and pairing parameters
 * Return Value : BLE_SUCCESS(0x0000)                         ; Success 
 *                BLE_ERR_INVALID_PTR(0x0001)                 ; gap_cb is specified as NULL. 
 *                BLE_ERR_INVALID_ARG(0x0003)                 ; The pairing parameter is out of range. 
 *                BLE_ERR_INVALID_STATE(0x0008)               ; The reason for this error is as follows:
 *                                                            ;    - Host Stack was already initialized.
 *                                                            ;    - The task for host stack is not running.  
 *                BLE_ERR_CONTEXT_FULL(0x000B)                ; Callback function has already registered.
 *                BLE_ERR_MEM_ALLOC_FAILED(0x000C)            ; Insufficient memory is needed to generate this function. 
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_Init(st_ble_abs_init_param_t * p_init_param)
{
    ble_status_t retval = BLE_SUCCESS;
    int32_t i;

    if((NULL == p_init_param) || (NULL == p_init_param->gap_cb))
    {
        return BLE_ERR_INVALID_PTR;
    }

    /* check pairing parameter */
    retval = set_pair_param(p_init_param->p_pairing_param);
    if(BLE_SUCCESS != retval)
    {
        return retval;
    }

    retval = R_BLE_GAP_Init(abs_gapcb);
    if(BLE_SUCCESS != retval)
    {
        return retval;
    }

    for(i=0; i<BLE_MAX_NO_OF_ADV_SETS_SUPPORTED; i++)
    {
        gs_adv_sets[i].status = 0;
    }

    gs_abs_scan.status = 0;
    gs_conn_t_hdl = BLE_TIMER_INVALID_HDL;
#if (BLE_CFG_LIB_TYPE != 0)
    gs_adv_t_hdl = BLE_TIMER_INVALID_HDL;
#endif /* (BLE_CFG_LIB_TYPE != 0) */
    gs_set_privacy_status = 0;

    /* set pairing parameter */
    set_pair_param(p_init_param->p_pairing_param);

    set_abs_cb(p_init_param->gap_cb, p_init_param->vs_cb);

    if(NULL != p_init_param->p_gatts_cbs)
    {
        retval = R_BLE_GATTS_Init(p_init_param->p_gatts_cbs->cb_num);
        if (BLE_SUCCESS != retval)
        {
            return retval;
        }

        if(NULL != p_init_param->p_gatts_cbs->p_cb_param)
        {
            for(i=0; i<p_init_param->p_gatts_cbs->cb_num; i++)
            {
                if(NULL != p_init_param->p_gatts_cbs->p_cb_param[i].cb)
                {
                    retval = R_BLE_GATTS_RegisterCb(
                                    p_init_param->p_gatts_cbs->p_cb_param[i].cb, 
                                    p_init_param->p_gatts_cbs->p_cb_param[i].priority
                                );

                    if (BLE_SUCCESS != retval)
                    {
                        return retval;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }

#if (BLE_CFG_LIB_TYPE != 2)
    if(NULL != p_init_param->p_gattc_cbs)
    {
        retval = R_BLE_GATTC_Init(p_init_param->p_gattc_cbs->cb_num);
        if (BLE_SUCCESS != retval)
        {
            return retval;
        }

        if(NULL != p_init_param->p_gattc_cbs->p_cb_param)
        {
            for(i=0; i<p_init_param->p_gattc_cbs->cb_num; i++)
            {
                if(NULL != p_init_param->p_gattc_cbs->p_cb_param[i].cb)
                {
                    retval = R_BLE_GATTC_RegisterCb(
                                    p_init_param->p_gattc_cbs->p_cb_param[i].cb, 
                                    p_init_param->p_gattc_cbs->p_cb_param[i].priority
                                );

                    if (BLE_SUCCESS != retval)
                    {
                        return retval;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
#endif /* (BLE_CFG_LIB_TYPE != 2) */

    retval = R_BLE_VS_Init(abs_vscb);

    return retval;
} /* End of function R_BLE_ABS_Init() */

/***********************************************************************************************************************
 * Function Name: R_BLE_ABS_StartLegacyAdv
 * Description  : Start Legacy Advertising after setting advertising parameters, advertising data and scan response data.
 *              : The legacy advertising uses the advertising set whose advertising handle is 0.
 *              : The advertising type is connectable and scannable(ADV_IND).
 *              : The address type of local device is Public Identity Address or 
 *              : RPA(If the resolving list contains no matching entry, use the public address.).
 *              : Scan request event(BLE_GAP_EVENT_SCAN_REQ_RECV) is not notified.
 * Arguments    : st_ble_abs_legacy_adv_param_t * p_adv_param ; Parameters for Legacy Advertising.
 * Return Value : BLE_SUCCESS(0x0000)                         ; Success 
 *              : BLE_ERR_INVALID_PTR(0x0001)                 ; p_adv_param is specified as NULL.
 *              : BLE_ERR_INVALID_ARG(0x0003)                 ; The advertising parameter is out of range. 
 *              : BLE_ERR_INVALID_STATE(0x0008)               ; Host stack hasn't been initialized. 
 *              : BLE_ERR_MEM_ALLOC_FAILED(0x000C)            ; Insufficient memory is needed for this function.
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_StartLegacyAdv(st_ble_abs_legacy_adv_param_t * p_adv_param)
{
    st_ble_gap_adv_param_t adv_param;
    ble_status_t retval = BLE_SUCCESS;
#if (BLE_CFG_LIB_TYPE == 0)
    int32_t adv_hdl = BLE_ABS_LEGACY_HDL;
#else /* (BLE_CFG_LIB_TYPE == 0) */
    int32_t adv_hdl = BLE_ABS_COMMON_HDL;
#endif /* (BLE_CFG_LIB_TYPE == 0) */

    if(NULL == p_adv_param)
    {
        return BLE_ERR_INVALID_PTR;
    }

    /* status check */
    if(0 != (gs_adv_sets[adv_hdl].status & 
             (BLE_ABS_ADV_STATUS_ADV_FAST_START | BLE_ABS_ADV_STATUS_ADV_SLOW_START)))
    {
        return BLE_ERR_INVALID_STATE;
    }

#if (BLE_CFG_LIB_TYPE == 0)
    set_adv_status(adv_hdl, 0, (BLE_ABS_ADV_STATUS_PARAM_FAST | BLE_ABS_ADV_STATUS_PARAM_SLOW));
#else /* (BLE_CFG_LIB_TYPE == 0) */
    set_adv_status(adv_hdl, 0, (BLE_ABS_ADV_STATUS_PARAM_FAST | BLE_ABS_ADV_STATUS_PARAM_SLOW | 
                                BLE_ABS_ADV_COMM_LEG | BLE_ABS_ADV_COMM_NON));
#endif /* (BLE_CFG_LIB_TYPE == 0) */

    retval = conv_legacy_adv_param(p_adv_param, &adv_param);
    if(BLE_SUCCESS != retval)
    {
        return retval;
    }

    /* check data length */
    if((BLE_ABS_LEGACY_ADV_DATA_LEN < p_adv_param->adv_data_length) || 
       (BLE_ABS_LEGACY_ADV_DATA_LEN < p_adv_param->sres_data_length))
    {
        return BLE_ERR_INVALID_ARG;
    }

    /* check advertising interval */
    set_conn_adv_intv(&adv_param, p_adv_param->fast_adv_intv, p_adv_param->slow_adv_intv, p_adv_param->fast_period);

    /* advertising data update check */
    update_data_status(BLE_ABS_ADV_STATUS_ADV_DATA, 
                       p_adv_param->p_adv_data, 
                       p_adv_param->adv_data_length, adv_hdl);
    /* scan response data update check */
    update_data_status(BLE_ABS_ADV_STATUS_SRES_DATA, 
                    p_adv_param->p_sres_data, 
                    p_adv_param->sres_data_length, adv_hdl);

    set_adv_param(p_adv_param, BLE_ABS_LEGACY_HDL);

    retval = R_BLE_GAP_SetAdvParam(&adv_param);

    if(BLE_SUCCESS == retval)
    {
#if (BLE_CFG_LIB_TYPE == 0)
        uint32_t status = p_adv_param->fast_period ? BLE_ABS_ADV_STATUS_PARAM_FAST : BLE_ABS_ADV_STATUS_PARAM_SLOW;
#else /* (BLE_CFG_LIB_TYPE == 0) */
        uint32_t status = p_adv_param->fast_period ? (BLE_ABS_ADV_STATUS_PARAM_FAST | BLE_ABS_ADV_COMM_LEG) : 
                                                     (BLE_ABS_ADV_STATUS_PARAM_SLOW | BLE_ABS_ADV_COMM_LEG);
#endif /* (BLE_CFG_LIB_TYPE == 0) */
        set_adv_status(adv_hdl, status, 0);
    }

    return retval;
} /* End of function R_BLE_ABS_StartLegacyAdv() */

/***********************************************************************************************************************
 * Function Name: R_BLE_ABS_StartExtAdv
 * Description  : Start Extended Advertising after setting advertising parameters, advertising data.
 *              : The extended advertising uses the advertising set whose advertising handle is 1.
 *              : The advertising type is connectable and non-scannable.
 *              : The address type of local device is Public Identity Address or 
 *              : RPA(If the resolving list contains no matching entry, use the public address.).
 *              : Scan request event(BLE_GAP_EVENT_SCAN_REQ_RECV) is not notified.
 *              : Secondary Advertising Max Skip is 0.
 * Arguments    : st_ble_abs_ext_adv_param_t * p_adv_param    ; Parameters for Extended Advertising.
 * Return Value : BLE_SUCCESS(0x0000)                         ; Success 
 *              : BLE_ERR_INVALID_PTR(0x0001)                 ; p_adv_param is specified as NULL.
 *              : BLE_ERR_INVALID_ARG(0x0003)                 ; The advertising parameter is out of range. 
 *              : BLE_ERR_INVALID_STATE(0x0008)               ; Host stack hasn't been initialized. 
 *              : BLE_ERR_MEM_ALLOC_FAILED(0x000C)            ; Insufficient memory is needed for this function.
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_StartExtAdv(st_ble_abs_ext_adv_param_t * p_adv_param)
{
#if (BLE_CFG_LIB_TYPE == 0)
    st_ble_gap_adv_param_t adv_param;
    ble_status_t retval = BLE_SUCCESS;

    if(NULL == p_adv_param)
    {
        return BLE_ERR_INVALID_PTR;
    }

    /*  status check */
    if(0 != (gs_adv_sets[BLE_ABS_EXT_HDL].status & 
             (BLE_ABS_ADV_STATUS_ADV_FAST_START | BLE_ABS_ADV_STATUS_ADV_SLOW_START)))
    {
        return BLE_ERR_INVALID_STATE;
    }

    set_adv_status(BLE_ABS_EXT_HDL, 0, (BLE_ABS_ADV_STATUS_PARAM_FAST | BLE_ABS_ADV_STATUS_PARAM_SLOW));

    retval = conv_ext_adv_param(p_adv_param, &adv_param);
    if(BLE_SUCCESS != retval)
    {
        return retval;
    }

    /*  check data length */
    if(BLE_ABS_CONN_EXT_ADV_DATA_LEN < p_adv_param->adv_data_length)
    {
        return BLE_ERR_INVALID_ARG;
    }

    /*  check advertising interval */
    set_conn_adv_intv(&adv_param, p_adv_param->fast_adv_intv, p_adv_param->slow_adv_intv, p_adv_param->fast_period);

    /*  data update check */
    update_data_status(BLE_ABS_ADV_STATUS_ADV_DATA, 
                       p_adv_param->p_adv_data, 
                       p_adv_param->adv_data_length, BLE_ABS_EXT_HDL);

    set_adv_param(p_adv_param, BLE_ABS_EXT_HDL);

    retval = R_BLE_GAP_SetAdvParam(&adv_param);

    if(BLE_SUCCESS == retval)
    {
        uint32_t status = p_adv_param->fast_period ? BLE_ABS_ADV_STATUS_PARAM_FAST : BLE_ABS_ADV_STATUS_PARAM_SLOW;
        set_adv_status(BLE_ABS_EXT_HDL, status, 0);
    }

    return retval;
#else /* (BLE_CFG_LIB_TYPE == 0) */
    return BLE_ERR_UNSUPPORTED;
#endif /* (BLE_CFG_LIB_TYPE == 0) */
} /* End of function R_BLE_ABS_StartExtAdv() */

/***********************************************************************************************************************
 * Function Name: R_BLE_ABS_StartNonConnAdv
 * Description  : Start Non-Connectable Advertising after setting advertising parameters, advertising data.
 *              : The non-connectable advertising uses the advertising set whose advertising handle is 2.
 *              : The advertising type is non-connectable and non-scannable.
 *              : The address type of local device is Public Identity Address or 
 *              : RPA(If the resolving list contains no matching entry, use the public address.).
 *              : Scan request event(BLE_GAP_EVENT_SCAN_REQ_RECV) is not notified.
 *              : Secondary Advertising Max Skip is 0.
 * Arguments    : st_ble_abs_non_conn_adv_param_t * p_adv_param ; Parameters for Non-Connectable Advertising.
 * Return Value : BLE_SUCCESS(0x0000)                           ; Success 
 *              : BLE_ERR_INVALID_PTR(0x0001)                   ; p_adv_param is specified as NULL.
 *              : BLE_ERR_INVALID_ARG(0x0003)                   ; The advertising parameter is out of range. 
 *              : BLE_ERR_INVALID_STATE(0x0008)                 ; Host stack hasn't been initialized. 
 *              : BLE_ERR_MEM_ALLOC_FAILED(0x000C)              ; Insufficient memory is needed for this function.
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_StartNonConnAdv(st_ble_abs_non_conn_adv_param_t * p_adv_param)
{
    st_ble_gap_adv_param_t adv_param;
    ble_status_t retval = BLE_SUCCESS;
#if (BLE_CFG_LIB_TYPE == 0)
    int32_t adv_hdl = BLE_ABS_NON_CONN_HDL;
#else /* (BLE_CFG_LIB_TYPE == 0) */
    int32_t adv_hdl = BLE_ABS_COMMON_HDL;
#endif /* (BLE_CFG_LIB_TYPE == 0) */

    if(NULL == p_adv_param)
    {
        return BLE_ERR_INVALID_PTR;
    }

    /*  status check */
    if(0 != (gs_adv_sets[adv_hdl].status & BLE_ABS_ADV_STATUS_ADV_SLOW_START))
    {
        return BLE_ERR_INVALID_STATE;
    }

#if (BLE_CFG_LIB_TYPE == 0)
    set_adv_status(adv_hdl, 0, BLE_ABS_ADV_STATUS_PARAM_SLOW);
#else /* (BLE_CFG_LIB_TYPE == 0) */
    set_adv_status(adv_hdl, 0, (BLE_ABS_ADV_STATUS_PARAM_SLOW | 
                                BLE_ABS_ADV_COMM_LEG | BLE_ABS_ADV_COMM_NON));
#endif /* (BLE_CFG_LIB_TYPE == 0) */

    retval = conv_non_conn_adv_param(p_adv_param, &adv_param, adv_hdl);
    if(BLE_SUCCESS != retval)
    {
        return retval;
    }

    adv_param.adv_intv_min = p_adv_param->adv_intv;
    adv_param.adv_intv_max = p_adv_param->adv_intv;

    /*  data update check */
    update_data_status(BLE_ABS_ADV_STATUS_ADV_DATA, 
                       p_adv_param->p_adv_data, 
                       p_adv_param->adv_data_length, adv_hdl);

    set_adv_param(p_adv_param, BLE_ABS_NON_CONN_HDL);

    retval = R_BLE_GAP_SetAdvParam(&adv_param);
    if(BLE_SUCCESS == retval)
    {
#if (BLE_CFG_LIB_TYPE == 0)
        set_adv_status(adv_hdl, BLE_ABS_ADV_STATUS_PARAM_SLOW, 0);
#else /* (BLE_CFG_LIB_TYPE == 0) */
        set_adv_status(adv_hdl, (BLE_ABS_ADV_STATUS_PARAM_SLOW | BLE_ABS_ADV_COMM_NON), 0);
#endif /* (BLE_CFG_LIB_TYPE == 0) */
    }

    return retval;
} /* End of function R_BLE_ABS_StartNonConnAdv() */

/***********************************************************************************************************************
 * Function Name: R_BLE_ABS_StartPerdAdv
 * Description  : Start Periodic Advertising after setting advertising parameters, periodic advertising parameters, 
 *              : advertising data and periodic advertising data.
 *              : The periodic advertising uses the advertising set whose advertising handle is 3.
 *              : The advertising type is non-connectable and non-scannable.
 *              : The address type of local device is Public Identity Address or 
 *              : RPA(If the resolving list contains no matching entry, use the public address.).
 *              : Scan request event(BLE_GAP_EVENT_SCAN_REQ_RECV) is not notified.
 *              : Secondary Advertising Max Skip is 0.
 * Arguments    : st_ble_abs_perd_adv_param_t * p_adv_param ; Advertising parameters for Periodic Advertising.
 * Return Value : BLE_SUCCESS(0x0000)                       ; Success 
 *              : BLE_ERR_INVALID_PTR(0x0001)               ; p_adv_param is specified as NULL.
 *              : BLE_ERR_INVALID_ARG(0x0003)               ; The advertising parameter is out of range. 
 *              : BLE_ERR_INVALID_STATE(0x0008)             ; Host stack hasn't been initialized. 
 *              : BLE_ERR_MEM_ALLOC_FAILED(0x000C)          ; Insufficient memory is needed for this function.
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_StartPerdAdv(st_ble_abs_perd_adv_param_t * p_adv_param)
{
#if (BLE_CFG_LIB_TYPE == 0)
    st_ble_gap_adv_param_t adv_param;
    ble_status_t retval = BLE_SUCCESS;

    if(NULL == p_adv_param)
    {
        return BLE_ERR_INVALID_PTR;
    }

    /*  status check */
    if(0 != (gs_adv_sets[BLE_ABS_PERD_HDL].status & 
             (BLE_ABS_ADV_STATUS_ADV_SLOW_START | BLE_ABS_ADV_STATUS_PERD_START)))
    {
        return BLE_ERR_INVALID_STATE;
    }

    set_adv_status(BLE_ABS_PERD_HDL, 0, (BLE_ABS_ADV_STATUS_PARAM_SLOW | BLE_ABS_ADV_STATUS_PERD_PARAM));

    retval = conv_non_conn_adv_param(&p_adv_param->param, &adv_param, BLE_ABS_PERD_HDL);
    if(BLE_SUCCESS != retval)
    {
        return retval;
    }

    adv_param.adv_intv_min = p_adv_param->param.adv_intv;
    adv_param.adv_intv_max = p_adv_param->param.adv_intv;

    /*  advertising data update check */
    update_data_status(BLE_ABS_ADV_STATUS_ADV_DATA, 
                       p_adv_param->param.p_adv_data, 
                       p_adv_param->param.adv_data_length, BLE_ABS_PERD_HDL);
    /*  periodic advertising data update check */
    update_data_status(BLE_ABS_ADV_STATUS_PERD_DATA, 
                       p_adv_param->p_perd_adv_data, 
                       p_adv_param->perd_adv_data_length, BLE_ABS_PERD_HDL);

    set_adv_param(p_adv_param, BLE_ABS_PERD_HDL);

    retval = R_BLE_GAP_SetAdvParam(&adv_param);

    if(BLE_SUCCESS == retval)
    {
        set_adv_status(BLE_ABS_PERD_HDL, BLE_ABS_ADV_STATUS_PARAM_SLOW, 0);
    }

    return retval;
#else /* (BLE_CFG_LIB_TYPE == 0) */
    return BLE_ERR_UNSUPPORTED;
#endif /* (BLE_CFG_LIB_TYPE == 0) */
} /* End of function R_BLE_ABS_StartPerdAdv() */

/***********************************************************************************************************************
 * Function Name: R_BLE_ABS_StartScan
 * Description  : Start scaning after setting scan parameters.
 *              : The scanner address type is Public Identity Address.
 *              : Fast scan is followed by slow scan. 
 *              : The end of fast scan or slow scan is notified with BLE_GAP_EVENT_SCAN_TO event.
 *              : If fast_period is 0, only slow scan is carried out.
 *              : If scan_period is 0, slow scan continues.
 * Arguments    : st_ble_abs_scan_param_t * p_scan_param ; Scan parameters.
 * Return Value : BLE_SUCCESS(0x0000)                       ; Success 
 *              : BLE_ERR_INVALID_PTR(0x0001)               ; p_scan_param is specified as NULL. 
 *              : BLE_ERR_INVALID_ARG(0x0003)               ; The scan parameter is out of range.
 *              : BLE_ERR_INVALID_STATE(0x0008)             ; Host stack hasn't been initialized. 
 *              : BLE_ERR_MEM_ALLOC_FAILED(0x000C)          ; Insufficient memory is needed for this function.
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_StartScan(st_ble_abs_scan_param_t * p_scan_param)
{
    st_ble_gap_scan_param_t gap_scan_param;
    st_ble_gap_scan_phy_param_t phy_param_1M;
#if (BLE_CFG_LIB_TYPE == 0)
    st_ble_gap_scan_phy_param_t phy_param_coded;
#endif /* (BLE_CFG_LIB_TYPE == 0) */
    st_ble_gap_scan_on_t gap_scan_enable;
    ble_status_t retval;
#if (BLE_CFG_LIB_TYPE != 0)
    uint32_t to;
    to = 0;
#endif /* (BLE_CFG_LIB_TYPE != 0) */

    if(NULL == p_scan_param)
    {
        return BLE_ERR_INVALID_PTR;
    }

    /*  set scan phy parameters */
    gap_scan_param.p_phy_param_1M = p_scan_param->p_phy_param_1M ? &phy_param_1M : NULL;
#if (BLE_CFG_LIB_TYPE == 0)
    gap_scan_param.p_phy_param_coded = p_scan_param->p_phy_param_coded ? &phy_param_coded : NULL;
#else /* (BLE_CFG_LIB_TYPE == 0) */
    gap_scan_param.p_phy_param_coded = NULL;
#endif /* (BLE_CFG_LIB_TYPE == 0) */

    /*  scan parameter check */
    retval = set_scan_param(p_scan_param);
    if(BLE_SUCCESS != retval)
    {
        return retval;
    }

    conv_scan_param(&gap_scan_param, &gap_scan_enable, BLE_ABS_SCAN_STATUS_FAST_START);

    retval = R_BLE_GAP_StartScan(&gap_scan_param, &gap_scan_enable);
    if(BLE_SUCCESS == retval)
    {
        if(0 == p_scan_param->fast_period)
        {
            set_scan_status(BLE_ABS_SCAN_STATUS_SLOW_START, 0);
#if (BLE_CFG_LIB_TYPE != 0)
            if(0 != p_scan_param->slow_period)
            {
                to = p_scan_param->slow_period;
            }
#endif /* (BLE_CFG_LIB_TYPE != 0) */
        }
        else
        {
            set_scan_status(BLE_ABS_SCAN_STATUS_FAST_START, 0);
#if (BLE_CFG_LIB_TYPE != 0)
            to = p_scan_param->fast_period;
#endif /* (BLE_CFG_LIB_TYPE != 0) */
        }
#if (BLE_CFG_LIB_TYPE != 0)
        R_BLE_TIMER_Create(&gs_scan_t_hdl, (to * 10), BLE_TIMER_ONE_SHOT, ads_scan_to_hdlr);
        R_BLE_TIMER_Start(gs_scan_t_hdl);
#endif /* (BLE_CFG_LIB_TYPE != 0) */

    }

    return retval;
} /* End of function R_BLE_ABS_StartScan() */

/***********************************************************************************************************************
 * Function Name: R_BLE_ABS_SetLocPrivacy
 * Description  : Generate a IRK, add it to the resolving list, set privacy mode and enable RPA function.
 *              : Register vendor specific callback function, if IRK is generated by this function.
 *              : After configuring local device privacy, 
 *              : BLE_GAP_ADDR_RPA_ID_PUBLIC is specified as own device address 
 *              : in theadvertising/scan/create connection API.
 * Arguments    : uint8_t * p_lc_irk                        ; IRK to be registered in the resolving list. 
 *              :                                           ; If p_lc_irk is specified as NULL, the new IRK is generated.
 *              : uint8_t privacy_mode                      ; Privacy mode.
 * Return Value : BLE_SUCCESS(0x0000)                       ; Success 
 *              : BLE_ERR_INVALID_ARG(0x0003)               ; The privacy_mode parameter is out of range.
 *              : BLE_ERR_INVALID_STATE(0x0008)             ; Host stack hasn't been initialized or 
 *                                                          ; configuring the resolving list or privacy mode.
 *              : BLE_ERR_MEM_ALLOC_FAILED(0x000C)          ; Insufficient memory is needed for this function.
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_SetLocPrivacy(uint8_t * p_lc_irk, uint8_t privacy_mode)
{
    ble_status_t retval;
    retval = BLE_SUCCESS;

    if(BLE_GAP_DEV_PRIV_MODE < privacy_mode)
    {
        return BLE_ERR_INVALID_ARG;
    }

    if(NULL == p_lc_irk)
    {
        retval = R_BLE_VS_GetRand(BLE_GAP_IRK_SIZE);
        gs_set_privacy_status = (BLE_SUCCESS == retval) ? BLE_ABS_PV_STATUS_CREATE_IRK : 0 ;
    }
    else
    {
        set_irk_to_rslv(p_lc_irk);
        if(0 == gs_set_privacy_status)
        {
            retval = BLE_ERR_INVALID_OPERATION;
        }
    }

    gs_privacy_mode = privacy_mode;

    return retval;
} /* End of function R_BLE_ABS_SetLocPrivacy() */

/***********************************************************************************************************************
 * Function Name: R_BLE_ABS_CreateConn
 * Description  : Request create connection.
 *              : The initiator address type is Public Identity Address.
 *              : The scan interval is 60ms and the scan window is 30ms in case of 1M PHY or 2M PHY.
 *              : The scan interval is 180ms and the scan window is 90ms in case of coded PHY.
 *              : The Minimum CE Length and the Maximum CE Length are 0xFFFF.
 *              : When the request for a connection has been received by the Controller, 
 *              : BLE_GAP_EVENT_CREATE_CONN_COMP event is notified.
 *              : When a link has been established, BLE_GAP_EVENT_CONN_IND event is notified.
 * Arguments    : st_ble_abs_conn_param_t * p_conn_param    ; Create connection parameters.
 * Return Value : BLE_SUCCESS(0x0000)                       ; Success 
 *              : BLE_ERR_INVALID_PTR(0x0001)               ; p_conn_param is specified as NULL. 
 *              : BLE_ERR_INVALID_ARG(0x0003)               ; The create connection parameter is out of range.
 *              : BLE_ERR_INVALID_STATE(0x0008)             ; Host stack hasn't been initialized. 
 *              : BLE_ERR_MEM_ALLOC_FAILED(0x000C)          ; Insufficient memory is needed for this function.
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_CreateConn(st_ble_abs_conn_param_t * p_conn_param)
{
    if(NULL == p_conn_param)
    {
        return BLE_ERR_INVALID_PTR;
    }

    if(10 < p_conn_param->conn_to)
    {
        return BLE_ERR_INVALID_ARG;
    }

    st_ble_gap_create_conn_param_t conn_param;
    conn_param.init_filter_policy = p_conn_param->filter;
    conn_param.own_addr_type = BLE_GAP_ADDR_PUBLIC;

    if(BLE_GAP_INIT_FILT_USE_ADDR == p_conn_param->filter)
    {
        memcpy(conn_param.remote_bd_addr, p_conn_param->p_addr->addr, BLE_BD_ADDR_LEN);
        conn_param.remote_bd_addr_type = p_conn_param->p_addr->type;
    }
    else
    {
        conn_param.remote_bd_addr_type = BLE_GAP_ADDR_PUBLIC;
    }

    /*  set connection parameters for 1M */
    st_ble_gap_conn_param_t conn_param_1M;
    st_ble_gap_conn_phy_param_t conn_phy_1M;
    set_conn_param(p_conn_param->p_conn_1M, &conn_phy_1M, &conn_param_1M);
    conn_param.p_conn_param_1M = p_conn_param->p_conn_1M ? &conn_phy_1M : NULL;

    /*  set connection parameters for 2M */
    st_ble_gap_conn_param_t conn_param_2M;
    st_ble_gap_conn_phy_param_t conn_phy_2M;
    set_conn_param(p_conn_param->p_conn_2M, &conn_phy_2M, &conn_param_2M);
    conn_param.p_conn_param_2M = p_conn_param->p_conn_2M ? &conn_phy_2M : NULL;

    /*  set connection parameters for coded */
    st_ble_gap_conn_param_t conn_param_coded;
    st_ble_gap_conn_phy_param_t conn_phy_coded;
    set_conn_param(p_conn_param->p_conn_coded, &conn_phy_coded, &conn_param_coded);
    conn_param.p_conn_param_coded = p_conn_param->p_conn_coded ? &conn_phy_coded : NULL;
    if(NULL != p_conn_param->p_conn_coded)
    {
        conn_phy_coded.scan_intv = BLE_ABS_CONN_SC_INTV_SLOW;
        conn_phy_coded.scan_window = BLE_ABS_CONN_SC_WINDOW_SLOW;
    }

    /*  create timer for cancel */
    if(0 != p_conn_param->conn_to)
    {
        R_BLE_TIMER_Create(&gs_conn_t_hdl, (uint32_t)(p_conn_param->conn_to * 1000), BLE_TIMER_ONE_SHOT, cancel_conn_func);
    }

    /*  create connection */
    ble_status_t retval;
    retval = R_BLE_GAP_CreateConn(&conn_param);
    if(0 != p_conn_param->conn_to)
    {
        if(BLE_SUCCESS == retval)
        {
            R_BLE_TIMER_Start(gs_conn_t_hdl);
        }
        else
        {
            R_BLE_TIMER_Delete(&gs_conn_t_hdl);
        }
    }

    return retval;
} /* End of function R_BLE_ABS_CreateConn() */

/***********************************************************************************************************************
 * Function Name: R_BLE_ABS_StartAuth
 * Description  : Start pairing or encryption. If pairing has been done, start encryption.
 *              : The pairing parameters are configured by R_BLE_ABS_Init() or R_BLE_GAP_SetPairingParams().
 *              : If the pairing parameters are configure by R_BLE_ABS_Init(), 
 *              :    - bonding policy is that bonding information is stored.
*               :    - Key press notification is not supported.
 * Arguments    : uint16_t conn_hdl                         ; Connection handle identifying the remote device 
 *                                                          ; which local device starts pairing or encryption with.
 * Return Value : BLE_SUCCESS(0x0000)                       ; Success 
 *              : BLE_ERR_INVALID_STATE(0x0008)             ; Host stack hasn't been initialized or 
 *                                                          ; while generating OOB data, this function was called.
 *              : BLE_ERR_MEM_ALLOC_FAILED(0x000C)          ; Insufficient memory is needed for this function.
 *              : BLE_ERR_INVALID_HDL(0x000E)               ; The remote device specified by conn_hdl is not found. 
 **********************************************************************************************************************/
ble_status_t R_BLE_ABS_StartAuth(uint16_t conn_hdl)
{
    st_ble_gap_auth_info_t sec_info;
    ble_status_t retval;

    /*  check security information */
    retval = R_BLE_GAP_GetDevSecInfo(conn_hdl, &sec_info);
    if(BLE_SUCCESS == retval)
    {
        retval = R_BLE_GAP_StartEnc(conn_hdl);
    }
    else
    {
        retval = R_BLE_GAP_StartPairing(conn_hdl);
    }

    return retval;
} /* End of function R_BLE_ABS_StartAuth() */

/************************************************
 *   static function definitions                *
 ***********************************************/
/***********************************************************************************************************************
 * Function Name: set_conn_param
 * Description  : Set Abstraction API connection parameters to GAP connection parameters.
 * Arguments    : st_ble_abs_conn_phy_param_t * p_abs_conn_param    ; Abstraction API connection parameters
 *              : st_ble_gap_conn_phy_param_t * p_conn_phy_param    ; GAP phy connection parameters
 *              : st_ble_gap_conn_param_t * p_conn_param            ; GAP connection parameters
 * Return Value : none
 **********************************************************************************************************************/
static void set_conn_param(st_ble_abs_conn_phy_param_t * p_abs_conn_param, 
                    st_ble_gap_conn_phy_param_t * p_conn_phy_param,
                    st_ble_gap_conn_param_t * p_conn_param)
{
    if(NULL != p_abs_conn_param)
    {
        p_conn_param->conn_intv_min = p_abs_conn_param->conn_intv;
        p_conn_param->conn_intv_max = p_abs_conn_param->conn_intv;
        p_conn_param->conn_latency = p_abs_conn_param->conn_latency;
        p_conn_param->sup_to = p_abs_conn_param->sup_to;
        p_conn_param->min_ce_length = 0xFFFF;
        p_conn_param->max_ce_length = 0xFFFF;
        p_conn_phy_param->scan_intv = BLE_ABS_CONN_SC_INTV_FAST;
        p_conn_phy_param->scan_window = BLE_ABS_CONN_SC_WINDOW_FAST;
        p_conn_phy_param->p_conn_param = p_conn_param;
    }
} /* End of function set_conn_param() */

/***********************************************************************************************************************
 * Function Name: set_pair_param
 * Description  : Set pairing parameters.
 * Arguments    : st_ble_abs_pairing_param_t * p_pairing_param      ; Pairing parameters
 * Return Value : BLE_SUCCESS(0x0000)                         ; Success 
 *                BLE_ERR_INVALID_ARG(0x0003)                 ; The pairing parameter is out of range. 
 **********************************************************************************************************************/
static ble_status_t set_pair_param(st_ble_abs_pairing_param_t * p_pairing_param)
{
    if(NULL != p_pairing_param)
    {
        st_ble_gap_pairing_param_t pair_param;

        pair_param.iocap = p_pairing_param->iocap;
        pair_param.mitm = p_pairing_param->mitm;
        pair_param.bonding = BLE_GAP_BONDING;
        pair_param.max_key_size = 0x10;
        pair_param.min_key_size = p_pairing_param->max_key_size;
        pair_param.loc_key_dist = p_pairing_param->loc_key_dist;
        pair_param.rem_key_dist = p_pairing_param->rem_key_dist;
        pair_param.key_notf = BLE_GAP_SC_KEY_PRESS_NTF_NOT_SPRT;
        pair_param.sec_conn_only = p_pairing_param->sec_conn_only;

        return R_BLE_GAP_SetPairingParams(&pair_param);
    }

    return BLE_SUCCESS;
} /* End of function set_pair_param() */

#if (BLE_CFG_LIB_TYPE != 0)
/***********************************************************************************************************************
 * Function Name: adv_to_func
 * Description  : Advertising timer handler for legacy advertising.
 * Arguments    : uint32_t hdl                                       ; Timer handle
 * Return Value : none
 **********************************************************************************************************************/
static void adv_to_func(uint32_t hdl)
{
    R_BLE_GAP_StopAdv(BLE_ABS_COMMON_HDL);
    set_adv_status(BLE_ABS_COMMON_HDL, BLE_ABS_ADV_COMM_TO, 0);

} /* End of function adv_to_func() */
#endif /* (BLE_CFG_LIB_TYPE != 0) */

/***********************************************************************************************************************
 * Function Name: cancel_conn_func
 * Description  : Cancel a request for connection.
 * Arguments    : uint32_t hdl                                       ; Timer handle
 * Return Value : none
 **********************************************************************************************************************/
static void cancel_conn_func(uint32_t hdl)
{
    R_BLE_GAP_CancelCreateConn();
} /* End of function cancel_conn_func() */

/***********************************************************************************************************************
 * Function Name: set_legacy_sres_data
 * Description  : Configure scan response data and start legacy advertising.
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
static void set_legacy_sres_data(void)
{
    if((gs_adv_sets[BLE_ABS_LEGACY_HDL].param.legacy.sres_data_length) && 
       (gs_adv_sets[BLE_ABS_LEGACY_HDL].param.legacy.p_sres_data))
    {
        if(gs_adv_sets[BLE_ABS_LEGACY_HDL].status & BLE_ABS_ADV_STATUS_SRES_DATA)
        {
            adv_start(BLE_ABS_LEGACY_HDL);
        }
        else
        {
            adv_set_data(BLE_ABS_LEGACY_HDL, BLE_GAP_SCAN_RSP_DATA_MODE);
        }
    }
    else
    {
        adv_start(BLE_ABS_LEGACY_HDL);
    }
} /* End of function set_legacy_sres_data() */

/***********************************************************************************************************************
 * Function Name: abs_adv_param_set_hdlr
 * Description  : Handler for GAP BLE_GAP_EVENT_ADV_PARAM_SET_COMP event.
 * Arguments    : st_ble_evt_data_t * p_event_data         ; BLE_GAP_EVENT_ADV_PARAM_SET_COMP event parameters 
 * Return Value : none
 **********************************************************************************************************************/
void abs_adv_param_set_hdlr(st_ble_evt_data_t * p_event_data)
{

#if (BLE_CFG_LIB_TYPE == 0)
    st_ble_gap_adv_set_evt_t * p_adv_set_param;
    p_adv_set_param = (st_ble_gap_adv_set_evt_t *)p_event_data->p_param;

    switch(p_adv_set_param->adv_hdl)
    {
        case BLE_ABS_LEGACY_HDL:
            if((gs_adv_sets[BLE_ABS_LEGACY_HDL].status & 
               (BLE_ABS_ADV_STATUS_PARAM_FAST | BLE_ABS_ADV_STATUS_PARAM_SLOW)))
            {
                if((gs_adv_sets[BLE_ABS_LEGACY_HDL].param.legacy.adv_data_length) && 
                   (gs_adv_sets[BLE_ABS_LEGACY_HDL].param.legacy.p_adv_data))
                {
                    if(gs_adv_sets[BLE_ABS_LEGACY_HDL].status & BLE_ABS_ADV_STATUS_ADV_DATA)
                    {
                        set_legacy_sres_data();
                    }
                    else
                    {
                        adv_set_data(BLE_ABS_LEGACY_HDL, BLE_GAP_ADV_DATA_MODE);
                    }
                }
                else
                {
                    set_legacy_sres_data();
                }
            }
        break;

        case BLE_ABS_EXT_HDL:
            if((gs_adv_sets[BLE_ABS_EXT_HDL].status & 
               (BLE_ABS_ADV_STATUS_PARAM_FAST | BLE_ABS_ADV_STATUS_PARAM_SLOW)))
            {
                if((gs_adv_sets[BLE_ABS_EXT_HDL].param.ext.adv_data_length) && 
                   (gs_adv_sets[BLE_ABS_EXT_HDL].param.ext.p_adv_data))
                {
                    if(gs_adv_sets[BLE_ABS_EXT_HDL].status & BLE_ABS_ADV_STATUS_ADV_DATA)
                    {
                        adv_start(BLE_ABS_EXT_HDL);
                    }
                    else
                    {
                        adv_set_data(BLE_ABS_EXT_HDL, BLE_GAP_ADV_DATA_MODE);
                    }
                }
                else
                {
                    adv_start(BLE_ABS_EXT_HDL);
                }
            }
        break;

        case BLE_ABS_NON_CONN_HDL:
            if(gs_adv_sets[BLE_ABS_NON_CONN_HDL].status & BLE_ABS_ADV_STATUS_PARAM_SLOW)
            {
                if((gs_adv_sets[BLE_ABS_NON_CONN_HDL].param.non_conn.adv_data_length) && 
                   (gs_adv_sets[BLE_ABS_NON_CONN_HDL].param.non_conn.p_adv_data))
                {
                    adv_set_data(BLE_ABS_NON_CONN_HDL, BLE_GAP_ADV_DATA_MODE);
                }
                else
                {
                    adv_start(BLE_ABS_NON_CONN_HDL);
                }
            }
        break;

        default /* BLE_ABS_PERD_HDL */:
            if(gs_adv_sets[BLE_ABS_PERD_HDL].status & BLE_ABS_ADV_STATUS_PARAM_SLOW)
            {
                st_ble_gap_perd_adv_param_t perd_param;
                perd_param.adv_hdl = BLE_ABS_PERD_HDL;
                perd_param.prop_type = 0x0000;
                perd_param.perd_intv_min = gs_adv_sets[BLE_ABS_PERD_HDL].param.perd.perd_intv;
                perd_param.perd_intv_max = perd_param.perd_intv_min;
                R_BLE_GAP_SetPerdAdvParam(&perd_param);
                set_adv_status(BLE_ABS_PERD_HDL, BLE_ABS_ADV_STATUS_PERD_PARAM, 0);
            }
        break;

    }
#else /* (BLE_CFG_LIB_TYPE == 0) */
    if(gs_adv_sets[BLE_ABS_COMMON_HDL].status & BLE_ABS_ADV_COMM_LEG)
    {
        if((gs_adv_sets[BLE_ABS_COMMON_HDL].status & 
            (BLE_ABS_ADV_STATUS_PARAM_FAST | BLE_ABS_ADV_STATUS_PARAM_SLOW)))
        {
            if((gs_adv_sets[BLE_ABS_COMMON_HDL].param.legacy.adv_data_length) && 
                (gs_adv_sets[BLE_ABS_COMMON_HDL].param.legacy.p_adv_data))
            {
                if(gs_adv_sets[BLE_ABS_COMMON_HDL].status & BLE_ABS_ADV_STATUS_ADV_DATA)
                {
                    set_legacy_sres_data();
                }
                else
                {
                    adv_set_data(BLE_ABS_COMMON_HDL, BLE_GAP_ADV_DATA_MODE);
                }
            }
            else
            {
                set_legacy_sres_data();
            }
        }
    }
    else
    {
        if(gs_adv_sets[BLE_ABS_COMMON_HDL].status & BLE_ABS_ADV_STATUS_PARAM_SLOW)
        {
            if((gs_adv_sets[BLE_ABS_COMMON_HDL].param.non_conn.adv_data_length) && 
                (gs_adv_sets[BLE_ABS_COMMON_HDL].param.non_conn.p_adv_data))
            {
                adv_set_data(BLE_ABS_COMMON_HDL, BLE_GAP_ADV_DATA_MODE);
            }
            else
            {
                adv_start(BLE_ABS_NON_CONN_HDL);
            }
        }
    }
#endif /* (BLE_CFG_LIB_TYPE == 0) */

    return;
} /* End of function abs_adv_param_set_hdlr() */

/***********************************************************************************************************************
 * Function Name: adv_start
 * Description  : Start advertising.
 * Arguments    : uint8_t adv_hdl           ; Advertising handle
 * Return Value : none
 **********************************************************************************************************************/
static void adv_start(uint8_t adv_hdl)
{
    ble_status_t retval;
    uint32_t status;

#if (BLE_CFG_LIB_TYPE == 0)
    uint16_t fast_period = (uint16_t)((BLE_ABS_LEGACY_HDL == adv_hdl) 
                                        ? gs_adv_sets[BLE_ABS_LEGACY_HDL].param.legacy.fast_period 
                                        : (BLE_ABS_EXT_HDL == adv_hdl) 
                                            ? gs_adv_sets[BLE_ABS_EXT_HDL].param.ext.fast_period 
                                            : 0x0000);
    uint16_t slow_period = (uint16_t)((BLE_ABS_LEGACY_HDL == adv_hdl) 
                                     ? gs_adv_sets[BLE_ABS_LEGACY_HDL].param.legacy.slow_period 
                                     : (BLE_ABS_EXT_HDL == adv_hdl)
                                            ? gs_adv_sets[BLE_ABS_EXT_HDL].param.ext.slow_period
                                            : (BLE_ABS_NON_CONN_HDL == adv_hdl)
                                                ? gs_adv_sets[BLE_ABS_NON_CONN_HDL].param.non_conn.duration
                                                : gs_adv_sets[BLE_ABS_PERD_HDL].param.perd.param.duration);
#else /* (BLE_CFG_LIB_TYPE == 0) */
    uint16_t fast_period = gs_adv_sets[BLE_ABS_COMMON_HDL].param.legacy.fast_period;
    uint16_t slow_period = (uint16_t)((0 != (gs_adv_sets[BLE_ABS_COMMON_HDL].status & BLE_ABS_ADV_COMM_LEG)) ?
                           gs_adv_sets[BLE_ABS_COMMON_HDL].param.legacy.slow_period :
                           gs_adv_sets[BLE_ABS_COMMON_HDL].param.non_conn.duration);
    uint32_t to = slow_period;
#endif /* (BLE_CFG_LIB_TYPE == 0) */

    if(0x0000 == fast_period)
    {
#if (BLE_CFG_LIB_TYPE == 0)
        if((BLE_ABS_PERD_HDL == adv_hdl) && 
           (!(gs_adv_sets[BLE_ABS_PERD_HDL].status & BLE_ABS_ADV_STATUS_PERD_START)))
        {
            retval = R_BLE_GAP_StartPerdAdv(BLE_ABS_PERD_HDL);
            status = BLE_ABS_ADV_STATUS_PERD_START;
        }
        else
#endif /* (BLE_CFG_LIB_TYPE == 0) */
        {
            retval = R_BLE_GAP_StartAdv(adv_hdl, slow_period, 0x0000);
            status = BLE_ABS_ADV_STATUS_ADV_SLOW_START;
        }
    }
    else
    {
        if(gs_adv_sets[adv_hdl].status & BLE_ABS_ADV_STATUS_PARAM_SLOW)
        {
            retval = R_BLE_GAP_StartAdv(adv_hdl, slow_period, 0x0000);
            status = BLE_ABS_ADV_STATUS_ADV_SLOW_START;
        }
        else
        {
#if (BLE_CFG_LIB_TYPE != 0)
            to = fast_period;
#endif /* (BLE_CFG_LIB_TYPE != 0) */
            retval = R_BLE_GAP_StartAdv(adv_hdl, fast_period, 0x0000);
            status = BLE_ABS_ADV_STATUS_ADV_FAST_START;
        }
    }

    if(BLE_SUCCESS == retval)
    {
#if (BLE_CFG_LIB_TYPE != 0)
        if(0 != to)
        {
            R_BLE_TIMER_Create(&gs_adv_t_hdl, (to * 10), BLE_TIMER_ONE_SHOT, adv_to_func);
            R_BLE_TIMER_Start(gs_adv_t_hdl);
        }
#endif /* (BLE_CFG_LIB_TYPE != 0) */
        set_adv_status(adv_hdl, status, 0);
    }

    return;
} /* End of function adv_start() */

/***********************************************************************************************************************
 * Function Name: adv_set_data
 * Description  : Configure advertising data or scan response data or periodic advertising data.
 * Arguments    : uint8_t adv_hdl           ; Advertising handle
 *              : uint8_t data_type         ; Data type
 * Return Value : none
 **********************************************************************************************************************/
static void adv_set_data(uint8_t adv_hdl, uint8_t data_type)
{
    st_ble_gap_adv_data_t adv_data;
    ble_status_t retval;
    uint32_t status;

    adv_data.adv_hdl = adv_hdl;
    adv_data.zero_length_flag = BLE_GAP_DATA_0_CLEAR;
#if (BLE_CFG_LIB_TYPE == 0)
    switch(adv_hdl)
    {
        case BLE_ABS_LEGACY_HDL :
            status = (BLE_GAP_ADV_DATA_MODE == data_type) 
                     ? BLE_ABS_ADV_STATUS_ADV_DATA 
                     : BLE_ABS_ADV_STATUS_SRES_DATA;
            adv_data.data_type = (uint8_t)((BLE_GAP_ADV_DATA_MODE == data_type) 
                                 ? BLE_GAP_ADV_DATA_MODE 
                                 : BLE_GAP_SCAN_RSP_DATA_MODE);
            adv_data.data_length = (uint16_t)((BLE_GAP_ADV_DATA_MODE == data_type) 
                                   ? gs_adv_sets[BLE_ABS_LEGACY_HDL].param.legacy.adv_data_length 
                                   : gs_adv_sets[BLE_ABS_LEGACY_HDL].param.legacy.sres_data_length);
            adv_data.p_data = (BLE_GAP_ADV_DATA_MODE == data_type) 
                              ? gs_adv_sets[BLE_ABS_LEGACY_HDL].param.legacy.p_adv_data 
                              : gs_adv_sets[BLE_ABS_LEGACY_HDL].param.legacy.p_sres_data;
        break;

        case BLE_ABS_EXT_HDL :
        case BLE_ABS_NON_CONN_HDL :
            status = BLE_ABS_ADV_STATUS_ADV_DATA;
            adv_data.data_type = BLE_GAP_ADV_DATA_MODE;
            adv_data.data_length = (uint16_t)((BLE_ABS_EXT_HDL == adv_hdl) 
                                               ? gs_adv_sets[BLE_ABS_EXT_HDL].param.ext.adv_data_length
                                               : gs_adv_sets[BLE_ABS_NON_CONN_HDL].param.non_conn.adv_data_length);
            adv_data.p_data = (BLE_ABS_EXT_HDL == adv_hdl) 
                              ? gs_adv_sets[BLE_ABS_EXT_HDL].param.ext.p_adv_data
                              : gs_adv_sets[BLE_ABS_NON_CONN_HDL].param.non_conn.p_adv_data;
        break;

        default /* BLE_ABS_PERD_HDL */:
            status = (BLE_GAP_ADV_DATA_MODE == data_type) 
                     ? BLE_ABS_ADV_STATUS_ADV_DATA 
                     : BLE_ABS_ADV_STATUS_PERD_DATA;
            adv_data.data_type = (uint8_t)((BLE_GAP_ADV_DATA_MODE == data_type) 
                                            ? BLE_GAP_ADV_DATA_MODE 
                                            : BLE_GAP_PERD_ADV_DATA_MODE);
            adv_data.data_length = (uint16_t)((BLE_GAP_ADV_DATA_MODE == data_type) 
                                               ? gs_adv_sets[BLE_ABS_PERD_HDL].param.perd.param.adv_data_length
                                               : gs_adv_sets[BLE_ABS_PERD_HDL].param.perd.perd_adv_data_length);
            adv_data.p_data = (BLE_GAP_ADV_DATA_MODE == data_type) 
                              ? gs_adv_sets[BLE_ABS_PERD_HDL].param.perd.param.p_adv_data 
                              : gs_adv_sets[BLE_ABS_PERD_HDL].param.perd.p_perd_adv_data;
        break;

    }
#else /* (BLE_CFG_LIB_TYPE == 0) */
    if(gs_adv_sets[BLE_ABS_COMMON_HDL].status & BLE_ABS_ADV_COMM_LEG)
    {
        status = (BLE_GAP_ADV_DATA_MODE == data_type) 
                    ? BLE_ABS_ADV_STATUS_ADV_DATA 
                    : BLE_ABS_ADV_STATUS_SRES_DATA;
        adv_data.data_type = (uint8_t)((BLE_GAP_ADV_DATA_MODE == data_type) 
                                ? BLE_GAP_ADV_DATA_MODE 
                                : BLE_GAP_SCAN_RSP_DATA_MODE);
        adv_data.data_length = (uint16_t)((BLE_GAP_ADV_DATA_MODE == data_type) 
                                ? gs_adv_sets[BLE_ABS_COMMON_HDL].param.legacy.adv_data_length 
                                : gs_adv_sets[BLE_ABS_COMMON_HDL].param.legacy.sres_data_length);
        adv_data.p_data = (BLE_GAP_ADV_DATA_MODE == data_type) 
                            ? gs_adv_sets[BLE_ABS_COMMON_HDL].param.legacy.p_adv_data 
                            : gs_adv_sets[BLE_ABS_COMMON_HDL].param.legacy.p_sres_data;
    }
    else
    {
        status = BLE_ABS_ADV_STATUS_ADV_DATA;
        adv_data.data_type = BLE_GAP_ADV_DATA_MODE;
        adv_data.data_length = gs_adv_sets[BLE_ABS_COMMON_HDL].param.non_conn.adv_data_length;
        adv_data.p_data = gs_adv_sets[BLE_ABS_COMMON_HDL].param.non_conn.p_adv_data;
    }
#endif /* (BLE_CFG_LIB_TYPE == 0) */

    retval = R_BLE_GAP_SetAdvSresData(&adv_data);
    if(BLE_SUCCESS == retval)
    {
        set_adv_status(adv_hdl, status, 0);
    }

    return;
} /* End of function adv_set_data() */

/***********************************************************************************************************************
 * Function Name: abs_perd_param_hdlr
 * Description  : Handler for GAP BLE_GAP_EVENT_PERD_ADV_PARAM_SET_COMP event.
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
static void abs_perd_param_hdlr(void)
{
#if (BLE_CFG_LIB_TYPE == 0)
    if(gs_adv_sets[BLE_ABS_PERD_HDL].status & BLE_ABS_ADV_STATUS_PERD_PARAM)
    {
        if((gs_adv_sets[BLE_ABS_PERD_HDL].param.perd.param.adv_data_length) && 
            (gs_adv_sets[BLE_ABS_PERD_HDL].param.perd.param.p_adv_data))
        {
            adv_set_data(BLE_ABS_PERD_HDL, BLE_GAP_ADV_DATA_MODE);
        }
        else
        {
            if((gs_adv_sets[BLE_ABS_PERD_HDL].param.perd.perd_adv_data_length) && 
               (gs_adv_sets[BLE_ABS_PERD_HDL].param.perd.p_perd_adv_data))
            {
                adv_set_data(BLE_ABS_PERD_HDL, BLE_GAP_PERD_ADV_DATA_MODE);
            }
            else
            {
                adv_start(BLE_ABS_PERD_HDL);
            }
        }
    }

#endif /* (BLE_CFG_LIB_TYPE == 0) */
    return;
} /* End of function abs_perd_param_hdlr() */

/***********************************************************************************************************************
 * Function Name: abs_adv_data_set_hdlr
 * Description  : Handler for GAP BLE_GAP_EVENT_ADV_DATA_UPD_COMP event.
 * Arguments    : st_ble_evt_data_t * p_event_data         ; BLE_GAP_EVENT_ADV_DATA_UPD_COMP event parameters
 * Return Value : none
 **********************************************************************************************************************/
static void abs_adv_data_set_hdlr(st_ble_evt_data_t * p_event_data)
{
    st_ble_gap_adv_data_evt_t * p_adv_data_set_param;

    p_adv_data_set_param = (st_ble_gap_adv_data_evt_t *)p_event_data->p_param;

#if (BLE_CFG_LIB_TYPE == 0)
    switch(p_adv_data_set_param->adv_hdl)
    {
        case BLE_ABS_LEGACY_HDL:
            if(BLE_GAP_ADV_DATA_MODE == p_adv_data_set_param->data_type)
            {
                set_legacy_sres_data();
            }
            else
            {
                adv_start(BLE_ABS_LEGACY_HDL);
            }
        break;

        case BLE_ABS_EXT_HDL:
        case BLE_ABS_NON_CONN_HDL:
            adv_start(p_adv_data_set_param->adv_hdl);
        break;

        default : /* BLE_ABS_PERD_HDL */
            if(BLE_GAP_ADV_DATA_MODE == p_adv_data_set_param->data_type)
            {
                adv_set_data(BLE_ABS_PERD_HDL, BLE_GAP_PERD_ADV_DATA_MODE);
            }
            else
            {
                adv_start(BLE_ABS_PERD_HDL);
            }
        break;

    }
#else /* (BLE_CFG_LIB_TYPE == 0) */
    if(gs_adv_sets[BLE_ABS_COMMON_HDL].status & BLE_ABS_ADV_COMM_LEG)
    {
        if(BLE_GAP_ADV_DATA_MODE == p_adv_data_set_param->data_type)
        {
            set_legacy_sres_data();
        }
        else
        {
            adv_start(BLE_ABS_COMMON_HDL);
        }
    }
    else
    {
        adv_start(BLE_ABS_COMMON_HDL);
    }
#endif /* (BLE_CFG_LIB_TYPE == 0) */

    return;
} /* End of function abs_adv_data_set_hdlr() */

/***********************************************************************************************************************
 * Function Name: abs_adv_off_hdlr
 * Description  : Handler for GAP BLE_GAP_EVENT_ADV_OFF event.
 * Arguments    : st_ble_evt_data_t * p_event_data         ; BLE_GAP_EVENT_ADV_OFF event parameters
 * Return Value : none
 **********************************************************************************************************************/
static void abs_adv_off_hdlr(st_ble_evt_data_t * p_event_data)
{
    st_ble_gap_adv_off_evt_t * p_adv_off_param;

    p_adv_off_param = (st_ble_gap_adv_off_evt_t *)p_event_data->p_param;

#if (BLE_CFG_LIB_TYPE == 0)
    switch(p_adv_off_param->adv_hdl)
    {
        case BLE_ABS_LEGACY_HDL:
        case BLE_ABS_EXT_HDL:
            if(0x02 == p_adv_off_param->reason)
            {
                if((gs_adv_sets[p_adv_off_param->adv_hdl].status & BLE_ABS_ADV_STATUS_ADV_FAST_START))
                {
                    st_ble_gap_adv_param_t adv_param;
                    /*  fast -> slow */
                    set_adv_status(p_adv_off_param->adv_hdl, 0, BLE_ABS_ADV_STATUS_ADV_FAST_START);
                    
                    if(BLE_ABS_LEGACY_HDL == p_adv_off_param->adv_hdl)
                    {
                        conv_legacy_adv_param(&gs_adv_sets[p_adv_off_param->adv_hdl].param.legacy, &adv_param);
                        adv_param.adv_intv_min = gs_adv_sets[p_adv_off_param->adv_hdl].param.legacy.slow_adv_intv;
                        adv_param.adv_intv_max = gs_adv_sets[p_adv_off_param->adv_hdl].param.legacy.slow_adv_intv;
                    }
                    else
                    {
                        conv_ext_adv_param(&gs_adv_sets[p_adv_off_param->adv_hdl].param.ext, &adv_param);
                        adv_param.adv_intv_min = gs_adv_sets[p_adv_off_param->adv_hdl].param.ext.slow_adv_intv;
                        adv_param.adv_intv_max = gs_adv_sets[p_adv_off_param->adv_hdl].param.ext.slow_adv_intv;
                    }

                    R_BLE_GAP_SetAdvParam(&adv_param);
                    set_adv_status(p_adv_off_param->adv_hdl, BLE_ABS_ADV_STATUS_PARAM_SLOW, 0);
                }
                else
                {
                    if((gs_adv_sets[p_adv_off_param->adv_hdl].status & BLE_ABS_ADV_STATUS_ADV_SLOW_START))
                    {
                        /*  slow -> off */
                        set_adv_status(p_adv_off_param->adv_hdl, 0, BLE_ABS_ADV_STATUS_ADV_SLOW_START);
                    }
                }
            }
            else
            {
                set_adv_status(p_adv_off_param->adv_hdl, 
                               0, 
                               (BLE_ABS_ADV_STATUS_ADV_SLOW_START | BLE_ABS_ADV_STATUS_ADV_FAST_START));
            }
        break;

        case BLE_ABS_NON_CONN_HDL:
            /*  slow -> off */
            set_adv_status(BLE_ABS_NON_CONN_HDL, 0, BLE_ABS_ADV_STATUS_ADV_SLOW_START);
        break;

        default :/* BLE_ABS_PERD_HDL */ 
            set_adv_status(BLE_ABS_PERD_HDL, 0, BLE_ABS_ADV_STATUS_ADV_SLOW_START);
            if(0 != (gs_adv_sets[BLE_ABS_PERD_HDL].status & BLE_ABS_ADV_STATUS_PERD_START))
            {
                R_BLE_GAP_StopPerdAdv(BLE_ABS_PERD_HDL);
            }
        break;

    }
#else /* (BLE_CFG_LIB_TYPE == 0) */
    if(gs_adv_sets[BLE_ABS_COMMON_HDL].status & BLE_ABS_ADV_COMM_TO)
    {
        p_adv_off_param->reason = 0x02;
        set_adv_status(BLE_ABS_COMMON_HDL, 0, BLE_ABS_ADV_COMM_TO);
    }

    if(gs_adv_sets[BLE_ABS_COMMON_HDL].status & BLE_ABS_ADV_COMM_LEG)
    {
        if(0x02 == p_adv_off_param->reason)
        {
            if((gs_adv_sets[BLE_ABS_COMMON_HDL].status & BLE_ABS_ADV_STATUS_ADV_FAST_START))
            {
                st_ble_gap_adv_param_t adv_param;
                /*  fast -> slow */
                set_adv_status(BLE_ABS_COMMON_HDL, 0, BLE_ABS_ADV_STATUS_ADV_FAST_START);
                
                conv_legacy_adv_param(&gs_adv_sets[BLE_ABS_COMMON_HDL].param.legacy, &adv_param);
                adv_param.adv_intv_min = gs_adv_sets[BLE_ABS_COMMON_HDL].param.legacy.slow_adv_intv;
                adv_param.adv_intv_max = gs_adv_sets[BLE_ABS_COMMON_HDL].param.legacy.slow_adv_intv;

                R_BLE_GAP_SetAdvParam(&adv_param);
                set_adv_status(BLE_ABS_COMMON_HDL, BLE_ABS_ADV_STATUS_PARAM_SLOW, 0);
            }
            else
            {
                if((gs_adv_sets[BLE_ABS_COMMON_HDL].status & BLE_ABS_ADV_STATUS_ADV_SLOW_START))
                {
                    /*  slow -> off */
                    set_adv_status(BLE_ABS_COMMON_HDL, 0, BLE_ABS_ADV_STATUS_ADV_SLOW_START);
                }
            }
        }
        else
        {
            set_adv_status(BLE_ABS_COMMON_HDL, 
                           0, 
                           (BLE_ABS_ADV_STATUS_ADV_SLOW_START | BLE_ABS_ADV_STATUS_ADV_FAST_START | 
                           BLE_ABS_ADV_COMM_LEG));
        }
    }
    else
    {
        /*  slow -> off */
        set_adv_status(BLE_ABS_COMMON_HDL, 0, (BLE_ABS_ADV_STATUS_ADV_SLOW_START | BLE_ABS_ADV_COMM_NON));
    }
#endif /* (BLE_CFG_LIB_TYPE == 0) */

    return;
} /* End of function abs_adv_off_hdlr() */

/***********************************************************************************************************************
 * Function Name: abs_adv_rept_hdlr
 * Description  : Handler for GAP BLE_GAP_EVENT_ADV_REPT_IND event.
 * Arguments    : st_ble_evt_data_t * p_event_data         ; BLE_GAP_EVENT_ADV_REPT_IND event parameters
 * Return Value : BLE_SUCCESS(0x0000)                       ; Success 
 *              : BLE_ERR_NOT_FOUND(0x000D)                 ; Filtering data is not included in the advertising data.
 **********************************************************************************************************************/
static ble_status_t abs_adv_rept_hdlr(st_ble_evt_data_t * p_event_data)
{
    st_ble_gap_adv_rept_evt_t * p_adv_rept_param;
    uint8_t * p_buf;
    uint32_t len;

    if((NULL == gs_abs_scan.param.p_filter_data) || (0 == gs_abs_scan.param.filter_data_length))
    {
        return BLE_SUCCESS;
    }

    p_adv_rept_param = (st_ble_gap_adv_rept_evt_t *)p_event_data->p_param;

    if(0x00 == p_adv_rept_param->adv_rpt_type)
    {
        p_buf = p_adv_rept_param->param.p_adv_rpt->p_data;
        len = p_adv_rept_param->param.p_adv_rpt->len;
    }
    else if(0x01 == p_adv_rept_param->adv_rpt_type)
    {
        p_buf = p_adv_rept_param->param.p_ext_adv_rpt->p_data;
        len = p_adv_rept_param->param.p_ext_adv_rpt->len;
    }
    else
    {
        if(0x02 == p_adv_rept_param->adv_rpt_type)
        {
            p_buf = p_adv_rept_param->param.p_per_adv_rpt->p_data;
            len = p_adv_rept_param->param.p_per_adv_rpt->len;
        }
    }

    /* If the AD data length is longer than the filter length, memory compare is performed. */
    if(len >= (uint32_t)gs_abs_scan.param.filter_data_length)
    {
        uint32_t i;
        uint16_t pos;
        pos = 0;

        while (pos < len)
        {
            /* Each advertising structure have following constructs.
            - Length: 1 byte (The length of AD type + AD data)
            - AD type: 1 byte
            - AD data: variable
            */
            uint8_t ad_len = p_buf[pos] - 1;
            uint8_t type   = p_buf[pos + 1];

            /* invalid ad_len */
            if( (0 == ad_len) || (len < (ad_len + 1 + pos)) )
            {
                /* do nothing */
            }
            /* Prefix search with AD type in p_filter_data */
            else if (0 == gs_abs_scan.param.filter_ad_type)
            {
                if ((ad_len + 2) >= gs_abs_scan.param.filter_data_length)
                {
                    /* Memory compare including Length + AD type + AD data : exact match */
                    if(0 == memcmp(&p_buf[pos],
                            gs_abs_scan.param.p_filter_data, 
                            (uint32_t)gs_abs_scan.param.filter_data_length))
                    {
                        return BLE_SUCCESS;
                    }
                }
            }
            else if ((type == gs_abs_scan.param.filter_ad_type))
            {
                /* Memory compare only AD data : Partial match */
                for(i=0; (i<ad_len) && ((i+gs_abs_scan.param.filter_data_length)<=ad_len); i++)
                {
                    if(0 == memcmp(&p_buf[pos+2+i], 
                            gs_abs_scan.param.p_filter_data, 
                            (uint32_t)gs_abs_scan.param.filter_data_length))
                    {
                        return BLE_SUCCESS;
                    }
                }
            }
            else
            {
                /* do nothing */
            }

            pos += ad_len + 2;
        }
    }

    return BLE_ERR_NOT_FOUND;
} /* End of function abs_adv_rept_hdlr() */

/***********************************************************************************************************************
 * Function Name: abs_loc_ver_hdlr
 * Description  : Handler for GAP BLE_GAP_EVENT_LOC_VER_INFO event.
 * Arguments    : st_ble_evt_data_t * p_event_data         ; BLE_GAP_EVENT_LOC_VER_INFO event parameters
 * Return Value : none
 **********************************************************************************************************************/
static void abs_loc_ver_hdlr(st_ble_evt_data_t * p_event_data)
{
    st_ble_gap_loc_dev_info_evt_t * ev_param;
    ev_param = (st_ble_gap_loc_dev_info_evt_t *)p_event_data->p_param;
    gs_loc_bd_addr = ev_param->l_dev_addr;

    return;
} /* End of function abs_loc_ver_hdlr() */

/***********************************************************************************************************************
 * Function Name: conv_scan_phy_param
 * Description  : Convert Abstraction API scan phy parameters to GAP scan phy parameters.
 * Arguments    : st_ble_abs_scan_phy_param_t * p_abs_phy        ; Abstraction API scan parameters
 *              : st_ble_gap_scan_phy_param_t * p_gap_phy        ; GAP phy scan parameters
 *              : st_ble_gap_scan_on_t * p_scan_enable           ; GAP scan enable parameters
 * Return Value : none
 **********************************************************************************************************************/
static void conv_scan_phy_param(st_ble_abs_scan_phy_param_t * p_abs_phy, 
                         st_ble_gap_scan_phy_param_t * p_gap_phy, 
                         st_ble_gap_scan_on_t * p_scan_enable)
{
    p_gap_phy->scan_type = p_abs_phy->scan_type;
    if(gs_abs_scan.param.fast_period)
    {
        p_gap_phy->scan_intv = p_abs_phy->fast_intv;
        p_gap_phy->scan_window = p_abs_phy->fast_window;
        p_scan_enable->duration = gs_abs_scan.param.fast_period;
    }
    else
    {
        p_gap_phy->scan_intv = p_abs_phy->slow_intv;
        p_gap_phy->scan_window = p_abs_phy->slow_window;
        p_scan_enable->duration = gs_abs_scan.param.slow_period;
    }
} /* End of function conv_scan_phy_param() */

/***********************************************************************************************************************
 * Function Name: conv_scan_param
 * Description  : Convert Abstraction API scan parameters to GAP scan parameters.
 * Arguments    : st_ble_gap_scan_param_t * p_gap_scan_param      ; Abstraction API scan parameters
 *              : st_ble_gap_scan_on_t * p_gap_scan_enable        ; GAP scan parameters
 *              : uint32_t status                                 ; Scan status
 * Return Value : none
 **********************************************************************************************************************/
static void conv_scan_param(st_ble_gap_scan_param_t * p_gap_scan_param, 
                     st_ble_gap_scan_on_t * p_gap_scan_enable, 
                     uint32_t status)
{
    p_gap_scan_param->o_addr_type = BLE_GAP_ADDR_PUBLIC;
    p_gap_scan_param->filter_policy = gs_abs_scan.param.dev_filter;
    p_gap_scan_enable->proc_type = BLE_GAP_SC_PROC_OBS;
    p_gap_scan_enable->period = 0;
    p_gap_scan_enable->filter_dups = gs_abs_scan.param.filter_dups;

    if(BLE_ABS_SCAN_STATUS_FAST_START == status)
    {
        if(gs_abs_scan.param.p_phy_param_1M)
        {
            conv_scan_phy_param(gs_abs_scan.param.p_phy_param_1M, 
                                p_gap_scan_param->p_phy_param_1M, p_gap_scan_enable);
        }

#if (BLE_CFG_LIB_TYPE == 0)
        if(gs_abs_scan.param.p_phy_param_coded)
        {
            conv_scan_phy_param(gs_abs_scan.param.p_phy_param_coded, 
                                p_gap_scan_param->p_phy_param_coded, p_gap_scan_enable);
        }
#endif /* (BLE_CFG_LIB_TYPE == 0) */

    }
    else
    {
        if(gs_abs_scan.param.p_phy_param_1M)
        {
            p_gap_scan_param->p_phy_param_1M->scan_type = gs_abs_scan.param.p_phy_param_1M->scan_type;
            p_gap_scan_param->p_phy_param_1M->scan_intv = gs_abs_scan.param.p_phy_param_1M->slow_intv;
            p_gap_scan_param->p_phy_param_1M->scan_window = gs_abs_scan.param.p_phy_param_1M->slow_window;
        }

#if (BLE_CFG_LIB_TYPE == 0)
        if(gs_abs_scan.param.p_phy_param_coded)
        {
            p_gap_scan_param->p_phy_param_coded->scan_type = gs_abs_scan.param.p_phy_param_coded->scan_type;
            p_gap_scan_param->p_phy_param_coded->scan_intv = gs_abs_scan.param.p_phy_param_coded->slow_intv;
            p_gap_scan_param->p_phy_param_coded->scan_window = gs_abs_scan.param.p_phy_param_coded->slow_window;
        }
#endif /* (BLE_CFG_LIB_TYPE == 0) */

        p_gap_scan_enable->duration = gs_abs_scan.param.slow_period;
    }

    return;
} /* End of function conv_scan_param() */

/***********************************************************************************************************************
 * Function Name: ads_scan_on_hdlr
 * Description  : Handler for GAP BLE_GAP_EVENT_SCAN_ON event.
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
#if (BLE_CFG_LIB_TYPE != 0)
static void ads_scan_on_hdlr(void)
{
    if(gs_abs_scan.status & BLE_ABS_SCAN_STATUS_SLOW_START)
    {
        if(0 != gs_abs_scan.param.slow_period)
        {
            R_BLE_TIMER_Create(&gs_scan_t_hdl, 
                               (gs_abs_scan.param.slow_period * 10), 
                               BLE_TIMER_ONE_SHOT, ads_scan_to_hdlr);
            R_BLE_TIMER_Start(gs_scan_t_hdl);
        }
    }
}
#endif /* (BLE_CFG_LIB_TYPE != 0) */

/***********************************************************************************************************************
 * Function Name: ads_scan_off_hdlr
 * Description  : Handler for GAP BLE_GAP_EVENT_SCAN_OFF event.
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
static void ads_scan_off_hdlr(void)
{
#if (BLE_CFG_LIB_TYPE != 0)
    R_BLE_TIMER_Stop(gs_scan_t_hdl);
    R_BLE_TIMER_Delete(&gs_scan_t_hdl);
    gs_scan_t_hdl = BLE_TIMER_INVALID_HDL;
#else /* BLE_CFG_LIB_TYPE != 0 */
    set_scan_status(0, 0xFFFFFFFF);
#endif /* (BLE_CFG_LIB_TYPE != 0) */
}

/***********************************************************************************************************************
 * Function Name: ads_scan_to_hdlr
 * Description  : Handler for GAP BLE_GAP_EVENT_SCAN_TO event.
 * Arguments    : uint32_t hdl                                 ; Timer handle
 * Return Value : none
 **********************************************************************************************************************/
static void ads_scan_to_hdlr(uint32_t hdl)
{
    st_ble_gap_scan_param_t scan_param;
    st_ble_gap_scan_phy_param_t phy_param_1M;
#if (BLE_CFG_LIB_TYPE == 0)
    st_ble_gap_scan_phy_param_t phy_param_coded;
#endif /* (BLE_CFG_LIB_TYPE == 0) */
    st_ble_gap_scan_on_t scan_enable;
    ble_status_t retval;

    (void)hdl;

#if (BLE_CFG_LIB_TYPE != 0)
    R_BLE_GAP_StopScan();
    R_BLE_TIMER_Stop(gs_scan_t_hdl);
    R_BLE_TIMER_Delete(&gs_scan_t_hdl);
    gs_scan_t_hdl = BLE_TIMER_INVALID_HDL;
#endif /* (BLE_CFG_LIB_TYPE != 0) */

    if(gs_abs_scan.status & BLE_ABS_SCAN_STATUS_FAST_START)
    {
        /*  fast -> slow */
        set_scan_status(0, BLE_ABS_SCAN_STATUS_SLOW_START);
        scan_param.p_phy_param_1M = gs_abs_scan.param.p_phy_param_1M ? &phy_param_1M : NULL;
#if (BLE_CFG_LIB_TYPE == 0)
        scan_param.p_phy_param_coded = gs_abs_scan.param.p_phy_param_coded ? &phy_param_coded : NULL;
#endif /* (BLE_CFG_LIB_TYPE == 0) */

        conv_scan_param(&scan_param, &scan_enable, BLE_ABS_SCAN_STATUS_SLOW_START);

        retval = R_BLE_GAP_StartScan(&scan_param, &scan_enable);
        if(BLE_SUCCESS == retval)
        {
            set_scan_status(BLE_ABS_SCAN_STATUS_SLOW_START, BLE_ABS_SCAN_STATUS_FAST_START);
        }
    }
    else
    {
        if(gs_abs_scan.status & BLE_ABS_SCAN_STATUS_SLOW_START)
        {
            /*  slow -> off */
            set_scan_status(0, BLE_ABS_SCAN_STATUS_SLOW_START);
        }
    }

    return;
} /* End of function ads_scan_to_hdlr() */

/***********************************************************************************************************************
 * Function Name: ads_conn_ind_hdlr
 * Description  : Handler for GAP BLE_GAP_EVENT_CONN_IND event.
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
static void ads_conn_ind_hdlr(void)
{
    R_BLE_TIMER_Stop(gs_conn_t_hdl);
    R_BLE_TIMER_Delete(&gs_conn_t_hdl);

#if (BLE_CFG_LIB_TYPE != 0)
    R_BLE_TIMER_Stop(gs_adv_t_hdl);
    R_BLE_TIMER_Delete(&gs_adv_t_hdl);

    set_adv_status(BLE_ABS_COMMON_HDL, 
                    0, 
                    (BLE_ABS_ADV_STATUS_ADV_SLOW_START | BLE_ABS_ADV_STATUS_ADV_FAST_START | 
                    BLE_ABS_ADV_COMM_LEG));
#endif /* (BLE_CFG_LIB_TYPE != 0) */

    return;
} /* End of function ads_conn_ind_hdlr() */

/***********************************************************************************************************************
 * Function Name: set_abs_cb
 * Description  : Set gap callback and vendor specific callback function.
 * Arguments    : ble_gap_app_cb_t gap_cb      ; GAP callback function
 *              : ble_vs_app_cb_t vs_cbe       ; Vendor Specific callback function
 * Return Value : none
 **********************************************************************************************************************/
static void set_abs_cb(ble_gap_app_cb_t gap_cb, ble_vs_app_cb_t vs_cb)
{
    gs_abs_gap_cb = gap_cb;
    gs_abs_vs_cb = vs_cb;
} /* End of function set_abs_cb() */

/***********************************************************************************************************************
 * Function Name: set_conn_adv_intv
 * Description  : Set advertising interval.
 * Arguments    : st_ble_gap_adv_param_t * p_adv_param      ; GAP advertising parameters.
 *              : uint32_t fast_adv_intv                    ; Fast advertising interval.
 *              : uint32_t slow_adv_intv                    ; Slow advertising interval.
 *              : uint16_t fast_periodv                     ; Fast advertising period.
 * Return Value : none
 **********************************************************************************************************************/
static void set_conn_adv_intv(st_ble_gap_adv_param_t * p_adv_param, 
                         uint32_t fast_adv_intv, 
                         uint32_t slow_adv_intv, 
                         uint16_t fast_period)
{
    /*  check advertising interval */
    if(fast_period)
    {
        p_adv_param->adv_intv_min = fast_adv_intv;
        p_adv_param->adv_intv_max = fast_adv_intv;
    }
    else
    {
        p_adv_param->adv_intv_min = slow_adv_intv;
        p_adv_param->adv_intv_max = slow_adv_intv;
    }

    return;
} /* End of function set_conn_adv_intv() */

/***********************************************************************************************************************
 * Function Name: update_data_status
 * Description  : Update advertising data status.
 * Arguments    : uint32_t status                      ; Advertising status.
 *              : uint8_t * p_data                     ; Advertising data.
 *              : uint16_t data_len                    ; Advertising data length.
 *              : uint8_t adv_hdl                      ; Advertising handle.
 * Return Value : none
 **********************************************************************************************************************/
static void update_data_status(uint32_t status, uint8_t * p_data, uint16_t data_len, uint8_t adv_hdl)
{
    if(gs_adv_sets[adv_hdl].status & status)
    {
        if((0 != data_len) && (NULL != p_data))
        {
            set_adv_status(adv_hdl, 0, status);
        }
    }
} /* End of function update_data_status() */

/***********************************************************************************************************************
 * Function Name: conv_legacy_adv_param
 * Description  : Convert the legacy advertising parameters to GAP advertising parameters.
 * Arguments    : st_ble_abs_legacy_adv_param_t * p_param     ; Abstraction API Legacy advertising parameters.
 *              : st_ble_gap_adv_param_t * p_adv_param        ; GAP advertising parameters.
 * Return Value : BLE_SUCCESS(0x0000)                         ; Success 
 *              : BLE_ERR_INVALID_ARG(0x0003)                 ; The advertising parameter is out of range. 
 **********************************************************************************************************************/
static ble_status_t conv_legacy_adv_param(st_ble_abs_legacy_adv_param_t * p_param, st_ble_gap_adv_param_t * p_adv_param)
{
    p_adv_param->adv_hdl = BLE_ABS_LEGACY_HDL;
    p_adv_param->adv_prop_type = BLE_GAP_LEGACY_PROP_ADV_IND;
    p_adv_param->adv_ch_map = p_param->adv_ch_map;

    if(BLE_GAP_ADDR_RPA_ID_RANDOM < p_param->o_addr_type)
    {
        return BLE_ERR_INVALID_ARG;
    }

    if(p_param->o_addr)
    {
        memcpy(p_adv_param->o_addr, p_param->o_addr, BLE_BD_ADDR_LEN);
    }

    p_adv_param->o_addr_type = p_param->o_addr_type;

    if(p_param->p_addr)
    {
        memcpy(p_adv_param->p_addr, p_param->p_addr->addr, BLE_BD_ADDR_LEN);
        p_adv_param->p_addr_type = p_param->p_addr->type;
    }
    else
    {
        p_adv_param->p_addr_type = BLE_GAP_ADDR_PUBLIC;
    }

    if(BLE_ABS_ADV_ALLOW_CONN_WLST < p_param->filter)
    {
        return BLE_ERR_INVALID_ARG;
    }

    p_adv_param->filter_policy = p_param->filter;

    p_adv_param->adv_phy = BLE_GAP_ADV_PHY_1M;
    p_adv_param->sec_adv_max_skip = 0x00;
    p_adv_param->sec_adv_phy = BLE_GAP_ADV_PHY_1M;
    p_adv_param->scan_req_ntf_flag = BLE_GAP_SCAN_REQ_NTF_DISABLE;

    return BLE_SUCCESS;
} /* End of function conv_legacy_adv_param() */

/***********************************************************************************************************************
 * Function Name: conv_ext_adv_param
 * Description  : Convert the extended advertising parameters to GAP advertising parameters.
 * Arguments    : st_ble_abs_ext_adv_param_t * p_param        ; Abstraction API Extended advertising parameters.
 *              : st_ble_gap_adv_param_t * p_adv_param        ; GAP advertising parameters.
 * Return Value : BLE_SUCCESS(0x0000)                         ; Success 
 *              : BLE_ERR_INVALID_ARG(0x0003)                 ; The advertising parameter is out of range. 
 **********************************************************************************************************************/
#if (BLE_CFG_LIB_TYPE == 0)
static ble_status_t conv_ext_adv_param(st_ble_abs_ext_adv_param_t * p_param, st_ble_gap_adv_param_t * p_adv_param)
{
    p_adv_param->adv_hdl = BLE_ABS_EXT_HDL;
    p_adv_param->adv_ch_map = p_param->adv_ch_map;

    if(BLE_GAP_ADDR_RPA_ID_RANDOM < p_param->o_addr_type)
    {
        return BLE_ERR_INVALID_ARG;
    }

    if(p_param->o_addr)
    {
        memcpy(p_adv_param->o_addr, p_param->o_addr, BLE_BD_ADDR_LEN);
    }

    p_adv_param->o_addr_type = p_param->o_addr_type;

    if(p_param->p_addr)
    {
        memcpy(p_adv_param->p_addr, p_param->p_addr->addr, BLE_BD_ADDR_LEN);
        p_adv_param->p_addr_type = p_param->p_addr->type;
        p_adv_param->adv_prop_type = BLE_GAP_EXT_PROP_ADV_CONN_NOSCAN_DIRECT;
    }
    else
    {
        p_adv_param->p_addr_type = BLE_GAP_ADDR_PUBLIC;
        p_adv_param->adv_prop_type = BLE_GAP_EXT_PROP_ADV_CONN_NOSCAN_UNDIRECT;
    }

    if(BLE_ABS_ADV_ALLOW_CONN_WLST < p_param->filter)
    {
        return BLE_ERR_INVALID_ARG;
    }

    p_adv_param->filter_policy = p_param->filter;

    if((BLE_GAP_ADV_PHY_1M != p_param->adv_phy) && (BLE_GAP_ADV_PHY_CD != p_param->adv_phy))
    {
        return BLE_ERR_INVALID_ARG;
    }

    p_adv_param->adv_phy = p_param->adv_phy;
    p_adv_param->sec_adv_max_skip = 0x00;

    if((BLE_GAP_ADV_PHY_1M > p_param->sec_adv_phy) || (BLE_GAP_ADV_PHY_CD < p_param->sec_adv_phy))
    {
        return BLE_ERR_INVALID_ARG;
    }

    p_adv_param->sec_adv_phy = p_param->sec_adv_phy;
    p_adv_param->scan_req_ntf_flag = BLE_GAP_SCAN_REQ_NTF_DISABLE;

    return BLE_SUCCESS;
} /* End of function conv_ext_adv_param() */
#endif /* (BLE_CFG_LIB_TYPE == 0) */

/***********************************************************************************************************************
 * Function Name: conv_non_conn_adv_param
 * Description  : Convert the non-connectable advertising parameters to GAP advertising parameters.
 * Arguments    : st_ble_abs_non_conn_adv_param_t * p_param   ; Abstraction API Non-Connectable advertising parameters.
 *              : st_ble_gap_adv_param_t * p_adv_param        ; GAP advertising parameters.
 * Return Value : BLE_SUCCESS(0x0000)                         ; Success 
 *              : BLE_ERR_INVALID_ARG(0x0003)                 ; The advertising parameter is out of range. 
 **********************************************************************************************************************/
static ble_status_t conv_non_conn_adv_param(st_ble_abs_non_conn_adv_param_t * p_param, 
                                     st_ble_gap_adv_param_t * p_adv_param, 
                                     uint8_t adv_hdl)
{
    p_adv_param->adv_hdl = adv_hdl;
    p_adv_param->adv_ch_map = p_param->adv_ch_map;

    if(BLE_GAP_ADDR_RPA_ID_RANDOM < p_param->o_addr_type)
    {
        return BLE_ERR_INVALID_ARG;
    }

    if(p_param->o_addr)
    {
        memcpy(p_adv_param->o_addr, p_param->o_addr, BLE_BD_ADDR_LEN);
    }

    p_adv_param->o_addr_type = p_param->o_addr_type;
#if (BLE_CFG_LIB_TYPE != 0)
    p_param->adv_phy = BLE_ABS_ADV_PHY_LEGACY;
#endif /* (BLE_CFG_LIB_TYPE != 0) */
    if((BLE_GAP_ADV_PHY_1M < p_param->adv_phy) && (BLE_GAP_ADV_PHY_CD != p_param->adv_phy))
    {
        return BLE_ERR_INVALID_ARG;
    }
#if (BLE_CFG_LIB_TYPE == 0)
    else
    {
        if((BLE_ABS_ADV_PHY_LEGACY == p_param->adv_phy) && (BLE_ABS_PERD_HDL == adv_hdl))
        {
            return BLE_ERR_INVALID_ARG;
        }
    }
#endif /* (BLE_CFG_LIB_TYPE == 0) */

    p_adv_param->adv_phy = (uint8_t)((BLE_ABS_ADV_PHY_LEGACY == p_param->adv_phy) 
                                      ? BLE_GAP_ADV_PHY_1M
                                      : p_param->adv_phy);

    if(p_param->p_addr)
    {
        memcpy(p_adv_param->p_addr, p_param->p_addr->addr, BLE_BD_ADDR_LEN);
        p_adv_param->p_addr_type = p_param->p_addr->type;
        p_adv_param->adv_prop_type = (uint16_t)((BLE_ABS_ADV_PHY_LEGACY != p_param->adv_phy) 
                                                 ? BLE_GAP_EXT_PROP_ADV_NOCONN_NOSCAN_DIRECT
                                                 : BLE_GAP_LEGACY_PROP_ADV_NONCONN_IND);
    }
    else
    {
        p_adv_param->p_addr_type = BLE_GAP_ADDR_PUBLIC;
        p_adv_param->adv_prop_type = (uint16_t)((BLE_ABS_ADV_PHY_LEGACY != p_param->adv_phy) 
                                                 ? BLE_GAP_EXT_PROP_ADV_NOCONN_NOSCAN_UNDIRECT 
                                                 : BLE_GAP_LEGACY_PROP_ADV_NONCONN_IND);
    }

    p_adv_param->filter_policy = BLE_ABS_ADV_ALLOW_CONN_ANY;
    p_adv_param->sec_adv_max_skip = 0x00;

    if((BLE_GAP_ADV_PHY_1M > p_param->sec_adv_phy) || (BLE_GAP_ADV_PHY_CD < p_param->sec_adv_phy))
    {
        return BLE_ERR_INVALID_ARG;
    }

    p_adv_param->sec_adv_phy = p_param->sec_adv_phy;
    p_adv_param->scan_req_ntf_flag = BLE_GAP_SCAN_REQ_NTF_DISABLE;

    return BLE_SUCCESS;
} /* End of function conv_non_conn_adv_param() */

/***********************************************************************************************************************
 * Function Name: set_adv_status
 * Description  : Set advertising status.
 * Arguments    : uint8_t adv_hdl      ; Advertising handle.
 *              : uint32_t set         ; Set bits.
 *              : uint32_t clear       ; Clear bits.
 * Return Value : none
 **********************************************************************************************************************/
static void set_adv_status(uint8_t adv_hdl, uint32_t set, uint32_t clear)
{
    gs_adv_sets[adv_hdl].status |= set;
    gs_adv_sets[adv_hdl].status &= ~clear;
} /* End of function set_adv_status() */

/***********************************************************************************************************************
 * Function Name: set_adv_param
 * Description  : Store advertising configuration.
 * Arguments    : void * p_adv_param   ; Advertising configuration to be stored.
 *              : uint8_t adv_hdl      ; Advertising handle.
 * Return Value : none
 **********************************************************************************************************************/
static void set_adv_param(void * p_adv_param, uint8_t adv_hdl)
{
    switch(adv_hdl)
    {
        case BLE_ABS_LEGACY_HDL :
            {
                st_ble_abs_legacy_adv_param_t * p_abs_legacy;
                p_abs_legacy = (st_ble_abs_legacy_adv_param_t * )p_adv_param;
#if (BLE_CFG_LIB_TYPE != 0)
                adv_hdl = BLE_ABS_COMMON_HDL;
#endif /* (BLE_CFG_LIB_TYPE == 0) */
                memcpy(&gs_adv_sets[adv_hdl].param.legacy, p_abs_legacy, sizeof(st_ble_abs_legacy_adv_param_t));
                if(NULL != p_abs_legacy->p_addr)
                {
                    memcpy(&gs_adv_sets[adv_hdl].rem_addr, p_abs_legacy->p_addr, sizeof(st_ble_dev_addr_t));
                    gs_adv_sets[adv_hdl].param.legacy.p_addr = &gs_adv_sets[adv_hdl].rem_addr;
                }
            }
        break;

#if (BLE_CFG_LIB_TYPE == 0)
        case BLE_ABS_EXT_HDL :
            {
                st_ble_abs_ext_adv_param_t * p_abs_ext;
                p_abs_ext = (st_ble_abs_ext_adv_param_t * )p_adv_param;
                memcpy(&gs_adv_sets[adv_hdl].param.ext, p_abs_ext, sizeof(st_ble_abs_ext_adv_param_t));
                if(NULL != p_abs_ext->p_addr)
                {
                    memcpy(&gs_adv_sets[adv_hdl].rem_addr, p_abs_ext->p_addr, sizeof(st_ble_dev_addr_t));
                    gs_adv_sets[adv_hdl].param.ext.p_addr = &gs_adv_sets[adv_hdl].rem_addr;
                }
            }
        break;
#endif /* (BLE_CFG_LIB_TYPE == 0) */

        case BLE_ABS_NON_CONN_HDL :
            {
                st_ble_abs_non_conn_adv_param_t * p_abs_non_conn;
                p_abs_non_conn = (st_ble_abs_non_conn_adv_param_t * )p_adv_param;
#if (BLE_CFG_LIB_TYPE != 0)
                adv_hdl = BLE_ABS_COMMON_HDL;
#endif /* (BLE_CFG_LIB_TYPE == 0) */
                memcpy(&gs_adv_sets[adv_hdl].param.non_conn, p_abs_non_conn, sizeof(st_ble_abs_non_conn_adv_param_t));
                if(NULL != p_abs_non_conn->p_addr)
                {
                    memcpy(&gs_adv_sets[adv_hdl].rem_addr, p_abs_non_conn->p_addr, sizeof(st_ble_dev_addr_t));
                    gs_adv_sets[adv_hdl].param.non_conn.p_addr = &gs_adv_sets[adv_hdl].rem_addr;
                }
            }
        break;

        default : /*  BLE_ABS_PERD_HDL */
#if (BLE_CFG_LIB_TYPE == 0)
            {
                st_ble_abs_perd_adv_param_t * p_abs_perd;
                p_abs_perd = (st_ble_abs_perd_adv_param_t * )p_adv_param;
                memcpy(&gs_adv_sets[adv_hdl].param.perd, p_abs_perd, sizeof(st_ble_abs_perd_adv_param_t));
                if(NULL != p_abs_perd->param.p_addr)
                {
                    memcpy(&gs_adv_sets[adv_hdl].rem_addr, p_abs_perd->param.p_addr, sizeof(st_ble_dev_addr_t));
                    gs_adv_sets[adv_hdl].param.perd.param.p_addr = &gs_adv_sets[adv_hdl].rem_addr;
                }
            }
#endif /* (BLE_CFG_LIB_TYPE == 0) */
        break;

    }
} /* End of function set_adv_param() */

/***********************************************************************************************************************
 * Function Name: check_scan_phy_param
 * Description  : Check scan phy parameters.
 * Arguments    : st_ble_abs_scan_phy_param_t * p_scan_phy   ; scan phy parameters to be stored.
 * Return Value : BLE_SUCCESS(0x0000)                        ; Success 
 *              : BLE_ERR_INVALID_ARG(0x0003)                ; Scan phy parameter is out of range.
 **********************************************************************************************************************/
static ble_status_t check_scan_phy_param(st_ble_abs_scan_phy_param_t * p_scan_phy)
{
    if((BLE_GAP_SCAN_INTV_MIN > p_scan_phy->fast_intv) ||
       (BLE_GAP_SCAN_INTV_MIN > p_scan_phy->slow_intv) ||
       (BLE_GAP_SCAN_INTV_MIN > p_scan_phy->fast_window) ||
       (BLE_GAP_SCAN_INTV_MIN > p_scan_phy->slow_window) ||
       (BLE_GAP_SCAN_ACTIVE < p_scan_phy->scan_type))
    {
        return BLE_ERR_INVALID_ARG;
    }

    return BLE_SUCCESS;
}

/***********************************************************************************************************************
 * Function Name: set_scan_param
 * Description  : Store scan configuration.
 * Arguments    : st_ble_abs_scan_param_t * p_scan_param   ; scan configuration to be stored.
 * Return Value : BLE_SUCCESS(0x0000)                        ; Success 
 *              : BLE_ERR_INVALID_ARG(0x0003)                ; Scan phy parameter is out of range.
 **********************************************************************************************************************/
static ble_status_t set_scan_param(st_ble_abs_scan_param_t * p_scan_param)
{
    ble_status_t retval;

    /* check abs scan parameters */
    if(p_scan_param->p_phy_param_1M)
    {
        retval = check_scan_phy_param(p_scan_param->p_phy_param_1M);
        if(BLE_SUCCESS != retval)
        {
            return retval;
        }
    }

#if (BLE_CFG_LIB_TYPE == 0)
    if(p_scan_param->p_phy_param_coded)
    {
        retval = check_scan_phy_param(p_scan_param->p_phy_param_coded);
        if(BLE_SUCCESS != retval)
        {
            return retval;
        }
    }
#endif /* (BLE_CFG_LIB_TYPE == 0) */

#if (BLE_CFG_LIB_TYPE == 0)
    if(BLE_ABS_CONN_EXT_ADV_DATA_LEN < p_scan_param->filter_data_length)
#else /* (BLE_CFG_LIB_TYPE == 0) */
    if(BLE_ABS_LEGACY_ADV_DATA_LEN < p_scan_param->filter_data_length)
#endif /* (BLE_CFG_LIB_TYPE == 0) */
    {
        return BLE_ERR_INVALID_ARG;
    }

    memcpy(&gs_abs_scan.param, p_scan_param, sizeof(st_ble_abs_scan_param_t));
    if(p_scan_param->p_phy_param_1M)
    {
        memcpy(&gs_abs_scan.phy_1M, p_scan_param->p_phy_param_1M, sizeof(st_ble_abs_scan_phy_param_t));
        gs_abs_scan.param.p_phy_param_1M = &gs_abs_scan.phy_1M;
    }

#if (BLE_CFG_LIB_TYPE == 0)
    if(p_scan_param->p_phy_param_coded)
    {
        memcpy(&gs_abs_scan.phy_coded, p_scan_param->p_phy_param_coded, sizeof(st_ble_abs_scan_phy_param_t));
        gs_abs_scan.param.p_phy_param_coded = &gs_abs_scan.phy_coded;
    }
#endif /* (BLE_CFG_LIB_TYPE == 0) */

    return BLE_SUCCESS;
} /* End of function set_scan_param() */

/***********************************************************************************************************************
 * Function Name: set_scan_status
 * Description  : Set scan status.
 * Arguments    : uint32_t set         ; Set bits.
 *              : uint32_t clear       ; Clear bits.
 * Return Value : none
 **********************************************************************************************************************/
static void set_scan_status(uint32_t set, uint32_t clear)
{
    gs_abs_scan.status |= set;
    gs_abs_scan.status &= ~clear;
} /* End of function set_scan_status() */

/***********************************************************************************************************************
 * Function Name: set_irk_to_rslv
 * Description  : Register IRK to the Resolving List.
 * Arguments    : uint8_t * p_lc_irk        ; IRK to be registered in the Resolving List.
 * Return Value : none
 **********************************************************************************************************************/
static void set_irk_to_rslv(uint8_t * p_lc_irk)
{
    st_ble_gap_rslv_list_key_set_t peer_irk;
    st_ble_dev_addr_t rem_addr;
    ble_status_t retval;

    memset(peer_irk.remote_irk, 0xAA, BLE_GAP_IRK_SIZE);
    peer_irk.local_irk_type = BLE_GAP_RL_LOC_KEY_REGISTERED;
    memset(rem_addr.addr, 0x55, BLE_BD_ADDR_LEN);
    rem_addr.type = BLE_GAP_ADDR_PUBLIC;

    R_BLE_GAP_SetLocIdInfo(&gs_loc_bd_addr, p_lc_irk);

    /*  store local id info */
    R_BLE_SECD_WriteLocInfo(&gs_loc_bd_addr, p_lc_irk, NULL);

    retval = R_BLE_GAP_ConfRslvList(BLE_GAP_LIST_ADD_DEV, &rem_addr, &peer_irk, 1);
    gs_set_privacy_status = (BLE_SUCCESS == retval) ? BLE_ABS_PV_STATUS_ADD_RSLV : 0 ;

    return;
} /* End of function set_irk_to_rslv() */

/***********************************************************************************************************************
 * Function Name: abs_conf_rslv_hdlr
 * Description  : Handler for GAP BLE_GAP_EVENT_RSLV_LIST_CONF_COMP event.
 * Arguments    : st_ble_evt_data_t * p_event_data         ; BLE_GAP_EVENT_RSLV_LIST_CONF_COMP event parameters
 * Return Value : none
 **********************************************************************************************************************/
static void abs_conf_rslv_hdlr(st_ble_evt_data_t * p_event_data)
{
    if(gs_set_privacy_status)
    {
        st_ble_gap_rslv_list_conf_evt_t * p_rslv_list_conf;
        ble_status_t retval;

        p_rslv_list_conf = (st_ble_gap_rslv_list_conf_evt_t *)p_event_data->p_param;
        if(BLE_GAP_LIST_ADD_DEV == p_rslv_list_conf->op_code)
        {
            st_ble_dev_addr_t rem_addr;
            memset(rem_addr.addr, 0x55, BLE_BD_ADDR_LEN);
            rem_addr.type = 0x00;
            retval = R_BLE_GAP_SetPrivMode(&rem_addr, &gs_privacy_mode, 1);
            gs_set_privacy_status = (BLE_SUCCESS == retval) 
                                    ? BLE_ABS_PV_STATUS_SET_MODE 
                                    : 0 ;
        }
    }

    return;
} /* End of function abs_conf_rslv_hdlr() */

/***********************************************************************************************************************
 * Function Name: abs_gapcb
 * Description  : GAP Event handler.
 * Arguments    : uint16_t event_type                  ; GAP event type.
 * Arguments    : ble_status_t event_result            ; GAP event result.
 * Arguments    : st_ble_evt_data_t * p_event_data     ; GAP event parameters
 * Return Value : none
 **********************************************************************************************************************/
static void abs_gapcb(uint16_t event_type, ble_status_t event_result, st_ble_evt_data_t * p_event_data)
{
    switch(event_type)
    {
        case BLE_GAP_EVENT_STACK_ON :
        {
            R_BLE_GAP_GetVerInfo();
            uint8_t irk[BLE_GAP_IRK_SIZE];
            st_ble_dev_addr_t idaddr;
            ble_status_t retval;

            R_BLE_SECD_Init();
            retval = R_BLE_SECD_ReadLocInfo(&idaddr, irk, NULL);
            if(BLE_SUCCESS == retval)
            {
                R_BLE_GAP_SetLocIdInfo(&idaddr, irk);
            }
        }
        break;

        case BLE_GAP_EVENT_LOC_VER_INFO :
            abs_loc_ver_hdlr(p_event_data);
        break;

        case BLE_GAP_EVENT_ADV_REPT_IND :
            if(BLE_SUCCESS != abs_adv_rept_hdlr(p_event_data))
            {
                return;
            }
        break;

        case BLE_GAP_EVENT_ADV_PARAM_SET_COMP :
            abs_adv_param_set_hdlr(p_event_data);
        break;

        case BLE_GAP_EVENT_ADV_DATA_UPD_COMP :
            abs_adv_data_set_hdlr(p_event_data);
        break;

        case BLE_GAP_EVENT_PERD_ADV_PARAM_SET_COMP :
            abs_perd_param_hdlr();
        break;

        case BLE_GAP_EVENT_PERD_ADV_ON :
            adv_start(BLE_ABS_PERD_HDL);
        break;

        case BLE_GAP_EVENT_PERD_ADV_OFF :
            set_adv_status(BLE_ABS_PERD_HDL, 0, BLE_ABS_ADV_STATUS_PERD_START);
        break;

        case BLE_GAP_EVENT_ADV_ON :
            if(BLE_SUCCESS != event_result)
            {
#if (BLE_CFG_LIB_TYPE == 0)
                st_ble_gap_adv_set_evt_t * p_param;
                p_param = (st_ble_gap_adv_set_evt_t *)p_event_data->p_param;
                set_adv_status(p_param->adv_hdl, 0, 0xFFFFFFFF);
#else /* (BLE_CFG_LIB_TYPE != 0) */
                set_adv_status(BLE_ABS_COMMON_HDL, 0, 0xFFFFFFFF);
#endif /* (BLE_CFG_LIB_TYPE != 0) */
            }
        break;

        case BLE_GAP_EVENT_ADV_OFF :
            abs_adv_off_hdlr(p_event_data);
        break;

        case BLE_GAP_EVENT_SCAN_TO :
            ads_scan_to_hdlr(BLE_TIMER_INVALID_HDL);
        break;

#if (BLE_CFG_LIB_TYPE != 0)
        case BLE_GAP_EVENT_SCAN_ON :
            ads_scan_on_hdlr();
        break;
#endif /* (BLE_CFG_LIB_TYPE != 0) */

        case BLE_GAP_EVENT_SCAN_OFF :
            ads_scan_off_hdlr();
        break;

        case BLE_GAP_EVENT_CONN_IND :
            ads_conn_ind_hdlr();
        break;

        case BLE_GAP_EVENT_CREATE_CONN_COMP:
            if(BLE_SUCCESS != event_result)
            {
                R_BLE_TIMER_Stop(gs_conn_t_hdl);
                R_BLE_TIMER_Delete(&gs_conn_t_hdl);
            }
        break;

        case BLE_GAP_EVENT_RSLV_LIST_CONF_COMP :
            abs_conf_rslv_hdlr(p_event_data);
        break;

        case BLE_GAP_EVENT_RPA_EN_COMP :
            gs_set_privacy_status = 0;
        break;

        case BLE_GAP_EVENT_PRIV_MODE_SET_COMP :
            if(BLE_ABS_PV_STATUS_SET_MODE == gs_set_privacy_status)
            {
                ble_status_t retval;
                retval = R_BLE_GAP_EnableRpa(BLE_GAP_RPA_ENABLED);
                gs_set_privacy_status = (BLE_SUCCESS == retval) 
                                         ? BLE_ABS_PV_STATUS_EN_RPA 
                                         : 0 ;
            }
        break;

        case BLE_GAP_EVENT_PAIRING_REQ :
            {
                st_ble_gap_pairing_info_evt_t * p_param;
                p_param = (st_ble_gap_pairing_info_evt_t *)p_event_data->p_param;
                R_BLE_GAP_ReplyPairing(p_param->conn_hdl, BLE_GAP_PAIRING_ACCEPT);
            }
        break;

        case BLE_GAP_EVENT_PAIRING_COMP :
            {
#if (BLE_CFG_EN_SEC_DATA == 1)
                if(BLE_SUCCESS == event_result)
                {
                    st_ble_gap_pairing_info_evt_t * p_param;
                    p_param = (st_ble_gap_pairing_info_evt_t *)p_event_data->p_param;
                    R_BLE_SECD_WriteRemKeys(&p_param->bd_addr, &p_param->auth_info);
                }
#endif /* (BLE_CFG_EN_SEC_DATA == 1) */
            }
        break;

        case BLE_GAP_EVENT_PEER_KEY_INFO :
            {
#if (BLE_CFG_EN_SEC_DATA == 1)
                st_ble_gap_peer_key_info_evt_t * p_param;
                p_param = (st_ble_gap_peer_key_info_evt_t *)p_event_data->p_param;
                R_BLE_SECD_RecvRemKeys(&p_param->bd_addr, &p_param->key_ex_param);
#endif /* (BLE_CFG_EN_SEC_DATA == 1) */
            }
        break;

        case BLE_GAP_EVENT_EX_KEY_REQ :
            {
                st_ble_gap_conn_hdl_evt_t * p_param;
                p_param = (st_ble_gap_conn_hdl_evt_t *)p_event_data->p_param;
                R_BLE_GAP_ReplyExKeyInfoReq(p_param->conn_hdl);
            }
        break;

        case BLE_GAP_EVENT_LTK_REQ :
            {
                st_ble_gap_ltk_req_evt_t * p_param;
                p_param = (st_ble_gap_ltk_req_evt_t *)p_event_data->p_param;
                R_BLE_GAP_ReplyLtkReq(p_param->conn_hdl, p_param->ediv, p_param->p_peer_rand, BLE_GAP_LTK_REQ_ACCEPT);
            }
        break;

        default :
        break;
    }

    gs_abs_gap_cb(event_type, event_result, p_event_data);

    return;
} /* End of function abs_gapcb() */

/***********************************************************************************************************************
 * Function Name: abs_rand_hdlr
 * Description  : Handler for Vendor Specific BLE_VS_EVENT_GET_RAND event.
 * Arguments    : st_ble_vs_evt_data_t * p_event_data         ; BLE_VS_EVENT_GET_RAND event parameters
 * Return Value : none
 **********************************************************************************************************************/
static void abs_rand_hdlr(st_ble_vs_evt_data_t * p_event_data)
{
    if(gs_set_privacy_status)
    {
        st_ble_vs_get_rand_comp_evt_t * p_rand_param;
        p_rand_param = (st_ble_vs_get_rand_comp_evt_t *)p_event_data->p_param;
        set_irk_to_rslv(p_rand_param->p_rand);
    }

    return;
} /* End of function abs_rand_hdlr() */

/***********************************************************************************************************************
 * Function Name: abs_vscb
 * Description  : Vendor Specific Event handler.
 * Arguments    : uint16_t event_type                  ; Vendor Specific event type.
 * Arguments    : ble_status_t event_result            ; Vendor Specific event result.
 * Arguments    : st_ble_vs_evt_data_t * p_event_data  ; Vendor Specific event parameters
 * Return Value : none
 **********************************************************************************************************************/
static void abs_vscb(uint16_t event_type, ble_status_t event_result, st_ble_vs_evt_data_t * p_event_data)
{
    switch(event_type)
    {
        case BLE_VS_EVENT_GET_RAND :
            abs_rand_hdlr(p_event_data);
        break;

        default :
        break;
    }

    if(gs_abs_vs_cb)
    {
        gs_abs_vs_cb(event_type, event_result, p_event_data);
    }

    return;
} /* End of function abs_vscb() */

#else /* (BLE_CFG_ABS_API_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0) */

ble_status_t R_BLE_ABS_Init(st_ble_abs_init_param_t * p_init_param)
{
    (void)p_init_param;
    return BLE_ERR_UNSUPPORTED;
}

void R_BLE_ABS_Reset(ble_event_cb_t init_cb)
{
    (void)init_cb;
}

ble_status_t R_BLE_ABS_StartLegacyAdv(st_ble_abs_legacy_adv_param_t * p_adv_param)
{
    (void)p_adv_param;
    return BLE_ERR_UNSUPPORTED;
}

ble_status_t R_BLE_ABS_StartExtAdv(st_ble_abs_ext_adv_param_t * p_adv_param)
{
    (void)p_adv_param;
    return BLE_ERR_UNSUPPORTED;
}

ble_status_t R_BLE_ABS_StartNonConnAdv(st_ble_abs_non_conn_adv_param_t * p_adv_param)
{
    (void)p_adv_param;
    return BLE_ERR_UNSUPPORTED;
}

ble_status_t R_BLE_ABS_StartPerdAdv(st_ble_abs_perd_adv_param_t * p_adv_param)
{
    (void)p_adv_param;
    return BLE_ERR_UNSUPPORTED;
}

ble_status_t R_BLE_ABS_StartScan(st_ble_abs_scan_param_t * p_scan_param)
{
    (void)p_scan_param;
    return BLE_ERR_UNSUPPORTED;
}

ble_status_t R_BLE_ABS_CreateConn(st_ble_abs_conn_param_t * p_conn_param)
{
    (void)p_conn_param;
    return BLE_ERR_UNSUPPORTED;
}

ble_status_t R_BLE_ABS_SetLocPrivacy(uint8_t * p_lc_irk, uint8_t privacy_mode)
{
    (void)p_lc_irk;
    (void)privacy_mode;
    return BLE_ERR_UNSUPPORTED;
}

ble_status_t R_BLE_ABS_StartAuth(uint16_t conn_hdl)
{
    (void)conn_hdl;
    return BLE_ERR_UNSUPPORTED;
}

#endif /* (BLE_CFG_ABS_API_EN == 1) && (BLE_CFG_HCI_MODE_EN == 0) */
