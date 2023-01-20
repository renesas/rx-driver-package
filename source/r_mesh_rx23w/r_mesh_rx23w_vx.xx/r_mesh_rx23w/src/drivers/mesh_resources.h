/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019-2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

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
#define SIZEOF_UINT8                (1U)
#define SIZEOF_UINT16               (2U)
#define SIZEOF_UINT32               (4U)
#define ALIGN_UINT32(size)          ((((size) + 4 - 1) / 4) * 4)
#define BITARRAY_BLK                (32U)
#define BITARRAY_SIZE(bits)         (((bits) + (BITARRAY_BLK - 1)) / (BITARRAY_BLK))

/* Element Size */
#define SIZEOF_BRR_BEARER_DATA                          (160)
#define SIZEOF_NET_CACHE_ELEMENT_EX                     (16)
#define SIZEOF_NET_CACHE_ELEMENT_EX_CONFIGURABLE \
            (SIZEOF_UINT32 * (1 + BITARRAY_SIZE(MESH_CFG_NET_SEQNUM_CACHE_SIZE)))
#define SIZEOF_NET_TX_Q_ELEMENT                         (16)
#define SIZEOF_PROXY_FILTER_LIST_FIXED                  (8)
#define SIZEOF_PROXY_ADDR                               (2)
#define SIZEOF_PROXY_FILTER_LIST_CONFIGURABLE \
            (SIZEOF_PROXY_ADDR * MESH_CFG_PROXY_FILTER_LIST_SIZE)
#define SIZEOF_BRR_HANDLE                               (1)
#define SIZEOF_LTRN_REPLAY_CACHE_ELEMENT                (8)
#define SIZEOF_LTRN_REASSEMBLED_FRAME_INFO              (12)
#define SIZEOF_LTRN_SAR_CTX                             (56)
#define SIZEOF_TRN_LPN_ELEMENT_FIXED                    (60)
#define SIZEOF_MS_NET_ADDR                              (2)
#define SIZEOF_TRN_FRN_Q_ELEMENT                        (36)
#define SIZEOF_TRN_LPN_ELEMENT_CONFIGURABLE \
            ((SIZEOF_MS_NET_ADDR * MESH_CFG_FRIEND_SUBSCRIPTION_LIST_SIZE) + \
            (2 * SIZEOF_TRN_FRN_Q_ELEMENT * MESH_CFG_FRIEND_MESSAGEQUEUE_SIZE))
#define SIZEOF_MS_ACCESS_ELEMENT_TYPE                   (6)
#define SIZEOF_MS_ACCESS_MODEL_TYPE_FIXED               (68)
#define SIZEOF_MS_ACCESS_MODEL_TYPE_CONFIGURABLE \
            (SIZEOF_UINT32 * (BITARRAY_SIZE(MESH_CFG_MAX_APPS) + BITARRAY_SIZE(MESH_CFG_MAX_VIRTUAL_ADDRS + MESH_CFG_MAX_NON_VIRTUAL_ADDRS)))
#define SIZEOF_MS_NETKEY_ENTRY_FIXED                    (120)
#define SIZEOF_MS_NETKEY_ENTRY_CONFIGURABLE \
            (SIZEOF_UINT32 * (BITARRAY_SIZE(MESH_CFG_MAX_APPS) + BITARRAY_SIZE(MESH_CFG_MAX_LPNS)))
#define SIZEOF_MS_APPKEY_ENTRY                          (40)
#define SIZEOF_MS_DEV_KEY_ENTRY                         (20)
#define SIZEOF_MS_ELEMENT_ADDR_ENTRY                    (4)
#define SIZEOF_MS_NON_VIRTUAL_ADDR_ENTRY                (4)
#define SIZEOF_MS_VIRTUAL_ADDR_ENTRY                    (20)
#define SIZEOF_TIMER_AL_ELEMENT                         (32)
#define SIZEOF_PROV_BRR_INFO                            (1)
#define SIZEOF_MS_ACCESS_STATE_TRANSITION_TYPE          (20)
#define SIZEOF_MS_HEALTH_SERVER_T                       (96)
#define SIZEOF_MS_LIGHT_LC_SERVER_CONTEXT               (52)

/* Array Size */
#define SIZEOF_MEM_BRR_BEARER \
            (ALIGN_UINT32(SIZEOF_BRR_BEARER_DATA) * MESH_CFG_NUM_NETWORK_INTERFACES)
#define SIZEOF_MEM_NET_CACHE_EX \
            (ALIGN_UINT32(SIZEOF_NET_CACHE_ELEMENT_EX) * MESH_CFG_NET_CACHE_SIZE)
#define SIZEOF_MEM_NET_CACHE_EX_CONFIGURABLE \
            (ALIGN_UINT32(SIZEOF_NET_CACHE_ELEMENT_EX_CONFIGURABLE) * MESH_CFG_NET_CACHE_SIZE)
#define SIZEOF_MEM_NET_TX_QUEUE \
            (ALIGN_UINT32(SIZEOF_NET_TX_Q_ELEMENT) * MESH_CFG_NET_TX_QUEUE_SIZE)
#define SIZEOF_MEM_NET_PROXY_LIST_FIXED \
            (ALIGN_UINT32(SIZEOF_PROXY_FILTER_LIST_FIXED) * (MESH_CFG_NUM_NETWORK_INTERFACES - 1))
#define SIZEOF_MEM_NET_PROXY_LIST_CONFIGURABLE \
            (ALIGN_UINT32(SIZEOF_PROXY_FILTER_LIST_CONFIGURABLE) * (MESH_CFG_NUM_NETWORK_INTERFACES - 1))
#define SIZEOF_MEM_NETIF_BRR_HANDLE \
            (ALIGN_UINT32(SIZEOF_BRR_HANDLE) * MESH_CFG_NUM_NETWORK_INTERFACES)
#define SIZEOF_MEM_REPLAY_CACHE \
            (ALIGN_UINT32(SIZEOF_LTRN_REPLAY_CACHE_ELEMENT) * MESH_CFG_REPLAY_CACHE_SIZE)
#define SIZEOF_MEM_REASSEMBLED_CACHE \
            (ALIGN_UINT32(SIZEOF_LTRN_REASSEMBLED_FRAME_INFO) * MESH_CFG_REASSEMBLED_CACHE_SIZE)
#define SIZEOF_MEM_LTRN_SAR_CTX \
            (ALIGN_UINT32(SIZEOF_LTRN_SAR_CTX) * MESH_CFG_LTRN_SAR_CTX_MAX)
#define SIZEOF_MEM_LPN_ELEMENT_FIXED \
            (ALIGN_UINT32(SIZEOF_TRN_LPN_ELEMENT_FIXED) * MESH_CFG_MAX_LPNS)
#define SIZEOF_MEM_LPN_ELEMENT_CONFIGURABLE \
            (ALIGN_UINT32(SIZEOF_TRN_LPN_ELEMENT_CONFIGURABLE) * MESH_CFG_MAX_LPNS)
#define SIZEOF_MEM_MS_ACCESS_ELEMENT_LIST \
            (ALIGN_UINT32(SIZEOF_MS_ACCESS_ELEMENT_TYPE) * MESH_CFG_ACCESS_ELEMENT_COUNT)
#define SIZEOF_MEM_MS_ACCESS_MODEL_LIST_FIXED \
            (ALIGN_UINT32(SIZEOF_MS_ACCESS_MODEL_TYPE_FIXED) * MESH_CFG_ACCESS_MODEL_COUNT)
#define SIZEOF_MEM_MS_ACCESS_MODEL_LIST_CONFIGURABLE \
            (ALIGN_UINT32(SIZEOF_MS_ACCESS_MODEL_TYPE_CONFIGURABLE) * MESH_CFG_ACCESS_MODEL_COUNT)
#define SIZEOF_MEM_MS_SUBNET_TABLE_FIXED \
            (ALIGN_UINT32(SIZEOF_MS_NETKEY_ENTRY_FIXED) * (MESH_CFG_MAX_SUBNETS + MESH_CFG_MAX_LPNS))
#define SIZEOF_MEM_MS_SUBNET_TABLE_CONFIGURABLE \
            (ALIGN_UINT32(SIZEOF_MS_NETKEY_ENTRY_CONFIGURABLE) * (MESH_CFG_MAX_SUBNETS + MESH_CFG_MAX_LPNS))
#define SIZEOF_MEM_APPKEY_TABLE \
            (ALIGN_UINT32(SIZEOF_MS_APPKEY_ENTRY) * MESH_CFG_MAX_APPS)
#define SIZEOF_MEM_MS_DEV_KEY_TABLE \
            (ALIGN_UINT32(SIZEOF_MS_DEV_KEY_ENTRY) * MESH_CFG_MAX_DEV_KEYS)
#define SIZEOF_MEM_MS_ELEMENT_ADDR_TABLE \
            (ALIGN_UINT32(SIZEOF_MS_ELEMENT_ADDR_ENTRY) * (1 + MESH_CFG_MAX_LPNS))
#define SIZEOF_MEM_MS_NON_VIRTUAL_ADDR_TABLE \
            (ALIGN_UINT32(SIZEOF_MS_NON_VIRTUAL_ADDR_ENTRY) * MESH_CFG_MAX_NON_VIRTUAL_ADDRS)
#define SIZEOF_MEM_MS_VIRTUAL_ADDR_TABLE \
            (ALIGN_UINT32(SIZEOF_MS_VIRTUAL_ADDR_ENTRY) * MESH_CFG_MAX_VIRTUAL_ADDRS)
#define SIZEOF_MEM_MS_PERIODIC_STEP_TIMERS \
            (ALIGN_UINT32(SIZEOF_TIMER_AL_ELEMENT) * MESH_CFG_MAX_NUM_PERIODIC_STEP_TIMERS)
#define SIZEOF_MEM_PROV_BRR \
            (ALIGN_UINT32(SIZEOF_PROV_BRR_INFO) * MESH_CFG_NUM_PROVISIONING_INTERFACES)
#define SIZEOF_MEM_MS_TRANSITION_TIMERS \
            (ALIGN_UINT32(SIZEOF_MS_ACCESS_STATE_TRANSITION_TYPE) * MESH_CFG_MAX_NUM_TRANSITION_TIMERS)
#define SIZEOF_MEM_HEALTH_SERVER \
            (ALIGN_UINT32(SIZEOF_MS_HEALTH_SERVER_T) * MESH_CFG_HEALTH_SERVER_MAX)
#define SIZEOF_MEM_LIGHT_LC_SERVER \
            (ALIGN_UINT32(SIZEOF_MS_LIGHT_LC_SERVER_CONTEXT) * MESH_CFG_LIGHT_LC_SERVER_MAX)

/* Memory Pool Size */
#define MESH_MEMPOOL_SIZE \
            ( \
              SIZEOF_MEM_BRR_BEARER \
            + SIZEOF_MEM_NET_CACHE_EX \
            + SIZEOF_MEM_NET_CACHE_EX_CONFIGURABLE \
            + SIZEOF_MEM_NET_TX_QUEUE \
            + SIZEOF_MEM_NET_PROXY_LIST_FIXED \
            + SIZEOF_MEM_NET_PROXY_LIST_CONFIGURABLE \
            + SIZEOF_MEM_NETIF_BRR_HANDLE \
            + SIZEOF_MEM_REPLAY_CACHE \
            + SIZEOF_MEM_REASSEMBLED_CACHE \
            + SIZEOF_MEM_LTRN_SAR_CTX \
            + SIZEOF_MEM_LPN_ELEMENT_FIXED \
            + SIZEOF_MEM_LPN_ELEMENT_CONFIGURABLE \
            + SIZEOF_MEM_MS_ACCESS_ELEMENT_LIST \
            + SIZEOF_MEM_MS_ACCESS_MODEL_LIST_FIXED \
            + SIZEOF_MEM_MS_ACCESS_MODEL_LIST_CONFIGURABLE \
            + SIZEOF_MEM_MS_SUBNET_TABLE_FIXED \
            + SIZEOF_MEM_MS_SUBNET_TABLE_CONFIGURABLE \
            + SIZEOF_MEM_APPKEY_TABLE \
            + SIZEOF_MEM_MS_DEV_KEY_TABLE \
            + SIZEOF_MEM_MS_ELEMENT_ADDR_TABLE \
            + SIZEOF_MEM_MS_NON_VIRTUAL_ADDR_TABLE \
            + SIZEOF_MEM_MS_VIRTUAL_ADDR_TABLE \
            + SIZEOF_MEM_MS_PERIODIC_STEP_TIMERS \
            + SIZEOF_MEM_PROV_BRR \
            + SIZEOF_MEM_MS_TRANSITION_TIMERS \
            + SIZEOF_MEM_HEALTH_SERVER \
            + SIZEOF_MEM_LIGHT_LC_SERVER \
            )

/* Storage Record Composition Size */
#define _SIZEOF_MS_ACCESS_ELEMENT_TYPE                  (6)
#define _SIZEOF_MS_ACCESS_COMPOSITION_DATA_PAGE_0_HDR   (10)
#define _SIZEOF_MS_ACCESS_MODEL_TYPE_FIXED              (60)
#define _SIZEOF_MS_ACCESS_MODEL_TYPE_CONFIGURABLE \
            ( \
                  (SIZEOF_UINT32 * (BITARRAY_SIZE(MESH_CFG_MAX_APPS))) \
                + (SIZEOF_UINT32 * (BITARRAY_SIZE(MESH_CFG_MAX_VIRTUAL_ADDRS + MESH_CFG_MAX_NON_VIRTUAL_ADDRS))) \
            )
#define _SIZEOF_MS_IV_INDEX                             (12)
#define _SIZEOF_VAR_MS_NETKEY_COUNT                     (2)
#define _SIZEOF_MS_NETKEY_ENTRY_FIXED                   (110)
#define _SIZEOF_MS_NETKEY_ENTRY_CONFIGURABLE \
            ( \
                  (SIZEOF_UINT32 * (BITARRAY_SIZE(MESH_CFG_MAX_APPS))) \
                + (SIZEOF_UINT32 * (BITARRAY_SIZE(MESH_CFG_MAX_LPNS))) \
            )
#define _SIZEOF_MS_DEV_KEY_ENTRY                        (20)
#define _SIZEOF_MS_APPKEY_ENTRY                         (40)
#define _SIZEOF_MS_ELEMENT_ADDR_ENTRY                   (4)
#define _SIZEOF_MS_VIRTUAL_ADDR_ENTRY                   (20)
#define _SIZEOF_MS_NON_VIRTUAL_ADDR_ENTRY               (4)
#define _SIZEOF_VAR_MS_TX_STATE                         (4)
#define _SIZEOF_VAR_MS_FEATURES                         (1)

/* Storage Signature Size */
#define MESH_STORAGE_SIGNATURE_SIZE                     (10)

/* Storage Record Size */
#define REC_ELEMENTS_SIZE \
            (SIZEOF_UINT16 + (_SIZEOF_MS_ACCESS_ELEMENT_TYPE * MESH_CFG_ACCESS_ELEMENT_COUNT) + _SIZEOF_MS_ACCESS_COMPOSITION_DATA_PAGE_0_HDR)
#define REC_MODELS_SIZE \
            (SIZEOF_UINT16 + ((_SIZEOF_MS_ACCESS_MODEL_TYPE_FIXED + _SIZEOF_MS_ACCESS_MODEL_TYPE_CONFIGURABLE) * MESH_CFG_ACCESS_MODEL_COUNT))
#define REC_SUBNETS_SIZE \
            (SIZEOF_UINT16 + _SIZEOF_MS_IV_INDEX + _SIZEOF_VAR_MS_NETKEY_COUNT + \
            ((_SIZEOF_MS_NETKEY_ENTRY_FIXED + _SIZEOF_MS_NETKEY_ENTRY_CONFIGURABLE) * (MESH_CFG_MAX_SUBNETS + MESH_CFG_MAX_LPNS)))
#define REC_DEV_KEYS_SIZE \
            (SIZEOF_UINT16 + SIZEOF_UINT16 + (_SIZEOF_MS_DEV_KEY_ENTRY * MESH_CFG_MAX_DEV_KEYS))
#define REC_APP_KEYS_SIZE \
            (SIZEOF_UINT16 + (_SIZEOF_MS_APPKEY_ENTRY * MESH_CFG_MAX_APPS))
#define REC_ELEMENT_ADDRS_SIZE \
            (SIZEOF_UINT16 + (_SIZEOF_MS_ELEMENT_ADDR_ENTRY * (1 + MESH_CFG_MAX_LPNS)))
#define REC_VIRTUAL_ADDRS_SIZE \
            (SIZEOF_UINT16 + (_SIZEOF_MS_VIRTUAL_ADDR_ENTRY * MESH_CFG_MAX_VIRTUAL_ADDRS))
#define REC_NON_VIRTUAL_ADDRS_SIZE \
            (SIZEOF_UINT16 + (_SIZEOF_MS_NON_VIRTUAL_ADDR_ENTRY * MESH_CFG_MAX_NON_VIRTUAL_ADDRS))
#define REC_SEQ_NUMBER_SIZE \
            (SIZEOF_UINT32)
#define REC_TX_STATES_FEATURES_SIZE \
            (SIZEOF_UINT16 + _SIZEOF_VAR_MS_TX_STATE + _SIZEOF_VAR_MS_FEATURES)

/* Storage Size */
#define MESH_STORAGE_SIZE \
            ( MESH_STORAGE_SIGNATURE_SIZE \
            + REC_ELEMENTS_SIZE \
            + REC_MODELS_SIZE \
            + REC_SUBNETS_SIZE \
            + REC_DEV_KEYS_SIZE \
            + REC_APP_KEYS_SIZE \
            + REC_ELEMENT_ADDRS_SIZE \
            + REC_VIRTUAL_ADDRS_SIZE \
            + REC_NON_VIRTUAL_ADDRS_SIZE \
            + REC_SEQ_NUMBER_SIZE \
            + REC_TX_STATES_FEATURES_SIZE )

/*******************************************************************************
* Prototype declarations
*******************************************************************************/
void mesh_section_init(void);
void mesh_mempool_init(void);
void mesh_storage_init(void);

#endif /* _MESH_MEMPOOL_ */

