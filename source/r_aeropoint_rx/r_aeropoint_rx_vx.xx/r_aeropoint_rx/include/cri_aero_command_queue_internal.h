/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2018 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : cri_aero_command_queue_internal.h
 *
 ***************************************************************************/
#ifndef CRI_AERO_COMMAND_QUEUE_INTERNAL_H
#define CRI_AERO_COMMAND_QUEUE_INTERNAL_H


#include "cri_aero_command_queue.h"
#include "cri_aero_command_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

APLCommand *CriAeroCommandQueue_GetBuffer(void);

void CriAeroCommandQueue_Initialize(void);

void CriAeroCommandQueue_Enqueue(APLCommand *pCmd);
APLCommand* CriAeroCommandQueue_Dequeue(void);

void CriAeroCommandQueue_QueueLog(CriUint32 Length, const CriChar8 *pBuf);
void CriAeroCommandQueue_QueueParam0(CriUint32 AplCommand);
void CriAeroCommandQueue_QueueParam1(CriUint32 AplCommand, CriUint32 Param);
void CriAeroCommandQueue_QueueParam6TextPrint(
	CriUint32 AplCommand,
	CriSint32 Param0,
	CriSint32 Param1,
	CriSint32 Param2,
	CriSint32 Param3,
	CriSint32 Param4,
	const CriChar8 *Param5
);
void CriAeroCommandQueue_QueueGotoSlide(CriUint32 no);

CriBool CriAeroCommandQueue_SendLog(const CriChar8 *pText);
void CriAeroCommandQueue_SendPrintPageChange(CriUint32 Slide, CriUint32 Param);
void CriAeroCommandQueue_SendNotifySlideNo(CriUint32 Slideno);
void CriAeroCommandQueue_SendNotifyButtonPress(CriUint32 Slide, CriUint32 Param);

typedef void (*CriAeroCommandQueue_UserCommandFunc)(APLCommand *pCommand);

void CriAeroCommandQueue_SetUserCommandFunc(CriAeroCommandQueue_UserCommandFunc Func);

void CriAeroCommandQueue_CallUserCommandFunc(APLCommand *pCommand);

#ifdef __cplusplus
}
#endif

#endif 

