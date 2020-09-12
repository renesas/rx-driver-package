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
File        : GUIDRV_Lin_Opt_16.h
Purpose     : Optimized routines, included by GUIDRV_Lin_..._16.c
---------------------------END-OF-HEADER------------------------------
*/

/*********************************************************************
*
*       Static functions
*
**********************************************************************
*/
/*********************************************************************
*
*       _FillRectOpt16
*
* Purpose:
*   Optimized filling routine for 16 bpp
*/
static void _FillRectOpt16(GUI_DEVICE * pDevice, int x0, int y0, int x1, int y1) {
  DRIVER_CONTEXT * pContext;
  U32 * pDest;
  U32 * pDest0;
  U32 Off, OffLine;
  int RemPixels, NumLines, RemLines, RemItems;
  U32 Data, ColorMask;
  LCD_PIXELINDEX ColorIndex;

  pContext   = (DRIVER_CONTEXT *)pDevice->u.pContext;
  ColorIndex = LCD__GetColorIndex();
  Off        = XY2OFF32(pContext->vxSizePhys, x0, y0);
  pDest0     = OFF2PTR32(pContext->VRAMAddr, Off);
  RemPixels  = x1 - x0 + 1;
  NumLines   = y1 - y0 + 1;
  OffLine    = pContext->vxSizePhys >> 1;
  pDest      = NULL;
  if (GUI_pContext->DrawMode & LCD_DRAWMODE_XOR) {
    //
    // First DWORD
    //
    if (x0 & 1) {
      for (RemLines = NumLines, pDest = pDest0; RemLines; RemLines--) {
        Data = READ_MEM32P(pDest);
        #if (LCD_ENDIAN_BIG == 0)
          Data ^= 0xFFFF0000;
        #else
          Data ^= 0xFFFF;
        #endif
        WRITE_MEM32P(pDest, Data);
        pDest += OffLine;
      }
      pDest0++;
      RemPixels--;
    }
    //
    // Complete DWORDS
    //
    if (RemPixels >= 2) {
      for (RemLines = NumLines; RemLines; RemLines--) {
        RemItems = RemPixels;
        pDest    = pDest0 + OffLine * (RemLines - 1);
        do {
          Data  = READ_MEM32P(pDest);
          Data ^= 0xFFFFFFFF;
          WRITE_MEM32P(pDest, Data);
          pDest++;
          RemItems -= 2;
        } while (RemItems >= 2);
      }
      pDest0 = pDest;
      RemPixels -= (RemPixels >> 1) << 1;
    }
    //
    // Last DWORD
    //
    if (RemPixels > 0) {
      for (RemLines = NumLines, pDest = pDest0; RemLines; RemLines--) {
        Data = READ_MEM32P(pDest);
        #if (LCD_ENDIAN_BIG == 0)
          Data ^= 0xFFFF;
        #else
          Data ^= 0xFFFF0000;
        #endif
        WRITE_MEM32P(pDest, Data);
        pDest += OffLine;
      }
    }
  } else {
    //
    // First DWORD
    //
    if (x0 & 1) {
      for (RemLines = NumLines, pDest = pDest0; RemLines; RemLines--) {
        Data = READ_MEM32P(pDest);
        #if (LCD_ENDIAN_BIG == 0)
          Data &= 0xFFFF;
          Data |= (((U32)ColorIndex) << 16);
        #else
          Data &= 0xFFFF0000;
          Data |= ColorIndex;
        #endif
        WRITE_MEM32P(pDest, Data);
        pDest += OffLine;
      }
      pDest0++;
      RemPixels--;
    }
    //
    // Complete DWORDS
    //
    ColorMask = ColorIndex * 0x00010001;
    if (RemPixels >= 16) {
      RemPixels -= 16;
      
      for (RemLines = NumLines; RemLines; RemLines--) {
        RemItems = RemPixels;
        pDest    = pDest0 + OffLine * (RemLines - 1);
        do {
          WRITE_MEM32P(pDest,     ColorMask);
          WRITE_MEM32P(pDest + 1, ColorMask);
          WRITE_MEM32P(pDest + 2, ColorMask);
          WRITE_MEM32P(pDest + 3, ColorMask);
          WRITE_MEM32P(pDest + 4, ColorMask);
          WRITE_MEM32P(pDest + 5, ColorMask);
          WRITE_MEM32P(pDest + 6, ColorMask);
          WRITE_MEM32P(pDest + 7, ColorMask);
          pDest    += 8;
          RemItems -= 16;
        } while (RemItems >= 0);
      }
      pDest0 = pDest;

      RemPixels += 16;
      RemPixels -= (RemPixels >> 4) << 4;
    }
    if (RemPixels >= 2) {
      for (RemLines = NumLines; RemLines; RemLines--) {
        RemItems = RemPixels;
        pDest    = pDest0 + OffLine * (RemLines - 1);
        do {
          WRITE_MEM32P(pDest, ColorMask);
          pDest++;
          RemItems -= 2;
        } while (RemItems >= 2);
      }
      pDest0 = pDest;
      RemPixels -= (RemPixels >> 1) << 1;
    }
    //
    // Last DWORD
    //
    if (RemPixels > 0) {
      for (RemLines = NumLines, pDest = pDest0; RemLines; RemLines--) {
        Data = READ_MEM32P(pDest);
        #if (LCD_ENDIAN_BIG == 0)
          Data &= 0xFFFF0000;
          Data |= ColorIndex;
        #else
          Data &= 0xFFFF;
          Data |= (((U32)ColorIndex) << 16);
        #endif
        WRITE_MEM32P(pDest, Data);
        pDest += OffLine;
      }
    }
  }
}

/*************************** End of file ****************************/
