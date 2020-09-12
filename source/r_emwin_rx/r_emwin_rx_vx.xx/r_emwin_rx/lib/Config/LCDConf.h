/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

***** emWin - Graphical user interface for embedded applications *****
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.


----------------------------------------------------------------------
File        : LCDConf.h
Purpose     : SEGGER internal standard configuration file
----------------------------------------------------------------------
*/

#ifndef LCDCONF_H
#define LCDCONF_H

#ifndef WIN32
#include "dave_driver.h"
#endif

#define LCD_MIRROR 1

void   * LCDCONF_GetWriteBuffer(void);
void     LCDCONF_DisableDave2D (void);
void     LCDCONF_EnableDave2D  (void);
void   * LCDCONF_GetBufferAddr(void);
unsigned LCDCONF_GetDaveActive(void);

#ifndef WIN32
d2_device * LCDCONF_GetD2(void);
#endif

#endif /* LCDCONF_H */

/*************************** End of file ****************************/
