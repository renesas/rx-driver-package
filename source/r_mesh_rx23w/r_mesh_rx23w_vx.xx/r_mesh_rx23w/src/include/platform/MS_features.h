
/**
 *  \file MS_features.h
 *
 *  \brief EtherMind Compilation Switches Configuration File.
 *
 *  This file lists all the Compilation Flags available in various
 *  EtherMind Mesh Stack modules.
 */

/*
 *  Copyright (C) 2016. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_MS_FEATURES_
#define _H_MS_FEATURES_

/* ----------------------------------------------------------------------- */
/* ==== Mesh v1.0 Feature Flags ========================================== */
/* ----------------------------------------------------------------------- */
/*
 *  MS_PROVISIONING_SUPPORT
 *
 *  This flag is used to enable support for Provisioning feature.
 *
 *  Dependency: None.
 */
#define MS_PROVISIONING_SUPPORT

/*
 *  MS_RELAY_SUPPORT
 *
 *  This flag is used to enable support for Relay feature.
 *
 *  Dependency: None.
 */
#define MS_RELAY_SUPPORT

/*
 *  MS_PROXY_SUPPORT
 *
 *  This flag is used to enable support for Proxy feature.
 *
 *  Dependency: None.
 */
#define MS_PROXY_SUPPORT

/*
 *  MS_PROXY_SERVER
 *
 *  This flag is used to enable support for Proxy Server feature.
 *
 *  Dependency: MS_PROXY_SUPPORT.
 */
#ifdef MS_PROXY_SUPPORT
#define MS_PROXY_SERVER
#endif /* MS_PROXY_SUPPORT */

/*
 *  MS_PROXY_CLIENT
 *
 *  This flag is used to enable support for Proxy Client feature.
 *
 *  Dependency: MS_PROXY_SUPPORT.
 */
#ifdef MS_PROXY_SUPPORT
#define MS_PROXY_CLIENT
#endif /* MS_PROXY_SUPPORT */

/**
 * Check if the relevant roles for Proxy are Enabled
 */
#ifdef MS_PROXY_SUPPORT
#if !((defined MS_PROXY_SERVER) || (defined MS_PROXY_CLIENT))
#error "Server or Client SHALL be supported when MS_PROXY_SUPPORT enabled"
#endif /* !((defined MS_PROXY_SERVER) || (defined MS_PROXY_CLIENT)) */
#endif /* MS_PROXY_SUPPORT */

/*
 *  MS_FRIEND_SUPPORT
 *
 *  This flag is used to enable support for Friend feature.
 *
 *  Dependency: None.
 */
#define MS_FRIEND_SUPPORT

/*
 *  MS_LPN_SUPPORT
 *
 *  This flag is used to enable support for Low Power feature.
 *
 *  Dependency: None.
 */
#define MS_LPN_SUPPORT


/* ----------------------------------------------------------------------- */
/* ==== Stack Architecture Flags ========================================= */
/* ----------------------------------------------------------------------- */


/* ----------------------------------------------------------------------- */
/* ==== Stack Feature Flags ============================================== */
/* ----------------------------------------------------------------------- */
/*
 *  MS_SUPPORT_STACK_VERSION_INFO
 *
 *  This flag enables EtherMind MS_get_version_number() API.
 *
 *  This API is used to retrieve the current stack build version
 *  information
 *
 *  Dependency: None.
 */
#define MS_SUPPORT_STACK_VERSION_INFO

/*
 *  MS_HAVE_DYNAMIC_CONFIG
 *
 *  This flag enables dynamic configuration of some of the parameters defined in MS_limits.h.
 *
 *  Dependency: None.
 */
#define MS_HAVE_DYNAMIC_CONFIG

/*
 *  MS_HAVE_DYNAMIC_GLOBAL_ARRAY
 *
 *  This flag enables dynamic allocation of global data structures.
 *
 *  If this flag is defined, the dynamic configuration of the data structures
 *  (like number of maximum AppKeys supported etc.) shall be configured using
 *  MS_CONFIG data structure and MS_init() API.
 *
 *  Dependency: MS_HAVE_DYNAMIC_CONFIG.
 */
#ifdef MS_HAVE_DYNAMIC_CONFIG
#define MS_HAVE_DYNAMIC_GLOBAL_ARRAY
#endif /* MS_HAVE_DYNAMIC_CONFIG */

/*
 *  MS_DISABLE_MUTEX
 *
 *  This flag disables the use of mutex and conditional variables from all
 *  stack modules (protocols and profiles).
 *  This should be defined in EtherMind builds where synchronization
 *  primitives are not required (such as, single task build of EtherMind).
 *
 *  Dependency: None.
 */
#define MS_DISABLE_MUTEX

/*
 *  VAR_ARG_IN_MACRO_NOT_SUPPORTED
 *
 *  This flag should be enabled if the compiler tool-chain does not support
 *  variable argument in macro.
 */
/* #define VAR_ARG_IN_MACRO_NOT_SUPPORTED */

/*
 *  MS_STORAGE
 *
 *  This flag enables support of the Storage Module
 *
 *  Dependency: None
 */
#define MS_STORAGE

/*
 *  MS_LOGGER
 *
 *  This flag enables support for the platform logger
 *
 *  Dependency: None
 */
#define MS_LOGGER

/*
 *  MS_MODEL_SERVER_EXTENDED_INTERFACE
 *
 *  This flag enables extended interfaces for Model Servers.
 *  Currently only for model server initialization. These new interfaces
 *  are in the format MS_<model_name>_server_init_ext()
 *                                               ^^^^
 */
#define MS_MODEL_SERVER_EXTENDED_INTERFACE

/* ----------------------------------------------------------------------- */
/* ==== Module Inclusion Flags for EtherMind Mesh Modules ================ */
/* ----------------------------------------------------------------------- */
/* Mesh Config Model */
#define MS_MODEL_CONFIG

/* ----------------------------------------------------------------------- */
/* ==== Module Inclusion Flags for EtherMind Mesh Modules ================ */
/* ----------------------------------------------------------------------- */
/*
 *  Module inclusion flags for various EtherMind Mesh Modules, and should be
 *  defined according to the modules included in the build.
 *
 *  Dependency: None.
 */
/* Mesh Bearer Module */
#define MS_BRR
/* Mesh Network Module */
#define MS_NET
/* Mesh Lower Transport Module */
#define MS_LTRN
/* Mesh Transport Module */
#define MS_TRN
/* Mesh Access Module */
#define MS_ACCESS

/* ----------------------------------------------------------------------- */
/* ==== Bearer Module Specific Flags ===================================== */
/* ----------------------------------------------------------------------- */

/*
 *  MS_BEARER_ADV
 *
 *  An advertising bearer.
 */
#define MS_BEARER_ADV

/*
 *  MS_BEARER_GATT
 *
 *  A GATT bearer.
 */
/* #define MS_BEARER_GATT */

/*
 * BLEBRR_LP_SUPPORT
 *
 * Enables Low Power Mode interfaces in the bearer.
 */
/* #define BLEBRR_LP_SUPPORT */

/* ----------------------------------------------------------------------- */
/* ==== Network Module Specific Flags ==================================== */
/* ----------------------------------------------------------------------- */
/*
 *  MS_NET_TAG_REPLAY_PKT_SUPPORT
 *
 *  Enables support for Network layer Callback handler to Tag specific
 *  received network packets as Replayed Packets. The network layer
 *  then can decide corresponding action for those packets.
 *
 *  Dependency: None.
 */
#define MS_NET_TAG_REPLAY_PKT_SUPPORT

/*
 *  MS_NET_BASIC_SEQNUM_CACHE
 *
 *  Enables support for Network layer to use the older basic sequence
 *  number tracking logic.
 *
 *  Dependency: None.
 */
/* #define MS_NET_BASIC_SEQNUM_CACHE */

/*
 *  MS_NET_SEQNUM_CACHE_ENABLE_STRICT_RANGE
 *
 *  Enables support for Network layer to use strict sequence number range check
 *  when bit-array based sequence number tracking logic is used.
 *
 *  Dependency: MS_NET_BASIC_SEQNUM_CACHE not be defined.
 */
#define MS_NET_SEQNUM_CACHE_ENABLE_STRICT_RANGE

/*
 *  MS_NET_IGNORE_CACHE_WRAP
 *
 *  Enables support for Network layer to Ignores cache wrapping and process
 *  packets with older sequence number which are received after the cache is
 *  full.
 *  This is needed for handling a PTS test item in the Mesh Relay category.
 */
#define MS_NET_IGNORE_CACHE_WRAP

/*
 *  MS_NET_PROCESS_MASTER_SUBNET_ON_LPN_PROXY_BEARER
 *
 *  Enables support for LPN to be listening to packets on Master Subnet
 *  even when there is an active ongoing Friendship with another Friend
 *  node on the network.
 *  This is an optional feature, can be enabled if this specific use-case
 *  needs to be supported.
 *
 *  Dependency: None.
 */
#define MS_NET_PROCESS_MASTER_SUBNET_ON_LPN_PROXY_BEARER

/*
 *  MS_NET_FILTER_TX_ON_NETIF_HNDL
 *
 *  Enables support for filtering of network packets based on network
 *  interface handles.
 *  When this feature is enabled, the Network Interface layers sends
 *  the enqueued Network packet for transmission only on the requested
 *  network bearer interface. If no Network Interface is specified, then
 *  the enqueued Network packet will be transmitted on all the available
 *  bearers.
 *  Also, when this feature is enabled all proxy configuration packets will
 *  be transmitted only over GATT Bearer Network Interface.
 *
 *  This is an optional feature, can be enabled if this specific use-case
 *  needs to be supported.
 *
 *  Dependency: None.
 */
#define MS_NET_FILTER_TX_ON_NETIF_HNDL

/* ----------------------------------------------------------------------- */
/* ==== Transport Module Specific Flags ================================== */
/* ----------------------------------------------------------------------- */
/*
 *  MS_TRN_SEND_UNSEG_MSG_SUPPORT
 *
 *  Enables support for sending Unsegmented Transport Message procedure.
 */
#define MS_TRN_SEND_UNSEG_MSG_SUPPORT

/*
 *  MS_TRN_SEND_CTRL_PKT_SUPPORT
 *
 *  Enables support for sending Transport Control Packet procedure.
 */
#define MS_TRN_SEND_CTRL_PKT_SUPPORT

#ifdef MS_TRN_SEND_CTRL_PKT_SUPPORT
/*
 *  MS_TRN_CTRL_FRND_PING_SUPPORT
 *
 *  Enables support for sending Transport Control Packet - Friend Poll procedure.
 */
#define MS_TRN_CTRL_FRND_POLL_SUPPORT

/*
 *  MS_TRN_CTRL_FRND_UPDATE_SUPPORT
 *
 *  Enables support for sending Transport Control Packet - Friend Update procedure.
 */
#define MS_TRN_CTRL_FRND_UPDATE_SUPPORT

/*
 *  MS_TRN_CTRL_FRND_REQ_SUPPORT
 *
 *  Enables support for sending Transport Control Packet - Friend Request procedure.
 */
#define MS_TRN_CTRL_FRND_REQ_SUPPORT

/*
 *  MS_TRN_CTRL_FRND_OFFER_SUPPORT
 *
 *  Enables support for sending Transport Control Packet - Friend Offer procedure.
 */
#define MS_TRN_CTRL_FRND_OFFER_SUPPORT

/*
 *  MS_TRN_CTRL_FRND_CLEAR_SUPPORT
 *
 *  Enables support for sending Transport Control Packet - Friend Clear procedure.
 */
#define MS_TRN_CTRL_FRND_CLEAR_SUPPORT

/*
 *  MS_TRN_CTRL_FRND_CLEAR_CNF_SUPPORT
 *
 *  Enables support for sending Transport Control Packet - Friend Clear Confirmation procedure.
 */
#define MS_TRN_CTRL_FRND_CLEAR_CNF_SUPPORT

/*
 *  MS_TRN_CTRL_FRND_SUBSCRN_LIST_ADD_SUPPORT
 *
 *  Enables support for sending Transport Control Packet - Friend Subscription List Add procedure.
 */
#define MS_TRN_CTRL_FRND_SUBSCRN_LIST_ADD_SUPPORT

/*
 *  MS_TRN_CTRL_FRND_SUBSCRN_LIST_REMOVE_SUPPORT
 *
 *  Enables support for sending Transport Control Packet - Friend Subscription List Remove procedure.
 */
#define MS_TRN_CTRL_FRND_SUBSCRN_LIST_REMOVE_SUPPORT

/*
 *  MS_TRN_CTRL_FRND_SUBSCRN_LIST_CNF_SUPPORT
 *
 *  Enables support for sending Transport Control Packet - Friend Subscription List Confirmation procedure.
 */
#define MS_TRN_CTRL_FRND_SUBSCRN_LIST_CNF_SUPPORT

/*
 *  MS_TRN_CTRL_HEARTBEAT_SUPPORT
 *
 *  Enables support for sending Transport Control Packet - Heartbeat procedure.
 */
#define MS_TRN_CTRL_HEARTBEAT_SUPPORT

#endif /* MS_TRN_SEND_CTRL_PKT_SUPPORT */

/*
 *  MS_TRN_NO_NULL_PARAM_CHECK
 *
 *  This flag disables 'null' parameter check in Mesh Transport APIs
 *
 *  When application using Mesh Transport APIs are validated and
 *  it is ensured that none of the API call path is using
 *  an unexpected 'null' parameter, this flag can be enabled
 *  to reduce the code size.
 *
 *  Dependency: None.
 */
/* #define MS_TRN_NO_NULL_PARAM_CHECK */

/*
 *  MS_TRN_NO_RANGE_CHECK
 *
 *  This flag disables the check in Mesh Transport APIs to verify if the parameter
 *  values are within specification defined valid range
 *
 *  When application using Mesh Transport APIs are validated and
 *  it is ensured that none of the API call path is using
 *  parameters with invalid value, this flag can be enabled
 *  to reduce the code size.
 *
 *  Dependency: None.
 */
/* #define MS_TRN_NO_RANGE_CHECK */

/* ----------------------------------------------------------------------- */
/* ==== Access Module Specific Flags ===================================== */
/* ----------------------------------------------------------------------- */
/*
 *  MS_ACCESS_PUBLISH_TIMER_SUPPORT
 *
 *  This flag enables Publication Timer Support.
 *
 *  Dependency: None.
 */
#define MS_ACCESS_PUBLISH_TIMER_SUPPORT

/*
 *  MS_ACCESS_IV_UPDT_TEST_MODE_SUPPORT
 *
 *  This flag enables the IVI Test Mode support.
 *
 *  Dependency: None.
 */
#define MS_ACCESS_IV_UPDT_TEST_MODE_SUPPORT

/*
 *  MS_ACESS_IV_INDEX_CHECK_IN_PROGRESS_SUPPORT
 *
 *  This flag enables the check for IV Index values when
 *  in "In-Progress" mode. If this is enabled, any IV index
 *  greater than the current value of IV Index when "In-Progress"
 *  is discarded.
 *
 *  Dependency: None.
 */
#define MS_ACESS_IV_INDEX_CHECK_IN_PROGRESS_SUPPORT

/* ----------------------------------------------------------------------- */
/* ==== Configuration Model Specific Flags =============================== */
/* ----------------------------------------------------------------------- */
/*
 *  MS_CONFIG_SERVER_HAVE_APPL_CB
 *
 *  This flag enables Configuration Server to provide a Callback to the
 *  Application layer.
 *
 *  Dependency: None.
 */
#define MS_CONFIG_SERVER_HAVE_APPL_CB

/* ----------------------------------------------------------------------- */
/* ==== Model Specific Flags ============================================= */
/* ----------------------------------------------------------------------- */
/*
 *  MS_LIGHT_LC_SERVER_INTERNAL_FSM
 *
 *  This flag enables internal FSM usage for Light LC Server Model.
 *
 *  Dependency: None.
 */
#define MS_LIGHT_LC_SERVER_INTERNAL_FSM

#endif /* _H_MS_FEATURES_ */

