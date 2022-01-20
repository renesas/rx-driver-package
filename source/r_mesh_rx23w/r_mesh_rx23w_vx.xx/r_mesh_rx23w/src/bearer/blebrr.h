/**
 *  \file blebrr.h
 */

/*
 *  Copyright (C) 2013-2021. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_BLEBRR_
#define _H_BLEBRR_

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_mesh_rx23w_if.h"
#include "cli/r_ble_cli.h"

/**
 * \defgroup ble_bearer Bluetooth Bearer Wrapper
 * \brief Bluetooth Bearer Wrapper (BLEBRR) provides Mesh Core block with the wrapper functions to utilize underlying BLE Protocol Stack as a Bluetooth Bearer.
 * \{
 */

/*******************************************************************************
* Compilation Switches
*******************************************************************************/
/** \cond compilation switches */

/** Enable GATT Bearer Client Role */
/*  ROM/RAM used can be reduced by disabling GATT Client functionalities */
#define BLEBRR_GATT_CLIENT                      (1)

/** Specify Device Address Type
  * Either Public Address or Static Random Address can be set by the macro below.
  *  - BLE_GAP_ADDR_PUBLIC
  *  - BLE_GAP_ADDR_RAND
  * Device Address is obtained from BLE Protocol Stack via Vendor Specific API.
  */
#define BLEBRR_VS_ADDR_TYPE                     (BLE_GAP_ADDR_RAND)

/* Compilation Switch to have this module print trace on console */
/* #define BLEBRR_CONSOLE_DEBUG */

/** \endcond */

/*******************************************************************************
* Macro definitions
*******************************************************************************/
/**
 * \defgroup ble_bearer_constants Constants
 * \{
 */

/** \cond definitions for internal module */
#ifdef BLEBRR_CONSOLE_DEBUG
#define BLEBRR_LOG(...)                         R_BLE_CLI_Printf(__VA_ARGS__)
#else /* BLEBRR_CONSOLE_DEBUG */
#define BLEBRR_LOG(...)
#endif /* BLEBRR_CONSOLE_DEBUG */
/** \endcond */

/** \name GATT Interface Modes
 *  \{
 */
/** Mesh Provisoning Service Mode */
#define BLEBRR_GATT_PROV_MODE                   0x00
/** Mesh Proxy Service Mode */
#define BLEBRR_GATT_PROXY_MODE                  0x01
/** Uninitialized Mode */
#define BLEBRR_GATT_UNINIT_MODE                 0xFF
/** \} */

/** \cond definitions for internal module */
/** Bearer Client Server Roles */
#define BLEBRR_CLIENT_ROLE                      BRR_CLIENT_ROLE
#define BLEBRR_SERVER_ROLE                      BRR_SERVER_ROLE
#define BLEBRR_INVALID_ROLE                     BRR_INVALID_ROLE

/** Bearer GATT communication Channel setup events */
#define BLEBRR_COM_CHANNEL_CONNECT              0x00
#define BLEBRR_COM_CHANNEL_DISCONNECT           0x01
/** \endcond */

/** \name GATT Interface Events
 *  \{
 */
/** Connection is Established */
#define BLEBRR_GATT_IFACE_UP                    0x00
/** Connection is Terminated */
#define BLEBRR_GATT_IFACE_DOWN                  0x01
/** GATT Bearer communication is Enabled */
#define BLEBRR_GATT_IFACE_ENABLE                0x02
/** GATT Bearer communication is Disabled */
#define BLEBRR_GATT_IFACE_DISABLE               0x03
/** Mesh GATT Service is found as a result of Service Discovery */
#define BLEBRR_GATT_IFACE_FOUND                 0x04
/** Mesh GATT Service is not found as a result of Service Discovery */
#define BLEBRR_GATT_IFACE_NOT_FOUND             0x05
/** GATT Database of peer connected device is changed */
#define BLEBRR_GATT_IFACE_CHANGED               0x06
/** Report of Connectable Device having Mesh GATT Service */
#define BLEBRR_GATT_IFACE_SCAN                  0x07
/** Creating Connection is Canceled */
#define BLEBRR_GATT_IFACE_CANCEL                0x08
/** \} */

/** \} */

/*******************************************************************************
* Type definitions
*******************************************************************************/
/**
 * \defgroup ble_bearer_callback Application Callbacks
 * \{
 */

/** Callback to Inform Application Layer about Bearer Initialization Completion */
typedef void (* BLEBRR_INIT_CB)
                (
                    st_ble_dev_addr_t * own_addr
                );

/** Callback to Inform Application Layer about GATT Bearer Iface Events */
typedef void (* BLEBRR_GATT_IFACE_CB)
                (
                    UCHAR  gatt_event,
                    UCHAR  gatt_mode,
                    UINT16 conn_hdl,
                    st_ble_dev_addr_t * peer_addr
                );

/** \} */

/** \cond definitions for internal module */
typedef struct {
    BRR_HANDLE          brr_hdl;
    UINT16              conn_hdl;
    UCHAR               role;
    st_ble_dev_addr_t   bd_addr;
} blebrr_gatt_env_t;
/** \endcond */

/*******************************************************************************
* Prototype declarations
*******************************************************************************/
/** \cond definitions for internal module */
/* Functions implemented in blebrr_pl.c */
API_RESULT blebrr_gatt_send_pl(BRR_HANDLE * handle, UCHAR * data, UINT16 datalen);
API_RESULT blebrr_scan_pl(UCHAR enable);
API_RESULT blebrr_advertise_data_pl(CHAR type, UCHAR * pdata, UINT16 pdatalen);
API_RESULT blebrr_advertise_pl(UCHAR state);
API_RESULT blebrr_set_gattmode_pl(UCHAR serv_mode);
blebrr_gatt_env_t * blebrr_find_gatt_env_by_connhdl(uint16_t conn_hdl);

/* Functions implemented in blebrr.c */
void blebrr_adv_enable(void);
void blebrr_adv_disable(void);
void blebrr_pl_scan_setup(UCHAR enable);
void blebrr_pl_advertise_setup(UCHAR enable);
void blebrr_pl_recv_advpacket(UCHAR type, UCHAR * pdata, UINT16 pdatalen, UCHAR rssi);

/* Functions implemented in blebrr_gatt.c */
API_RESULT blebrr_pl_gatt_connection(BRR_HANDLE * handle, UCHAR role, UINT16 mtu, UCHAR mode);
API_RESULT blebrr_pl_gatt_disconnection(BRR_HANDLE * handle, UCHAR role, UCHAR mode);
API_RESULT blebrr_pl_recv_gattpacket(BRR_HANDLE * handle, UCHAR * pdata, UINT16 pdatalen);
/** \endcond */

/*******************************************************************************
* Macros for Backward Compatibility
*******************************************************************************/
/** \cond backward compatibility */
#define blebrr_init                             R_MS_BRR_Init
#define blebrr_register                         R_MS_BRR_Setup
#define blebrr_register_gatt_iface_event_pl     R_MS_BRR_Register_GattIfaceCallback
#define blebrr_gatt_mode_set                    R_MS_BRR_Set_GattMode
#define blebrr_gatt_mode_get                    R_MS_BRR_Get_GattMode
#define blebrr_disconnect_pl                    R_MS_BRR_Disconnect
#define blebrr_set_adv_scanrsp_data_pl          R_MS_BRR_Set_ScanRspData
#define blebrr_scan_gatt_pl                     R_MS_BRR_Scan_GattBearer
#define blebrr_create_conn_pl                   R_MS_BRR_Create_Connection
#define blebrr_discover_service_pl              R_MS_BRR_Discover_Service
#define blebrr_config_ntf_pl                    R_MS_BRR_Config_Notification
/** \endcond */

/*******************************************************************************
* API declarations
*******************************************************************************/
/**
 * \defgroup ble_bearer_api API Definitions
 * \{
 */

/**
 *  \brief Initialize underlying BLE Protocol Stack to use as a Mesh Bearer.
 *
 *  \par Description
 *  API to initialize underlying BLE Protocol Stack to use as a Mesh Bearer.
 *  Completion of the initialization is notified by the callback function.
 *
 *  \param [in] init_cb     callback function to notify completion of the initialization
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT R_MS_BRR_Init(BLEBRR_INIT_CB init_cb);

/**
 *  \brief Register ADV bearer with Mesh stack and Start Scan.
 *
 *  \par Description
 *  API to register ADV bearer with Mesh stack. After registering, this routine starts Scan.
 *
 *  \note GATT Bearer is registered to Mesh Stack when Mesh Service Notification
 *  is enabled after connection is established. Then, GATT Bearer registered is 
 *  removed from Mesh Stack when Notification is disabled or when connection is 
 *  terminated.
 */
void  R_MS_BRR_Setup(void);

/**
 *  \brief Register callback function to receive GATT Interface Events.
 *
 *  \par Description
 *  API to register callback function to receive GATT Interface Events defined by the BLEBRR_GATT_IFACE_* macros.
 *
 *  \param [in] cb          callback function to notify GATT interface events
 *
 *  \return API_SUCCESS or API_FAILURE
 */
API_RESULT R_MS_BRR_Register_GattIfaceCallback(BLEBRR_GATT_IFACE_CB cb);

/**
 *  \brief Set GATT Bearer Mode.
 *
 *  \param [in] mode        GATT interface mode, either BLEBRR_GATT_PROV_MODE or BLEBRR_GATT_PROXY_MODE
 */
void  R_MS_BRR_Set_GattMode(UCHAR mode);

/**
 *  \brief Get Current GATT Bearer Mode.
 *
 *  \return GATT Interface Mode, either BLEBRR_GATT_PROV_MODE or BLEBRR_GATT_PROXY_MODE
 */
UCHAR R_MS_BRR_Get_GattMode(void);

/**
 *  \brief Terminate Connection.
 *
 *  \par Description
 *  API to terminate connection. Completion of disconnection is notified by the BLEBRR_GATT_IFACE_DOWN event.
 *
 *  \param [in] conn_hdl    connection handle to identify device
 *
 *  \return API_SUCCESS or API_FAILURE
 */
API_RESULT R_MS_BRR_Disconnect(UINT16 conn_hdl);

/**
 *  \brief Set Scan Response Data in Connectable and scannable undirected Advertising event.
 *
 *  \par Description
 *  API to Set Scan Response Data in Connectable and scannable undirected Advertising event.
 *  Scan Response Data can be used for indicating additional information such as << Complete Local Name>>.
 *
 *  \param [in] srp_data        scan response data
 *  \param [in] srp_datalen     scan response data length
 *
 *  \note Scan Response Data specified by this API is used only when Connectable Advertising for Provisioning and Proxy.
 *
 *  \return API_SUCCESS or API_FAILURE
 */
API_RESULT R_MS_BRR_Set_ScanRspData(const UCHAR * srp_data, UCHAR srp_datalen);

/**
 *  \brief Manage Reporting Connectable Device having Mesh GATT Service.
 *
 *  \par Description
 *  API to manage Reporting Connectable Device having Mesh GATT Service.
 *  Reports including Device Address and its Connection Handle are notified by the BLEBRR_GATT_IFACE_SCAN event.
 *
 *  \param [in] enable      reporting flag; enabled if MS_TRUE, or disabled if MS_FALSE
 *  \param [in] mode        GATT interface mode, either BLEBRR_GATT_PROV_MODE or BLEBRR_GATT_PROXY_MODE
 *
 *  \note This API is for GATT Client only.
 *  \note Scan Operation state is managed by Mesh Stack API.
 *  \note Duplication Filter is not implemented.
 *
 *  \return API_SUCCESS or API_FAILURE
 */
API_RESULT R_MS_BRR_Scan_GattBearer(UCHAR enable, UCHAR mode);

/**
 *  \brief Request to Create Connection.
 *
 *  \par Description
 *  API to request to Create Connection.
 *  Completion of the establishing a connection is notified by BLEBRR_GATT_IFACE_UP.
 *  After establishing a connection, R_MS_BRR_Discover_Service() is invoked automatically.
 *
 *  \param [in] remote_addr remove device address to 
 *  \param [in] mode        GATT interface mode, either BLEBRR_GATT_PROV_MODE or BLEBRR_GATT_PROXY_MODE
 *
 *  \note This API is for GATT Client only.
 *  \note By invoking this API, BLE Link Layer transitions to Initiating state and notifies no Advertising Report
 *  until connection is established. Thus Mesh Stack cannot receive message from ADV bearer during Initiating state.
 *
 *  \return API_SUCCESS or API_FAILURE
 */
API_RESULT R_MS_BRR_Create_Connection(st_ble_dev_addr_t * remote_addr, UCHAR mode);

/**
 *  \brief Cancel Creating a Connection.
 *
 *  \par Description
 *  API to Cancel Createing a Connection while BLE Link Layer is Initiating state.
 *
 *  \note This API is for GATT Client only.
 *
 *  \return API_SUCCESS or API_FAILURE
 */
API_RESULT R_MS_BRR_Cancel_CreateConnection(void);

/**
 *  \brief Start Service Discovery for Mesh GATT Service.
 *
 *  \par Description
 *  API to start Service Discovery for Mesh GATT Service.
 *  If Mesh GATT Service specified by the "mode" argument, this routine enables Notification of the Mesh GATT Service
 *  by invoking R_MS_BRR_Config_Notification(). Completion of the Notification is notified by the BLEBRR_GATT_IFACE_ENABLE event.
 *  Also, if Mesh GATT Service is not found in the GATT Database, this routine notifies the BLEBRR_GATT_IFACE_NOT_FOUND event.
 *
 *  \param [in] conn_hdl    connection handle to identify device
 *  \param [in] mode        GATT interface mode, either BLEBRR_GATT_PROV_MODE or BLEBRR_GATT_PROXY_MODE
 *
 *  \note This API is for GATT Client only.
 *
 *  \return API_SUCCESS or API_FAILURE
 */
API_RESULT R_MS_BRR_Discover_Service(UINT16 conn_hdl, UCHAR mode);

/**
 *  \brief Configure GATT Notification of Mesh GATT Service.
 *
 *  \par Description
 *  API to configure GATT Notification of Mesh GATT Service.
 *
 *  \param [in] conn_hdl    connection handle to identify device
 *  \param [in] config_ntf  notification configuration flag; enable if MS_TRUE, or disable if MS_FALSE
 *  \param [in] mode        GATT interface mode, either BLEBRR_GATT_PROV_MODE or BLEBRR_GATT_PROXY_MODE
 *
 *  \note This API is for GATT Client only.
 *
 *  \return API_SUCCESS or API_FAILURE
 */
API_RESULT R_MS_BRR_Config_Notification(UINT16 conn_hdl, UCHAR config_ntf, UCHAR mode);

/**
 *  \brief Configure Service Changed Indication of GATT Service.
 *
 *  \par Description
 *  API to configure Service Changed Indication of GATT Service.
 *
 *  \param [in] conn_hdl    connection handle to identify device
 *  \param [in] config_ntf  indication configuration flag; enable if MS_TRUE, or disable if MS_FALSE
 *
 *  \note This API is for GATT Client only.
 *
 *  \return API_SUCCESS or API_FAILURE
 */
API_RESULT R_MS_BRR_Config_ServChanged(UINT16 conn_hdl, UCHAR config_ind);

/** \} */

/** \} */

#endif /* _H_BLEBRR_ */
