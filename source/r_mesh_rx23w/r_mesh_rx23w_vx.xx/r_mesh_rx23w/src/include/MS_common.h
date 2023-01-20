
/**
 *  \file MS_common.h
 *
 *  This Header file describes common declarations for the
 *  EtherMind Mesh modules.
 */

/*
 *  Copyright (C) 2017. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_MS_COMMON_
#define _H_MS_COMMON_

/**
 * \defgroup mesh_core_block Mesh Core
 * \brief Mesh Core block is composed of modules corresponding with each layer
 * defined by Mesh Core Specification and provides application with the
 * features to perform Provisioning process and mesh networking operations.
 * Visit the below URL from Bluetooth SIG for the current supported
 * version of Mesh Profile Specification.
 * https://www.bluetooth.com/specifications/specs/mesh-profile-1-0-1/
 */

/**
 * \defgroup mesh_models_block Mesh Models
 * \brief Mesh Models block is composed of modules corresponding with each
 * model defined by Mesh Model Specification and provides application with the
 * features to support Mesh models that defines basic functionality on a mesh
 * network.
 * Visit the below URL from Bluetooth SIG for the current supported
 * version of Mesh Model Specification.
 * https://www.bluetooth.com/specifications/specs/mesh-model-1-0-1/
 */

/**
 * \defgroup foundation_models Foundation Models
 * \ingroup mesh_models_block
 * \brief This section corresponds to the Foundation Models specified by
 * the Mesh Core Specification.
 */

/**
 * \defgroup generics_models Generics Models
 * \ingroup mesh_models_block
 * \brief This section corresponds to the Generics Models specified by
 * the Mesh Model Specification.
 */

/**
 * \defgroup sensors_models Sensors Models
 * \ingroup mesh_models_block
 * \brief This section corresponds to the Sensors Models specified by
 * the Mesh Model Specification.
 */

/**
 * \defgroup time_and_scenes_models Time and Scenes Models
 * \ingroup mesh_models_block
 * \brief This section corresponds to the Time and Scenes Models specified by
 * the Mesh Model Specification.
 */

/**
 * \defgroup lighting_models Lighting Models
 * \ingroup mesh_models_block
 * \brief This section corresponds to the Lighting Models specified by
 * the Mesh Model Specification.
 */

/**
 * \defgroup ms_common_module Mesh Common
 * \ingroup mesh_core_block
 * \{
 * \brief This section describes the interfaces & APIs offered by the EtherMind
 * Mesh Common module to the Application and other upper layers of the stack.
 */

/* -------------------------------------------- Header File Inclusion */
/* The EtherMind OS Abstraction */
#include "EM_os.h"

/* The EtherMind Configuration Parameters */
#include "MS_features.h"

/* The Bluetooth Assigned Numbers */
#include "MS_assigned_numbers.h"

/* The EtherMind Error Codes */
#include "MS_error.h"

/* The EtherMind Timer Library */
#include "EM_timer.h"

/* Mesh Model States related Inclusion */
#include "MS_model_states.h"

/* -------------------------------------------- Global Definitions */

/**
 * \defgroup ms_common_defines Defines
 * \{
 * \brief This section describes the various Defines in EtherMind Mesh
 * Common Layer.
 */

/**
 * \defgroup ms_common_constants Constants
 * \{
 * \brief This section describes the various Constants in EtherMind Mesh
 * Common Layer.
 */

/**
 * \name Module States
 * \{
 */
/** Mesh Core Module State - Invalid */
#define MS_MODULE_STATE_INVALID                0x00
/** Mesh Core Module State - Initialized */
#define MS_MODULE_STATE_INITIALIZED            0x01
/** \} */

/**
 * \name Boolean States
 * \{
 */
/** Definition for True/False */
#ifndef MS_FALSE
/** Mesh Stack Common Definition for "False" */
#define MS_FALSE                                   0
#endif /* MS_FALSE */

#ifndef MS_TRUE
/** Mesh Stack Common Definition for "True" */
#define MS_TRUE                                    1
#endif /* MS_TRUE */

/** \} */

/** \} */

/* -------------------------------------------- Macros */

/**
 * \defgroup ms_common_utility_macros Utility Macros
 * \{
 * \brief This section describes the various Utility Macros in EtherMind Mesh
 * Common Layer.
 */

/**
 *  \name Packing Macros.
 *
 *  \b Syntax: MS_PACK_<Endian-ness LE/BE>_<no_of_bytes>_BYTE
 *
 *  \b Usage: Based on the endian-ness defined for each Mesh and Model layer,
 *  appropriate packing macros to be used by each layer.
 *
 *  \b Example: Access Layer, Foundation Model Layer and other Mesh Model
 *  layers are defined as little endian i.e. multiple-octet numeric values
 *  in these layers are represented as little endian.
 *  So in these layers, for packing a parameter of size 2 byte, \ref
 *  MS_PACK_LE_2_BYTE macro is to be used.
 *
 *  By default both the packing and unpacking macros uses pointer to
 *  a single or multi-octet variable which to be packed to or unpacked from
 *  a buffer (unsigned character array).
 *
 *  For the packing macro, another variation is available,
 *  where the single or multi-octet variable itself is used (not its pointer).
 *
 *  \b Syntax: MS_PACK_<Endian-ness LE/BE>_<no_of_bytes>_BYTE_VAL
 *
 *  \{
 */

/** Mesh Little Endian Packing Macros */
/** Mesh Common Macro to Pack 1 Byte in Little Endian */
#define MS_PACK_LE_1_BYTE(dst, src) \
    { \
        UCHAR val; \
        val = (UCHAR)(*(src)); \
        MS_PACK_LE_1_BYTE_VAL((dst), val); \
    }

/** Mesh Common Macro to Pack 1 Byte Value in Little Endian */
#define MS_PACK_LE_1_BYTE_VAL(dst, src) \
    *((UCHAR *)(dst) + 0) = src;

/** Mesh Common Macro to Pack 2 Bytes in Little Endian */
#define MS_PACK_LE_2_BYTE(dst, src) \
    { \
        UINT16 val; \
        val = (UINT16)(*(src)); \
        MS_PACK_LE_2_BYTE_VAL((dst), val); \
    }

/** Mesh Common Macro to Pack 2 Bytes Value in Little Endian */
#define MS_PACK_LE_2_BYTE_VAL(dst, src) \
    *((UCHAR *)(dst) + 0) = (UCHAR)(src); \
    *((UCHAR *)(dst) + 1) = (UCHAR)(src >> 8);

/** Mesh Common Macro to Pack 3 Bytes in Little Endian */
#define MS_PACK_LE_3_BYTE(dst, src) \
    { \
        UINT32 val; \
        val = (UINT32)(*(src)); \
        MS_PACK_LE_3_BYTE_VAL((dst), val); \
    }

/** Mesh Common Macro to Pack 3 Bytes Value in Little Endian */
#define MS_PACK_LE_3_BYTE_VAL(dst, src) \
    *((UCHAR *)(dst) + 0) = (UCHAR)(src);\
    *((UCHAR *)(dst) + 1) = (UCHAR)(src >> 8);\
    *((UCHAR *)(dst) + 2) = (UCHAR)(src >> 16);

/** Mesh Common Macro to Pack 4 Bytes in Little Endian */
#define MS_PACK_LE_4_BYTE(dst, src) \
    { \
        UINT32 val; \
        val = (UINT32)(*(src)); \
        MS_PACK_LE_4_BYTE_VAL((dst), val); \
    }

/** Mesh Common Macro to Pack 4 Bytes Value in Little Endian */
#define MS_PACK_LE_4_BYTE_VAL(dst, src) \
    *((UCHAR *)(dst) + 0) = (UCHAR)(src);\
    *((UCHAR *)(dst) + 1) = (UCHAR)(src >> 8);\
    *((UCHAR *)(dst) + 2) = (UCHAR)(src >> 16);\
    *((UCHAR *)(dst) + 3) = (UCHAR)(src >> 24);

/** Mesh Common Macro to Pack 8 Bytes in Little Endian */
#define MS_PACK_LE_8_BYTE(dst,val)\
        EM_mem_copy ((dst), (val), 8)

/** Mesh Common Macro to Pack 16 Bytes in Little Endian */
#define MS_PACK_LE_16_BYTE(dst,val)\
        EM_mem_copy ((dst), (val), 16)

/** Mesh Common Macro to Pack "n" Bytes in Little Endian */
#define MS_PACK_LE_N_BYTE(dst,val,n)\
        EM_mem_copy ((dst), (val), (n))

/** Big Endian Packing Macros */
/** Mesh Common Macro to Pack 1 Byte in Big Endian */
#define MS_PACK_BE_1_BYTE(dst, src) \
    { \
        UCHAR val; \
        val = (UCHAR)(*((UCHAR *)(src))); \
        MS_PACK_BE_1_BYTE_VAL((dst), val); \
    }

/** Mesh Common Macro to Pack 1 Byte Value in Big Endian */
#define MS_PACK_BE_1_BYTE_VAL(dst, src) \
    *((UCHAR *)(dst) + 0) = src;

/** Mesh Common Macro to Pack 2 Bytes in Big Endian */
#define MS_PACK_BE_2_BYTE(dst, src) \
    { \
        UINT16 val; \
        val = (UINT16)(*((UINT16 *)(src))); \
        MS_PACK_BE_2_BYTE_VAL((dst), val); \
    }

/** Mesh Common Macro to Pack 2 Bytes Value in Big Endian */
#define MS_PACK_BE_2_BYTE_VAL(dst, src) \
    *((UCHAR *)(dst) + 1) = (UCHAR)(src); \
    *((UCHAR *)(dst) + 0) = (UCHAR)(src >> 8);

/** Mesh Common Macro to Pack 3 Bytes in Big Endian */
#define MS_PACK_BE_3_BYTE(dst, src) \
    { \
        UINT32 val; \
        val = (UINT32)(*((UINT32 *)(src))); \
        MS_PACK_BE_3_BYTE_VAL((dst), val); \
    }

/** Mesh Common Macro to Pack 3 Bytes Value in Big Endian */
#define MS_PACK_BE_3_BYTE_VAL(dst, src) \
    *((UCHAR *)(dst) + 2) = (UCHAR)(src);\
    *((UCHAR *)(dst) + 1) = (UCHAR)(src >> 8);\
    *((UCHAR *)(dst) + 0) = (UCHAR)(src >> 16);

/** Mesh Common Macro to Pack 4 Bytes in Big Endian */
#define MS_PACK_BE_4_BYTE(dst, src) \
    { \
        UINT32 val; \
        val = (UINT32)(*((UINT32 *)(src))); \
        MS_PACK_BE_4_BYTE_VAL((dst), val); \
    }

/** Mesh Common Macro to Pack 4 Bytes Value in Big Endian */
#define MS_PACK_BE_4_BYTE_VAL(dst, src) \
    *((UCHAR *)(dst) + 3) = (UCHAR)(src);\
    *((UCHAR *)(dst) + 2) = (UCHAR)(src >> 8);\
    *((UCHAR *)(dst) + 1) = (UCHAR)(src >> 16);\
    *((UCHAR *)(dst) + 0) = (UCHAR)(src >> 24);

/** Mesh Common Macro to Pack 8 Bytes in Big Endian */
#define MS_PACK_BE_8_BYTE(dst,val)\
        EM_mem_copy ((dst), (val), 8)

/** Mesh Common Macro to Pack 16 Bytes in Big Endian */
#define MS_PACK_BE_16_BYTE(dst,val)\
        EM_mem_copy ((dst), (val), 16)

/** Mesh Common Macro to Pack "n" Bytes in Big Endian */
#define MS_PACK_BE_N_BYTE(dst,val,n)\
        EM_mem_copy ((dst), (val), (n))

/** \} */

/**
 *  \name Unpacking Macros.
 *
 *  \b Syntax: MS_UNPACK_<Endian-ness LE/BE>_<no_of_bytes>_BYTE
 *
 *  \b Usage: Based on the endian-ness defined for each protocol/profile layer,
 *  appropriate unpacking macros to be used by each layer.
 *
 *  \b Example: Access Layer, Foundation Model Layer and other Mesh Model
 *  layers are defined as little endian i.e. multiple-octet numeric values
 *  in these layers are represented as little endian.
 *  So in these layers, for unpacking a parameter of size 4 byte, \ref
 *  MS_UNPACK_LE_4_BYTE macro is to be used.
 *
 *  \{
 */

/** Little Endian Unpacking Macros */
/** Mesh Common Macro to Unpack 1 Byte in Little Endian */
#define MS_UNPACK_LE_1_BYTE(dst,src)\
    *((UCHAR *)(dst)) = (UCHAR)(*((UCHAR *)(src)));

/** Mesh Common Macro to Unpack 2 Bytes in Little Endian */
#define MS_UNPACK_LE_2_BYTE(dst,src)\
        *((UINT16 *)(dst))  = *((src) + 1); \
        *((UINT16 *)(dst))  = *((UINT16 *)(dst)) << 8; \
        *((UINT16 *)(dst)) |= *((src) + 0);

/** Mesh Common Macro to Unpack 3 Bytes in Little Endian */
#define MS_UNPACK_LE_3_BYTE(dst,src)\
        *((UINT32 *)(dst))  = *((src) + 2);\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8;\
        *((UINT32 *)(dst)) |= *((src) + 1);\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8;\
        *((UINT32 *)(dst)) |= *((src) + 0);

/** Mesh Common Macro to Unpack 4 Bytes in Little Endian */
#define MS_UNPACK_LE_4_BYTE(dst,src)\
        *((UINT32 *)(dst))  = *((src) + 3);\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8;\
        *((UINT32 *)(dst)) |= *((src) + 2);\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8;\
        *((UINT32 *)(dst)) |= *((src) + 1);\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8;\
        *((UINT32 *)(dst)) |= *((src) + 0);

/** Mesh Common Macro to Unpack 8 Bytes in Little Endian */
#define MS_UNPACK_LE_8_BYTE(dst,src)\
        EM_mem_copy ((dst), (src), 8)

/** Mesh Common Macro to Unpack 16 Bytes in Little Endian */
#define MS_UNPACK_LE_16_BYTE(dst,src)\
        EM_mem_copy ((dst), (src), 16)

/** Mesh Common Macro to Unpack "n" Bytes in Little Endian */
#define MS_UNPACK_LE_N_BYTE(dst,src,n)\
        EM_mem_copy ((dst), (src), (n))

/** Big Endian Unpacking Macros */
/** Mesh Common Macro to Unpack 1 Byte in Big Endian */
#define MS_UNPACK_BE_1_BYTE(dst,src)\
    *((UCHAR *)(dst)) = (UCHAR)(*((UCHAR *)(src)));

/** Mesh Common Macro to Unpack 2 Bytes in Big Endian */
#define MS_UNPACK_BE_2_BYTE(dst,src)\
        *((UINT16 *)(dst))  = *((src) + 0); \
        *((UINT16 *)(dst))  = *((UINT16 *)(dst)) << 8; \
        *((UINT16 *)(dst)) |= *((src) + 1);

/** Mesh Common Macro to Unpack 3 Bytes in Big Endian */
#define MS_UNPACK_BE_3_BYTE(dst,src)\
        *((UINT32 *)(dst))  = *((src) + 0);\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8;\
        *((UINT32 *)(dst)) |= *((src) + 1);\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8;\
        *((UINT32 *)(dst)) |= *((src) + 2);

/** Mesh Common Macro to Unpack 4 Bytes in Big Endian */
#define MS_UNPACK_BE_4_BYTE(dst,src)\
        *((UINT32 *)(dst))  = *((src) + 0);\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8;\
        *((UINT32 *)(dst)) |= *((src) + 1);\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8;\
        *((UINT32 *)(dst)) |= *((src) + 2);\
        *((UINT32 *)(dst))  = (*((UINT32 *)(dst))) << 8;\
        *((UINT32 *)(dst)) |= *((src) + 3);

/** Mesh Common Macro to Unpack 8 Bytes in Big Endian */
#define MS_UNPACK_BE_8_BYTE(dst,src)\
        EM_mem_copy ((dst), (src), 8)

/** Mesh Common Macro to Unpack 16 Bytes in Big Endian */
#define MS_UNPACK_BE_16_BYTE(dst,src)\
        EM_mem_copy ((dst), (src), 16)

/** Mesh Common Macro to Unpack "n" Bytes in Big Endian */
#define MS_UNPACK_BE_N_BYTE(dst,src,n)\
        EM_mem_copy ((dst), (src), (n))

/** \} */

/**
 * \name Task Synchronization Macros
 * \{
 */

#ifndef MS_DISABLE_MUTEX

/** Macro to define a Mutex Variable */
#define MS_DEFINE_MUTEX(mutex) EM_thread_mutex_type mutex;

/** Macro to define a Mutex Variable with a type qualifier */
#define MS_DEFINE_MUTEX_TYPE(type, mutex) type EM_thread_mutex_type mutex;

/** Macro to define a Conditional Variable */
#define MS_DEFINE_COND(cond) EM_thread_cond_type cond;

/** Macro to define a Conditional Variable with a type qualifier */
#define MS_DEFINE_COND_TYPE(type, cond) type EM_thread_cond_type cond;

/**
 *  Macro to Initialize Mutex.
 *  To be used in void functions as it returns no error.
 */
#define MS_MUTEX_INIT_VOID(mutex, MODULE)                                \
    if (EM_thread_mutex_init(&(mutex), NULL) < 0)                        \
    {                                                                    \
        EM_debug_error(                                                  \
        MS_MODULE_ID_##MODULE,                                           \
        "FAILED to Initialize Mutex in " #MODULE ".\n");                 \
        return;                                                          \
    }

/**
 *  Macro to Initialize Mutex.
 *  This returns an error if mutex initialization fails.
 */
#define MS_MUTEX_INIT(mutex, MODULE)                                     \
    if (EM_thread_mutex_init(&(mutex), NULL) < 0)                        \
    {                                                                    \
        EM_debug_error(                                                  \
        MS_MODULE_ID_##MODULE,                                           \
        "FAILED to Initialize Mutex in " #MODULE ".\n");                 \
        return MODULE##_MUTEX_INIT_FAILED;                               \
    }

/**
 *  Macro to Initialize Conditional Variable.
 *  To be used in void function as it returns no error.
 */
#define MS_COND_INIT_VOID(cond, MODULE)                                  \
    if (EM_thread_cond_init(&(cond), NULL) < 0)                          \
    {                                                                    \
        EM_debug_error(                                                  \
        MS_MODULE_ID_##MODULE,                                           \
        "FAILED to Initialize Conditional Variable in " #MODULE ".\n");  \
        return;                                                          \
    }

/**
 *  Macro to Initialize Conditional Variable.
 *  It returns an error if conditional variable initialization fails.
 */
#define MS_COND_INIT(cond, MODULE)                                       \
    if (EM_thread_cond_init(&(cond), NULL) < 0)                          \
    {                                                                    \
        EM_debug_error(                                                  \
        MS_MODULE_ID_##MODULE,                                           \
        "FAILED to Initialize Conditional Variable in " #MODULE ".\n");  \
        return MODULE##_COND_INIT_FAILED;                                \
    }

/**
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. It returns an error if mutex lock fails.
 */
#define MS_MUTEX_LOCK(mutex, MODULE)                                 \
    if (EM_thread_mutex_lock(&(mutex)) < 0)                          \
    {                                                                \
        EM_debug_error(                                              \
        MS_MODULE_ID_##MODULE,                                       \
        "FAILED to Lock Mutex in " #MODULE ".\n");                   \
        return MODULE##_MUTEX_LOCK_FAILED;                           \
    }

/**
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. To be used in void function as it
 *  returns no error.
 */
#define MS_MUTEX_LOCK_VOID(mutex, MODULE)                            \
    if (EM_thread_mutex_lock(&(mutex)) < 0)                          \
    {                                                                \
        EM_debug_error(                                              \
        MS_MODULE_ID_##MODULE,                                       \
        "FAILED to Lock Mutex in " #MODULE ".\n");                   \
        return;                                                      \
    }

/**
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function.
 *  It returns the error 'value' if mutex lock failes.
 */
#define MS_MUTEX_LOCK_RETURN_ON_FAILURE(mutex, MODULE, value)        \
    if (EM_thread_mutex_lock(&(mutex)) < 0)                          \
    {                                                                \
        EM_debug_error(                                              \
        MS_MODULE_ID_##MODULE,                                       \
        "FAILED to Lock Mutex in " #MODULE ".\n");                   \
        return (value);                                              \
    }

/**
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. On failure, only an Error is logged.
 *  It can be used from both void and non-void functions.
 */
#define MS_MUTEX_LOCK_DONOT_RETURN_ON_FAILURE(mutex, MODULE)         \
    if (EM_thread_mutex_lock(&(mutex)) < 0)                          \
    {                                                                \
        EM_debug_error(                                              \
        MS_MODULE_ID_##MODULE,                                       \
        "FAILED to Lock Mutex in " #MODULE ".\n");                   \
    }

/**
 *  Unlocks the Module Specific Mutex which releases the global variables
 *  to be written into. It returns an error if mutex unlock fails.
 */
#define MS_MUTEX_UNLOCK(mutex, MODULE)                               \
    if (EM_thread_mutex_unlock(&(mutex)) < 0)                        \
    {                                                                \
        EM_debug_error(                                              \
        MS_MODULE_ID_##MODULE,                                       \
        "FAILED to Unlock Mutex in " #MODULE ".\n");                 \
        return MODULE##_MUTEX_UNLOCK_FAILED;                         \
    }

/**
 *  Unlocks the Module Specific Mutex which releases the global variables
 *  to be written into. To be used in void functions as it returns
 *  no error.
 */
#define MS_MUTEX_UNLOCK_VOID(mutex, MODULE)                          \
    if (EM_thread_mutex_unlock(&(mutex)) < 0)                        \
    {                                                                \
        EM_debug_error(                                              \
        MS_MODULE_ID_##MODULE,                                       \
        "FAILED to Unlock Mutex in " #MODULE ".\n");                 \
        return;                                                      \
    }

/**
 *  Unlocks the Module Specific Mutex which releases the global variables
 *  to be written into.
 *  It returns the error 'value' if mutex unlock fails.
 */
#define MS_MUTEX_UNLOCK_RETURN_ON_FAILURE(mutex, MODULE, value)      \
    if (EM_thread_mutex_unlock(&(mutex)) < 0)                        \
    {                                                                \
        EM_debug_error(                                              \
        MS_MODULE_ID_##MODULE,                                       \
        "FAILED to Unlock Mutex in " #MODULE ".\n");                 \
        return (value);                                              \
    }

/**
 *  Unlocks the Module Specific Mutex which releases the global variables
 *  to be written into. On failure, only Error is logged.
 *  It can be used from both void and non-void functions.
 */
#define MS_MUTEX_UNLOCK_DONOT_RETURN_ON_FAILURE(mutex, MODULE)       \
    if (EM_thread_mutex_unlock(&(mutex)) < 0)                        \
    {                                                                \
        EM_debug_error(                                              \
        MS_MODULE_ID_##MODULE,                                       \
        "FAILED to Unlock Mutex in " #MODULE ".\n");                 \
    }

#else  /* MS_DISABLE_MUTEX */

/** Macro to define a Mutex Variable */
#define MS_DEFINE_MUTEX(mutex)

/** Macro to define a Mutex Variable with a type qualifier */
#define MS_DEFINE_MUTEX_TYPE(type, mutex)

/** Macro to define a Conditional Variable */
#define MS_DEFINE_COND(cond)

/** Macro to define a Conditional Variable with a type qualifier */
#define MS_DEFINE_COND_TYPE(type, cond)

/**
 *  Macro to Initialize Mutex.
 *  To be used in void function as it returns no error.
 */
#define MS_MUTEX_INIT_VOID(mutex, MODULE)

/**
 *  Macro to Initialize Mutex.
 *  It returns an error if mutex initialization fails.
 */
#define MS_MUTEX_INIT(mutex, MODULE)

/**
 *  Macro to Initialize Conditional Variable.
 *  To be used in void function as it returns no error.
 */
#define MS_COND_INIT_VOID(cond, MODULE)

/**
 *  Macro to Initialize Conditional Variable.
 *  It returns an error if conditional variable initialization fails.
 */
#define MS_COND_INIT(cond, MODULE)

/**
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. It returns an error if mutex lock fails.
 */
#define MS_MUTEX_LOCK(mutex, MODULE)

/**
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. To be used in void function as it
 *  returns no error.
 */
#define MS_MUTEX_LOCK_VOID(mutex, MODULE)

/**
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function.
 *  It returns the error 'value' if mutex lock fails.
 */
#define MS_MUTEX_LOCK_RETURN_ON_FAILURE(mutex, MODULE, value)

/**
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. On failure, only an Error is logged.
 *  It can be used from both void and non-void functions.
 */
#define MS_MUTEX_LOCK_DONOT_RETURN_ON_FAILURE(mutex, MODULE)

/**
 *  Unlocks the Module Specific Mutex which releases the global variables
 *  to be written into. It returns an error if mutex unlock fails.
 */
#define MS_MUTEX_UNLOCK(mutex, MODULE)

/**
 *  Unlocks the Module Specific Mutex which releases the global variables
 *  to be written into. To be used in void functions as it returns
 *  no error.
 */
#define MS_MUTEX_UNLOCK_VOID(mutex, MODULE)

/**
 *  Unlocks the Module Specific Mutex which releases the global variables
 *  to be written into.
 *  It returns the error 'value' if mutex unlock fails.
 */
#define MS_MUTEX_UNLOCK_RETURN_ON_FAILURE(mutex, MODULE, value)

/**
 *  Unlocks the Module Specific Mutex which releases the global variables
 *  to be written into. On failure, only Error is logged.
 *  It can be used from both void and non-void functions.
 */
#define MS_MUTEX_UNLOCK_DONOT_RETURN_ON_FAILURE(mutex, MODULE)

#endif /* MS_DISABLE_MUTEX */

/** \} */

/**
 * \name Bit-wise Operation Macros
 * \{
 */
/** Abstractions for bit-wise operation */
/** Macro to extract value of the corresponding bit number */
#define MS_EXTRACT_BITNUM(val, bitnum)        (((val) >> (bitnum)) & 1)
/** Macro to set the value of the corresponding bit number */
#define MS_SET_BITNUM(val, bitnum)            ((val) |= (1 << (bitnum)))
/** Macro to clear the value of the corresponding bit number */
#define MS_CLR_BITNUM(val, bitnum)            ((val) &= (~(1 << (bitnum))))
/** \} */

/**
 * \name Comparison Macros
 * \{
 */
/** Macro to find Minimum value between 2 given entities */
#define MS_GET_MIN(a, b) \
        (((a) > (b)) ? (b) : (a))

/** Macro to find Maximum value between 2 given entities */
#define MS_GET_MAX(a, b) \
        (((a) > (b)) ? (a) : (b))
/** \} */

/**
 * \name Miscellaneous Macros
 * \{
 */
/** Unreferenced variable macro to avoid compilation warnings */
#define MS_IGNORE_UNUSED_PARAM(v) (void)(v)

/** Loop for ever */
#define MS_LOOP_FOREVER() for(;;)

/** Macro to reverse the provided Byte-Stream Endianness */
#define MS_STREAM_REV_ENDIANNESS(s, d, n) \
{ \
    UCHAR i; \
    for (i = 0; i < (n); i++) \
    { \
        (d)[(n - 1) - i] = (s)[i]; \
    } \
}
/** \} */

/**
 * \name Module State Tracking Macros
 * \{
 */
/** Define Global Mesh Module State */
#define MS_DEFINE_MODULE_STATE(module_name) UINT8 g_ms_##module_name = MS_MODULE_STATE_INVALID;

/** Declare Global Mesh Module State */
#define MS_DECLARE_MODULE_STATE(module_name) extern UINT8 g_ms_##module_name;

/** Set Global Mesh Module State */
#define MS_SET_MODULE_STATE(module_name, s) g_ms_##module_name = MS_MODULE_STATE_##s;

/** Check the Global Mesh Module State */
#define IF_MS_MODULE_STATE(module_name, s) if(g_ms_##module_name == MS_MODULE_STATE_##s)
/** \} */

/**
 * \name Model Handler Macros
 * \{
 */
#ifdef MS_HAVE_MODEL_OPCODE_EMPTY_HANDLERS
/** Macro to define an empty Mesh Model Opcode Handler */
#define MODEL_OPCODE_HANDLER_EMPTY_DEF(x) \
static API_RESULT (x) \
                  ( \
                      MS_ACCESS_MODEL_HANDLE * handle, \
                      MS_NET_ADDR              saddr, \
                      MS_NET_ADDR              daddr, \
                      MS_SUBNET_HANDLE         subnet_handle, \
                      MS_APPKEY_HANDLE         appkey_handle, \
                      UINT32                   opcode, \
                      UCHAR                  * data_param, \
                      UINT16                   data_len \
                  ) \
{ \
    API_RESULT retval; \
 \
    MS_IGNORE_UNUSED_PARAM(handle); \
    MS_IGNORE_UNUSED_PARAM(saddr); \
    MS_IGNORE_UNUSED_PARAM(daddr); \
    MS_IGNORE_UNUSED_PARAM(subnet_handle); \
    MS_IGNORE_UNUSED_PARAM(appkey_handle); \
    MS_IGNORE_UNUSED_PARAM(opcode); \
    MS_IGNORE_UNUSED_PARAM(data_param); \
    MS_IGNORE_UNUSED_PARAM(data_len); \
 \
    retval = API_SUCCESS; \
 \
    return retval; \
}

/** Macro to Invoke a Mesh Model Opcode Handler */
#define MODEL_OPCODE_HANDLER_CALL(handler) \
        (handler) (handle, saddr, daddr, subnet_handle, appkey_handle, opcode, data_param, data_len)
#else
/** Macro to define an empty Mesh Model Opcode Handler */
#define MODEL_OPCODE_HANDLER_EMPTY_DEF(x)

/** Macro to Invoke a Mesh Model Opcode Handler */
#define MODEL_OPCODE_HANDLER_CALL(handler)
#endif /* MS_HAVE_MODEL_OPCODE_EMPTY_HANDLERS */

/** \} */

/** \} */

/**
 * \addtogroup ms_common_constants Constants
 * \{
 */

/**
 * \name Module Identifier definitions.
 * \{
 * \brief Currently used for runtime debug enable/disable scenario.
 * In future, this can be used for other purposes as well,
 * hence these defines are placed under common header file.
 */
/** Page 4 - Bluetooth Protocol Modules */
#define MS_MODULE_PAGE_4                      0x40000000
/** \} */

/**
 * \name Module - Bit Mask
 * \{
 */
/** Module Bitmask for Common Module */
#define MS_MODULE_BIT_MASK_COMMON             0x00000001
/** Module Bitmask for Bearer Module */
#define MS_MODULE_BIT_MASK_BRR                0x00000002
/** Module Bitmask for Network Module */
#define MS_MODULE_BIT_MASK_NET                0x00000004
/** Module Bitmask for Lower Transport Module */
#define MS_MODULE_BIT_MASK_LTRN               0x00000008
/** Module Bitmask for Transport Module */
#define MS_MODULE_BIT_MASK_TRN                0x00000010
/** Module Bitmask for Access Module */
#define MS_MODULE_BIT_MASK_ACCESS             0x00000020
/** Module Bitmask for Application Module */
#define MS_MODULE_BIT_MASK_APP                0x00000040
/** Module Bitmask for Security Tool-Box Module */
#define MS_MODULE_BIT_MASK_STBX               0x00000080
/** Module Bitmask for Config Module */
#define MS_MODULE_BIT_MASK_CONFIG             0x00000100
/** Module Bitmask for FSM Module */
#define MS_MODULE_BIT_MASK_FSM                0x00000200
/** Module Bitmask for Provisioning Module */
#define MS_MODULE_BIT_MASK_PROV               0x00000400
/** Module Bitmask for Health Module */
#define MS_MODULE_BIT_MASK_HEALTH             0x00000800
/** Module Bitmask for Mesh Models Module */
#define MS_MODULE_BIT_MASK_MESH_MODEL         0x00001000
/** \} */

/**
 * \name Module - ID
 * \{
 */
/** Module ID for Common Module */
#define MS_MODULE_ID_COMMON                   (MS_MODULE_PAGE_4 | MS_MODULE_BIT_MASK_COMMON)
/** Module ID for Bearer Module */
#define MS_MODULE_ID_BRR                      (MS_MODULE_PAGE_4 | MS_MODULE_BIT_MASK_BRR)
/** Module ID for NEtwork Module */
#define MS_MODULE_ID_NET                      (MS_MODULE_PAGE_4 | MS_MODULE_BIT_MASK_NET)
/** Module ID for Lower Transport Module */
#define MS_MODULE_ID_LTRN                     (MS_MODULE_PAGE_4 | MS_MODULE_BIT_MASK_LTRN)
/** Module ID for Transport Module */
#define MS_MODULE_ID_TRN                      (MS_MODULE_PAGE_4 | MS_MODULE_BIT_MASK_TRN)
/** Module ID for Access Module */
#define MS_MODULE_ID_ACCESS                   (MS_MODULE_PAGE_4 | MS_MODULE_BIT_MASK_ACCESS)
/** Module ID for Application Module */
#define MS_MODULE_ID_APP                      (MS_MODULE_PAGE_4 | MS_MODULE_BIT_MASK_APP)
/** Module ID for Security Tool-Box Module */
#define MS_MODULE_ID_STBX                     (MS_MODULE_PAGE_4 | MS_MODULE_BIT_MASK_STBX)
/** Module ID for Config Module */
#define MS_MODULE_ID_CONFIG                   (MS_MODULE_PAGE_4 | MS_MODULE_BIT_MASK_CONFIG)
/** Module ID for FSM Module */
#define MS_MODULE_ID_FSM                      (MS_MODULE_PAGE_4 | MS_MODULE_BIT_MASK_FSM)
/** Module ID for Provisioning Module */
#define MS_MODULE_ID_PROV                     (MS_MODULE_PAGE_4 | MS_MODULE_BIT_MASK_PROV)
/** Module ID for Health Module */
#define MS_MODULE_ID_HEALTH                   (MS_MODULE_PAGE_4 | MS_MODULE_BIT_MASK_HEALTH)
/** Module ID for Mesh Models Module */
#define MS_MODULE_ID_MESH_MODEL               (MS_MODULE_PAGE_4 | MS_MODULE_BIT_MASK_MESH_MODEL)
/** \} */

/**
 * \name Beacon Types
 * \{
 */
/** Unprovisioned Device Beacon Type */
#define MS_BCON_TYPE_UNPRVSNG_DEV    0x00

/** Secure Network Beacon Type */
#define MS_BCON_TYPE_SECURE          0x01
/** \} */

/**
 * \name Friendship Roles
 * \{
 */
/** Friendship Role - Invalid */
#define MS_FRND_ROLE_INVALID   0x00

/** Friendship Role - Friend */
#define MS_FRND_ROLE_FRIEND    0x01

/** Friendship Role - Low Power Node */
#define MS_FRND_ROLE_LPN       0x02
/** \} */

/**
 * \name Mesh Features
 * \{
 */
/** Relay Feature */
#define MS_FEATURE_RELAY          0x00
/** Proxy Feature */
#define MS_FEATURE_PROXY          0x01
/** Friend Feature */
#define MS_FEATURE_FRIEND         0x02
/** Low Power Feature */
#define MS_FEATURE_LPN            0x03

/** Secure Network Beacon */
#define MS_FEATURE_SEC_NET_BEACON 0x04
/** \} */

/**
 * \name Mesh Operation States
 * \{
 */
/** Operation: Enable */
#define MS_ENABLE           0x01
/** Operation: Disable */
#define MS_DISABLE          0x00
/**
 * Feature not supported.
 * Used as status for Get/Set Friend/Proxy etc.,
 * when the feature is not supported.
 */
#define MS_NOT_SUPPORTED    0x02
/** \} */

/**
 * \name Mesh TX States
 * \{
 */
/** Network Tx State */
#define MS_NETWORK_TX_STATE            0x00
/** Relay Tx State */
#define MS_RELAY_TX_STATE              0x01
/** \} */

/**
 * \name Miscellaneous
 * \{
 */
/** Device UUID Size */
#define MS_DEVICE_UUID_SIZE    16

/** Label UUID Length. Associated with Virtual Address */
#define MS_LABEL_UUID_LENGTH           16

/** Beacon Type - Size */
#define MS_BCON_TYPE_SIZE      1

/** Beacon OOB Indicator Size */
#define MS_BCON_OOB_IND_SIZE   2

/** Beacon URI Hash Size */
#define MS_BCON_URI_HASH_SIZE   4
/** \} */

/** \} */

/* -------------------------------------------- Structures/Data Types */

/**
 * \defgroup ms_common_structures Structures
 * \{
 * \brief This section describes the various Data-Types and Structures in
 * EtherMind Mesh Common Layer Layer.
 */
/** Payload type */
typedef struct _MS_BUFFER
{
    /** Payload Pointer */
    UCHAR * payload;

    /** Payload Length */
    UINT16 length;

} MS_BUFFER;

/**
 *  Dynamic configuration of Mesh Data-structure.
 *  Used only if <b> "MS_HAVE_DYNAMIC_GLOBAL_ARRAY" </b> Compilation Feature
 *  Flag is defined.
 */
typedef struct _MS_CONFIG
{
#ifdef MS_HAVE_DYNAMIC_GLOBAL_ARRAY
    /** Number of Network Interfaces */
    UINT16 config_MS_NUM_NETWORK_INTERFACES;

    /** Number of Provisioning Interfaces */
    UINT16 config_MS_NUM_PROVISIONING_INTERFACES;

    /** Network Cache Size */
    UINT16 config_MS_NET_CACHE_SIZE;

#ifndef MS_NET_BASIC_SEQNUM_CACHE
    /** Network Sequence Number Cache Size */
    UINT16 config_MS_NET_SEQNUM_CACHE_SIZE;
#endif /* MS_NET_BASIC_SEQNUM_CACHE */

    /** Maximum number of subnets the device can store information about. */
    UINT16 config_MS_MAX_SUBNETS;

    /** Maximum number of device keys the device can store information about. */
    UINT16 config_MS_MAX_DEV_KEYS;

    /** Maximum number of addresses present in each proxy filter list. */
    UINT16 config_MS_PROXY_FILTER_LIST_SIZE;

    /** Maximum number of LPNs. */
    UINT16 config_MS_MAX_LPNS;

    /** The size of Reassembled Cache. */
    UINT16 config_MS_REASSEMBLED_CACHE_SIZE;

    /** Number of Segmentation and Reassembly contexts. */
    UINT16 config_LTRN_SAR_CTX_MAX;

    /**
     *  Maximum number of messages that the friend is capable to queue
     *  for a single Low Power Node.
     */
    UINT16 config_MS_FRIEND_MESSAGEQUEUE_SIZE;

    /**
     *  Maximum number of subscription addresses that the friend
     *  is capable to store for a single Low Power Node.
     */
    UINT16 config_MS_FRIEND_SUBSCRIPTION_LIST_SIZE;

    /** Maximum number of elements. */
    UINT16 config_MS_ACCESS_ELEMENT_COUNT;

    /** Maximum number of models. */
    UINT16 config_MS_ACCESS_MODEL_COUNT;

    /**
     *  Maximum number of Applications (keys) the device can store
     *  information about.
     */
    UINT16 config_MS_MAX_APPS;

    /**
     *  Maximum number of Virtual Addresses the device can store
     *  information about.
     */
    UINT16 config_MS_MAX_VIRTUAL_ADDRS;

    /**
     *  Maximum number of Non-Virtual Addresses the device can store
     *  information about.
     */
    UINT16 config_MS_MAX_NON_VIRTUAL_ADDRS;

    /**
     *  The distance between the network sequence numbers, for every persistent
     *  storage write. If the device is powered cycled, it will resume transmission
     *  using the sequence number from start of next block.
     */
    UINT16 config_MS_NET_SEQ_NUMBER_BLOCK_SIZE;

    /** Network Transmit Count for network packets - default value */
    UINT8 config_MS_NET_TX_COUNT;

    /** Network Interval Steps for network packets - default value */
    UINT8 config_MS_NET_TX_INTERVAL_STEPS;

    /** Network Transmit Count for relayed packets - default value */
    UINT8 config_MS_NET_RELAY_TX_COUNT;

    /** Network Interval Steps for relayed packets - default value */
    UINT8 config_MS_NET_RELAY_TX_INTERVAL_STEPS;

    /** Secure Network Beacon Interval - default value */
    UINT16 config_MS_CONFIG_SERVER_SNB_TIMEOUT;

    /**
     * Poxy ADV Network ID timeout for each Subnet in milliseconds.
     */
    UINT16 config_MS_PROXY_SUBNET_NETID_ADV_TIMEOUT;

    /**
     * Poxy ADV Node Identity timeout for each Subnet in milliseconds.
     */
    UINT16 config_MS_PROXY_SUBNET_NODEID_ADV_TIMEOUT;

    /**
     * Poxy ADV Node Identity overall time period in milliseconds.
     */
    UINT32 config_MS_PROXY_NODEID_ADV_TIMEOUT;

    /** Friend Poll Retry Count - default value */
    UINT16 config_MS_FRND_POLL_RETRY_COUNT;

    /** Lower Transport Segment Transmission Timeout in milliseconds */
    UINT16 config_MS_LTRN_RTX_TIMEOUT;

    /** Lower Transport Segment Transmission Count - default value */
    UINT8  config_MS_LTRN_RTX_COUNT;

    /** Lower Transport Acknowledgement Timeout in milliseconds */
    UINT16 config_MS_LTRN_ACK_TIMEOUT;

    /** Lower Transport Incomplete Timeout in milliseconds */
    UINT16 config_MS_LTRN_INCOMPLETE_TIMEOUT;

    /** Friendship Receive Window - default value */
    UINT8  config_MS_FRND_RECEIVE_WINDOW;

    /** Friend Clear Confirmation Timeout in milliseconds */
    UINT16 config_MS_LPN_CLEAR_RETRY_TIMEOUT_INITIAL;

    /** LPN Friend Clear Retry Count */
    UINT16 config_MS_LPN_CLEAR_RETRY_COUNT;

    /** Friendship Retry Timeout in milliseconds */
    UINT16 config_MS_TRN_FRNDREQ_RETRY_TIMEOUT;

    /** Unprovisioned Device Beacon Interleaved Beacon Timeout */
    UINT16 config_MS_UNPROV_DEVICE_BEACON_TIMEOUT;

    /**
     * Maximum number of messages that can be queued in Network layer
     * for Transmission.
     */
    UINT16 config_MS_NET_TX_QUEUE_SIZE;

    /** Maximum number of Transition Timers */
    UINT16 config_MS_MAX_NUM_TRANSITION_TIMERS;

    /** Maximum number of Periodic Step Timers */
    UINT16 config_MS_MAX_NUM_PERIODIC_STEP_TIMERS;

    /**
     *  Maximum number of Health Server Instances.
     */
    UINT16 config_MS_HEALTH_SERVER_MAX;

    /**
     *  Maximum number of Light LC Server Instances.
     */
    UINT16 config_MS_LIGHT_LC_SERVER_MAX;
#endif /* MS_HAVE_DYNAMIC_GLOBAL_ARRAY */

    /** The size of the Replay Protection cache. */
    UINT16 config_MS_REPLAY_CACHE_SIZE;

    /** Company ID */
    UINT16 config_MS_DEFAULT_COMPANY_ID;

    /** Product ID */
    UINT16 config_MS_DEFAULT_PID;

    /** Vendor ID */
    UINT16 config_MS_DEFAULT_VID;

} MS_CONFIG;

/** \} */

/** \} */

/* -------------------------------------------- Function/API Declarations */
#ifdef __cplusplus
extern "C"{
#endif

/**
 * \cond ignore_this Ignore this function while generating Doxygen document
 */
#ifdef MS_HAVE_DYNAMIC_CONFIG
/** Global Configuration for Mesh Stack */
extern MS_CONFIG ms_global_config;
#endif /* MS_HAVE_DYNAMIC_CONFIG */
/** \endcond */

/**
 * \defgroup ms_common_api API Definitions
 * \{
 * \brief This section describes the various APIs exposed by
 * EtherMind Mesh Common Layer Layer to the Application and other upper
 * layers of the stack.
 */

/**
 * \name Initialization and Shutdown Routines
 * \{
 */

/**
 *  \brief To initialize Mesh Stack.
 *
 *  \par Description
 *  API to initialize Mesh Stack. This is the first API that the
 *  application should call before any other API. This function
 *  initializes all the internal stack modules and data structures.
 *
 *  \param [in] blob
 *         If 'MS_HAVE_DYNAMIC_CONFIG' defined,
 *             application shall provide the desired dynamic configuration
 *             using a pointer to MS_CONFIG data structure instance.
 *         else,
 *             this parameter shall be NULL and ignored by the API.
 *
 *  \return None
 *
 *  \note Use \ref MS_init_ext to initialize Mesh Stack with return value.
 */
void MS_init
     (
         /* IN */ void * blob
     );

/**
 * API to turn off Bluetooth Hardware. This API should be called after
 * \ref MS_init.
 *
 * \return
 *      \ref API_RESULT on successful Bluetooth OFF
 */
API_RESULT MS_shutdown
           (
               void
           );
/** \} */

/**
 * \name Transition Timer
 * \{
 */

/**
 *  \brief To start transition timer.
 *
 *  \par Description
 *  API to start a transition timer.
 *
 *  \param [in] transition
 *         State Transition data structure, which includes the timeout,
 *         transition start and complete callback etc.
 *
 *  \param [out] transition_time_handle
 *         Transition Time Handle, which can be used to stop the transition
 *         timer if required.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_common_start_transition_timer
           (
               /* IN */  MS_ACCESS_STATE_TRANSITION_TYPE   * transition,
               /* OUT */ UINT16                            * transition_time_handle
           );

/**
 *  \brief To stop transition timer.
 *
 *  \par Description
 *  API to stop a transition timer.
 *
 *  \param [in] transition_time_handle
 *         Transition Time Handle, returned by the Start Transition Timer
 *         interface.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_common_stop_transition_timer
           (
               /* IN */ UINT16  transition_time_handle
           );

/**
 *  \brief To get remaining Transition Time.
 *
 *  \par Description
 *  API to get remaining Transition Time.
 *
 *  \param [in] transition_time_handle
 *         Transition Time Handle, returned by the Start Transition Timer
 *         interface.
 *
 *  \param [out] remaining_transition_time
 *         Remaining Transition Time.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_common_get_remaining_transition_time
           (
               /* IN */  UINT16   transition_time_handle,
               /* OUT */ UINT8  * remaining_transition_time
           );

/**
 *  \brief To get remaining Transition Time, with offset.
 *
 *  \par Description
 *  API to get remaining Transition Time with offset in ms.
 *
 *  \param [in] transition_time_handle
 *         Transition Time Handle, returned by the Start Transition Timer
 *         interface.
 *
 *  \param [in] offset_in_ms
 *         Offset in ms.
 *
 *  \param [out] remaining_transition_time
 *         Remaining Transition Time.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_common_get_remaining_transition_time_with_offset
           (
               /* IN */  UINT16   transition_time_handle,
               /* IN */  UINT32   offset_in_ms,
               /* OUT */ UINT8  * remaining_transition_time
           );

/**
 *  \brief To convert transition time from millisecond.
 *
 *  \par Description
 *  API to convert transition timer in millisecond to Generic Default
 *  Transition Time state format.
 *
 *  \param [in] transition_time_in_ms
 *         Transition Time in millisecond.
 *
 *  \param [out] transition_time
 *         Converted value in Generic Default Transition Time state format.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_common_get_transition_time_from_ms
           (
               /* IN */  UINT32 transition_time_in_ms,
               /* OUT */ UINT8 * transition_time
           );

/**
 *  \brief To convert transition time to millisecond.
 *
 *  \par Description
 *  API to convert Generic Default Transition Time state format to
 *  required transition time value in milliseconds.
 *
 *  \param [in] transition_time
 *         Transition time in Generic Default Transition Time state format.
 *
 *  \param [out] transition_time_in_ms
 *         Converted value of Transition Time in millisecond.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_common_get_transition_time_in_ms
           (
               /* IN */  UINT8 transition_time,
               /* OUT */ UINT32 * transition_time_in_ms
           );

/**
 * \cond ignore_this Ignore this function while generating Doxygen document
 */

/** Internal Function for Transition Timer Initialization */
API_RESULT ms_common_init_transition_timer(void);

/**
 * \endcond
 */

/** \} */

/** \} */

#ifdef __cplusplus
};
#endif

/** \} */

#endif /* _H_MS_COMMON_ */

