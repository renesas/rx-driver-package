
/**
 *  \file MS_error.h
 *
 *  This file lists all the Error Codes returned by EtherMind
 *  Mesh APIs.
 */

/*
 *  Copyright (C) 2017. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_MS_ERROR_
#define _H_MS_ERROR_

/**
 * \defgroup ms_error_codes Error Code Definitions
 * \ingroup ms_common_defines
 * \{
 *  \brief This section contains all the error codes defined for Mesh stack
 *  and profiles.
 *  <p>
 *  \anchor error_code_overview
 *  <b>Theory:</b>
 *  <p>
 *    Every API under Mesh Protocol Suite returns \ref API_RESULT,
 *    which is a 2 Byte Unsigned Short variable. The higher order byte
 *    signifies the Module from which the Error has been generated, and
 *    the lower order byte encodes the actual reason of Error.
 *    <p>
 *    Each module under Mesh Stack is given unique
 *    Error ID (the higher order byte). Also, for each module the Error
 *    Code (the lower order byte) signifies an unique error situation.
 *    <p>
 *    For Mesh Protocol Modules (e.g., Transport, Network etc.), Error IDs are
 *    assigned from the range 0x10 to 0x4F. For Profiles, the range
 *    is from 0x50 to 0x7F.
 *    <p>
 *    The definition of \ref API_SUCCESS is 0x0000 - which is the 'Success'
 *    return value for an API returning \ref API_RESULT. All other values for
 *    should be treated as Errors.
 *    <p>
 *    The definition of \ref API_FAILURE is 0xFFFF - which stands for
 *    "Unknown Error Situation".
 *    <p>
 *  <b>Note:</b>
 *    <p>
 *    The applications using native/profile Mesh API should NOT do
 *    any check on the basis of \ref API_FAILURE - rather, the correct way to
 *    detect an error situation is by doing a negative check on \ref
 *    API_SUCCESS.
 *    <p>
 *    For example,
 *    \code if ( API_FAILURE == MS_access_register_model(x, y, z) ) \endcode
 *    ... Wrong !
 *    <p>
 *    <p>
 *    \code if ( API_SUCCESS != MS_access_register_model(x, y, z) ) \endcode
 *    ... Correct !
 */

/** \} */

/**
 * \addtogroup ms_common_structures Structures
 * \{
 */
#ifndef API_RESULT_DEFINED
/** Definition of API_RESULT */
typedef UINT16    API_RESULT;

/**
 * \cond ignore_this Ignore this function while generating Doxygen document
 */

/** Flag to track if \ref API_RESULT is already defined */
#define API_RESULT_DEFINED

/**
 * \endcond
 */

#endif /* API_RESULT_DEFINED */
/** \} */

/**
 * \defgroup ms_common_error_codes Error Codes
 * \ingroup ms_error_codes
 * \{
 * \brief This section describes the various Modules and their Error Codes
 * exposed and used by the EtherMind Mesh and Model Stack.
 */

/**
 * \defgroup ms_common_default_error_codes Default Error Codes
 * \{
 * \brief This section describes the Default Error Codes used across the
 * EtherMind Mesh and Model Stack and exposed to its Applications.
 */
/* Definitions of API_SUCCESS & API_FAILURE */
#ifdef API_SUCCESS
#undef API_SUCCESS
#endif /* API_SUCCESS */
/** Status - 'Success' */
#define API_SUCCESS             0x0000

#ifdef API_FAILURE
#undef API_FAILURE
#endif /* API_FAILURE */
/** Status - 'Failure' */
#define API_FAILURE             0xFFFF

/** \} */

/** \} */

/* ====================== EtherMind Module Error IDs ====================== */

/**
 * \defgroup ms_common_error_groups Error Grouping
 * \ingroup ms_error_codes
 * \{
 * \brief This section describes the various Error Code Groupings exposed
 * by the EtherMind Mesh and Model Stack.
 */

/**
 * \name Mesh Specification Error Codes (0x00XX - 0x11XX)
 * Error IDs for Bluetooth Mesh Specification Defined Error Codes (0x00 - 0x11).
 * \{
 */

/** Error Codes for Mesh - \ref ms_common_mesh_spec_error_codes */
#define MS_ERR_ID                               0x0000

/** \} */

/**
 * \name Mesh Common Modules (0x10 - 0x1F)
 * Error IDs for Mesh Common Modules (0x10 - 0x1F).
 * \{
 */

/** Error Codes for MS Common - \ref ms_common_common_error_codes */
#define MS_COMMON_ERR_ID                        0x1000

/** \} */

/**
 * \name Mesh Core Modules (0x20 - 0x3F)
 * \anchor error_code_core_modules_tag Error IDs for Mesh Core Modules (0x20 - 0x3F).
 * \{
 */

/** Error Codes for Bearer - \ref brr_error_codes */
#define BRR_ERR_ID                              0x2000
/** Error Codes for Network - \ref net_error_codes */
#define NET_ERR_ID                              0x2100
/** Error Codes for Lower Transport - \ref ltrn_error_codes */
#define LTRN_ERR_ID                             0x2200
/** Error Codes for Transport - \ref trn_error_codes */
#define TRN_ERR_ID                              0x2300
/** Error Codes for Access - \ref access_error_codes */
#define ACCESS_ERR_ID                           0x2400

/** Error Codes for Provisioning - \ref prov_error_codes */
#define PROV_ERR_ID                             0x3000

/** \} */

/**
 * \name Mesh Models Modules (0x40 - 0x7F)
 * Error IDs for Mesh Model Modules (0x40 - 0x7F).
 * \{
 */
/** Error Codes for Generic Models - \ref ms_model_common_error_codes */
#define GENERIC_MMDL_ERR_ID                     0x4000
/** Error Codes for Config Model - \ref config_error_codes */
#define CONFIG_ERR_ID                           0x4100
/** Error Codes for Health Model - \ref health_error_codes */
#define HEALTH_ERR_ID                           0x4200

/** \} */

/** \} */

/* ================== EtherMind Common Reason Error Codes ================= */

/**
 * \defgroup ms_common_common_error_codes Common Error Codes
 * \ingroup ms_common_error_codes
 * \{
 * \brief This section describes the Common Error Codes exposed and used
 * by the EtherMind Mesh and Model Stack.
 */

/**
 * \name Mesh Common OSAL Error Codes
 * \{
 */
/** Error Code for MUTEX Initialization Failure */
#define _MUTEX_INIT_FAILED                      0x0001
/** Error Code for Conditional Variable Initialization Failure */
#define _COND_INIT_FAILED                       0x0002
/** Error Code for MUTEX Lock Failure */
#define _MUTEX_LOCK_FAILED                      0x0003
/** Error Code for MUTEX Unlock Failure */
#define _MUTEX_UNLOCK_FAILED                    0x0004
/** Error Code for Memory Allocation Failure */
#define _MEMORY_ALLOCATION_FAILED               0x0005
/** \} */

/** \} */

/* ======================================= Section 'Mesh' */

/**
 * \defgroup ms_common_mesh_spec_error_codes Mesh Result Codes
 * \ingroup ms_common_error_codes
 * \{
 * \brief This section describes the Mesh Result/Status/Error
 * Codes defined by the Bluetooth Mesh and Model Specification.
 */
/**
 * \name Mesh Result/Status Codes
 * \{
 */
/** Success status */
#define MS_SUCCESS                              (0x0000 | MS_ERR_ID)
/** Invalid Address */
#define MS_INVALID_ADDRESS                      (0x0001 | MS_ERR_ID)
/** Invalid Model */
#define MS_INVALID_MODEL                        (0x0002 | MS_ERR_ID)
/** Invalid AppKey Index */
#define MS_INVALID_APPKEY_INDEX                 (0x0003 | MS_ERR_ID)
/** Invalid NetKey Index */
#define MS_INVALID_NETKEY_INDEX                 (0x0004 | MS_ERR_ID)
/** Insufficient Resources */
#define MS_INSUFFICIENT_RESOURCES               (0x0005 | MS_ERR_ID)
/** Key Index Already Stored */
#define MS_KEY_INDEX_ALREADY_STORED             (0x0006 | MS_ERR_ID)
/** Invalid Publish Parameters */
#define MS_INVALID_PUBLISH_PARAMETER            (0x0007 | MS_ERR_ID)
/** Not a Subscribe Model */
#define MS_NOT_A_SUBSCRIBE_MODEL                (0x0008 | MS_ERR_ID)
/** Storage Failure */
#define MS_STORAGE_FAILURE                      (0x0009 | MS_ERR_ID)
/** Feature Not Supported */
#define MS_FEATURE_NOT_SUPPORTED                (0x000A | MS_ERR_ID)
/** Cannot Update */
#define MS_CANNOT_UPDATE                        (0x000B | MS_ERR_ID)
/** Cannot Remove */
#define MS_CANNOT_REMOVE                        (0x000C | MS_ERR_ID)
/** Cannot Bind */
#define MS_CANNOT_BIND                          (0x000D | MS_ERR_ID)
/** Temporarily Unable to Change State */
#define MS_TEMP_UNABLE_TO_CHANGE_STATE          (0x000E | MS_ERR_ID)
/** Cannot Set */
#define MS_CANNOT_SET                           (0x000F | MS_ERR_ID)
/** Unspecified Error */
#define MS_UNSPECIFIED_ERROR                    (0x0010 | MS_ERR_ID)
/** Invalid Binding */
#define MS_INVALID_BINDING                      (0x0011 | MS_ERR_ID)
/** \} */

/** \} */

/* ======================================= Section 'Bearer' */
/**
 * \addtogroup brr_defines
 * \{
 */

/**
 * \defgroup brr_error_codes Error Codes
 * \{
 * \brief This sections describes the Error Codes exposed and used
 * by the EtherMind Mesh Bearer Layer.
 */

/**
 * \name Bearer Layer OSAL Error Codes
 * \{
 */
/** Bearer Error Code for MUTEX Initialization Failure */
#define BRR_MUTEX_INIT_FAILED                    \
        (_MUTEX_INIT_FAILED | BRR_ERR_ID)
/** Bearer Error Code for Conditional Variable Initialization Failure */
#define BRR_COND_INIT_FAILED                     \
        (_COND_INIT_FAILED | BRR_ERR_ID)
/** Bearer Error Code for MUTEX Lock Failure */
#define BRR_MUTEX_LOCK_FAILED                    \
        (_MUTEX_LOCK_FAILED | BRR_ERR_ID)
/** Bearer Error Code for MUTEX Unlock Failure */
#define BRR_MUTEX_UNLOCK_FAILED                  \
        (_MUTEX_UNLOCK_FAILED | BRR_ERR_ID)
/** Bearer Error Code for Memory Allocation Failure */
#define BRR_MEMORY_ALLOCATION_FAILED             \
        (_MEMORY_ALLOCATION_FAILED | BRR_ERR_ID)
/** \} */

/**
 * \name Bearer Layer API/Event Error Codes
 * \{
 */
/** Bearer Error Code for Invalid Parameter Value */
#define BRR_INVALID_PARAMETER_VALUE             (0x0011 | BRR_ERR_ID)
/** Bearer Error Code for Parameter Outside Range */
#define BRR_PARAMETER_OUTSIDE_RANGE             (0x0012 | BRR_ERR_ID)
/** Bearer Error Code for NULL Parameter Not Allowed */
#define BRR_NULL_PARAMETER_NOT_ALLOWED          (0x0013 | BRR_ERR_ID)
/** Bearer Error Code for Interface Not Read */
#define BRR_INTERFACE_NOT_READY                 (0x0014 | BRR_ERR_ID)
/** Bearer Error Code for API Not Supported */
#define BRR_API_NOT_SUPPORTED                   (0x00FF | BRR_ERR_ID)
/** \} */

/** \} */

/** \} */

/* ======================================= Section 'Network' */
/**
 * \addtogroup net_defines
 * \{
 */

/**
 * \defgroup net_error_codes Error Codes
 * \{
 * \brief This sections describes the Error Codes exposed and used
 * by the EtherMind Mesh Network Layer.
 */

/**
 * \name Network Layer OSAL Error Codes
 * \{
 */
/** Network Error Code for MUTEX Initialization Failure */
#define NET_MUTEX_INIT_FAILED                    \
        (_MUTEX_INIT_FAILED | NET_ERR_ID)
/** Network Error Code for Conditional Variable Initialization Failure */
#define NET_COND_INIT_FAILED                     \
        (_COND_INIT_FAILED | NET_ERR_ID)
/** Network Error Code for MUTEX Lock Failure */
#define NET_MUTEX_LOCK_FAILED                    \
        (_MUTEX_LOCK_FAILED | NET_ERR_ID)
/** Network Error Code for MUTEX Unlock Failure */
#define NET_MUTEX_UNLOCK_FAILED                  \
        (_MUTEX_UNLOCK_FAILED | NET_ERR_ID)
/** Network Error Code for Memory Allocation Failure */
#define NET_MEMORY_ALLOCATION_FAILED             \
        (_MEMORY_ALLOCATION_FAILED | NET_ERR_ID)
/** \} */

/**
 * \name Network Layer API/Event Error Codes
 * \{
 */
/** Network Error Code for Invalid Parameter Value */
#define NET_INVALID_PARAMETER_VALUE             (0x0011 | NET_ERR_ID)
/** Network Error Code for Parameter Outside Range */
#define NET_PARAMETER_OUTSIDE_RANGE             (0x0012 | NET_ERR_ID)
/** Network Error Code for NULL Parameter Not Allowed */
#define NET_NULL_PARAMETER_NOT_ALLOWED          (0x0013 | NET_ERR_ID)
/** Network Error Code for Transmit Queue Full */
#define NET_TX_QUEUE_FULL                       (0x0014 | NET_ERR_ID)
/** Network Error Code for Transmit Queue Empty */
#define NET_TX_QUEUE_EMPTY                      (0x0015 | NET_ERR_ID)
/**
 * Network Error Code returned by Network Callback, indicating if it
 * detected an invalid packet format or if the packet to be further
 * processed, by the network layer like to be relayed or proxied etc.
 */
#define NET_INVALID_RX_PKT_FORMAT               (0x0016 | NET_ERR_ID)
/** Network Error Code for reception of locally sourced packet */
#define NET_RX_LOCAL_SRC_ADDR_PKT               (0x0017 | NET_ERR_ID)
/**
 * Network Error Code returned by Network Callback, indicating if the
 * received packet needs further processing by the network layer like
 * to be relayed or proxied etc. after the control being returned by
 * the callback.
 */
#define NET_POST_PROCESS_RX_PKT                 (0x0018 | NET_ERR_ID)
/** Network Error Code for Already Relayed Packet */
#define NET_RX_ALREADY_RELAYED_PKT              (0x0019 | NET_ERR_ID)
/**
 * Network Error code to tag all errors returned by the Crypto Interface
 * to the Network layer.
 */
#define NET_CRYPTO_UNLIKELY_ERR                 (0x0020 | NET_ERR_ID)
/**
 * Network Error Code returned by Network Callback, indicating if the
 * received packet is for a known LPN with an ongoing Friendship which
 * needs further processing by the network layer like to be relayed or
 * proxied etc. after the control being returned by the callback.
 */
#define NET_RX_LPN_SRC_ADDR_TO_RELAY_PKT        (0x0021 | NET_ERR_ID)
/** Network Error Code for API Not Supported */
#define NET_API_NOT_SUPPORTED                   (0x00FF | NET_ERR_ID)
/** \} */

/** \} */

/** \} */

/* ======================================= Section 'Lower Transport' */
/**
 * \addtogroup ltrn_defines
 * \{
 */

/**
 * \defgroup ltrn_error_codes Error Codes
 * \{
 * \brief This sections describes the Error Codes exposed and used
 * by the EtherMind Mesh Lower Transport Layer.
 */

/**
 * \name Lower Transport Layer OSAL Error Codes
 * \{
 */
/** Lower Transport Error Code for MUTEX Initialization Failure */
#define LTRN_MUTEX_INIT_FAILED                    \
        (_MUTEX_INIT_FAILED | LTRN_ERR_ID)
/** Lower Transport Error Code for Conditional Variable Initialization Failure */
#define LTRN_COND_INIT_FAILED                     \
        (_COND_INIT_FAILED | LTRN_ERR_ID)
/** Lower Transport Error Code for MUTEX Lock Failure */
#define LTRN_MUTEX_LOCK_FAILED                    \
        (_MUTEX_LOCK_FAILED | LTRN_ERR_ID)
/** Lower Transport Error Code for MUTEX Unlock Failure */
#define LTRN_MUTEX_UNLOCK_FAILED                  \
        (_MUTEX_UNLOCK_FAILED | LTRN_ERR_ID)
/** Lower Transport Error Code for Memory Allocation Failure */
#define LTRN_MEMORY_ALLOCATION_FAILED             \
        (_MEMORY_ALLOCATION_FAILED | LTRN_ERR_ID)
/** \} */

/**
 * \name Lower Transport Layer API/Event Error Codes
 * \{
 */
/** Lower Transport Error Code for Invalid Parameter Value */
#define LTRN_INVALID_PARAMETER_VALUE             (0x0011 | LTRN_ERR_ID)
/** Lower Transport Error Code for Parameter Outside Range */
#define LTRN_PARAMETER_OUTSIDE_RANGE             (0x0012 | LTRN_ERR_ID)
/** Lower Transport Error Code for NULL Parameter Not Allowed */
#define LTRN_NULL_PARAMETER_NOT_ALLOWED          (0x0013 | LTRN_ERR_ID)
/** Lower Transport Error Code for SAR Context Allocation Failure */
#define LTRN_SAR_CTX_ALLOCATION_FAILED           (0x0014 | LTRN_ERR_ID)
/** \} */

/** \} */

/** \} */

/* ======================================= Section 'Transport' */
/**
 * \addtogroup trn_defines
 * \{
 */

/**
 * \defgroup trn_error_codes Error Codes
 * \{
 * \brief This sections describes the Error Codes exposed and used
 * by the EtherMind Mesh Transport Layer.
 */

/**
 * \name Transport Layer OSAL Error Codes
 * \{
 */
/** Transport Error Code for MUTEX Initialization Failure */
#define TRN_MUTEX_INIT_FAILED                    \
        (_MUTEX_INIT_FAILED | TRN_ERR_ID)
/** Transport Error Code for Conditional Variable Initialization Failure */
#define TRN_COND_INIT_FAILED                     \
        (_COND_INIT_FAILED | TRN_ERR_ID)
/** Transport Error Code for MUTEX Lock Failure */
#define TRN_MUTEX_LOCK_FAILED                    \
        (_MUTEX_LOCK_FAILED | TRN_ERR_ID)
/** Transport Error Code for MUTEX Unlock Failure */
#define TRN_MUTEX_UNLOCK_FAILED                  \
        (_MUTEX_UNLOCK_FAILED | TRN_ERR_ID)
/** Transport Error Code for Memory Allocation Failure */
#define TRN_MEMORY_ALLOCATION_FAILED             \
        (_MEMORY_ALLOCATION_FAILED | TRN_ERR_ID)
/** \} */

/**
 * \name Transport Layer API/Event Error Codes
 * \{
 */
/** Transport Error Code for Invalid Parameter Value */
#define TRN_INVALID_PARAMETER_VALUE             (0x0011 | TRN_ERR_ID)
/** Transport Error Code for Parameter Outside Range */
#define TRN_PARAMETER_OUTSIDE_RANGE             (0x0012 | TRN_ERR_ID)
/** Transport Error Code for NULL Parameter Not Allowed */
#define TRN_NULL_PARAMETER_NOT_ALLOWED          (0x0013 | TRN_ERR_ID)
/** Transport Error Code for Queue Full */
#define TRN_QUEUE_FULL                          (0x0014 | TRN_ERR_ID)
/** Transport Error Code for Queue Empty */
#define TRN_QUEUE_EMPTY                         (0x0015 | TRN_ERR_ID)
/** Transport Error Code for Incomplete Packet Reception */
#define TRN_INCOMPLETE_PKT_RECEIVED             (0x0016 | TRN_ERR_ID)
/** Transport Error Code for Invalid Friendship State */
#define TRN_INVALID_FRNDSHIP_STATE              (0x0017 | TRN_ERR_ID)
/**
 * Transport Error code to tag all errors returned by the Crypto Interface
 * to the Transport layer.
 */
#define TRN_CRYPTO_UNLIKELY_ERR                 (0x0018 | TRN_ERR_ID)
/** Transport Error Code for Friendship Cleared on Timeout */
#define TRN_FRNDSHIP_CLEARED_ON_TIMEOUT         (0x0019 | TRN_ERR_ID)
/** Transport Error Code for API Not Supported */
#define TRN_API_NOT_SUPPORTED                   (0x00FF | TRN_ERR_ID)
/** \} */

/** \} */

/** \} */

/* ======================================= Section 'Access' */
/**
 * \addtogroup access_defines
 * \{
 */

/**
 * \defgroup access_error_codes Error Codes
 * \{
 * \brief This section describes the Error Codes exposed and used
 * by the EtherMind Mesh Access Layer.
 */

/**
 * \name Access Layer OSAL Error Codes
 * \{
 */
/** Access Error Code for MUTEX Initialization Failure */
#define ACCESS_MUTEX_INIT_FAILED                    \
        (_MUTEX_INIT_FAILED | ACCESS_ERR_ID)
/** Access Error Code for Conditional Variable Initialization Failure */
#define ACCESS_COND_INIT_FAILED                     \
        (_COND_INIT_FAILED | ACCESS_ERR_ID)
/** Access Error Code for MUTEX Lock Failure */
#define ACCESS_MUTEX_LOCK_FAILED                    \
        (_MUTEX_LOCK_FAILED | ACCESS_ERR_ID)
/** Access Error Code for MUTEX Unlock Failure */
#define ACCESS_MUTEX_UNLOCK_FAILED                  \
        (_MUTEX_UNLOCK_FAILED | ACCESS_ERR_ID)
/** Access Error Code for Memory Allocation Failure */
#define ACCESS_MEMORY_ALLOCATION_FAILED             \
        (_MEMORY_ALLOCATION_FAILED | ACCESS_ERR_ID)
/** \} */

/**
 * \name Access Layer API/Event Error Codes
 * \{
 */
/** Access Error Code for Invalid Parameter Value */
#define ACCESS_INVALID_PARAMETER_VALUE             (0x0011 | ACCESS_ERR_ID)
/** Access Error Code for Parameter Outside Range */
#define ACCESS_PARAMETER_OUTSIDE_RANGE             (0x0012 | ACCESS_ERR_ID)
/** Access Error Code for NULL Parameter Not Allowed */
#define ACCESS_NULL_PARAMETER_NOT_ALLOWED          (0x0013 | ACCESS_ERR_ID)
/** Access Error Code for No Resources */
#define ACCESS_NO_RESOURCE                         (0x0020 | ACCESS_ERR_ID)
/** Access Error Code for No Match */
#define ACCESS_NO_MATCH                            (0x0021 | ACCESS_ERR_ID)
/**
 * Access Error Code for Invalid Handle. This is to be interpreted based
 * on the type of the parameter which is being validated.
 * This error can occur for validation of
 * Invalid \ref MS_SUBNET_HANDLE
 * Invalid \ref MS_APPKEY_HANDLE
 * Invalid \ref MS_ACCESS_ADDRESS_HANDLE
 * Invalid \ref MS_ACCESS_MODEL_HANDLE
 * Invalid \ref MS_ACCESS_ELEMENT_HANDLE
 */
#define ACCESS_INVALID_HANDLE                      (0x0022 | ACCESS_ERR_ID)
/** Access Error Code for Model Already Registered */
#define ACCESS_MODEL_ALREADY_REGISTERED            (0x0023 | ACCESS_ERR_ID)
/** Access Error Code for Invalid Source Address */
#define ACCESS_INVALID_SRC_ADDR                    (0x0024 | ACCESS_ERR_ID)
/** Access Error Code for Device Key Table Full */
#define ACCESS_DEV_KEY_TABLE_FULL                  (0x0025 | ACCESS_ERR_ID)
/**
 * Access Error Code when detecting Packets with Master Network
 * Credentials from a known address with ongoing Friendship
 */
#define ACCESS_MASTER_NID_ON_LPN                   (0x0026 | ACCESS_ERR_ID)
/** Access Error Code for Invalid Publication State */
#define ACCESS_INVALID_PUBLICATION_STATE           (0x0027 | ACCESS_ERR_ID)
/** Access Error Code for Invalid Publication TTL */
#define ACCESS_INVALID_PUBLICATION_TTL             (0x0028 | ACCESS_ERR_ID)
/** Access Error Code for not permitted IV Index Value */
#define ACCESS_IV_VAL_NOT_PERMITTED                (0x0030 | ACCESS_ERR_ID)
/**
 * Access Error Code for IV Update occurring too soon within the
 * stipulated/specified Time Limit
 */
#define ACCESS_IV_UPDATE_TOO_SOON                  (0x0031 | ACCESS_ERR_ID)
/** Access Error Code for IV Update in Incorrect State */
#define ACCESS_IV_INCORRECT_STATE                  (0x0032 | ACCESS_ERR_ID)
/** Access Error Code for IV Update deferred due to currently Busy State */
#define ACCESS_IV_UPDATE_DEFERRED_IN_BUSY          (0x0033 | ACCESS_ERR_ID)

/** Access Error Code to track Device is Provisioned */
#define ACCESS_DEVICE_IS_PROVISIONED               (0x00A0 | ACCESS_ERR_ID)
/** Access Error Code to track Device is Not Provisioned */
#define ACCESS_DEVICE_IS_NOT_PROVISIONED           (0x00A1 | ACCESS_ERR_ID)
/** Access Error Code for No Model Addition after Provisioning */
#define ACCESS_ERR_MODEL_ADD_NODE_PROVISIONED      (0x00A2 | ACCESS_ERR_ID)
/** Access Error Code for No Element Addition after Provisioning */
#define ACCESS_ERR_ELEMENT_ADD_NODE_PROVISIONED    (0x00A3 | ACCESS_ERR_ID)

/** Access Error Code for API Not Supported */
#define ACCESS_API_NOT_SUPPORTED                   (0x00FF | ACCESS_ERR_ID)
/** \} */

/** \} */

/** \} */

/* ======================================= Section 'Provisioning' */
/**
 * \addtogroup prov_error_codes
 * \{
 * \brief This section describes the Provisioning Layer Error Codes.
 */

/**
 * \name Provisioning Module OSAL Error Codes
 * \{
 */
/** Provisioning Error Code for MUTEX Initialization Failure */
#define PROV_MUTEX_INIT_FAILED                    \
        (_MUTEX_INIT_FAILED | PROV_ERR_ID)
/** Provisioning Error Code for Conditional Variable Initialization Failure */
#define PROV_COND_INIT_FAILED                     \
        (_COND_INIT_FAILED | PROV_ERR_ID)
/** Provisioning Error Code for MUTEX Lock Failure */
#define PROV_MUTEX_LOCK_FAILED                    \
        (_MUTEX_LOCK_FAILED | PROV_ERR_ID)
/** Provisioning Error Code for MUTEX Unlock Failure */
#define PROV_MUTEX_UNLOCK_FAILED                  \
        (_MUTEX_UNLOCK_FAILED | PROV_ERR_ID)
/** Provisioning Error Code for Memory Allocation Failure */
#define PROV_MEMORY_ALLOCATION_FAILED             \
        (_MEMORY_ALLOCATION_FAILED | PROV_ERR_ID)
/** \} */

/**
 * \name Provisioning Module API/Event Error Codes
 * \{
 */
/** Provisioning Error Code for Invalid State */
#define PROV_INVALID_STATE                          (0x0011 | PROV_ERR_ID)
/** Bearer Error Code for Invalid Parameter Value */
#define PROV_INVALID_PARAMETER                      (0x0012 | PROV_ERR_ID)
/** Provisioning Error Code for Context Allocation Failure */
#define PROV_CONTEXT_ALLOC_FAILED                   (0x0013 | PROV_ERR_ID)
/** Provisioning Error Code for Context Assertion Failure */
#define PROV_CONTEXT_ASSERT_FAILED                  (0x0014 | PROV_ERR_ID)
/** Provisioning Error Code for Context Link Open */
#define PROV_CONTEXT_LINK_OPEN                      (0x0015 | PROV_ERR_ID)
/** Provisioning Error Code for Bearer Assertion Failure */
#define PROV_BEARER_ASSERT_FAILED                   (0x0016 | PROV_ERR_ID)
/** Provisioning Error Code for Procedure Timeout */
#define PROV_PROCEDURE_TIMEOUT                      (0x0017 | PROV_ERR_ID)
/** \} */

/** \} */

/** \} */

/* ======================================= Section 'Generic Models' */

/**
 * \defgroup ms_model_common Model Common
 * \ingroup mesh_models_block
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * Mesh Models Common Layer to the Application and other upper layers of the
 * stack.
 */

/**
 * \defgroup ms_model_common_defines Defines
 * \{
 * \brief This section describes the various Defines in EtherMind Mesh
 * Model Common Layer.
 */

/**
 * \defgroup ms_model_common_error_codes Error Codes
 * \{
 * \brief This section describes the Common Error Codes exposed and used
 * by the EtherMind Mesh Model Common Layer.
 *
 * \b NOTE:
 * The error codes in this section have the prefix <b><em> GENERIC_ </em></b>,
 * but these are not to be confused with the
 * \ref generics_models "Generics Models".
 * These are named so, as these are generic error codes that may occur in any
 * of the Mesh Model Layers(i.e. Generics Model, Light Model, Scene Model etc.)
 */

/**
 * \name Mesh Model Common OSAL Error Codes
 * \{
 */
/** Generic Model Error Code for MUTEX Initialization Failure */
#define GENERIC_MMDL_MUTEX_INIT_FAILED                    \
        (_MUTEX_INIT_FAILED | GENERIC_MMDL_ERR_ID)
/** Generic Model Error Code for Conditional Variable Initialization Failure */
#define GENERIC_MMDL_COND_INIT_FAILED                     \
        (_COND_INIT_FAILED | GENERIC_MMDL_ERR_ID)
/** Generic Model Error Code for MUTEX Lock Failure */
#define GENERIC_MMDL_MUTEX_LOCK_FAILED                    \
        (_MUTEX_LOCK_FAILED | GENERIC_MMDL_ERR_ID)
/** Generic Model Error Code for MUTEX Unlock Failure */
#define GENERIC_MMDL_MUTEX_UNLOCK_FAILED                  \
        (_MUTEX_UNLOCK_FAILED | GENERIC_MMDL_ERR_ID)
/** Generic Model Error Code for Memory Allocation Failure */
#define GENERIC_MMDL_MEMORY_ALLOCATION_FAILED             \
        (_MEMORY_ALLOCATION_FAILED | GENERIC_MMDL_ERR_ID)
/** \} */

/**
 * \name Mesh Model Common API/Event Error Codes
 * \{
 */
/** Generic Model Error Code for Invalid State */
#define GENERIC_MMDL_INVALID_STATE                          (0x0011 | GENERIC_MMDL_ERR_ID)
/** Generic Model Error Code for Invalid Parameter Value */
#define GENERIC_MMDL_INVALID_PARAMETER                      (0x0012 | GENERIC_MMDL_ERR_ID)
/** Generic Model Error Code for Parameter Outside Range */
#define GENERIC_MMDL_PARAMETER_OUTSIDE_RANGE                (0x0013 | GENERIC_MMDL_ERR_ID)
/** Generic Model Error Code for NULL Parameter Not Allowed */
#define GENERIC_MMDL_NULL_PARAMETER_NOT_ALLOWED             (0x0014 | GENERIC_MMDL_ERR_ID)
/** Generic Model Error Code for Invalid Model Handle */
#define GENERIC_MMDL_INVALID_MODEL_HANDLE                   (0x0015 | GENERIC_MMDL_ERR_ID)
/** Generic Model Error Code for Invalid Element Handle */
#define GENERIC_MMDL_INVALID_ELEMENT_HANDLE                 (0x0016 | GENERIC_MMDL_ERR_ID)
/**
 * Generic Model Error Code when Operation is detected on a Node element which
 * is not Primary Element
 */
#define GENERIC_MMDL_NOT_PRIMARY_ELEMENT                    (0x0017 | GENERIC_MMDL_ERR_ID)
/** Generic Model Error Code for Model Context Allocation Failure */
#define GENERIC_MMDL_CONTEXT_ALLOC_FAILED                   (0x0018 | GENERIC_MMDL_ERR_ID)
/** Generic Model Error Code for Model Context Assertion Failure */
#define GENERIC_MMDL_CONTEXT_ASSERT_FAILED                  (0x0019 | GENERIC_MMDL_ERR_ID)
/** Generic Model Error Code for Invalid Length */
#define GENERIC_MMDL_INVALID_VALUE_LENGTH                   (0x001A | GENERIC_MMDL_ERR_ID)
/** Generic Model Error Code for API Not Supported */
#define GENERIC_MMDL_API_NOT_SUPPORTED                      (0x00FF | GENERIC_MMDL_ERR_ID)
/** \} */

/** \} */

/** \} */

/** \} */

/* ======================================= Section 'Config Model' */
/**
 * \addtogroup config_defines
 * \{
 */

/**
 * \defgroup config_error_codes Error Codes
 * \{
 * \brief This sections describes the Error Codes exposed and used
 * by the EtherMind Mesh Configuration Model Layer.
 */

/**
 * \name Configuration Model OSAL Error Codes
 * \{
 */
/** Config Model Error Code for MUTEX Initialization Failure */
#define CONFIG_MUTEX_INIT_FAILED                    \
        (_MUTEX_INIT_FAILED | CONFIG_ERR_ID)
/** Config Model Error Code for Conditional Variable Initialization Failure */
#define CONFIG_COND_INIT_FAILED                     \
        (_COND_INIT_FAILED | CONFIG_ERR_ID)
/** Config Model Error Code for MUTEX Lock Failure */
#define CONFIG_MUTEX_LOCK_FAILED                    \
        (_MUTEX_LOCK_FAILED | CONFIG_ERR_ID)
/** Config Model Error Code for MUTEX Unlock Failure */
#define CONFIG_MUTEX_UNLOCK_FAILED                  \
        (_MUTEX_UNLOCK_FAILED | CONFIG_ERR_ID)
/** Config Model Error Code for Memory Allocation Failure */
#define CONFIG_MEMORY_ALLOCATION_FAILED             \
        (_MEMORY_ALLOCATION_FAILED | CONFIG_ERR_ID)
/** \} */

/**
 * \name Configuration Model API/Event Error Codes
 * \{
 */
/** Config Model Error Code for Invalid State */
#define CONFIG_INVALID_STATE                          (0x0011 | CONFIG_ERR_ID)
/** Config Model Error Code for Invalid Parameter Value */
#define CONFIG_INVALID_PARAMETER                      (0x0012 | CONFIG_ERR_ID)
/** Config Model Error Code for Parameter Outside Range */
#define CONFIG_PARAMETER_OUTSIDE_RANGE                (0x0013 | CONFIG_ERR_ID)
/** Config Model Error Code for NULL Parameter Not Allowed */
#define CONFIG_NULL_PARAMETER_NOT_ALLOWED             (0x0014 | CONFIG_ERR_ID)
/** Config Model Error Code for API Not Supported */
#define CONFIG_API_NOT_SUPPORTED                      (0x00FF | CONFIG_ERR_ID)
/** \} */

/** \} */

/** \} */

/* ======================================= Section 'Health Model' */
/**
 * \addtogroup health_defines
 * \{
 */

/**
 * \defgroup health_error_codes Error Codes
 * \{
 * \brief This sections describes the Error Codes exposed and used
 * by the EtherMind Mesh Health Model Layer.
 */

/**
 * \name Health Model OSAL Error Codes
 * \{
 */
/** Health Model Error Code for MUTEX Initialization Failure */
#define HEALTH_MUTEX_INIT_FAILED                    \
        (_MUTEX_INIT_FAILED | HEALTH_ERR_ID)
/** Health Model Error Code for Conditional Variable Initialization Failure */
#define HEALTH_COND_INIT_FAILED                     \
        (_COND_INIT_FAILED | HEALTH_ERR_ID)
/** Health Model Error Code for MUTEX Lock Failure */
#define HEALTH_MUTEX_LOCK_FAILED                    \
        (_MUTEX_LOCK_FAILED | HEALTH_ERR_ID)
/** Health Model Error Code for MUTEX Unlock Failure */
#define HEALTH_MUTEX_UNLOCK_FAILED                  \
        (_MUTEX_UNLOCK_FAILED | HEALTH_ERR_ID)
/** Health Model Error Code for Memory Allocation Failure */
#define HEALTH_MEMORY_ALLOCATION_FAILED             \
        (_MEMORY_ALLOCATION_FAILED | HEALTH_ERR_ID)
/** \} */

/**
 * \name Health Model API/Event Error Codes
 * \{
 */
/** Health Model Error Code for Invalid State */
#define HEALTH_INVALID_STATE                          (0x0011 | HEALTH_ERR_ID)
/** Health Model Error Code for Invalid Parameter Value */
#define HEALTH_INVALID_PARAMETER                      (0x0012 | HEALTH_ERR_ID)
/** Health Model Error Code for Context Allocation Failure */
#define HEALTH_CONTEXT_ALLOC_FAILED                   (0x0013 | HEALTH_ERR_ID)
/** Health Model Error Code for Context Assertion Failure */
#define HEALTH_CONTEXT_ASSERT_FAILED                  (0x0014 | HEALTH_ERR_ID)
/** \} */

/** \} */

/** \} */

#endif /* _H_MS_ERROR_ */

