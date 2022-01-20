
/**
 *  \file gatt_services.h
 */

/*
 *  Copyright (C) 2018-2021. Mindtree Limited.
 *  All rights reserved.
 */
#ifndef _GATT_MESH_SERVICES_H
#define _GATT_MESH_SERVICES_H

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_mesh_rx23w_if.h"
#include "r_ble_rx23w_if.h"
#include "gatt_db.h"
#include "blebrr.h"

/* Compilation Switch to have this module print trace on console */
/* #define MESH_SERVER_CONSOLE_DEBUG */

/*******************************************************************************
* Macro definitions
*******************************************************************************/
#ifdef MESH_SERVER_CONSOLE_DEBUG
#define MESH_SERVER_LOG(...)                    R_BLE_CLI_Printf(__VA_ARGS__)
#else /* MESH_SERVER_CONSOLE_DEBUG */
#define MESH_SERVER_LOG(...)
#endif /* MESH_SERVER_CONSOLE_DEBUG */

/*******************************************************************************
* Type definitions
*******************************************************************************/
typedef uint16_t (* mesh_gatt_data_in_wt_cb)
                 (
                     uint16_t conn_hndl,
                     uint16_t length,
                     uint8_t *value
                 );

typedef uint16_t (* mesh_gatt_data_out_ccd_cb)
                 (
                     uint16_t conn_hndl,
                     uint8_t enabled
                 );

/**
 * Mesh GATT Service application callbacks
 */
typedef struct
{
    /** Mesh GATT Service Data IN Callback */
    mesh_gatt_data_in_wt_cb              data_in_cb;

    /** Mesh GATT Service Data OUT Notification Changed */
    mesh_gatt_data_out_ccd_cb            data_out_ccd_cb;

} mesh_gatt_cb;

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
 *                          to the BLE Protocol Stack.
 */
ble_status_t mesh_serv_init(uint8_t priority);

/**
 * Get Current MTU from the Mesh Services Layer
 *
 * \param [in] mtu         Connection Handle
 *
 * \return mtu         Current MTU being used by Mesh Services.
 */
uint16_t mesh_serv_get_mtu(uint16_t conn_hdl);

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
ble_status_t mesh_serv_prov_init (mesh_gatt_cb *cb);

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
 * \param [in] attr_hdl  Attribute Handle
 * \param [in] val     Pointer to Data to be sent
 * \param [in] val_len Length of Data to be sent
 */
ble_status_t mesh_prov_notify_data_out
          (
              uint16_t  conn_hndl,
              uint16_t  attr_hdl,
              uint8_t   * val,
              uint8_t   val_len
          );

/**
 * Register Mesh Proxy Service Instance
 *
 * \param [in] cb        application callbacks
 */
ble_status_t mesh_serv_proxy_init (mesh_gatt_cb *cb);

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
 * \param [in] attr_hdl  Attribute Handle
 * \param [in] val     Pointer to Data to be sent
 * \param [in] val_len Length of Data to be sent
 */
ble_status_t mesh_proxy_notify_data_out
          (
              uint16_t  conn_hndl,
              uint16_t  attr_hdl,
              uint8_t   * val,
              uint8_t   val_len
          );

/**
 * GATT Service Changed Indication
 *
 * \param [in] conn_hndl  Connection Identifier
 */
ble_status_t mesh_indicate_serv_changed(uint16_t  conn_hdl);

#ifdef __cplusplus
}
#endif

#endif /* _GATT_MESH_SERVICES_H */
