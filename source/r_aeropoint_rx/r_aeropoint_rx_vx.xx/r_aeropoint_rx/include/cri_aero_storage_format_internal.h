/****************************************************************************
 *
 * CRI Middleware STORAGEK
 *
 * Copyright (c) 2018 - 2019 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Player
 * File     : cri_aero_storage_format_internal.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_STORAGE_FORMAT_INTERNAL_H
#define CRI_AERO_STORAGE_FORMAT_INTERNAL_H



#include "cri_xpt.h"


typedef struct _aplStorageFormat
{
	char FormatName[16];
	char TargetName[16];
	char DeviceName[16];
	CriUint32 StartSlide;           
	CriUint32 ConfigStartBlock;
	CriUint32 ConfigSize;
	CriUint32 ConfigNum;
	CriUint32 AeroStartBlock;
	CriUint32 AeroSize;
	CriUint32 AeroNum;
	CriUint32 FontStartBlock;
	CriUint32 FontSize;
	CriUint32 FontNum;
	CriUint32 DAmpStartBlock;
	CriUint32 MovieStartBlock;
	CriUint32 MovieSize;
	CriUint32 MovieNum;
} _aplStorageFormat;


const char *CriAeroStorageFormat_GetFormatSymbol(void);
const char *CriAeroStorageFormat_GetTargetSymbol(void);
const char *CriAeroStorageFormat_GetDeviceSdSymbol(void);
const char *CriAeroStorageFormat_GetDeviceFlashSymbol(void);


void CriAeroStorageFormat_AdoptForMap(const _aplStorageFormat *pHeader);

void CriAeroStorageFormat_EraseMap(void);

CriBool CriAeroStorageFormat_IsValid(void);


void CriAeroStorageFormat_SetSdFormat(_aplStorageFormat *pHeader);

void CriAeroStorageFormat_SetSdFormat(_aplStorageFormat *pHeader);

CriBool CriAeroStorageFormat_CheckFormatName(const char *pName);


CriBool CriAeroStorageFormat_MainWriteFormatSd(void);

CriBool CriAeroStorageFormat_MainWriteFormatFlash(void);

#endif 


