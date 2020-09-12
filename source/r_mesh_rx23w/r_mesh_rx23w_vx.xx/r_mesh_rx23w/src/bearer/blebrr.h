
/**
 *  \file blebrr.h
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_BLEBRR_
#define _H_BLEBRR_

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_mesh_rx23w_if.h"
#include "cli/r_ble_cli.h"

/*******************************************************************************
* Macro definitions
*******************************************************************************/
/* Compilation Switch to have this module print trace on console */
/* #define BLEBRR_CONSOLE_DEBUG */

#ifdef BLEBRR_CONSOLE_DEBUG
#define BLEBRR_LOG(...)                         R_BLE_CLI_Printf(__VA_ARGS__)
#else /* BLEBRR_CONSOLE_DEBUG */
#define BLEBRR_LOG(...)
#endif /* BLEBRR_CONSOLE_DEBUG */

/** GATT Modes */
#define BLEBRR_GATT_PROV_MODE                   0x00
#define BLEBRR_GATT_PROXY_MODE                  0x01

/** Bearer Client Server Roles */
#define BLEBRR_CLIENT_ROLE                      BRR_CLIENT_ROLE
#define BLEBRR_SERVER_ROLE                      BRR_SERVER_ROLE

/** Bearer GATT communication Channel setup events */
#define BLEBRR_COM_CHANNEL_CONNECT              0x00
#define BLEBRR_COM_CHANNEL_DISCONNECT           0x01

/** Bearer GATT MTU related defines */
#define BLEBRR_GATT_MIN_MTU                     (23 - 3)

/* Bearer GATT Service related defines */
#define BLEBRR_MESH_PRVSNG_SERVICE              0x1827
#define BLEBRR_MESH_PROXY_SERVICE               0x1828

#define BLEBRR_MESH_PRVSNG_DATA_IN_CHAR         0x2ADB
#define BLEBRR_MESH_PRVSNG_DATA_OUT_CHAR        0x2ADC
#define BLEBRR_MESH_PROXY_DATA_IN_CHAR          0x2ADD
#define BLEBRR_MESH_PROXY_DATA_OUT_CHAR         0x2ADE

/** GATT Interface Events */
#define BLEBRR_GATT_IFACE_UP                    0x00
#define BLEBRR_GATT_IFACE_DOWN                  0x01
#define BLEBRR_GATT_IFACE_ENABLE                0x02
#define BLEBRR_GATT_IFACE_DISABLE               0x03

/*******************************************************************************
* Type definitions
*******************************************************************************/
/* Callback to Inform Application Layer about Bearer Initialization Completion */
typedef void (* BLEBRR_INIT_CB)
                (
                    void
                );

/* Callback to Inform Application Layer about GATT Bearer Iface Events */
typedef void (* BLEBRR_GATT_IFACE_EVENT_PL_CB)
                (
                    UCHAR ev_name,
                    UCHAR ev_param
                );

/*******************************************************************************
* Prototype declarations
*******************************************************************************/
void  blebrr_init(BLEBRR_INIT_CB init_cb);
void  blebrr_register(void);
void  blebrr_scan_enable(void);
void  blebrr_pl_scan_setup(UCHAR enable);
void  blebrr_pl_advertise_setup(UCHAR enable);
void  blebrr_pl_advertise_end(void);
void  blebrr_pl_recv_advpacket(UCHAR type, UCHAR * pdata, UINT16 pdatalen, UCHAR rssi);
void  blebrr_gatt_mode_set(UCHAR flag);
UCHAR blebrr_gatt_mode_get(void);
API_RESULT blebrr_pl_gatt_connection(BRR_HANDLE * handle, UCHAR role, UCHAR mode, UINT16 mtu);
API_RESULT blebrr_pl_gatt_disconnection(BRR_HANDLE * handle);
API_RESULT blebrr_pl_recv_gattpacket(BRR_HANDLE * handle, UCHAR * pdata, UINT16 pdatalen);
API_RESULT blebrr_gatt_send_pl(BRR_HANDLE * handle, UCHAR * data, UINT16 datalen);
API_RESULT blebrr_create_gatt_conn_pl(UCHAR p_bdaddr_type, UCHAR * p_bdaddr);
API_RESULT blebrr_disconnect_pl(void);
API_RESULT blebrr_discover_service_pl(UCHAR serv);
API_RESULT blebrr_confige_ntf_pl(UCHAR config_ntf, UCHAR mode);
API_RESULT blebrr_set_adv_scanrsp_data_pl(UCHAR * srp_data, UCHAR srp_datalen);
API_RESULT blebrr_register_gatt_iface_event_pl(BLEBRR_GATT_IFACE_EVENT_PL_CB gatt_iface_evt_cb);
void  blebrr_scan_pl(UCHAR enable);
void  blebrr_advertise_data_pl(CHAR type, UCHAR * pdata, UINT16 pdatalen);
void  blebrr_advertise_pl(UCHAR state);
UCHAR blebrr_get_advdata_offset_pl(void);
void  blebrr_set_gattmode_pl(UCHAR flag);

extern INT32 cry_rand_generate(UCHAR * prand, UINT16 randlen);

#endif /* _H_BLEBRR_ */

