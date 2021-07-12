/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2020 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_primitive_draw.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_PRIMITIVE_DRAW_H
#define CRI_AERO_PRIMITIVE_DRAW_H

#include "cri_xpt.h"
#include <cri_aero_color_def.h>

typedef enum {
	CRI_AERO_ETYPE_RECTANGLE = 0,	
	CRI_AERO_ETYPE_CIRCLE,			
	CRI_AERO_ETYPE_DIA,				
	CRI_AERO_ETYPE_TRIANGLE,		
} CriAeroEndPointType;

#ifdef __cplusplus
extern "C" {
#endif

void CRIAPI CriAeroPrimitiveDraw_EndPoint( CriAeroEndPointType Type, CriSint32 X, CriSint32 Y, CriAeroColorID Color );

void CRIAPI CriAeroPrimitiveDraw_Line( CriSint32 X1, CriSint32 Y1, CriSint32 X2, CriSint32 Y2, CriAeroColorID Color );

void CRIAPI CriAeroPrimitiveDraw_Bar( CriSint32 X1, CriSint32 Y1, CriSint32 X2, CriSint32 Y2, CriAeroColorID Color );

void CRIAPI CriAeroPrimitiveDraw_Clear();

#ifdef __cplusplus
}
#endif

#endif	

