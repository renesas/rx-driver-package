/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2020 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_sound.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_SOUND_H
#define CRI_AERO_SOUND_H

#include <stdio.h>
#include <stdarg.h>

#include "cri_xpt.h"


#ifdef __cplusplus
extern "C" {
#endif

CriBool CRIAPI CriAeroSound_PlaySound( CriUint32 SoundId );

void CRIAPI CriAeroSound_StopSE();

void CRIAPI CriAeroSound_StopBGM();

void CRIAPI CriAeroSound_SetVolumeSE( CriUint32 Volume );

void CRIAPI CriAeroSound_SetVolumeBGM( CriUint32 Volume );

#ifdef __cplusplus
}
#endif

#endif	


