/****************************************************************************
 *
 * CRI Middleware FLASHK
 *
 * Copyright (c) 2018 - 2019 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Player
 * File     : cri_aero_flash_map_internal.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_FLASH_MAP_INTERNAL_H
#define CRI_AERO_FLASH_MAP_INTERNAL_H



#include "cri_xpt.h"
#include "cri_aero_storage_format_internal.h"


CriUint32 CriAeroFlashMap_GetUnitBytes_AIS(void);

CriUint32 CriAeroFlashMap_GetMovieBlockAddr_AIS(CriUint32 MoviePlayId);

CriUint32 CriAeroFlashMap_GetFontTopBlockAddr_AIS(void);

CriUint32 CriAeroFlashMap_GetDampTopBlockAddr_AIS(void);

CriUint32 CriAeroFlashMap_GetSlideBlockAddr_AIS(CriUint32 SlideNo);

CriUint32 CriAeroFlashMap_GetFontCodeBlockAddr_AIS(CriUint32 Fontid, CriUint32 Code);

CriUint32 CriAeroFlashMap_GetSlideBlocks_AIS(void);

CriUint32 CriAeroFlashMap_GetZeroBlockAddr_AIS(void);

void CriAeroFlashMap_SetFlashFormat_AIS(_aplStorageFormat *pFormat);

#endif 


