
/**
 *  \file blebrr_pl.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
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

#define GATT_MESH_CLIENT

/*******************************************************************************
* Macro definitions
*******************************************************************************/
#define BLEBRR_DEFAULT_ADV_HNDL             0x00
#define BLEBRR_DEFAULT_DURATION             0x00
#define BLEBRR_DEFAULT_MAX_EXT_ADV_EVTS     0x00

#define BLEBRR_NCON_ADVINTMIN               0x20 /* 0xA0 */ /* ADV for NON_CONN_IND should be greater than 0x00A0 */
#define BLEBRR_NCON_ADVINTMAX               0x20 /* 0xA0 */ /* ADV for NON_CONN_IND should be greater than 0x00A0 */
#define BLEBRR_NCON_ADVTYPE                 BLE_GAP_LEGACY_PROP_ADV_NONCONN_IND
#define BLEBRR_NCON_DIRADDRTYPE             BLE_GAP_ADDR_PUBLIC
#define BLEBRR_NCON_ADVCHMAP                BLE_GAP_ADV_CH_ALL
#define BLEBRR_NCON_ADVFILTERPOLICY         BLE_GAP_ADV_ALLOW_SCAN_ANY_CONN_ANY

#define BLEBRR_CON_ADVINTMIN                0x20
#define BLEBRR_CON_ADVINTMAX                0x20
#define BLEBRR_CON_ADVTYPE                  BLE_GAP_LEGACY_PROP_ADV_IND
#define BLEBRR_CON_DIRADDRTYPE              BLE_GAP_ADDR_PUBLIC
#define BLEBRR_CON_ADVCHMAP                 BLE_GAP_ADV_CH_ALL
#define BLEBRR_CON_ADVFILTERPOLICY          BLE_GAP_ADV_ALLOW_SCAN_ANY_CONN_ANY
#define BLEBRR_CON_SCANRSP_DATALEN          31

#define BLEBRR_SCANTYPE                     BLE_GAP_SCAN_PASSIVE
#define BLEBRR_SCANINTERVAL                 0x08
#define BLEBRR_SCANWINDOW                   0x08
#define BLEBRR_SCANFILTERPOLICY             BLE_GAP_SCAN_ALLOW_ADV_ALL
#define BLEBRR_SCANFILTERDUPS               BLE_GAP_SCAN_FILT_DUPLIC_DISABLE

#define BLEBRR_CONN_FILTER_POLICY_WL        0x01
#define BLEBRR_CONN_FILTER_POLICY_NWL       0x00
#define BLEBRR_CONN_INTERVAL_MIN            0x0040
#define BLEBRR_CONN_INTERVAL_MAX            0x0040
#define BLEBRR_CONN_LATENCY                 0x0000
#define BLEBRR_CONN_SUPERVISION_TO          0x03BB
#define BLEBRR_CONN_MIN_CE_LEN              0x0000
#define BLEBRR_CONN_MAX_CE_LEN              0x0000

/*******************************************************************************
* Global Variables and Private Functions declaration
*******************************************************************************/
static st_ble_gap_adv_param_t blebrr_noncon_adv_param_pl =
{
    .adv_hdl           = BLEBRR_DEFAULT_ADV_HNDL,
    .adv_prop_type     = BLEBRR_NCON_ADVTYPE,
    .adv_intv_min      = BLEBRR_NCON_ADVINTMIN,
    .adv_intv_max      = BLEBRR_NCON_ADVINTMAX,
    .adv_ch_map        = BLEBRR_NCON_ADVCHMAP,
    .o_addr_type       = BLEBRR_NCON_DIRADDRTYPE,
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
    .o_addr_type       = BLEBRR_CON_DIRADDRTYPE,
    .filter_policy     = BLEBRR_CON_ADVFILTERPOLICY,
    .adv_phy           = BLE_GAP_ADV_PHY_1M,
    .scan_req_ntf_flag = BLE_GAP_SCAN_REQ_NTF_DISABLE,
};

static st_ble_gap_adv_data_t blebrr_adv_data_param_pl =
{
    .adv_hdl          = BLEBRR_DEFAULT_ADV_HNDL,
    .data_type        = BLE_GAP_ADV_DATA_MODE,
    .data_length      = 0,
    .p_data           = NULL,
    .zero_length_flag = BLE_GAP_DATA_0_CLEAR,
};

static st_ble_gap_adv_data_t blebrr_srsp_data_param_pl =
{
    .adv_hdl          = BLEBRR_DEFAULT_ADV_HNDL,
    .data_type        = BLE_GAP_SCAN_RSP_DATA_MODE,
    .data_length      = 0,
    .p_data           = NULL,
    .zero_length_flag = BLE_GAP_DATA_0_CLEAR,
};

static st_ble_gap_scan_on_t blebrr_scan_enable_pl =
{
    .proc_type   = BLE_GAP_SC_PROC_OBS,
    .filter_dups = BLEBRR_SCANFILTERDUPS,
    .duration    = 0x0000,
    .period      = 0x0000,
};

static st_ble_gap_conn_param_t blebrr_conn_param_pl =
{
    .conn_intv_min = BLEBRR_CONN_INTERVAL_MIN,
    .conn_intv_max = BLEBRR_CONN_INTERVAL_MAX,
    .conn_latency  = BLEBRR_CONN_LATENCY,
    .sup_to        = BLEBRR_CONN_SUPERVISION_TO,
    .min_ce_length = BLEBRR_CONN_MIN_CE_LEN,
    .max_ce_length = BLEBRR_CONN_MAX_CE_LEN,
};

#ifdef GATT_MESH_CLIENT
static st_ble_gap_conn_phy_param_t blebrr_conn_1m_phy_param_pl =
{
    .scan_intv    = BLEBRR_SCANINTERVAL,
    .scan_window  = BLEBRR_SCANWINDOW,
    .p_conn_param = &blebrr_conn_param_pl,
};

static st_ble_gap_create_conn_param_t blebrr_create_conn_param_pl =
{
    .init_filter_policy  = BLE_GAP_INIT_FILT_USE_ADDR,
    .remote_bd_addr      = { 0 },
    .remote_bd_addr_type = BLE_GAP_ADDR_PUBLIC,
    .own_addr_type       = BLE_GAP_ADDR_PUBLIC,
    .p_conn_param_1M     = &blebrr_conn_1m_phy_param_pl,
};
#endif /* GATT_MESH_CLIENT */

UINT16 blebrr_gatt_connection_handle;
UCHAR blebrr_gatt_role;

static BLEBRR_INIT_CB blebrr_init_cb;

/* Bearer Scan Response Data related */
static UCHAR blebrr_scanrsp_data[BLEBRR_CON_SCANRSP_DATALEN];
static UCHAR blebrr_scanrsp_datalen;

static st_ble_dev_addr_t blebrr_peer_bd_addr;

static UCHAR blebrr_advstate;
static UCHAR blebrr_scanstate;

BRR_HANDLE blebrr_gatt_handle_pl;

/* Mesh Provisioning service related data structures */
static uint16_t appl_mesh_prov_data_out_ccd_cb
                (
                    uint16_t conn_hdl,
                    uint8_t enabled
                );
static uint16_t appl_mesh_prov_data_in_wt_cb
                (
                    uint16_t conn_hdl,
                    uint16_t offset,
                    uint16_t length,
                    uint8_t  * value
                );
static mesh_prov_cb appl_mesh_prov_cb =
{
    .prov_data_in_cb      = appl_mesh_prov_data_in_wt_cb,
    .prov_data_out_ccd_cb = appl_mesh_prov_data_out_ccd_cb,
};

/* Mesh Proxy service related data structures */
static uint16_t appl_mesh_proxy_data_out_ccd_cb
                (
                    uint16_t conn_hdl,
                    uint8_t enabled
                );
static uint16_t appl_mesh_proxy_data_in_wt_cb
                (
                    uint16_t conn_hdl,
                    uint16_t offset,
                    uint16_t length,
                    uint8_t  * value
                );
static mesh_proxy_cb appl_mesh_proxy_cb =
{
    .proxy_data_in_cb      = appl_mesh_proxy_data_in_wt_cb,
    .proxy_data_out_ccd_cb = appl_mesh_proxy_data_out_ccd_cb,
};

/* Global to hold GATT Iface events Application Callback pointer */
static BLEBRR_GATT_IFACE_EVENT_PL_CB  blebrr_gatt_iface_pl_cb;

static void blebrr_vs_cb
            (
                uint16_t             type,
                ble_status_t         result,
                st_ble_vs_evt_data_t * data
            );
static void blebrr_gap_cb
            (
                uint16_t          type,
                ble_status_t      result,
                st_ble_evt_data_t * data
            );
static void blebrr_enable_mesh_serv_pl (UCHAR serv_type);
#if 0 /* unused */
static void blebrr_disable_mesh_serv_pl (UCHAR serv_type);
#endif /* unused */

/*******************************************************************************
* Functions
*******************************************************************************/
/***************************************************************************//**
* @brief Initializes BLE Stack
*******************************************************************************/
void blebrr_init(BLEBRR_INIT_CB init_cb)
{
    ble_status_t status;

    if (NULL == init_cb)
    {
        return;
    }

    /* register callback to notify the completion of initializing BLE Bearer */
    blebrr_init_cb = init_cb;

    BLEBRR_LOG("Initializing with BLE Stack...\n");

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

    /**
     * TODO: NOTE-2
     * Check if Mesh Services/Profile Specific Initializations to be done here
     */
    status = R_BLE_GATTS_Init(BLE_GATTS_MAX_CB);
    if (BLE_SUCCESS != status)
    {
        BLEBRR_LOG("R_BLE_GATTS_Init(...) failed with status 0x%04X\n", status);
    }

    /* Initialize GATT Mesh Services */
    mesh_serv_init(2);

    /* Set Mode to GATT Prov Mode */
    blebrr_set_gattmode_pl(BLEBRR_GATT_PROV_MODE);

#ifdef GATT_MESH_CLIENT
    status = R_BLE_GATTC_Init(BLE_GATTC_MAX_CB);
    if (status != BLE_SUCCESS)
    {
        BLEBRR_LOG("R_BLE_GATTC_Init is failed: 0x%04X\n", status);
    }

    /* Initialize GATT Mesh Client */
    mesh_client_init();
#endif /* GATT_MESH_CLIENT */
}

/***************************************************************************//**
* @brief Starts or Stops Scan
*******************************************************************************/
void blebrr_scan_pl (UCHAR enable)
{
    ble_status_t ret;

    /* Is request to enable? */
    if (MS_TRUE == enable)
    {
        /* Update global scan state */
        blebrr_scanstate = 0x01;

        /* Enable Scan */
        /**
         * TODO: NOTE-5a
         * Invoke GAP API for Scan Start.
         */
        ret = R_BLE_GAP_StartScan
              (
                  NULL,
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
        /**
         * TODO: NOTE-5b
         * Invoke GAP API for Scan Stop.
         */
        ret = R_BLE_GAP_StopScan();
        if (BLE_SUCCESS != ret)
        {
            BLEBRR_LOG("R_BLE_GAP_StopScan is failed: 0x%04X\n", ret);
        }
    }
}

/***************************************************************************//**
* @brief Sets Scan Response Data to BLE Stack
*******************************************************************************/
API_RESULT blebrr_set_adv_scanrsp_data_pl
           (
               UCHAR * srp_data,
               UCHAR   srp_datalen
           )
{
    /* Initialize the Globals */
    EM_mem_set
    (
        blebrr_scanrsp_data,
        0x0,
        sizeof(blebrr_scanrsp_data)
    );
    blebrr_scanrsp_datalen = 0;

    /* Set the application provided Scan Response Data to Global */
    if ((NULL != srp_data) && (0 != srp_datalen))
    {
        EM_mem_copy
        (
            blebrr_scanrsp_data,
            srp_data,
            srp_datalen
        );
        blebrr_scanrsp_datalen = srp_datalen;

        return API_SUCCESS;
    }

    return API_FAILURE;
}

/***************************************************************************//**
* @brief Sets Advertising Data to BLE Stack
*******************************************************************************/
void blebrr_advertise_data_pl (CHAR type, UCHAR * pdata, UINT16 pdatalen)
{
    ble_status_t ret;

    /* Is request to enable? */
    if ((NULL != pdata) && (0 != pdatalen))
    {
        /* Set Advertising Parameters */
        if (BRR_BCON_PASSIVE == type)
        {
            /**
             * TODO: NOTE-6a
             * Invoke GAP API for Setting Legacy Non Connectable
             * ADV Parameters and ADV Data.
             * Also check, if the ADV is Enabled by the API itself!
             */
            ret = R_BLE_GAP_SetAdvParam(&blebrr_noncon_adv_param_pl);
            if (BLE_SUCCESS != ret)
            {
                BLEBRR_LOG("R_BLE_GAP_SetAdvParam is failed: 0x%04X\n", ret);
            }
        }
        else
        {
            /**
             * TODO: NOTE-6b
             * Invoke GAP API for Setting Legacy Connectable
             * ADV Parameters and ADV Data.
             * Also check, if the ADV is Enabled by the API itself!
             */
            ret = R_BLE_GAP_SetAdvParam(&blebrr_con_adv_param_pl);
            if (BLE_SUCCESS != ret)
            {
                BLEBRR_LOG("R_BLE_GAP_SetAdvParam is failed: 0x%04X\n", ret);
            }

            /* Set the Scan Response Data to Stack if length is valid */
            if (0 != blebrr_scanrsp_datalen)
            {
                /**
                 * TODO: NOTE-6c
                 * Also, Check if that API supports Setting of
                 * Scan Response Data!
                 * Here, Scan Resp data is present in "blebrr_scanrsp_data"
                 */
                blebrr_srsp_data_param_pl.p_data      = blebrr_scanrsp_data;
                blebrr_srsp_data_param_pl.data_length = blebrr_scanrsp_datalen;
                ret = R_BLE_GAP_SetAdvSresData(&blebrr_srsp_data_param_pl);

                if (BLE_SUCCESS != ret)
                {
                    /* Scan Response Data Setting Failure */
                    BLEBRR_LOG("R_BLE_GAP_SetAdvSresData for SRSP is failed: 0x%04X\n", ret);
                }
            }
        }

        /* Set Advertising Data */
        /**
         * TODO: NOTE-6d
         * Invoke GAP API for Setting Legacy Connectable ADV Data.
         */
        blebrr_adv_data_param_pl.p_data      = pdata;
        blebrr_adv_data_param_pl.data_length = pdatalen;
        ret = R_BLE_GAP_SetAdvSresData(&blebrr_adv_data_param_pl);

        if (BLE_SUCCESS != ret)
        {
            /* Scan Response Data Setting Failure */
            BLEBRR_LOG("R_BLE_GAP_SetAdvSresData for ADV is failed: 0x%04X\n", ret);
        }

        /* Enable Advertising */
        blebrr_advertise_pl(MS_TRUE);
    }
    else
    {
        /* Disable Advertising */
        blebrr_advertise_pl(MS_FALSE);
    }
}

/***************************************************************************//**
* @brief Starts or Stops Advertisement
*******************************************************************************/
void blebrr_advertise_pl(UCHAR state)
{
    ble_status_t ret;

    /* Update global state */
    blebrr_advstate = (MS_TRUE == state) ? 0x01 : 0x00;

    /**
     * TODO: NOTE-7
     * Invoke GAP API for ADV Enable or ADV Disable.
     * If ADV Enable is started automatically by the Stack
     * after setting the Parameters, then invoke only Stop
     * based on the "blebrr_advstate".
     */
    if (0x01 == blebrr_advstate)
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
            BLEBRR_LOG("Adv Operation failed with reason 0x%04X\n", ret);
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
}

/***************************************************************************//**
* @brief Sends data over GATT Bearer
*******************************************************************************/
API_RESULT blebrr_gatt_send_pl(BRR_HANDLE * handle, UCHAR * data, UINT16 datalen)
{
    UCHAR      type;
    API_RESULT retval = API_SUCCESS;

    /* Get the current mode */
    /* TODO: See if we need to get specific mode for a GATT transport */
    type = blebrr_gatt_mode_get();

    /* Check the PDU type received and Add bearer to Mesh stack */
    if (BLEBRR_GATT_PROV_MODE == type)
    {
        /* BLEBRR_LOG("\nBLEBRR_GATT_PROV_MODE with role 0x%02X\n", blebrr_gatt_role); */

        if (BLEBRR_SERVER_ROLE == blebrr_gatt_role)
        {
            mesh_prov_notify_data_out
            (
                blebrr_gatt_connection_handle,
                MESH_PROV_DATA_OUT_VAL_HDL,
                data,
                datalen
            );
        }
#ifdef GATT_MESH_CLIENT
        else
        {
            mesh_prov_client_data_in_write(blebrr_gatt_connection_handle, data, datalen);

            retval = API_SUCCESS;
        }
#endif /* GATT_MESH_CLIENT */
    }
    else
    {
        /* BLEBRR_LOG("\nBLEBRR_GATT_PROXY_MODE with role 0x%02X\n", blebrr_gatt_role); */

        if (BLEBRR_SERVER_ROLE == blebrr_gatt_role)
        {
            mesh_proxy_notify_data_out
            (
                blebrr_gatt_connection_handle,
                MESH_PROXY_DATA_OUT_VAL_HDL,
                data,
                datalen
            );
        }
#ifdef GATT_MESH_CLIENT
        else
        {
            mesh_proxy_client_data_in_write(blebrr_gatt_connection_handle, data, datalen);

            retval = API_SUCCESS;
        }
#endif /* GATT_MESH_CLIENT */
    }

    return retval;
}

/***************************************************************************//**
* @brief Sets up GATT Bearer Interface
*******************************************************************************/
API_RESULT blebrr_gatt_com_channel_setup_pl
           (
               UCHAR role,
               UCHAR mode,
               UCHAR evt
           )
{
    API_RESULT retval;
    UINT16     mtu;

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

    retval = API_FAILURE;
    if (BLEBRR_COM_CHANNEL_CONNECT == evt)
    {
       /* Store the gatt role to be used during write */
        blebrr_gatt_role = role;

        /* Initialie MTU */
        mtu = BLEBRR_GATT_MIN_MTU;

        if (BLEBRR_SERVER_ROLE == role)
        {
            mtu = mesh_serv_get_mtu();

            /* Adjust the MTU for 1 Bytes of ATT Opcode and 2 Bytes of Handle */
            mtu -= 3;
        }
        else if (BLEBRR_CLIENT_ROLE == role)
        {
            mtu = mesh_serv_get_mtu();

            /* Adjust the MTU for 1 Bytes of ATT Opcode and 2 Bytes of Handle */
            mtu -= 3;
        }
        else
        {
            /* Empty */
        }

        retval = blebrr_pl_gatt_connection
                 (
                     &blebrr_gatt_handle_pl,
                     role,
                     mode,
                     mtu
                 );

        if (NULL != blebrr_gatt_iface_pl_cb)
        {
            blebrr_gatt_iface_pl_cb
            (
                BLEBRR_GATT_IFACE_ENABLE,
                mode  /* BLEBRR_GATT_PROV_MODE or BLEBRR_GATT_PROXY_MODE */
            );
        }
    }
    else if (BLEBRR_COM_CHANNEL_DISCONNECT == evt)
    {
        /**
         * Currently BLE Bearer GATT Channel Disconnection
         * is called only from HCI/ACL link disconnection.
         */
        blebrr_gatt_role = 0xFF;

        /* Delete Device from the Bearer */
        retval = blebrr_pl_gatt_disconnection (&blebrr_gatt_handle_pl);
        blebrr_gatt_handle_pl = BRR_HANDLE_INVALID;

        if (NULL != blebrr_gatt_iface_pl_cb)
        {
            blebrr_gatt_iface_pl_cb
            (
                BLEBRR_GATT_IFACE_DISABLE,
                mode  /* BLEBRR_GATT_PROV_MODE or BLEBRR_GATT_PROXY_MODE */
            );
        }
    }
    else
    {
        /* Empty */
    }

    return retval;
}

static uint16_t appl_mesh_prov_data_out_ccd_cb(uint16_t conn_hdl, uint8_t enabled)
{
    /* Check the Current mode is not PROV */
    if (BLEBRR_GATT_PROV_MODE != blebrr_gatt_mode_get())
    {
        BLEBRR_LOG("Mesh Prov Out CCD being Written when PROV is not Active!\n");
        return 0xFFFF;
    }

    if (MS_TRUE == enabled)
    {
        BLEBRR_LOG("Mesh Prov Out CCD Enabled\n");
    }
    else
    {
        BLEBRR_LOG("Mesh Prov Out CCD Disabled\n");
    }

    blebrr_gatt_com_channel_setup_pl
    (
        BLEBRR_SERVER_ROLE,
        BLEBRR_GATT_PROV_MODE,
        (enabled) ? BLEBRR_COM_CHANNEL_CONNECT : BLEBRR_COM_CHANNEL_DISCONNECT
    );

    return 0x0000;
}

static uint16_t appl_mesh_prov_data_in_wt_cb
                (
                    uint16_t conn_hdl,
                    uint16_t offset,
                    uint16_t length,
                    uint8_t  * value
                )
{
    if (NULL != value)
    {
        blebrr_pl_recv_gattpacket
        (
            &blebrr_gatt_handle_pl,
            value,
            length
        );
    }

    return 0x0000;
}

static uint16_t appl_mesh_proxy_data_out_ccd_cb(uint16_t conn_hdl, uint8_t enabled)
{
    /* Check the Current mode is not PROV */
    if (BLEBRR_GATT_PROXY_MODE != blebrr_gatt_mode_get())
    {
        BLEBRR_LOG("Mesh Proxy Out CCD being Written when PROXY is not Active!\n");
        return 0xFFFF;
    }

    if (MS_TRUE == enabled)
    {
        BLEBRR_LOG("Mesh Proxy Out CCD Enabled\n");
    }
    else
    {
        BLEBRR_LOG("Mesh Proxy Out CCD Disabled\n");
    }

    blebrr_gatt_com_channel_setup_pl
    (
        BLEBRR_SERVER_ROLE,
        BLEBRR_GATT_PROXY_MODE,
        (enabled) ? BLEBRR_COM_CHANNEL_CONNECT : BLEBRR_COM_CHANNEL_DISCONNECT
    );

    return 0x0000;
}

static uint16_t appl_mesh_proxy_data_in_wt_cb
                (
                    uint16_t conn_hdl,
                    uint16_t offset,
                    uint16_t length,
                    uint8_t  * value
                )
{
    if (NULL != value)
    {
        blebrr_pl_recv_gattpacket
        (
            &blebrr_gatt_handle_pl,
            value,
            length
        );
    }

    return 0x0000;
}

#ifdef GATT_MESH_CLIENT
static void mesh_prov_data_out_notif_cb
     (
         uint16_t  conn_hdl,
         uint16_t  length,
         uint8_t   * value
     )
{
    blebrr_pl_recv_gattpacket
    (
        &blebrr_gatt_handle_pl,
        (UCHAR *)value,
        length
    );
}

static void mesh_prov_notif_config_status_cb
     (
         uint16_t  conn_hdl,
         uint8_t   flag,
         uint8_t   status
     )
{
    if (status == 0x00)
    {
        BLEBRR_LOG("Mesh Provisioning Data Out notifications %s\n",
        flag ?  "enabled" : "disabled");

        blebrr_gatt_mode_set(BLEBRR_GATT_PROV_MODE);
        blebrr_gatt_com_channel_setup_pl
        (
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
void mesh_prov_disc_comp_cb (uint16_t  conn_hdl)
{
    //BLEBRR_LOG("Discovery Process Completed !!!\n");
    R_BLE_CLI_Printf("Discovery Process Completed !!!\n");
}

static mesh_client_prov_cb mesh_prov_callbacks =
{
    .mesh_prov_data_out_notif = mesh_prov_data_out_notif_cb,
    .mesh_prov_ntf_status     = mesh_prov_notif_config_status_cb,
    .mesh_prov_disc_comp      = mesh_prov_disc_comp_cb,
};

static void mesh_proxy_data_out_notif_cb
     (
         uint16_t  conn_hdl,
         uint16_t  length,
         uint8_t   * value
     )
{
    blebrr_pl_recv_gattpacket
    (
        &blebrr_gatt_handle_pl,
        (UCHAR *)value,
        length
    );
}

static void mesh_proxy_notif_config_status_cb
     (
         uint16_t  conn_hdl,
         uint8_t   flag,
         uint8_t   status
     )
{
    if (status == 0x00)
    {
        BLEBRR_LOG("Mesh Proxy Data Out notifications %s\n",
        flag ?  "enabled" : "disabled");

        blebrr_gatt_mode_set(BLEBRR_GATT_PROXY_MODE);

        blebrr_gatt_com_channel_setup_pl
        (
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
void mesh_proxy_disc_comp_cb (uint16_t  conn_hdl)
{
    BLEBRR_LOG("Discovery Process Completed !!!\n");
}

static mesh_client_proxy_cb mesh_proxy_callbacks =
{
    .mesh_proxy_data_out_notif = mesh_proxy_data_out_notif_cb,
    .mesh_proxy_ntf_status     = mesh_proxy_notif_config_status_cb,
    .mesh_proxy_disc_comp      = mesh_proxy_disc_comp_cb,
};
#endif /* GATT_MESH_CLIENT */

/***************************************************************************//**
* @brief Registers GATT Bearer Interface Callback function to Bearer
*******************************************************************************/
API_RESULT blebrr_register_gatt_iface_event_pl
           (
               BLEBRR_GATT_IFACE_EVENT_PL_CB gatt_iface_evt_cb
           )
{
    if (NULL != gatt_iface_evt_cb)
    {
        blebrr_gatt_iface_pl_cb = gatt_iface_evt_cb;

        BLEBRR_LOG("\nRegistered GATT Bearer Iface Events Appl Callback!\n");

        return API_SUCCESS;
    }

    /* If NULL callback is registered */
    return API_FAILURE;
}

/***************************************************************************//**
* @brief Creates a connection for GATT Bearer
*******************************************************************************/
API_RESULT blebrr_create_gatt_conn_pl
           (
               UCHAR p_bdaddr_type,
               UCHAR * p_bdaddr
           )
{
#ifdef GATT_MESH_CLIENT
    UCHAR        t_addr[BLE_BD_ADDR_LEN];
    UINT32       count;
    UINT32       i;
    ble_status_t ret;

    /* Reverse the address from LSB to MSB */
    i = (BLE_BD_ADDR_LEN -1);
    for (count = 0; count < BLE_BD_ADDR_LEN; count++)
    {
        t_addr[count] = p_bdaddr[i--];
    }

    memcpy
    (
        blebrr_create_conn_param_pl.remote_bd_addr,
        t_addr,
        BLE_BD_ADDR_LEN
    );

    blebrr_create_conn_param_pl.remote_bd_addr_type = p_bdaddr_type;

    ret = R_BLE_GAP_CreateConn(&blebrr_create_conn_param_pl);

    if (BLE_SUCCESS != ret)
    {
        BLEBRR_LOG("R_BLE_GAP_CreateConn is failed: 0x%04X\n", ret);
    }

    return (BLE_SUCCESS == ret) ? API_SUCCESS : API_FAILURE;
#else /* GATT_MESH_CLIENT */
    return API_FAILURE;
#endif /* GATT_MESH_CLIENT */
}

/***************************************************************************//**
* @brief Disconnects
*******************************************************************************/
API_RESULT blebrr_disconnect_pl(void)
{
    ble_status_t ret;

    ret = R_BLE_GAP_Disconnect
          (
              blebrr_gatt_connection_handle,
              0x13
          );

    if (BLE_SUCCESS != ret)
    {
        BLEBRR_LOG("R_BLE_GAP_Disconnect is failed: 0x%04X\n", ret);
    }

    return (BLE_SUCCESS == ret) ? API_SUCCESS : API_FAILURE;
}

/***************************************************************************//**
* @brief Start Mesh Service Discovery
*******************************************************************************/
API_RESULT blebrr_discover_service_pl(UCHAR serv)
{
    /* Set the mode with bearer */
    (serv == 0) ? blebrr_gatt_mode_set(BLEBRR_GATT_PROV_MODE) :
                  blebrr_gatt_mode_set(BLEBRR_GATT_PROXY_MODE);

#ifdef GATT_MESH_CLIENT
    /* Register the corresponding Callbacks */
    (serv == BLEBRR_GATT_PROV_MODE) ?             \
    mesh_client_prov_init(&mesh_prov_callbacks) : \
    mesh_client_proxy_init(&mesh_proxy_callbacks);

    return mesh_client_discover_services
           (
               blebrr_gatt_connection_handle,
               serv
           );
#else /* GATT_MESH_CLIENT */
    BLEBRR_LOG("\nBLE_CLIENT_ROLE Disabled!\n");
    return API_FAILURE;
#endif /* GATT_MESH_CLIENT */
}

/***************************************************************************//**
* @brief Enables or Disables Mesh Service Notification
*******************************************************************************/
API_RESULT blebrr_confige_ntf_pl(UCHAR config_ntf, UCHAR mode)
{
    /* Set the mode with bearer */
    (BLEBRR_GATT_PROV_MODE == mode) ? blebrr_gatt_mode_set(BLEBRR_GATT_PROV_MODE) :
                                      blebrr_gatt_mode_set(BLEBRR_GATT_PROXY_MODE);

#ifdef GATT_MESH_CLIENT
    return  mesh_client_config_ntf
            (
                blebrr_gatt_connection_handle,
                mode,
                (0x00 == config_ntf) ? MS_FALSE : MS_TRUE
            );
#else /* GATT_MESH_CLIENT */
    BLEBRR_LOG("\nBLE_CLIENT_ROLE Disabled!\n");
    return API_FAILURE;
#endif /* GATT_MESH_CLIENT */
}

static void blebrr_enable_mesh_serv_pl (UCHAR serv_type)
{
    BLEBRR_LOG ("Serv Enable called with 0x%02X\n", serv_type);

    /* If serv_type :
     *  BLEBRR_GATT_PROVILINK - Mesh Prov
     *  BLEBRR_GATT_PROXYLINK - Mesh Proxy
     */
    if (BLEBRR_GATT_PROV_MODE == serv_type)
    {
        BLEBRR_LOG ("Enabling Mesh Prov Service...\n");
        mesh_serv_prov_init(&appl_mesh_prov_cb);
    }
    else
    {
        BLEBRR_LOG ("Enabling Mesh Proxy Service...\n");
        mesh_serv_proxy_init(&appl_mesh_proxy_cb);
    }
}

#if 0 /* unused */
static void blebrr_disable_mesh_serv_pl (UCHAR serv_type)
{
    BLEBRR_LOG ("Serv Disable called with 0x%02X\n", serv_type);

    /* If serv_type :
     *  BLEBRR_GATT_PROVILINK - Mesh Prov
     *  BLEBRR_GATT_PROXYLINK - Mesh Proxy
     */
    if (BLEBRR_GATT_PROV_MODE == serv_type)
    {
        BLEBRR_LOG ("Disabling Mesh Prov Service...\n");
        /*
         * Disable Mesh Provisioing Serivce
         */
        mesh_serv_prov_deinit();
    }
    else
    {
        BLEBRR_LOG ("Disabling Mesh Proxy Service...\n");
        /*
         * Disable Mesh Proxy Serivce
         */
        mesh_serv_proxy_deinit();
    }
}
#endif /* unused */

/***************************************************************************//**
* @brief Sets GATT mode
*******************************************************************************/
void blebrr_set_gattmode_pl (UCHAR flag)
{
    /* Setting Provisioning or Proxy Mode */
    if ((BLEBRR_GATT_PROV_MODE  == flag) ||
        (BLEBRR_GATT_PROXY_MODE == flag))
    {
        blebrr_enable_mesh_serv_pl(flag);
    }
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

static void blebrr_gap_cb
            (
                uint16_t          type,
                ble_status_t      result,
                st_ble_evt_data_t * data
            )
{
    UCHAR adv_type;

    switch (type)
    {
        case BLE_GAP_EVENT_STACK_ON:
        {
            /** Equivalent to Bluetooth ON ! */
            BLEBRR_LOG("\nReceived BLE_GAP_EVENT_STACK_ON\n");
            blebrr_init_cb();
        }
        break;

        case BLE_GAP_EVENT_CONN_IND:
        {
            BLEBRR_LOG("\nReceived BLE_GAP_EVENT_CONN_IND\n");
            /**
             * Equivalent to HCI LE Connection Complete
             *  or Enhanced Connection Complete
             */
            if (result == BLE_SUCCESS)
            {
                st_ble_gap_conn_evt_t *conn_evt_param =
                    (st_ble_gap_conn_evt_t *)data->p_param;

                /* Store the connection handle Globaly */
                blebrr_gatt_connection_handle = conn_evt_param->conn_hdl;

                switch (conn_evt_param->remote_addr_type)
                {
                    case BLE_GAP_ADDR_PUBLIC:
                    {
                        memcpy
                        (
                            blebrr_peer_bd_addr.addr,
                            conn_evt_param->remote_addr,
                            BLE_BD_ADDR_LEN
                        );
                        blebrr_peer_bd_addr.type = BLE_GAP_ADDR_PUBLIC;
                    } break;

                    case BLE_GAP_ADDR_RAND:
                    {
                        memcpy
                        (
                            blebrr_peer_bd_addr.addr,
                            conn_evt_param->remote_addr,
                            BLE_BD_ADDR_LEN
                        );
                        blebrr_peer_bd_addr.type = BLE_GAP_ADDR_RAND;
                    } break;

                    case BLE_GAP_ADDR_RPA_ID_PUBLIC:
                    case BLE_GAP_ADDR_RPA_ID_RANDOM:
                    {
                        memcpy
                        (
                            blebrr_peer_bd_addr.addr,
                            conn_evt_param->remote_rpa,
                            BLE_BD_ADDR_LEN
                        );
                        blebrr_peer_bd_addr.type = BLE_GAP_ADDR_RAND;
                    } break;
                }

                BLEBRR_LOG("gap: connected conn_hdl:0x%04X, addr:%s\n",
                blebrr_gatt_connection_handle,
                BLE_BD_ADDR_STR(blebrr_peer_bd_addr.addr,
                                blebrr_peer_bd_addr.type));

                /* Stop Advertising */
                #if 1
                if (BLEBRR_GATT_PROV_MODE == blebrr_gatt_mode_get())
                {
                    extern void prov_stop_beaconing(void);
                    prov_stop_beaconing();
                }
                else
                {
                    MS_proxy_server_adv_stop();
                }
                #endif
                blebrr_pl_advertise_end();

                /**
                 * Inform Application of GATT/BLE Link Layer Connection.
                 */
                if (NULL != blebrr_gatt_iface_pl_cb)
                {
                    blebrr_gatt_iface_pl_cb
                    (
                        BLEBRR_GATT_IFACE_UP,
                        BLE_SUCCESS
                    );
                }
            }
        }
        break;

        case BLE_GAP_EVENT_DISCONN_IND:
        {
            st_ble_gap_disconn_evt_t *disc_evt_param =
                (st_ble_gap_disconn_evt_t *)data->p_param;

            BLEBRR_LOG("\nReceived BLE_GAP_EVENT_DISCONN_IND\n");
            /**
             * Equivalent to HCI Disconnection
             */
            BLEBRR_LOG("gap: disconnected conn_hdl:0x%04X, addr:%s with reason:0x%02X\n",
            disc_evt_param->conn_hdl,
            BLE_BD_ADDR_STR(blebrr_peer_bd_addr.addr,
                            blebrr_peer_bd_addr.type),
            disc_evt_param->reason);

            blebrr_gatt_connection_handle = BLE_GAP_INVALID_CONN_HDL;
            blebrr_gatt_role = 0xFF;

            /* Delete Device from the Bearer */
            blebrr_pl_gatt_disconnection (&blebrr_gatt_handle_pl);
            blebrr_gatt_handle_pl = BRR_HANDLE_INVALID;

            /**
             * Inform Application of GATT/BLE Link Layer Disconnection.
             */
            if (NULL != blebrr_gatt_iface_pl_cb)
            {
                blebrr_gatt_iface_pl_cb
                (
                    BLEBRR_GATT_IFACE_DOWN,
                    disc_evt_param->reason
                );
            }
        }
        break;

        case BLE_GAP_EVENT_CONN_PARAM_UPD_REQ:
        {
            /**
             * Equivalent to HCI LE Connection Updt Request
             */
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

        case BLE_GAP_EVENT_SCAN_ON:
        {
            blebrr_pl_scan_setup (blebrr_scanstate);
        }
        break;
        case BLE_GAP_EVENT_SCAN_OFF:
        {
            blebrr_pl_scan_setup (blebrr_scanstate);
        }
        break;
        case BLE_GAP_EVENT_ADV_REPT_IND:
        {
            st_ble_gap_adv_rept_evt_t *adv_rept_evt_param =
                (st_ble_gap_adv_rept_evt_t *)data->p_param;

            if (0x01 == adv_rept_evt_param->adv_rpt_type)
            {
                st_ble_gap_ext_adv_rept_t *ext_adv_rept_param =
                    (st_ble_gap_ext_adv_rept_t *)adv_rept_evt_param->param.p_ext_adv_rpt;

                /**
                 * Discard the packet if its anything other than
                 * LEGACY ADV packets.
                 * TODO:
                 * Need to add logic for Scan Response Data packets
                 * for future use when connectable packets are
                 * to be decoded.
                 */
                if ((0x0013 != ext_adv_rept_param->adv_type) &&
                    (0x0010 != ext_adv_rept_param->adv_type))
                {
                    break;
                }

                /* Give packet to the bearer */
                adv_type = (0x0010 == ext_adv_rept_param->adv_type)?
                        BRR_BCON_PASSIVE: BRR_BCON_ACTIVE;

                blebrr_pl_recv_advpacket
                (
                    adv_type,
                    &ext_adv_rept_param->p_data[1],
                    ext_adv_rept_param->p_data[0],
                    ext_adv_rept_param->rssi
                );
            }
        }
        break;
        case BLE_GAP_EVENT_ADV_ON:
        {
            blebrr_pl_advertise_setup (blebrr_advstate);
        }
        break;
        case BLE_GAP_EVENT_ADV_OFF:
        {
            blebrr_pl_advertise_setup (blebrr_advstate);
        }
        break;

        default:
        break;
    }
}

static void blebrr_vs_cb
            (
                uint16_t             type,
                ble_status_t         result,
                st_ble_vs_evt_data_t * data
            )
{
    /* do nothing */
}
