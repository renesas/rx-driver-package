
/**
 *  \file EM_os.h
 *
 *  This header file is part of EtherMind OS Abstraction module.
 *  In this file the platform specific data types are abstracted and
 *  mapped to data types used within the EtherMind Stack.
 *
 *  Version: Mint_OS
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_EM_OS_
#define _H_EM_OS_

/* -------------------------------------------- Header File Inclusion */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* -------------------------------------------- Structures/Data Types */
/** Commonly used Data Types */
/* 'unsigned' datatype of size '1 octet' */
typedef unsigned char BOOLEAN;
/* 'unsigned' datatype of size '1 octet' */
typedef unsigned char UINT8;
/* 'unsigned' datatype of size '1 octet' */
typedef unsigned char UCHAR;
/* 'signed' datatype of size '1 octet' */
typedef signed char INT8;
/* 'signed' datatype of size '1 octet' */
typedef char CHAR;

/* 'unsigned' datatype of size '2 octet' */
typedef unsigned short int UINT16;
/* 'signed' datatype of size '2 octet' */
typedef signed short int INT16;
/* 'unsigned' datatype of size '4 octet' */
typedef unsigned long int  UINT32;
/* 'signed' datatype of size '4 octet' */
typedef signed long int INT32;
/* 'unsigned' datatype of size '8 octet' */
typedef unsigned long long UINT64;
/* 'signed' datatype of size '8 octet' */
typedef signed long long INT64;

/* Function Return Value type */
typedef UINT16 EM_RESULT;

/*
 * Generic handle can used for the unknow
 * datatypes (e.g. Mutex Type, Conditional Type etc.)
 */
typedef void * EM_GENERIC_HANDLE;

/* Datatype to represent File Handle */
typedef EM_GENERIC_HANDLE EM_FILE_HANDLE;

/* Datatype to represent Thread or Task Identifier */
typedef EM_GENERIC_HANDLE EM_thread_type;

/* Datatype to represent Thread or Task Attribute */
typedef EM_GENERIC_HANDLE EM_thread_attr_type;

/* Datatype to represent Mutex object */
typedef EM_GENERIC_HANDLE EM_thread_mutex_type;

/* Datatype to represent Attributes of a Mutex object */
typedef EM_GENERIC_HANDLE EM_thread_mutex_attr_type;

/* Datatype to represent Conditional Variable object */
typedef EM_GENERIC_HANDLE EM_thread_cond_type;

/* Datatype to represent Attributes of a Conditional Variable object */
typedef EM_GENERIC_HANDLE EM_thread_cond_attr_type;

/* Datatype to represent the OS time for the platform */
typedef int EM_time_type;

/* Data types for task parameters and retun types */
typedef EM_GENERIC_HANDLE EM_THREAD_RETURN_TYPE;
typedef EM_GENERIC_HANDLE EM_THREAD_ARGS;
typedef EM_THREAD_RETURN_TYPE (*EM_THREAD_START_ROUTINE)(EM_THREAD_ARGS);

/** Structure to have the local time */
typedef struct _EM_LOCAL_TIME
{
    UINT16  dyear;
    UINT16  dmonth;
    UINT16  dday;
    UINT16  dwday;

    UINT16  thour;
    UINT16  tmin;
    UINT16  tsec;
    UINT16  tmsec;

} EM_LOCAL_TIME;

/* --------------------------------------------------- Macros */
/**
 * Abstractions for a Thread's return values.
 * Generally once a thread is started it is never killed.
 * So, these defines are rarely used.
 */
#define EM_THREAD_RETURN_VAL_SUCCESS NULL
#define EM_THREAD_RETURN_VAL_FAILURE NULL

/* Abstractions for String library functions */
#define EM_str_len(s)                 strlen((char *)(s))
#define EM_str_n_len(s, sz)           strnlen((char *)(s), (sz))
#define EM_str_copy(d, s)             strcpy((char *)(d), (char *)(s))
#define EM_str_n_copy(d, s, n)        strncpy((char *)(d), (char *)(s), n)
#define EM_str_cmp(s1, s2)            strcmp((char *)(s1), (char *)(s2))
#define EM_str_n_cmp(s1, s2, n)       strncmp((char *)(s1), (char *)(s2), n)
#define EM_str_cat(d, s)              strcat((char *)(d), (char *)(s))
#define EM_str_n_cat(d, s, sz)        strncat((char *)(d), (char *)(s), (sz))
#define EM_str_str(s, ss)             strstr((char *)(s), (char *)(ss))
#define EM_str_n_casecmp(s1, s2, n)   _strnicmp ((char *)(s1), (char *)(s2), n)

/* Abstractions for memory functions */
#define EM_mem_move(d, s, n)          memmove((d), (s), (n))
#define EM_mem_cmp(p1, p2, n)         memcmp((p1), (p2), (n))
#define EM_mem_set(p, v, n)           memset((p), (v),(size_t) (n))
#define EM_mem_copy(p1, p2, n)        memcpy((p1), (p2), (n))

/**
 * NOTE:
 * Currently this Extended Memory Allocation is used only for the Mesh
 * Module Dynamic Configuration which is provided by the Upper Layer.
 * The Upper Layer provides the configuration limits for various Mesh
 * Layer entities and it also provides the Memory Block (abstracted by the
 * platform) required for the dynamic allocation for these entities.
 *
 * 1. The Extended Memory Allocation is mapped to Platform Memory Allocation.
 * 2. The Extended Memory Free is currently mapped to Empty
 */
#define EM_alloc_mem_ext(s)           ms_mempool_alloc_pl((size_t)(s))
#define EM_free_mem_ext(ptr)          ms_mempool_free_pl((ptr))

/* -------------------------------------------- Function Declarations */
#ifdef __cplusplus
extern "C" {
#endif

/** Initialize the OS */
void EM_os_init(void);

/* Task/Thread Creation Primitives */
INT32 EM_thread_create
      (
          /* OUT */ EM_thread_type *         thread,
          /* IN */  EM_thread_attr_type *    thread_attr,
          /* IN */  EM_THREAD_START_ROUTINE  start_routine,
          /* IN */  EM_THREAD_ARGS           thread_args
      );

INT32 EM_thread_attr_init
      (
          /* OUT */ EM_thread_attr_type *    thread_attr
      );

/* Task/Thread Synchronization Primitives */
INT32 EM_thread_mutex_init
      (
          /* OUT */ EM_thread_mutex_type *         mutex,
          /* IN */  EM_thread_mutex_attr_type *    mutex_attr
      );

INT32 EM_thread_mutex_lock
      (
          /* INOUT */ EM_thread_mutex_type *    mutex
      );

INT32 EM_thread_mutex_unlock
      (
          /* INOUT */ EM_thread_mutex_type *    mutex
      );

INT32 EM_thread_cond_init
      (
          /* OUT */ EM_thread_cond_type *         cond,
          /* IN */  EM_thread_cond_attr_type *    cond_attr
      );

INT32 EM_thread_cond_wait
      (
          /* INOUT */ EM_thread_cond_type *     cond,
          /* INOUT */ EM_thread_mutex_type *    cond_mutex
      );

INT32 EM_thread_cond_signal
      (
          /* INOUT */ EM_thread_cond_type *    cond
      );

/* Memory Management Primitives */
void * EM_alloc_mem (/* IN */ UINT32 size);
void EM_free_mem (/* IN */ void * ptr);

/* Task/Thread Delay Primitives */
void EM_sleep ( /* IN */ UINT32 tm );
void EM_usleep ( /* IN */ UINT32 tm );
void EM_get_current_time (/* OUT */ EM_time_type * curtime);
void EM_get_local_time( /* OUT */ EM_LOCAL_TIME *local);
INT32 EM_get_time_ms(void);
UINT64 EM_get_us_timestamp(void);

/* Process termination handling */
void EM_process_term_notify(void(*handler)(void));

#ifdef __cplusplus
};
#endif

#endif /* _H_EM_OS_ */

