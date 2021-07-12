/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : cri_aero_sd_internal.h
 *
 ***************************************************************************/
#ifndef CRI_AERO_SD_INTERNAL_H
#define CRI_AERO_SD_INTERNAL_H

#include "cri_xpt.h"

CriBool CriAeroSD_Initialize_AIS(void);
CriBool CriAeroSD_Finalize_AIS(void);
CriBool CriAeroSd_Write_AIS(const CriUint8 *pBuf, CriUint32 Lbn, CriUint32 Cnt);
CriBool CriAeroSd_Read_AIS(CriUint8 *pBuf, CriUint32 Lbn, CriUint32 Cnt);

CriBool CriAeroSd_Detect_AIS(void);

CriBool CriAeroSd_GetDitection_AIS(void);

#endif 

