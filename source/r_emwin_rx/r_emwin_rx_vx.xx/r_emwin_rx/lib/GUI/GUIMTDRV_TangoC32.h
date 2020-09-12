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
File        : GUIMTDRV_TangoC32.h
Purpose     : Interface definition for GUIMTDRV_TangoC32 driver
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUIMTDRV_TANGOC32_H
#define GUIMTDRV_TANGOC32_H

#include "GUI_Type.h"

#if defined(__cplusplus)
//extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  int LayerIndex;
  //
  // Initialization
  //
  void (* pf_I2C_Init)(unsigned char SlaveAddr);
  //
  // Read- and write access
  //
  int (* pf_I2C_Read  )(unsigned char * pData, int Start, int Stop);
  int (* pf_I2C_ReadM )(unsigned char * pData, int NumItems, int Start, int Stop);
  int (* pf_I2C_Write )(unsigned char Data, int Start, int Stop);
  int (* pf_I2C_WriteM)(unsigned char * pData, int NumItems, int Start, int Stop);
  //
  // 7-bit address to be used to address the I2C slave
  //
  U8 SlaveAddr;
} GUIMTDRV_TANGOC32_CONFIG;

/*********************************************************************
*
*       Interface
*
**********************************************************************
*/
int GUIMTDRV_TangoC32_Init(GUIMTDRV_TANGOC32_CONFIG * pConfig);
int GUIMTDRV_TangoC32_Exec(void);

#endif /* GUIMTDRV_TANGOC32_H */

/*************************** End of file ****************************/
