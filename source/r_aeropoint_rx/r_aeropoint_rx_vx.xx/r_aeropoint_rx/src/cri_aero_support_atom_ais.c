/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2020 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : cri_aero_support_atom_ais.c
 *
 ****************************************************************************/


#include "cri_aero_config_ais.h"
#include "cri_aero_support_atom_internal.h"
#include "machine.h"
#include "r_cmt_rx_if.h"

#if CRI_AERO_CONFIG_AIS_USE_DAMP

#include "cri_atom.h"


void CriAeroSupportAtom_EnableInterrupt_AIS(void)
{
	set_psw(get_psw() | 0x00010000);
}


void CriAeroSupportAtom_Nop_AIS(void)
{
	nop();
}

CriBool CriAeroSupportAtom_CreatePeriodic_AIS(CriUint32 Frequency_hz, void(*pCallback)(void * pData), CriUint32 *pChannel)
{
	return R_CMT_CreatePeriodic(Frequency_hz, pCallback, pChannel);
}

CriBool CriAeroSupportAtom_StopPeriodic_AIS(CriUint32 Channel)
{

	return R_CMT_Stop(Channel);
}


CriAeroSupportAtomStatus CriAeroSupportAtom_GetStatus(CriAeroSupportAtomHandle Handle)
{
	return (CriAeroSupportAtomStatus)criAtomPlayer_GetStatus((CriAtomPlayerHn) Handle);
}

void CriAeroSupportAtom_ExecuteMain(void)
{
	criAtom_ExecuteMain();
}

void CriAeroSupportAtom_Stop(CriAeroSupportAtomHandle Handle)
{
	criAtomPlayer_Stop((CriAtomPlayerHn)Handle);
}

void CriAeroSupportAtom_SetData(CriAeroSupportAtomHandle Handle,void *pData,CriSint32 Size)
{
	criAtomPlayer_SetData((CriAtomPlayerHn)Handle, pData, Size);
}

void CriAeroSupportAtom_Start(CriAeroSupportAtomHandle Handle)
{
	criAtomPlayer_Start((CriAtomPlayerHn)Handle);
}

void CriAeroSupportAtom_Destroy(CriAeroSupportAtomHandle Handle)
{
	criAtomPlayer_Destroy((CriAtomPlayerHn)Handle);
}

CriSint32	CriAeroSupportAtom_CalculateWorkSizeForHcaPlayer(CriAeroSupportAtomPlayerConfig Config)
{
	return criAtomPlayer_CalculateWorkSizeForHcaPlayer((CriAtomHcaPlayerConfig *)Config);
}

CriAeroSupportAtomHandle CriAeroSupportAtom_CreateHcaPlayer(CriAeroSupportAtomPlayerConfig Config,void *pWork,CriSint32 Size)
{
	return (CriAeroSupportAtomHandle)criAtomPlayer_CreateHcaPlayer((CriAtomHcaPlayerConfig *)Config,pWork,Size);
}

void CriAeroSupportAtom_SetDataRequestCallback(CriAeroSupportAtomHandle Handle, void *pFunc,void *pObj)
{
	criAtomPlayer_SetDataRequestCallback((CriAtomPlayerHn)Handle,(CriAtomPlayerDataRequestCbFunc)pFunc,pObj);
}

void CriAeroSupportAtom_SetVolume(CriAeroSupportAtomHandle Handle,CriUint8 Vol)
{
	criAtomPlayer_SetVolume((CriAtomPlayerHn)Handle, Vol);
}

void CriAeroSupportAtom_DeferCallback(CriAeroSupportAtomHandle Handle)
{
	criAtomPlayer_DeferCallback((CriAtomPlayerHn)Handle);
}

CriBool CriAeroSupportAtom_IsDataPlaying(CriAeroSupportAtomHandle Handle,void *pData,CriUint32 Size)
{
	return criAtomPlayer_IsDataPlaying((CriAtomPlayerHn)Handle,pData,Size);
}

#else

void CriAeroSupportAtom_ExecuteMain(void)
{
}

CriBool CriAeroSupportAtom_StopPeriodic_AIS(CriUint32 Channel)
{
	return R_CMT_Stop(Channel);
}

void CriAeroSupportAtom_EnableInterrupt_AIS(void)
{
	set_psw(get_psw() | 0x00010000);
}

void CriAeroSupportAtom_Stop(CriAeroSupportAtomHandle Handle)
{
}

CriSint32	CriAeroSupportAtom_CalculateWorkSizeForHcaPlayer(CriAeroSupportAtomPlayerConfig Config)
{
	return 0;
}

void CriAeroSupportAtom_SetVolume(CriAeroSupportAtomHandle Handle, CriUint8 Vol)
{
}

void CriAeroSupportAtom_Start(CriAeroSupportAtomHandle Handle)
{
}

void CriAeroSupportAtom_SetDataRequestCallback(CriAeroSupportAtomHandle Handle, void *pFunc, void *pObj)
{

}

CriBool CriAeroSupportAtom_CreatePeriodic_AIS(CriUint32 Frequency_hz, void(*pCallback)(void * pData), CriUint32 *pChannel)
{
	return R_CMT_CreatePeriodic(Frequency_hz, pCallback, pChannel);
}

void CriAeroSupportAtom_Destroy(CriAeroSupportAtomHandle Handle)
{
}

CriBool CriAeroSupportAtom_IsDataPlaying(CriAeroSupportAtomHandle Handle, void *pData, CriUint32 Size)
{
	return CRI_FALSE;
}

CriAeroSupportAtomHandle CriAeroSupportAtom_CreateHcaPlayer(CriAeroSupportAtomPlayerConfig Config, void *pWork, CriSint32 Size)
{
	return (CriAeroSupportAtomHandle)1;
}

void CriAeroSupportAtom_DeferCallback(CriAeroSupportAtomHandle Handle)
{
}

void CriAeroSupportAtom_Nop_AIS(void)
{
	nop();
}

CriAeroSupportAtomStatus CriAeroSupportAtom_GetStatus(CriAeroSupportAtomHandle Handle)
{
	return 0;
}

#endif
