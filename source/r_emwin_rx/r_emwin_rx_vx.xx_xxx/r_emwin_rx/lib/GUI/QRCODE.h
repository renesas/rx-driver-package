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
File        : QRCODE.h
Purpose     : QRCODE public header file (API)
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef QRCODE_H
#define QRCODE_H

#include "WM.h"
#include "DIALOG_Type.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"

#if GUI_WINSUPPORT

#if defined(__cplusplus)
  extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/*********************************************************************
*
*       QRCODE WiFi encryption types
*
*  Description
*    These macros are to be used for the \a{Encryption} parameter of
*    QRCODE_SetWiFiText().
*/
#define QRCODE_WIFI_WPA    0    // If the WiFi password is WPA encrypted.
#define QRCODE_WIFI_WEP    1    // If the WiFi password is WEP encrypted.

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef WM_HMEM QRCODE_Handle;

/*********************************************************************
*
*       Create function(s)

  Note: the parameters to a create function may vary.
         Some widgets may have multiple create functions
*/
QRCODE_Handle QRCODE_CreateUser    (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, U32 WinFlags, int ExFlags, int Id, const char * pText, int PixelSize, int EccLevel, int Version, int NumExtraBytes);
QRCODE_Handle QRCODE_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void QRCODE_Callback(WM_MESSAGE *pMsg);

/*********************************************************************
*
*       Member functions
*
**********************************************************************
*/
void QRCODE_SetEccLevel  (QRCODE_Handle hObj, int EccLevel);
void QRCODE_SetNumModules(QRCODE_Handle hObj, int NumModules);
void QRCODE_SetPixelSize (QRCODE_Handle hObj, int PixelSize);
void QRCODE_SetText      (QRCODE_Handle hObj, const char * pText);
void QRCODE_SetVersion   (QRCODE_Handle hObj, int Version);
void QRCODE_SetWiFiText  (QRCODE_Handle hObj, const char * pSSID, U8 Encryption, const char * pPassword, U8 Hidden);

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // QRCODE_H

/*************************** End of file ****************************/

