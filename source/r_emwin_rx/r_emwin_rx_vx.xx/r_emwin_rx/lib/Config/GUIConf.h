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
File        : GUIConf.h
Purpose     : GUI compile time configuration
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUICONF_H
#define GUICONF_H

#define GUI_NUM_LAYERS            2
#define GUI_USE_ARGB              1

#define GUI_OS                    1

#define GUI_WINSUPPORT            1  /* Use window manager if true (1)  */
#define GUI_SUPPORT_TOUCH         1  /* Support a touch screen */
#define GUI_SUPPORT_MOUSE         1  /* Support a mouse */
#define GUI_SUPPORT_MEMDEV        1  /* Support for memory devices */
#define GUI_SUPPORT_BIDI          0  /* Save RAM */

                                       /* Request of a function pointer for... */
#define LCD_DEVFUNC_SET_BUFFERPTR 0x29 /* ...setting an array of buffer pointers */

#endif /* GUICONF_H */

/*************************** End of file ****************************/
