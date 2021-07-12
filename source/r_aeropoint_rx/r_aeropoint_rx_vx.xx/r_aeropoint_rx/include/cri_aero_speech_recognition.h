/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2020 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : speech recognition
 * File     : cri_aero_speech_recognition.h
 *
 ****************************************************************************/

#ifndef CRI_AEROLITE_SPEECH_RECOGNITION_H
#define CRI_AEROLITE_SPEECH_RECOGNITION_H


#include "cri_xpt.h"


#ifndef CRI_AERO_SPEECHRECOGNITION_OFF	

#ifdef __cplusplus
extern "C" {
#endif

void criAero_SpeechRecognition_Initialize(void);

void criAero_SpeechRecognition_Finalize(void);

void criAero_SpeechRecognition_Main(void);

CriSint32 criAero_SpeechRecognition_GetVoiceID(void);

#ifdef __cplusplus
}
#endif

#endif	

#endif 

