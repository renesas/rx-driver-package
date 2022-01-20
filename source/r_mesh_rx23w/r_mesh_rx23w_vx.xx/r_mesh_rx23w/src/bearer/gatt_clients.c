
/**
 *  \file gatt_clients.c
 */

/*
 *  Copyright (C) 2018-2021. Mindtree Limited.
 *  All rights reserved.
 */

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "gatt_clients.h"
#include "blebrr.h"

#if BLEBRR_GATT_CLIENT

/*******************************************************************************
* Global Variables and Private Functions declaration
*******************************************************************************/
/* Mesh GATT Bearer Related Service Assigned Numbers as arrays */
const uint8_t MESH_PROV_SERVICE_UUID16   [BLE_GATT_16_BIT_UUID_SIZE] = { 0x27, 0x18};
const uint8_t MESH_PROXY_SERVICE_UUID16  [BLE_GATT_16_BIT_UUID_SIZE] = { 0x28, 0x18};

/* GATT Service UUID */
const uint8_t BLE_GATT_SERVICE_UUID16[BLE_GATT_16_BIT_UUID_SIZE] = { 0x01, 0x18 };

static void mesh_client_prov_disc_cb
            (
                uint16_t conn_hdl,
                uint8_t  serv_idx,
                uint16_t type,
                void     * p_param
            );
static void mesh_client_proxy_disc_cb
            (
                uint16_t conn_hdl,
                uint8_t  serv_idx,
                uint16_t type,
                void     * p_param
            );
static void mesh_client_common_disc_cb
            (
                uint16_t conn_hdl,
                uint8_t  serv_idx,
                uint16_t type,
                void     * p_param
            );
static void mesh_clients_gattc_cb
            (
                uint16_t                type,
                ble_status_t            result,
                st_ble_gattc_evt_data_t * data
            );
static void mesh_client_disc_comp_cb_pl (uint16_t conn_hdl);

/* Global variable definition */
static mesh_client_env_t mesh_client_env[MESH_MAX_CLIENT_ENV];

/* Mesh Provisioning Service Client Related Callbacks */
static mesh_gatt_client_cb * prov_client_cb;

/* Mesh Proxy Service Client Related Callbacks */
static mesh_gatt_client_cb * proxy_client_cb;

/* GATT MTU Exchange callback */
static mesh_gatt_client_exchange_mtu_cb exchange_mtu_cb;

static const st_ble_disc_entry_t mesh_client_prov_entry[] =
{
    {
        .p_uuid    = (uint8_t *)MESH_PROV_SERVICE_UUID16,
        .uuid_type = BLE_GATT_16_BIT_UUID_FORMAT,
        .serv_cb   = mesh_client_prov_disc_cb,
    },
    {
        .p_uuid    = (uint8_t *)BLE_GATT_SERVICE_UUID16,
        .uuid_type = BLE_GATT_16_BIT_UUID_FORMAT,
        .serv_cb   = mesh_client_common_disc_cb,
    }
};

static const st_ble_disc_entry_t mesh_client_proxy_entry[] =
{
    {
        .p_uuid    = (uint8_t *)MESH_PROXY_SERVICE_UUID16,
        .uuid_type = BLE_GATT_16_BIT_UUID_FORMAT,
        .serv_cb   = mesh_client_proxy_disc_cb,
    },
    {
        .p_uuid    = (uint8_t *)BLE_GATT_SERVICE_UUID16,
        .uuid_type = BLE_GATT_16_BIT_UUID_FORMAT,
        .serv_cb   = mesh_client_common_disc_cb,
    }
};

/*******************************************************************************
* Functions
*******************************************************************************/
/***************************************************************************//**
* @brief Initializes GATT Client environment
*******************************************************************************/
void mesh_client_init(void)
{
    for (uint16_t i = 0; i < MESH_MAX_CLIENT_ENV; i++)
    {
        /* Reset the application manager environment */
        memset(&mesh_client_env[i], 0, sizeof(mesh_client_env_t));

        mesh_client_env[i].conn_hdl = BLE_GAP_INVALID_CONN_HDL;
    }

    prov_client_cb  = NULL;
    proxy_client_cb = NULL;

    /* Initialize Discovery. */
    R_BLE_DISC_Init();

    R_BLE_GATTC_RegisterCb(mesh_clients_gattc_cb, 2);
}

static mesh_client_env_t * mesh_client_find_env(uint16_t conn_hdl)
{
    for (uint16_t i = 0; i < MESH_MAX_CLIENT_ENV; i++)
    {
        if (mesh_client_env[i].conn_hdl == conn_hdl)
        {
            return &mesh_client_env[i];
        }
    }
    return NULL;
}

/***************************************************************************//**
* @brief Initializes Mesh Provisioning Client
*******************************************************************************/
void mesh_client_prov_init(mesh_gatt_client_cb * cb)
{
    /* Register the upper layer provided Provisioning Client Callbacks */
    if (NULL != cb)
    {
        prov_client_cb = cb;
    }
}

/***************************************************************************//**
* @brief Initializes Mesh Proxy Client
*******************************************************************************/
void mesh_client_proxy_init(mesh_gatt_client_cb * cb)
{
    /* Register the upper layer provided Provisioning Client Callbacks */
    if (NULL != cb)
    {
        proxy_client_cb = cb;
    }
}

/***************************************************************************//**
* @brief Request the peer GATT Server to expand MTU size
*******************************************************************************/
ble_status_t mesh_client_expand_mtu(uint16_t conn_hdl, mesh_gatt_client_exchange_mtu_cb cb)
{
    exchange_mtu_cb = cb;
    return R_BLE_GATTC_ReqExMtu(conn_hdl, BLE_CFG_GATT_MTU_SIZE);
}

/***************************************************************************//**
* @brief Starts Mesh Service Discovery
*******************************************************************************/
ble_status_t mesh_client_discover_services
           (
               uint16_t conn_hdl,
               uint8_t  serv_mode
           )
{
    ble_status_t status;
    mesh_client_env_t * client_env;

    client_env = mesh_client_find_env(conn_hdl);
    if (NULL == client_env)
    {
        /* client_env not found */
        return BLE_ERR_NOT_FOUND;
    }

    client_env->curr_disc_mode = serv_mode;
    client_env->start_hdl = BLE_GATT_INVALID_ATTR_HDL_VAL;
    client_env->end_hdl = BLE_GATT_INVALID_ATTR_HDL_VAL;
    client_env->data_in_hdl = BLE_GATT_INVALID_ATTR_HDL_VAL;
    client_env->data_out_hdl = BLE_GATT_INVALID_ATTR_HDL_VAL;
    client_env->data_out_cccd_hdl = BLE_GATT_INVALID_ATTR_HDL_VAL;
    client_env->serv_changed_hdl = BLE_GATT_INVALID_ATTR_HDL_VAL;
    client_env->serv_changed_cccd_hdl = BLE_GATT_INVALID_ATTR_HDL_VAL;

    status = R_BLE_DISC_Start(conn_hdl,
            (BLEBRR_GATT_PROV_MODE == serv_mode) ?
                mesh_client_prov_entry : mesh_client_proxy_entry,
            2, mesh_client_disc_comp_cb_pl);

    MESH_CLIENT_LOG(
        "Discovery Primary Service UUID 0x%04X returned with retval 0x%04X\n",
        (BLEBRR_GATT_PROV_MODE == serv_mode) ?
                MESH_PROVISIONING_SERVICE : MESH_PROXY_SERVICE, status);

    return status;
}

/***************************************************************************//**
* @brief Sends a Write Without Response
*******************************************************************************/
ble_status_t mesh_client_send_wwr
     (
         uint16_t  conn_hdl,
         uint8_t   * value,
         uint16_t  length,
         uint8_t   mode
     )
{
    ble_status_t status;
    mesh_client_env_t * client_env;
    st_ble_gatt_hdl_value_pair_t req;

    client_env = mesh_client_find_env(conn_hdl);
    if (NULL == client_env)
    {
        /* client_env not found */
        return BLE_ERR_NOT_FOUND;
    }
    if (mode != client_env->curr_disc_mode)
    {
        /* client_env mode mismatch */
        return BLE_ERR_INVALID_MODE;
    }

    /* Assign the Handle Based on the Service Preference for Write */
    req.attr_hdl = client_env->data_in_hdl;

    req.value.value_len = length;
    req.value.p_value   = value;

    status = R_BLE_GATTC_WriteCharWithoutRsp(conn_hdl, &req);

    MESH_CLIENT_LOG(
        "Writing data : len 0x%04X : handle 0x%02X : ret 0x%04X\n",
        req.value.value_len, req.attr_hdl, status);

    return status;
}

/***************************************************************************//**
* @brief Sends a Write Characteristic Value to either enable or disable Notification
*******************************************************************************/
ble_status_t mesh_client_config_ntf
           (
               uint16_t  conn_hdl,
               uint8_t   flag,
               uint8_t   mode
           )
{
    ble_status_t status;
    mesh_client_env_t * client_env;
    st_ble_gatt_hdl_value_pair_t req;
    uint8_t                      cccd_value[2];

    client_env = mesh_client_find_env(conn_hdl);
    if (NULL == client_env)
    {
        /* client_env not found */
        return BLE_ERR_NOT_FOUND;
    }
    if (mode != client_env->curr_disc_mode)
    {
        /* client_env mode mismatch */
        return BLE_ERR_INVALID_MODE;
    }

    BT_PACK_LE_2_BYTE_VAL(cccd_value,
        (MS_TRUE == flag) ? BLE_GATTS_CLI_CNFG_NOTIFICATION : BLE_GATTS_CLI_CNFG_DEFAULT);

    req.attr_hdl = client_env->data_out_cccd_hdl;
    req.value.p_value = cccd_value;
    req.value.value_len = 2;

    /* Store CCCD State in global */
    client_env->curr_notif_state = flag;

    status = R_BLE_GATTC_WriteChar(conn_hdl, &req);

    MESH_CLIENT_LOG(
        "Writing %d to CCCD 0x%04X for Mode 0x%02X\n",
        flag, req.attr_hdl, mode);

    return status;
}

/***************************************************************************//**
* @brief Sends a Write Characteristic Value to either enable or disable Service Changed Indication
*******************************************************************************/
ble_status_t mesh_client_config_serv_changed
           (
               uint16_t  conn_hdl,
               uint8_t   flag
           )
{
    ble_status_t status;
    mesh_client_env_t * client_env;
    st_ble_gatt_hdl_value_pair_t req;
    uint8_t                      cccd_value[2];

    client_env = mesh_client_find_env(conn_hdl);
    if (NULL == client_env)
    {
        /* client_env not found */
        return API_FAILURE;
    }

    BT_PACK_LE_2_BYTE_VAL(cccd_value,
        (MS_TRUE == flag) ? BLE_GATTS_CLI_CNFG_INDICATION : BLE_GATTS_CLI_CNFG_DEFAULT);

    req.attr_hdl = client_env->serv_changed_cccd_hdl;
    req.value.p_value = cccd_value;
    req.value.value_len = 2;

    status = R_BLE_GATTC_WriteChar(conn_hdl, &req);

    MESH_CLIENT_LOG(
        "Writing %d to CCCD 0x%04X for Service Changed\n",
        flag, req.attr_hdl);

    return status;
}

static void mesh_client_prov_disc_cb
            (
                uint16_t conn_hdl,
                uint8_t  serv_idx,
                uint16_t type,
                void     * p_param
            )
{
    mesh_client_env_t * client_env;

    client_env = mesh_client_find_env(conn_hdl);
    if ((NULL == client_env) || (BLEBRR_GATT_PROV_MODE != client_env->curr_disc_mode))
    {
        /* client_env not found or mode mismatch */
        return;
    }

    switch (type)
    {
        case BLE_DISC_PRIM_SERV_FOUND:
        {
            st_disc_serv_param_t *serv_param = (st_disc_serv_param_t *)p_param;
            if (MESH_PROVISIONING_SERVICE == serv_param->value.serv_16.uuid_16)
            {
                MESH_CLIENT_LOG(
                    "\nMESH Provisioning Services Found!\n");
                MESH_CLIENT_LOG(
                    "Service Handles are 0x%04X :: 0x%04X\n",
                    serv_param->value.serv_16.range.start_hdl,
                    serv_param->value.serv_16.range.end_hdl);

                client_env->start_hdl = serv_param->value.serv_16.range.start_hdl;
                client_env->end_hdl   = serv_param->value.serv_16.range.end_hdl;
                MESH_CLIENT_LOG(
                    "Prov Service Start Handle is 0x%04X\n",
                    client_env->start_hdl);
                MESH_CLIENT_LOG(
                    "Prov Service End Handle is 0x%04X\n\n",
                    client_env->end_hdl);
            }
        } break;

        case BLE_DISC_CHAR_FOUND:
        {
            st_disc_char_param_t *char_param = (st_disc_char_param_t *)p_param;

            if (char_param->uuid_type == BLE_GATT_16_BIT_UUID_FORMAT)
            {
                /* Find the <<Mesh Provisioning Data Out>> Characteristic */
                if (MESH_CH_PROVISIONING_DATA_OUT == char_param->value.char_16.uuid_16)
                {
                    client_env->data_out_hdl = char_param->value.char_16.value_hdl;
                    for (uint8_t pos = 0; pos < char_param->num_of_descs; pos++)
                    {
                        if (char_param->descs[pos].uuid_type == BLE_GATT_16_BIT_UUID_FORMAT)
                        {
                            /* Find the <<Client Characteristic Configuration>> Characteristic Descriptor */
                            if (MESH_BLE_UUID_CCCD == char_param->descs[pos].value.desc_16.uuid_16)
                            {
                                client_env->data_out_cccd_hdl = char_param->descs[pos].value.desc_16.desc_hdl;
                                break;
                            }
                        }
                    }
                }
                /* Find the <<Mesh Provisioning Data In>> Characteristic */
                else if (MESH_CH_PROVISIONING_DATA_IN == char_param->value.char_16.uuid_16)
                {
                    client_env->data_in_hdl = char_param->value.char_16.value_hdl;
                }
                else
                {
                    /* Do nothing. */
                }
            }
        } break;

        default:
            break;
    }
}

static void mesh_client_proxy_disc_cb
            (
                uint16_t conn_hdl,
                uint8_t  serv_idx,
                uint16_t type,
                void     * p_param
            )
{
    mesh_client_env_t * client_env;

    client_env = mesh_client_find_env(conn_hdl);
    if ((NULL == client_env) || (BLEBRR_GATT_PROXY_MODE != client_env->curr_disc_mode))
    {
        /* client_env not found or mode mismatch */
        return;
    }

    switch (type)
    {
        case BLE_DISC_PRIM_SERV_FOUND:
        {
            st_disc_serv_param_t *serv_param = (st_disc_serv_param_t *)p_param;
            if (MESH_PROXY_SERVICE == serv_param->value.serv_16.uuid_16)
            {
                MESH_CLIENT_LOG(
                    "\nMESH Proxy Services Found!\n");
                MESH_CLIENT_LOG(
                    "Service Handles are 0x%04X :: 0x%04X\n",
                    serv_param->value.serv_16.range.start_hdl,
                    serv_param->value.serv_16.range.end_hdl);

                client_env->start_hdl = serv_param->value.serv_16.range.start_hdl;
                client_env->end_hdl   = serv_param->value.serv_16.range.end_hdl;
                MESH_CLIENT_LOG(
                    "Proxy Service Start Handle is 0x%04X\n",
                    client_env->start_hdl);
                MESH_CLIENT_LOG(
                    "Proxy Service End Handle is 0x%04X\n\n",
                    client_env->end_hdl);
            }
        } break;

        case BLE_DISC_CHAR_FOUND:
        {
            st_disc_char_param_t *char_param = (st_disc_char_param_t *)p_param;

            if (char_param->uuid_type == BLE_GATT_16_BIT_UUID_FORMAT)
            {
                /* Find the <<Mesh Proxy Data Out>> Characteristic */
                if (MESH_CH_PROXY_DATA_OUT == char_param->value.char_16.uuid_16)
                {
                    client_env->data_out_hdl = char_param->value.char_16.value_hdl;
                    for (uint8_t pos = 0; pos < char_param->num_of_descs; pos++)
                    {
                        if (char_param->descs[pos].uuid_type == BLE_GATT_16_BIT_UUID_FORMAT)
                        {
                            /* Find the <<Client Characteristic Configuration>> Characteristic Descriptor */
                            if (MESH_BLE_UUID_CCCD == char_param->descs[pos].value.desc_16.uuid_16)
                            {
                                client_env->data_out_cccd_hdl = char_param->descs[pos].value.desc_16.desc_hdl;
                                break;
                            }
                        }
                    }
                }
                /* Find the <<Mesh Proxy Data In>> Characteristic */
                else if (MESH_CH_PROXY_DATA_IN == char_param->value.char_16.uuid_16)
                {
                    client_env->data_in_hdl = char_param->value.char_16.value_hdl;
                }
                else
                {
                    /* Do nothing. */
                }
            }
        } break;

        default:
           break;
    }
}

static void mesh_client_common_disc_cb
            (
                uint16_t conn_hdl,
                uint8_t  serv_idx,
                uint16_t type,
                void     * p_param
            )
{
    mesh_client_env_t * client_env;

    client_env = mesh_client_find_env(conn_hdl);
    if (NULL == client_env)
    {
        /* client_env not found */
        return;
    }

    switch (type)
    {
        case BLE_DISC_CHAR_FOUND:
        {
            st_disc_char_param_t *char_param = (st_disc_char_param_t *)p_param;

            if (BLE_GATT_16_BIT_UUID_FORMAT == char_param->uuid_type)
            {
                /* Found the <<Service Changed>> Characteristic */
                if (MESH_BLE_UUID_SERV_CHANGED == char_param->value.char_16.uuid_16)
                {
                    client_env->serv_changed_hdl = char_param->value.char_16.value_hdl;
                    for (uint8_t pos = 0; pos < char_param->num_of_descs; pos++)
                    {
                        if (BLE_GATT_16_BIT_UUID_FORMAT == char_param->descs[pos].uuid_type)
                        {
                            /* Found the <<Client Characteristic Configuration>> Characteristic Descriptor */
                            if (MESH_BLE_UUID_CCCD == char_param->descs[pos].value.desc_16.uuid_16)
                            {
                                client_env->serv_changed_cccd_hdl = char_param->descs[pos].value.desc_16.desc_hdl;
                                break;
                            }
                        }
                    }
                }
            }
        } break;

        default:
           break;
    }
}

static void mesh_clients_gattc_cb
            (
                uint16_t                type,
                ble_status_t            result,
                st_ble_gattc_evt_data_t * data
            )
{
    mesh_client_env_t * client_env;

    if (type != BLE_GATTC_EVENT_CONN_IND)
    {
        client_env = mesh_client_find_env(data->conn_hdl);
        if (NULL == client_env)
        {
            /* client_env not found */
            return;
        }
    }

    switch (type)
    {
        case BLE_GATTC_EVENT_CONN_IND:
        {
            /* Register the connection handle with mesh_client_env[] */
            client_env = mesh_client_find_env(BLE_GAP_INVALID_CONN_HDL);
            if (NULL != client_env)
            {
                client_env->conn_hdl = data->conn_hdl;
            }
        } break;

        case BLE_GATTC_EVENT_DISCONN_IND:
        {
            client_env->conn_hdl = BLE_GAP_INVALID_CONN_HDL;
        } break;

        case BLE_GATTC_EVENT_EX_MTU_RSP:
        {
            exchange_mtu_cb(data->conn_hdl);
        } break;

        case BLE_GATTC_EVENT_CHAR_WRITE_RSP:
        {
            st_ble_gattc_wr_char_evt_t *wr_char_evt_param =
                (st_ble_gattc_wr_char_evt_t *)data->p_param;

            uint16_t attr_hdl = wr_char_evt_param->value_hdl;

            if (client_env->data_out_cccd_hdl == attr_hdl)
            {
                if (BLEBRR_GATT_PROV_MODE == client_env->curr_disc_mode)
                {
                    /* Call the Prov CCCD ntf complete Callback */
                    if (NULL != prov_client_cb)
                    {
                        prov_client_cb->ntf_status
                        (
                            client_env->conn_hdl,
                            client_env->curr_notif_state,
                            0x00
                        );
                    }
                }
                else if (BLEBRR_GATT_PROXY_MODE == client_env->curr_disc_mode)
                {
                    /* Call the Proxy CCCD ntf complete Callback */
                    if (NULL != proxy_client_cb)
                    {
                        proxy_client_cb->ntf_status
                        (
                            client_env->conn_hdl,
                            client_env->curr_notif_state,
                            0x00
                        );
                    }
                }
                else
                {
                    /* DO Nothing ! */
                }
            }
        } break;

        case BLE_GATTC_EVENT_HDL_VAL_NTF:
        {
            st_ble_gattc_ntf_evt_t *ntf_evt_param =
                (st_ble_gattc_ntf_evt_t *)data->p_param;

            uint16_t attr_hdl = ntf_evt_param->data.attr_hdl;

            if (ntf_evt_param->data.value.value_len > 0)
            {
                /* Check the Attribute Handle:
                 * - If Provisioning Data Out Handle: then call the Prov Callback
                 * - If Proxy Data Out Handle: the ncall the Proxy Callback
                 */
                if (client_env->data_out_hdl == attr_hdl)
                {
                    if (BLEBRR_GATT_PROV_MODE == client_env->curr_disc_mode)
                    {
                        if (NULL != prov_client_cb)
                        {
                            prov_client_cb->data_out_notif
                            (
                                client_env->conn_hdl,
                                ntf_evt_param->data.value.value_len,
                                ntf_evt_param->data.value.p_value
                            );
                        }
                    }
                    else if (BLEBRR_GATT_PROXY_MODE == client_env->curr_disc_mode)
                    {
                        if (NULL != proxy_client_cb)
                        {
                            proxy_client_cb->data_out_notif
                            (
                                client_env->conn_hdl,
                                ntf_evt_param->data.value.value_len,
                                ntf_evt_param->data.value.p_value
                            );
                        }
                    }
                }
            }
        } break;

        case BLE_GATTC_EVENT_HDL_VAL_IND:
        {
            st_ble_gattc_ind_evt_t *ind_evt_param =
                (st_ble_gattc_ind_evt_t *)data->p_param;

            if (client_env->serv_changed_hdl == ind_evt_param->data.attr_hdl)
            {
                if (BLEBRR_GATT_PROV_MODE == client_env->curr_disc_mode)
                {
                    if (NULL != prov_client_cb)
                    {
                        prov_client_cb->serv_changed(client_env->conn_hdl);
                    }
                }
                else if (BLEBRR_GATT_PROXY_MODE == client_env->curr_disc_mode)
                {
                    if (NULL != proxy_client_cb)
                    {
                        proxy_client_cb->serv_changed(client_env->conn_hdl);
                    }
                }
            }
        } break;

        default:
            break;
    }
}

static void mesh_client_disc_comp_cb_pl (uint16_t conn_hdl)
{
    mesh_client_env_t * client_env;
    mesh_gatt_client_disc_comp_cb disc_comp_cb = NULL;
    API_RESULT status;

    client_env = mesh_client_find_env(conn_hdl);
    if (NULL == client_env)
    {
        /* client_env not found */
        return;
    }

    if (BLEBRR_GATT_PROV_MODE == client_env->curr_disc_mode)
    {
        if (NULL != prov_client_cb)
        {
            disc_comp_cb = prov_client_cb->disc_comp;
        }
    }
    else if (BLEBRR_GATT_PROXY_MODE == client_env->curr_disc_mode)
    {
        if (NULL != proxy_client_cb)
        {
            disc_comp_cb = proxy_client_cb->disc_comp;
        }
    }
    else
    {
        /* Do Nothing */
    }

    /* Call the Service Discovery Complete Callback */
    if (NULL != disc_comp_cb)
    {
        /* Are all of the Mesh Service Attribute Handles found? */
        status =
            ((BLE_GATT_INVALID_ATTR_HDL_VAL != client_env->start_hdl) &&
             (BLE_GATT_INVALID_ATTR_HDL_VAL != client_env->end_hdl) &&
             (BLE_GATT_INVALID_ATTR_HDL_VAL != client_env->data_in_hdl) &&
             (BLE_GATT_INVALID_ATTR_HDL_VAL != client_env->data_out_hdl) &&
             (BLE_GATT_INVALID_ATTR_HDL_VAL != client_env->data_out_cccd_hdl)) ?
                API_SUCCESS : API_FAILURE;

        disc_comp_cb(client_env->conn_hdl, status);
    }
}

#endif /* BLEBRR_GATT_CLIENT */
