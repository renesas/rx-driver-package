/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2020 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_touch_internal.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_TOUCH_INTERNAL_H
#define CRI_AERO_TOUCH_INTERNAL_H

#include "cri_aero_touch.h"

#ifdef __cplusplus
extern "C" {
#endif

	void CRIAPI CriAeroTouch_Initialize( CriUint32 PanelWidth, CriUint32 PanelHeight, const CriAeroTouch_TouchCallback CallBack );

	void CRIAPI CriAeroTouch_Finalize( void );

	void CRIAPI CriAeroTouch_Timer( CriSint32 SlideNo );

	void CRIAPI CriAeroTouch_Sensing( void );

	void CRIAPI CriAeroTouch_GetRecentTouchInfo( CriTouchEvent *pEvent, CriUint32 *pPosX, CriUint32 *pPosY );

	void CRIAPI CriAeroTouch_ClearTouchInfo();

	void CRIAPI CriAeroTouch_Callback( CriButtonMessage Message, CriSint32 Param, CriSint32 SoundId, CriSint32 MovieId, CriSint32 OutSignalId, CriSint32 OutOperationId );

	void CRIAPI CriAeroTouch_Initialize_AIS( void );

	void CRIAPI CriAeroTouch_Timer_AIS( CriSint32 SlideNo );

	void CRIAPI CriAeroTouch_Sensing_AIS( void );

	void CriAeroTouch_SoftwareDelayMs_AIS(Uint32 Ms);


#ifdef __cplusplus
}
#endif

#endif 
