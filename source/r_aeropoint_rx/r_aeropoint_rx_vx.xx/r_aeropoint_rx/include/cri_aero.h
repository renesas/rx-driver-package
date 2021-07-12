/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2020 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Aeropoint Lite Common Header
 * File     : cri_aero.h
 *
 ****************************************************************************/
#ifndef	CRI_AERO_H
#define	CRI_AERO_H

#include <stdint.h>

#define CRI_AERO_RUNTIME_VERSION				(2021031201)
#define CRI_AERO_RUNTIME_VERSION_TEXT			"20210312R1"

#define CRI_AERO_DATA_VERSION					(2020121801)
#define CRI_AERO_DATA_VERSION_TEXT				"20201218R1"

#define R_AEROPOINT_VERSION_MAJOR  (1)
#define R_AEROPOINT_VERSION_MINOR  (5)

uint32_t R_AEROPOINT_GetVersion(void);

#endif	

