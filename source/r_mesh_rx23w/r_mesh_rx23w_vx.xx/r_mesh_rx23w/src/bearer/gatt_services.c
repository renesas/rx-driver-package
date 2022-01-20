
/**
 *  \file gatt_services.c
 */

/*
 *  Copyright (C) 2018-2021. Mindtree Limited.
 *  All rights reserved.
 */

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "gatt_services.h"

/*******************************************************************************
* Macro definitions
*******************************************************************************/
#if BLE_MESH_PROVS_DECL_HDL < BLE_MESH_PROXYS_DECL_HDL
#define BLEBRR_MESH_SERV_ATTR_HDL_START     BLE_MESH_PROVS_DECL_HDL
#define BLEBRR_MESH_SERV_ATTR_HDL_END       BLE_MESH_PROXYS_DATA_OUT_CLI_CNFG_DESC_HDL
#else /* BLE_MESH_PROVS_DECL_HDL > BLE_MESH_PROXYS_DECL_HDL */
#define BLEBRR_MESH_SERV_ATTR_HDL_START     BLE_MESH_PROXYS_DECL_HDL
#define BLEBRR_MESH_SERV_ATTR_HDL_END       BLE_MESH_PROVS_DATA_OUT_CLI_CNFG_DESC_HDL
#endif

/*******************************************************************************
* Global Variables and Private Functions declaration
*******************************************************************************/
static mesh_gatt_cb * prov_cb;
static mesh_gatt_cb * proxy_cb;

static uint8_t gs_mesh_serv_mode = BLEBRR_GATT_UNINIT_MODE;

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
    st_ble_gatt_value_t gatt_value = {
        .p_value     = data,
        .value_len = 2,
    };

    BT_PACK_LE_2_BYTE(data, &cli_cnfg);
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
            if (BLE_MESH_PROVS_DATA_IN_VAL_HDL == params->attr_hdl)
            {
                if (NULL != prov_cb)
                {
                    prov_cb->data_in_cb
                    (
                        conn_hdl,
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
            if (BLE_MESH_PROXYS_DATA_IN_VAL_HDL == params->attr_hdl)
            {
                if (NULL != proxy_cb)
                {
                    proxy_cb->data_in_cb
                    (
                        conn_hdl,
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

    MS_IGNORE_UNUSED_PARAM(result);
    MS_IGNORE_UNUSED_PARAM(status);

    MESH_SERVER_LOG("\nmesh_serv_gatts_cb :: type 0x%04X: Service 0x%02X \n",
    type, gs_mesh_serv_mode);

    if (BLE_GATTS_EVENT_CONN_IND != type)
    {
        if (NULL == blebrr_find_gatt_env_by_connhdl(data->conn_hdl))
        {
            /* not found */
            return;
        }
    }

    switch (type)
    {
        case BLE_GATTS_EVENT_CONN_IND:
        {
            MESH_SERVER_LOG("[MESH SERV] Received BLE_GATTS_EVENT_CONN_IND");

            if (BLEBRR_GATT_PROV_MODE == gs_mesh_serv_mode)
            {
                /* Reset the PROV DATA OUT CCCD Value */
                mesh_serv_set_cccd(data->conn_hdl, BLE_MESH_PROVS_DATA_OUT_CLI_CNFG_DESC_HDL, BLE_GATTS_CLI_CNFG_DEFAULT);
            }
            else if (BLEBRR_GATT_PROXY_MODE == gs_mesh_serv_mode)
            {
                /* Reset the PROXY DATA OUT CCCD Value */
                mesh_serv_set_cccd(data->conn_hdl, BLE_MESH_PROXYS_DATA_OUT_CLI_CNFG_DESC_HDL, BLE_GATTS_CLI_CNFG_DEFAULT);
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
            /* Responding with Configured MTU Size */
            status = R_BLE_GATTS_RspExMtu(data->conn_hdl, BLE_CFG_GATT_MTU_SIZE);

            MESH_SERVER_LOG("R_BLE_GATTS_RspExMtu Sent with MTU: %d, Status : 0x%04X",
            BLE_CFG_GATT_MTU_SIZE, status);
        }
        break;

        case BLE_GATTS_EVENT_DB_ACCESS_IND:
        {
            st_ble_gatts_db_access_evt_t * p_db_access =
                (st_ble_gatts_db_access_evt_t *)data->p_param;

            /* Check for Current Active Service */
            if (BLEBRR_GATT_PROV_MODE == gs_mesh_serv_mode)
            {
                mesh_prov_gatt_db_cb
                (
                    data->conn_hdl,
                    p_db_access->p_params
                );
            }
            else if (BLEBRR_GATT_PROXY_MODE == gs_mesh_serv_mode)
            {
                mesh_proxy_gatt_db_cb
                (
                    data->conn_hdl,
                    p_db_access->p_params
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

            /* Check for Current Active Service */
            if (BLEBRR_GATT_PROV_MODE == gs_mesh_serv_mode)
            {
                /* Check if the Write RSP if for Prov CCCD */
                if (BLE_MESH_PROVS_DATA_OUT_CLI_CNFG_DESC_HDL == db_wt_rsp_evt_param->attr_hdl)
                {
                    /* Get the PROXY DATA OUT CCCD Value */
                    mesh_serv_get_cccd
                    (
                        data->conn_hdl,
                        BLE_MESH_PROVS_DATA_OUT_CLI_CNFG_DESC_HDL,
                        &cli_cnfg
                    );

                    /* Invoke CCCD Update Callback */
                    if (NULL != prov_cb)
                    {
                        prov_cb->data_out_ccd_cb
                        (
                            data->conn_hdl,
                            (uint8_t)((cli_cnfg == BLE_GATTS_CLI_CNFG_NOTIFICATION) ? MS_TRUE : MS_FALSE)
                        );
                    }
                }
            }
            else if (BLEBRR_GATT_PROXY_MODE == gs_mesh_serv_mode)
            {
                /* Check if the Write RSP if for Proxy CCCD */
                if (BLE_MESH_PROXYS_DATA_OUT_CLI_CNFG_DESC_HDL == db_wt_rsp_evt_param->attr_hdl)
                {
                    /* Get the PROXY DATA OUT CCCD Value */
                    mesh_serv_get_cccd
                    (
                        data->conn_hdl,
                        BLE_MESH_PROXYS_DATA_OUT_CLI_CNFG_DESC_HDL,
                        &cli_cnfg
                    );

                    /* Invoke CCCD Update Callback */
                    if (NULL != proxy_cb)
                    {
                        proxy_cb->data_out_ccd_cb
                        (
                            data->conn_hdl,
                            (uint8_t)((cli_cnfg == BLE_GATTS_CLI_CNFG_NOTIFICATION) ? MS_TRUE : MS_FALSE)
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
    /* Initialize GATT Database */
    R_BLE_GATTS_SetDbInst(&g_gatt_db_table);

    /* Register Internal Mesh Services Callback to GATT Server */
    R_BLE_GATTS_RegisterCb(mesh_serv_gatts_cb, priority);

    gs_mesh_serv_mode = BLEBRR_GATT_UNINIT_MODE;

    return BLE_SUCCESS;
}

/***************************************************************************//**
* @brief Gets current MTU size
*******************************************************************************/
uint16_t mesh_serv_get_mtu(uint16_t conn_hdl)
{
    ble_status_t status;
    uint16_t mtu;

    status = R_BLE_GATT_GetMtu(conn_hdl, &mtu);
    return (BLE_SUCCESS == status) ? mtu : 0;
}

/***************************************************************************//**
* @brief Sets Mesh Provisioning Service instance to BLE Protocol Stack
*******************************************************************************/
ble_status_t mesh_serv_prov_init(mesh_gatt_cb *cb)
{
    if (NULL != cb)
    {
        /* Enable Mesh Provisioning Service */
        g_gatt_db_attr_table[BLE_MESH_PROVS_DECL_HDL].aux_prop &= ~BLE_GATT_DB_ATTR_DISABLED;
        g_gatt_db_attr_table[BLE_MESH_PROVS_DATA_IN_DECL_HDL].aux_prop  &= ~BLE_GATT_DB_ATTR_DISABLED;
        g_gatt_db_attr_table[BLE_MESH_PROVS_DATA_IN_VAL_HDL].aux_prop   &= ~BLE_GATT_DB_ATTR_DISABLED;
        g_gatt_db_attr_table[BLE_MESH_PROVS_DATA_OUT_DECL_HDL].aux_prop &= ~BLE_GATT_DB_ATTR_DISABLED;
        g_gatt_db_attr_table[BLE_MESH_PROVS_DATA_OUT_VAL_HDL].aux_prop  &= ~BLE_GATT_DB_ATTR_DISABLED;
        g_gatt_db_attr_table[BLE_MESH_PROVS_DATA_OUT_CLI_CNFG_DESC_HDL].aux_prop &= ~BLE_GATT_DB_ATTR_DISABLED;

        /* Save the Callback Provided */
        prov_cb = cb;

        /* Set the Current Service State */
        gs_mesh_serv_mode = BLEBRR_GATT_PROV_MODE;
    }

    return BLE_SUCCESS;
}

/***************************************************************************//**
* @brief Sets Mesh Proxy Service instance to BLE Protocol Stack
*******************************************************************************/
ble_status_t mesh_serv_proxy_init(mesh_gatt_cb *cb)
{
    if (NULL != cb)
    {
        /* Enable Mesh Proxy Service */
        g_gatt_db_attr_table[BLE_MESH_PROXYS_DECL_HDL].aux_prop &= ~BLE_GATT_DB_ATTR_DISABLED;
        g_gatt_db_attr_table[BLE_MESH_PROXYS_DATA_IN_DECL_HDL].aux_prop  &= ~BLE_GATT_DB_ATTR_DISABLED;
        g_gatt_db_attr_table[BLE_MESH_PROXYS_DATA_IN_VAL_HDL].aux_prop   &= ~BLE_GATT_DB_ATTR_DISABLED;
        g_gatt_db_attr_table[BLE_MESH_PROXYS_DATA_OUT_DECL_HDL].aux_prop &= ~BLE_GATT_DB_ATTR_DISABLED;
        g_gatt_db_attr_table[BLE_MESH_PROXYS_DATA_OUT_VAL_HDL].aux_prop  &= ~BLE_GATT_DB_ATTR_DISABLED;
        g_gatt_db_attr_table[BLE_MESH_PROXYS_DATA_OUT_CLI_CNFG_DESC_HDL].aux_prop &= ~BLE_GATT_DB_ATTR_DISABLED;

        /* Save the Callback Provided */
        proxy_cb = cb;

        /* Set the Current Service State */
        gs_mesh_serv_mode = BLEBRR_GATT_PROXY_MODE;
    }

    return BLE_SUCCESS;
}

/***************************************************************************//**
* @brief Deinitializes Mesh Provisioning Service
*******************************************************************************/
ble_status_t mesh_serv_prov_deinit(void)
{
    /* Reset the Callback Provided */
    prov_cb = NULL;

    /* Disable Mesh Provisioning Service */
    g_gatt_db_attr_table[BLE_MESH_PROVS_DECL_HDL].aux_prop |= BLE_GATT_DB_ATTR_DISABLED;
    g_gatt_db_attr_table[BLE_MESH_PROVS_DATA_IN_DECL_HDL].aux_prop  |= BLE_GATT_DB_ATTR_DISABLED;
    g_gatt_db_attr_table[BLE_MESH_PROVS_DATA_IN_VAL_HDL].aux_prop   |= BLE_GATT_DB_ATTR_DISABLED;
    g_gatt_db_attr_table[BLE_MESH_PROVS_DATA_OUT_DECL_HDL].aux_prop |= BLE_GATT_DB_ATTR_DISABLED;
    g_gatt_db_attr_table[BLE_MESH_PROVS_DATA_OUT_VAL_HDL].aux_prop  |= BLE_GATT_DB_ATTR_DISABLED;
    g_gatt_db_attr_table[BLE_MESH_PROVS_DATA_OUT_CLI_CNFG_DESC_HDL].aux_prop |= BLE_GATT_DB_ATTR_DISABLED;

    return BLE_SUCCESS;
}

/***************************************************************************//**
* @brief Deinitializes Mesh Proxy Service
*******************************************************************************/
ble_status_t mesh_serv_proxy_deinit(void)
{
    /* Reset the Callback Provided */
    proxy_cb = NULL;

    /* Disable Mesh Proxy Service */
    g_gatt_db_attr_table[BLE_MESH_PROXYS_DECL_HDL].aux_prop |= BLE_GATT_DB_ATTR_DISABLED;
    g_gatt_db_attr_table[BLE_MESH_PROXYS_DATA_IN_DECL_HDL].aux_prop  |= BLE_GATT_DB_ATTR_DISABLED;
    g_gatt_db_attr_table[BLE_MESH_PROXYS_DATA_IN_VAL_HDL].aux_prop   |= BLE_GATT_DB_ATTR_DISABLED;
    g_gatt_db_attr_table[BLE_MESH_PROXYS_DATA_OUT_DECL_HDL].aux_prop |= BLE_GATT_DB_ATTR_DISABLED;
    g_gatt_db_attr_table[BLE_MESH_PROXYS_DATA_OUT_VAL_HDL].aux_prop  |= BLE_GATT_DB_ATTR_DISABLED;
    g_gatt_db_attr_table[BLE_MESH_PROXYS_DATA_OUT_CLI_CNFG_DESC_HDL].aux_prop |= BLE_GATT_DB_ATTR_DISABLED;

    return BLE_SUCCESS;
}

/***************************************************************************//**
* @brief Sends a Mesh Provisioning Service Notification
*******************************************************************************/
ble_status_t mesh_prov_notify_data_out
             (
                 uint16_t  conn_hndl,
                 uint16_t  attr_hdl,
                 uint8_t   * val,
                 uint8_t   val_len
              )
{
    uint16_t cli_cnfg;
    ble_status_t ret = BLE_ERR_INVALID_STATE;

    /* Get the PROV DATA OUT CCCD Value */
    mesh_serv_get_cccd(conn_hndl, BLE_MESH_PROVS_DATA_OUT_CLI_CNFG_DESC_HDL, &cli_cnfg);

    if (cli_cnfg == BLE_GATTS_CLI_CNFG_NOTIFICATION)
    {
        st_ble_gatt_hdl_value_pair_t ntf_data = {
            .attr_hdl        = attr_hdl,
            .value.p_value   = val,
            .value.value_len = val_len,
        };
        ret = R_BLE_GATTS_Notification(conn_hndl, &ntf_data);
    }

    return ret;
}

/***************************************************************************//**
* @brief Sends a Mesh Proxy Service Notification
*******************************************************************************/
ble_status_t mesh_proxy_notify_data_out
          (
              uint16_t  conn_hndl,
              uint16_t  attr_hdl,
              uint8_t   * val,
              uint8_t   val_len
          )
{
    uint16_t cli_cnfg;
    ble_status_t ret = BLE_ERR_INVALID_STATE;

    /* Get the PROXY DATA OUT CCCD Value */
    mesh_serv_get_cccd(conn_hndl, BLE_MESH_PROXYS_DATA_OUT_CLI_CNFG_DESC_HDL, &cli_cnfg);

    if (cli_cnfg == BLE_GATTS_CLI_CNFG_NOTIFICATION)
    {
        st_ble_gatt_hdl_value_pair_t ntf_data = {
            .attr_hdl        = attr_hdl,
            .value.p_value   = val,
            .value.value_len = val_len,
        };
        ret = R_BLE_GATTS_Notification(conn_hndl, &ntf_data);
    }

    return ret;
}

ble_status_t mesh_indicate_serv_changed(uint16_t conn_hdl)
{
    uint16_t cli_cnfg;
    uint16_t hdls[2] = {BLEBRR_MESH_SERV_ATTR_HDL_START, BLEBRR_MESH_SERV_ATTR_HDL_END};
    ble_status_t ret = BLE_ERR_INVALID_STATE;

    /* Get the SERVICE CHANGED CCCD Value */
    mesh_serv_get_cccd(conn_hdl, BLE_GATS_SERV_CHGED_CLI_CNFG_DESC_HDL, &cli_cnfg);

    if (cli_cnfg == BLE_GATTS_CLI_CNFG_INDICATION)
    {
        st_ble_gatt_hdl_value_pair_t ind_data = {
            .attr_hdl = BLE_GATS_SERV_CHGED_VAL_HDL,
            .value.p_value = (uint8_t *)hdls,
            .value.value_len = sizeof(hdls),
        };
        ret = R_BLE_GATTS_Indication(conn_hdl, &ind_data);
    }

    return ret;
}
