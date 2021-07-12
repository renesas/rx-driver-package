/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : cri_aero_sd_map_ais.c
 *
 ***************************************************************************/

#include "cri_xpt.h"
#include "cri_aero_sd_map_internal.h"
#include <string.h>
#include "cri_aero_storage_format_internal.h"


#define _SDMAP_SD_UNIT_SIZE		(512L)


#define _SDMAP_CONFIG_DATA	(0x00000000)	



#define _SDMAP_AEROPOINT_DATA	(0x00000200)	

	#define _SDMAP_SLIDE_ALIGNMENT	(256L*1024L)		



#define _SDMAP_FONT_DATA		(0x00020000)	

	#define _SDMAP_FONT_ALIGNMENT	(32L*1024L*1024L)	



#define _SDMAP_DAMP_DATA		(0x000A0000)	



#define _SDMAP_VIDEO_DATA		(0x000C0000)	

	#define _SDMAP_VIDEO_ALIGNMENT	(64L*1024L*1024L)	




CriUint32 CriAeroSdMap_GetMovieBlockAddr_AIS(CriUint32 MoviePlayId)
{
	return _SDMAP_VIDEO_DATA + ((_SDMAP_VIDEO_ALIGNMENT / 512)* MoviePlayId);
}


CriUint32 CriAeroSdMap_GetDampTopBlockAddr_AIS(void)
{
	return _SDMAP_DAMP_DATA;
}

CriUint32 CriAeroSdMap_GetFontTopBlockAddr_AIS(void)
{
	return _SDMAP_FONT_DATA;
}

CriUint32 CriAeroSdMap_GetSlideBlockAddr_AIS(CriUint32 SlideNo)
{
	return _SDMAP_AEROPOINT_DATA + (SlideNo)*((_SDMAP_SLIDE_ALIGNMENT) / (_SDMAP_SD_UNIT_SIZE));
}

CriUint32 CriAeroSdMap_GetUnitBytes_AIS(void)
{
	return _SDMAP_SD_UNIT_SIZE;
}

CriUint32 CriAeroSdMap_GetFontCodeBlockAddr_AIS(CriUint32 FontId, CriUint32 Code)
{
	return _SDMAP_FONT_DATA+((_SDMAP_FONT_ALIGNMENT/512)*FontId)+Code;
}


CriUint32 CriAeroSdMap_GetSlideBlocks_AIS(void)
{
	return _SDMAP_SLIDE_ALIGNMENT / _SDMAP_SD_UNIT_SIZE;
}

CriUint32 CriAeroSdMap_GetZeroBlockAddr_AIS(void)
{
	return 0;
}


void CriAeroSdMap_SetSdFormat_AIS(_aplStorageFormat *pFormat)
{
	memset(pFormat, 0, sizeof(_aplStorageFormat));

	memcpy(pFormat->FormatName, CriAeroStorageFormat_GetFormatSymbol(), strlen(CriAeroStorageFormat_GetFormatSymbol()));
	memcpy(pFormat->TargetName, CriAeroStorageFormat_GetTargetSymbol(), strlen(CriAeroStorageFormat_GetTargetSymbol()));
	memcpy(pFormat->DeviceName, CriAeroStorageFormat_GetDeviceSdSymbol(), strlen(CriAeroStorageFormat_GetDeviceSdSymbol()));

	pFormat->StartSlide=0;

	pFormat->ConfigStartBlock = _SDMAP_CONFIG_DATA;
	pFormat->ConfigSize = (_SDMAP_AEROPOINT_DATA-_SDMAP_CONFIG_DATA) * _SDMAP_SD_UNIT_SIZE;
	pFormat->ConfigNum = 1;

	pFormat->AeroStartBlock = _SDMAP_AEROPOINT_DATA;
	pFormat->AeroSize = _SDMAP_SLIDE_ALIGNMENT;
	pFormat->AeroNum = (_SDMAP_FONT_DATA - _SDMAP_AEROPOINT_DATA) * _SDMAP_SD_UNIT_SIZE / _SDMAP_SLIDE_ALIGNMENT;

	pFormat->FontStartBlock = _SDMAP_FONT_DATA;
	pFormat->FontSize = _SDMAP_FONT_ALIGNMENT;
	pFormat->FontNum = (_SDMAP_DAMP_DATA - _SDMAP_FONT_DATA)  * _SDMAP_SD_UNIT_SIZE / _SDMAP_FONT_ALIGNMENT;

	pFormat->DAmpStartBlock = _SDMAP_DAMP_DATA;

	pFormat->MovieStartBlock = _SDMAP_VIDEO_DATA;
	pFormat->MovieSize = _SDMAP_VIDEO_ALIGNMENT;
	pFormat->MovieNum = 8;
}

CriBool CriAeroSdMap_CheckFormatName(const char *pName)
{
	return strcmp(pName, CriAeroStorageFormat_GetFormatSymbol()) == 0;
}


