/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2021 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_config_internal.h
 *
 ****************************************************************************/

#ifndef SRC_CRI_AERO_CONFIG_INTERNAL_H_
#define SRC_CRI_AERO_CONFIG_INTERNAL_H_

#include "cri_xpt.h"

#ifdef __cplusplus
extern "C" {
#endif

	CriUint32 CRIAPI criAeroConfig_GetDisplayWidth();

	CriUint32 CRIAPI criAeroConfig_GetDisplayHeight();

	CriUint32 CRIAPI criAeroConfig_GetFrameBufferBaseAddr1();
	CriUint32 CRIAPI criAeroConfig_GetFrameBufferBaseAddr2();

	CriUint32 CRIAPI criAeroConfig_GetAeroDataBaseAddr();

	CriUint32 CRIAPI criAeroConfig_GetDisplayWidth_AIS();
	CriUint32 CRIAPI criAeroConfig_GetDisplayHeight_AIS();
	CriUint32 CRIAPI criAeroConfig_GetFrameBufferBaseAddr1_AIS();
	CriUint32 CRIAPI criAeroConfig_GetFrameBufferBaseAddr2_AIS();
	CriUint32 CRIAPI criAeroConfig_GetAeroDataBaseAddr_AIS();

#ifdef __cplusplus
}
#endif

#endif 
