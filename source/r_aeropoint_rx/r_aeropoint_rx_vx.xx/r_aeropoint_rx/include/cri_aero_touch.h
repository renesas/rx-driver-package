/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2020 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_touch.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_TOUCH_H
#define CRI_AERO_TOUCH_H

#include "cri_xpt.h"

typedef enum {
	CRI_TEVT_NONE = 0,	
	CRI_TEVT_UP,		
	CRI_TEVT_DOWN,		
	CRI_TEVT_MOVE,		
} CriTouchEvent;

typedef enum {
	CRI_BSTATE_NONE = 0,	
	CRI_BSTATE_PRESS,		
	CRI_BSTATE_RELEASE,		
	CRI_BSTATE_CANCEL,		
} CriButtonState;

typedef enum {
	CRI_BMESSAGE_PAGE_CHANGE = 0,	
	CRI_BMESSAGE_PRESS,				
	CRI_BMESSAGE_ON,				
	CRI_BMESSAGE_OFF,				
} CriButtonMessage;

typedef void (*CriAeroTouch_TouchCallback)( CriButtonMessage Message, CriSint32 Param, CriSint32 SoundId, CriSint32 MovieId, CriSint32 OutSignalId, CriSint32 OutOperationId );

#endif	

