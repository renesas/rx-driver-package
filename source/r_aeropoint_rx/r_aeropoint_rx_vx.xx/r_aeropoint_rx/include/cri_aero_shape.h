/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_shape.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_SHAPE_H
#define CRI_AERO_SHAPE_H

#include "cri_xpt.h"


struct CriAeroPoint {
	CriSint32 x;
	CriSint32 y;
};

enum {
	CRI_AERO_SHAPE_EFFECT_NONE = 0,
	CRI_AERO_SHAPE_EFFECT_SHAKE,
	CRI_AERO_SHAPE_EFFECT_BACKGROUND,
	CRI_AERO_SHAPE_EFFECT_STAMP_3D,
	CRI_AERO_SHAPE_EFFECT_STAMP_UP,
	CRI_AERO_SHAPE_EFFECT_STAMP_DOWN,
	CRI_AERO_SHAPE_EFFECT_STAMP_LEFT,
	CRI_AERO_SHAPE_EFFECT_STAMP_RIGHT,
	CRI_AERO_SHAPE_EFFECT_BUTTON,
};

enum {
	CRI_AERO_SHAPE_PRINT_NONE = 0,
	CRI_AERO_SHAPE_PRINT_NUMBER,
};

enum {
	CRI_AERO_SHAPE_SPEED_NORMAL = 0,
	CRI_AERO_SHAPE_SPEED_BREAK,
	CRI_AERO_SHAPE_SPEED_ACCEL,
};

#define CRI_AERO_SHAPE_USE_TRANSLATE (1u <<  0)
#define CRI_AERO_SHAPE_USE_ROTATE    (1u <<  2)
#define CRI_AERO_SHAPE_USE_SCALE     (1u <<  1)
#define CRI_AERO_SHAPE_USE_ALPHA     (1u <<  5)

enum {
	CRI_AERO_SHAPE_ADDACTION_NONE = 0,
	CRI_AERO_SHAPE_ADDACTION_TARGET,
	CRI_AERO_SHAPE_ADDACTION_LOOP,
	CRI_AERO_SHAPE_ADDACTION_ROUND_TRIP,
};

#define CRI_AERO_SHAPE_FAMILY_SUB    ( -5 )
#define CRI_AERO_SHAPE_FAMILY_PARENT ( -2 )
#define CRI_AERO_SHAPE_FAMILY_ALONE  ( -1 )

#define CRI_AERO_SHAPE_WORKFLAGS_CHANGENOW    (1u <<  0)

struct CriAeroParamCtrl {
	const  CriChar8    *_pName;			

	CriUint32 _UseFlag;

	CriSint32 _ParamMin;
	CriSint32 _ParamMax;
	CriSint32 _ParamInit;

	CriSint32 _Time;
	CriSint32 _SpeedType;
	CriSint32 _Pivot;

	CriBool   _AutoStart;
	CriUint32 _WaitStart;
	CriSint32 _ParamTarget;
	CriSint32 _AddAction;

	CriSint32 _X_Min;
	CriSint32 _X_Max;
	CriSint32 _Y_Min;
	CriSint32 _Y_Max;

	CriSint32 _R_Min;
	CriSint32 _R_Max;

	CriFloat32 _W_Min;
	CriFloat32 _W_Max;
	CriFloat32 _H_Min;
	CriFloat32 _H_Max;

	CriSint32 _T_Min;
	CriSint32 _T_Max;

	CriUint32 _workFlags;
	CriBool   _tarSet;
	CriSint32 _tarParam;
	CriSint32 _sttParam;
	CriSint32 _nowParam;
	CriUint32 _waitStartCount;

	const CriUint8 *_pTexture;
};

struct CriAeroPattern {
	const CriUint8  *_pTexture;	
	      CriSint32  _Resized;	
};

#define CRI_AERO_SHAPE_FLAGS_UPDATED    (1u <<  0)

struct CriAeroShape {

	CriSint32 _Id;

	CriSint32 BaseX;
	CriSint32 BaseY;
	CriSint32 BaseW;
	CriSint32 BaseH;

	CriSint32 _Resized;

	struct CriAeroParamCtrl *_pParamCtrl;
	       CriSint32          _ParentId;

	       CriBool       _IsDraw;
	struct CriAeroPoint _Pos;
	       CriSint32     _Rot;
	       CriFloat32    _ScaleW;
	       CriFloat32    _ScaleH;
	       CriSint32     _Alpha;

	const  CriUint8   *_pTexture;
	       CriSint32   _TextureSize;
	       CriUint32   _TextureIndex;

	struct CriAeroPattern  *_pPattern;		
	       CriSint32        _PatternNum;	
	       CriSint32        _PatternInit;	
	       CriSint32        _PatternNow;
	const  CriChar8        *_pPatternName;	

	       CriSint32   _Phase;
	       CriSint32   _InWait;
	       CriSint32   _Count;
	       CriSint32   _Effect;

	       CriUint32   _ButtonState;
	       CriSint32   _ButtonId;
	       CriSint32   _SoundId;
	       CriSint32   _MovieId;
	       CriSint32   _OutSignalId;		
	       CriSint32   _OutOperationId;		
	       CriSint32   _TransPageId;

	       CriSint32   _PrintType;
	       CriSint32   _PrintDataIndex;

	       CriSint32   _rotVec;
	       CriSint32   _rotCalc;
	       CriSint32   _IsVisible;
	       CriUint32   _Flags;
};


#ifdef __cplusplus
extern "C" {
#endif
CriBool CRIAPI criAeroShape_ExecBackground( struct CriAeroShape *pShape, CriSint32 Step, CriBool IsFirst, struct CriAeroParamCtrl *pAreaParamCtrl );
CriBool CRIAPI criAeroShape_Exec( struct CriAeroShape *pShape, CriSint32 Step, CriBool IsFirst, struct CriAeroParamCtrl *pAreaParamCtrl, CriSint32 *pDstSlide );

CriBool CRIAPI criAeroShape_Out( struct CriAeroShape *pShape, CriSint32 Step, CriBool IsFirst );

void CRIAPI criAeroShape_SetVisible( struct CriAeroShape *pShape, CriBool IsVisible );

#ifdef __cplusplus
}
#endif

#endif	

