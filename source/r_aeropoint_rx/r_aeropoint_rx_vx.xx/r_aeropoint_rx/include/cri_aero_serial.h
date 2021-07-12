/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : cri_aero_serial.h
 *
 ***************************************************************************/

#ifndef CRI_AERO_SERIAL_H
#define CRI_AERO_SERIAL_H



#ifndef NO_CRI_XPT_H

#include "cri_xpt.h"

#else

#include "stdint.h"
typedef uint32_t CriUint32;
typedef uint8_t CriChar8;
typedef int32_t CriSint32;
typedef uint8_t CriUint8;
typedef uint32_t CriBool;

#define CRI_TRUE (1)
#define CRI_FALSE (0)

#endif


typedef void CRI_AERO_SERIAL_LOG_CALLBACK(CriUint32 Length,const CriChar8 *pText);


void CriAeroSerial_SetLogCallback(CRI_AERO_SERIAL_LOG_CALLBACK *pFunction);


void CriAeroSerial_Start(void);
void CriAeroSerial_Stop(void);

void CriAeroSerial_ChangeVolumeBgm(CriUint32 Vol);
void CriAeroSerial_ChangeVolumeSe(CriUint32 Vol);
void CriAeroSerial_SendMoviePlay(CriUint32 Id);
void CriAeroSerial_SendMovieStop(void);
void CriAeroSerial_SendScrUpdate(void);
void CriAeroSerial_SendShapeOff(CriUint32 Id);
void CriAeroSerial_SendShapeOn(CriUint32 Id);
void CriAeroSerial_SendShowTitle(void);
void CriAeroSerial_SendSlideNo(CriUint32 No);
void CriAeroSerial_SendSoundPlay(CriUint32 Id);
void CriAeroSerial_SendSoundStop(void);
void CriAeroSerial_SendTextClear(void);
void CriAeroSerial_SendTextPrint(CriSint32 Index, CriSint32 X, CriSint32 Y, CriSint32 FontId, CriSint32 ColorId, const CriChar8 *pText);

void CriAeroSerial_LogString(const CriChar8 *pText);

#endif 
