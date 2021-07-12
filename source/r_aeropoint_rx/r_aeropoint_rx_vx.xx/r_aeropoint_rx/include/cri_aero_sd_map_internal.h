/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 - 2019 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Player
 * File     : cri_aero_sd_map_internal.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_SD_MAP_INTERNAL_H
#define CRI_AERO_SD_MAP_INTERNAL_H



#include "cri_xpt.h"
#include "cri_aero_storage_format_internal.h"


CriUint32 CriAeroSdMap_GetUnitBytes_AIS(void);

CriUint32 CriAeroSdMap_GetMovieBlockAddr_AIS(CriUint32 MoviePlayId);

CriUint32 CriAeroSdMap_GetFontTopBlockAddr_AIS(void);

CriUint32 CriAeroSdMap_GetDampTopBlockAddr_AIS(void);

CriUint32 CriAeroSdMap_GetSlideBlockAddr_AIS(CriUint32 SlideNo);

CriUint32 CriAeroSdMap_GetFontCodeBlockAddr_AIS(CriUint32 Fontid, CriUint32 Code);

CriUint32 CriAeroSdMap_GetSlideBlocks_AIS(void);

CriUint32 CriAeroSdMap_GetZeroBlockAddr_AIS(void);

void CriAeroSdMap_SetSdFormat_AIS(_aplStorageFormat *pFormat);

CriBool CriAeroSdMap_CheckFormatName(const char *pName);

#endif 


