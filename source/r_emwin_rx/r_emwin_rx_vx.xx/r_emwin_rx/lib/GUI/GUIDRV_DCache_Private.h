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
File        : GUIDRV_DCache_Private.h
Purpose     : Private declarations for GUIDRV_DCache driver
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUIDRV_DCACHE_PRIVATE_H
#define GUIDRV_DCACHE_PRIVATE_H

#include "GUIDRV_DCache.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Use unique context identified
//
#define DRIVER_CONTEXT DRIVER_CONTEXT_DCACHE

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
/*********************************************************************
*
*       DRIVER_CONTEXT
*/
typedef struct {
  //
  // Data
  //
  int xSize, ySize;       // Display size
  int vxSize, vySize;     // Virtual display size
  int BitsPerPixelDriver;
  int BitsPerPixel;
  int NumColors;
  LCD_PIXELINDEX IndexMask;
  U32 MemSize;
  GUI_RECT rDirty;
  const GUI_DEVICE_API * pMemdev_API;
  //
  // Cache mamagement
  //
  void           (* pfFlush        )(GUI_DEVICE * pDevice);
  void           (* pfSendCacheRect)(GUI_DEVICE * pDevice, int x0, int y0, int x1, int y1);
  //
  // Setting the rectangle to be filled up within the real driver
  //
  void           (* pfSetRect      )(GUI_DEVICE * pDevice, int x0, int y0, int x1, int y1, int OnOff);
  //
  // Mode dependent drawing functions
  //
  void           (* pfDrawBitmap   )(GUI_DEVICE * pDevice, int x0, int y0, int xsize, int ysize, int _BitsPerPixel, int BytesPerLine, const U8 * pData, int Diff, const LCD_PIXELINDEX * pTrans);
  void           (* pfFillRect     )(GUI_DEVICE * pDevice, int x0, int y0, int x1, int y1);
  LCD_PIXELINDEX (* pfGetPixelIndex)(GUI_DEVICE * pDevice, int x, int y);
  void           (* pfSetPixelIndex)(GUI_DEVICE * pDevice, int x, int y, LCD_PIXELINDEX ColorIndex);
  //
  // Request information
  //
  I32            (* pfGetDevProp   )(GUI_DEVICE * pDevice, int Index);
  //
  // Initialization
  //
  void           (* pfInit)         (GUI_DEVICE * pDevice);
  //
  // Conversion array from cache to real display driver
  //
  LCD_PIXELINDEX * pConvert;
  LCD_PIXELINDEX * pIndex;
  //
  // Cache
  //
  U8 * pVRAM;
  U8 * pVRAM_Lock;
  int CacheLocked;
  int CacheStat;
  int CacheDirty;
  //
  // The driver which is used for the actual drawing operations
  //
  GUI_DEVICE * pDriver;
} DRIVER_CONTEXT;

/*********************************************************************
*
*       Private interface
*
**********************************************************************
*/
void GUIDRV_DCache__AddDirtyRect  (DRIVER_CONTEXT * pContext, int x0, int y0, int x1, int y1);
void GUIDRV_DCache__ClearDirtyRect(DRIVER_CONTEXT * pContext);
void GUIDRV_DCache__InitOnce      (GUI_DEVICE * pDevice);

#if defined(__cplusplus)
}
#endif

#endif /* GUIDRV_DCACHE_PRIVATE_H */

/*************************** End of file ****************************/

