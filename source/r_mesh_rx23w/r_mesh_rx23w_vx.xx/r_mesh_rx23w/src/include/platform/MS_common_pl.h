
/**
 *  \file MS_common_pl.h
 *
 *  This file contains the Function Declaration, and Constant Definitions
 */

/*
 *  Copyright (C) 2016. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_MS_COMMON_PL_
#define _H_MS_COMMON_PL_

/* ------------------------------------------- Header File Inclusion */


/* ------------------------------------------- Global Definitions/Macros */

/* ------------------------------------------- Data Structures */


/* ------------------------------------------- Function Declarations */
/**
 * \addtogroup ms_common_api
 * \{
 */

/**
 *  \brief To initialize Mesh Stack with return value.
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
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_init_ext
     (
         /* IN */ void * blob
     );

/**
 * \name Randomized Number Generation
 * \{
 */

/**
 *  \brief Generate 32bits-length Randomized Number.
 *
 *  \par Description
 *  API to generate 32bits-length Randomized number. This function returns value within the range from 0 to (max-1).
 *
 *  \return 32bits-length Randomized Number within the range from 0 to (max-1)
 */
UINT32 MS_rand_u32_pl(UINT32 max);

/**
 *  \brief Generate variable-length Randomized Number.
 *
 *  \par Description
 *  API to generate variable-length Randomized number.
 *
 *  \return API_SUCCESS or an error code indicating reason for failure
 */
API_RESULT MS_rand_bytes_pl(UCHAR * rand, UINT16 randlen);

/** \} */

/** \} */

#endif /* _H_MS_COMMON_PL_ */

