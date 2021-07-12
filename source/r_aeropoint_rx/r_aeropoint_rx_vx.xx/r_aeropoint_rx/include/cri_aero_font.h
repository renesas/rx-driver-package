/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2020 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_font.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_FONT_H
#define CRI_AERO_FONT_H

#include <stdio.h>
#include <stdarg.h>

#include "cri_xpt.h"
#include "cri_aero_color_def.h"


#ifdef __cplusplus
extern "C" {
#endif

void CRIAPI CriAeroFont_Clear(void);

CriBool CRIAPI CriAeroFont_Print(CriSint32 X, CriSint32 Y, CriSint32 FontId, CriAeroColorID ColorId, const char *pText);

#ifdef __cplusplus
}
#endif

#endif	

