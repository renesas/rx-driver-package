
/**
 *  \file MS_systemtime_pl.h
 *
 * Platform specific header file for systemtime module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_MS_SYSTEMTIME_PL_
#define _H_MS_SYSTEMTIME_PL_

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "MS_common.h"

/*******************************************************************************
* Prototype declarations
*******************************************************************************/
/** \cond definitions for internal module */
UINT32 ms_systemtime_read_pl(void);
/** \endcond */

/*******************************************************************************
* API declarations
*******************************************************************************/
/**
 * \addtogroup ms_common_api
 * \{
 */

/**
 *  \brief Initialize System Time of Mesh Stack.
 *
 *  \par Description
 *  API to register Platform System Time Read Function with Mesh Stack.
 *  Register the read function that returns 32bit system time in units of milliseconds.
 *
 *  \param [in] read_func   platform system time read functions
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_systemtime_init_pl(UINT32 (*read_func)(void));

/** \} */

#endif /* _H_MS_SYSTEMTIME_PL_ */
