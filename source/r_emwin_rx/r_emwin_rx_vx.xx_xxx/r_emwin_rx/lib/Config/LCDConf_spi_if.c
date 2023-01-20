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
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : LCDConf_spi_if.c
 * Version      : 1.00
 * Description  : Display controller configuration (use SPI interface).
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version        Description
 *         : 31.08.2022 6.26.c.1.00    First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_emwin_rx_if.h"

#if((EMWIN_LCD_IF == LCD_IF_RSPI)||(EMWIN_LCD_IF == LCD_IF_SCI_SPI))

/* emWin library header */
#include "GUI.h"
#include "GUIDRV_FlexColor.h"

/* FIT module header */
#include "r_gpio_rx_if.h"

#if(EMWIN_LCD_IF == LCD_IF_SCI_SPI)

#include "r_sci_rx_if.h"
#include "r_sci_rx_pinset.h"

#else /* (EMWIN_LCD_IF == LCD_IF_RSPI) */

#include "r_rspi_rx_if.h"
#include "r_rspi_rx_pinset.h"

#endif

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

#if (EMWIN_LCD_IF == LCD_IF_SCI_SPI)

/* SCI_SPI pin setting */
#if (EMWIN_LCD_IF_NUMBER == 0)
#define SCI_PIN_SET     R_SCI_PinSet_SCI0
#elif (EMWIN_LCD_IF_NUMBER == 1)
#define SCI_PIN_SET     R_SCI_PinSet_SCI1
#elif (EMWIN_LCD_IF_NUMBER == 2)
#define SCI_PIN_SET     R_SCI_PinSet_SCI2
#elif (EMWIN_LCD_IF_NUMBER == 3)
#define SCI_PIN_SET     R_SCI_PinSet_SCI3
#elif (EMWIN_LCD_IF_NUMBER == 4)
#define SCI_PIN_SET     R_SCI_PinSet_SCI4
#elif (EMWIN_LCD_IF_NUMBER == 5)
#define SCI_PIN_SET     R_SCI_PinSet_SCI5
#elif (EMWIN_LCD_IF_NUMBER == 6)
#define SCI_PIN_SET     R_SCI_PinSet_SCI6
#elif (EMWIN_LCD_IF_NUMBER == 7)
#define SCI_PIN_SET     R_SCI_PinSet_SCI7
#elif (EMWIN_LCD_IF_NUMBER == 8)
#define SCI_PIN_SET     R_SCI_PinSet_SCI8
#elif (EMWIN_LCD_IF_NUMBER == 9)
#define SCI_PIN_SET     R_SCI_PinSet_SCI9
#elif (EMWIN_LCD_IF_NUMBER == 10)
#define SCI_PIN_SET     R_SCI_PinSet_SCI10
#elif (EMWIN_LCD_IF_NUMBER == 11)
#define SCI_PIN_SET     R_SCI_PinSet_SCI11
#elif (EMWIN_LCD_IF_NUMBER == 12)
#define SCI_PIN_SET     R_SCI_PinSet_SCI12
#endif

#else /* (EMWIN_LCD_IF == LCD_IF_RSPI) */

/* RSPI pin setting */
#if (EMWIN_LCD_IF_NUMBER == 0)
#define RSPI_PIN_SET     R_RSPI_PinSet_RSPI0
#elif (EMWIN_LCD_IF_NUMBER == 1)
#define RSPI_PIN_SET     R_RSPI_PinSet_RSPI1
#elif (EMWIN_LCD_IF_NUMBER == 2)
#define RSPI_PIN_SET     R_RSPI_PinSet_RSPI2
#endif

#endif /* EMWIN_LCD_IF */



/*** Settings for each display controller  ***/

#if (EMWIN_LCD_DRIVER_IC == LCD_DRV_IC_ST7715)
/******************************************************************************
 ST7715
 *****************************************************************************/

/* Display controller class */
#define DISPLAY_CONTROLLER (GUIDRV_FLEXCOLOR_F66709)

/* Color Conversion Routine */
#if (EMWIN_BITS_PER_PIXEL == 24)

#define CONTROLLER_MODE  (GUIDRV_FLEXCOLOR_M24C0B8)
#define COLOR_CONVERSION (GUICC_888)
#define COLMOD_COLOR     (0x06)

#if (EMWIN_GUI_USE_CACHE == 1)
#error "Cache is not available with this setting!"
#endif

#elif (EMWIN_BITS_PER_PIXEL == 18)

#error "This controller does not support this color depth!"

#elif (EMWIN_BITS_PER_PIXEL == 16)

#if (EMWIN_GUI_USE_CACHE == 1)
#define CONTROLLER_MODE  (GUIDRV_FLEXCOLOR_M16C1B8)
#else
#define CONTROLLER_MODE  (GUIDRV_FLEXCOLOR_M16C0B8)
#endif

#define COLOR_CONVERSION (GUICC_565)
#define COLMOD_COLOR     (0x05)

#endif

/* Display orientation */
#if (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_0)
#define DISPLAY_ORIENTATION (0)
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CW)
#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_SWAP_XY)   /* CW */
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_180)
#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_MIRROR_Y)  /* 180 */
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CCW)
#define DISPLAY_ORIENTATION (GUI_MIRROR_Y | GUI_SWAP_XY)   /* CCW */
#else
#error "No display driver orientation defined!"
#endif

/* LCD controller register addresses */
#define ST7715_SWRESET  (0x01)
#define ST7715_SLPOUT   (0x11)
#define ST7715_NORON    (0x13)
#define ST7715_DISPON   (0x29)
#define ST7715_MADCTL   (0x36)
#define ST7715_COLMOD   (0x3A)

/* Offset of the SEG- and COM-lines */
#define FIRST_COM       ((DISPLAY_ORIENTATION & GUI_MIRROR_Y) ? 3 : 1)
#define FIRST_SEG       (2)

#elif (EMWIN_LCD_DRIVER_IC == LCD_DRV_IC_ILI9341)
/******************************************************************************
 ILI9341
 *****************************************************************************/

/* display controller class */
#define DISPLAY_CONTROLLER (GUIDRV_FLEXCOLOR_F66709)

/* Color Conversion Routine */
#if (EMWIN_BITS_PER_PIXEL == 24)

#define CONTROLLER_MODE  (GUIDRV_FLEXCOLOR_M24C0B8)
#define COLOR_CONVERSION (GUICC_888)
#define COLMOD_COLOR     (0x06)

#if (EMWIN_GUI_USE_CACHE == 1)
#error "Cache is not available with this setting!"
#endif

#elif (EMWIN_BITS_PER_PIXEL == 18)

#error "This controller does not support this color depth!"

#elif (EMWIN_BITS_PER_PIXEL == 16)

#if (EMWIN_GUI_USE_CACHE == 1)
#define CONTROLLER_MODE  (GUIDRV_FLEXCOLOR_M16C1B8)
#else
#define CONTROLLER_MODE  (GUIDRV_FLEXCOLOR_M16C0B8)
#endif

#define COLOR_CONVERSION (GUICC_565)
#define COLMOD_COLOR     (0x05)

#endif

/* Display orientation */
#if (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_0)
#define DISPLAY_ORIENTATION (GUI_MIRROR_X)                              /* Default orientation is X-mirror */
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CW)
#define DISPLAY_ORIENTATION (GUI_SWAP_XY)                               /* CW */
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_180)
#define DISPLAY_ORIENTATION (GUI_MIRROR_Y)                              /* 180 */
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CCW)
#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_MIRROR_Y | GUI_SWAP_XY) /* CCW */
#else
#error "No display driver orientation defined!"
#endif

/* LCD controller register addresses */
#define ILI9341_SWRESET  (0x01)
#define ILI9341_SLPOUT   (0x11)
#define ILI9341_NORON    (0x13)
#define ILI9341_DISPON   (0x29)
#define ILI9341_MADCTL   (0x36)
#define ILI9341_COLMOD   (0x3A)

/* Offset of the SEG- and COM-lines */
#define FIRST_COM        (0)
#define FIRST_SEG        (0)

#else /* (EMWIN_LCD_DRIVER_IC == LCD_DRV_IC_OTHER) */

#warning "Warning!! It is necessary to set definition of the display driver ic."

#endif /* EMWIN_LCD_DRIVER_IC */





/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

#if(EMWIN_LCD_IF == LCD_IF_SCI_SPI)

static sci_hdl_t s_lcd_sci_spi_handle;
static sci_err_t s_lcd_sci_spi_err;
static sci_cfg_t s_lcd_sci_spi_setting;

#else /* (EMWIN_LCD_IF == LCD_IF_RSPI) */

static rspi_command_word_t  s_lcd_rspi_command;
static rspi_err_t           s_lcd_rspi_err;
static rspi_handle_t        s_lcd_rspi_handle;
static rspi_chnl_settings_t s_lcd_rspi_setting;

#endif

static volatile bool s_transfer_busy = false;



/**********************************************************************************************************************
 * Function Name: write_command
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void write_command(uint8_t * pCmd, int NumBytes)
{
#if (EMWIN_USE_DATA_CMD_PIN == 1)
    /* DATA/CMD pin is Low */
    R_GPIO_PinDirectionSet(EMWIN_DATA_CMD_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(EMWIN_DATA_CMD_PIN, GPIO_LEVEL_LOW);
#endif

#if (EMWIN_USE_LCD_CS_PIN == 1)
    /* Chip select pin is Low */
    R_GPIO_PinDirectionSet(EMWIN_LCD_CS_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(EMWIN_LCD_CS_PIN, GPIO_LEVEL_LOW);
#endif

    s_transfer_busy = true;

#if (EMWIN_LCD_IF == LCD_IF_SCI_SPI)
    s_lcd_sci_spi_err = R_SCI_Send(s_lcd_sci_spi_handle, pCmd, NumBytes);

    if (SCI_SUCCESS != s_lcd_sci_spi_err)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP();  /* no operation */
        }
    }

#else /* EMWIN_LCD_IF == LCD_IF_RSPI */
    s_lcd_rspi_err = R_RSPI_Write(s_lcd_rspi_handle, s_lcd_rspi_command, pCmd, NumBytes);

    if (RSPI_SUCCESS != s_lcd_rspi_err)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }
#endif

    /* WAIT_LOOP */
    while (true == s_transfer_busy)
    {
        R_BSP_NOP(); /* no operation */
    }

#if (EMWIN_USE_LCD_CS_PIN == 1)
    /* Chip select pin is High */
    R_GPIO_PinDirectionSet(EMWIN_LCD_CS_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(EMWIN_LCD_CS_PIN, GPIO_LEVEL_HIGH);
#endif

#if (EMWIN_USE_DATA_CMD_PIN == 1)
    /* DATA/CMD pin is High */
    R_GPIO_PinDirectionSet(EMWIN_DATA_CMD_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(EMWIN_DATA_CMD_PIN, GPIO_LEVEL_HIGH);
#endif
}
/**********************************************************************************************************************
 * End of function write_command
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: write_data
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void write_data(uint8_t * pData, int NumBytes)
{
#if (EMWIN_USE_DATA_CMD_PIN == 1)
    /* DATA/CMD pin is High */
    R_GPIO_PinDirectionSet(EMWIN_DATA_CMD_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(EMWIN_DATA_CMD_PIN, GPIO_LEVEL_HIGH);
#endif

#if (EMWIN_USE_LCD_CS_PIN == 1)
    /* Chip select pin is Low */
    R_GPIO_PinDirectionSet(EMWIN_LCD_CS_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(EMWIN_LCD_CS_PIN, GPIO_LEVEL_LOW);
#endif

    s_transfer_busy = true;

#if (EMWIN_LCD_IF == LCD_IF_SCI_SPI)

    s_lcd_sci_spi_err = R_SCI_Send(s_lcd_sci_spi_handle, pData, NumBytes);

    if (SCI_SUCCESS != s_lcd_sci_spi_err)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP();  /* no operation */
        }
    }

#else /* EMWIN_LCD_IF == LCD_IF_RSPI */

    s_lcd_rspi_err = R_RSPI_Write(s_lcd_rspi_handle, s_lcd_rspi_command, pData, NumBytes);

    if (RSPI_SUCCESS != s_lcd_rspi_err)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }

#endif

    /* WAIT_LOOP */
    while (true == s_transfer_busy)
    {
        R_BSP_NOP(); /* no operation */
    }

#if (EMWIN_USE_LCD_CS_PIN == 1)
    /* Chip select pin is High */
    R_GPIO_PinDirectionSet(EMWIN_LCD_CS_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(EMWIN_LCD_CS_PIN, GPIO_LEVEL_HIGH);
#endif
}
/**********************************************************************************************************************
 * End of function write_data
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: write8_a0
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void write8_a0(uint8_t Cmd)
{
    write_command( &Cmd, 1);
}
/**********************************************************************************************************************
 * End of function write8_a0
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: writem8_a0
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void writem8_a0(uint8_t * pCmd, int NumItems)
{
    write_command(pCmd, NumItems);
}
/**********************************************************************************************************************
 * End of function writem8_a0
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: write8_a1
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void write8_a1(uint8_t Data)
{
    write_data( &Data, 1);
}
/**********************************************************************************************************************
 * End of function write8_a1
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: writem8_a1
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void writem8_a1(uint8_t * pData, int NumItems)
{
    write_data(pData, NumItems);
}
/**********************************************************************************************************************
 * End of function writem8_a1
 *********************************************************************************************************************/

#if (EMWIN_LCD_DRIVER_IC == LCD_DRV_IC_ST7715) || (EMWIN_LCD_DRIVER_IC == LCD_DRV_IC_ILI9341)
/**********************************************************************************************************************
 * Function Name: read8_a0
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static uint8_t read8_a0(void)
{
    return 0x00;  /* Dummy */
}
/**********************************************************************************************************************
 * End of function read8_a0
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: readm8_a0
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void readm8_a0(uint8_t * pData, int NumItems)
{
    GUI_USE_PARA(pData);
    GUI_USE_PARA(NumItems);
}
/**********************************************************************************************************************
 * End of function readm8_a0
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: read8_a1
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static uint8_t read8_a1(void)
{
    return 0x00;  /* Dummy */
}
/**********************************************************************************************************************
 * End of function read8_a1
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: readm8_a1
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void readm8_a1(uint8_t * pData, int NumItems)
{
    GUI_USE_PARA(pData);
    GUI_USE_PARA(NumItems);
}
/**********************************************************************************************************************
 * End of function readm8_a1
 *********************************************************************************************************************/
#else /* (EMWIN_LCD_DRIVER_IC == LCD_DRV_IC_OTHER) */

#warning "Warning!! It is necessary to set definition of the display driver ic."

#endif


/**********************************************************************************************************************
 * Function Name: init_display
 * Description  : Should initialize the display controller.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void init_display(void)
{
#if (EMWIN_LCD_DRIVER_IC == LCD_DRV_IC_ST7715)

    /*** Preset the output ports for i/o pins prior to setting as outputs ***/

#if (EMWIN_USE_DATA_CMD_PIN == 1)
    /* DATA/CMD pin is High */
    R_GPIO_PinWrite(EMWIN_DATA_CMD_PIN, GPIO_LEVEL_HIGH);
    R_GPIO_PinDirectionSet(EMWIN_DATA_CMD_PIN, GPIO_DIRECTION_OUTPUT);
#endif

#if (EMWIN_USE_LCD_CS_PIN == 1)
    /* Chip select pin is High */
    R_GPIO_PinWrite(EMWIN_LCD_CS_PIN, GPIO_LEVEL_HIGH);
    R_GPIO_PinDirectionSet(EMWIN_LCD_CS_PIN, GPIO_DIRECTION_OUTPUT);
#endif

#if (EMWIN_USE_DISP_SIGNAL_PIN == 1)
    /* Reset pin is High */
    R_GPIO_PinWrite(EMWIN_DISP_SIGNAL_PIN, GPIO_LEVEL_HIGH);
    R_GPIO_PinDirectionSet(EMWIN_DISP_SIGNAL_PIN, GPIO_DIRECTION_OUTPUT);
#endif

    /** Back-light off **/

#if (EMWIN_USE_BACKLIGHT_PIN == 1)
    /* Back-light pin is Low */
    R_GPIO_PinWrite(EMWIN_BACKLIGHT_PIN, GPIO_LEVEL_LOW);
    R_GPIO_PinDirectionSet(EMWIN_BACKLIGHT_PIN, GPIO_DIRECTION_OUTPUT);
#endif

    GUI_X_Delay(10); /* Wait 10ms */

    /** Clear reset line to reset display **/

#if (EMWIN_USE_DISP_SIGNAL_PIN == 1)
    /* Reset pin is Low */
    R_GPIO_PinDirectionSet(EMWIN_DISP_SIGNAL_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(EMWIN_DISP_SIGNAL_PIN, GPIO_LEVEL_LOW);
#endif

    GUI_X_Delay(10); /* Wait 10ms */

    /** Release reset **/

#if (EMWIN_USE_DISP_SIGNAL_PIN == 1)
    /* Reset pin is High */
    R_GPIO_PinDirectionSet(EMWIN_DISP_SIGNAL_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(EMWIN_DISP_SIGNAL_PIN, GPIO_LEVEL_HIGH);
#endif

    /** Software reset **/
    write8_a0(ST7715_SWRESET);

    GUI_X_Delay(120); /* Wait 120ms */

    /** out of sleep mode **/
    write8_a0(ST7715_SLPOUT);

    GUI_X_Delay(120); /* Wait 120ms */

    /** Set color mode **/
    write8_a0(ST7715_COLMOD);

    write8_a1(COLMOD_COLOR); /* color format */

    GUI_X_Delay(10); /* Wait 10ms */

    /** Set Normal mode **/
    write8_a0(ST7715_NORON);

    GUI_X_Delay(10); /* Wait 10ms */

    /* Display controller on, backlight is turned on later (LCD_X_DisplayDriver() with cmd LCD_X_ON) */
    write8_a0(ST7715_DISPON);

    GUI_X_Delay(120); /* Wait 120ms */

#elif (EMWIN_LCD_DRIVER_IC == LCD_DRV_IC_ILI9341)
    /*** Preset the output ports for i/o pins prior to setting as outputs ***/

#if (EMWIN_USE_DATA_CMD_PIN == 1)
    /* DATA/CMD pin is High */
    R_GPIO_PinWrite(EMWIN_DATA_CMD_PIN, GPIO_LEVEL_HIGH);
    R_GPIO_PinDirectionSet(EMWIN_DATA_CMD_PIN, GPIO_DIRECTION_OUTPUT);
#endif

#if (EMWIN_USE_LCD_CS_PIN == 1)
    /* Chip select pin is High */
    R_GPIO_PinWrite(EMWIN_LCD_CS_PIN, GPIO_LEVEL_HIGH);
    R_GPIO_PinDirectionSet(EMWIN_LCD_CS_PIN, GPIO_DIRECTION_OUTPUT);
#endif

#if (EMWIN_USE_DISP_SIGNAL_PIN == 1)
    /* Reset pin is High */
    R_GPIO_PinWrite(EMWIN_DISP_SIGNAL_PIN, GPIO_LEVEL_HIGH);
    R_GPIO_PinDirectionSet(EMWIN_DISP_SIGNAL_PIN, GPIO_DIRECTION_OUTPUT);
#endif

    /** Back-light off **/

#if (EMWIN_USE_BACKLIGHT_PIN == 1)
    /* Back-light pin is Low */
    R_GPIO_PinWrite(EMWIN_BACKLIGHT_PIN, GPIO_LEVEL_LOW);
    R_GPIO_PinDirectionSet(EMWIN_BACKLIGHT_PIN, GPIO_DIRECTION_OUTPUT);
#endif

    GUI_X_Delay(10); /* Wait 10ms */

    /** Clear reset line to reset display **/

#if (EMWIN_USE_DISP_SIGNAL_PIN == 1)
    /* Reset pin is Low */
    R_GPIO_PinDirectionSet(EMWIN_DISP_SIGNAL_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(EMWIN_DISP_SIGNAL_PIN, GPIO_LEVEL_LOW);
#endif

    GUI_X_Delay(10); /* Wait 10ms */

    /** Release reset **/

#if (EMWIN_USE_DISP_SIGNAL_PIN == 1)
    /* Reset pin is High */
    R_GPIO_PinDirectionSet(EMWIN_DISP_SIGNAL_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(EMWIN_DISP_SIGNAL_PIN, GPIO_LEVEL_HIGH);
#endif

    /** Software reset **/
    write8_a0(ILI9341_SWRESET);

    GUI_X_Delay(120); /* Wait 120ms */

    /** out of sleep mode **/
    write8_a0(ILI9341_SLPOUT);

    GUI_X_Delay(120); /* Wait 120ms */

    /** Set color mode **/
    write8_a0(ILI9341_COLMOD);

    write8_a1(COLMOD_COLOR); /* color format */

    GUI_X_Delay(10); /* Wait 10ms */

    /** Set Normal mode **/
    write8_a0(ILI9341_NORON);

    GUI_X_Delay(10); /* Wait 10ms */

    /* Display controller on, back-light is turned on later (LCD_X_DisplayDriver() with cmd LCD_X_ON) */
    write8_a0(ILI9341_DISPON);

    GUI_X_Delay(120); /* Wait 120ms */

#else /* (EMWIN_LCD_DRIVER_IC == LCD_DRV_IC_OTHER) */

#warning "Warning!! It is necessary to set definition of the display driver ic."

#endif /* EMWIN_LCD_DRIVER_IC */
}
/**********************************************************************************************************************
 * End of function init_display
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: com_end_callback
 * Description  : communication completion callback
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void com_end_callback(void * pData)
{
    GUI_USE_PARA(pData);
    s_transfer_busy = false;
}
/**********************************************************************************************************************
 * End of function com_end_callback
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: init_controller
 * Description  : Should initialize the display controller.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void init_controller(void)
{
#if (EMWIN_LCD_IF == LCD_IF_SCI_SPI)

    /* Initialize SCI pins */
    SCI_PIN_SET();

    /* Open the SSPI module */
    s_lcd_sci_spi_setting.sspi.bit_rate     = EMWIN_LCD_BAUDRATE;
    s_lcd_sci_spi_setting.sspi.int_priority = 5;
    s_lcd_sci_spi_setting.sspi.invert_data  = false;
    s_lcd_sci_spi_setting.sspi.msb_first    = true;
    s_lcd_sci_spi_setting.sspi.spi_mode     = SCI_SPI_MODE_0;

    /* Open the SSPI channel using the API function */
    s_lcd_sci_spi_err = R_SCI_Open( EMWIN_LCD_IF_NUMBER,        /* SSPI channel number */
            SCI_MODE_SSPI,             /* SCI mode                                     */
            &s_lcd_sci_spi_setting,    /* Address of the SSPI settings structure.      */
            &com_end_callback,         /* Address of user-defined callback function.   */
            &s_lcd_sci_spi_handle);    /* Address of where the handle is to be stored  */

    /* If there were an error this would demonstrate error detection of API calls. */
    if (SCI_SUCCESS != s_lcd_sci_spi_err)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }

#else /* (EMWIN_LCD_IF == LCD_IF_RSPI) */

    /* Initialize RSPI pins */
    RSPI_PIN_SET();

    /* Open the RSPI module */
    s_lcd_rspi_command.cpha          = RSPI_SPCMD_CPHA_SAMPLE_ODD;
    s_lcd_rspi_command.cpol          = RSPI_SPCMD_CPOL_IDLE_LO;
    s_lcd_rspi_command.br_div        = RSPI_SPCMD_BR_DIV_1;
    s_lcd_rspi_command.ssl_assert    = RSPI_SPCMD_ASSERT_SSL0;
    s_lcd_rspi_command.ssl_negate    = RSPI_SPCMD_SSL_KEEP;
    s_lcd_rspi_command.bit_length    = RSPI_SPCMD_BIT_LENGTH_8;
    s_lcd_rspi_command.bit_order     = RSPI_SPCMD_ORDER_MSB_FIRST;
    s_lcd_rspi_command.next_delay    = RSPI_SPCMD_NEXT_DLY_SSLND;
    s_lcd_rspi_command.ssl_neg_delay = RSPI_SPCMD_SSL_NEG_DLY_SSLND;
    s_lcd_rspi_command.clock_delay   = RSPI_SPCMD_CLK_DLY_SPCKD;
    s_lcd_rspi_command.dummy         = RSPI_SPCMD_DUMMY;

    /* Configure the RSPI settings structure */
    s_lcd_rspi_setting.bps_target        = EMWIN_LCD_BAUDRATE;   /* Ask for 8Mbps bit-rate.           */
    s_lcd_rspi_setting.master_slave_mode = RSPI_MS_MODE_MASTER;  /* Configure the RSPI as SPI Master. */
    s_lcd_rspi_setting.gpio_ssl          = RSPI_IF_MODE_4WIRE;   /* Set interface mode to 4-wire.     */

    /* Open the RSPI channel using the API function */
    s_lcd_rspi_err = R_RSPI_Open(EMWIN_LCD_IF_NUMBER,        /* RSPI channel number */
            &s_lcd_rspi_setting,    /* Address of the RSPI settings structure.      */
            s_lcd_rspi_command,
            &com_end_callback,      /* Address of user-defined callback function.   */
            &s_lcd_rspi_handle);    /* Address of where the handle is to be stored. */

    /* If there were an error this would demonstrate error detection of API calls. */
    if (RSPI_SUCCESS != s_lcd_rspi_err)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }

#endif /* EMWIN_LCD_IF */

    /* Initialize display */
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
    GUI_PORT_API     port_api = { 0 };
    CONFIG_FLEXCOLOR config   = { 0 };
    int32_t          swap_xy_ex;
    int              Temp;

    /* Set display driver and color conversion for 1st layer */
    p_device = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, COLOR_CONVERSION, 0, 0);

    /* Display driver configuration */
    swap_xy_ex = LCD_GetSwapXYEx(0);
    if (swap_xy_ex)
    {
        LCD_SetSizeEx (0, EMWIN_YSIZE_PHYS, EMWIN_XSIZE_PHYS);
        LCD_SetVSizeEx(0, EMWIN_YSIZE_PHYS, EMWIN_XSIZE_PHYS);
    }
    else
    {
        LCD_SetSizeEx (0, EMWIN_XSIZE_PHYS, EMWIN_YSIZE_PHYS);
        LCD_SetVSizeEx(0, EMWIN_XSIZE_PHYS, EMWIN_YSIZE_PHYS);
    }

    /* Function selection, hardware routines (PortAPI) and operation mode (bus, bpp and cache) */
    port_api.pfWrite8_A0  = write8_a0;
    port_api.pfWrite8_A1  = write8_a1;
    port_api.pfWriteM8_A0 = writem8_a0;
    port_api.pfWriteM8_A1 = writem8_a1;
    port_api.pfRead8_A0   = read8_a0;
    port_api.pfRead8_A1   = read8_a1;
    port_api.pfReadM8_A0  = readm8_a0;
    port_api.pfReadM8_A1  = readm8_a1;

    /* Set controller and operation mode */
    GUIDRV_FlexColor_SetFunc(p_device, &port_api, DISPLAY_CONTROLLER, CONTROLLER_MODE);

    /* Orientation */
    config.Orientation = DISPLAY_ORIENTATION;

    config.FirstCOM = FIRST_COM;
    config.FirstSEG = FIRST_SEG;

    if (DISPLAY_ORIENTATION & GUI_SWAP_XY)
    {
        Temp            = config.FirstCOM;
        config.FirstCOM = config.FirstSEG;
        config.FirstSEG = Temp;
    }

    GUIDRV_FlexColor_Config(p_device, &config);
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
    int32_t r = 0;

    GUI_USE_PARA(layer_index);
    GUI_USE_PARA(p_data);
    switch (cmd)
    {
        /* Required */
        case LCD_X_INITCONTROLLER:
        {
            /* Called during the initialization process in order to set up the
             * display controller and put it into operation. If the display
             * controller is not initialized by any external routine this needs
             * to be adapted by the customer... */
            init_controller();
            return 0;
        }
        case LCD_X_ON:
        {

            /* Turn on back light */
            R_GPIO_PinDirectionSet(EMWIN_BACKLIGHT_PIN, GPIO_DIRECTION_OUTPUT);
            R_GPIO_PinWrite(EMWIN_BACKLIGHT_PIN, GPIO_LEVEL_HIGH);
            break;
        }
        case LCD_X_OFF:
        {
            /* Turn off back light */
            R_GPIO_PinDirectionSet(EMWIN_BACKLIGHT_PIN, GPIO_DIRECTION_OUTPUT);
            R_GPIO_PinWrite(EMWIN_BACKLIGHT_PIN, GPIO_LEVEL_LOW);
            break;
        }
        default:
            r = -1;
    }
    return r;
}
/**********************************************************************************************************************
 * End of function LCD_X_DisplayDriver
 *********************************************************************************************************************/

#endif /* (EMWIN_LCD_IF == LCD_IF_SCI_RSPI)||(EMWIN_LCD_IF == LCD_IF_SCI_SPI) */

