
/**
 *  \file MS_common_pl.h
 *
 *  This file contains the Function Declaration, and Constant Definitions
 *
 */

/*
 *  Copyright (C) 2016. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_MS_COMMON_PL_
#define _H_MS_COMMON_PL_

/* ------------------------------------------- Header File Inclusion */


/* ------------------------------------------- Common PL Debug */


/* ------------------------------------------- Global Definitions/Macros */
#define MS_rand_u32_pl          ms_rand_pl
#define MS_rand_bytes_pl        cry_rand_generate

/* ------------------------------------------- Data Structures */


/* ------------------------------------------- Function Declarations */
/* Interface to get random number in the range [0, max-1] */
UINT32 ms_rand_pl(UINT32 max);

extern INT32 cry_rand_generate(UCHAR * prand, UINT16 randlen);

#endif /* _H_MS_COMMON_PL_ */

