
/**
 *  \file blebrr_pl.c
 */

/*
 *  Copyright (C) 2013-2021. Mindtree Limited.
 *  All rights reserved.
 */

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_mesh_rx23w_if.h"
#include "blebrr.h"
#include "gatt_services.h"
#include "gatt_clients.h"

#include "logger/r_ble_logger.h"

/*******************************************************************************
* FIT module configuration check
*******************************************************************************/
#if (BLE_CFG_LIB_TYPE != 0) && (BLE_CFG_LIB_TYPE != 1)
#error Compact type of BLE Protocol Stack not available for Mesh FIT; because \
Compact type not support Scan. Set the BLE_CFG_LIB_TYPE to either 0 or 1.
#endif /* (BLE_CFG_LIB_TYPE != 0) && (BLE_CFG_LIB_TYPE != 1) */

#if (BLE_CFG_ABS_API_EN != 0)
#error Abstraction API of BLE FIT Module not supported; because \
BLE Initialization process conflicts with Mesh Bearer. Set the BLE_CFG_ABS_API_EN to 0.
#endif /* (BLE_CFG_ABS_API_EN != 0) */

/*******************************************************************************
* Macro definitions
*******************************************************************************/
#define BLEBRR_DEFAULT_ADV_HNDL             0x00        /* Advertising handle  : 0x00 */
#define BLEBRR_DEFAULT_DURATION             0x00        /* Advertising duration: unlimited */
#define BLEBRR_DEFAULT_MAX_EXT_ADV_EVTS     0x00        /* The maximum number of advertising events: unlimited */

#define BLEBRR_NCON_ADVINTMIN               0xA0        /* Min Advertising interval: 100[msec] */
#define BLEBRR_NCON_ADVINTMAX               0xA0        /* Max Advertising interval: 100[msec] */
#define BLEBRR_NCON_ADVTYPE                 BLE_GAP_LEGACY_PROP_ADV_NONCONN_IND
#define BLEBRR_NCON_ADDRTYPE                BLEBRR_VS_ADDR_TYPE
#define BLEBRR_NCON_ADVCHMAP                BLE_GAP_ADV_CH_ALL
#define BLEBRR_NCON_ADVFILTERPOLICY         BLE_GAP_ADV_ALLOW_SCAN_ANY_CONN_ANY
#define BLEBRR_ADV_DATALEN                  31          /* Max advertising data length: 31[bytes] */

#define BLEBRR_CON_ADVINTMIN                0xA0        /* Min Advertising interval: 100[msec] */
#define BLEBRR_CON_ADVINTMAX                0xA0        /* Max Advertising interval: 100[msec] */
#define BLEBRR_CON_ADVTYPE                  BLE_GAP_LEGACY_PROP_ADV_IND
#define BLEBRR_CON_ADDRTYPE                 BLEBRR_VS_ADDR_TYPE
#define BLEBRR_CON_ADVCHMAP                 BLE_GAP_ADV_CH_ALL
#define BLEBRR_CON_ADVFILTERPOLICY          BLE_GAP_ADV_ALLOW_SCAN_ANY_CONN_ANY
#define BLEBRR_CON_SCANRSP_DATALEN          31          /* Max scan response data length: 31[bytes] */

#define BLEBRR_SCAN_ADDRTYPE                BLEBRR_VS_ADDR_TYPE
#define BLEBRR_SCAN_PROCTYPE                BLE_GAP_SC_PROC_OBS
#define BLEBRR_SCAN_TYPE                    BLE_GAP_SCAN_PASSIVE
/**
 * NOTE: To avoid missing any incoming Advertising packets,
 * BLEBRR_SCAN_WINDOW should be equal to BLEBRR_SCAN_INTERVAL.
 */
#define BLEBRR_SCAN_INTERVAL                0x0060      /* Scan interval: 60[msec] */
#define BLEBRR_SCAN_WINDOW                  0x0060      /* Scan window  : 60[msec] */
#define BLEBRR_SCAN_FILTERPOLICY            BLE_GAP_SCAN_ALLOW_ADV_ALL
#define BLEBRR_SCAN_FILTERDUPS              BLE_GAP_SCAN_FILT_DUPLIC_DISABLE

#define BLEBRR_INIT_FILTERPOLICY            BLE_GAP_INIT_FILT_USE_ADDR
#define BLEBRR_INIT_ADDRTYPE                BLEBRR_VS_ADDR_TYPE
#define BLEBRR_INIT_SCANINTERVAL            0x0060      /* Scan interval: 60[msec] */
#define BLEBRR_INIT_SCANWINDOW              0x0060      /* Scan window  : 60[msec] */

#define BLEBRR_CONN_INTERVAL_MIN            0x0040      /* Min connection interval: 80[msec] */
#define BLEBRR_CONN_INTERVAL_MAX            0x0040      /* Max connection interval: 80[msec] */
#define BLEBRR_CONN_LATENCY                 0x0000      /* Slave latency          : 0[events] */
#define BLEBRR_CONN_SUPERVISION_TO          0x03BB      /* Supervision timeout    : 9550[msec] */
#define BLEBRR_CONN_MIN_CE_LEN              0xFFFF      /* Min connection event length: over 40[sec], virtually unlimited */
#define BLEBRR_CONN_MAX_CE_LEN              0xFFFF      /* Max connection event length: over 40[sec], virtually unlimited */

#define BLEBRR_GATT_MAX_ENV                 (MESH_CFG_NUM_NETWORK_INTERFACES - 1)

/*******************************************************************************
* Global Variables and Private Functions declaration
*******************************************************************************/
static UCHAR blebrr_adv_data[BLEBRR_ADV_DATALEN];
static UCHAR blebrr_scanrsp_data[BLEBRR_CON_SCANRSP_DATALEN];

static st_ble_gap_adv_param_t blebrr_noncon_adv_param_pl =
{
    .adv_hdl           = BLEBRR_DEFAULT_ADV_HNDL,
    .adv_prop_type     = BLEBRR_NCON_ADVTYPE,
    .adv_intv_min      = BLEBRR_NCON_ADVINTMIN,
    .adv_intv_max      = BLEBRR_NCON_ADVINTMAX,
    .adv_ch_map        = BLEBRR_NCON_ADVCHMAP,
    .o_addr_type       = BLEBRR_NCON_ADDRTYPE,
    .filter_policy     = BLEBRR_NCON_ADVFILTERPOLICY,
    .adv_phy           = BLE_GAP_ADV_PHY_1M,
    .scan_req_ntf_flag = BLE_GAP_SCAN_REQ_NTF_DISABLE,
};

static st_ble_gap_adv_param_t blebrr_con_adv_param_pl =
{
    .adv_hdl           = BLEBRR_DEFAULT_ADV_HNDL,
    .adv_prop_type     = BLEBRR_CON_ADVTYPE,
    .adv_intv_min      = BLEBRR_CON_ADVINTMIN,
    .adv_intv_max      = BLEBRR_CON_ADVINTMAX,
    .adv_ch_map        = BLEBRR_CON_ADVCHMAP,
    .o_addr_type       = BLEBRR_CON_ADDRTYPE,
    .filter_policy     = BLEBRR_CON_ADVFILTERPOLICY,
    .adv_phy           = BLE_GAP_ADV_PHY_1M,
    .scan_req_ntf_flag = BLE_GAP_SCAN_REQ_NTF_DISABLE,
};

static st_ble_gap_adv_data_t blebrr_adv_data_param_pl =
{
    .adv_hdl          = BLEBRR_DEFAULT_ADV_HNDL,
    .data_type        = BLE_GAP_ADV_DATA_MODE,
    .data_length      = 0,
    .p_data           = blebrr_adv_data,
    .zero_length_flag = BLE_GAP_DATA_0_CLEAR,
};

static st_ble_gap_adv_data_t blebrr_srsp_data_param_pl =
{
    .adv_hdl          = BLEBRR_DEFAULT_ADV_HNDL,
    .data_type        = BLE_GAP_SCAN_RSP_DATA_MODE,
    .data_length      = 0,
    .p_data           = blebrr_scanrsp_data,
    .zero_length_flag = BLE_GAP_DATA_0_CLEAR,
};

#if (BLE_CFG_LIB_TYPE == 0)
static st_ble_gap_adv_data_t blebrr_clear_srsp_data_param_pl =
{
    .adv_hdl          = BLEBRR_DEFAULT_ADV_HNDL,
    .data_type        = BLE_GAP_SCAN_RSP_DATA_MODE,
    .data_length      = 0,
    .p_data           = NULL,
    .zero_length_flag = BLE_GAP_DATA_0_CLEAR,
};
#endif /* (BLE_CFG_LIB_TYPE == 0) */

static st_ble_gap_scan_phy_param_t blebrr_scan_1m_phy_param_pl =
{
    .scan_type   = BLEBRR_SCAN_TYPE,
    .scan_intv   = BLEBRR_SCAN_INTERVAL,
    .scan_window = BLEBRR_SCAN_WINDOW,
};

static st_ble_gap_scan_param_t blebrr_scan_param_pl =
{
    .o_addr_type       = BLEBRR_SCAN_ADDRTYPE,
    .filter_policy     = BLEBRR_SCAN_FILTERPOLICY,
    .p_phy_param_1M    = &blebrr_scan_1m_phy_param_pl,
};

static st_ble_gap_scan_on_t blebrr_scan_enable_pl =
{
    .proc_type   = BLEBRR_SCAN_PROCTYPE,
    .filter_dups = BLEBRR_SCAN_FILTERDUPS,
    .duration    = 0x0000,
    .period      = 0x0000,
};

#if BLEBRR_GATT_CLIENT
static st_ble_gap_conn_param_t blebrr_conn_param_pl =
{
    .conn_intv_min = BLEBRR_CONN_INTERVAL_MIN,
    .conn_intv_max = BLEBRR_CONN_INTERVAL_MAX,
    .conn_latency  = BLEBRR_CONN_LATENCY,
    .sup_to        = BLEBRR_CONN_SUPERVISION_TO,
    .min_ce_length = BLEBRR_CONN_MIN_CE_LEN,
    .max_ce_length = BLEBRR_CONN_MAX_CE_LEN,
};

static st_ble_gap_conn_phy_param_t blebrr_conn_1m_phy_param_pl =
{
    .scan_intv    = BLEBRR_INIT_SCANINTERVAL,
    .scan_window  = BLEBRR_INIT_SCANWINDOW,
    .p_conn_param = &blebrr_conn_param_pl,
};

static st_ble_gap_create_conn_param_t blebrr_create_conn_param_pl =
{
    .init_filter_policy  = BLEBRR_INIT_FILTERPOLICY,
    .own_addr_type       = BLEBRR_INIT_ADDRTYPE,
    .p_conn_param_1M     = &blebrr_conn_1m_phy_param_pl,
    .remote_bd_addr      = {0}
};

static UCHAR blebrr_create_conn_client_mode;
#endif /* BLEBRR_GATT_CLIENT */

static blebrr_gatt_env_t blebrr_gatt_env[BLEBRR_GATT_MAX_ENV];

static BLEBRR_INIT_CB       blebrr_init_cb;
static BLEBRR_GATT_IFACE_CB blebrr_gatt_iface_pl_cb;

static UCHAR blebrr_advertise_type;
static UCHAR blebrr_is_scanrsp_data_set = MS_FALSE;

UCHAR blebrr_scanstate;
#if BLEBRR_GATT_CLIENT
static UCHAR blebrr_scanstate_gatt;
static UCHAR blebrr_scan_mode;
#endif /* BLEBRR_GATT_CLIENT */

static uint16_t appl_mesh_prov_data_out_ccd_cb(uint16_t conn_hdl, uint8_t enabled);
static uint16_t appl_mesh_prov_data_in_wt_cb(uint16_t conn_hdl, uint16_t length, uint8_t * value);
static uint16_t appl_mesh_proxy_data_out_ccd_cb(uint16_t conn_hdl, uint8_t enabled);
static uint16_t appl_mesh_proxy_data_in_wt_cb(uint16_t conn_hdl, uint16_t length, uint8_t * value);

static mesh_gatt_cb appl_mesh_prov_cb =
{
    .data_in_cb      = appl_mesh_prov_data_in_wt_cb,
    .data_out_ccd_cb = appl_mesh_prov_data_out_ccd_cb,
};

static mesh_gatt_cb appl_mesh_proxy_cb =
{
    .data_in_cb      = appl_mesh_proxy_data_in_wt_cb,
    .data_out_ccd_cb = appl_mesh_proxy_data_out_ccd_cb,
};

#if BLEBRR_GATT_CLIENT
static void mesh_prov_data_out_notif_cb(uint16_t conn_hdl, uint16_t length, uint8_t * value);
static void mesh_prov_notif_config_status_cb(uint16_t conn_hdl, uint8_t flag, uint8_t status);
static void mesh_prov_discovery_comp_cb(uint16_t conn_hdl, API_RESULT status);
static void mesh_prov_service_changed_cb(uint16_t conn_hdl);
static void mesh_proxy_data_out_notif_cb(uint16_t conn_hdl, uint16_t length, uint8_t * value);
static void mesh_proxy_notif_config_status_cb(uint16_t conn_hdl, uint8_t flag, uint8_t status);
static void mesh_proxy_discovery_comp_cb(uint16_t conn_hdl, API_RESULT status);
static void mesh_proxy_service_changed_cb(uint16_t conn_hdl);

static mesh_gatt_client_cb mesh_prov_callbacks =
{
    .data_out_notif = mesh_prov_data_out_notif_cb,
    .ntf_status     = mesh_prov_notif_config_status_cb,
    .disc_comp      = mesh_prov_discovery_comp_cb,
    .serv_changed   = mesh_prov_service_changed_cb,
};

static mesh_gatt_client_cb mesh_proxy_callbacks =
{
    .data_out_notif = mesh_proxy_data_out_notif_cb,
    .ntf_status     = mesh_proxy_notif_config_status_cb,
    .disc_comp      = mesh_proxy_discovery_comp_cb,
    .serv_changed   = mesh_proxy_service_changed_cb,
};
#endif /* BLEBRR_GATT_CLIENT */

static void blebrr_gap_cb(uint16_t type, ble_status_t result, st_ble_evt_data_t * data);
static void blebrr_vs_cb(uint16_t type, ble_status_t result, st_ble_vs_evt_data_t * data);

static st_ble_dev_addr_t blebrr_invald_dev_addr = {0};

/*******************************************************************************
* Functions
*******************************************************************************/
/***************************************************************************//**
* @brief Initializes BLE Protocol Stack
*******************************************************************************/
API_RESULT R_MS_BRR_Init(BLEBRR_INIT_CB init_cb)
{
    UINT16 idx;
    ble_status_t status;

    if (NULL == init_cb)
    {
        return API_FAILURE;
    }

    /* register callback to notify the completion of initializing BLE Bearer */
    blebrr_init_cb = init_cb;

    BLEBRR_LOG("Initializing with BLE Protocol Stack...\n");

    status = R_BLE_GAP_Init(blebrr_gap_cb);
    if (BLE_SUCCESS != status)
    {
        BLEBRR_LOG("R_BLE_GAP_Init(...) failed with status 0x%04X\n", status);
    }

    status = R_BLE_VS_Init(blebrr_vs_cb);
    if (BLE_SUCCESS != status)
    {
        BLEBRR_LOG("R_BLE_VS_Init(...) failed with status 0x%04X\n", status);
    }

    status = R_BLE_GATTS_Init(BLE_GATTS_MAX_CB);
    if (BLE_SUCCESS != status)
    {
        BLEBRR_LOG("R_BLE_GATTS_Init(...) failed with status 0x%04X\n", status);
    }

    /**
     * Since Mesh GATT Services use only Write Without Response and Notification,
     * registering Prepare Queue by R_BLE_GATTS_SetPrepareQueue() is not needed.
     */

    /* Initialize GATT Mesh Services */
    mesh_serv_init(2);

    /* Set Mode to GATT Prov Mode */
    blebrr_set_gattmode_pl(BLEBRR_GATT_PROV_MODE);

    #if BLEBRR_GATT_CLIENT
    status = R_BLE_GATTC_Init(BLE_GATTC_MAX_CB);
    if (status != BLE_SUCCESS)
    {
        BLEBRR_LOG("R_BLE_GATTC_Init is failed: 0x%04X\n", status);
    }

    /* Initialize GATT Mesh Client */
    mesh_client_init();

    blebrr_scanstate_gatt = MS_FALSE;
    #endif /* BLEBRR_GATT_CLIENT */

    for (idx = 0; idx < BLEBRR_GATT_MAX_ENV; idx++)
    {
        blebrr_gatt_env[idx].brr_hdl = BRR_HANDLE_INVALID;
        blebrr_gatt_env[idx].conn_hdl = BLE_GAP_INVALID_CONN_HDL;
        blebrr_gatt_env[idx].role = BLEBRR_INVALID_ROLE;
    }

    return API_SUCCESS;
}

blebrr_gatt_env_t * blebrr_find_gatt_env_by_connhdl(uint16_t conn_hdl)
{
    for (uint16_t idx = 0; idx < BLEBRR_GATT_MAX_ENV; idx++)
    {
        if (blebrr_gatt_env[idx].conn_hdl == conn_hdl)
        {
            return &blebrr_gatt_env[idx];
        }
    }
    BLEBRR_LOG("ERROR: Could not find gatt_env by conn_hdl\n");
    return NULL;
}

static blebrr_gatt_env_t * blebrr_find_gatt_env_by_brrhdl(BRR_HANDLE brr_hdl)
{
    for (uint16_t idx = 0; idx < BLEBRR_GATT_MAX_ENV; idx++)
    {
        if (blebrr_gatt_env[idx].brr_hdl == brr_hdl)
        {
            return &blebrr_gatt_env[idx];
        }
    }
    BLEBRR_LOG("ERROR: Could not find gatt_env by brr_hdl\n");
    return NULL;
}

/***************************************************************************//**
* @brief Starts or Stops Scan
*******************************************************************************/
API_RESULT blebrr_scan_pl(UCHAR enable)
{
    ble_status_t ret;

    /* Is request to enable? */
    if (MS_TRUE == enable)
    {
        /* Update global scan state */
        blebrr_scanstate = 0x01;

        /* Enable Scan */
        ret = R_BLE_GAP_StartScan
              (
                  &blebrr_scan_param_pl,
                  &blebrr_scan_enable_pl
              );

        if (BLE_SUCCESS != ret)
        {
            BLEBRR_LOG("R_BLE_GAP_StartScan is failed: 0x%04X\n", ret);
        }
    }
    else
    {
        /* Update global scan state */
        blebrr_scanstate = 0x00;

        /* Disable Scan */
        ret = R_BLE_GAP_StopScan();
        if (BLE_SUCCESS != ret)
        {
            BLEBRR_LOG("R_BLE_GAP_StopScan is failed: 0x%04X\n", ret);
        }
    }

    return (BLE_SUCCESS == ret) ? API_SUCCESS : API_FAILURE;
}

/***************************************************************************//**
* @brief Sets Scan Response Data to BLE Protocol Stack
*******************************************************************************/
API_RESULT R_MS_BRR_Set_ScanRspData(const UCHAR * srp_data, UCHAR srp_datalen)
{
    /* Initialize the global */
    blebrr_srsp_data_param_pl.data_length = 0;

    if (0 != srp_datalen)
    {
        if ((NULL != srp_data) && (BLEBRR_CON_SCANRSP_DATALEN >= srp_datalen))
        {
            /* Copy Scan Response Data to global */
            memcpy(blebrr_scanrsp_data, srp_data, srp_datalen);
            blebrr_srsp_data_param_pl.data_length = srp_datalen;
        }
        else
        {
            return API_FAILURE;
        }
    }

    return API_SUCCESS;
}

/***************************************************************************//**
* @brief Sets Advertising Data to BLE Protocol Stack
*******************************************************************************/
API_RESULT blebrr_advertise_data_pl(CHAR type, UCHAR * pdata, UINT16 pdatalen)
{
    ble_status_t ret;
    API_RESULT   retval = API_SUCCESS;

    /* Is request to enable? */
    if ((NULL != pdata) && (0 != pdatalen))
    {
        /*BLEBRR_LOG("blebrr_advertise_data_pl(): %s\n",
            (BRR_BCON_PASSIVE == type) ? "Non-connectable ADV" : "Connectable ADV");*/
        blebrr_advertise_type = type;

        /* Copy Advertising Data to global */
        memcpy(blebrr_adv_data, pdata, pdatalen);
        blebrr_adv_data_param_pl.data_length = pdatalen;

        #if (BLE_CFG_LIB_TYPE == 0)
        /* Was Scan Response data set just before transmitting Non-connectable Advertising? */
        if ((BRR_BCON_PASSIVE == type) && (MS_TRUE == blebrr_is_scanrsp_data_set))
        {
            /**
             * Invoke GAP API for Clearing Scan Response data
             */
            ret = R_BLE_GAP_SetAdvSresData(&blebrr_clear_srsp_data_param_pl);
            if (BLE_SUCCESS != ret)
            {
                BLEBRR_LOG("R_BLE_GAP_SetAdvSresData for Clear is failed: 0x%04X\n", ret);
            }
        }
        else
        #endif /* (BLE_CFG_LIB_TYPE == 0) */
        {
            /**
             * Invoke GAP API for Setting Non-connectable/Connectable Advertising Parameters
             */
            ret = R_BLE_GAP_SetAdvParam((BRR_BCON_PASSIVE == type) ?
                        &blebrr_noncon_adv_param_pl : &blebrr_con_adv_param_pl);
            if (BLE_SUCCESS != ret)
            {
                BLEBRR_LOG("R_BLE_GAP_SetAdvParam is failed: 0x%04X\n", ret);
            }
        }
    }
    else
    {
        /* Disable Advertising */
        retval = blebrr_advertise_pl(MS_FALSE);
    }

    return retval;
}

/***************************************************************************//**
* @brief Handles BLE_GAP_EVENT_ADV_PARAM_SET_COMP event
*******************************************************************************/
static void blebrr_adv_param_set_comp_hdlr(void)
{
    ble_status_t ret;

    /**
     * Invoke GAP API for Setting Advertising Data
     */
    ret = R_BLE_GAP_SetAdvSresData(&blebrr_adv_data_param_pl);
    if (BLE_SUCCESS != ret)
    {
        BLEBRR_LOG("R_BLE_GAP_SetAdvSresData for ADV is failed: 0x%04X\n", ret);
    }
}

/***************************************************************************//**
* @brief Handles BLE_GAP_EVENT_ADV_DATA_UPD_COMP event
*******************************************************************************/
static void blebrr_adv_data_upd_comp_hdlr(UCHAR data_type)
{
    ble_status_t ret;

    /* Is Scan Response data cleared? */
    if ((BRR_BCON_PASSIVE == blebrr_advertise_type) &&
        (MS_TRUE == blebrr_is_scanrsp_data_set))
    {
        blebrr_is_scanrsp_data_set = MS_FALSE;

        /**
         * Invoke GAP API for Setting Non-connectable Advertising Parameters
         */
        ret = R_BLE_GAP_SetAdvParam(&blebrr_noncon_adv_param_pl);
        if (BLE_SUCCESS != ret)
        {
            BLEBRR_LOG("R_BLE_GAP_SetAdvParam is failed: 0x%04X\n", ret);
        }
    }
    /* Is Scan Response data needed for Connectable Advertising? */
    else if ((BLE_GAP_ADV_DATA_MODE == data_type) &&
             (BRR_BCON_ACTIVE == blebrr_advertise_type) &&
             ((0 != blebrr_srsp_data_param_pl.data_length) || (MS_TRUE == blebrr_is_scanrsp_data_set)))
    {
        /**
         * Invoke GAP API for Setting Scan Response Data
         */
        ret = R_BLE_GAP_SetAdvSresData(&blebrr_srsp_data_param_pl);
        if (BLE_SUCCESS != ret)
        {
            BLEBRR_LOG("R_BLE_GAP_SetAdvSresData for SRSP is failed: 0x%04X\n", ret);
        }
    }
    else
    {
        #if (BLE_CFG_LIB_TYPE == 0)
        /* Is Scan Response data set? */
        if (BLE_GAP_SCAN_RSP_DATA_MODE == data_type)
        {
            blebrr_is_scanrsp_data_set = MS_TRUE;
        }
        #endif /* (BLE_CFG_LIB_TYPE == 0) */

        blebrr_advertise_pl(MS_TRUE);
    }
}

/***************************************************************************//**
* @brief Starts or Stops Advertisement
*******************************************************************************/
API_RESULT blebrr_advertise_pl(UCHAR state)
{
    ble_status_t ret;

    if (MS_TRUE == state)
    {
        ret = R_BLE_GAP_StartAdv
              (
                  BLEBRR_DEFAULT_ADV_HNDL,
                  BLEBRR_DEFAULT_DURATION,
                  BLEBRR_DEFAULT_MAX_EXT_ADV_EVTS
              );
        if (BLE_SUCCESS != ret)
        {
            BLEBRR_LOG("R_BLE_GAP_StartAdv is failed: 0x%04X\n", ret);
        }
    }
    else
    {
        ret = R_BLE_GAP_StopAdv(BLEBRR_DEFAULT_ADV_HNDL);
        if (BLE_SUCCESS != ret)
        {
            BLEBRR_LOG("R_BLE_GAP_StopAdv is failed: 0x%04X\n", ret);
        }
    }

    return (BLE_SUCCESS == ret) ? API_SUCCESS : API_FAILURE;
}

/***************************************************************************//**
* @brief Sends data over GATT Bearer
*******************************************************************************/
API_RESULT blebrr_gatt_send_pl(BRR_HANDLE * handle, UCHAR * data, UINT16 datalen)
{
    UCHAR      type;
    ble_status_t ret = BLE_ERR_INVALID_ARG;
    blebrr_gatt_env_t * gatt_env;

    gatt_env = blebrr_find_gatt_env_by_brrhdl(*handle);
    if (NULL == gatt_env)
    {
        return BLE_ERR_NOT_FOUND;
    }

    /* Get the current mode */
    /* TODO: See if we need to get specific mode for a GATT transport */
    type = R_MS_BRR_Get_GattMode();

    /* Check the PDU type received and Add bearer to Mesh stack */
    if (BLEBRR_GATT_PROV_MODE == type)
    {
        if (BLEBRR_SERVER_ROLE == gatt_env->role)
        {
            ret = mesh_prov_notify_data_out
                    (
                        gatt_env->conn_hdl,
                        BLE_MESH_PROVS_DATA_OUT_VAL_HDL,
                        data,
                        datalen
                    );
        }
        #if BLEBRR_GATT_CLIENT
        else
        {
            ret = mesh_prov_client_data_in_write(gatt_env->conn_hdl, data, datalen);
        }
        #endif /* BLEBRR_GATT_CLIENT */
    }
    else
    {
        if (BLEBRR_SERVER_ROLE == gatt_env->role)
        {
            ret = mesh_proxy_notify_data_out
                    (
                        gatt_env->conn_hdl,
                        BLE_MESH_PROXYS_DATA_OUT_VAL_HDL,
                        data,
                        datalen
                    );
        }
        #if BLEBRR_GATT_CLIENT
        else
        {
            ret = mesh_proxy_client_data_in_write(gatt_env->conn_hdl, data, datalen);
        }
        #endif /* BLEBRR_GATT_CLIENT */
    }

    if (BLE_SUCCESS != ret)
    {
        BLEBRR_LOG("GATT Send failed with reason 0x%04X, type:0x%02X role:0x%02X\n", ret, type, gatt_env->role);
    }

    return (BLE_SUCCESS == ret) ? API_SUCCESS : API_FAILURE;
}

/***************************************************************************//**
* @brief Sets up GATT Bearer Interface
*******************************************************************************/
static API_RESULT blebrr_gatt_com_channel_setup_pl
    (
        UINT16 conn_hdl,
        UCHAR  role,
        UCHAR  mode,
        UCHAR  evt
    )
{
    API_RESULT retval = API_FAILURE;
    UINT16 mtu;
    blebrr_gatt_env_t * gatt_env;

    /**
     * Possible Values of Role are
     *  1. 0x00 - GATT Client ~ BLEBRR_CLIENT_ROLE
     *  2. 0x01 - GATT Server ~ BLEBRR_SERVER_ROLE
     */

    /**
     * Possible Values of Mode are
     *  1. 0x00 - BLEBRR_GATT_PROV_MODE
     *  2. 0x01 - BLEBRR_GATT_PROXY_MODE
     */

    /**
     * Possible Values of evt are
     *  1. 0x00 - BLEBRR_COM_CHANNEL_CONNECT
     *  2. 0x01 - BLEBRR_COM_CHANNEL_DISCONNECT
     */

    if (BLEBRR_COM_CHANNEL_CONNECT == evt)
    {
        gatt_env = blebrr_find_gatt_env_by_connhdl(conn_hdl);
        if (NULL != gatt_env)
        {
            /* Store the GATT role */
            gatt_env->role = role;

            /* Adjust the MTU for 1 Bytes of ATT Opcode and 2 Bytes of Handle */
            /* NOTE: This MTU Size is used for GATT bearer communication by Mesh Stack,
             * so MTU Exchange needs to finish before Mesh GATT Service Notification is enabled.
             */
            mtu = mesh_serv_get_mtu(conn_hdl) - 3;

            if (BRR_HANDLE_INVALID == gatt_env->brr_hdl)
            {
                /* Add Device to the Bearer */
                retval = blebrr_pl_gatt_connection(&gatt_env->brr_hdl, role, mtu, mode);

                /* Check if valid Bearer Handle is returned */
                if (BRR_HANDLE_INVALID == gatt_env->brr_hdl)
                {
                    gatt_env = NULL;
                }
            }
        }

        if (NULL != blebrr_gatt_iface_pl_cb)
        {
            blebrr_gatt_iface_pl_cb
            (
                BLEBRR_GATT_IFACE_ENABLE,
                mode,
                (NULL != gatt_env) ? conn_hdl : BLE_GAP_INVALID_CONN_HDL,
                (NULL != gatt_env) ? &gatt_env->bd_addr : &blebrr_invald_dev_addr
            );
        }
    }
    else if (BLEBRR_COM_CHANNEL_DISCONNECT == evt)
    {
        /**
         * Currently BLE Bearer GATT Channel Disconnection
         * is called only from HCI/ACL link disconnection.
         */
        gatt_env = blebrr_find_gatt_env_by_connhdl(conn_hdl);
        if (NULL != gatt_env)
        {
            if (BRR_HANDLE_INVALID != gatt_env->brr_hdl)
            {
                /* Delete Device from the Bearer */
                retval = blebrr_pl_gatt_disconnection(&gatt_env->brr_hdl, gatt_env->role, mode);
                gatt_env->brr_hdl = BRR_HANDLE_INVALID;
            }
        }

        if (NULL != blebrr_gatt_iface_pl_cb)
        {
            blebrr_gatt_iface_pl_cb
            (
                BLEBRR_GATT_IFACE_DISABLE,
                mode,
                conn_hdl,
                (NULL != gatt_env) ? &gatt_env->bd_addr : &blebrr_invald_dev_addr
            );
        }
    }
    else
    {
        /* Empty */
    }

    return retval;
}

static API_RESULT appl_mesh_prov_data_out_ccd_cb(uint16_t conn_hdl, uint8_t enabled)
{
    /* Check the Current mode is not PROV */
    if (BLEBRR_GATT_PROV_MODE != R_MS_BRR_Get_GattMode())
    {
        BLEBRR_LOG("Mesh Prov Out CCD being Written when PROV is not Active!\n");
        return API_FAILURE;
    }

    BLEBRR_LOG("Mesh Prov Out CCD %s\n", (MS_TRUE == enabled) ? "Enabled" : "Disabled");

    blebrr_gatt_com_channel_setup_pl
    (
        conn_hdl,
        BLEBRR_SERVER_ROLE,
        BLEBRR_GATT_PROV_MODE,
        (enabled) ? BLEBRR_COM_CHANNEL_CONNECT : BLEBRR_COM_CHANNEL_DISCONNECT
    );

    return API_SUCCESS;
}

static API_RESULT appl_mesh_prov_data_in_wt_cb(uint16_t conn_hdl, uint16_t length, uint8_t * value)
{
    blebrr_gatt_env_t * gatt_env;

    gatt_env = blebrr_find_gatt_env_by_connhdl(conn_hdl);
    if ((NULL == gatt_env) || (BRR_HANDLE_INVALID == gatt_env->brr_hdl))
    {
        BLEBRR_LOG("ERROR: failed to inform mesh stack of Provisioning Data by WWR\n");
        return API_FAILURE;
    }

    if (NULL != value)
    {
        blebrr_pl_recv_gattpacket
        (
            &gatt_env->brr_hdl,
            value,
            length
        );
    }

    return API_SUCCESS;
}

static API_RESULT appl_mesh_proxy_data_out_ccd_cb(uint16_t conn_hdl, uint8_t enabled)
{
    /* Check the Current mode is not PROV */
    if (BLEBRR_GATT_PROXY_MODE != R_MS_BRR_Get_GattMode())
    {
        BLEBRR_LOG("Mesh Proxy Out CCD being Written when PROXY is not Active!\n");
        return API_FAILURE;
    }

    BLEBRR_LOG("Mesh Proxy Out CCD %s\n", (MS_TRUE == enabled) ? "Enabled" : "Disabled");

    blebrr_gatt_com_channel_setup_pl
    (
        conn_hdl,
        BLEBRR_SERVER_ROLE,
        BLEBRR_GATT_PROXY_MODE,
        (enabled) ? BLEBRR_COM_CHANNEL_CONNECT : BLEBRR_COM_CHANNEL_DISCONNECT
    );

    return API_SUCCESS;
}

static API_RESULT appl_mesh_proxy_data_in_wt_cb(uint16_t conn_hdl, uint16_t length, uint8_t * value)
{
    blebrr_gatt_env_t * gatt_env;

    gatt_env = blebrr_find_gatt_env_by_connhdl(conn_hdl);
    if ((NULL == gatt_env) || (BRR_HANDLE_INVALID == gatt_env->brr_hdl))
    {
        BLEBRR_LOG("ERROR: failed to inform mesh stack of Proxy Data by WWR\n");
        return API_FAILURE;
    }

    if (NULL != value)
    {
        blebrr_pl_recv_gattpacket
        (
            &gatt_env->brr_hdl,
            value,
            length
        );
    }

    return API_SUCCESS;
}

#if BLEBRR_GATT_CLIENT
static void mesh_gatt_expand_mtu_cb(uint16_t conn_hdl)
{
    /* Start Service Discovery */
    R_MS_BRR_Discover_Service(conn_hdl, blebrr_create_conn_client_mode);
}

static void mesh_prov_data_out_notif_cb(uint16_t conn_hdl, uint16_t length, uint8_t * value)
{
    blebrr_gatt_env_t * gatt_env;

    gatt_env = blebrr_find_gatt_env_by_connhdl(conn_hdl);
    if ((NULL != gatt_env) && (BRR_HANDLE_INVALID != gatt_env->brr_hdl))
    {
        blebrr_pl_recv_gattpacket
        (
            &gatt_env->brr_hdl,
            (UCHAR *)value,
            length
        );
    }
    else
    {
        BLEBRR_LOG("ERROR: failed to inform mesh stack of Provisioning Data by Notification\n");
    }
}

static void mesh_prov_notif_config_status_cb(uint16_t conn_hdl, uint8_t flag, uint8_t status)
{
    if (status == 0x00)
    {
        BLEBRR_LOG("Mesh Provisioning Data Out notifications %s\n",
            flag ?  "enabled" : "disabled");

        R_MS_BRR_Set_GattMode(BLEBRR_GATT_PROV_MODE);
        blebrr_gatt_com_channel_setup_pl
        (
            conn_hdl,
            BLEBRR_CLIENT_ROLE,
            BLEBRR_GATT_PROV_MODE,
            (flag)? BLEBRR_COM_CHANNEL_CONNECT : BLEBRR_COM_CHANNEL_DISCONNECT
        );
    }
    else
    {
        BLEBRR_LOG("ERROR: failed to set notifications (0x%02x)\n", status);
    }
}

/***************************************************************************//**
* @brief Callback function to receive Mesh Provisioning Service Discovery completion
*******************************************************************************/
static void mesh_prov_discovery_comp_cb(uint16_t conn_hdl, API_RESULT status)
{
    blebrr_gatt_env_t * gatt_env;

    BLEBRR_LOG("Discovery Process Completed !!!\n");
    if (API_SUCCESS == status)
    {
        R_MS_BRR_Config_Notification(conn_hdl, MS_TRUE, BLEBRR_GATT_PROV_MODE);
    }

    gatt_env = blebrr_find_gatt_env_by_connhdl(conn_hdl);
    blebrr_gatt_iface_pl_cb
    (
        (API_SUCCESS == status) ? BLEBRR_GATT_IFACE_FOUND : BLEBRR_GATT_IFACE_NOT_FOUND,
        BLEBRR_GATT_PROV_MODE,
        conn_hdl,
        (NULL != gatt_env) ? &gatt_env->bd_addr : &blebrr_invald_dev_addr
    );
}

static void mesh_proxy_data_out_notif_cb(uint16_t conn_hdl, uint16_t length, uint8_t * value)
{
    blebrr_gatt_env_t * gatt_env;

    gatt_env = blebrr_find_gatt_env_by_connhdl(conn_hdl);
    if ((NULL != gatt_env) && (BRR_HANDLE_INVALID != gatt_env->brr_hdl))
    {
        blebrr_pl_recv_gattpacket
        (
            &gatt_env->brr_hdl,
            (UCHAR *)value,
            length
        );
    }
    else
    {
        BLEBRR_LOG("ERROR: failed to inform mesh stack of Proxy Data by Notification\n");
    }
}

static void mesh_proxy_notif_config_status_cb(uint16_t conn_hdl, uint8_t flag, uint8_t status)
{
    if (status == 0x00)
    {
        BLEBRR_LOG("Mesh Proxy Data Out notifications %s\n",
            flag ?  "enabled" : "disabled");

        R_MS_BRR_Set_GattMode(BLEBRR_GATT_PROXY_MODE);
        blebrr_gatt_com_channel_setup_pl
        (
            conn_hdl,
            BLEBRR_CLIENT_ROLE,
            BLEBRR_GATT_PROXY_MODE,
            (flag)? BLEBRR_COM_CHANNEL_CONNECT : BLEBRR_COM_CHANNEL_DISCONNECT
        );
    }
    else
    {
        BLEBRR_LOG("ERROR: failed to set notifications (0x%02x)\n", status);
    }
}

/***************************************************************************//**
* @brief Callback function to receive Mesh Proxy Service Discovery completion
*******************************************************************************/
static void mesh_proxy_discovery_comp_cb(uint16_t conn_hdl, API_RESULT status)
{
    blebrr_gatt_env_t * gatt_env;

    BLEBRR_LOG("Discovery Process Completed !!!\n");
    if (API_SUCCESS == status)
    {
        R_MS_BRR_Config_Notification(conn_hdl, MS_TRUE, BLEBRR_GATT_PROXY_MODE);
    }

    gatt_env = blebrr_find_gatt_env_by_connhdl(conn_hdl);
    blebrr_gatt_iface_pl_cb
    (
        (API_SUCCESS == status) ? BLEBRR_GATT_IFACE_FOUND : BLEBRR_GATT_IFACE_NOT_FOUND,
        BLEBRR_GATT_PROXY_MODE,
        conn_hdl,
        (NULL != gatt_env) ? &gatt_env->bd_addr : &blebrr_invald_dev_addr
    );
}

/***************************************************************************//**
* @brief Callback function to receive Mesh GATT Service Changed Indication received
*******************************************************************************/
static void mesh_prov_service_changed_cb(uint16_t conn_hdl)
{
    blebrr_gatt_env_t * gatt_env;

    BLEBRR_LOG("Service Changed Indication received !!!\n");

    gatt_env = blebrr_find_gatt_env_by_connhdl(conn_hdl);
    blebrr_gatt_iface_pl_cb
    (
        BLEBRR_GATT_IFACE_CHANGED,
        BLEBRR_GATT_PROV_MODE,
        conn_hdl,
        (NULL != gatt_env) ? &gatt_env->bd_addr : &blebrr_invald_dev_addr
    );
}

/***************************************************************************//**
* @brief Callback function to receive Mesh GATT Service Changed Indication received
*******************************************************************************/
static void mesh_proxy_service_changed_cb(uint16_t conn_hdl)
{
    blebrr_gatt_env_t * gatt_env;

    BLEBRR_LOG("Service Changed Indication received !!!\n");

    gatt_env = blebrr_find_gatt_env_by_connhdl(conn_hdl);
    blebrr_gatt_iface_pl_cb
    (
        BLEBRR_GATT_IFACE_CHANGED,
        BLEBRR_GATT_PROXY_MODE,
        conn_hdl,
        (NULL != gatt_env) ? &gatt_env->bd_addr : &blebrr_invald_dev_addr
    );
}
#endif /* BLEBRR_GATT_CLIENT */

/***************************************************************************//**
* @brief Registers GATT Bearer Interface Callback function to Bearer
*******************************************************************************/
API_RESULT R_MS_BRR_Register_GattIfaceCallback(BLEBRR_GATT_IFACE_CB cb)
{
    if (NULL != cb)
    {
        blebrr_gatt_iface_pl_cb = cb;

        BLEBRR_LOG("\nRegistered GATT Bearer Iface Events Appl Callback!\n");
        return API_SUCCESS;
    }

    /* If NULL callback is registered */
    return API_FAILURE;
}

/***************************************************************************//**
* @brief Scan Connectable Advertising including Mesh GATT Service
*******************************************************************************/
API_RESULT R_MS_BRR_Scan_GattBearer(UCHAR enable, UCHAR mode)
{
    #if BLEBRR_GATT_CLIENT
    API_RESULT retval = API_SUCCESS;

    if ((MS_TRUE == enable) && (0x00 == blebrr_scanstate))
    {
        /* Scan is not working */
        retval = API_FAILURE;
    }
    else
    {
        blebrr_scanstate_gatt = enable;
        blebrr_scan_mode = mode;
    }

    return retval;
    #else /* BLEBRR_GATT_CLIENT */
    return API_FAILURE;
    #endif /* BLEBRR_GATT_CLIENT */
}

#if BLEBRR_GATT_CLIENT
/***************************************************************************//**
* @brief Check if Advertising Data includes Mesh GATT Service
*******************************************************************************/
static API_RESULT blebrr_scan_find_gatt_pl(UINT8 * adv_data, UINT8 adv_datalen, UCHAR mode)
{
    UINT16  service = (BLEBRR_GATT_PROV_MODE == mode) ?
                        MESH_PROVISIONING_SERVICE : MESH_PROXY_SERVICE;
    UINT8 * data = adv_data;
    UINT8   type;
    UINT8   len;

    while (0 != adv_datalen)
    {
        /* Extract AD Length and AD Type */
        len = data[0] + 1;
        type = data[1];
        if ((0 == len) || (len > adv_datalen))
        {
            break;
        }

        /* Check if AD Type is <<Incomplete List of 16-bit Service UUIDs>> or
         * <<Complete List of 16-bit Service UUIDs>> */
        if ((0x02 == type) || (0x03 == type))
        {
            for (UINT8 offset = 2; offset < len; offset += sizeof(UINT16))
            {
                /* Check if there is Mesh GATT Service in the Service List */
                if ((data[offset + 0] == (service & 0xFF)) &&
                    (data[offset + 1] == ((service >> 8) & 0xFF)))
                {
                    /* Mesh GATT Service is found */
                    return API_SUCCESS;
                }
            }
        }
        /* Forward the data pointer */
        data = &data[len];
        adv_datalen -= len;
    }

    /* Mesh GATT Service is not found */
    return API_FAILURE;
}
#endif /* BLEBRR_GATT_CLIENT */

/***************************************************************************//**
* @brief Creates a connection for GATT Bearer
*******************************************************************************/
API_RESULT R_MS_BRR_Create_Connection(st_ble_dev_addr_t * remote_addr, UCHAR mode)
{
    #if BLEBRR_GATT_CLIENT
    ble_status_t ret;

    memcpy(blebrr_create_conn_param_pl.remote_bd_addr, remote_addr->addr, BLE_BD_ADDR_LEN);
    blebrr_create_conn_param_pl.remote_bd_addr_type = remote_addr->type;
    blebrr_create_conn_client_mode = mode;

    ret = R_BLE_GAP_CreateConn(&blebrr_create_conn_param_pl);
    if (BLE_SUCCESS != ret)
    {
        BLEBRR_LOG("R_BLE_GAP_CreateConn is failed: 0x%04X\n", ret);
        return API_FAILURE;
    }
    return API_SUCCESS;
    #else /* BLEBRR_GATT_CLIENT */
    return API_FAILURE;
    #endif /* BLEBRR_GATT_CLIENT */
}

/***************************************************************************//**
* @brief Cancels creating a connection
*******************************************************************************/
API_RESULT R_MS_BRR_Cancel_CreateConnection(void)
{
    #if BLEBRR_GATT_CLIENT
    ble_status_t ret;

    ret = R_BLE_GAP_CancelCreateConn();
    if (BLE_SUCCESS != ret)
    {
        BLEBRR_LOG("R_BLE_GAP_CancelCreateConn is failed: 0x%04X\n", ret);
        return API_FAILURE;
    }
    return API_SUCCESS;
    #else /* BLEBRR_GATT_CLIENT */
    return API_FAILURE;
    #endif /* BLEBRR_GATT_CLIENT */
}

/***************************************************************************//**
* @brief Disconnects
*******************************************************************************/
API_RESULT R_MS_BRR_Disconnect(UINT16 conn_hdl)
{
    ble_status_t ret;

    /* Disconnect with reason REMOTE USER TERMINATED CONNECTION (0x13) */
    ret = R_BLE_GAP_Disconnect(conn_hdl, 0x13);
    if (BLE_SUCCESS != ret)
    {
        BLEBRR_LOG("R_BLE_GAP_Disconnect is failed: 0x%04X\n", ret);
        return API_FAILURE;
    }

    return API_SUCCESS;
}

/***************************************************************************//**
* @brief Start Mesh Service Discovery
*******************************************************************************/
API_RESULT R_MS_BRR_Discover_Service(UINT16 conn_hdl, UCHAR mode)
{
    #if BLEBRR_GATT_CLIENT
    /* Set the mode with bearer and Register the corresponding Callback */
    if (BLEBRR_GATT_PROV_MODE == mode)
    {
        R_MS_BRR_Set_GattMode(BLEBRR_GATT_PROV_MODE);
        mesh_client_prov_init(&mesh_prov_callbacks);
    }
    else
    {
        R_MS_BRR_Set_GattMode(BLEBRR_GATT_PROXY_MODE);
        mesh_client_proxy_init(&mesh_proxy_callbacks);
    }

    return (BLE_SUCCESS == mesh_client_discover_services(conn_hdl, mode)) ? API_SUCCESS : API_FAILURE;
    #else /* BLEBRR_GATT_CLIENT */
    BLEBRR_LOG("\nBLEBRR_GATT_CLIENT Disabled!\n");
    return API_FAILURE;
    #endif /* BLEBRR_GATT_CLIENT */
}

/***************************************************************************//**
* @brief Enables or Disables Mesh Service Notification
*******************************************************************************/
API_RESULT R_MS_BRR_Config_Notification(UINT16 conn_hdl, UCHAR config_ntf, UCHAR mode)
{
    #if BLEBRR_GATT_CLIENT
    /* Set the mode with bearer */
    if (mode == BLEBRR_GATT_PROV_MODE)
    {
        R_MS_BRR_Set_GattMode(BLEBRR_GATT_PROV_MODE);
    }
    else
    {
        R_MS_BRR_Set_GattMode(BLEBRR_GATT_PROXY_MODE);
    }

    return  (BLE_SUCCESS == mesh_client_config_ntf(conn_hdl, config_ntf, mode)) ? API_SUCCESS : API_FAILURE;
    #else /* BLEBRR_GATT_CLIENT */
    BLEBRR_LOG("\nBLEBRR_GATT_CLIENT Disabled!\n");
    return API_FAILURE;
    #endif /* BLEBRR_GATT_CLIENT */
}

/***************************************************************************//**
* @brief Enables or Disables GATT Service Changed Indication
*******************************************************************************/
API_RESULT R_MS_BRR_Config_ServChanged(UINT16 conn_hdl, UCHAR config_ind)
{
    #if BLEBRR_GATT_CLIENT
    return  (BLE_SUCCESS == mesh_client_config_serv_changed(conn_hdl, config_ind)) ? API_SUCCESS : API_FAILURE;
    #else /* BLEBRR_GATT_CLIENT */
    BLEBRR_LOG("\nBLEBRR_GATT_CLIENT Disabled!\n");
    return API_FAILURE;
    #endif /* BLEBRR_GATT_CLIENT */
}

/***************************************************************************//**
* @brief Sets Mesh GATT Service Mode
*******************************************************************************/
API_RESULT blebrr_set_gattmode_pl(UCHAR serv_mode)
{
    ble_status_t ret = BLE_ERR_INVALID_ARG;

    /* Setting Provisioning or Proxy Mode */
    if (BLEBRR_GATT_PROV_MODE == serv_mode)
    {
        BLEBRR_LOG ("Enabling Mesh Provisioning Service...\n");
        if (BLE_SUCCESS == mesh_serv_proxy_deinit())
        {
            ret = mesh_serv_prov_init(&appl_mesh_prov_cb);
        }
    }
    else if (BLEBRR_GATT_PROXY_MODE == serv_mode)
    {
        BLEBRR_LOG ("Enabling Mesh Proxy Service...\n");
        if (BLE_SUCCESS == mesh_serv_prov_deinit())
        {
            ret = mesh_serv_proxy_init(&appl_mesh_proxy_cb);
        }
    }

    /**
     * 7.1 "SERVICE CHANGED" in Bluetooth Core Vol3, PartG:
     *  The <<Service Changed>> characteristic is a control-point attribute that 
     *  shall be used to indicate to connected devices that services have changed
     *  (i.e., added, removed or modified).
     */
    if (BLE_SUCCESS == ret)
    {
        for (uint8_t idx = 0; idx < BLEBRR_GATT_MAX_ENV; idx++)
        {
            if (BLE_GAP_INVALID_CONN_HDL != blebrr_gatt_env[idx].conn_hdl)
            {
                ret = mesh_indicate_serv_changed(blebrr_gatt_env[idx].conn_hdl);
            }
        }
    }

    return (BLE_SUCCESS == ret) ? API_SUCCESS : API_FAILURE;
}

/***************************************************************************//**
* @brief Handles Connection Parameter Update Request
*******************************************************************************/
void blebrr_handle_conn_param_req_pl
     (
         UINT16 conn_hdl,
         UINT16 min_conn_int,
         UINT16 max_conn_int,
         UINT16 conn_latency,
         UINT16 sup_timeout
     )
{
    ble_status_t ret;

    /**
     * NOTE:
     * Ideally, the application might not be aware of how many concurrent
     * connections are present, and if the parameters requested by remote
     * device are suitable or not.
     * The application should choose to have better parameters for connection
     *
     * If BLEBRR_PL_REUSE_PEER_CONN_PARAMS Compilation Flag is defined,
     * then application replies with the incoming connection parameters sent
     * bt the Peer.
     * Else,
     * The default behaviour is to reject any incoming connection parameter
     * update request with a 'Negative Reply' with 'Unnaceptable Connection
     * interval' as the reason.
     * This is because, it is seen during various Mobile App Interoperability
     * that the Public Key Generation from the Crypto ECDH module and the
     * Connection parameters might clash and there is frequent disconnection
     * of the link.
     */
#ifdef BLEBRR_PL_REUSE_PEER_CONN_PARAMS
    st_ble_gap_conn_param_t conn_updt_param =
    {
        .conn_intv_min = min_conn_int,
        .conn_intv_max = max_conn_int,
        .conn_latency  = conn_latency,
        .sup_to        = sup_timeout,
    };

    /* Accept the Connection Update */
    ret = R_BLE_GAP_UpdConn
          (
              conn_hdl,
              BLE_GAP_CONN_UPD_MODE_RSP,
              BLE_GAP_CONN_UPD_ACCEPT,
              &conn_updt_param
          );
#else /* BLEBRR_PL_REUSE_PEER_CONN_PARAMS */
    /* Reject the Connection Update */
    ret = R_BLE_GAP_UpdConn
          (
              conn_hdl,
              BLE_GAP_CONN_UPD_MODE_RSP,
              BLE_GAP_CONN_UPD_REJECT,
              NULL
          );
#endif /* BLEBRR_PL_REUSE_PEER_CONN_PARAMS */
    if (BLE_SUCCESS != ret)
    {
        BLEBRR_LOG("R_BLE_GAP_UpdConn is failed: 0x%04X\n", ret);
    }
}

/***************************************************************************//**
* @brief callback function to receive GAP(Generic Access Profile) events from BLE Protocol Stack
*******************************************************************************/
static void blebrr_gap_cb(uint16_t type, ble_status_t result, st_ble_evt_data_t * data)
{
    blebrr_gatt_env_t * gatt_env;
    st_ble_dev_addr_t   peer_addr;

    switch (type)
    {
        /**
         *  BLE Protocol Stack has been initialized by invoking R_BLE_GAP_Init()
         */
        case BLE_GAP_EVENT_STACK_ON:
        {
            BLEBRR_LOG("\nReceived BLE_GAP_EVENT_STACK_ON\n");

            /* Request the Device Address and wait for VS Callback (blebrr_vs_cb) */
            R_BLE_VS_GetBdAddr(BLE_VS_ADDR_AREA_REG, BLEBRR_VS_ADDR_TYPE);
        }
        break;

        /**
         * Equivalent to HCI LE Connection Complete event
         *  or HCI LE Enhanced Connection Complete event
         */
        case BLE_GAP_EVENT_CONN_IND:
        {
            BLEBRR_LOG("\nReceived BLE_GAP_EVENT_CONN_IND\n");

            if (result == BLE_SUCCESS)
            {
                st_ble_gap_conn_evt_t *conn_evt_param =
                    (st_ble_gap_conn_evt_t *)data->p_param;

                switch (conn_evt_param->remote_addr_type)
                {
                    case BLE_GAP_ADDR_PUBLIC:
                    {
                        memcpy(peer_addr.addr, conn_evt_param->remote_addr, BLE_BD_ADDR_LEN);
                        peer_addr.type = BLE_GAP_ADDR_PUBLIC;
                    } break;

                    case BLE_GAP_ADDR_RAND:
                    {
                        memcpy(peer_addr.addr, conn_evt_param->remote_addr, BLE_BD_ADDR_LEN);
                        peer_addr.type = BLE_GAP_ADDR_RAND;
                    } break;

                    case BLE_GAP_ADDR_RPA_ID_PUBLIC:
                    case BLE_GAP_ADDR_RPA_ID_RANDOM:
                    {
                        memcpy(peer_addr.addr, conn_evt_param->remote_rpa, BLE_BD_ADDR_LEN);
                        peer_addr.type = BLE_GAP_ADDR_RAND;
                    } break;
                }

                BLEBRR_LOG("gap: connected conn_hdl:0x%04X, addr:%s\n",
                    conn_evt_param->conn_hdl,
                    BLE_BD_ADDR_STR(peer_addr.addr, peer_addr.type));

                gatt_env = blebrr_find_gatt_env_by_connhdl(BLE_GAP_INVALID_CONN_HDL);
                if (NULL != gatt_env)
                {
                    /* Add the Device Information to the GATT Interface Table */
                    gatt_env->conn_hdl = conn_evt_param->conn_hdl;
                    memcpy(&gatt_env->bd_addr, &peer_addr, sizeof(st_ble_dev_addr_t));

                    /**
                     * Inform Application of GATT/BLE Link Layer Connection.
                     */
                    if (NULL != blebrr_gatt_iface_pl_cb)
                    {
                        blebrr_gatt_iface_pl_cb
                        (
                            BLEBRR_GATT_IFACE_UP,
                            R_MS_BRR_Get_GattMode(),
                            (NULL != gatt_env) ? conn_evt_param->conn_hdl : BLE_GAP_INVALID_CONN_HDL,
                            (NULL != gatt_env) ? &peer_addr : &blebrr_invald_dev_addr
                        );
                    }

                    #if BLEBRR_GATT_CLIENT
                    if (BLE_MASTER == conn_evt_param->role)
                    {
                        /* Clear the device address set to the argument of R_MS_BRR_Create_Connection() */
                        memset(blebrr_create_conn_param_pl.remote_bd_addr, 0, BLE_BD_ADDR_LEN);

                        /* Request the peer GATT Server to expand MTU size */
                        mesh_client_expand_mtu(conn_evt_param->conn_hdl, mesh_gatt_expand_mtu_cb);
                    }
                    #endif /* BLEBRR_GATT_CLIENT */
                }
                else
                {
                    /* Disconnect because the number of connection established exceeds to BLEBRR_GATT_MAX_ENV */
                    R_MS_BRR_Disconnect(conn_evt_param->conn_hdl);
                }
            }
        }
        break;

        /**
         * Equivalent to HCI Disconnection Complete event
         */
        case BLE_GAP_EVENT_DISCONN_IND:
        {
            st_ble_gap_disconn_evt_t *disc_evt_param =
                (st_ble_gap_disconn_evt_t *)data->p_param;

            BLEBRR_LOG("\nReceived BLE_GAP_EVENT_DISCONN_IND\n");

            gatt_env = blebrr_find_gatt_env_by_connhdl(disc_evt_param->conn_hdl);
            BLEBRR_LOG("gap: disconnected conn_hdl:0x%04X, addr:%s with reason:0x%02X\n",
                disc_evt_param->conn_hdl,
                (NULL != gatt_env) ? BLE_BD_ADDR_STR(gatt_env->bd_addr.addr, gatt_env->bd_addr.type) : "--",
                disc_evt_param->reason);

            if (NULL != gatt_env)
            {
                if (BRR_HANDLE_INVALID != gatt_env->brr_hdl)
                {
                    /* Remove Device from the Bearer */
                    blebrr_pl_gatt_disconnection(&gatt_env->brr_hdl, gatt_env->role, R_MS_BRR_Get_GattMode());
                    gatt_env->brr_hdl = BRR_HANDLE_INVALID;
                }

                /* Remove the Device Information from the GATT Interface Table */
                gatt_env->conn_hdl = BLE_GAP_INVALID_CONN_HDL;
                gatt_env->role = BLEBRR_INVALID_ROLE;
            }

            /**
             * Inform Application of GATT/BLE Link Layer Disconnection.
             */
            if (NULL != blebrr_gatt_iface_pl_cb)
            {
                blebrr_gatt_iface_pl_cb
                (
                    BLEBRR_GATT_IFACE_DOWN,
                    R_MS_BRR_Get_GattMode(),
                    disc_evt_param->conn_hdl,
                    (NULL != gatt_env) ? &gatt_env->bd_addr : &blebrr_invald_dev_addr
                );
            }
        }
        break;

        case BLE_GAP_EVENT_CONN_CANCEL_COMP:
        {
            #if BLEBRR_GATT_CLIENT
            st_ble_dev_addr_t cancel_addr;
            cancel_addr.type = blebrr_create_conn_param_pl.remote_bd_addr_type;
            memcpy(cancel_addr.addr, blebrr_create_conn_param_pl.remote_bd_addr, BLE_BD_ADDR_LEN);

            /**
             * Inform Application of GATT/BLE Link Layer Canceling Connection Creation.
             */
            blebrr_gatt_iface_pl_cb
            (
                BLEBRR_GATT_IFACE_CANCEL,
                BLEBRR_GATT_UNINIT_MODE,
                BLE_GAP_INVALID_CONN_HDL,
                &cancel_addr
            );
            #endif /* BLEBRR_GATT_CLIENT */
        }
        break;

        /**
         * Equivalent to HCI LE Remote Connection Parameter Request event
         *  or L2CAP Connection Parameter Update Request
         */
        case BLE_GAP_EVENT_CONN_PARAM_UPD_REQ:
        {
            st_ble_gap_conn_upd_req_evt_t *conn_upd_req_evt_param =
                (st_ble_gap_conn_upd_req_evt_t *)data->p_param;

            BLEBRR_LOG("\nReceived BLE_GAP_EVENT_CONN_PARAM_UPD_REQ\n");

            blebrr_handle_conn_param_req_pl
            (
                conn_upd_req_evt_param->conn_hdl,
                conn_upd_req_evt_param->conn_intv_min,
                conn_upd_req_evt_param->conn_intv_max,
                conn_upd_req_evt_param->conn_latency,
                conn_upd_req_evt_param->sup_to
            );
        }
        break;

        /**
         * Scanning has been enabled by invoking R_BLE_GAP_StartScan()
         */
        case BLE_GAP_EVENT_SCAN_ON:
        {
            blebrr_pl_scan_setup(blebrr_scanstate);
        }
        break;

        /**
         * Scanning has been disabled by invoking R_BLE_GAP_StopScan()
         */
        case BLE_GAP_EVENT_SCAN_OFF:
        {
            blebrr_pl_scan_setup(blebrr_scanstate);
        }
        break;

        /**
          * Equivalent to HCI LE Advertising Report event or other advertising reports
          */
        case BLE_GAP_EVENT_ADV_REPT_IND:
        {
            st_ble_gap_adv_rept_evt_t *adv_rept_evt_param =
                (st_ble_gap_adv_rept_evt_t *)data->p_param;

            /* Check Report Type */
            /**
             * NOTE: If the BLE Protocol Stack library type is "all features",
             * Legacy Advertising packet is also reported by a Extended Advertising Report (adv_rpt_type=1).
             */
            #if (BLE_CFG_LIB_TYPE == 0)
            if (0x01 == adv_rept_evt_param->adv_rpt_type)
            #else /* (BLE_CFG_LIB_TYPE == 0) */
            if (0x00 == adv_rept_evt_param->adv_rpt_type)
            #endif /* (BLE_CFG_LIB_TYPE == 0) */
            {
                #if (BLE_CFG_LIB_TYPE == 0)
                st_ble_gap_ext_adv_rept_t *adv_rept_param =
                    (st_ble_gap_ext_adv_rept_t *)adv_rept_evt_param->param.p_ext_adv_rpt;
                #else
                st_ble_gap_adv_rept_t *adv_rept_param =
                    (st_ble_gap_adv_rept_t *)adv_rept_evt_param->param.p_adv_rpt;
                #endif

                /** NOTE: Although each Advertising Report structure has the "num", BLE Protocol Stack always reports event with num=1. */
                switch (adv_rept_param->adv_type)
                {
                    /* Non-connectable and non-scannable undirected Legacy Advertising Packet */
                    case BLE_GAP_LEGACY_PROP_ADV_NONCONN_IND:
                    {
                        /* Give packet to the bearer */
                        blebrr_pl_recv_advpacket
                        (
                            BRR_BCON_PASSIVE,
                            &adv_rept_param->p_data[1],
                            adv_rept_param->p_data[0],
                            adv_rept_param->rssi
                        );
                    }
                    break;

                    /* Connectable and scannable undirected Legacy Advertising Packet */
                    case BLE_GAP_LEGACY_PROP_ADV_IND:
                    {
                        #if BLEBRR_GATT_CLIENT
                        if (blebrr_scanstate_gatt)
                        {
                            if (API_SUCCESS == blebrr_scan_find_gatt_pl(adv_rept_param->p_data, adv_rept_param->len, blebrr_scan_mode))
                            {
                                st_ble_dev_addr_t adv_addr;
                                adv_addr.type = adv_rept_param->addr_type;
                                memcpy(adv_addr.addr, adv_rept_param->p_addr, BLE_BD_ADDR_LEN);

                                /**
                                 * Inform Application of a connectable device having Mesh GATT Service.
                                 */
                                blebrr_gatt_iface_pl_cb
                                (
                                    BLEBRR_GATT_IFACE_SCAN,
                                    blebrr_scan_mode,
                                    BLE_GAP_INVALID_CONN_HDL,
                                    &adv_addr
                                );
                            }
                        }
                        #endif /* BLEBRR_GATT_CLIENT */
                    }
                    break;

                    default:
                        break;
                }
            }
        }
        break;

        case BLE_GAP_EVENT_ADV_PARAM_SET_COMP:
        {
            if (BLE_SUCCESS == result)
            {
                blebrr_adv_param_set_comp_hdlr();
            }
        }
        break;

        case BLE_GAP_EVENT_ADV_DATA_UPD_COMP:
        {
            if (BLE_SUCCESS == result)
            {
                st_ble_gap_adv_data_evt_t * adv_data_evt = (st_ble_gap_adv_data_evt_t*)data->p_param;
                blebrr_adv_data_upd_comp_hdlr(adv_data_evt->data_type);
            }
        }
        break;

        /**
         * Advertising has been enabled by invoking R_BLE_GAP_StartAdv()
         */
        case BLE_GAP_EVENT_ADV_ON:
        {
            blebrr_pl_advertise_setup((BLE_SUCCESS == result) ? MS_TRUE : MS_FALSE);
        }
        break;

        /**
         * Advertising has been disabled by invoking R_BLE_GAP_StopAdv()
         */
        case BLE_GAP_EVENT_ADV_OFF:
        {
            blebrr_pl_advertise_setup(MS_FALSE);
        }
        break;

        default:
        break;
    }
}

/***************************************************************************//**
* @brief callback function to receive VS(Vendor Specific) events from BLE Protocol Stack
*******************************************************************************/
static void blebrr_vs_cb(uint16_t type, ble_status_t result, st_ble_vs_evt_data_t * data)
{
    switch (type)
    {
        /**
         * Device Address has been gotten by invoking R_BLE_VS_GetBdAddr()
         */
        case BLE_VS_EVENT_GET_ADDR_COMP:
        {
            st_ble_dev_addr_t * own_addr = NULL;

            if ((BLE_SUCCESS == result) && (NULL != data))
            {
                if (NULL != data->p_param)
                {
                    own_addr = &((st_ble_vs_get_bd_addr_comp_evt_t*)data->p_param)->addr;

                    #if (BLEBRR_VS_ADDR_TYPE == BLE_GAP_ADDR_RAND)
                    /* Set own Static Random Address to Advertising parameters structures */
                    memcpy(blebrr_noncon_adv_param_pl.o_addr, own_addr, BLE_BD_ADDR_LEN);
                    memcpy(blebrr_con_adv_param_pl.o_addr, own_addr, BLE_BD_ADDR_LEN);
                    #endif /* (BLEBRR_VS_ADDR_TYPE == BLE_GAP_ADDR_RAND) */
                }
            }
            blebrr_init_cb(own_addr);
        }
        break;
    }
}
