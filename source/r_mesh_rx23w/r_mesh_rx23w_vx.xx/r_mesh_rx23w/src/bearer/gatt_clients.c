
/**
 *  \file gatt_clients.c
 *
 *
 */

/*
 *  Copyright (C) 2018. Mindtree Limited.
 *  All rights reserved.
 */

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "gatt_clients.h"

/*******************************************************************************
* Global Variables and Private Functions declaration
*******************************************************************************/
/* Mesh GATT Bearer Related Service Assigned Numbers as arrays */
const uint8_t MESH_PROV_SERVICE_UUID16   [BLE_GATT_16_BIT_UUID_SIZE] = { 0x27, 0x18};
const uint8_t MESH_PROV_DATA_IN_UUID16   [BLE_GATT_16_BIT_UUID_SIZE] = { 0xDB, 0x2A};
const uint8_t MESH_PROV_DATA_OUT_UUID16  [BLE_GATT_16_BIT_UUID_SIZE] = { 0xDC, 0x2A};

const uint8_t MESH_PROXY_SERVICE_UUID16  [BLE_GATT_16_BIT_UUID_SIZE] = { 0x28, 0x18};
const uint8_t MESH_PROXY_DATA_IN_UUID16  [BLE_GATT_16_BIT_UUID_SIZE] = { 0xDD, 0x2A};
const uint8_t MESH_PROXY_DATA_OUT_UUID16 [BLE_GATT_16_BIT_UUID_SIZE] = { 0xDE, 0x2A};

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

static void mesh_clients_gattc_cb
            (
                uint16_t                type,
                ble_status_t            result,
                st_ble_gattc_evt_data_t * data
            );
static void mesh_client_disc_comp_cb_pl (uint16_t conn_hdl);

/* Global variable definition */
static struct mesh_client_env_tag mesh_client_env[MESH_MAX_CLIENT_ENV];

/* Mesh Provisioning Service Client Related Callbacks */
static mesh_client_prov_cb   * prov_client_cb;

/* Mesh Proxy Service Client Related Callbacks */
static mesh_client_proxy_cb  * proxy_client_cb;

/* Track current Service Discovery Entry */
static st_ble_disc_entry_t* mesh_client_curr_disc_entry;

static st_ble_disc_entry_t mesh_client_prov_entry =
{
    .p_uuid    = (uint8_t *)MESH_PROV_SERVICE_UUID16,
    .uuid_type = BLE_GATT_16_BIT_UUID_FORMAT,
    .serv_cb   = mesh_client_prov_disc_cb,
};

static st_ble_disc_entry_t mesh_client_proxy_entry =
{
    .p_uuid    = (uint8_t *)MESH_PROXY_SERVICE_UUID16,
    .uuid_type = BLE_GATT_16_BIT_UUID_FORMAT,
    .serv_cb   = mesh_client_proxy_disc_cb,
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
        memset(&mesh_client_env[i], 0, sizeof(struct mesh_client_env_tag));

        mesh_client_env[i].conn_hdl = BLE_GAP_INVALID_CONN_HDL;
    }

    prov_client_cb  = NULL;
    proxy_client_cb = NULL;

    /* Initialize Discovery. */
    R_BLE_DISC_Init();

    R_BLE_GATTC_RegisterCb(mesh_clients_gattc_cb, 2);
}

static uint16_t mesh_client_find_env(uint16_t conn_hdl)
{
    uint16_t i;

    for (i = 0; i < MESH_MAX_CLIENT_ENV; i++)
    {
        if (mesh_client_env[i].conn_hdl == conn_hdl)
        {
            break;
        }
    }
    return i;
}

/***************************************************************************//**
* @brief Initializes Mesh Provisioning Client
*******************************************************************************/
void mesh_client_prov_init(mesh_client_prov_cb * cb)
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
void mesh_client_proxy_init(mesh_client_proxy_cb * cb)
{
    /* Register the upper layer provided Provisioning Client Callbacks */
    if (NULL != cb)
    {
        proxy_client_cb = cb;
    }
}

/***************************************************************************//**
* @brief Starts Mesh Service Discovery
*******************************************************************************/
API_RESULT mesh_client_discover_services
           (
               uint16_t conn_hdl,
               uint8_t  serv_mode
           )
{
    #define SERVICE_UUID  (MESH_CLIENT_PROV_MODE == serv_mode) ? \
                UUID_MESH_PROVISIONING_SERVICE : UUID_MESH_PROXY_SERVICE

    ble_status_t ret;

    mesh_client_curr_disc_entry = (MESH_CLIENT_PROV_MODE == serv_mode) ?\
           &mesh_client_prov_entry : &mesh_client_proxy_entry;

    ret = R_BLE_DISC_Start(conn_hdl, mesh_client_curr_disc_entry, 1, mesh_client_disc_comp_cb_pl);

    MESH_CLIENT_LOG(
    "Discovery Primiary Service UUID 0x%04X "
    "returned with retval 0x%04X\n",
    SERVICE_UUID, ret);

    return (0 == ret) ? API_SUCCESS : API_FAILURE;
}

/***************************************************************************//**
* @brief Sends a Write Without Response
*******************************************************************************/
API_RESULT mesh_client_send_wwr
     (
         uint16_t  conn_hdl,
         uint8_t   * value,
         uint16_t  length,
         uint8_t   serv_pref
     )
{
    st_ble_gatt_hdl_value_pair_t req;
    ble_status_t     ret;
    uint16_t idx;

    idx = mesh_client_find_env(conn_hdl);
    if (MESH_MAX_CLIENT_ENV == idx)
    {
        /* not found */
        return API_FAILURE;
    }

    /* Assign the Handle Based on the Service Preference for Write */
    req.attr_hdl = (MESH_CLIENT_PROV_MODE == serv_pref) ?
                  mesh_client_env[idx].prov_data_in_hdl :
                  mesh_client_env[idx].proxy_data_in_hdl;

    req.value.value_len = length;
    req.value.p_value   = value;

    ret = R_BLE_GATTC_WriteCharWithoutRsp(conn_hdl, &req);

    MESH_CLIENT_LOG(
    "Writing data : len 0x%04X : handle 0x%02X : ret 0x%04X",
    req.value.value_len, req.attr_hdl, ret);

    return (0 == ret) ? API_SUCCESS : API_FAILURE;
}

/***************************************************************************//**
* @brief Sends a Write Characteristic Value to either enable or disable Notification
*******************************************************************************/
API_RESULT mesh_client_config_ntf
           (
               uint16_t  conn_hdl,
               uint8_t   serv_pref,
               uint8_t   flag
           )
{
    st_ble_gatt_hdl_value_pair_t req;
    ble_status_t                 ret;
    uint8_t                      byte_value[2];
    uint16_t idx;

    idx = mesh_client_find_env(conn_hdl);
    if (MESH_MAX_CLIENT_ENV == idx)
    {
        /* not found */
        return API_FAILURE;
    }

    /* Assign the Handle Based on the Service Preference for Write */
    /**
     * Increment the Handle by 1 as the CCCD is present in the handle next to
     * the Value handle
     */
    byte_value[0] = (MS_TRUE == flag) ? 0x01 : 0x00;
    byte_value[1] = 0x00;

    req.attr_hdl = (MESH_CLIENT_PROV_MODE == serv_pref) ?
                   mesh_client_env[idx].prov_data_out_cccd_hdl :
                   mesh_client_env[idx].proxy_data_out_cccd_hdl;
    req.value.p_value = byte_value;
    req.value.value_len = 2;

    /* Store CCCD Mode and State in global */
    mesh_client_env[idx].curr_notif_state = flag;
    mesh_client_env[idx].curr_notif_mode  = (MESH_CLIENT_PROV_MODE == serv_pref) ?
                                       MESH_CLIENT_PROV_MODE :
                                       MESH_CLIENT_PROXY_MODE;

    ret = R_BLE_GATTC_WriteChar(conn_hdl, &req);

    MESH_CLIENT_LOG
    ("Writing %d to CCCD 0x%04X for Mode 0x%02X",
    flag, req.attr_hdl, serv_pref);

    return (0 == ret) ? API_SUCCESS : API_FAILURE;
}

static void mesh_client_prov_disc_cb
            (
                uint16_t conn_hdl,
                uint8_t  serv_idx,
                uint16_t type,
                void     * p_param
            )
{
    uint16_t idx;

    idx = mesh_client_find_env(conn_hdl);
    if (MESH_MAX_CLIENT_ENV == idx)
    {
        /* not found */
        return;
    }

    switch (type)
    {
        case BLE_DISC_PRIM_SERV_FOUND:
        {
            st_disc_serv_param_t *serv_param = (st_disc_serv_param_t *)p_param;
            if (UUID_MESH_PROVISIONING_SERVICE == serv_param->value.serv_16.uuid_16)
            {
                MESH_CLIENT_LOG(
                "\nMESH Provisioning Services Found!");

                MESH_CLIENT_LOG(
                "Service Handles are 0x%04X :: 0x%04X",
                serv_param->value.serv_16.range.start_hdl,
                serv_param->value.serv_16.range.end_hdl);

                mesh_client_env[idx].prov_start_hdl   = serv_param->value.serv_16.range.start_hdl;
                mesh_client_env[idx].prov_end_hdl     = serv_param->value.serv_16.range.end_hdl;
                MESH_CLIENT_LOG(
                "Prov Service Start Handle is 0x%04X",
                mesh_client_env[idx].prov_start_hdl);
                MESH_CLIENT_LOG(
                "Prov Service End Handle is 0x%04X\n",
                mesh_client_env[idx].prov_end_hdl);
            }
        } break;

        case BLE_DISC_CHAR_FOUND:
        {
            st_disc_char_param_t *char_param = (st_disc_char_param_t *)p_param;

            if (char_param->uuid_type == BLE_GATT_16_BIT_UUID_FORMAT)
            {
                if (UUID_MESH_PROVISIONING_DATA_OUT == char_param->value.char_16.uuid_16)
                {
                    mesh_client_env[idx].prov_data_out_hdl = char_param->value.char_16.value_hdl;
                    mesh_client_env[idx].prov_data_out_cccd_hdl = char_param->descs[0].value.desc_16.desc_hdl;
                }
                else if (UUID_MESH_PROVISIONING_DATA_IN == char_param->value.char_16.uuid_16)
                {
                    mesh_client_env[idx].prov_data_in_hdl = char_param->value.char_16.value_hdl;
                }
                else
                {
                    /* Do nothing. */
                }
            }
        } break;

        default:
        {
            /* Do nothing. */
        }
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
    uint16_t idx;

    idx = mesh_client_find_env(conn_hdl);
    if (MESH_MAX_CLIENT_ENV == idx)
    {
        /* not found */
        return;
    }

    switch (type)
    {
        case BLE_DISC_PRIM_SERV_FOUND:
        {
            st_disc_serv_param_t *serv_param = (st_disc_serv_param_t *)p_param;
            if (UUID_MESH_PROXY_SERVICE == serv_param->value.serv_16.uuid_16)
            {
                MESH_CLIENT_LOG(
                "\nMESH Proxy Services Found!");

                MESH_CLIENT_LOG(
                "Service Handles are 0x%04X :: 0x%04X",
                serv_param->value.serv_16.range.start_hdl,
                serv_param->value.serv_16.range.end_hdl);

                mesh_client_env[idx].proxy_start_hdl   = serv_param->value.serv_16.range.start_hdl;
                mesh_client_env[idx].proxy_end_hdl     = serv_param->value.serv_16.range.end_hdl;
                MESH_CLIENT_LOG(
                "Proxy Service Start Handle is 0x%04X",
                mesh_client_env[idx].proxy_start_hdl);
                MESH_CLIENT_LOG(
                "Proxy Service End Handle is 0x%04X\n",
                mesh_client_env[idx].proxy_end_hdl);
            }
        } break;

        case BLE_DISC_CHAR_FOUND:
        {
            st_disc_char_param_t *char_param = (st_disc_char_param_t *)p_param;

            if (char_param->uuid_type == BLE_GATT_16_BIT_UUID_FORMAT)
            {
                if (UUID_MESH_PROXY_DATA_OUT == char_param->value.char_16.uuid_16)
                {
                    mesh_client_env[idx].proxy_data_out_hdl = char_param->value.char_16.value_hdl;
                    mesh_client_env[idx].proxy_data_out_cccd_hdl = char_param->descs[0].value.desc_16.desc_hdl;
                }
                else if (UUID_MESH_PROXY_DATA_IN == char_param->value.char_16.uuid_16)
                {
                    mesh_client_env[idx].proxy_data_in_hdl = char_param->value.char_16.value_hdl;
                }
                else
                {
                    /* Do nothing. */
                }
            }
        } break;

        default:
        {
            /* Do nothing. */
        }
    }
}

static void mesh_clients_gattc_cb
            (
                uint16_t                type,
                ble_status_t            result,
                st_ble_gattc_evt_data_t * data
            )
{
    uint16_t idx;

    idx = mesh_client_find_env
            (
                (type == BLE_GATTC_EVENT_CONN_IND) ?
                    BLE_GAP_INVALID_CONN_HDL : data->conn_hdl
            );
    if (MESH_MAX_CLIENT_ENV == idx)
    {
        /* not found */
        return;
    }

    switch (type)
    {
        case BLE_GATTC_EVENT_CONN_IND:
        {
            mesh_client_env[idx].conn_hdl = data->conn_hdl;
        } break;

        case BLE_GATTC_EVENT_DISCONN_IND:
        {
            mesh_client_env[idx].conn_hdl = BLE_GAP_INVALID_CONN_HDL;
        } break;

        case BLE_GATTC_EVENT_CHAR_WRITE_RSP:
        {
            /** TODO: Check for CCCD Handle in the Write Response */
            if (MESH_CLIENT_PROV_MODE == mesh_client_env[idx].curr_notif_mode)
            {
                /* Call the Prov CCCD ntf complete Callback */
                if (NULL != prov_client_cb)
                {
                    prov_client_cb->mesh_prov_ntf_status
                    (
                        mesh_client_env[idx].conn_hdl,
                        mesh_client_env[idx].curr_notif_state,
                        0x00
                    );
                }
            }
            else if (MESH_CLIENT_PROXY_MODE == mesh_client_env[idx].curr_notif_mode)
            {
                /* Call the Proxy CCCD ntf complete Callback */
                if (NULL != proxy_client_cb)
                {
                    proxy_client_cb->mesh_proxy_ntf_status
                    (
                        mesh_client_env[idx].conn_hdl,
                        mesh_client_env[idx].curr_notif_state,
                        0x00
                    );
                }
            }
            else
            {
                /* DO Nothing ! */
            }
        } break;

        case BLE_GATTC_EVENT_HDL_VAL_NTF:
        {
            st_ble_gattc_ntf_evt_t *ntf_evt_param =
                (st_ble_gattc_ntf_evt_t *)data->p_param;

            uint16_t notifHandle = ntf_evt_param->data.attr_hdl;

            if (ntf_evt_param->data.value.value_len > 0)
            {
                /* Check the Attribute Handle:
                 * - If Provisioning Data Out Handle: then call the Prov Callback
                 * - If Proxy Data Out Handle: the ncall the Proxy Callback
                 */
                if (mesh_client_env[idx].prov_data_out_hdl == notifHandle)
                {
                    if (NULL != prov_client_cb)
                    {
                        prov_client_cb->mesh_prov_data_out_notif
                        (
                            mesh_client_env[idx].conn_hdl,
                            ntf_evt_param->data.value.value_len,
                            ntf_evt_param->data.value.p_value
                        );
                    }
                }
                else if (mesh_client_env[idx].proxy_data_out_hdl == notifHandle)
                {
                    if (NULL != proxy_client_cb)
                    {
                        proxy_client_cb->mesh_proxy_data_out_notif
                        (
                            mesh_client_env[idx].conn_hdl,
                            ntf_evt_param->data.value.value_len,
                            ntf_evt_param->data.value.p_value
                        );
                    }
                }
            }
        } break;

        default:
        {
            /* Do nothing. */
        }
    }
}

static void mesh_client_disc_comp_cb_pl (uint16_t conn_hdl)
{
    uint16_t idx;

    idx = mesh_client_find_env(conn_hdl);
    if (MESH_MAX_CLIENT_ENV == idx)
    {
        /* not found */
        return;
    }

    if (MESH_CLIENT_PROV_MODE == mesh_client_env[idx].curr_notif_mode)
    {
        /* Call the Prov CCCD ntf complete Callback */
        if (NULL != prov_client_cb)
        {
            prov_client_cb->mesh_prov_disc_comp
            (
                mesh_client_env[idx].conn_hdl
            );
        }
    }
    else if (MESH_CLIENT_PROXY_MODE == mesh_client_env[idx].curr_notif_mode)
    {
        /* Call the Proxy CCCD ntf complete Callback */
        if (NULL != proxy_client_cb)
        {
            proxy_client_cb->mesh_proxy_disc_comp
            (
                mesh_client_env[idx].conn_hdl
            );
        }
    }
    else
    {
        /* Do Nothing */
    }
}
