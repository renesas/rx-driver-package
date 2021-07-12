/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 - 2019 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_error.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_ERROR_H
#define CRI_AERO_ERROR_H


#include "cri_xpt.h"


#define CRI_AERO_ERROR_NOERROR						(0)
#define CRI_AERO_ERROR_SD_FORMAT_HEADER_INVALID		(1)
#define CRI_AERO_ERROR_FLASH_FORMAT_HEADER_INVALID	(2)
#define CRI_AERO_ERROR_NO_VALID_STORAGE				(3)
#define CRI_AERO_ERROR_NO_STORAGE_CANDIDATES		(4)
#define CRI_AERO_ERROR_WRITE_SD_FAILED				(5)

#ifdef __cplusplus
extern "C" {
#endif

CriUint32 CriAeroError_GetLastError(void);

void CriAeroError_SetLastError(CriUint32 ErrorNo);

#ifdef __cplusplus
}
#endif

#endif	

