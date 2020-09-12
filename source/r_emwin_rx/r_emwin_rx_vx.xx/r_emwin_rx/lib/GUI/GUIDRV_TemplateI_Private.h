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
File        : GUIDRV_TemplateI_Private.h
Purpose     : Interface definition for GUIDRV_TemplateI driver
---------------------------END-OF-HEADER------------------------------
*/

#include "GUIDRV_TemplateI.h"
#include "GUIDRV_NoOpt_1_8.h"

#ifndef GUIDRV_TEMPLATE_I_PRIVATE_H
#define GUIDRV_TEMPLATE_I_PRIVATE_H

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define PRIVATE_DEVFUNC_ONINITHOOK 0x1000

//
// Use unique context identified
//
#define DRIVER_CONTEXT DRIVER_CONTEXT_TEMPLATE_I

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct DRIVER_CONTEXT DRIVER_CONTEXT;

typedef void (* T_ONINITHOOK)(DRIVER_CONTEXT * pContext);

/*********************************************************************
*
*       MANAGE_VMEM_API
*/
typedef struct {
  //
  // TBD: Add private function pointers...
  //
  int Dummy;
} MANAGE_VMEM_API;

/*********************************************************************
*
*       DRIVER_CONTEXT
*/
struct DRIVER_CONTEXT {
  //
  // Common data
  //
  int xSize, ySize;
  int vxSize, vySize;
  //
  // Driver specific data
  //
  //
  // Accelerators for calculation
  //
  int BytesPerLine;
  int BitsPerPixel;
  //
  // VRAM
  //
  U8 * pVMEM;
  //
  // Pointer to driver internal initialization routine
  //
  void (* pfInit) (GUI_DEVICE * pDevice);
  void (* pfCheck)(GUI_DEVICE * pDevice);
  //
  // API-Tables
  //
  MANAGE_VMEM_API ManageVMEM_API; // Memory management
  GUI_PORT_API    HW_API;         // Hardware routines
};

/*********************************************************************
*
*       LOG2PHYS_xxx
*/
#define LOG2PHYS_X      (                  x    )
#define LOG2PHYS_X_OX   (pContext->xSize - x - 1)
#define LOG2PHYS_X_OY   (                  x    )
#define LOG2PHYS_X_OXY  (pContext->xSize - x - 1)
#define LOG2PHYS_X_OS   (                  y    )
#define LOG2PHYS_X_OSX  (pContext->ySize - y - 1)
#define LOG2PHYS_X_OSY  (                  y    )
#define LOG2PHYS_X_OSXY (pContext->ySize - y - 1)

#define LOG2PHYS_Y      (                  y    )
#define LOG2PHYS_Y_OX   (                  y    )
#define LOG2PHYS_Y_OY   (pContext->ySize - y - 1)
#define LOG2PHYS_Y_OXY  (pContext->ySize - y - 1)
#define LOG2PHYS_Y_OS   (                  x    )
#define LOG2PHYS_Y_OSX  (                  x    )
#define LOG2PHYS_Y_OSY  (pContext->xSize - x - 1)
#define LOG2PHYS_Y_OSXY (pContext->xSize - x - 1)

/*********************************************************************
*
*       _SetPixelIndex_##EXT
*/
#define DEFINE_SETPIXELINDEX(EXT, X_PHYS, Y_PHYS)                                                 \
static void _SetPixelIndex_##EXT(GUI_DEVICE * pDevice, int x, int y, LCD_PIXELINDEX PixelIndex) { \
  DRIVER_CONTEXT * pContext;                                                                      \
                                                                                                  \
  pContext = (DRIVER_CONTEXT *)pDevice->u.pContext;                                               \
  pContext->xSize = pContext->xSize; /* Keep compiler happy */                                    \
  _SetPixelIndex(pDevice, X_PHYS, Y_PHYS, PixelIndex);                                            \
}

/*********************************************************************
*
*       _GetPixelIndex_##EXT
*/
#define DEFINE_GETPIXELINDEX(EXT, X_PHYS, Y_PHYS)                                \
static LCD_PIXELINDEX _GetPixelIndex_##EXT(GUI_DEVICE * pDevice, int x, int y) { \
  LCD_PIXELINDEX PixelIndex;                                                     \
  DRIVER_CONTEXT * pContext;                                                     \
                                                                                 \
  pContext = (DRIVER_CONTEXT *)pDevice->u.pContext;                              \
  pContext->xSize = pContext->xSize; /* Keep compiler happy */                   \
  PixelIndex = _GetPixelIndex(pDevice, X_PHYS, Y_PHYS);                          \
  return PixelIndex;                                                             \
}

/*********************************************************************
*
*       _GetDevProp_##EXT
*/
#define DEFINE_GETDEVPROP(EXT, MX, MY, SWAP)                    \
static I32 _GetDevProp_##EXT(GUI_DEVICE * pDevice, int Index) { \
  switch (Index) {                                              \
  case LCD_DEVCAP_MIRROR_X: return MX;                          \
  case LCD_DEVCAP_MIRROR_Y: return MY;                          \
  case LCD_DEVCAP_SWAP_XY:  return SWAP;                        \
  }                                                             \
  return _GetDevProp(pDevice, Index);                           \
}

/*********************************************************************
*
*       DEFINE_FUNCTIONS
*/
#define DEFINE_FUNCTIONS(EXT, X_PHYS, Y_PHYS, MX, MY, SWAP) \
  DEFINE_SETPIXELINDEX(EXT, X_PHYS, Y_PHYS)                 \
  DEFINE_GETPIXELINDEX(EXT, X_PHYS, Y_PHYS)                 \
  DEFINE_GETDEVPROP(EXT, MX, MY, SWAP)                      \
  DEFINE_GUI_DEVICE_API(EXT)


/*********************************************************************
*
*       Private functions
*
**********************************************************************
*/
void (*GUIDRV__TemplateI_GetDevFunc(GUI_DEVICE ** ppDevice, int Index))(void);
void   GUIDRV__TemplateI_SetOrg    (GUI_DEVICE *  pDevice,  int x, int y);
I32    GUIDRV__TemplateI_GetDevProp(GUI_DEVICE *  pDevice,  int Index);
void   GUIDRV__TemplateI_GetRect   (GUI_DEVICE *  pDevice,  LCD_RECT * pRect);

#endif

/*************************** End of file ****************************/
