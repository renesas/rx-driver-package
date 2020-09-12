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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/*******************************************************************************
* File Name    : r_mesh_rx23w_config.h
* Description  : Configures the BLE Mesh Stack.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version  Description
*           30.09.2019 1.00     First Release.
*           29.11.2019 1.01     Add random-delay to advertising transmission.
*******************************************************************************/

#ifndef R_MESH_RX23W_CONFIG_H
#define R_MESH_RX23W_CONFIG_H

/*******************************************************************************
Configuration Options
********************************************************************************/
/*
 * the number of bearer interface for mesh network
 */
#define MESH_CFG_NUM_NETWORK_INTERFACES                 2
/*
 * the number of bearer interface for provisioning
 */
#define MESH_CFG_NUM_PROVISIONING_INTERFACES            2

/* ----------------------------------------------------------------------- */
/* =============================  Network  =============================== */
/* ----------------------------------------------------------------------- */
/*
 *  In a 'flooding' mesh implementation, one of the methods used to restrict
 *  unlimited flooding, is using message cache.
 *  This parameter specifies the size of the network message cache.
 *
 *  Minimum Value: 2
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_NET_CACHE_SIZE                         10

/*
 *  Maximum number of subnets the device can store information about.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_MAX_SUBNETS                            4

/*
 *  Maximum number of device keys the device can store information about.
 *  As a configuration client, there should be one additional space to
 *  contain device key of configuration server.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_MAX_DEV_KEYS                           4

/*
 *  Maximum number of addresses present in each proxy filter list.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_PROXY_FILTER_LIST_SIZE                 2

/*
 *  The distance between the network sequence numbers for every persistent
 *  storage write. If the device is powered cycled, it will resume transmission
 *  using the sequence number from start of next block.
 *
 *  Minimum Value: can be anything. A smaller value will reduce the flash lifetime.
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_NET_SEQ_NUMBER_BLOCK_SIZE              2048

/* ----------------------------------------------------------------------- */
/* =============================  Transport  ============================= */
/* ----------------------------------------------------------------------- */
/*
 *  This parameter specifies the maximum number of Low Power Nodes (LPNs)
 *  to which friendship can be established as a Friend.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_MAX_LPNS                               1

/*
 *  Replay Protection cache is required to protect against relay attacks.
 *  This parameter specifies the size of the Replay Protection cache.
 *
 *  Minimum Value: 2
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_REPLAY_CACHE_SIZE                      10

/*
 *  Reassembled SAR Rx cache is to avoid handling of segmented frames
 *  which are already received and acked by the local implementation.
 *  Saves the unnecessary effort of reassmbly and complex handling by
 *  the upper layers to handle reception of same payload again from
 *  the same source device.
 *
 *  Minimum Value: 2
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_REASSEMBLED_CACHE_SIZE                 8

/*
 *  Number of Segmentation and Reassembly contexts.
 *  Used during both reception and transmission and also for associated
 *  LPNs.
 *
 *  Minimum Value: 2
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_LTRN_SAR_CTX_MAX                       8

/*
 *  This parameter defines the maximum number of messages that the friend
 *  is capable to queue for a single Low Power Node
 *
 *  Minimum Value: 2
 *  Maximum Value: As required
 */
#define MESH_CFG_FRIEND_MESSAGEQUEUE_SIZE               15

/*
 *  This parameter defines the maximum number of subscription addresses
 *  that the friend is capable to store for a single Low Power Node
 *
 *  Minimum Value: 1
 *  Maximum Value: As required
 */
#define MESH_CFG_FRIEND_SUBSCRIPTION_LIST_SIZE          8

/* ----------------------------------------------------------------------- */
/* =============================  Access ================================= */
/* ----------------------------------------------------------------------- */
/*
 *  This parameter specifies the maximum number of elements.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_ACCESS_ELEMENT_COUNT                   4

/*
 *  This parameter specifies the maximum number of models.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_ACCESS_MODEL_COUNT                     20

/*
 *  Maximum number of Applications (keys) the device can store information about.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_MAX_APPS                               8

/*
 *  Maximum number of Virtual Addresses the device can store information about.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_MAX_VIRTUAL_ADDRS                      8

/*
 *  Maximum number of Non-Virtual Addresses the device can store information about.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_MAX_NON_VIRTUAL_ADDRS                  8

/* ----------------------------------------------------------------------- */
/* ==========================  Health Model ============================== */
/* ----------------------------------------------------------------------- */
/*
 *  This parameter specifies the maximum number of Health Servers
 *  to be supported.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_HEALTH_SERVER_MAX                      2

/* ----------------------------------------------------------------------- */
/* =============================  Misc. ================================== */
/* ----------------------------------------------------------------------- */
/*
 *  This parameter specifies Default Company ID.
 *
 *  Company ID
 *  Ref: https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers
 */
#define MESH_CFG_DEFAULT_COMPANY_ID                     0x0036

/*
 *  This parameter specifies a 16-bit vendor-assigned product identifier.
 */
#define MESH_CFG_DEFAULT_PID                            0x0001

/*
 *  This parameter specifies a 16-bit vendor-assigned product version identifier.
 */
#define MESH_CFG_DEFAULT_VID                            0x0100

/*
 *
 */
#define MESH_CFG_DATA_FLASH_BLOCK_ID                    (1)

/*
 *
 */
#define MESH_CFG_DATA_FLASH_BLOCK_NUM                   (5)


/* do not edit the following macro definitions */

#define BLE_CFG_MESH_DATA_DF_BLOCK_BYTES                (0x400)
#define BLE_CFG_MESH_DATA_DF_SIZE   (BLE_CFG_MESH_DATA_DF_BLOCK_BYTES * MESH_CFG_DATA_FLASH_BLOCK_NUM)

#define MESH_MS_CONFIG(config) \
    (config).config_MS_NUM_NETWORK_INTERFACES        = MESH_CFG_NUM_NETWORK_INTERFACES; \
    (config).config_MS_NUM_PROVISIONING_INTERFACES   = MESH_CFG_NUM_PROVISIONING_INTERFACES; \
    (config).config_MS_NET_CACHE_SIZE                = MESH_CFG_NET_CACHE_SIZE; \
    (config).config_MS_MAX_SUBNETS                   = MESH_CFG_MAX_SUBNETS; \
    (config).config_MS_MAX_DEV_KEYS                  = MESH_CFG_MAX_DEV_KEYS; \
    (config).config_MS_PROXY_FILTER_LIST_SIZE        = MESH_CFG_PROXY_FILTER_LIST_SIZE; \
    (config).config_MS_MAX_LPNS                      = MESH_CFG_MAX_LPNS; \
    (config).config_MS_REASSEMBLED_CACHE_SIZE        = MESH_CFG_REASSEMBLED_CACHE_SIZE; \
    (config).config_LTRN_SAR_CTX_MAX                 = MESH_CFG_LTRN_SAR_CTX_MAX; \
    (config).config_MS_FRIEND_MESSAGEQUEUE_SIZE      = MESH_CFG_FRIEND_MESSAGEQUEUE_SIZE; \
    (config).config_MS_FRIEND_SUBSCRIPTION_LIST_SIZE = MESH_CFG_FRIEND_SUBSCRIPTION_LIST_SIZE; \
    (config).config_MS_ACCESS_ELEMENT_COUNT          = MESH_CFG_ACCESS_ELEMENT_COUNT; \
    (config).config_MS_ACCESS_MODEL_COUNT            = MESH_CFG_ACCESS_MODEL_COUNT; \
    (config).config_MS_MAX_APPS                      = MESH_CFG_MAX_APPS; \
    (config).config_MS_MAX_VIRTUAL_ADDRS             = MESH_CFG_MAX_VIRTUAL_ADDRS; \
    (config).config_MS_MAX_NON_VIRTUAL_ADDRS         = MESH_CFG_MAX_NON_VIRTUAL_ADDRS; \
    (config).config_MS_NET_SEQ_NUMBER_BLOCK_SIZE     = MESH_CFG_NET_SEQ_NUMBER_BLOCK_SIZE; \
    (config).config_MS_HEALTH_SERVER_MAX             = MESH_CFG_HEALTH_SERVER_MAX; \
    (config).config_MS_REPLAY_CACHE_SIZE             = MESH_CFG_REPLAY_CACHE_SIZE; \
    (config).config_MS_DEFAULT_COMPANY_ID            = MESH_CFG_DEFAULT_COMPANY_ID; \
    (config).config_MS_DEFAULT_PID                   = MESH_CFG_DEFAULT_PID; \
    (config).config_MS_DEFAULT_VID                   = MESH_CFG_DEFAULT_VID;

#endif /* R_MESH_RX23W_CONFIG_H */

/** @} */

