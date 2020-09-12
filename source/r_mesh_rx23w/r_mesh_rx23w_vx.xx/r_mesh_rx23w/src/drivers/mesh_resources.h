/**
 *  \file mesh_mempool.h
 */

#ifndef _MESH_MEMPOOL_
#define _MESH_MEMPOOL_

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_mesh_rx23w_if.h"

/*******************************************************************************
* Macro definitions
*******************************************************************************/
/* utility */
#define ALIGN_UINT32(size)          ((((size) + 4 - 1) / 4) * 4)
#define STRARRAY_SIZE(name)          (ALIGN_UINT32(((TYPE_##name) * (LEN_##name))))
#define BITARRAY_BLK                (32U)
#define BITARRAY_SIZE(bits)         (((bits) + (BITARRAY_BLK - 1)) / (BITARRAY_BLK))

/* Structure Size */
#define STR_BRR_BEARER_DATA                             (160)
#define STR_NET_CACHE_ELEMENT                           (8)
#define STR_PROXY_FILTER_LIST                           (8)
#define STR_PROXY_ADDR                                  (2)
#define STR_BRR_HANDLE                                  (1)
#define STR_LTRN_REPLAY_CACHE_ELEMENT                   (8)
#define STR_LTRN_REASSEMBLED_FRAME_INFO                 (12)
#define STR_LTRN_SAR_CTX                                (48)
#define STR_TRN_LPN_ELEMENT                             (56)
#define STR_MS_NET_ADDR                                 (2)
#define STR_TRN_FRN_Q_ELEMENT                           (32)
#define STR_TRN_FRN_Q_ELEMENT                           (32)
#define STR_MS_ACCESS_ELEMENT_TYPE                      (6)
#define STR_MS_ACCESS_MODEL_TYPE                        (60)
#define STR_UINT32                                      (4)
#define STR_MS_NETKEY_ENTRY                             (120)
#define STR_MS_APPKEY_ENTRY                             (40)
#define STR_MS_DEV_KEY_ENTRY                            (20)
#define STR_MS_ELEMENT_ADDR_ENTRY                       (4)
#define STR_MS_NON_VIRTUAL_ADDR_ENTRY                   (4)
#define STR_MS_VIRTUAL_ADDR_ENTRY                       (20)
#define STR_PROV_BRR_INFO                               (1)
#define STR_MS_HEALTH_SERVER_T                          (88)

/* Array Type */
#define TYPE_BRR_BEARER                                 (STR_BRR_BEARER_DATA)
#define TYPE_NET_CACHE                                  (STR_NET_CACHE_ELEMENT)
#define TYPE_NET_PROXY_LIST                             (STR_PROXY_FILTER_LIST)
#define TYPE_NET_PROXY_LIST__P_ADDR                     (STR_PROXY_ADDR)
#define TYPE_NETIF_BRR_HANDLE                           (STR_BRR_HANDLE)
#define TYPE_REPLAY_CACHE                               (STR_LTRN_REPLAY_CACHE_ELEMENT)
#define TYPE_REASSEMBLED_CACHE                          (STR_LTRN_REASSEMBLED_FRAME_INFO)
#define TYPE_LTRN_SAR_CTX                               (STR_LTRN_SAR_CTX)
#define TYPE_LPN_ELEMENT                                (STR_TRN_LPN_ELEMENT)
#define TYPE_LPN_ELEMENT__SUBSCRIPTION_LIST             (STR_MS_NET_ADDR)
#define TYPE_LPN_ELEMENT__0_QUEUE                       (STR_TRN_FRN_Q_ELEMENT)
#define TYPE_LPN_ELEMENT__1_QUEUE                       (STR_TRN_FRN_Q_ELEMENT)
#define TYPE_MS_ACCESS_ELEMENT_LIST                     (STR_MS_ACCESS_ELEMENT_TYPE)
#define TYPE_MS_ACCCES_MODEL_LIST                       (STR_MS_ACCESS_MODEL_TYPE)
#define TYPE_MS_ACCCES_MODEL_LIST_APP_KEY               (STR_UINT32)
#define TYPE_MS_ACCCES_MODEL_LIST_APP_SUBSCRIPTION      (STR_UINT32)
#define TYPE_MS_SUBNET_TABLE                            (STR_MS_NETKEY_ENTRY)
#define TYPE_MS_SUBNET_TABLE__APPKEY                    (STR_UINT32)
#define TYPE_MS_SUBNET_TABLE__FRIEND                    (STR_UINT32)
#define TYPE_MS_APPKEY_TABLE                            (STR_MS_APPKEY_ENTRY)
#define TYPE_MS_DEV_KEY_TABLE                           (STR_MS_DEV_KEY_ENTRY)
#define TYPE_MS_ELEMENT_ADDR_TABLE                      (STR_MS_ELEMENT_ADDR_ENTRY)
#define TYPE_MS_NON_VIRTUAL_ADDR_TABLE                  (STR_MS_NON_VIRTUAL_ADDR_ENTRY)
#define TYPE_MS_VIRTUAL_ADDR_TABLE                      (STR_MS_VIRTUAL_ADDR_ENTRY)
#define TYPE_PROV_BRR                                   (STR_PROV_BRR_INFO)
#define TYPE_HEALTH_SERVER                              (STR_MS_HEALTH_SERVER_T)

/* Array Length */
#define LEN_BRR_BEARER                                  (MESH_CFG_NUM_NETWORK_INTERFACES)
#define LEN_NET_CACHE                                   (MESH_CFG_NET_CACHE_SIZE)
#define LEN_NET_PROXY_LIST                              (MESH_CFG_NUM_NETWORK_INTERFACES - 1)
#define LEN_NET_PROXY_LIST__P_ADDR                      (MESH_CFG_PROXY_FILTER_LIST_SIZE)
#define LEN_NETIF_BRR_HANDLE                            (MESH_CFG_NUM_NETWORK_INTERFACES)
#define LEN_REPLAY_CACHE                                (MESH_CFG_REPLAY_CACHE_SIZE)
#define LEN_REASSEMBLED_CACHE                           (MESH_CFG_REASSEMBLED_CACHE_SIZE)
#define LEN_LTRN_SAR_CTX                                (MESH_CFG_LTRN_SAR_CTX_MAX)
#define LEN_LPN_ELEMENT                                 (MESH_CFG_MAX_LPNS)
#define LEN_LPN_ELEMENT__SUBSCRIPTION_LIST              (MESH_CFG_FRIEND_SUBSCRIPTION_LIST_SIZE)
#define LEN_LPN_ELEMENT__0_QUEUE                        (MESH_CFG_FRIEND_MESSAGEQUEUE_SIZE)
#define LEN_LPN_ELEMENT__1_QUEUE                        (MESH_CFG_FRIEND_MESSAGEQUEUE_SIZE)
#define LEN_MS_ACCESS_ELEMENT_LIST                      (MESH_CFG_ACCESS_ELEMENT_COUNT)
#define LEN_MS_ACCCES_MODEL_LIST                        (MESH_CFG_ACCESS_MODEL_COUNT)
#define LEN_MS_ACCCES_MODEL_LIST_APP_KEY                (BITARRAY_SIZE(MESH_CFG_MAX_APPS))
#define LEN_MS_ACCCES_MODEL_LIST_APP_SUBSCRIPTION       (BITARRAY_SIZE(MESH_CFG_MAX_VIRTUAL_ADDRS + MESH_CFG_MAX_NON_VIRTUAL_ADDRS))
#define LEN_MS_SUBNET_TABLE                             (MESH_CFG_MAX_SUBNETS + MESH_CFG_MAX_LPNS)
#define LEN_MS_SUBNET_TABLE__APPKEY                     (BITARRAY_SIZE(MESH_CFG_MAX_APPS))
#define LEN_MS_SUBNET_TABLE__FRIEND                     (BITARRAY_SIZE(MESH_CFG_MAX_LPNS))
#define LEN_MS_APPKEY_TABLE                             (MESH_CFG_MAX_APPS)
#define LEN_MS_DEV_KEY_TABLE                            (MESH_CFG_MAX_DEV_KEYS)
#define LEN_MS_ELEMENT_ADDR_TABLE                       (1 + MESH_CFG_MAX_LPNS)
#define LEN_MS_NON_VIRTUAL_ADDR_TABLE                   (MESH_CFG_MAX_NON_VIRTUAL_ADDRS)
#define LEN_MS_VIRTUAL_ADDR_TABLE                       (MESH_CFG_MAX_VIRTUAL_ADDRS)
#define LEN_PROV_BRR                                    (MESH_CFG_NUM_PROVISIONING_INTERFACES)
#define LEN_HEALTH_SERVER                               (MESH_CFG_HEALTH_SERVER_MAX)

#define MEMPOOL_SIZE ( \
                  STRARRAY_SIZE(BRR_BEARER) \
                + STRARRAY_SIZE(NET_CACHE) \
                + STRARRAY_SIZE(NET_PROXY_LIST) \
                + ( \
                      LEN_NET_PROXY_LIST * STRARRAY_SIZE(NET_PROXY_LIST__P_ADDR) \
                  ) \
                + STRARRAY_SIZE(NETIF_BRR_HANDLE) \
                + STRARRAY_SIZE(REPLAY_CACHE) \
                + STRARRAY_SIZE(REASSEMBLED_CACHE) \
                + STRARRAY_SIZE(LTRN_SAR_CTX) \
                + STRARRAY_SIZE(LPN_ELEMENT) \
                + ( \
                      LEN_LPN_ELEMENT * STRARRAY_SIZE(LPN_ELEMENT__SUBSCRIPTION_LIST) \
                    + LEN_LPN_ELEMENT * STRARRAY_SIZE(LPN_ELEMENT__0_QUEUE) \
                    + LEN_LPN_ELEMENT * STRARRAY_SIZE(LPN_ELEMENT__1_QUEUE) \
                  ) \
                + STRARRAY_SIZE(MS_ACCESS_ELEMENT_LIST) \
                + STRARRAY_SIZE(MS_ACCCES_MODEL_LIST) \
                + ( \
                      LEN_MS_ACCCES_MODEL_LIST * STRARRAY_SIZE(MS_ACCCES_MODEL_LIST_APP_KEY) \
                    + LEN_MS_ACCCES_MODEL_LIST * STRARRAY_SIZE(MS_ACCCES_MODEL_LIST_APP_SUBSCRIPTION) \
                  ) \
                + STRARRAY_SIZE(MS_SUBNET_TABLE) \
                + ( \
                      LEN_MS_SUBNET_TABLE * STRARRAY_SIZE(MS_SUBNET_TABLE__APPKEY) \
                    + LEN_MS_SUBNET_TABLE * STRARRAY_SIZE(MS_SUBNET_TABLE__FRIEND) \
                  ) \
                + STRARRAY_SIZE(MS_APPKEY_TABLE) \
                + STRARRAY_SIZE(MS_DEV_KEY_TABLE) \
                + STRARRAY_SIZE(MS_ELEMENT_ADDR_TABLE) \
                + STRARRAY_SIZE(MS_NON_VIRTUAL_ADDR_TABLE) \
                + STRARRAY_SIZE(MS_VIRTUAL_ADDR_TABLE) \
                + STRARRAY_SIZE(PROV_BRR) \
                + STRARRAY_SIZE(HEALTH_SERVER) \
            )

/*******************************************************************************
* Prototype declarations
*******************************************************************************/
void mesh_section_init(void);
void mesh_mempool_init(void);
void mesh_storage_init(void);

#endif /* _MESH_MEMPOOL_ */

