/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2019  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.50 - Graphical user interface for embedded applications **
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
License model:            License and Service Agreement, signed December 16th, 2016 and Amendment No. 1, signed May 16th, 2019
License valid for:        RX65N, RX651, RX72M, RX72N, RX661, RX66N
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2016-12-22 - 2019-12-31
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : KNOB.h
Purpose     : KNOB include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef KNOB_PRIVATE_H
#define KNOB_PRIVATE_H

#include "WM_Intern.h"
#include "KNOB.h"

#if (GUI_SUPPORT_MEMDEV && GUI_WINSUPPORT)

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  I32 Snap;          // Position where the knob snaps
  I32 Period;        // Time it takes to stop the knob in ms
  GUI_COLOR BkColor; // The Bk color
  I32 Offset;        // the offset
  I32 MinRange;
  I32 MaxRange;
  I32 MinVRange;
  I32 MaxVRange;
  I32 TickSize;      // Minimum movement range in 1/10 of degree
  I32 KeyValue;      // Range of movement for one key push
  U8  Invert;
} KNOB_PROPS;

typedef struct {
  WIDGET Widget;
  WIDGET_DRAW_ITEM_FUNC * pfOwnerDraw;
  void (* pfRotate)(GUI_MEMDEV_Handle hSrc, GUI_MEMDEV_Handle hDst, int dx, int dy, int a, int Mag);
  KNOB_PROPS Props;
  WM_HMEM hContext;
  I32 Angle;
  I32 Value;
  int xSize;
  int ySize;
  I32 AngleRotate;
  GUI_MEMDEV_Handle hMemSrc;
  GUI_MEMDEV_Handle hMemDst;
  GUI_MEMDEV_Handle hMemBk;
  I32               VRangeABS;
} KNOB_OBJ;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define KNOB_INIT_ID(p) p->Widget.DebugId = KNOB_ID
#else
  #define KNOB_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  KNOB_OBJ * KNOB_LockH(KNOB_Handle h);
  #define KNOB_LOCK_H(h)   KNOB_LockH(h)
#else
  #define KNOB_LOCK_H(h)   (KNOB_OBJ *)GUI_LOCK_H(h)
#endif

/*********************************************************************
*
*       Module internal data
*
**********************************************************************
*/
extern KNOB_PROPS KNOB__DefaultProps;

#endif   // (GUI_SUPPORT_MEMDEV && GUI_WINSUPPORT)
#endif   // KNOB_PRIVATE_H
