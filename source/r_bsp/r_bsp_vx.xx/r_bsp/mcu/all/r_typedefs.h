/*
* Copyright (c) 2011 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : r_typedefs.h
* Description  : This has fixed-width integer and bool support for compilers that are not C99 compliant and do not have
*                the header files stdint.h, stdbool.h, and stddef.h.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 18.09.2012 1.00     First Release
*         : 21.06.2013 1.10     Added NULL and size_t support.
*         : 22.11.2013 1.11     Changed size_t to 'unsigned long' from 'unsigned int'.
*         : 28.02.2019 1.12     Fixed coding style.
*         : 26.02.2025 1.13     Changed the disclaimer.
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef R_TYPEDEFS_H
#define R_TYPEDEFS_H

#define bool  (_Bool)
#define false (0)
#define true  (1)
#define NULL  (0)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed long int32_t;
typedef unsigned long uint32_t;
typedef signed long long int64_t;
typedef unsigned long long uint64_t;
typedef unsigned int _Bool;
typedef unsigned long size_t;

#endif  /* End of multiple inclusion prevention macro */

