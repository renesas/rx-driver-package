/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_graphics_internal.h
 *
 ****************************************************************************/

#ifndef SRC_CRI_AERO_GRAPHICS_INTERNAL_H_
#define SRC_CRI_AERO_GRAPHICS_INTERNAL_H_

#include "cri_aero_graphics.h"

#ifdef __cplusplus
extern "C" {
#endif

	void CRIAPI criAeroGraphics_Clear();

	CriBool CRIAPI criAeroGraphics_IsDrawing();

	void CRIAPI criAeroGraphics_SetFillColor( CriUint8 Alpha, CriUint8 Red, CriUint8 Green, CriUint8 Blue );

	void CRIAPI criAeroGraphics_SetGlobalAlpha( CriUint8 Alpha );

	void CRIAPI criAeroGraphics_SetGlobalAlphaOperation( CriBool UseAlpha );

	void CRIAPI criAeroGraphics_FillRect( CriSint32 X, CriSint32 Y, CriSint32 Width, CriSint32 Height );

	CriUint32 CRIAPI criAeroGraphics_SetupTexture( const void *pTexture, CriUint32 Size );

	void CRIAPI criAeroGraphics_FillArea( CriSint32 X, CriSint32 Y, CriSint32 Width, CriSint32 Height, CriUint8 Alpha, CriUint8 Red, CriUint8 Green, CriUint8 Blue );

	void CRIAPI criAeroGraphics_DrawTexture( const CriUint8 *pTexture, CriUint32 TextureId, CriSint32 Width, CriSint32 Height, CriSint32 OffX, CriSint32 OffY );

	void CRIAPI criAeroGraphics_DrawTextureResize( const CriUint8 *pTexture, CriUint32 TextureId, CriSint32 X, CriSint32 Y, CriSint32 Width, CriSint32 Height );

	void CRIAPI criAeroGraphics_DrawTextureCut( const CriUint8 *pTexture, CriUint32 TextureId, CriSint32 SrcX, CriSint32 SrcY, CriSint32 SrcWidth, CriSint32 SrcHeight, CriSint32 DstX, CriSint32 DstY, CriSint32 DstWidth, CriSint32 DstHeight );

	void CRIAPI criAeroGraphics_SetNumVSynckSwitch( CriUint32 NumSwitch );

	CriUint32 CRIAPI criAeroGraphics_Convert565To8888( CriUint16 Color );

	CriBool CRIAPI criAeroGraphics_Initialize_AIS( CriGraphics *pGraphics );

	void CRIAPI criAeroGraphics_VSyncCallback_AIS( void *pData );

	void CRIAPI criAeroGraphics_WaitVSync_AIS( CriBool Flip );

	void CRIAPI criAeroGraphics_SetNumVSynckSwitch_AIS( CriUint32 NumSwitch );

#ifdef __cplusplus
}
#endif

#endif 
