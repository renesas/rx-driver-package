/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : cri_aero_flash_internal.h
 *
 ***************************************************************************/
#ifndef CRI_AERO_FLASH_INTERNAL_H
#define CRI_AERO_FLASH_INTERNAL_H

#include "cri_xpt.h"

CriBool CriAeroFlash_Initialize_AIS(void);
void CriAeroFlash_Finalize_AIS(void);
CriBool CriAeroFlash_Write256_AIS(const CriUint8 *pMemoryAddr, CriUint32 FlashAddr);
CriBool CriAeroFlash_ReadByte_AIS(CriUint8 *pMemoryAddr, CriUint32 FlashAddr, CriUint32 CntByte);

#endif 
