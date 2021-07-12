/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_slide.h
 *
 ****************************************************************************/


#ifndef CRI_AERO_SLIDE_H
#define CRI_AERO_SLIDE_H

#include "cri_aero_shape.h"

enum {
	CRI_AERO_SLIDE_BACKGROUND = 0,	
	CRI_AERO_SLIDE_STATIC,			
	CRI_AERO_SLIDE_FOREGROUND,		
	CRI_AERO_SLIDE_STANDARD,		
};

enum {
	CRI_AERO_SLIDE_THEME_SCALE_UP = 0,
	CRI_AERO_SLIDE_THEME_SCALE_DOWN,
	CRI_AERO_SLIDE_THEME_TRANS_LEFT,
	CRI_AERO_SLIDE_THEME_TRANS_RIGHT,
	CRI_AERO_SLIDE_THEME_TRANS_DOWN,
	CRI_AERO_SLIDE_THEME_TRANS_UP,
	CRI_AERO_SLIDE_THEME_INOUT_2D,
	CRI_AERO_SLIDE_THEME_INOUT_3D,
};

struct CriAeroSlide {
	struct CriAeroShape  *pShape;
	CriSint32      NumShape;
	CriSint32      Page;
	CriSint32      Type;
	CriSint32      BgPage;
	CriSint32      FgPage;
	CriSint32      StPage;
	CriSint32      Theme;
	CriUint32      BgColorBGR;
	CriSint32      Transition;
	CriSint32      AdvanceTime;
	CriSint32      AdvanceOnClick;
	CriBool        Camera;
};

struct CriAeroArea {
	const  CriChar8    *_pName;			
	CriBool      _Moved;		
	CriBool      _First;			
	CriSint32    _X;
	CriSint32    _Y;
	CriSint32    _Width;
	CriSint32    _Height;

	CriSint32    _Count;			
	CriUint32    _A;
	CriUint32    _R;
	CriUint32    _G;
	CriUint32    _B;

	struct CriAeroParamCtrl  *_pParamCtrl;	
	CriSint32          _ParamCtrlNum;
	struct CriAeroShape      *_pShape;		
	CriSint32          _NumShape;	
};

struct CriAeroRegion {
	const  CriChar8      *_pName;		
	struct CriAeroShape  *_pShape;		
	CriSint32      _NumShape;	
};

struct CriAeroSound {
	const CriUint8 *_pSound;
	CriSint32       _SoundSize;
	const CriChar8 *_pComment;
};

struct CriAeroCLUT {
	void *_pCLUT;
};

struct CriAerolData {
	CriSint32					_Version;
	CriSint32					_ResolutionWidth;
	CriSint32					_ResolutionHeight;
	CriSint32					_Orientation;

	struct CriAeroSlide			*_pSlide;
	CriSint32					_NumSlide;

	struct CriAeroPrintNumber	*_pPrintNumber;
	CriSint32					_NumPrintNumber;

	struct CriAeroParamCtrl		*_pParamCtrl;
	CriSint32					_NumParamCtrl;

	struct CriAeroArea			*_pArea;
	CriSint32					_NumArea;

	struct CriAeroRegion		*_pRegion;
	CriSint32					_NumRegion;

	struct CriAeroSound			*_pSound;
	CriSint32					_NumSound;
};


#endif	

