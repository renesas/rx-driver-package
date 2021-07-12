/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2020 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Player
 * File     : cri_aero_player.h
 *
 ****************************************************************************/

#ifndef CRI_AERO_PLAYER_H
#define CRI_AERO_PLAYER_H

#include "cri_xpt.h"
#include "cri_aero_main.h"

typedef enum {
    CRI_AERO_MESSAGE_SLIDE_PRE_CHANGE = 0,	
    CRI_AERO_MESSAGE_SLIDE_POST_CHANGE,		

    CRI_AERO_MESSAGE_BUTTON_PRESS,			
} CriAeroMessage;

typedef enum {
    CRI_AERO_RESPONSED_NONE         = 0,          

    CRI_AERO_RESPONSED_SLIDE_PRE_CHANGE   = (1 <<  0),  
    CRI_AERO_RESPONSED_SLIDE_POST_CHANGE  = (1 <<  1),  
    CRI_AERO_RESPONSED_BUTTON_PRESS       = (1 <<  2),  
    CRI_AERO_RESPONSED_PLAY_SOUND         = (1 <<  3),  
    CRI_AERO_RESPONSED_PLAY_MOVIE         = (1 <<  4),  
} CriAeroResponsed;

typedef CriAeroResponsed (*CriAeroPlayer_Callback_SlideBased1)( CriAeroMessage Message, CriSint32 Param, CriSint32 SoundId, CriSint32 MovieId );

#ifdef __cplusplus
extern "C" {
#endif

CriBool CRIAPI CriAeroPlayer_Initialize( struct CriAerolData *pData );

CriBool CRIAPI CriAeroPlayer_Main();

void CRIAPI CriAeroPlayer_Finalize();

void CRIAPI CriAeroPlayer_SetCallback( const CriAeroPlayer_Callback_SlideBased1 callback );

CriSint32 CRIAPI CriAeroPlayer_ReadSlide( CriSint32 SlideNoBased1 );

CriSint32 CRIAPI CriAeroPlayer_GetSlideNo(void);

CriSint32 CRIAPI CriAeroPlayer_SetShapeVisible( CriSint32 ShapeId, CriBool IsVisible );


#ifdef __cplusplus
}
#endif

#endif 

