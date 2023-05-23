/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2023  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V6.32 - Graphical user interface for embedded applications **
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
SUA period:               2016-12-22 - 2023-12-31
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : MOVIE_Private.h
Purpose     : MOVIE private header file
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef MOVIE_PRIVATE_H
#define MOVIE_PRIVATE_H

#include "WM_Intern.h"
#include "MOVIE.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define MOVIE_FLAG_DOLOOP         (1 << 0)
#define MOVIE_FLAG_TIMER          (1 << 1)
#define MOVIE_FLAG_PROGBARPRESSED (1 << 2)

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  GUI_ANIM_HANDLE hAnim;
  int             Start;
  int             End;
  I16             Pos;
} MOVIE_ANIM_DATA;

typedef struct {
  GUI_COLOR aColor[5];
  int       aPeriod[3];
  int       ySizePanel;
  int       ySizeBar;
  int       Space;
} MOVIE_PROPS;

typedef struct {
  WIDGET           Widget;
  MOVIE_PROPS      Props;
  GUI_MOVIE_INFO   Info;
  GUI_MOVIE_HANDLE hMovie;
  WM_HMEM          ahDrawObj[4];
  int              yOffPanel;
  WM_HTIMER        hTimerPanel;
  MOVIE_ANIM_DATA  AnimShift;
  GUI_MOVIE_FUNC * pfNotify;
  U8               Flags;
  U8               BitmapIndex;
  U8               PanelVisible;
} MOVIE_OBJ;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define MOVIE_INIT_ID(p) (p->Widget.DebugId = WIDGET_TYPE_MOVIE)
#else
  #define MOVIE_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  MOVIE_OBJ * MOVIE_LockH(MOVIE_Handle h);
  #define MOVIE_LOCK_H(h)   MOVIE_LockH(h)
#else
  #define MOVIE_LOCK_H(h)   (MOVIE_OBJ *)WM_LOCK_H(h)
#endif

#endif   /* GUI_WINSUPPORT */
#endif   /* MOVIE_PRIVATE_H */

/*************************** End of file ****************************/
