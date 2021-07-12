/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2021 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : cri_aero_config_ais.c
 *
 ****************************************************************************/

#include "cri_aero_config_internal.h"
#include "cri_aero_config_ais.h"

CriUint32 CRIAPI criAeroConfig_GetDisplayWidth_AIS()
{
	return CRI_AERO_CONFIG_AIS_LCD_WIDTH;
}

CriUint32 CRIAPI criAeroConfig_GetDisplayHeight_AIS()
{
	return CRI_AERO_CONFIG_AIS_LCD_HEIGHT;
}

CriUint32 CRIAPI criAeroConfig_GetFrameBufferBaseAddr1_AIS()
{
	return CRI_AERO_CONFIG_AIS_FRAME_BUFFER_1;
}

CriUint32 CRIAPI criAeroConfig_GetFrameBufferBaseAddr2_AIS()
{
	return CRI_AERO_CONFIG_AIS_FRAME_BUFFER_2;
}

CriUint32 CRIAPI criAeroConfig_GetAeroDataBaseAddr_AIS()
{
	return CRI_AERO_CONFIG_AIS_AERODATA;
}


