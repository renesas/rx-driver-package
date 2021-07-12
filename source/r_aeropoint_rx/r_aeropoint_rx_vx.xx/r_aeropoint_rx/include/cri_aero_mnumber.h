/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_mnumber.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_MNUMBER_H
#define CRI_AERO_MNUMBER_H

#include "cri_xpt.h"


#define	_CRI_AERO_MNUMBER_NMLIST_MAX	(32)
#define	_CRI_AERO_MNUMBER_NMPARS_MAX	(23)

struct CriAeroMNParts {
	const  CriUint8   *_pTexture;
	       CriSint32   _ClipL;
	       CriSint32   _ClipR;
};

struct CriAeroMNList {
	struct CriAeroMNParts *_pParts[_CRI_AERO_MNUMBER_NMPARS_MAX];
	CriSint32   _Size;
};

struct CriAeroMNTexture {
	struct CriAeroMNList *_pList[_CRI_AERO_MNUMBER_NMLIST_MAX];
};

#define CRI_AERO_MNUMBER_NMTEXTURE_MAX	(8)

#define _CRI_AERO_MNUMBER_PRINT_NUMS	(16)
struct CriAeroPrintNumber {
	CriSint32 _Width;
	CriSint32 _Height;
	CriSint32 _Max;
	CriSint32 _Min;
	CriSint32 _Init;

	CriSint32 _Comma;
	CriSint32 _Align;
	CriSint32 _Type;
	CriSint32 _Format;
	CriSint32 _Digit;

	CriSint32 _TextureIndex;
	CriSint32 _OffsetWidth;
	CriSint32 _Header;
	CriSint32 _Footer;
	CriSint32 _Nums[_CRI_AERO_MNUMBER_PRINT_NUMS];
	CriBool   _Updated;
	CriSint32 _DrawCount;
};


#ifdef __cplusplus
extern "C" {
#endif

CriBool CRIAPI CriAeroMnumber_SetNumber( CriUint32 NumberIndex, CriFloat32 Number );

CriBool CRIAPI CriAeroMnumber_SetUnit( CriUint32 NumberIndex, CriSint32 Header, CriSint32 Footer );

CriBool CRIAPI CriAeroMnumber_SetMultiNumberTexture( struct CriAeroMNTexture *pMNTexture, CriSint32 TextureIndex );

#ifdef __cplusplus
}
#endif

#endif	

