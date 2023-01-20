
/**
 *  \file gatt_clients.h
 */

/*
 *  Copyright (C) 2018-2022. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _GATT_CLIENTS_H
#define _GATT_CLIENTS_H

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_mesh_rx23w_if.h"
#include "r_ble_rx23w_if.h"
#include "discovery/r_ble_disc.h"
#include "blebrr.h"
#include "cli/r_ble_cli.h"

/*******************************************************************************
* Compilation Switches
*******************************************************************************/
/* Compilation Switch to have this module print trace on console */
/* #define MESH_CLIENT_CONSOLE_DEBUG */

/*******************************************************************************
* Macro definitions
*******************************************************************************/
#ifdef MESH_CLIENT_CONSOLE_DEBUG
#define MESH_CLIENT_LOG(...)                    R_BLE_CLI_Printf(__VA_ARGS__)
#else /* MESH_CLIENT_CONSOLE_DEBUG */
#define MESH_CLIENT_LOG(...)
#endif /* MESH_CLIENT_CONSOLE_DEBUG */

#define MESH_MAX_CLIENT_ENV             (1)

#define MESH_BLE_UUID_SERV_CHANGED      (0x2A05)
#define MESH_BLE_UUID_CCCD              (0x2902)

/*******************************************************************************
* Type definitions
*******************************************************************************/
/**
 * \brief Mesh GATT Service Data Out notification callback
 *
 * Called when Client received Mesh GATT Service data out
 * notification from server.
 *
 * \param [in] conn_hdl connection handle
 * \param [in] length   length of data
 * \param [in] value    mesh gatt data out value from Server
 *
 */
typedef void (* mesh_gatt_client_data_out_cb)
             (
                 uint16_t  conn_hdl,
                 uint16_t  length,
                 uint8_t   * value
             );

/**
 * \brief Mesh GATT Service Set Notifications Enable Disable Callback
 *
 * \param [in] conn_hdl connection handle
 * \param [in] flag     Enable/Disable State Flag
 * \param [in] status   operation status
 *
 */
typedef void (* mesh_gatt_client_data_out_ntf_status_cb)
             (
                 uint16_t  conn_hdl,
                 uint8_t   flag,
                 uint8_t   status
             );

/**
 * \brief Mesh GATT Service Discovery Complete callback
 *
 * Called when Client has completed the Discovery Process
 *
 * \param [in] conn_hdl connection handle
 * \param [in] status   discovery status
 *
 */
typedef void (* mesh_gatt_client_disc_comp_cb)
             (
                 uint16_t  conn_hdl,
                 API_RESULT status
             );

/**
 * \brief Mesh GATT Service Changed callback
 *
 * Called when Client received Service Changed Indication
 *
 * \param [in] conn_hdl connection handle
 * \param [in] status   discovery status
 *
 */
typedef void (* mesh_gatt_client_serv_changed_cb)
             (
                 uint16_t  conn_hdl
             );

/**
 * \brief GATT MTU Exchange callback
 *
 * Called when Client received Exchange MTU Response
 *
 * \param [in] conn_hdl connection handle
 *
 */
typedef void (* mesh_gatt_client_exchange_mtu_cb)
             (
                 uint16_t  conn_hdl
             );

/**
 * \brief Mesh GATT Service Client application callbacks
 *
 */
typedef struct
{
    /** Mesh GATT Service Client Data Out callback */
    mesh_gatt_client_data_out_cb             data_out_notif;

    /**
     * Called once client enabled/disabled event
     * characteristic notifications/indications
     */
    mesh_gatt_client_data_out_ntf_status_cb  ntf_status;

    /** Mesh GATT Service Client Discover Complete callback */
    mesh_gatt_client_disc_comp_cb            disc_comp;

    /** Mesh GATT Service Changed callback */
    mesh_gatt_client_serv_changed_cb         serv_changed;

} mesh_gatt_client_cb;

typedef struct
{
    /* Connection handle */
    uint16_t conn_hdl;

    /* GATT Service Changed Attribute Handles */
    uint16_t serv_changed_hdl;
    uint16_t serv_changed_cccd_hdl;

    /* No need to store Attribute Handles of both Provisioning Service and Proxy Service at a time. */
    /**
     *  7 "Mesh GATT services" in Mesh Profile:
     *  If both are supported, only one of these services shall be exposed in the GATT database at a time.
     */

    /* Mesh Service Attribute Handles */
    uint16_t start_hdl;
    uint16_t end_hdl;
    uint16_t data_in_hdl;
    uint16_t data_out_hdl;
    uint16_t data_out_cccd_hdl;

    /* Current Notification State */
    uint16_t curr_notif_state;

    /* Current Discovery Mode */
    uint8_t  curr_disc_mode;
} mesh_client_env_t;

#define mesh_prov_client_discover_serv(conhdl) \
        mesh_client_discover_services((conhdl), (BLEBRR_GATT_PROV_MODE));

#define mesh_proxy_client_discover_serv(conhdl) \
        mesh_client_discover_services((conhdl), (BLEBRR_GATT_PROXY_MODE));

#define mesh_prov_client_data_in_write(conhdl, val, len) \
        mesh_client_send_wwr((conhdl), (val), (len), BLEBRR_GATT_PROV_MODE);

#define mesh_proxy_client_data_in_write(conhdl, val, len) \
        mesh_client_send_wwr((conhdl), (val), (len), BLEBRR_GATT_PROXY_MODE);

#define mesh_prov_client_enable_data_out(conhdl) \
        mesh_client_config_ntf((conhdl), (MS_TRUE), BLEBRR_GATT_PROV_MODE);

#define mesh_proxy_client_enable_data_out(conhdl) \
        mesh_client_config_ntf((conhdl), (MS_TRUE), BLEBRR_GATT_PROXY_MODE);

#define mesh_prov_client_disable_data_out(conhdl) \
        mesh_client_config_ntf((conhdl), (MS_FALSE), BLEBRR_GATT_PROV_MODE);

#define mesh_proxy_client_disable_data_out(conhdl) \
        mesh_client_config_ntf((conhdl), (MS_FALSE), BLEBRR_GATT_PROXY_MODE);

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
* Prototype declarations
*******************************************************************************/
/**
 * \brief Register Mesh Provisioning Client instance
 *
 * Function registers new Mesh Provisioning Client instance.
 *
 * \param [in] cb               client application callbacks
 *
 * \return None
 *
 */
void mesh_client_prov_init
     (
         mesh_gatt_client_cb *cb
     );

/**
 * \brief Register Mesh Proxy Client instance
 *
 * Function registers new Mesh Proxy Client instance.
 *
 * \param [in] cb               client application callbacks
 *
 * \return None
 *
 */
void mesh_client_proxy_init
     (
         mesh_gatt_client_cb *cb
     );

ble_status_t mesh_client_send_wwr
     (
         uint16_t  conn_hdl,
         uint8_t   * value,
         uint16_t  length,
         uint8_t   mode
     );

ble_status_t mesh_client_config_ntf
           (
               uint16_t  conn_hdl,
               uint8_t   flag,
               uint8_t   mode
           );

ble_status_t mesh_client_config_serv_changed
           (
               uint16_t  conn_hdl,
               uint8_t   flag
           );

void mesh_client_init(void);
ble_status_t mesh_client_discover_services(uint16_t conn_hdl, uint8_t serv_mode);
ble_status_t mesh_client_expand_mtu(uint16_t conn_hdl, mesh_gatt_client_exchange_mtu_cb cb);
#ifdef __cplusplus
}
#endif
#endif /* _GATT_CLIENTS_H */
