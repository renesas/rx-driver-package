
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

/* Compilation Switch to have this module print trace on console */
/* #define MESH_CLIENT_CONSOLE_DEBUG */

/*******************************************************************************
* Macro definitions
*******************************************************************************/
#ifdef MESH_CLIENT_CONSOLE_DEBUG
#define MESH_CLIENT_LOG(...)                    printf(__VA_ARGS__)
#else /* MESH_CLIENT_CONSOLE_DEBUG */
#define MESH_CLIENT_LOG(...)
#endif /* MESH_CLIENT_CONSOLE_DEBUG */

/* Mesh GATT Bearer Related Service Assigned Numbers */
/* Use the UUID from Mesh Asigned numbers headers */
#define UUID_MESH_PROVISIONING_SERVICE  (0x1827)
#define UUID_MESH_PROVISIONING_DATA_IN  (0x2ADB)
#define UUID_MESH_PROVISIONING_DATA_OUT (0x2ADC)

/* Use the UUID from Mesh Asigned numbers headers */
#define UUID_MESH_PROXY_SERVICE         (0x1828)
#define UUID_MESH_PROXY_DATA_IN         (0x2ADD)
#define UUID_MESH_PROXY_DATA_OUT        (0x2ADE)

/** GATT Modes */
#define MESH_CLIENT_PROV_MODE           0x00
#define MESH_CLIENT_PROXY_MODE          0x01

/**
 * TODO:
 * Need to map this to more than 1 incase multiple peers are to be
 * supported.
 */
#define MESH_MAX_CLIENT_ENV             1

/*******************************************************************************
* Type definitions
*******************************************************************************/
/**
 * \brief Mesh Provisioning Data Out notification callback
 *
 * Called when Client received Mesh Provisioning data out
 * notification from server.
 *
 * \param [in] client   client instance
 * \param [in] length   length of data
 * \param [in] value    mesh prov data out value from Server
 *
 */
typedef void (* mesh_prov_client_data_out_cb)
             (
                 uint16_t  conidx,
                 uint16_t  length,
                 uint8_t   * value
             );
/**
 * \brief Set Notifications Enable Disable Callback
 *
 * \param [in] conidx           connection index
 * \param [in] flag             Enable/Disable State Flag
 * \param [in] status           operation status
 *
 */
typedef void (* mesh_prov_client_data_out_ntf_status_cb)
             (
                 uint16_t  conidx,
                 uint8_t   flag,
                 uint8_t   status
             );

/**
 * \brief Mesh Provisioning Discovery Complete callback
 *
 * Called when Client has completed the Discovery Process
 *
 * \param [in] client   client instance
 *
 */
typedef void (* mesh_prov_client_disc_comp_cb)
             (
                 uint16_t  conidx
             );

/**
 * \brief MEsh Provisioning Cliet application callbacks
 *
 */
typedef struct
{
    /** Mesh Prov Client Data Out callback */
    mesh_prov_client_data_out_cb             mesh_prov_data_out_notif;

    /**
     * Called once client enabled/disabled event
     * characteristic notifications/indications
     */
    mesh_prov_client_data_out_ntf_status_cb  mesh_prov_ntf_status;

    /** Mesh Prov Client Discover Complete callback */
    mesh_prov_client_disc_comp_cb            mesh_prov_disc_comp;

} mesh_client_prov_cb;

/**
 * \brief Mesh Provisioning Data Out notification callback
 *
 * Called when Client received Mesh Provisioning data out
 * notification from server.
 *
 * \param [in] client   client instance
 * \param [in] length   length of data
 * \param [in] value    mesh prov data out value from Server
 *
 */
typedef void (* mesh_proxy_client_data_out_cb)
             (
                 uint16_t  conidx,
                 uint16_t  length,
                 uint8_t   * value
             );
/**
 * \brief Set Notifications Enable Disable Callback
 *
 * \param [in] conidx           connection index
 * \param [in] flag             Enable/Disable State Flag
 * \param [in] status           operation status
 *
 */
typedef void (* mesh_proxy_client_data_out_ntf_status_cb)
             (
                 uint16_t  conidx,
                 uint8_t   flag,
                 uint8_t   status
             );

/**
 * \brief Mesh Proxy Discovery Complete callback
 *
 * Called when Client has completed the Discovery Process
 *
 * \param [in] client   client instance
 *
 */
typedef void (* mesh_proxy_client_disc_comp_cb)
             (
                 uint16_t  conidx
             );

/**
 * \brief Mesh Provisioning Cliet application callbacks
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

struct mesh_client_env_tag
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

    /* Current Notification Mode and State */
    uint16_t curr_notif_state;
    uint8_t  curr_notif_mode;
};

#define mesh_prov_client_discover_serv(conhdl) \
        mesh_client_discover_services((conhdl), (BLEBRR_GATT_PROV_MODE));

#define mesh_proxy_client_discover_serv(conhdl) \
        mesh_client_discover_services((conhdl), (BLEBRR_GATT_PROV_MODE));

#define mesh_prov_client_data_in_write(conhdl, val, len) \
        mesh_client_send_wwr((conhdl), (val), (len), BLEBRR_GATT_PROV_MODE);

#define mesh_proxy_client_data_in_write(conhdl, val, len) \
        mesh_client_send_wwr((conhdl), (val), (len), BLEBRR_GATT_PROXY_MODE);

#define mesh_prov_client_enable_data_out(conhdl) \
        mesh_client_config_ntf((conhdl), BLEBRR_GATT_PROV_MODE, (MS_TRUE));

#define mesh_proxy_client_enable_data_out(conhdl) \
        mesh_client_config_ntf((conhdl), BLEBRR_GATT_PROXY_MODE, (MS_TRUE));

#define mesh_prov_client_disable_data_out(conhdl) \
        mesh_client_config_ntf((conhdl), BLEBRR_GATT_PROV_MODE, (MS_FALSE));

#define mesh_proxy_client_disable_data_out(conhdl) \
        mesh_client_config_ntf((conhdl), BLEBRR_GATT_PROXY_MODE, (MS_FALSE));

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
         uint8_t   serv_pref
     );

API_RESULT mesh_client_config_ntf
           (
               uint16_t  conn_hdl,
               uint8_t   serv_pref,
               uint8_t   flag
           );

void mesh_client_init(void);
API_RESULT mesh_client_discover_services(uint16_t conn_hdl, uint8_t serv_mode);
#ifdef __cplusplus
}
#endif
#endif /* _GATT_CLIENTS_H */
