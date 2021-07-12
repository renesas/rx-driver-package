/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2020 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_testio.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_TESTIO_H
#define CRI_AERO_TESTIO_H

#include <stdio.h>
#include <stdarg.h>

#include "cri_xpt.h"

enum
{
	CRI_AERO_TESTIO_LED_GREEN = 0,
	CRI_AERO_TESTIO_LED_BLUE,
	CRI_AERO_TESTIO_LED_YELLOW,
	CRI_AERO_TESTIO_LED_RED,
	CRI_AERO_TESTIO_LED_WHITE,

	CRI_AERO_TESTIO_LED_NUM
};

typedef CriUint32 CriAeroTestIOLedColor;
#ifdef __cplusplus
extern "C" {
#endif

void CriAeroTestIO_SetLed(CriAeroTestIOLedColor led, CriBool bit);
	

CriBool CriAeroTestIO_CheckLed(CriAeroTestIOLedColor led);

CriBool CriAeroTestIO_IsSwitch();

#ifdef __cplusplus
}
#endif

#endif	


