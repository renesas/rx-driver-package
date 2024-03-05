/* ${REA_DISCLAIMER_PLACEHOLDER} */

#ifndef R_BLE_GTL_H
#define R_BLE_GTL_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include "r_ble_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#if defined(__CCRX__) || defined(__ICCRX__) || defined(__RX__)
/* SCK port pin macros.  */
#define BLE_SCK_DDR(x, y)           (BLE_SCK_DDR_PREPROC(x, y))
#define BLE_SCK_DDR_PREPROC(x, y)   ((PORT ## x .PDR.BIT.B ## y))
#define BLE_SCK_DR(x, y)            (BLE_SCK_DR_PREPROC(x, y))
#define BLE_SCK_DR_PREPROC(x, y)    ((PORT ## x .PODR.BIT.B ## y))

/* Reset port pin macros.  */
#define BLE_RESET_DDR(x, y)           (BLE_RESET_DDR_PREPROC(x, y))
#define BLE_RESET_DDR_PREPROC(x, y)   ((PORT ## x .PDR.BIT.B ## y))
#define BLE_RESET_DR(x, y)            (BLE_RESET_DR_PREPROC(x, y))
#define BLE_RESET_DR_PREPROC(x, y)    ((PORT ## x .PODR.BIT.B ## y))
#endif

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef struct r_ble_gtl_transport_api
{
    /* To be called before any write or reads are executed */
    int (* open)(void *);
    int (* write)(void *, uint8_t * p_data, uint32_t len);
    int (* read)(void *, uint8_t * p_data, uint32_t len);
    /* To be called after all write or reads are complete and module is disabled. */
    int (* close)(void *);
    void * p_context;
} r_ble_gtl_transport_api_t;

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
#if defined(BLE_CFG_TRANSPORT_INTERFACE_UART)
void R_BLE_GTL_UartCallback (void * pArgs);
#endif

ble_status_t R_BLE_GTL_Open (r_ble_gtl_transport_api_t *p_api);
ble_status_t R_BLE_GTL_Close (r_ble_gtl_transport_api_t *p_api);
ble_status_t R_BLE_GTL_Execute(void);
uint32_t     R_BLE_GTL_IsTaskFree(void);
ble_status_t R_BLE_GTL_GAP_Init(ble_gap_app_cb_t gap_cb);
ble_status_t R_BLE_GTL_GAP_Terminate(void);
ble_status_t R_BLE_GTL_GAP_ConnParamUpdateCfm(uint16_t conn_hdl, uint16_t accept);
ble_status_t R_BLE_GTL_GAP_ConnParamUpdateReq(uint16_t conn_hdl, st_ble_gap_conn_param_t * p_conn_updt_param);
ble_status_t R_BLE_GTL_GAP_SetDataLen(uint16_t conn_hdl, uint16_t tx_octets, uint16_t tx_time);
ble_status_t R_BLE_GTL_GAP_Disconnect(uint16_t conn_hdl, uint8_t reason);
ble_status_t R_BLE_GTL_GAP_GetVerInfo(void);
ble_status_t R_BLE_GTL_GAP_ReadRssi(uint16_t conn_hdl);
ble_status_t R_BLE_GTL_GAP_ReadChMap(uint16_t conn_hdl);
ble_status_t R_BLE_GTL_GAP_SetAdvParam(st_ble_gap_adv_param_t * p_adv_param);
ble_status_t R_BLE_GTL_GAP_SetAdvSresData(st_ble_gap_adv_data_t * p_adv_srsp_data);
ble_status_t R_BLE_GTL_GAP_StartAdv(uint8_t adv_hdl, uint16_t duration);
ble_status_t R_BLE_GTL_GAP_StopAdv(uint8_t adv_hdl);
ble_status_t R_BLE_GTL_GAP_GetRemainAdvBufSize(uint16_t * p_remain_adv_data_size, 
                                               uint16_t * p_remain_perd_adv_data_size);
ble_status_t R_BLE_GTL_GAP_GetRemDevInfo(uint16_t conn_hdl);
ble_status_t R_BLE_GTL_GATT_GetMtu(uint16_t conn_hdl, uint16_t * p_mtu);
ble_status_t R_BLE_GTL_GATTS_RegisterCb(ble_gatts_app_cb_t cb, uint8_t priority);
ble_status_t R_BLE_GTL_GATTS_DeregisterCb(ble_gatts_app_cb_t cb);
ble_status_t R_BLE_GTL_GATTS_SetDbInst(st_ble_gatts_db_cfg_t * p_db_inst);
ble_status_t R_BLE_GTL_GATTS_Notification(uint16_t conn_hdl, st_ble_gatt_hdl_value_pair_t * p_ntf_data);
ble_status_t R_BLE_GTL_GATTS_Indication(uint16_t conn_hdl, st_ble_gatt_hdl_value_pair_t * p_ind_data);
ble_status_t R_BLE_GTL_GATTS_GetAttr(uint16_t conn_hdl, uint16_t attr_hdl, st_ble_gatt_value_t * p_value);
ble_status_t R_BLE_GTL_GATTS_SetAttr(uint16_t conn_hdl, uint16_t attr_hdl, st_ble_gatt_value_t * p_value);
ble_status_t R_BLE_GTL_GATTC_RegisterCb(ble_gattc_app_cb_t cb, uint8_t priority);
ble_status_t R_BLE_GTL_GATTC_DeregisterCb(ble_gattc_app_cb_t cb);
ble_status_t R_BLE_GTL_GATTC_ReqExMtu(uint16_t conn_hdl, uint16_t mtu);
ble_status_t R_BLE_GTL_GATTC_DiscAllPrimServ(uint16_t conn_hdl);
ble_status_t R_BLE_GTL_GATTC_DiscPrimServ(uint16_t conn_hdl, uint8_t * p_uuid, uint8_t uuid_type);
ble_status_t R_BLE_GTL_GATTC_DiscIncServ(uint16_t conn_hdl, st_ble_gatt_hdl_range_t * p_range);
ble_status_t R_BLE_GTL_GATTC_DiscAllChar(uint16_t conn_hdl, st_ble_gatt_hdl_range_t * p_range);
ble_status_t R_BLE_GTL_GATTC_DiscCharByUuid(uint16_t                  conn_hdl,
                                            uint8_t                 * p_uuid,
                                            uint8_t                   uuid_type,
                                            st_ble_gatt_hdl_range_t * p_range);
ble_status_t R_BLE_GTL_GATTC_DiscAllCharDesc (uint16_t conn_hdl, st_ble_gatt_hdl_range_t * p_range);
ble_status_t R_BLE_GTL_GATTC_ReadChar(uint16_t conn_hdl, uint16_t value_hdl);
ble_status_t R_BLE_GTL_GATTC_ReadCharUsingUuid(uint16_t                  conn_hdl,
                                               uint8_t                 * p_uuid,
                                               uint8_t                   uuid_type,
                                               st_ble_gatt_hdl_range_t * p_range);
ble_status_t R_BLE_GTL_GATTC_ReadLongChar(uint16_t conn_hdl, uint16_t value_hdl, uint16_t offset);
ble_status_t R_BLE_GTL_GATTC_ReadMultiChar(uint16_t conn_hdl, st_ble_gattc_rd_multi_req_param_t * p_list);
ble_status_t R_BLE_GTL_GATTC_WriteCharWithoutRsp(uint16_t conn_hdl, st_ble_gatt_hdl_value_pair_t * p_write_data);
ble_status_t R_BLE_GTL_GATTC_SignedWriteChar(uint16_t conn_hdl, st_ble_gatt_hdl_value_pair_t * p_write_data);
ble_status_t R_BLE_GTL_GATTC_WriteChar(uint16_t conn_hdl, st_ble_gatt_hdl_value_pair_t * p_write_data);
ble_status_t R_BLE_GTL_GATTC_WriteLongChar(uint16_t conn_hdl, 
                                           st_ble_gatt_hdl_value_pair_t * p_write_data, 
                                           uint16_t offset);
ble_status_t R_BLE_GTL_GATTC_ReliableWrites (uint16_t                                   conn_hdl,
                                             st_ble_gattc_reliable_writes_char_pair_t * p_char_pair,
                                             uint8_t                                    pair_num,
                                             uint8_t                                    auto_flag);
ble_status_t R_BLE_GTL_GATTC_ExecWrite(uint16_t conn_hdl, uint8_t exe_flag);
ble_status_t R_BLE_GTL_L2CAP_RegisterCfPsm (ble_l2cap_cf_app_cb_t cb, uint16_t psm, uint16_t lwm);
ble_status_t R_BLE_GTL_L2CAP_DeregisterCfPsm (uint16_t psm);
ble_status_t R_BLE_GTL_L2CAP_ReqCfConn(uint16_t conn_hdl, st_ble_l2cap_conn_req_param_t * p_conn_req_param);
ble_status_t R_BLE_GTL_L2CAP_DisconnectCf(uint16_t lcid);
ble_status_t R_BLE_GTL_L2CAP_SendCfCredit(uint16_t lcid, uint16_t credit);
ble_status_t R_BLE_GTL_L2CAP_SendCfData(uint16_t conn_hdl, uint16_t lcid, uint16_t data_len, uint8_t * p_sdu);
ble_status_t R_BLE_GTL_VS_Init(ble_vs_app_cb_t vs_cb);
ble_status_t R_BLE_GTL_VS_GetBdAddr(uint8_t addr_type);
ble_status_t R_BLE_GTL_VS_SetBdAddr(st_ble_dev_addr_t * p_addr);
ble_status_t R_BLE_GTL_VS_GetRand(uint8_t rand_size);

#endif                                 /* R_BLE_GTL_H */
