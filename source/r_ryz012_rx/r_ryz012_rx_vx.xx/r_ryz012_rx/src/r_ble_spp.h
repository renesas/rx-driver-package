/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_ble_spp.h
 * Description  :
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include "hal_data.h"
#include "wrap_sci.h"
#include "qe_ble_profile.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef R_BLE_SPP_H_
#define R_BLE_SPP_H_

/* Version information format Major.Minor (3.3) */
#define R_BLE_SPP_MODULE_MAJOR_VERSION					(0x03)
#define R_BLE_SPP_MODULE_MINOR_VERSION					(0x03)

#define R_BLE_SPP_EVENT_SEND_INDICATION                 (0x0726)
#define R_BLE_SPP_EVENT_NUMERIC_COMPARE_SUCCESS         (0x0732)
#define R_BLE_SPP_EVENT_NUMERIC_COMPARE_FAIL            (0x0733)
#define R_BLE_SPP_EVENT_NO_SECURITY                     (0x0735)
#define R_BLE_SPP_EVENT_SECURITY_PASSKEY                (0x0736)
#define R_BLE_SPP_EVENT_SECURITY_PASSWORD               (0x0737)
#define R_BLE_SPP_EVENT_BLE_INIT                        (0x0742)
#define R_BLE_SPP_EVENT_SET_SCAN_RES_DATA               (0x0744)
#define R_BLE_SPP_EVENT_SET_ADV_CHANNEL                 (0x0745)
#define R_BLE_SPP_EVENT_BLE_READY                       (0x0746)
#define R_BLE_SPP_EVENT_UNPAIR                          (0x0747)
#define R_BLE_SPP_EVENT_ENA_DEBUG_EVENT                 (0x0748)
#define R_BLE_SPP_EVENT_EXT_PHY_TYPE                    (0x0749)
#define R_BLE_SPP_EVENT_ENA_EXT_ADV                     (0x0750)
#define R_BLE_SPP_EVENT_EXT_SET_ADV_TYPE                (0x0751)
#define R_BLE_SPP_EVENT_EXT_SET_ADV_DATA                (0x0752)
#define R_BLE_SPP_EVENT_EXT_SCAN_RES_DATA               (0x0755)
#define R_BLE_SPP_EVENT_EXT_ADV_ENABLE                  (0x0757)
#define R_BLE_SPP_EVENT_DYNAMIC_ATTRIBUTE_CREATE        (0x0770)
#define R_BLE_SPP_EVENT_DYNAMIC_ATTRIBUTE_COMPLETE      (0x0771)
#define R_BLE_SPP_EVENT_GET_VERSION                     (0x0772)
#define R_BLE_SPP_EVENT_BLE_CONFIG                      (0x0773)
#define R_BLE_SPP_EVENT_SET_SCAN_PARAM                  (0x0774)
#define R_BLE_SPP_EVENT_START_SCAN                      (0x0775)
#define R_BLE_SPP_EVENT_STOP_SCAN                       (0x0776)
#define R_BLE_SPP_EVENT_CONNECT                         (0x0777)
#define R_BLE_SPP_EVENT_DISCONNECT                      (0x0778)
#define R_BLE_SPP_EVENT_CONNECTION_EVENT                (0x0783)
#define R_BLE_SPP_EVENT_DISCONNECTION_EVENT             (0x0784)
#define R_BLE_SPP_SMP_PAIRING_BEGIN_EVENT 				(0x0e80)
#define R_BLE_SPP_SMP_PAIRING_SUCCESS_EVENT 			(0x0e81)
#define R_BLE_SPP_SMP_PAIRING_FAIL_EVENT 				(0x0e82)
#define R_BLE_SPP_SMP_CONNECTION_ENCRY_DONE_EVENT		(0x0e83)
#define R_BLE_SPP_SMP_NUMERIC_COMPARE_EVENT				(0x0e87)

/* error status for event frame */
#define R_BLE_SPP_ERR_UNKNOWN_COMMAND                   (0xF0)
#define R_BLE_SPP_ERR_INVALID_PARAM                     (0xF1)
#define R_BLE_SPP_ERR_CHCK_SUM_ERROR                    (0xF2)
#define R_BLE_SPP_ERR_INIT_ERROR                        (0xF3)
#define R_BLE_SPP_ERR_DYN_MEMALLOC_ERROR                (0xF4)
#define R_BLE_SPP_ERR_DYN_ATTR_SIZE_ERROR               (0xF5)
#define R_BLE_SPP_ERR_INVALID_LENGTH                    (0xF6)
#define R_BLE_SPP_ERR_INVALID_COMMAND                   (0xF7)
#define R_BLE_SPP_ERR_CONFIG_ERROR                      (0xF8)
#define R_BLE_SPP_ERR_BLE_READ_ERROR                    (0xF9)
#define R_BLE_SPP_ERR_BLE_INVALID_ATT_HANDLE            (0xFA)

#define R_BLE_SPP_BLE_ADDR_LEN		                    (6)
#define R_BLE_SPP_MAX_DEV_NAME_LEN	                    (248)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
typedef enum
{
	R_BLE_SPP_SUCCESS = 0,
	R_BLE_SPP_ERROR = 1,
	R_BLE_SPP_INVALID_PARAMETER = 2,
	R_BLE_SPP_DEVICE_NOT_OPEN = 3 ,
	R_BLE_SPP_DEVICE_NOT_CLOSED = 4 ,
	R_BLE_APP_TRANSPORT_WRITE_ERROR = 5,
	R_BLE_SPP_DEVICE_INVALID_OPTION = 6
} r_ble_spp_cmd_rsp_t;

typedef enum
{
	R_BLE_SPP_SCAN_TYPE_PASSIVE,
	R_BLE_SPP_SCAN_TYPE_ACTIVE
} r_ble_spp_scan_type_t;

typedef enum
{
	R_BLE_SPP_SCAN_FP_ALLOW_ADV_ANY,                // except direct adv address not match
	R_BLE_SPP_SCAN_FP_ALLOW_ADV_WL,                 // except direct adv address not match
	R_BLE_SPP_SCAN_FP_ALLOW_UNDIRECT_ADV,           // and direct adv address match initiator's resolvable private MAC
	R_BLE_SPP_SCAN_FP_ALLOW_ADV_WL_DIRECT_ADV_MACTH // and direct adv address match initiator's resolvable private MAC

} r_ble_spp_scan_filterpolicy_t;

typedef enum r_ble_spp_mode
{
	R_BLE_SPP_MASTER =1,
	R_BLE_SPP_SLAVE=2
} r_ble_spp_mode_t;

typedef enum r_ble_spp_adv_type
{
	R_BLE_SPP_ADV_DATA_MODE,
	R_BLE_SPP_SCAN_RSP_DATA_MODE,
	R_BLE_SPP_PERD_ADV_DATA_MODE

} r_ble_spp_adv_type_t;

typedef enum r_ble_spp_whiteList_optype
{
	R_BLE_SPP_LIST_ADD_DEV,
	R_BLE_SPP_LIST_REM_DEV,
	R_BLE_SPP_LIST_CLR

} r_ble_spp_whiteList_optype_t;

typedef enum r_ble_spp_phy_type
{
	R_BLE_SPP_SET_PHYS_PREF_1M = 1,
	R_BLE_SPP_SET_PHYS_PREF_2M = 2,
	R_BLE_SPP_SET_PHYS_PREF_CD = 3

}r_ble_spp_phy_type_t;

typedef enum r_ble_spp_phy_options
{
	R_BLE_SPP_SET_PHYS_NO_PREF,
	R_BLE_SPP_SET_PHYS_S_2,
	R_BLE_SPP_SET_PHYS_S_8

} r_ble_spp_phy_options_t;

typedef enum r_ble_spp_own_bd_addrType
{
	R_BLE_SPP_ADDR_PUB,
	R_BLE_SPP_ADDR_RAND,
	R_BLE_SPP_ADDR_RPA_ID_PUBLIC,
	R_BLE_SPP_ADDR_RPA_ID_RANDOM

} r_ble_spp_own_addrType_t;

typedef enum r_ble_spp_peer_addrType
{
	R_BLE_SPP_ADDR_PUBLIC,
	R_BLE_SPP_ADDR_RANDOM

}r_ble_spp_peer_addrType_t;

typedef enum r_ble_spp_filter_policy
{
	R_BLE_SPP_ADV_ALLOW_SCAN_ANY_CONN_ANY,		// Process scan and connection requests from all devices.
	R_BLE_SPP_ADV_ALLOW_SCAN_WLST_CONN_ANY,		// Process connection requests from all devices and scan requests from only devices that are in the White List.
	R_BLE_SPP_ADV_ALLOW_SCAN_ANY_CONN_WLST,		// Process scan requests from all devices and connection requests from only devices that are in the White List.
	R_BLE_SPP_ADV_ALLOW_SCAN_WLST_CONN_WLST		// Process scan and connection requests from only devices in the White List.

} r_ble_spp_filter_policy_t;

typedef enum r_ble_spp_cur_state
{
	R_BLE_SPP_DEV_IN_IDLE_MODE = 0,
	R_BLE_SPP_DEV_IN_ADV_MODE = 1,
	R_BLE_SPP_DEV_IN_CONNECTED_MODE = 8,
} r_ble_spp_cur_state_t;

typedef enum
{
    R_BLE_SPP_IO_CAP_UNKNOWN = 0xff,
    R_BLE_SPP_IO_CAP_DISPLAY_ONLY = 0,
    R_BLE_SPP_IO_CAP_DISPLAY_YES_NO = 1,
    R_BLE_SPP_IO_CAP_KEYBOARD_ONLY = 2,
    R_BLE_SPP_IO_CAP_NO_INPUT_NO_OUTPUT = 3,
    R_BLE_SPP_IO_CAP_KEYBOARD_DISPLAY = 4,
} r_ble_spp_io_capability_t;

typedef enum
{
    R_BLE_SPP_NON_BONDABLE_MODE = 0,
    R_BLE_SPP_BONDABLE_MODE     = 1,
} r_ble_spp_bonding_mode_t;

typedef enum
{
    R_BLE_SPP_SEC_MODE_1_LEVEL_1 = 0,
    R_BLE_SPP_SEC_MODE_1_LEVEL_2,
    R_BLE_SPP_SEC_MODE_1_LEVEL_3,
    R_BLE_SPP_SEC_MODE_1_LEVEL_4
} r_ble_spp_security_mode_level_t;

typedef enum
{
    R_BLE_SPP_SEC_LEVEL_2_METHOD_1 = 0,
    R_BLE_SPP_SEC_LEVEL_2_METHOD_2,
} r_ble_spp_security_level_method_t;

typedef enum
{
    R_BLE_SPP_FP_ADV_SPECIFY = 0,   // connect ADV specified by host
    R_BLE_SPP_FP_ADV_WL             // connect ADV in whiteList
} r_ble_spp_conn_fp_t;

typedef enum
{
	R_BLE_SPP_ADV_TYPE_CONNECTABLE_UNDIRECTED,
	R_BLE_SPP_ADV_TYPE_CONNECTABLE_DIRECTED_HIGH_DUTY,
	R_BLE_SPP_ADV_TYPE_SCANNABLE_UNDIRECTED,
	R_BLE_SPP_ADV_TYPE_NONCONNECTABLE_UNDIRECTED,
	R_BLE_SPP_ADV_TYPE_CONNECTABLE_DIRECTED_LOW_DUTY,
	R_BLE_SPP_ADV_EVT_PROP_LEGACY_CONNECTABLE_SCANNABLE_UNDIRECTED,
	R_BLE_SPP_ADV_EVT_PROP_LEGACY_CONNECTABLE_DIRECTED_LOW_DUTY,
	R_BLE_SPP_ADV_EVT_PROP_LEGACY_CONNECTABLE_DIRECTED_HIGH_DUTY,
	R_BLE_SPP_ADV_EVT_PROP_LEGACY_SCANNABLE_UNDIRECTED,
	R_BLE_SPP_ADV_EVT_PROP_LEGACY_NON_CONNECTABLE_NON_SCANNABLE_UNDIRECTED,
	R_BLE_SPP_ADV_EVT_PROP_EXTENDED_NON_CONNECTABLE_NON_SCANNABLE_UNDIRECTED,
	R_BLE_SPP_ADV_EVT_PROP_EXTENDED_CONNECTABLE_UNDIRECTED,
	R_BLE_SPP_ADV_EVT_PROP_EXTENDED_SCANNABLE_UNDIRECTED,
	R_BLE_SPP_ADV_EVT_PROP_EXTENDED_NON_CONNECTABLE_NON_SCANNABLE_DIRECTED,
	R_BLE_SPP_ADV_EVT_PROP_EXTENDED_CONNECTABLE_DIRECTED,
	R_BLE_SPP_ADV_EVT_PROP_EXTENDED_SCANNABLE_DIRECTED,
	R_BLE_SPP_ADV_EVT_PROP_EXTENDED_MASK_ANONYMOUS_ADV,
	R_BLE_SPP_ADV_EVT_PROP_EXTENDED_MASK_TX_POWER_INCLUDE,
} r_ble_spp_adv_prop_type_t;


/*********************************************************
 * Structure types
 ********************************************************/
typedef struct r_ble_spp_profile_params
{
	uint16_t    payload_length;
	uint16_t	attNum;
	uint8_t		perm;
	uint8_t  	uuidLen;
	uint16_t 	attrLen;
	uint8_t     *uuid;
	uint8_t     *pAttrValue;
    uint8_t 	write_flag;
	uint8_t 	read_flag;
} r_ble_spp_profile_params_t;

typedef struct r_ble_spp_whitelist
{
	r_ble_spp_peer_addrType_t addr_type;
	uint8_t *p_addr;
} r_ble_spp_whitelist_t;

typedef struct r_ble_spp_scan_phy_param
{
	r_ble_spp_scan_type_t   scan_type;
	uint16_t                scan_interval;
    uint16_t                scan_window;

} r_ble_spp_scan_phy_param_t;

typedef struct r_ble_spp_scan_parameter
{
	r_ble_spp_own_addrType_t 			own_addr_type;
	r_ble_spp_scan_filterpolicy_t 		scan_filter_policy;
	r_ble_spp_scan_phy_param_t			p_scan_param;
} r_ble_spp_scan_parameter_t;

typedef struct r_ble_spp_conn_param
{
	uint16_t conn_intv_min;
	uint16_t conn_intv_max;
	uint16_t conn_latency;
	uint16_t sup_timout;

} r_ble_spp_conn_param_t;

typedef struct r_ble_spp_conn_phy_param
{
	uint16_t scan_intv;
	uint16_t scan_window;
	r_ble_spp_conn_param_t p_param;

} r_ble_spp_conn_phy_param_t;

typedef struct r_ble_spp_create_conn_param
{
	r_ble_spp_conn_fp_t init_filter_policy;         // This field specifies whether the White List is used or not, when connecting with a remote device.
	uint8_t remote_bd_addr[R_BLE_SPP_BLE_ADDR_LEN];	// Address of the device to be connected.
	r_ble_spp_peer_addrType_t remote_bd_addr_type;	// Address type of the device to be connected.
	r_ble_spp_own_addrType_t own_addr_type;
	r_ble_spp_conn_phy_param_t  p_conn_param;
} r_ble_spp_create_conn_param_t;

typedef struct r_ble_spp_adv_data
{
	r_ble_spp_adv_type_t	data_type;
	uint16_t data_length;
	uint8_t *p_data;
} r_ble_spp_adv_data_t;

typedef struct r_ble_spp_adv_param
{
	uint8_t                     adv_ch_map;
	r_ble_spp_filter_policy_t   filter_policy;
	r_ble_spp_phy_type_t        adv_phy;	    // Primary ADV PHY
	r_ble_spp_phy_type_t        sec_adv_phy;	// Secondary ADV PHY
	r_ble_spp_adv_prop_type_t   adv_prop_type;  // Advertising packet type
	uint32_t adv_intv_min;
	uint32_t adv_intv_max;
} r_ble_spp_adv_param_t;

typedef struct r_ble_spp_version_info
{
	uint8_t minor_ver;
	uint8_t major_ver;
	uint8_t build_ver;
} r_ble_spp_version_info_t;

typedef struct r_ble_spp_set_phy_param
{
	r_ble_spp_phy_type_t phy_type;
	r_ble_spp_phy_options_t phy_options;
} r_ble_spp_set_phy_param_t;

typedef struct r_ble_spp_gatt_value
{
	uint16_t value_len;
	uint8_t  *p_value;
} r_ble_spp_gatt_value_t;

typedef struct r_ble_spp_gatt_hdl_value_pair
{
	uint16_t attr_hdl;
	r_ble_spp_gatt_value_t  value;
} r_ble_spp_gatt_hdl_value_pair_t;

typedef struct r_ble_spp_payload
{
	uint32_t  event_id;
	uint32_t  out_len;
	uint8_t   *out_data;
} r_ble_spp_payload_t;

typedef struct r_ble_spp_cfg
{
	/* To be called before any write or reads are executed */
	int (* open)(void *);
	int (* write)(void *, uint8_t * p_data, uint32_t len);
	int (* read)(void *, uint8_t * p_data, uint32_t len); // for spi. if required for uart please notify renesas so we can rework up our code.

	/* To be called after all write or reads are complete and module is disabled. */
	int (* close)(void *);
	void * p_context;
	void (* mw_callback)(r_ble_spp_payload_t* p_payload);
} r_ble_spp_cfg_t;

typedef struct r_ble_spp_send_data
{
	uint16_t handle;
	uint8_t *data;
} r_ble_spp_send_data_t;

typedef struct r_ble_spp_gap_pairing_param
{
	r_ble_spp_io_capability_t iocap;    // IO capabilities of local device.
	r_ble_spp_bonding_mode_t bonding;   // Bonding policy
	r_ble_spp_security_mode_level_t sec_level;
	r_ble_spp_security_level_method_t auth_methods;
} r_ble_spp_gap_pairing_param_t;

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Open(r_ble_spp_cfg_t *p_api);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Config_Mode(r_ble_spp_mode_t mode);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Config_Profile(r_ble_spp_profile_params_t *prf_info);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Profile_Complete(void);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetMACID(r_ble_spp_peer_addrType_t p_addr_type, uint8_t *mac_id);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Init(void);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_StartAdv(void);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetAdvSresData(r_ble_spp_adv_data_t *p_adv_srsp_data);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetAdvParam(r_ble_spp_adv_param_t *p_adv_param);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetPhy(r_ble_spp_set_phy_param_t *p_phy_info);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_StopAdv(void);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Disconnect(void);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Restart(void);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_ConfWhiteList(r_ble_spp_whiteList_optype_t op_code,r_ble_spp_whitelist_t *p_whitelist);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetDevName(unsigned char *dev_name);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetTxPower(uint8_t tx_power);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetAddr(r_ble_spp_own_addrType_t addr_type, uint8_t *p_addr);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetConnParam(r_ble_spp_conn_param_t *p_param);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_GetConnParam(void);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_GetRSSI(void);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_GetCurrState(void);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_GetVersion(void);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_GetBufSize(void);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Unpair(void);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTS_Notification(r_ble_spp_gatt_hdl_value_pair_t  *p_ntf_data);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTS_Indication(r_ble_spp_gatt_hdl_value_pair_t  *p_ind_data);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_DebugEvent(uint8_t debug_event, uint8_t status);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SecurityPasskey(void);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetPairingParams(r_ble_spp_gap_pairing_param_t *p_pair_param);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_StartPairing(r_ble_spp_gap_pairing_param_t *p_pair_param);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_ReplyNumComp(uint8_t response);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetScanParam(r_ble_spp_scan_parameter_t *scan_params);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_StartScan(void);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_StopScan(void);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Connect(r_ble_spp_create_conn_param_t *p_param);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTC_DiscoverAllAttributes(void);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTC_ReadCharacteristic(uint16_t handle);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTC_ReadBlob(uint16_t handle);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTC_WriteCharacteristic(uint16_t handle, uint8_t *data, uint16_t length);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTC_WriteCharWithoutResponse(uint16_t handle, uint8_t *data, uint16_t length);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_EnableRpa(uint8_t enable);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_ConfigRpa(void);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_SetRemOobData( uint8_t oob_data_flag, uint8_t *p_oob);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTS_SetAttr(r_ble_spp_gatt_hdl_value_pair_t *p_value);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GATTS_GetAttr(r_ble_spp_gatt_hdl_value_pair_t *p_value);
r_ble_spp_cmd_rsp_t R_BLE_SPP_GAP_Close(r_ble_spp_cfg_t *p_api);
void r_ble_spp_rx_char_callback(uint8_t byte);

#endif /* R_BLE_SPP_H */
