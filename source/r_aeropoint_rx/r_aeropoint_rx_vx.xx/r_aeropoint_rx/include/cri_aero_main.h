/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_main.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_MAIN_H
#define CRI_AERO_MAIN_H

#include "cri_xpt.h"
#include "cri_aero_draw.h"

typedef enum {
	CRI_AEROL_DRAWMODE_PASSIVE = 0,	
	CRI_AEROL_DRAWMODE_ACTIVE		
} CriAerolDrawMode;

struct CriAerolMain {
	       CriAerolDrawMode _DrawMode;
	       CriBool          _FBClear;
	       CriBool          _DrawWhenMoved;
	       CriBool          _ButtonBackClear;
	       CriUint32        _Error;

	       CriSint32       _ContentsWidth;
	       CriSint32       _ContentsHeight;

		   CriSint32       _FPX;				
		   CriSint32       _FPY;
		   CriSint32       _FPW;
		   CriSint32       _FPH;
		   CriSint32       _FCount;				
           CriUint32       _A;					
           CriUint32       _R;
           CriUint32       _G;
           CriUint32       _B;

	struct CriAeroSlide  *_pSlide;
	       CriSint32       _NumSlide;
	       CriSint32       _NowSlide;
	       CriBool         _ChangedSlide;
	       CriSint32       _ChangedCount;
	       CriSint32       _ChangedCountBak;
	       CriSint32       _PassedFrame;		

	struct CriAeroPrintNumber *_pPrintNumber;
	       CriSint32            _NumPrintNumber;

	struct CriAeroParamCtrl *_pParamCtrl;
	       CriSint32          _NumParamCtrl;

	struct CriAeroArea    *_pArea;
	       CriSint32       _NumArea;

	struct CriAeroRegion  *_pRegion;
	       CriSint32       _NumRegion;

	struct CriAeroSound   *_pSound;
	       CriSint32       _NumSound;

	       CriBool         _UseTouch;

			CriSint32 _Phase;
			CriBool   _Moved;
			CriBool   _MovedBak;
			CriBool   _Updated;
			CriSint32 _CntI;
			CriSint32 _CntJ;
			CriSint32 _DstSlide;

			CriBool _Exit;
};

#ifdef __cplusplus
extern "C" {
#endif

void CRIAPI CriAero_ClearAeropointStructure( struct CriAerolMain *pMain );

void CRIAPI CriAero_InitializeBinary( struct CriAerolMain *pMain, void *pSlideData, void *pAreaData, void *pRegionData );
void CRIAPI CriAero_Initialize( struct CriAerolMain *pMain, struct CriAerolData *pSlideData, struct CriAerolData *pAreaData, struct CriAerolData *pRegionData );

CriBool CRIAPI CriAero_CheckExit( struct CriAerolMain *pMain );

void CRIAPI CriAero_SetDrawMode( struct CriAerolMain *pMain, CriAerolDrawMode DrawMode );

void CRIAPI CriAero_SetFrameBufferClear( struct CriAerolMain *pMain, CriBool FrameBufferClear );

void CRIAPI CriAero_SetButtonBackClear( struct CriAerolMain *pMain, CriBool ButtonBackClear );

void CRIAPI CriAero_SetTouchFunction( struct CriAerolMain *pMain, CriBool UseTouch );

CriUint32 CRIAPI CriAero_CheckError( struct CriAerolMain *pMain );

CriSint32 CRIAPI CriAero_GetContentsWidth( struct CriAerolMain *pMain );
CriSint32 CRIAPI CriAero_GetContentsHeight( struct CriAerolMain *pMain );

void CRIAPI CriAero_Activate( struct CriAerolMain *pMain, CriSint32 FrameWidth, CriSint32 FrameHeight );

CriBool CRIAPI CriAero_Background( struct CriAerolMain *pMain, CriSint32 FrameWidth, CriSint32 FrameHeight );
CriBool CRIAPI CriAero_Run( struct CriAerolMain *pMain, CriSint32 FrameWidth, CriSint32 FrameHeight );

CriBool CRIAPI CriAero_CheckUpdate( struct CriAerolMain *pMain );

CriSint32 CRIAPI CriAero_PrintNextSlide( struct CriAerolMain *pMain );

CriBool CRIAPI CriAero_PrintSlide( struct CriAerolMain *pMain, CriSint32 SlideNo );

CriSint32 CRIAPI CriAero_GetSlideNo( struct CriAerolMain *pMain );

CriBool CRIAPI CriAero_PrintArea( struct CriAerolMain *pMain, CriSint32 AreaNo, CriSint32 RegionNo );

CriSint32 CRIAPI CriAero_GetAreaNo( struct CriAerolMain *pMain, const CriChar8 *pName );

CriSint32 CRIAPI CriAero_GetRegionNo( struct CriAerolMain *pMain, const CriChar8 *pName );

CriBool CRIAPI CriAero_ClearArea( struct CriAerolMain *pMain, CriSint32 AreaNo );

CriBool CRIAPI CriAero_ClearAllArea( struct CriAerolMain *pMain );

CriBool CRIAPI CriAero_SetFillColor( struct CriAerolMain *pMain, CriUint32 A, CriUint32 R, CriUint32 G, CriUint32 B );

CriBool CRIAPI CriAero_FillArea( struct CriAerolMain *pMain, CriSint32 AreaNo );

CriBool CRIAPI CriAero_Clear( struct CriAerolMain *pMain, CriSint32 X, CriSint32 Y, CriSint32 Width, CriSint32 Height );

CriBool CRIAPI CriAero_SetParamControl( struct CriAerolMain *pMain, CriSint32 ParamCtrlNo, CriSint32 Param );
CriBool CRIAPI CriAero_SetParamControlNow( struct CriAerolMain *pMain, CriSint32 ParamCtrlNo, CriSint32 Param );

CriSint32 CRIAPI CriAero_GetParamControlNo( struct CriAerolMain *pMain, const CriChar8 *pName );

CriBool CRIAPI CriAero_ChangeParamControlTexture( struct CriAerolMain *pMain, CriSint32 ParamCtrlNo, const CriUint8 *pTexture );

CriBool CRIAPI CriAero_CheckUseCamera( struct CriAerolMain *pMain );

void CRIAPI CriAero_SetUseCamera( struct CriAerolMain *pMain, CriBool UseCamera );

void CRIAPI _CriAero_GraphTest();
void CRIAPI _CriAero_GraphTestLocal( struct CriAerolMain *pMain );

CriBool CRIAPI CriAero_SetShapeVisible( struct CriAerolMain *pMain, CriSint32 ShapeId, CriBool IsVisible );

#ifdef __cplusplus
}
#endif

#endif	

