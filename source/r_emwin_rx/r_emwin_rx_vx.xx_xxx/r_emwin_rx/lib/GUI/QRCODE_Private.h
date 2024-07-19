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

** emWin V6.34 - Graphical user interface for embedded applications **
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
File        : QRCODE_Private.h
Purpose     : Internal header file
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef QRCODE_PRIVATE_H
#define QRCODE_PRIVATE_H

#include "WM_Intern.h"
#include "QRCODE.h"

#if (GUI_WINSUPPORT)

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  GUI_COLOR Color;
  GUI_COLOR BkColor;
} QRCODE_PROPS;

typedef struct {
  WIDGET       Widget;
  QRCODE_PROPS Props;
  GUI_HMEM     hMem;
  WM_HMEM      hText;
  int          PixelSize;
  int          EccLevel;
  int          Version;
} QRCODE_OBJ;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define QRCODE_INIT_ID(p) (p->Widget.DebugId = WIDGET_TYPE_QRCODE)
#else
  #define QRCODE_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  QRCODE_OBJ * QRCODE_LockH(QRCODE_Handle h);
  #define QRCODE_LOCK_H(h) QRCODE_LockH(h)
#else
  #define QRCODE_LOCK_H(h) (QRCODE_OBJ *)WM_LOCK_H(h)
#endif

/*********************************************************************
*
*       Public data (internal defaults)
*
**********************************************************************
*/
extern QRCODE_PROPS QRCODE__DefaultProps;

#endif  // (GUI_SUPPORT_MEMDEV && GUI_WINSUPPORT)
#endif  // QRCODE_PRIVATE_H
