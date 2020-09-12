
/**
 *  \file EM_os.h
 *
 *  This header file is part of EtherMind OS Abstraction module.
 *  In this file the platform specific data types are abstracted and
 *  mapped to data types used within the EtherMind Stack.
 *
 *  Version: Windows User Mode
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_EM_OS_
#define _H_EM_OS_

/* -------------------------------------------- Header File Inclusion */
/* EtherMind Features */
/* #include "EM_features.h" */

#include "EM_platform.h"

/* -------------------------------------------- Global Definitions */

/*
 *  EM_HAVE_STATIC_DECL
 *
 *  This flag enables provision for declaring functions and/or globals in
 *  a file as 'static'.
 */
#define EM_HAVE_STATIC_DECL

/*
 *  EM_HAVE_CONST_DECL
 *
 *  This flag enables provision for declaring globals in a file as 'const'.
 */
#define EM_HAVE_CONST_DECL

/** defining DECL_CONST_QUALIFIER. */
#define DECL_CONST_QUALIFIER

/* Definitions of EM_SUCCESS & EM_FAILURE */
#define EM_SUCCESS                    0x0000
#define EM_FAILURE                    0xFFFF

/**
 * Defining the Below items from bt_types.h of BlueLitE.
 * TODO: Map these to exposed data-types from RX23W SDK.
 */
#ifndef IN
#define IN              /*@in@*/
#endif
#ifndef OUT
#define OUT             /*@out@*/
#endif
#ifndef INOUT
#define INOUT           IN OUT
#endif

/** Declaring Compilation Specifier DECL_STATIC used with 'static' variable/functions. */
#ifdef EM_HAVE_STATIC_DECL
#define DECL_STATIC                     static
#else  /* EM_HAVE_STATIC_DECL */
#define DECL_STATIC
#endif /* EM_HAVE_STATIC_DECL */

/* Declaration of 'const' variable */
/** Declaring Compilation Specifier DECL_CONST used with 'const' variable. */
#ifdef EM_HAVE_CONST_DECL
#define DECL_CONST                      const
#else  /* EM_HAVE_CONST_DECL */
#define DECL_CONST
#endif /* EM_HAVE_CONST_DECL */

/* Reentrant Function Declaration */
/** Declaring Compilation Specifier DECL_REENTRANT used with 'reentrant' functions. */
#ifdef EM_HAVE_REENTRANT_DECL
#define DECL_REENTRANT          reentrant
#else /* EM_HAVE_REENTRANT_DECL */
#define DECL_REENTRANT
#endif /* EM_HAVE_REENTRANT_DECL */

typedef unsigned char        BOOLEAN;
typedef unsigned char          UINT8;
typedef unsigned char          UCHAR;
typedef signed char             INT8;
typedef signed char             CHAR;

typedef unsigned short        UINT16;
typedef signed short           INT16;
typedef unsigned long int     UINT32;
typedef signed long int        INT32;
typedef signed long long       INT64;
typedef unsigned long long    UINT64;


/* -------------------------------------------- Structures/Data Types */
/* Function Return Value type */
typedef UINT16    EM_RESULT;

typedef INT32     EM_thread_type;

typedef void * EM_THREAD_RETURN_TYPE;
typedef void * EM_THREAD_ARGS;
typedef EM_THREAD_RETURN_TYPE (*EM_THREAD_START_ROUTINE)(EM_THREAD_ARGS);

#define EM_THREAD_RETURN_VAL_SUCCESS NULL
#define EM_THREAD_RETURN_VAL_FAILURE NULL
/* --------------------------------------------------- Macros */
/* Abstractions for String library functions */
#define EM_str_len(s)                 strlen((char *)(s))
#define EM_str_copy(d, s)             strcpy((char *)(d), (char *)(s))
#define EM_str_n_copy(d, s, n)        strncpy((char *)(d), (char *)(s), n)
#define EM_str_cmp(s1, s2)            strcmp((char *)(s1), (char *)(s2))
#define EM_str_n_cmp(s1, s2, n)       strncmp((char *)(s1), (char *)(s2), n)
#define EM_str_cat(d, s)              strcat((char *)(d), (char *)(s))
#define EM_str_str(s, ss)             strstr((char *)(s), (char *)(ss))
#define EM_str_n_casecmp(s1, s2, n)   _strnicmp ((char *)(s1), (char *)(s2), n)

/* Abstractions for memory functions */
#define EM_alloc_mem(s)               malloc((size_t)(s))
#define EM_alloc_mem_ext(s)           mempool_alloc_pl((size_t)(s))
#define EM_free_mem(p)                free(p)
#define EM_mem_move(d, s, n)          memmove((d), (s), (n))
#define EM_mem_cmp(p1, p2, n)         memcmp((p1), (p2), (n))
#define EM_mem_set(p, v, n)           memset((p), (v), (n))
#define EM_mem_copy(p1, p2, n)        memcpy((p1), (p2), (n))

/* -------------------------------------------- Function Declarations */
#ifdef __cplusplus
extern "C" {
#endif

/** Initialize the OS */
#define EM_os_init(...)

/* Task/Thread Creation Primitives */
#define EM_thread_create            BT_thread_create
#define EM_thread_attr_init         BT_thread_attr_init

/* Task/Thread Synchronization Primitives */
#define EM_thread_mutex_init        BT_thread_mutex_init
#define EM_thread_mutex_lock        BT_thread_mutex_lock
#define EM_thread_mutex_unlock      BT_thread_mutex_unlock
#define EM_thread_cond_init         BT_thread_cond_init
#define EM_thread_cond_wait         BT_thread_cond_wait
#define EM_thread_cond_signal       BT_thread_cond_signal

#define EM_thread_mutex_type        BT_thread_mutex_type
/* Task/Thread Delay Primitives */
#define EM_sleep(...)
#define EM_usleep(...)
#define EM_get_current_time(ct)     (*ct) = 0

#define EM_process_term_notify(...)

#ifdef __cplusplus
};
#endif

#endif /* _H_EM_OS_ */

