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
File        : MULTIEDIT_Private.h
Purpose     : MULTIEDIT include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef MULTIEDIT_PRIVATE_H
#define MULTIEDIT_PRIVATE_H

#include "GUI_Private.h"
#include "WM_Intern.h"

#if GUI_WINSUPPORT

#include <stddef.h>

#include "MULTIEDIT.h"

#if defined(__cplusplus)
  extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/

#define NUM_DISP_MODES 2

#define INVALID_NUMCHARS (1 << 0)
#define INVALID_NUMLINES (1 << 1)
#define INVALID_TEXTSIZE (1 << 2)
#define INVALID_CURSORXY (1 << 3)
#define INVALID_LINEPOSB (1 << 4)

//
// MULTIEDIT properties
//
typedef struct {
  GUI_COLOR        aBkColor    [NUM_DISP_MODES];
  GUI_COLOR        aColor      [NUM_DISP_MODES];
  GUI_COLOR        aCursorColor[2];
  U16              Align;
  const GUI_FONT * pFont;
  U8               HBorder;
} MULTIEDIT_PROPS;

//
// MULTIEDIT object
//
typedef struct {
  WIDGET           Widget;
  MULTIEDIT_PROPS  Props;
  WM_HMEM          hText;
  U16              MaxNumChars;         /* Maximum number of characters including the prompt */
  U16              NumChars;            /* Number of characters (text and prompt) in object */
  U16              NumCharsPrompt;      /* Number of prompt characters */
  U16              NumLines;            /* Number of text lines needed to show all data */
  U16              TextSizeX;           /* Size in X of text depending of wrapping mode */
  U16              BufferSize;
  U16              CursorLine;          /* Number of current cursor line */
  U16              CursorPosChar;       /* Character offset number of cursor */
  U16              CursorPosByte;       /* Byte offset number of cursor */
  I16              CursorPosX;          /* Cursor position in X */
  U16              CursorPosY;          /* Cursor position in Y */
  U16              CacheLinePosByte;    /*  */
  U16              CacheLineNumber;     /*  */
  U16              CacheFirstVisibleLine;
  U16              CacheFirstVisibleByte;
  WM_SCROLL_STATE  ScrollStateV;
  WM_SCROLL_STATE  ScrollStateH;
  U16              Flags;
  U8               InvalidFlags;         /* Flags to save validation status */
  U8               EditMode;
  U8               Radius;               // Currently only used by AppWizard
  WM_HTIMER        hTimer;
  U8               CursorVis;            /* Indicates whether cursor is visible or not*/
  U8               InvertCursor;
  GUI_WRAPMODE     WrapMode;
  int              MotionPosX;
  int              MotionPosY;
  U8               MotionActive;
} MULTIEDIT_OBJ;

/*********************************************************************
*
*       Private (module internal) data
*
**********************************************************************
*/
extern MULTIEDIT_PROPS MULTIEDIT_DefaultProps;

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // MULTIEDIT_PRIVATE_H

/*************************** End of file ****************************/
