/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : cri_aero_support_atom_internal.h
 *
 ***************************************************************************/
#ifndef CRI_AERO_SUPPORT_ATOM_INTERNAL_H
#define  CRI_AERO_SUPPORT_ATOM_INTERNAL_H


#include "cri_xpt.h"


typedef void *CriAeroSupportAtomPlayerConfig;

typedef void *CriAeroSupportAtomHandle;

typedef CriSint32 CriAeroSupportAtomStatus;


CriAeroSupportAtomStatus CriAeroSupportAtom_GetStatus(CriAeroSupportAtomHandle Handle);
void CriAeroSupportAtom_ExecuteMain(void);
void CriAeroSupportAtom_Stop(CriAeroSupportAtomHandle Handle);
void CriAeroSupportAtom_SetData(CriAeroSupportAtomHandle Handle,void *pData,CriSint32 Size);
void CriAeroSupportAtom_Start(CriAeroSupportAtomHandle Handle);
void CriAeroSupportAtom_Destroy(CriAeroSupportAtomHandle Handle);
CriSint32 CriAeroSupportAtom_CalculateWorkSizeForHcaPlayer(CriAeroSupportAtomPlayerConfig Config);
CriAeroSupportAtomPlayerConfig CriAeroSupportAtom_GetPlayerConfig(void);
CriAeroSupportAtomHandle CriAeroSupportAtom_CreateHcaPlayer(CriAeroSupportAtomPlayerConfig Config,void *pWork,CriSint32 Size);
void CriAeroSupportAtom_SetDataRequestCallback(CriAeroSupportAtomHandle Handle, void *pFunc,void *pObj);
void CriAeroSupportAtom_SetVolume(CriAeroSupportAtomHandle Handle,CriUint8 Vol);
void CriAeroSupportAtom_DeferCallback(CriAeroSupportAtomHandle Handle);
CriBool CriAeroSupportAtom_IsDataPlaying(CriAeroSupportAtomHandle Handle,void *pData,CriUint32 Size);

void CriAeroSupportAtom_Finalize(void);
CriUint32 CriAeroSupportAtom_CalculateWorkSize(void);
void CriAeroSupportAtom_Initialize(void* work, CriSint32 work_size);


void CriAeroSupportAtom_HardwareInitialize(void);
void CriAeroSupportAtom_HardwareFinalize(void);

void CriAeroSupportAtom_EnableInterrupt_AIS(void);

void CriAeroSupportAtom_Nop_AIS(void);

CriBool CriAeroSupportAtom_CreatePeriodic_AIS(CriUint32 Frequency_hz, void(*pCallback)(void * pData),CriUint32 *pChannel);

CriBool CriAeroSupportAtom_StopPeriodic_AIS(CriUint32 Channel);


#endif 





