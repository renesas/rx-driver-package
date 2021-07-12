/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : cri_aero_flash_map_ais.c
 *
 ***************************************************************************/

#include "cri_xpt.h"
#include "cri_aero_flash_map_internal.h"
#include <string.h>


#define _FLASHMAP_FLASH_UNIT_SIZE		(512L)


#define _FLASHMAP_CONFIG_DATA	(0x00000000)	



#define _FLASHMAP_AEROPOINT_DATA	(0x00000200)	

	#define _FLASHMAP_SLIDE_ALIGNMENT	(256L*1024L)		






CriUint32 CriAeroFlashMap_GetMovieBlockAddr_AIS(CriUint32 MoviePlayId)
{
	return 0;
}


CriUint32 CriAeroFlashMap_GetDampTopBlockAddr_AIS(void)
{
	return 0;
}

CriUint32 CriAeroFlashMap_GetFontTopBlockAddr_AIS(void)
{
	return 0;
}

CriUint32 CriAeroFlashMap_GetSlideBlockAddr_AIS(CriUint32 SlideNo)
{
	return _FLASHMAP_AEROPOINT_DATA + (SlideNo)*((_FLASHMAP_SLIDE_ALIGNMENT) / (_FLASHMAP_FLASH_UNIT_SIZE));
}

CriUint32 CriAeroFlashMap_GetUnitBytes_AIS(void)
{
	return _FLASHMAP_FLASH_UNIT_SIZE;
}

CriUint32 CriAeroFlashMap_GetFontCodeBlockAddr_AIS(CriUint32 FontId, CriUint32 Code)
{
	return 0;
}


CriUint32 CriAeroFlashMap_GetSlideBlocks_AIS(void)
{
	return _FLASHMAP_SLIDE_ALIGNMENT / _FLASHMAP_FLASH_UNIT_SIZE;
}

CriUint32 CriAeroFlashMap_GetZeroBlockAddr_AIS(void)
{
	return 0;
}

void CriAeroFlashMap_SetFlashFormat_AIS(_aplStorageFormat *pFormat)
{
	memset(pFormat, 0, sizeof(_aplStorageFormat));

	memcpy(pFormat->FormatName, CriAeroStorageFormat_GetFormatSymbol(), strlen(CriAeroStorageFormat_GetFormatSymbol()));
	memcpy(pFormat->TargetName, CriAeroStorageFormat_GetTargetSymbol(), strlen(CriAeroStorageFormat_GetTargetSymbol()));
	memcpy(pFormat->DeviceName, CriAeroStorageFormat_GetDeviceSdSymbol(), strlen(CriAeroStorageFormat_GetDeviceSdSymbol()));

	pFormat->StartSlide=0;

	pFormat->ConfigStartBlock = _FLASHMAP_CONFIG_DATA;
	pFormat->ConfigSize = (_FLASHMAP_AEROPOINT_DATA-_FLASHMAP_CONFIG_DATA) * _FLASHMAP_FLASH_UNIT_SIZE;
	pFormat->ConfigNum = 1;

	pFormat->AeroStartBlock = _FLASHMAP_AEROPOINT_DATA;
	pFormat->AeroSize = _FLASHMAP_SLIDE_ALIGNMENT;
	pFormat->AeroNum = (0x00020000 - _FLASHMAP_AEROPOINT_DATA) * _FLASHMAP_FLASH_UNIT_SIZE / _FLASHMAP_SLIDE_ALIGNMENT;

	pFormat->FontStartBlock = 0;
	pFormat->FontSize = 0;
	pFormat->FontNum = 0;

	pFormat->DAmpStartBlock = 0;

	pFormat->MovieStartBlock = 0;
	pFormat->MovieSize = 0;
	pFormat->MovieNum = 0;
}
