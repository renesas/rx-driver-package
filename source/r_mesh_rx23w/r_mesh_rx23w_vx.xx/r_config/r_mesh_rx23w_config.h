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

/*******************************************************************************
* File Name    : r_mesh_rx23w_config.h
* Description  : Configures the BLE Mesh Stack.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version  Description
*           30.09.2019  1.00    First Release.
*           29.11.2019  1.01    No updated in Mesh Configuration.
*           30.09.2020  1.10    Added the following Mesh Configuration option(s):
*                               - MESH_CFG_NET_SEQNUM_CACHE_SIZE
*           30.09.2021  1.20    Added the following Mesh Configuration option(s):
*                               - MESH_CFG_UNPROV_DEVICE_BEACON_TIMEOUT
*                               - MESH_CFG_NET_TX_COUNT
*                               - MESH_CFG_NET_TX_INTERVAL_STEPS
*                               - MESH_CFG_NET_RELAY_TX_COUNT
*                               - MESH_CFG_NET_RELAY_TX_INTERVAL_STEPS
*                               - MESH_CFG_PROXY_SUBNET_NETID_ADV_TIMEOUT
*                               - MESH_CFG_PROXY_SUBNET_NODEID_ADV_TIMEOUT
*                               - MESH_CFG_PROXY_NODEID_ADV_TIMEOUT
*                               - MESH_CFG_NET_TX_QUEUE_SIZE
*                               - MESH_CFG_FRND_POLL_RETRY_COUNT
*                               - MESH_CFG_LTRN_RTX_TIMEOUT
*                               - MESH_CFG_LTRN_RTX_COUNT
*                               - MESH_CFG_LTRN_ACK_TIMEOUT
*                               - MESH_CFG_LTRN_INCOMPLETE_TIMEOUT
*                               - MESH_CFG_FRND_RECEIVE_WINDOW
*                               - MESH_CFG_LPN_CLEAR_RETRY_TIMEOUT_INITIAL
*                               - MESH_CFG_TRN_FRNDREQ_RETRY_TIMEOUT
*                               - MESH_CFG_MAX_NUM_TRANSITION_TIMERS
*                               - MESH_CFG_MAX_NUM_PERIODIC_STEP_TIMERS
*                               - MESH_CFG_CONFIG_SERVER_SNB_TIMEOUT
*           23.12.2022  1.30    Added the following Mesh Configuration option(s):
*                               - MESH_CFG_LPN_CLEAR_RETRY_COUNT
*                               - MESH_CFG_LIGHT_LC_SERVER_MAX
*
* For details, refer to "Program Updates (MESH FIT Module)" in the application note of 
* RX23W Group Bluetooth Mesh FIT Module (R01AN4930).
*******************************************************************************/

#ifndef R_MESH_RX23W_CONFIG_H
#define R_MESH_RX23W_CONFIG_H

/*******************************************************************************
Configuration Options
********************************************************************************/
/* ----------------------------------------------------------------------- */
/* =============================  Bearer  ================================ */
/* ----------------------------------------------------------------------- */
/*
 *  The number of bearer interface for mesh network
 *  Mesh Stack is supposed to add ADV bearer as a first bearer, and then add GATT bearers
 *  as subsequent bearers. When this configuration is set to 1, only ADV bearer can be used.
 *
 *  Minimum Value: 1
 *  Maximum Value: (1 + BLE_CFG_RF_CONN_MAX), BLE_CFG_RF_CONN_MAX macro is defined by BLE FIT Module.
 */
#define MESH_CFG_NUM_NETWORK_INTERFACES                     2

/*
 *  The number of bearer interface for provisioning
 *  When this configuration is set to 1, only PB-ADV bearer can be used.
 *  When this configuration is set to 2, PB-ADV bearer and one PB-GATT bearer can be used.
 *
 *  Minimum Value: 1
 *  Maximum Value: 2
 */
#define MESH_CFG_NUM_PROVISIONING_INTERFACES                2

/* ----------------------------------------------------------------------- */
/* ==========================  Provisioning  ============================= */
/* ----------------------------------------------------------------------- */
/*
 *  The interval time in millisecond for interleaved Unprovisioned Device beacons.
 *
 *  Minimum Value: 20
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_UNPROV_DEVICE_BEACON_TIMEOUT               200 /* Milliseconds */

/* ----------------------------------------------------------------------- */
/* =============================  Network  =============================== */
/* ----------------------------------------------------------------------- */
/*
 *  In a 'flooding' mesh implementation, one of the methods used to restrict
 *  unlimited flooding, is using message cache.
 *  This parameter specifies the size of the Network message cache.
 *
 *  Minimum Value: 2
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_NET_CACHE_SIZE                             10

/*
 *  In a 'flooding' mesh implementation, one of the methods used to restrict
 *  unlimited flooding, is using message cache.
 *  This parameter specifies the size sequence number cache per node.
 *
 *  Minimum Value: 32
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_NET_SEQNUM_CACHE_SIZE                      32

/*
 *  Maximum number of subnets the device can store information about.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_MAX_SUBNETS                                4

/*
 *  Maximum number of device keys the device can store information about.
 *  As a configuration client, there should be one additional space to
 *  contain device key of configuration server.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_MAX_DEV_KEYS                               4

/*
 *  Maximum number of addresses present in each proxy filter list.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_PROXY_FILTER_LIST_SIZE                     2

/*
 *  The distance between the network sequence numbers, for every persistent
 *  storage write. If the device is powered cycled, it will resume transmission
 *  using the sequence number from start of next block.
 *
 *  Minimum Value: can be anything. A smaller value will reduce the flash lifetime.
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_NET_SEQ_NUMBER_BLOCK_SIZE                  2048

/*
 *  Default Network Transmit Count State value for network packets.
 *
 *  Minimum Value: 0
 *  Maximum Value: 7
 */
#define MESH_CFG_NET_TX_COUNT                               1

/*
 *  Default Network Transmit Interval Steps State value for network packets.
 *
 *  Minimum Value: 0
 *  Maximum Value: 31
 */
#define MESH_CFG_NET_TX_INTERVAL_STEPS                      4

/*
 *  Default Relay Retransmit Count State value for relayed packets.
 *
 *  Minimum Value: 0
 *  Maximum Value: 7
 */
#define MESH_CFG_NET_RELAY_TX_COUNT                         0

/*
 *  Default Relay Retransmit Interval Steps State value for relayed packets.
 *
 *  Minimum Value: 0
 *  Maximum Value: 31
 */
#define MESH_CFG_NET_RELAY_TX_INTERVAL_STEPS                9

/*
 *  The interval time in millisecond for proxy advertisement with Network ID for
 *  each subnet.
 *
 *  Minimum Value: 20, Larger value will have lesser timeout load.
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_PROXY_SUBNET_NETID_ADV_TIMEOUT             300 /* Milliseconds */

/*
 *  The interval time in millisecond for proxy advertisement with Node Identity for
 *  each subnet.
 *
 *  Minimum Value: 20, Larger value will have lesser timeout load.
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_PROXY_SUBNET_NODEID_ADV_TIMEOUT            300 /* Milliseconds */

/*
 *  The time period in seconds for proxy advertisement with Node Identity.
 *  Default 60 Seconds as recommended in specification.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_PROXY_NODEID_ADV_TIMEOUT                   60 /* Seconds */

/*
 *  This parameter defines the maximum number of messages that can be queued
 *  for transmission in the Network Layer.
 *
 *  Minimum Value: 2
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_NET_TX_QUEUE_SIZE                          64

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
#define MESH_CFG_MAX_LPNS                                   1

/*
 *  Replay Protection cache is required to protect against relay attacks.
 *  This parameter specifies the size of the Replay Protection cache.
 *
 *  Minimum Value: 2
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_REPLAY_CACHE_SIZE                          10

/*
 *  Reassembled SAR Rx cache is to avoid handling of segmented frames
 *  which are already received and acknowledged by the local implementation.
 *  Saves the unnecessary effort of reassembly and complex handling by
 *  the upper layers to handle reception of same payload again from
 *  the same source device.
 *
 *  Minimum Value: 2
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_REASSEMBLED_CACHE_SIZE                     8

/*
 *  The number of times to retry the FriendPoll message when
 *  the FriendUpdate message is not received for the first attempt
 *  of FriendPoll in the current sequence. When this count expires,
 *  the friendship will be terminated with the Friend node.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_FRND_POLL_RETRY_COUNT                      10

/*
 *  The number of Segmentation and Reassembly contexts.
 *  Used during both reception and transmission and also for associated
 *  LPNs.
 *
 *  Minimum Value: 2
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_LTRN_SAR_CTX_MAX                           8

/*
 *  The timeout in millisecond for Segment Transmission Timer
 *
 *  Minimum Value: 200 Milliseconds
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_LTRN_RTX_TIMEOUT                           300 /* Milliseconds */

/*
 *  The number of times to transmit a Segmented Message
 *
 *  Minimum Value: 2
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_LTRN_RTX_COUNT                             2

/*
 *  The timeout in millisecond for Acknowledgement Timer
 *
 *  Minimum Value: 200 Milliseconds
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_LTRN_ACK_TIMEOUT                           200 /* Milliseconds */

/*
 *  The timeout in millisecond for Incomplete Timer.
 *
 *  Minimum Value: 10 Seconds
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_LTRN_INCOMPLETE_TIMEOUT                    20 /* Seconds */

/*
 *  Receive window is the time in millisecond for which the Friend will be transmitting the
 *  response to any request from the LPN after the Receive delay time of
 *  getting any request
 *
 *  Minimum Value: 100 Milliseconds
 *  Maximum Value: As required
 */
#define MESH_CFG_FRND_RECEIVE_WINDOW                        100 /* Milliseconds */

/*
 *  This parameter defines the maximum number of messages that the friend
 *  is capable to queue for a single Low Power Node
 *
 *  Minimum Value: 2
 *  Maximum Value: As required
 */
#define MESH_CFG_FRIEND_MESSAGEQUEUE_SIZE                   15

/*
 *  This parameter defines the maximum number of subscription addresses
 *  that the friend is capable to store for a single Low Power Node
 *
 *  Minimum Value: 1
 *  Maximum Value: As required
 */
#define MESH_CFG_FRIEND_SUBSCRIPTION_LIST_SIZE              8

/*
 *  This parameter defines the initial timeout in milliseconds
 *  to be used to track the Friend Clear Confirm message after
 *  sending of a Friend Clear message.
 *
 *  When the Friend Clear message is originating from an LPN,
 *  this parameter is used to calculate the total time period
 *  for which the LPN waits for the reception of Friend Clear
 *  Confirmation from the Friend Node. The total time period
 *  for which an LPN waits for Friend clear Confirmation is
 *  product of this parameter and
 *  \ref MESH_CFG_LPN_CLEAR_RETRY_COUNT in milliseconds. After each
 *  individual timeout LPN retires Friend Clear message.
 *
 *  When the Friend Clear message is originating from an Friend,
 *  this parameter is used as the initial timeout value. After
 *  the first timeout the Friend doubles the timeout and retries
 *  Friend Clear message.
 *
 *  Minimum Value: 1000 Milliseconds
 *  Maximum Value: As required
 */
#define MESH_CFG_LPN_CLEAR_RETRY_TIMEOUT_INITIAL            1000 /* Milliseconds */

/*
 *  This parameter defines the total number of times an LPN
 *  sends Friend clear message if it has not received Friend
 *  clear Confirmation from a Friend Node.
 *  The total time period for which an LPN waits for Friend
 *  clear Confirmation is product of this parameter and
 *  \ref MESH_CFG_LPN_CLEAR_RETRY_TIMEOUT_INITIAL in milliseconds.
 *
 *  Minimum Value: 1
 *  Maximum Value: As required
 */
#define MESH_CFG_LPN_CLEAR_RETRY_COUNT                       5

/*
 *  This parameter defines the interval time in milliseconds
 *  to be used to retry friend request attempts in case an
 *  offer is not received.
 *
 *  Minimum Value: 1100 Milliseconds
 *  Maximum Value: As required
 */
#define MESH_CFG_TRN_FRNDREQ_RETRY_TIMEOUT                  1200 /* Milliseconds */

/* ----------------------------------------------------------------------- */
/* =============================  Access ================================= */
/* ----------------------------------------------------------------------- */
/*
 *  This parameter specifies the maximum number of elements.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_ACCESS_ELEMENT_COUNT                       4

/*
 *  This parameter specifies the maximum number of models.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_ACCESS_MODEL_COUNT                         20

/*
 *  Maximum number of Applications (keys) the device can store information about.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_MAX_APPS                                   8

/*
 *  Maximum number of Virtual Addresses the device can store information about.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_MAX_VIRTUAL_ADDRS                          8

/*
 *  Maximum number of Non-Virtual Addresses the device can store information about.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_MAX_NON_VIRTUAL_ADDRS                      8

/*
 *  Maximum number of Transition Timers.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_MAX_NUM_TRANSITION_TIMERS                  5

/*
 *  Maximum number of Periodic Step Timers.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_MAX_NUM_PERIODIC_STEP_TIMERS               5

/* ----------------------------------------------------------------------- */
/* ==========================  Config Model ============================== */
/* ----------------------------------------------------------------------- */
/*
 *  Default Configuration Server Secure Network Beacon Interval value in Seconds.
 *
 *  Minimum Value: 10 Seconds
 *  Maximum Value: 600 Seconds
 */
#define MESH_CFG_CONFIG_SERVER_SNB_TIMEOUT                  10 /* Seconds */

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
#define MESH_CFG_HEALTH_SERVER_MAX                          2

/* ----------------------------------------------------------------------- */
/* =====================  Light LC Server Model ========================== */
/* ----------------------------------------------------------------------- */
/*
 *  This parameter specifies the maximum number of Light LC Servers
 *  to be supported.
 *
 *  Minimum Value: 1
 *  Maximum Value: can be anything.
 */
#define MESH_CFG_LIGHT_LC_SERVER_MAX                        1

/* ----------------------------------------------------------------------- */
/* =============================  Misc. ================================== */
/* ----------------------------------------------------------------------- */
/*
 *  This parameter specifies Default Company ID.
 *
 *  Company ID registered with Bluetooth SIG
 *  Refer to https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers
 */
#define MESH_CFG_DEFAULT_COMPANY_ID                         0x0036

/*
 *  This parameter specifies a 16-bit vendor-assigned product identifier.
 */
#define MESH_CFG_DEFAULT_PID                                0x0001

/*
 *  This parameter specifies a 16-bit vendor-assigned product version identifier.
 */
#define MESH_CFG_DEFAULT_VID                                0x0100

/*
 *  Data Flash Block ID of the first of Data Flash blocks used for storing mesh information
 *
 *  Minimum Value: 0
 *  Maximum Value: 7
 */
#define MESH_CFG_DATA_FLASH_BLOCK_ID                        (1)

/*
 *  The number of Data Flash Blocks used for storing mesh information
 *
 *  Minimum Value: 1
 *  Maximum Value: 8
 */
#define MESH_CFG_DATA_FLASH_BLOCK_NUM                       (5)

/* do not edit the following macro definitions */

#define BLE_CFG_MESH_DATA_DF_BLOCK_BYTES                    (0x400)
#define BLE_CFG_MESH_DATA_DF_SIZE   (BLE_CFG_MESH_DATA_DF_BLOCK_BYTES * MESH_CFG_DATA_FLASH_BLOCK_NUM)

#define MESH_MS_CONFIG(config) \
    (config).config_MS_NUM_NETWORK_INTERFACES        = MESH_CFG_NUM_NETWORK_INTERFACES; \
    (config).config_MS_NUM_PROVISIONING_INTERFACES   = MESH_CFG_NUM_PROVISIONING_INTERFACES; \
    (config).config_MS_NET_CACHE_SIZE                = MESH_CFG_NET_CACHE_SIZE; \
    (config).config_MS_NET_SEQNUM_CACHE_SIZE         = MESH_CFG_NET_SEQNUM_CACHE_SIZE; \
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
    (config).config_MS_NET_TX_COUNT                  = MESH_CFG_NET_TX_COUNT; \
    (config).config_MS_NET_TX_INTERVAL_STEPS         = MESH_CFG_NET_TX_INTERVAL_STEPS; \
    (config).config_MS_NET_RELAY_TX_COUNT            = MESH_CFG_NET_RELAY_TX_COUNT; \
    (config).config_MS_NET_RELAY_TX_INTERVAL_STEPS   = MESH_CFG_NET_RELAY_TX_INTERVAL_STEPS; \
    (config).config_MS_CONFIG_SERVER_SNB_TIMEOUT     = MESH_CFG_CONFIG_SERVER_SNB_TIMEOUT; \
    (config).config_MS_PROXY_SUBNET_NETID_ADV_TIMEOUT = MESH_CFG_PROXY_SUBNET_NETID_ADV_TIMEOUT; \
    (config).config_MS_PROXY_SUBNET_NODEID_ADV_TIMEOUT = MESH_CFG_PROXY_SUBNET_NODEID_ADV_TIMEOUT; \
    (config).config_MS_PROXY_NODEID_ADV_TIMEOUT      = MESH_CFG_PROXY_NODEID_ADV_TIMEOUT; \
    (config).config_MS_FRND_POLL_RETRY_COUNT         = MESH_CFG_FRND_POLL_RETRY_COUNT; \
    (config).config_MS_LTRN_RTX_TIMEOUT              = MESH_CFG_LTRN_RTX_TIMEOUT; \
    (config).config_MS_LTRN_RTX_COUNT                = MESH_CFG_LTRN_RTX_COUNT; \
    (config).config_MS_LTRN_ACK_TIMEOUT              = MESH_CFG_LTRN_ACK_TIMEOUT; \
    (config).config_MS_LTRN_INCOMPLETE_TIMEOUT       = MESH_CFG_LTRN_INCOMPLETE_TIMEOUT; \
    (config).config_MS_FRND_RECEIVE_WINDOW           = MESH_CFG_FRND_RECEIVE_WINDOW; \
    (config).config_MS_LPN_CLEAR_RETRY_TIMEOUT_INITIAL = MESH_CFG_LPN_CLEAR_RETRY_TIMEOUT_INITIAL; \
    (config).config_MS_LPN_CLEAR_RETRY_COUNT         = MESH_CFG_LPN_CLEAR_RETRY_COUNT; \
    (config).config_MS_TRN_FRNDREQ_RETRY_TIMEOUT     = MESH_CFG_TRN_FRNDREQ_RETRY_TIMEOUT; \
    (config).config_MS_UNPROV_DEVICE_BEACON_TIMEOUT  = MESH_CFG_UNPROV_DEVICE_BEACON_TIMEOUT; \
    (config).config_MS_NET_TX_QUEUE_SIZE             = MESH_CFG_NET_TX_QUEUE_SIZE; \
    (config).config_MS_MAX_NUM_TRANSITION_TIMERS     = MESH_CFG_MAX_NUM_TRANSITION_TIMERS; \
    (config).config_MS_MAX_NUM_PERIODIC_STEP_TIMERS  = MESH_CFG_MAX_NUM_PERIODIC_STEP_TIMERS; \
    (config).config_MS_HEALTH_SERVER_MAX             = MESH_CFG_HEALTH_SERVER_MAX; \
    (config).config_MS_LIGHT_LC_SERVER_MAX           = MESH_CFG_LIGHT_LC_SERVER_MAX; \
    (config).config_MS_REPLAY_CACHE_SIZE             = MESH_CFG_REPLAY_CACHE_SIZE; \
    (config).config_MS_DEFAULT_COMPANY_ID            = MESH_CFG_DEFAULT_COMPANY_ID; \
    (config).config_MS_DEFAULT_PID                   = MESH_CFG_DEFAULT_PID; \
    (config).config_MS_DEFAULT_VID                   = MESH_CFG_DEFAULT_VID;

#endif /* R_MESH_RX23W_CONFIG_H */

/** @} */

