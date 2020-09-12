
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
 *  If this flag is defined, the dynamic configuration of the data strucutures
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

/* ----------------------------------------------------------------------- */
/* ==== Module Inclusion Flags for EtherMind Mesh Modules ================ */
/* ----------------------------------------------------------------------- */
/* Mesh Models */
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
#define MS_BRR
#define MS_NET
#define MS_LTRN
#define MS_TRN
#define MS_ACCESS

/* ----------------------------------------------------------------------- */
/* ==== Transport Module Specific Flags ================================== */
/* ----------------------------------------------------------------------- */
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

#endif /* _H_MS_FEATURES_ */
