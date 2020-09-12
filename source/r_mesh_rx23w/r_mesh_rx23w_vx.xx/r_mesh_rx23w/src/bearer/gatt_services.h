
/**
 *  \file gatt_services.h
 *
 *  Generic Module to handle both
 *  - Mesh Provisioning Service :: 0x1827
 *  - Mesh Proxy Service        :: 0x1828
 */

/*
 *  Copyright (C) 2018. Mindtree Limited.
 *  All rights reserved.
 */
#ifndef _GATT_MESH_SERVICES_H
#define _GATT_MESH_SERVICES_H

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_mesh_rx23w_if.h"
#include "r_ble_rx23w_if.h"
#include "gatt_db_prov.h"
#include "gatt_db_proxy.h"

/* Compilation Switch to have this module print trace on console */
/* #define MESH_SERVER_CONSOLE_DEBUG */

/*******************************************************************************
* Macro definitions
*******************************************************************************/
#ifdef MESH_SERVER_CONSOLE_DEBUG
#define MESH_SERVER_LOG(...)                    printf(__VA_ARGS__)
#else /* MESH_SERVER_CONSOLE_DEBUG */
#define MESH_SERVER_LOG(...)
#endif /* MESH_SERVER_CONSOLE_DEBUG */

/*******************************************************************************
* Type definitions
*******************************************************************************/
/* Define the available custom service states */
enum mesh_serv_state
{
    MESH_NO_SERVICES,
    MESH_PROV_SERVICE_DONE,
    MESH_PROXY_SERVICE_DONE,
    MESH_SERV_STATE_MAX
};

typedef uint16_t (* mesh_prov_data_in_wt_cb)
                 (
                     uint16_t conn_hndl,
                     uint16_t offset,
                     uint16_t length,
                     uint8_t *value
                 );

typedef uint16_t (* mesh_prov_data_out_ccd_cb)
                 (
                     uint16_t conn_hndl,
                     uint8_t enabled
                 );

/**
 * Mesh Prov application callbacks
 */
typedef struct
{
    /** Provisioning Data IN Callback */
    mesh_prov_data_in_wt_cb              prov_data_in_cb;

    /** Provisioning Data OUT notif Changed */
    mesh_prov_data_out_ccd_cb            prov_data_out_ccd_cb;

} mesh_prov_cb;

typedef uint16_t (* mesh_proxy_data_in_wt_cb)
                 (
                     uint16_t conn_hndl,
                     uint16_t offset,
                     uint16_t length,
                     uint8_t *value
                 );

typedef uint16_t (* mesh_proxy_data_out_ccd_cb)
                 (
                     uint16_t conn_hndl,
                     uint8_t enabled
                 );

/**
 * Mesh Proxy application callbacks
 */
typedef struct
{
    /** Proxy Data IN Callback */
    mesh_proxy_data_in_wt_cb              proxy_data_in_cb;

    /** Proxy Data OUT notif Changed */
    mesh_proxy_data_out_ccd_cb            proxy_data_out_ccd_cb;

} mesh_proxy_cb;

/*******************************************************************************
* Prototype declarations
*******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Register Mesh Service [one time Call]
 *
 * \param [in] priority     priority associated with internal callbacks
 *                          to the BLE Stack.
 */
ble_status_t mesh_serv_init(uint8_t priority);

/**
 * Get Current MTU from the Mesh Services Layer
 *
 * \return mtu         Current MTU being used by Mesh Services.
 */
uint16_t mesh_serv_get_mtu(void);

/**
 * Set Current MTU from the Mesh Services Layer
 *
 * \param [out] mtu         Current MTU to be used by Mesh Services
 */
ble_status_t mesh_serv_set_mtu(uint16_t mtu);

/**
 * Register Mesh Provisioning Service Instance
 *
 * \param [in] cb        application callbacks
 */
ble_status_t mesh_serv_prov_init (mesh_prov_cb *cb);

/**
 * Unregister Mesh Provisioning Service Instance
 *
 */
ble_status_t mesh_serv_prov_deinit(void);

/**
 * Mesh Provisioning Data out Notifications
 *
 * Notification will only be sent if given client enabled notifications before.
 *
 * \param [in] conn_hndl  Connection Identifier
 * \param [in] attidx  Attribute Index
 * \param [in] val     Pointer to Data to be sent
 * \param [in] val_len Length of Data to be sent
 */
ble_status_t mesh_prov_notify_data_out
          (
              uint16_t  conn_hndl,
              uint8_t   attidx,
              uint8_t   * val,
              uint8_t   val_len
          );

/**
 * Register Mesh Proxy Service Instance
 *
 * \param [in] cb        application callbacks
 */
ble_status_t mesh_serv_proxy_init (mesh_proxy_cb *cb);

/**
 * Unregister Mesh Proxy Service Instance
 *
 */
ble_status_t mesh_serv_proxy_deinit(void);

/**
 * Mesh Proxy Data out Notifications
 *
 * Notification will only be sent if given client enabled notifications before.
 *
 * \param [in] conn_hndl  Connection Identifier
 * \param [in] attidx  Attribute Index
 * \param [in] val     Pointer to Data to be sent
 * \param [in] val_len Length of Data to be sent
 */
ble_status_t mesh_proxy_notify_data_out
          (
              uint16_t  conn_hndl,
              uint8_t   attidx,
              uint8_t   * val,
              uint8_t   val_len
          );

static void mesh_serv_set_cccd
            (
                uint16_t conn_hdl,
                uint16_t attr_hdl,
                uint16_t cli_cnfg
            );

static void mesh_serv_get_cccd
            (
                uint16_t conn_hdl,
                uint16_t attr_hdl,
                uint16_t * cli_cnfg
            );

#ifdef __cplusplus
}
#endif

#endif /* _GATT_MESH_SERVICES_H */
