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
File        : IMAGE_Private.h
Purpose     : Private IMAGE include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef IMAGE_PRIVATE_H
#define IMAGE_PRIVATE_H

#include "GUI_Private.h"
#include "WM_Intern.h"
#include "IMAGE.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Private config defaults
*
**********************************************************************
*/
#ifndef IMAGE_SUPPORT_TRANSPARENCY
  #define IMAGE_SUPPORT_TRANSPARENCY WM_SUPPORT_TRANSPARENCY
#endif

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  U32 Flags;
} IMAGE_PROPS;

typedef struct {
  WIDGET              Widget;
  IMAGE_PROPS         Props;
  const void        * pData;                                                          // Data pointer of the object to be drawn (Bitmap, BMP, GIF, JPEG, PNG)
  void             (* pfDrawImageAt) (IMAGE_Handle hObj, int xPos, int yPos);         // Object specific (Bitmap, BMP, GIF, JPEG, PNG) code
  void             (* pfPaint)       (IMAGE_Handle hObj);                             // Drawing method specific (Default, Tiled, Magnified) code
  void             (* pfGetImageSize)(IMAGE_Handle hObj, int * pxSize, int * pySize); // Returns the image size of the attached item
  void             (* pfOnTimer)     (IMAGE_Handle hObj);                             // Timer function for animated images (currently only animated GIFs are supported)
  U32                 FileSize;
  //
  // Data items used by IAMGE_GIF.c
  //
  int                 NumImages;    // Number of (sub)images
  int                 CurrentImage; // Image index used for animated images
  GUI_TIMER_HANDLE    hTimer;       // Timer used for animated images
  //
  // Data items used by IAMGE_DTA.c
  //
  GUI_BITMAP          Bitmap;       // Bitmap structure
  GUI_LOGPALETTE      Palette;      // Palette structure
  //
  // Data items used by Image_...Ex() - functions
  //
  void              * pVoid;        // Void pointer passed to GetData() function
  GUI_GET_DATA_FUNC * pfGetData;    // Pointer to GetData() function
  //
  // Alignment (Important: When tiling is active, alignment does not have any effect)
  //
  I16                 xOff, yOff;   // Additional offsets
  U8                  Align;        // Alignment
  //
  // Data items used if memory devices are available and IMAGE_CF_MEMDEV has been set
  //
  #if GUI_SUPPORT_MEMDEV
    GUI_MEMDEV_Handle hMem;
    unsigned          Scale;
    unsigned          Angle;
    U8                Alpha;
    U8                IsDirty;
    //
    // Pointer to scaling function
    //
    void (* pFunc)(GUI_MEMDEV_Handle hSrc, GUI_MEMDEV_Handle hDst, int dx, int dy, int a, int Mag);
  #endif
} IMAGE_OBJ;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define IMAGE_INIT_ID(p) (p->Widget.DebugId = IMAGE_ID)
#else
  #define IMAGE_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  IMAGE_OBJ * IMAGE__LockH(IMAGE_Handle h);
  #define IMAGE_LOCK_H(h)   IMAGE__LockH(h)
#else
  #define IMAGE_LOCK_H(h)   (IMAGE_OBJ *)GUI_LOCK_H(h)
#endif

/*********************************************************************
*
*       Module internal data
*
**********************************************************************
*/
extern IMAGE_PROPS IMAGE__DefaultProps;

/*********************************************************************
*
*       Private functions
*
**********************************************************************
*/
void IMAGE__SetWindowSize(IMAGE_Handle hObj);
void IMAGE__FreeAttached (IMAGE_Handle hObj, int LeaveTimer);
void IMAGE__SetVoid      (IMAGE_Handle hObj, const void * pData);

#endif // GUI_WINSUPPORT
#endif // IMAGE_PRIVATE_H

/*************************** End of file ****************************/
