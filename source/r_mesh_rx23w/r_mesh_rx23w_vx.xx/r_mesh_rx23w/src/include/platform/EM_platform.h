
/**
 *  \file EM_platform.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_EM_PLATFORM_
#define _H_EM_PLATFORM_

/* --------------------------------------------- Header File Inclusion */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "EM_os.h"
#include "MS_common_pl.h"
#include "MS_mempool_pl.h"
#include "MS_storage_pl.h"
#include "MS_systemtime_pl.h"
#include "MS_monitor_pl.h"
#include "MS_logger.h"
#include "MS_prov_pl.h"

/* --------------------------------------------- Global Definitions */
//#define BLE_LOG_TAG "mesh"
//#include "logger/r_ble_logger.h"

#if 1
#define printf(...)
#define cli_printf(...)
#else
#define printf(...)             console_putsf(__VA_ARGS__)
#define cli_printf(...)         console_putsf(__VA_ARGS__)
#endif

extern void console_putsf(const char *p_format, ...);

/* #define printf(...)     printf (__VA_ARGS__); printf("\r\n"); fflush(stdout); */

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */
/**
 * NOTE: These 2 below defines can be mapped to platform specific
 *       function calls, which can be used for a LPN implementation.
 */
#define EM_enter_sleep_pl(...)
#define EM_exit_sleep_pl(...)

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- External Functions */
extern void r_ble_rf_control_error(uint32_t err_no);

/* --------------------------------------------- API Declarations */
#define EM_PLATFORM_REBOOT()    r_ble_rf_control_error(0x00000000)

#endif /* _H_EM_PLATFORM_ */

