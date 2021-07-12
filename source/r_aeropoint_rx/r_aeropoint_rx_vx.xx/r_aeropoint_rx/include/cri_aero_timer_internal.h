/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2020 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_timer_internal.h
 *
 ****************************************************************************/

#ifndef SRC_CRI_AERO_TIMER_INTERNAL_H_
#define SRC_CRI_AERO_TIMER_INTERNAL_H_

#include "cri_xpt.h"

typedef enum {
	CRI_AERO_TIMER_TIMERINDEX_0,
	CRI_AERO_TIMER_TIMERINDEX_1,
	CRI_AERO_TIMER_TIMERINDEX_2,
	CRI_AERO_TIMER_TIMERINDEX_3,
	CRI_AERO_TIMER_TIMERINDEX_4,
	CRI_AERO_TIMER_TIMERINDEX_5,

	CRI_AERO_TIMER_TIMERINDEX_MAX
} CriAeroTimerIndex;

typedef void (*criAeroTimer_Callback)();

#ifdef __cplusplus
extern "C" {
#endif

	void CRIAPI criAeroTimer_Initialize();

	void CRIAPI criAeroTimer_SaveFrequency( CriUint32 Hz );

	CriUint32 CRIAPI criAeroTimer_GetFrequency();

	CriUint32 CRIAPI criAeroTimer_GetCount();

	void CRIAPI criAeroTimer_SystemCallback();

	CriBool CRIAPI criAeroTimer_AddCallback( CriAeroTimerIndex Index, criAeroTimer_Callback Callback, CriUint32 Interval );

	void CRIAPI criAeroTimer_End();

	void CRIAPI criAeroTimer_Initialize_AIS();

	void CRIAPI criAeroTimer_Finalize_AIS();

#ifdef __cplusplus
}
#endif

#endif 
