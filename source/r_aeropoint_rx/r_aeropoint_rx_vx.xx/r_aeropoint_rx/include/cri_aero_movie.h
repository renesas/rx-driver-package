/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2020 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_movie.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_MOVIE_H
#define CRI_AERO_MOVIE_H

#include <stdio.h>
#include <stdarg.h>

#include "cri_xpt.h"


#ifdef __cplusplus
extern "C" {
#endif

CriBool CRIAPI CriAeroMovie_Play( CriUint32 MovieId );

void CRIAPI CriAeroMovie_Stop();

CriBool CRIAPI CriAeroMovie_IsPlay();

#ifdef __cplusplus
}
#endif

#endif	


