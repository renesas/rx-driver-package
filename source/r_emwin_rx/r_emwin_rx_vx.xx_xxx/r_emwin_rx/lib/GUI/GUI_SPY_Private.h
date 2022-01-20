/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2021  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V6.22 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only  be used  in accordance  with  a license  and should  not be  re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Software GmbH
Licensed to:              Renesas Electronics Europe GmbH, Arcadiastrasse 10, 40472 Duesseldorf, Germany
Licensed SEGGER software: emWin
License number:           GUI-00678
License model:            License and Service Agreement, signed December 16th, 2016, Amendment No. 1 signed May 16th, 2019 and Amendment No. 2, signed September 20th, 2021 by Carsten Jauch, Managing Director
License valid for:        RX (based on RX-V1, RX-V2 or RX-V3)
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2016-12-22 - 2022-12-31
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : GUI_SPY_Private.h
Purpose     : Private header file for Spy
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_SPY_PRIVATE_H
#define GUI_SPY_PRIVATE_H

#include "GUI_Private.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#ifndef   GUI_SPY_COM_BUFFER_SIZE
  #define GUI_SPY_COM_BUFFER_SIZE 1000
#endif

#define JOB_REQUEST_STATUS  1
#define JOB_REQUEST_VERSION 2
#define JOB_REQUEST_WINDOWS 3
#define JOB_REQUEST_INPUT   4
#define JOB_REQUEST_BMP     5
#define JOB_QUIT            6

#define INPUT_PID    1
#define INPUT_KEY    2
#define INPUT_MTOUCH 3

#define TRY(x) x; if (r) return 1

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct SPY_CONTEXT SPY_CONTEXT;

struct SPY_CONTEXT{
  GUI_tSend pfSend;
  GUI_tRecv pfRecv;
  void * pConnectInfo;
  U8 acBuffer[GUI_SPY_COM_BUFFER_SIZE];
  int NumBytesInBuffer;
  int Result;
  int (* pfFlush)   (SPY_CONTEXT * pContext);
  int (* pfStoreU8) (SPY_CONTEXT * pContext,  U8 Byte);
  int (* pfStoreU16)(SPY_CONTEXT * pContext, U16 Data);
  int (* pfStoreU32)(SPY_CONTEXT * pContext, U32 Data);
  int (* pfRead)    (SPY_CONTEXT * pContext, U8 * buf, int len);
};

typedef struct {
  U16 Type;
  U32 Time;
  union {
    GUI_KEY_STATE    StateKEY;
    GUI_PID_STATE    StatePID;
    GUI_MTOUCH_STATE StateMTOUCH;
  } u;
} SPY_INPUT;



#endif // GUI_SPY_PRIVATE_H

/*************************** End of file ****************************/
