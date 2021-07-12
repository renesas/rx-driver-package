/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2020 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_dataflash.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_DATAFLASH_H
#define CRI_AERO_DATAFLASH_H


#include "cri_xpt.h"

#define CRI_AERO_DATA_FLASH_BLOCK_BYTES	(64)

#ifdef __cplusplus
extern "C" {
#endif

void CriAeroDataFlash_QueryAboutDataFlash(CriUint32 *StartAddress,CriUint32 *BytesBlock,CriUint32 *NumBlocks);

CriUint32 CriAeroDataFlash_Write(CriUint32 SrcAddress,CriUint32 DestAddress,CriUint32 NumBlocks);

#ifdef __cplusplus
}
#endif

#endif	
