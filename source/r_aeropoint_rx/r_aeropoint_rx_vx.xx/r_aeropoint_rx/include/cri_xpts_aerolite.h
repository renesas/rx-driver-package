/***************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 - 2019 CRI Middleware Co., Ltd.
 *
 * Library  : CRI Middleware Library
 * Module   : CRI Common Header for AeropointLite
 * File     : cri_xpts_aerolite.h
 *
 ***************************************************************************/
#ifndef CRI_INCL_CRI_XPTS_AEROLITE_H
#define CRI_INCL_CRI_XPTS_AEROLITE_H


#ifndef _TYPEDEF_CriUint8
#define _TYPEDEF_CriUint8
typedef unsigned char			CriUint8;		
#endif

#ifndef _TYPEDEF_CriSint8
#define _TYPEDEF_CriSint8
typedef signed char				CriSint8;		
#endif

#ifndef _TYPEDEF_CriUint16
#define _TYPEDEF_CriUint16
typedef unsigned short			CriUint16;		
#endif

#ifndef _TYPEDEF_CriSint16
#define _TYPEDEF_CriSint16
typedef signed short			CriSint16;		
#endif

#ifndef _TYPEDEF_CriUint32
#define _TYPEDEF_CriUint32
typedef unsigned long			CriUint32;		
#endif

#ifndef _TYPEDEF_CriSint32
#define _TYPEDEF_CriSint32
typedef signed long				CriSint32;		
#endif

#ifndef _TYPEDEF_CriUint64
#define _TYPEDEF_CriUint64
typedef unsigned long long		CriUint64;		
#endif

#ifndef _TYPEDEF_CriSint64
#define _TYPEDEF_CriSint64
typedef signed long long		CriSint64;		
#endif

#ifndef _TYPEDEF_CriUint128
#define _TYPEDEF_CriUint128
typedef struct {								
	CriUint64	h;								
	CriUint64	l;								
} CriUint128;
#endif

#ifndef _TYPEDEF_CriSint128
#define _TYPEDEF_CriSint128
typedef struct {								
	CriSint64	h;								
	CriUint64	l;								
} CriSint128;
#endif

#ifndef _TYPEDEF_CriFloat16
#define _TYPEDEF_CriFloat16
typedef signed short			CriFloat16;		
#endif

#ifndef _TYPEDEF_CriFloat32
#define _TYPEDEF_CriFloat32
typedef float					CriFloat32;		
#endif

#ifndef _TYPEDEF_CriFloat64
#define _TYPEDEF_CriFloat64
typedef double					CriFloat64;		
#endif

#ifndef _TYPEDEF_CriFixed32
#define _TYPEDEF_CriFixed32
typedef signed long				CriFixed32;		
#endif

#ifndef _TYPEDEF_CriBool
#define _TYPEDEF_CriBool
typedef CriSint32				CriBool;		
#endif

#ifndef _TYPEDEF_CriChar8
#define _TYPEDEF_CriChar8
typedef char					CriChar8;		
#endif


#ifndef _TYPEDEF_CriSintPtr
#define _TYPEDEF_CriSintPtr
typedef CriSint32				CriSintPtr;
#endif

#ifndef _TYPEDEF_CriUintPtr
#define _TYPEDEF_CriUintPtr
typedef CriUint32				CriUintPtr;
#endif


#ifndef CRIAPI
#define CRIAPI
#endif	

#endif	

