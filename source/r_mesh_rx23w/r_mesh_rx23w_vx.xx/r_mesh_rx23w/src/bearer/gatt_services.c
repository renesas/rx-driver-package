
/**
 *  \file gatt_services.c
 *
 *  Generic Module to handle both
 *  - Mesh Provisioning Service :: 0x1827
 *  - Mesh Proxy Service        :: 0x1828
 */

/*
 *  Copyright (C) 2018. Mindtree Limited.
 *  All rights reserved.
 */

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "gatt_services.h"

/*******************************************************************************
* Global Variables and Private Functions declaration
*******************************************************************************/
static mesh_prov_cb   * prov_cb;
static mesh_proxy_cb  * proxy_cb;

static uint8_t        g_mesh_serv_state;
static uint16_t       g_mesh_curr_mtu;

static void mesh_prov_handle_conn
            (
                uint16_t connHandle,
                uint8_t changeType
            );

static void mesh_proxy_handle_conn
            (
                uint16_t connHandle,
                uint8_t changeType
            );

/*******************************************************************************
* Functions
*******************************************************************************/
static void mesh_serv_set_cccd
            (
                uint16_t conn_hdl,
                uint16_t attr_hdl,
                uint16_t cli_cnfg
            )
{
    uint8_t data[2];

    BT_PACK_LE_2_BYTE(data, &cli_cnfg);

    st_ble_gatt_value_t gatt_value = {
        .p_value     = data,
        .value_len = 2,
    };

    R_BLE_GATTS_SetAttr(conn_hdl, attr_hdl, &gatt_value);
}

static void mesh_serv_get_cccd
            (
                uint16_t conn_hdl,
                uint16_t attr_hdl,
                uint16_t * cli_cnfg
            )
{
    st_ble_gatt_value_t gatt_value;

    R_BLE_GATTS_GetAttr(conn_hdl, attr_hdl, &gatt_value);

    BT_UNPACK_LE_2_BYTE(cli_cnfg, gatt_value.p_value);
}

static void mesh_prov_gatt_db_cb
            (
                uint16_t                 conn_hdl,
                st_ble_gatts_db_params_t * params
            )
{
    MESH_SERVER_LOG("\nmesh_prov_gatt_db_cb invoked with type 0x%04X\n",
    params->db_op);

    switch (params->db_op)
    {
        /* Handle Write Command to Prov Data IN */
        case BLE_GATTS_OP_CHAR_PEER_WRITE_CMD:
        {
            if (MESH_PROV_DATA_IN_VAL_HDL == params->attr_hdl)
            {
                if (NULL != prov_cb)
                {
                    prov_cb->prov_data_in_cb
                    (
                        conn_hdl,
                        0, /* Offset is currently ZERO */
                        params->value.value_len,
                        params->value.p_value
                    );
                }
            }
        }
        break;

        /* Handle Prov Data Out CCCD Write */
        case BLE_GATTS_OP_CHAR_PEER_CLI_CNFG_WRITE_REQ:
        break;
    }
}

static void mesh_proxy_gatt_db_cb
            (
                uint16_t                 conn_hdl,
                st_ble_gatts_db_params_t * params
            )
{
    MESH_SERVER_LOG("\nmesh_proxy_gatt_db_cb invoked with type 0x%04X\n",
    params->db_op);

    switch (params->db_op)
    {
        /* Handle Write Command to Proxy Data IN */
        case BLE_GATTS_OP_CHAR_PEER_WRITE_CMD:
        {
            if (MESH_PROXY_DATA_IN_VAL_HDL == params->attr_hdl)
            {
                if (NULL != proxy_cb)
                {
                    proxy_cb->proxy_data_in_cb
                    (
                        conn_hdl,
                        0, /* Offset is currently ZERO */
                        params->value.value_len,
                        params->value.p_value
                    );
                }
            }
        }
        break;

        /* Handle Proxy Data Out CCCD Write */
        case BLE_GATTS_OP_CHAR_PEER_CLI_CNFG_WRITE_REQ:
        break;
    }
}

static void mesh_serv_gatts_cb
            (
                 uint16_t type,
                 ble_status_t result,
                 st_ble_gatts_evt_data_t *data
            )
{
    ble_status_t status;

    MESH_SERVER_LOG("\nmesh_serv_gatts_cb :: type 0x%04X: Service 0x%02X \n",
    type, g_mesh_serv_state);

    switch (type)
    {
        case BLE_GATTS_EVENT_CONN_IND:
        {
            MESH_SERVER_LOG("[MESH SERV] Received BLE_GATTS_EVENT_CONN_IND");

            if (MESH_PROV_SERVICE_DONE == g_mesh_serv_state)
            {
                mesh_prov_handle_conn(data->conn_hdl, MS_TRUE);
            }
            else if (MESH_PROXY_SERVICE_DONE == g_mesh_serv_state)
            {
                mesh_proxy_handle_conn(data->conn_hdl, MS_TRUE);
            }
            else
            {
                /* Do nothing */
            }

        }
        break;

        case BLE_GATTS_EVENT_DISCONN_IND:
        {
            MESH_SERVER_LOG("[MESH SERV] Received BLE_GATTS_EVENT_DISCONN_IND");
        }
        break;

        case BLE_GATTS_EVENT_EX_MTU_REQ:
        {
            st_ble_gatts_ex_mtu_req_evt_t *db_access_evt_param =
                (st_ble_gatts_ex_mtu_req_evt_t *)data->p_param;
            uint16_t p_cli_mtu;

            /* Extract Peer Requested MTU Value */
            p_cli_mtu = db_access_evt_param->mtu;

            MESH_SERVER_LOG("[MESH SERV] Received BLE_GATTS_EVENT_EX_MTU_REQ with Value %d",
            p_cli_mtu);

            /* Set the MTU to be Responded */
            g_mesh_curr_mtu = (p_cli_mtu >= g_mesh_curr_mtu) ?\
                              g_mesh_curr_mtu : p_cli_mtu;

            /* Responding with Default MTU here */
            status = R_BLE_GATTS_RspExMtu(data->conn_hdl, g_mesh_curr_mtu);

            MESH_SERVER_LOG("R_BLE_GATTS_RspExMtu Sent with MTU: %d, Status : 0x%04X",
            g_mesh_curr_mtu, status);

            if (BLE_SUCCESS != status)
            {
                g_mesh_curr_mtu = BLE_GATT_DEFAULT_MTU;
            }

            MESH_SERVER_LOG("Negotiated Current ATT MTU: %d",
            g_mesh_curr_mtu);
        }
        break;

        case BLE_GATTS_EVENT_DB_ACCESS_IND:
        {
            st_ble_gatts_db_access_evt_t *db_access_evt_param =
                (st_ble_gatts_db_access_evt_t *)data->p_param;

            /* Check for Current Active Service */
            if (MESH_PROV_SERVICE_DONE == g_mesh_serv_state)
            {
                mesh_prov_gatt_db_cb
                (
                    data->conn_hdl,
                    db_access_evt_param->p_params
                );
            }
            else if (MESH_PROXY_SERVICE_DONE == g_mesh_serv_state)
            {
                mesh_proxy_gatt_db_cb
                (
                    data->conn_hdl,
                    db_access_evt_param->p_params
                );
            }
            else
            {
                /* DO Nothing */
                /* Log it as Error! */
            }

        } break;

        case BLE_GATTS_EVENT_WRITE_RSP_COMP:
        {
            st_ble_gatts_write_rsp_evt_t *db_wt_rsp_evt_param =
                (st_ble_gatts_write_rsp_evt_t *)data->p_param;
            uint16_t cli_cnfg;
            uint16_t t_cccd_val;

            /* Check for Current Active Service */
            if (MESH_PROV_SERVICE_DONE == g_mesh_serv_state)
            {
                /* Check if the Write RSP if for Prov CCCD */
                if (MESH_PROV_DATA_OUT_CCD_HDL == db_wt_rsp_evt_param->attr_hdl)
                {
                    /* Get the PROXY DATA OUT CCD Value */
                    mesh_serv_get_cccd
                    (
                        data->conn_hdl,
                        MESH_PROV_DATA_OUT_CCD_HDL,
                        &cli_cnfg
                    );

                    t_cccd_val = (cli_cnfg == BLE_GATTS_CLI_CNFG_NOTIFICATION) ?\
                                                 MS_TRUE : MS_FALSE;

                    /* Invoke CCCD Update Callback */
                    if (NULL != prov_cb)
                    {
                        prov_cb->prov_data_out_ccd_cb
                        (
                            data->conn_hdl,
                            t_cccd_val
                        );
                    }
                }
            }
            else if (MESH_PROXY_SERVICE_DONE == g_mesh_serv_state)
            {
                /* Check if the Write RSP if for Proxy CCCD */
                if (MESH_PROXY_DATA_OUT_CCD_HDL == db_wt_rsp_evt_param->attr_hdl)
                {
                    /* Get the PROXY DATA OUT CCD Value */
                    mesh_serv_get_cccd
                    (
                        data->conn_hdl,
                        MESH_PROXY_DATA_OUT_CCD_HDL,
                        &cli_cnfg
                    );

                    t_cccd_val = (cli_cnfg == BLE_GATTS_CLI_CNFG_NOTIFICATION) ?\
                                                 MS_TRUE : MS_FALSE;

                    /* Invoke CCCD Update Callback */
                    if (NULL != proxy_cb)
                    {
                        proxy_cb->proxy_data_out_ccd_cb
                        (
                            data->conn_hdl,
                            t_cccd_val
                        );
                    }
                }

            }
            else
            {
                /* DO Nothing */
                /* Log it as Error! */
            }

        } break;
    }
}

/***************************************************************************//**
* @brief Initializes GATT Server
*******************************************************************************/
ble_status_t mesh_serv_init(uint8_t priority)
{
    /* Register Internal MEsh Services Callback to GATT Server */
    R_BLE_GATTS_RegisterCb(mesh_serv_gatts_cb, priority);

    g_mesh_serv_state = MESH_NO_SERVICES;
    g_mesh_curr_mtu   = BLE_GATT_DEFAULT_MTU;

    return BLE_SUCCESS;
}

/***************************************************************************//**
* @brief Gets current MTU size
*******************************************************************************/
uint16_t mesh_serv_get_mtu(void)
{
    return g_mesh_curr_mtu;
}

/***************************************************************************//**
* @brief Sets new MTU size
*******************************************************************************/
ble_status_t mesh_serv_set_mtu(uint16_t mtu)
{
    g_mesh_curr_mtu = (BLE_GATT_DEFAULT_MTU > mtu)? \
                       BLE_GATT_DEFAULT_MTU : mtu;

    return BLE_SUCCESS;
}

/***************************************************************************//**
* @brief Sets Mesh Provisioning Service instance to BLE Stack
*******************************************************************************/
ble_status_t mesh_serv_prov_init(mesh_prov_cb *cb)
{
    ble_status_t status = BLE_SUCCESS;
    static uint8_t prov_init_first_time = 0x00;

    if (NULL != cb)
    {
        if (0x00 == prov_init_first_time)
        {
            /* Initialize GATTS. */
            status = R_BLE_GATTS_SetDbInst(&g_gatt_db_table_mesh_prov);
            if (BLE_SUCCESS != status)
            {
                MESH_SERVER_LOG("R_BLE_GATTS_SetDbInst is failed: 0x%04X", status)
            }

            /* Save the Callback Provided */
            prov_cb = cb;

            /* Set the Current Service State */
            g_mesh_serv_state = MESH_PROV_SERVICE_DONE;

            prov_init_first_time = 0x01;
        }
    }

    return (status);
}

/***************************************************************************//**
* @brief Sets Mesh Proxy Service instance to BLE Stack
*******************************************************************************/
ble_status_t mesh_serv_proxy_init(mesh_proxy_cb *cb)
{
    ble_status_t status = BLE_SUCCESS;
    static uint8_t proxy_init_first_time = 0x00;

    if (NULL != cb)
    {
        if (0x00 == proxy_init_first_time)
        {
            /* Initialize GATTS. */
            status = R_BLE_GATTS_SetDbInst(&g_gatt_db_table_mesh_proxy);
            if (BLE_SUCCESS != status)
            {
                MESH_SERVER_LOG("R_BLE_GATTS_SetDbInst is failed: 0x%04X", status);
            }

            /* Save the Callback Provided */
            proxy_cb = cb;

            /* Set the Current Service State */
            g_mesh_serv_state = MESH_PROXY_SERVICE_DONE;

            proxy_init_first_time = 0x01;
        }
    }

    return (status);
}

/***************************************************************************//**
* @brief Deinitializes Mesh Provisioning Service
*******************************************************************************/
ble_status_t mesh_serv_prov_deinit(void)
{
    uint8_t status = BLE_SUCCESS;

    /* Reset the Callback Provided */
    prov_cb = NULL;

    /**
     * Deregister Mesh Prov Service attribute list and
     * CBs from GATT Server Application
     */

    return ( status );
}

/***************************************************************************//**
* @brief Deinitializes Mesh Proxy Service
*******************************************************************************/
ble_status_t mesh_serv_proxy_deinit(void)
{
    uint8_t status = BLE_SUCCESS;

    /* Reset the Callback Provided */
    proxy_cb = NULL;

    /**
     * Deregister Mesh Prov Service attribute list and
     * CBs from GATT Server Application
     */

    return ( status );
}

/***************************************************************************//**
* @brief Sends a Mesh Provisioning Service Notification
*******************************************************************************/
ble_status_t mesh_prov_notify_data_out
             (
                 uint16_t  conn_hndl,
                 uint8_t   attidx,
                 uint8_t   * val,
                 uint8_t   val_len
              )
{
    uint16_t cli_cnfg;
    ble_status_t ret;

    /* Get the PROV DATA OUT CCD Value */
    mesh_serv_get_cccd(conn_hndl, MESH_PROV_DATA_OUT_CCD_HDL, &cli_cnfg);

    /* NOTE: Currently the attidx is ignored */
    (void)attidx;

    if (cli_cnfg == BLE_GATTS_CLI_CNFG_NOTIFICATION)
    {
        st_ble_gatt_hdl_value_pair_t ntf_data = {
            .attr_hdl        = MESH_PROV_DATA_OUT_VAL_HDL,
            .value.p_value   = val,
            .value.value_len = val_len,
        };
        ret = R_BLE_GATTS_Notification(conn_hndl, &ntf_data);
    }
    else
    {
        ret = BLE_ERR_INVALID_STATE;
    }

    return ret;
}

/***************************************************************************//**
* @brief Sends a Mesh Proxy Service Notification
*******************************************************************************/
ble_status_t mesh_proxy_notify_data_out
          (
              uint16_t  conn_hndl,
              uint8_t   attidx,
              uint8_t   * val,
              uint8_t   val_len
          )
{
    uint16_t cli_cnfg;
    ble_status_t ret;

    /* Get the PROXY DATA OUT CCD Value */
    mesh_serv_get_cccd(conn_hndl, MESH_PROXY_DATA_OUT_CCD_HDL, &cli_cnfg);

    /* NOTE: Currently the attidx is ignored */
    (void)attidx;

    if (cli_cnfg == BLE_GATTS_CLI_CNFG_NOTIFICATION)
    {
        st_ble_gatt_hdl_value_pair_t ntf_data = {
            .attr_hdl        = MESH_PROXY_DATA_OUT_VAL_HDL,
            .value.p_value   = val,
            .value.value_len = val_len,
        };
        ret = R_BLE_GATTS_Notification(conn_hndl, &ntf_data);
    }
    else
    {
        ret = BLE_ERR_INVALID_STATE;
    }

    return ret;
}

static void mesh_prov_handle_conn( uint16_t connHandle, uint8_t changeType )
{
    /* Reset the PROV DATA OUT CCD Value */
    if (MS_TRUE == changeType)
    {
        mesh_serv_set_cccd
        (
            connHandle,
            MESH_PROV_DATA_OUT_CCD_HDL,
            BLE_GATTS_CLI_CNFG_DEFAULT
        );
    }
}

static void mesh_proxy_handle_conn( uint16_t connHandle, uint8_t changeType )
{
    /* Reset the PROXY DATA OUT CCD Value */
    if (MS_TRUE == changeType)
    {
        mesh_serv_set_cccd
        (
            connHandle,
            MESH_PROXY_DATA_OUT_CCD_HDL,
            BLE_GATTS_CLI_CNFG_DEFAULT
        );
    }
}


