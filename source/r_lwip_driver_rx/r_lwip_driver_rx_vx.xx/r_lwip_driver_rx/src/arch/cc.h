/**********************************************************************************************************************
 * Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *********************************************************************************************************************/

#ifndef __CC_H__
#define __CC_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef int sys_prot_t;

#if defined (__GNUC__)
#define LWIP_TIMEVAL_PRIVATE    0
#include <sys/time.h>
#include <sys/types.h>

#endif

#include <string.h>
//#define BYTE_ORDER  LITTLE_ENDIAN

/* define compiler specific symbols */
#if defined (__GNUC__)
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT      __attribute__ ((__packed__))
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x)    x

#define SSIZE_MAX               INT_MAX
#define LWIP_NO_UNISTD_H        1

#elif defined (__CCRX__)
#define PACK_STRUCT_BEGIN       _Pragma("pack")
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END         _Pragma("unpack")
#define PACK_STRUCT_FIELD(x)    x

#endif

#define LWIP_PLATFORM_ASSERT(x) do {printf("Assertion \"%s\" failed at line %d in %s\n", \
                                     x, __LINE__, __FILE__); } while(0)

/* Define random number generator function */
uint32_t r_lwip_driver_get_rand(void);
#define LWIP_RAND()             ((uint32_t)r_lwip_driver_get_rand())

#endif /* __CC_H__ */
