/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : LCDConf_user_if.c
 * Version      : 1.00
 * Description  : Display controller configuration (user coding).
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version        Description
 *         : 31.08.2022 6.26.c.1.00    First Release
 *         : 31.03.2023 6.32.a.1.00    Update emWin library to v6.32a.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_emwin_rx_if.h"

#if(EMWIN_LCD_IF == LCD_IF_OTHER)

/* emWin library header */
#include "GUI.h"

#warning "Warning!! Please implement the user program."


/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/******************************************************************************
*
*       Basic defines
*
******************************************************************************/

//
// Physical display size
//
#define XSIZE_PHYS EMWIN_XSIZE_PHYS
#define YSIZE_PHYS EMWIN_YSIZE_PHYS

//
// Color conversion
//
#define COLOR_CONVERSION    (NULL)/* User Value */

//
// Display driver
//
#define DISPLAY_DRIVER      (NULL)/* User Value */

//
// Orientation
//
#define DISPLAY_ORIENTATION (0)/* User Value */

/******************************************************************************
*
*       Configuration checking
*
******************************************************************************/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   COLOR_CONVERSION
  #error Color conversion not defined!
#endif
#ifndef   DISPLAY_DRIVER
  #error No display driver defined!
#endif
#ifndef   DISPLAY_ORIENTATION
  #define DISPLAY_ORIENTATION 0
#endif

/******************************************************************************
*
* User Defines
*
******************************************************************************/



/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/



/**********************************************************************************************************************
 * Function Name: init_display
 * Description  : Initializes the display
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void init_display (void)
{

}
/**********************************************************************************************************************
 * End of function init_display
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: init_controller
 * Description  : Initializes the display controller
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void init_controller(void)
{
    /* Initialize  controller */



    /* Initialize  display */
    init_display();

}
/**********************************************************************************************************************
 * End of function init_controller
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: LCD_X_Config
 * Description  : Called during the initialization process in order to set up the display driver configuration.
 * Arguments    : .
 * Return Value : .
 * Note         : This function is called from the emWin library.
 *********************************************************************************************************************/
void LCD_X_Config(void)
{
  GUI_DEVICE *     p_device;
  GUI_PORT_API     port_api = {0};

  //
  // Set display driver and color conversion for 1st layer
  //
  p_device = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);

  //
  // Display orientation
  //
  if (DISPLAY_ORIENTATION & GUI_SWAP_XY)
  {
    LCD_SetSizeEx (0, YSIZE_PHYS,   XSIZE_PHYS);
    LCD_SetVSizeEx(0, VYSIZE_PHYS,  VXSIZE_PHYS);
  }
  else
  {
    LCD_SetSizeEx (0, XSIZE_PHYS,   YSIZE_PHYS);
    LCD_SetVSizeEx(0, VXSIZE_PHYS,  VYSIZE_PHYS);
  }

  //
  // Function selection, hardware routines (PortAPI) and operation mode (bus, bpp and cache)
  //


}
/**********************************************************************************************************************
 * End of function LCD_X_Config
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: LCD_X_DisplayDriver
 * Description  : This function is called by the display driver for several purposes.
 *                To support the according task the routine needs to be adapted to
 *                the display controller. Please note that the commands marked with
 *                'optional' are not cogently required and should only be adapted if
 *                the display controller supports these features.
 * Arguments    : layer_index - Index of layer to be configured
 *                cmd         - Please refer to the details in the switch statement below
 *                p_data      - Pointer to a LCD_X_DATA structure
 * Return Value : < -1 - Error
 *                  -1 - Command not handled
 *                   0 - Ok
 * Note         : This function is called from the emWin library.
 *********************************************************************************************************************/
int LCD_X_DisplayDriver(unsigned layer_index, unsigned cmd, void * p_data)
{
  int r;

  GUI_USE_PARA(layer_index);
  GUI_USE_PARA(p_data);
  switch (cmd)
  {
  //
  // Required
  //
  case LCD_X_INITCONTROLLER:
  {
    //
    // Called during the initialization process in order to set up the
    // display controller and put it into operation. If the display
    // controller is not initialized by any external routine this needs
    // to be adapted by the customer...
    //
    init_controller();
    return 0;
  }
  case LCD_X_ON:

    break;
  case LCD_X_OFF:

    break;
  default:
    r = -1;
  }
  return r;
}
/**********************************************************************************************************************
 * End of function LCD_X_DisplayDriver
 *********************************************************************************************************************/

#endif /* (EMWIN_LCD_IF == LCD_IF_OTHER) */

