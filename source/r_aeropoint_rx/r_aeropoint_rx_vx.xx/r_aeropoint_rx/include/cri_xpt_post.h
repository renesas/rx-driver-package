/*****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 1998-2016 CRI Middleware Co., Ltd.
 *
 * Library  : CRI Middleware Library
 * Module   : CRI Common Header / Post-Process
 * File     : cri_xpt_post.h
 * Date     : 2016-04-01
 * Version  : 2.09
 *
 *****************************************************************************/
#ifndef CRI_INCL_CRI_XPT_POST_H
#define CRI_INCL_CRI_XPT_POST_H



#define XPT_STATIC_ASSERT(cond) void xpt_static_assert_dummy_function(void* xpt_static_assert_array[(cond)?1:-1])

#ifdef __cplusplus
extern "C"
{
#endif
XPT_STATIC_ASSERT(1);
#ifdef __cplusplus
}
#endif



#if defined(__MWERKS__) 
#elif defined(__GNUC__)
#endif


#if !defined(CRI_NULL)
#ifdef __cplusplus
#define CRI_NULL	(0)
#else
#define CRI_NULL	((void *)0)
#endif
#endif

#if !defined(CRI_FALSE)
#define CRI_FALSE	(0)
#endif
#if !defined(CRI_TRUE)
#define CRI_TRUE	(1)
#endif

#if !defined(CRI_OFF)
#define CRI_OFF		(0)
#endif
#if !defined(CRI_ON)
#define CRI_ON		(1)
#endif

#if !defined(CRI_OK)
#define CRI_OK		(0)			
#endif
#if !defined(CRI_NG)
#define CRI_NG		(-1)		
#endif


#if !defined(_TYPEDEF_CriUint64Adr)
#define _TYPEDEF_CriUint64Adr
typedef CriUint64				CriUint64Adr;		
#endif

#if !defined(CRI_PTR_TO_UINT64ADR)
#define CRI_PTR_TO_UINT64ADR(ptr)		((CriUint64Adr)((CriUintPtr)(ptr)))	
#endif

#if !defined(CRI_UINT64ADR_TO_PTR)
#define CRI_UINT64ADR_TO_PTR(uint64adr)	((void *)(CriUintPtr)(uint64adr))	
#endif

#if !defined(CRI_XPT_DISABLE_UNPREFIXED_TYPE)


#if !defined(NULL)
#define NULL		(CRI_NULL)
#endif

#if !defined(FALSE)
#define FALSE		(CRI_FALSE)
#endif
#if !defined(TRUE)
#define TRUE		(CRI_TRUE)
#endif

#if !defined(OFF)
#define OFF			(CRI_OFF)
#endif
#if !defined(ON)
#define ON			(CRI_ON)
#endif

#if !defined(OK)
#define OK			(CRI_OK)
#endif
#if !defined(NG)
#define NG			(CRI_NG)
#endif


#if !defined(_TYPEDEF_Uint8)
#define _TYPEDEF_Uint8
typedef CriUint8				Uint8;		
#endif

#if !defined(_TYPEDEF_Sint8)
#define _TYPEDEF_Sint8
typedef CriSint8				Sint8;		
#endif

#if !defined(_TYPEDEF_Uint16)
#define _TYPEDEF_Uint16
typedef CriUint16				Uint16;		
#endif

#if !defined(_TYPEDEF_Sint16)
#define _TYPEDEF_Sint16
typedef CriSint16				Sint16;		
#endif

#if !defined(_TYPEDEF_Uint32)
#define _TYPEDEF_Uint32
typedef CriUint32				Uint32;		
#endif

#if !defined(_TYPEDEF_Sint32)
#define _TYPEDEF_Sint32
typedef CriSint32				Sint32;		
#endif

#if !defined(_TYPEDEF_Uint64)
#define _TYPEDEF_Uint64
typedef CriUint64				Uint64;		
#endif

#if !defined(_TYPEDEF_Sint64)
#define _TYPEDEF_Sint64
typedef CriSint64				Sint64;		
#endif

#if !defined(_TYPEDEF_Uint128)
#define _TYPEDEF_Uint128
typedef CriUint128				Uint128;	
#endif

#if !defined(_TYPEDEF_Sint128)
#define _TYPEDEF_Sint128
typedef CriSint128				Sint128;	
#endif

#if !defined(_TYPEDEF_Float16)
#define _TYPEDEF_Float16
typedef CriFloat16				Float16;	
#endif

#if !defined(_TYPEDEF_Float32)
#define _TYPEDEF_Float32
typedef CriFloat32				Float32;	
#endif

#if !defined(_TYPEDEF_Float64)
#define _TYPEDEF_Float64
typedef CriFloat64				Float64;	
#endif

#if !defined(_TYPEDEF_Fixed32)
#define _TYPEDEF_Fixed32
typedef CriFixed32				Fixed32;	
#endif

#if !defined(_TYPEDEF_Bool) && !defined(Bool)
#define _TYPEDEF_Bool
typedef CriBool					Bool;		
#endif

#if !defined(_TYPEDEF_Char8)
#define _TYPEDEF_Char8
typedef CriChar8				Char8;		
#endif

#if !defined(_TYPEDEF_SintPtr)
#define _TYPEDEF_SintPtr
typedef CriSintPtr				SintPtr;
#endif

#if !defined(_TYPEDEF_UintPtr)
#define _TYPEDEF_UintPtr
typedef CriUintPtr				UintPtr;
#endif

#endif	

#endif	

