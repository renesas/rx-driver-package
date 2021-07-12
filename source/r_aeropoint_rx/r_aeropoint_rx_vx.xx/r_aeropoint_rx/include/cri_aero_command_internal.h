/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2019 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Header
 * File     : cri_aero_command_internal.h
 *
 ****************************************************************************/

#ifndef SRC_CRI_AERO_COMMAND_INTERNAL_H_
#define SRC_CRI_AERO_COMMAND_INTERNAL_H_


#include "cri_xpt.h"



#define RCV_UNIT_DATA	(512L)

#define _PACKET_MODE_TAG		( 0 )

#define _PACKET_MODE_DATA		( 1 )

#define _PACKET_MODE_RLE		( 2 )



 
typedef struct APLCommand
{
	CriUint32 Tag;			
	CriUint32 IDCounter;	
	CriUint32 Command;		
	CriSint32 Buffer[13];	
} APLCommand;

typedef struct APLCommandForCaptureInfo
{
	CriUint32 Tag;			
	CriUint32 IDCounter;	
	CriUint32 Command;		
	CriUint32 Width;		
	CriUint32 Height;		
	CriUint32 PaletteSize;	
} APLCommandForCaptureInfo;

typedef struct Rle2Bin512State
{
	CriBool bInit;
	CriSint32 readCntOfRbuffer;
	CriSint32 gainCntofRbuffer;
	CriSint32 readCntOfRle;
	CriSint32 gainCntOfRle;
	CriSint32 lastData;
	CriBool bContinuation;
}Rle2Bin512State;

typedef struct CriAeroLiteCommandStruct {
	CriUint8  recvbuf[RCV_UNIT_DATA];
	CriUint8  rlebuf[RCV_UNIT_DATA];
	CriSint32 phase;
	CriSint32 wait;
	CriBool   recieved;
	CriUint32 tag_mode;
	CriUint32 gain;
	CriUint32 retry;

	CriUint32 targetDataSize;
	CriUint32 targetDataSizeRoundUp;
	CriUint32 targetSDAddr;
	CriUint32 targetDataGain;
	CriUint32 targetRleGain;

	Rle2Bin512State rleState;

} CriAeroLiteCommandStruct;

extern CriAeroLiteCommandStruct s_commandStruct;

#ifdef __cplusplus
extern "C" {
#endif

void func_packet_mode_tag(void);
void func_packet_mode_data(void);
void func_packet_mode_rle(void);

void criAeroSocket_SendInServiceMessage(void);


#ifdef __cplusplus
}
#endif

#endif 
