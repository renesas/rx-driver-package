
/**
 *  \file EM_timer.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_EM_TIMER_
#define _H_EM_TIMER_

/* --------------------------------------------- Header File Inclusion */

/* --------------------------------------------- Global Definitions */
#define EM_TIMER_MAX_ENTITIES                       10

/* Mask to indicate millisecond timeout */
#define EM_TIMEOUT_MILLISEC                         0x80000000

/* Timer Handles must be initialized to this value */
/* #define EM_TIMER_HANDLE_INIT_VAL                    NULL */

/* Flag: Timer Entity State */
#define TIMER_ENTITY_FREE                           0x00
#define TIMER_ENTITY_IN_USE                         0x01
#define TIMER_ENTITY_IN_FREE                        0x02

/* Flag: Timer Entity Data to be freed or not */
#define TIMER_ENTITY_HOLD_ALLOC_DATA                0x00
#define TIMER_ENTITY_FREE_ALLOC_DATA                0x01

/* Timer module ID and Error codes */
#define EM_TIMER_ERR_ID                            0xC000

#define EM_TIMER_MUTEX_INIT_FAILED                 (0x0001 | EM_TIMER_ERR_ID)
#define EM_TIMER_COND_INIT_FAILED                  (0x0002 | EM_TIMER_ERR_ID)
#define EM_TIMER_MUTEX_LOCK_FAILED                 (0x0003 | EM_TIMER_ERR_ID)
#define EM_TIMER_MUTEX_UNLOCK_FAILED               (0x0004 | EM_TIMER_ERR_ID)
#define EM_TIMER_MEMORY_ALLOCATION_FAILED          (0x0005 | EM_TIMER_ERR_ID)

#define EM_TIMER_HANDLE_IS_NULL                    (0x0011 | EM_TIMER_ERR_ID)
#define EM_TIMER_CALLBACK_IS_NULL                  (0x0012 | EM_TIMER_ERR_ID)
#define EM_TIMER_QUEUE_EMPTY                       (0x0013 | EM_TIMER_ERR_ID)
#define EM_TIMER_QUEUE_FULL                        (0x0014 | EM_TIMER_ERR_ID)
#define EM_TIMER_ENTITY_SEARCH_FAILED              (0x0015 | EM_TIMER_ERR_ID)
#define EM_TIMER_NULL_PARAMETER_NOT_ALLOWED        (0x0016 | EM_TIMER_ERR_ID)
#define EM_TIMER_TIMEOUT_ZERO_NOT_ALLOWED          (0x0017 | EM_TIMER_ERR_ID)
#define EM_TIMER_FAILED_SET_TIME_EVENT             (0x0018 | EM_TIMER_ERR_ID)

/* --------------------------------------------- Structures/Data Types */
typedef UCHAR EM_timer_handle;
typedef UINT32 EM_time_type;

#define EM_TIMER_HANDLE_INIT_VAL                   (0xFF)
/* --------------------------------------------- Macros */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

/* Timer Related */
#define EM_start_timer         BT_start_timer
#define EM_restart_timer       BT_restart_timer
#define EM_stop_timer          BT_stop_timer
#define EM_is_active_timer     BT_is_active_timer

/* Empty Functions */
#define EM_timer_init(...)
#define timer_em_init(...)

/* -------------------------------------------- Global Definitions */

#define USE_BT_PL_TIMER
#define SUPPORT_32_BIT_MS_TIMER
#define BIT15           0x8000
#define BIT31       0x80000000

/* ----------------------------------------------- Structures/Data Types */

#ifdef USE_BT_PL_TIMER
/* Index of the timer table */
typedef UCHAR BT_TIMER_ID;

/* In milliseconds */
typedef UINT32 BT_TIME_TYPE;
#endif /* USE_BT_PL_TIMER */

#ifdef SUPPORT_32_BIT_MS_TIMER
#define TIMER_VAL_UNIT_MASK BIT31
typedef UINT32 BT_timer_type;
#else /*SUPPORT_32_BIT_MS_TIMER */
#define TIMER_VAL_UNIT_MASK BIT15
typedef UINT16 BT_timer_type;
#endif  /*SUPPORT_32_BIT_MS_TIMER */

#ifdef USE_BT_PL_TIMER
typedef BT_TIMER_ID BT_timer_handle;
#else
typedef TIMER_ID BT_timer_handle;
#endif /* USE_BT_PL_TIMER */

/* --------------------------------------------------- Function Declarations */
/**
 *  \fn BT_start_timer
 *
 *  \brief Start a timer.
 *
 *  \par Description:
 *  This API starts a timer.
 *
 *  \param handle(OUT)
 *         Handle of the started timer
 *  \param timeout (IN)
 *         Timeout for which timer to be started.
 *         (If MSB of timeout is set, it is treated as milliseconds
 *          otherwise seconds).
 *  \param callback (IN)
 *         Callback function to be called on timeout.
 *  \param args (IN)
 *         Argument to be passed to callback function.
 *  \param size_args (IN)
 *         Size of "args".
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT BT_start_timer
           (
               /* OUT */ BT_timer_handle * handle,
               /* IN */  BT_timer_type   timeout,
               /* IN */  void           (* callback) (void *, UINT16 ),
               /* IN */  void            * args,
               /* IN */  UINT16            size_args
           );


/**
 *  \fn BT_stop_timer
 *
 *  \brief Stops a timer.
 *
 *  \par Description:
 *  This API stops a timer.
 *
 *  \param handle(IN)
 *         Handle of the timer to be stopped.
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT BT_stop_timer (BT_timer_handle handle);

/**
 *  \fn BT_timer_get_remaining_time
 *
 *  \brief Returns a remained time of the timer specified by handle.
 *
 *  \par Description:
 *  This API returns a remained time of the timer specified by handle.
 *
 *  \param handle(IN)
 *         Handle of the timer.
 *
 *  \param remain_time(OUT)
 *         Remaining timer of the timer.
 *
 *  \return API_RESULT:
 *          API_SUCCESS on success, or, an Error Code (see BT_error.h)
 *          describing the cause of failure.
 */
API_RESULT BT_timer_get_remaining_time
           (
               /* IN */  BT_timer_handle handle,
               /* OUT */ BT_timer_type   * remain_time
           );


#endif /* _H_EM_TIMER_ */
