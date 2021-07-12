/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : cri_aero_stream_internal.h
 *
 ***************************************************************************/
#ifndef CRI_AERO_STREAM_INTERNAL_H
#define CRI_AERO_STREAM_INTERNAL_H


#include "cri_xpt.h"


#define SERVER_FREQUENCY		(100)		
#define RXHCA_NUM_CH			(2)			



void CriAeroStream_Initialize(void);

void CriAeroStream_Finalize(void);

void CriAeroStream_Start(CriUint32 Ch, CriUint32 SdBlockAddr, CriUint32 SizeBytes);

void CriAeroStream_StartId(CriUint32 Ch, CriUint32 Id );

void CriAeroStream_Stop(CriUint32 Ch);

void CriAeroStream_DataRead(CriUint32 SdBlockAddr, CriUint32 SdBlockNum, void *pBuffer);

CriBool CriAeroStream_DataGetStatus(void);

void CriAeroStream_VideoDraw(CriUint32 SdBlockAddr,CriUint32 FrameIndex,void *pFrameAddr,void *pPaletteAddr);

void CriAeroStream_CmtCreate(void);

void CriAeroStream_CmtStop(void);

void CriAeroStream_SetVolume(CriUint32 Ch, CriUint8 Vol);

#endif 


