/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_graphics.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_GRAPHICS_H
#define CRI_AERO_GRAPHICS_H

#include "cri_xpt.h"

#define TAG_CLT8	(0x38544C43)	

typedef void (*_criAero_VSyncCallback)( void *pData );

typedef struct CriGraphics
{
	CriUint32 *pPalette;
	CriUint32  PaletteNum;

	void      *pForegroundFrameBuffer;
	void      *pBackgroundFrameBuffer;

	CriUint32  PanelWidth;
	CriUint32  PanelHeight;
	CriUint32  PanelPitch;

	CriUint32  PixelSize;
}CriGraphics;

typedef struct CriCLT8
{
	CriUint32 clut8Tag;		
	CriUint32 clutFlags;
	CriUint32 width;
	CriUint32 height;
	CriUint16 table[256];
}CriCLT8;

#ifdef __cplusplus
extern "C" {
#endif

CriBool CRIAPI CriAeroGraphics_Initialize( CriGraphics *pGraphics );

void CRIAPI CriAeroGraphics_Finalize();

void* CRIAPI CriAeroGraphics_GetHandle();

CriUint32 CRIAPI CriAeroGraphics_GetPanelWidth();

CriUint32 CRIAPI CriAeroGraphics_GetPanelHeight();

CriUint32 CRIAPI CriAeroGraphics_GetPanelPitch();

void CRIAPI CriAeroGraphics_SetForegroundFrameBuffer(void *pForegroundFrameBuffer);

void CRIAPI CriAeroGraphics_SetBackgroundFrameBuffer(void *pBackgroundFrameBuffer);

void* CRIAPI CriAeroGraphics_GetForegroundFrameBuffer();

void* CRIAPI CriAeroGraphics_GetBackgroundFrameBuffer();


#ifdef __cplusplus
}
#endif

#endif	

