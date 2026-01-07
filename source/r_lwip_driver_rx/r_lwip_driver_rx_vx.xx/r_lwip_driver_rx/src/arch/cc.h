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
#include <string.h>

#if (NO_SYS == 1)
typedef int sys_prot_t;
#endif

#define LWIP_ERRNO_STDINCLUDE

/* define compiler specific symbols */
#if defined (__GNUC__)
#define LWIP_TIMEVAL_PRIVATE    0
#include <sys/time.h>
#include <sys/types.h>

#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT      __attribute__ ((__packed__))
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x)    x

#define SSIZE_MAX               INT_MAX
#define LWIP_NO_UNISTD_H        1

/* errno */
#define ENOTBLK                 (15)

#define EDEADLOCK               (160)

#define ECHRNG                  (44)
#define EL2NSYNC                (161)
#define EL3HLT                  (162)
#define EL3RST                  (47)
#define ELNRNG                  (48)
#define EUNATCH                 (49)
#define ENOCSI                  (50)
#define EL2HLT                  (51)
#define EBADE                   (52)
#define EBADR                   (53)
#define EXFULL                  (54)
#define ENOANO                  (55)
#define EBADRQC                 (56)
#define EBADSLT                 (57)
#define EBFONT                  (59)

#define ENONET                  (64)
#define ENOPKG                  (65)
#define EREMOTE                 (66)

#define EADV                    (68)
#define ESRMNT                  (69)
#define ECOMM                   (70)

#define EDOTDOT                 (73)

#define ENOTUNIQ                (76)
#define EBADFD                  (163)
#define EREMCHG                 (78)
#define ELIBACC                 (79)
#define ELIBBAD                 (80)
#define ELIBSCN                 (81)
#define ELIBMAX                 (82)
#define ELIBEXEC                (83)

#define ERESTART                (85)
#define ESTRPIPE                (86)
#define EUSERS                  (87)

#define ESOCKTNOSUPPORT         (94)

#define ESHUTDOWN               (164)

#define EUCLEAN                 (165)
#define ENOTNAM                 (166)
#define ENAVAIL                 (167)
#define EISNAM                  (168)
#define EREMOTEIO               (169)

#define ENOMEDIUM               (170)
#define EMEDIUMTYPE             (124)

#elif defined (__CCRX__)
#define PACK_STRUCT_BEGIN       _Pragma("pack")
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END         _Pragma("unpack")
#define PACK_STRUCT_FIELD(x)    x

#if (__BIG == 1)
#define BYTE_ORDER BIG_ENDIAN
#else /* __BIG == 1 */
#define BYTE_ORDER LITTLE_ENDIAN
#endif /* __BIG == 1 */

/* errno */
#define EWOULDBLOCK             (EAGAIN)

#define ENOTBLK                 (15)

#define ETXTBSY                 (26)

#define EDEADLOCK               (35)

#define ELOOP                   (40)
#define ENOMSG                  (42)
#define EIDRM                   (43)
#define ECHRNG                  (44)
#define EL2NSYNC                (160)
#define EL3HLT                  (161)
#define EL3RST                  (47)
#define ELNRNG                  (48)
#define EUNATCH                 (49)
#define ENOCSI                  (50)
#define EL2HLT                  (51)
#define EBADE                   (52)
#define EBADR                   (53)
#define EXFULL                  (54)
#define ENOANO                  (55)
#define EBADRQC                 (56)
#define EBADSLT                 (57)
#define EBFONT                  (59)
#define ENOSTR                  (60)
#define ENODATA                 (61)
#define ETIME                   (62)
#define ENOSR                   (63)
#define ENONET                  (64)
#define ENOPKG                  (65)
#define EREMOTE                 (66)
#define ENOLINK                 (67)
#define EADV                    (68)
#define ESRMNT                  (69)
#define ECOMM                   (70)
#define EPROTO                  (71)
#define EMULTIHOP               (72)
#define EDOTDOT                 (73)

#define EOVERFLOW               (75)
#define ENOTUNIQ                (76)
#define EBADFD                  (162)
#define EREMCHG                 (163)
#define ELIBACC                 (79)
#define ELIBBAD                 (80)
#define ELIBSCN                 (81)
#define ELIBMAX                 (82)
#define ELIBEXEC                (83)

#define ERESTART                (85)
#define ESTRPIPE                (86)
#define EUSERS                  (87)
#define ENOTSOCK                (164)
#define EDESTADDRREQ            (165)

#define EPROTOTYPE              (91)
#define ENOPROTOOPT             (92)
#define EPROTONOSUPPORT         (166)
#define ESOCKTNOSUPPORT         (94)
#define EOPNOTSUPP              (95)
#define EPFNOSUPPORT            (96)
#define EAFNOSUPPORT            (167)
#define EADDRINUSE              (98)
#define EADDRNOTAVAIL           (99)
#define ENETDOWN                (100)
#define ENETUNREACH             (101)
#define ENETRESET               (102)
#define ECONNABORTED            (103)
#define ECONNRESET              (104)
#define ENOBUFS                 (105)
#define EISCONN                 (106)
#define ENOTCONN                (107)
#define ESHUTDOWN               (108)
#define ETOOMANYREFS            (109)

#define ECONNREFUSED            (111)
#define EHOSTDOWN               (112)
#define EHOSTUNREACH            (113)
#define EALREADY                (114)

#define ESTALE                  (116)
#define EUCLEAN                 (117)
#define ENOTNAM                 (118)
#define ENAVAIL                 (119)
#define EISNAM                  (120)
#define EREMOTEIO               (121)
#define EDQUOT                  (122)
#define ENOMEDIUM               (123)
#define EMEDIUMTYPE             (124)

#endif /* __GNUC__ */

#define LWIP_PLATFORM_ASSERT(x) do {printf("Assertion \"%s\" failed at line %d in %s\n", \
                                     x, __LINE__, __FILE__); } while(0)

/* Define random number generator function */
uint32_t r_lwip_driver_get_rand(void);
#define LWIP_RAND()             ((uint32_t)r_lwip_driver_get_rand())

#endif /* __CC_H__ */
