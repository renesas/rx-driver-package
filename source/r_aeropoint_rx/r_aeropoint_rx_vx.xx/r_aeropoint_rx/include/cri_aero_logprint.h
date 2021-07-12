/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 - 2019 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_logprint.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_LOGPRINT_H
#define CRI_AERO_LOGPRINT_H

#include <stdio.h>
#include <stdarg.h>

#include "cri_xpt.h"


#ifdef __cplusplus
extern "C" {
#endif

CRIAPI void CriAero_LogPrint(const char *fmt, ...);

CRIAPI void CriAero_WarningPrint(const char *fmt, ...);

CRIAPI void CriAero_ErrorPrint(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif	

