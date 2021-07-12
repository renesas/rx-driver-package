/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 - 2019 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_debugtext.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_DEBUGTEXT_H
#define CRI_AERO_DEBUGTEXT_H

#include <stdio.h>
#include <stdarg.h>

#include "cri_xpt.h"
#include "cri_aero_color_def.h"


#ifdef __cplusplus
extern "C" {
#endif


void CRIAPI CriAeroDebugText_Clear(void);

CriBool CRIAPI CriAeroDebugText_FreePrint(CriSint32 X, CriSint32 Y, CriAeroColorID ColorId, CriAeroColorID BackgroundColorId, const char *pText);

void CRIAPI CriAeroDebugText_SetConsoleMode(CriSint32 X, CriSint32 Y, CriUint16 lineLimit);

void CRIAPI CriAeroDebugText_SetFreeMode(void);

CriBool CRIAPI CriAeroDebugText_ConsolePrint(CriAeroColorID ColorId, CriAeroColorID BackgroundColorId, const char *pText);


#ifdef __cplusplus
}
#endif

#endif	

