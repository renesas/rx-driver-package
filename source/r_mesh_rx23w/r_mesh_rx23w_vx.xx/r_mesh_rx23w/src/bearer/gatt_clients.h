
/**
 *  \file gatt_clients.h
 *
 *
 */

/*
 *  Copyright (C) 2018. Mindtree Limited.
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

#define MESH_BLE_UUID_CCCD              (0x2902)

/*******************************************************************************
* Type definitions
*******************************************************************************/
/**
 * \brief Mesh Provisioning Data Out notification callback
 *
 * Called when Client received Mesh Provisioning data out
 * notification from server.
 *
 * \param [in] conn_hdl connection handle
 * \param [in] length   length of data
 * \param [in] value    mesh prov data out value from Server
 *
 */
typedef void (* mesh_prov_client_data_out_cb)
             (
                 uint16_t  conn_hdl,
                 uint16_t  length,
                 uint8_t   * value
             );
/**
 * \brief Set Notifications Enable Disable Callback
 *
 * \param [in] conn_hdl connection handle
 * \param [in] flag     Enable/Disable State Flag
 * \param [in] status   operation status
 *
 */
typedef void (* mesh_prov_client_data_out_ntf_status_cb)
             (
                 uint16_t  conn_hdl,
                 uint8_t   flag,
                 uint8_t   status
             );

/**
 * \brief Mesh Provisioning Discovery Complete callback
 *
 * Called when Client has completed the Discovery Process
 *
 * \param [in] conn_hdl connection handle
 * \param [in] status   discovery status
 *
 */
typedef void (* mesh_prov_client_disc_comp_cb)
             (
                 uint16_t  conn_hdl,
                 API_RESULT status
             );

/**
 * \brief Mesh Provisioning Client application callbacks
 *
 */
typedef struct
{
    /** Mesh Provisioning Client Data Out callback */
    mesh_prov_client_data_out_cb             mesh_prov_data_out_notif;

    /**
     * Called once client enabled/disabled event
     * characteristic notifications/indications
     */
    mesh_prov_client_data_out_ntf_status_cb  mesh_prov_ntf_status;

    /** Mesh Provisioning Client Discover Complete callback */
    mesh_prov_client_disc_comp_cb            mesh_prov_disc_comp;

} mesh_client_prov_cb;

/**
 * \brief Mesh Proxy Data Out notification callback
 *
 * Called when Client received Mesh Provisioning data out
 * notification from server.
 *
 * \param [in] conn_hdl connection handle
 * \param [in] length   length of data
 * \param [in] value    mesh prov data out value from Server
 *
 */
typedef void (* mesh_proxy_client_data_out_cb)
             (
                 uint16_t  conn_hdl,
                 uint16_t  length,
                 uint8_t   * value
             );
/**
 * \brief Set Notifications Enable Disable Callback
 *
 * \param [in] conn_hdl connection index
 * \param [in] flag     Enable/Disable State Flag
 * \param [in] status   operation status
 *
 */
typedef void (* mesh_proxy_client_data_out_ntf_status_cb)
             (
                 uint16_t  conn_hdl,
                 uint8_t   flag,
                 uint8_t   status
             );

/**
 * \brief Mesh Proxy Discovery Complete callback
 *
 * Called when Client has completed the Discovery Process
 *
 * \param [in] conn_hdl connection handle
 * \param [in] status   discovery status
 *
 */
typedef void (* mesh_proxy_client_disc_comp_cb)
             (
                 uint16_t  conn_hdl,
                 API_RESULT status
             );

/**
 * \brief Mesh Provisioning Client application callbacks
 *
 */
typedef struct
{
    /** Mesh Proxy Client Data Out callback */
    mesh_proxy_client_data_out_cb             mesh_proxy_data_out_notif;

    /**
     * Called once client enabled/disabled event
     * characteristic notifications/indications
     */
    mesh_proxy_client_data_out_ntf_status_cb  mesh_proxy_ntf_status;

    /** Mesh Proxy Client Discover Complete callback */
    mesh_proxy_client_disc_comp_cb            mesh_proxy_disc_comp;

} mesh_client_proxy_cb;

typedef struct
{
    /* Connection handle */
    uint16_t conn_hdl;

    /* Provisioning Service Related Handles */
    uint16_t prov_start_hdl;
    uint16_t prov_end_hdl;
    uint16_t prov_data_in_hdl;
    uint16_t prov_data_out_hdl;
    uint16_t prov_data_out_cccd_hdl;

    /* Proxy Service Related Handles */
    uint16_t proxy_start_hdl;
    uint16_t proxy_end_hdl;
    uint16_t proxy_data_in_hdl;
    uint16_t proxy_data_out_hdl;
    uint16_t proxy_data_out_cccd_hdl;

    /* Current Notification State */
    uint16_t curr_notif_state;

    /* Current Discovery Mode */
    uint8_t  curr_disc_mode;
} mesh_client_env_t;

#define mesh_prov_client_discover_serv(conhdl) \
        mesh_client_discover_services((conhdl), (BLEBRR_GATT_PROV_MODE));

#define mesh_proxy_client_discover_serv(conhdl) \
        mesh_client_discover_services((conhdl), (BLEBRR_GATT_PROV_MODE));

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
         mesh_client_prov_cb *cb
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
         mesh_client_proxy_cb *cb
     );

API_RESULT mesh_client_send_wwr
     (
         uint16_t  conn_hdl,
         uint8_t   * value,
         uint16_t  length,
         uint8_t   mode
     );

API_RESULT mesh_client_config_ntf
           (
               uint16_t  conn_hdl,
               uint8_t   flag,
               uint8_t   mode
           );

void mesh_client_init(void);
API_RESULT mesh_client_discover_services(uint16_t conn_hdl, uint8_t serv_mode);
#ifdef __cplusplus
}
#endif
#endif /* _GATT_CLIENTS_H */
