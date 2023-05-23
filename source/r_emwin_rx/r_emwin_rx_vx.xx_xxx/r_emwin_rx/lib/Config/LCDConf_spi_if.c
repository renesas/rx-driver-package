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
 * File Name    : LCDConf_spi_if.c
 * Version      : 1.00
 * Description  : Display controller configuration (use SPI interface).
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version        Description
 *         : 31.08.2022 6.26.c.1.00    First Release
 *         : 31.03.2023 6.32.a.1.00    Update emWin library to v6.32a.
 *                                     Added support for runtime orientation.
 *                                     Added support for DTC and DMAC in SPI interface.
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

#if (EMWIN_SELECT_DMAC_DTC == 1)
#include "r_dtc_rx_if.h"
#elif (EMWIN_SELECT_DMAC_DTC == 2)
#include "r_dmaca_rx_if.h"
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

/* RSPI pin & transfer source setting */
#if (EMWIN_LCD_IF_NUMBER == 0)
#define RSPI_PIN_SET    R_RSPI_PinSet_RSPI0
#define DTCE_RSPI_SPTI  (DTCE_RSPI0_SPTI0)
#define DTCE_RSPI_SPRI  (DTCE_RSPI0_SPRI0)
#define IR_RSPI_SPTI    (IR_RSPI0_SPTI0)
#define IR_RSPI_SPRI    (IR_RSPI0_SPRI0)
#elif (EMWIN_LCD_IF_NUMBER == 1)
#define RSPI_PIN_SET    R_RSPI_PinSet_RSPI1
#define DTCE_RSPI_SPTI  (DTCE_RSPI1_SPTI1)
#define DTCE_RSPI_SPRI  (DTCE_RSPI1_SPRI1)
#define IR_RSPI_SPTI    (IR_RSPI1_SPTI1)
#define IR_RSPI_SPRI    (IR_RSPI1_SPRI1)
#elif (EMWIN_LCD_IF_NUMBER == 2)
#define RSPI_PIN_SET    R_RSPI_PinSet_RSPI2
#define DTCE_RSPI_SPTI  (DTCE_RSPI2_SPTI2)
#define DTCE_RSPI_SPRI  (DTCE_RSPI2_SPRI2)
#define IR_RSPI_SPTI    (IR_RSPI2_SPTI2)
#define IR_RSPI_SPRI    (IR_RSPI2_SPRI2)
#endif

/* RSPI Version */
/* Note:
 * It is necessary to change the transfer size depending on the RSPI version. */
#if defined BSP_MCU_RX23T || defined BSP_MCU_RX23W || defined BSP_MCU_RX24T || defined BSP_MCU_RX24U || \
    defined BSP_MCU_RX64M || defined BSP_MCU_RX71M || defined BSP_MCU_RX110 || defined BSP_MCU_RX111 || \
    defined BSP_MCU_RX113 || defined BSP_MCU_RX130 || defined BSP_MCU_RX230 || defined BSP_MCU_RX231 || \
    defined BSP_MCU_RX23E_A

#define RSPI_VERSION        (1)

/* Data transfer size */
#if (EMWIN_SELECT_DMAC_DTC == 1)
#define RSPI_DMA_TRANS_SIZE (DTC_DATA_SIZE_WORD)
#elif (EMWIN_SELECT_DMAC_DTC == 2)
#define RSPI_DMA_TRANS_SIZE (DMACA_DATA_SIZE_WORD)
#endif

/* Intermediate buffer for RSPI version 1 */
#define BYTE_PER_PIXEL (EMWIN_BITS_PER_PIXEL / 8)

#if (EMWIN_XSIZE_PHYS >= EMWIN_YSIZE_PHYS)
#if ((EMWIN_XSIZE_PHYS * BYTE_PER_PIXEL) >= (EMWIN_BITS_PER_PIXEL * 32))
#define RSPI_WRITE_BUF_SIZE (EMWIN_XSIZE_PHYS * BYTE_PER_PIXEL)
#else
#define RSPI_WRITE_BUF_SIZE (EMWIN_BITS_PER_PIXEL * 32)
#endif
#else
#if ((EMWIN_YSIZE_PHYS * BYTE_PER_PIXEL) >= (EMWIN_BITS_PER_PIXEL * 32))
#define RSPI_WRITE_BUF_SIZE (EMWIN_YSIZE_PHYS * BYTE_PER_PIXEL)
#else
#define RSPI_WRITE_BUF_SIZE (EMWIN_BITS_PER_PIXEL * 32)
#endif
#endif

#else /* Later Device */

#define RSPI_VERSION        (2)

/* Data transfer size */
#if (EMWIN_SELECT_DMAC_DTC == 1)
#define RSPI_DMA_TRANS_SIZE (DTC_DATA_SIZE_BYTE)
#elif (EMWIN_SELECT_DMAC_DTC == 2)
#define RSPI_DMA_TRANS_SIZE (DMACA_DATA_SIZE_BYTE)
#endif

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
#define DISPLAY_ORIENTATION_0 (GUI_ROTATION_0)
#define DISPLAY_ORIENTATION_1 (GUI_ROTATION_CW)
#define DISPLAY_ORIENTATION_2 (GUI_ROTATION_180)
#define DISPLAY_ORIENTATION_3 (GUI_ROTATION_CCW)
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CW)
#define DISPLAY_ORIENTATION_0 (GUI_ROTATION_CW)
#define DISPLAY_ORIENTATION_1 (GUI_ROTATION_180)
#define DISPLAY_ORIENTATION_2 (GUI_ROTATION_CCW)
#define DISPLAY_ORIENTATION_3 (GUI_ROTATION_0)
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_180)
#define DISPLAY_ORIENTATION_0 (GUI_ROTATION_180)
#define DISPLAY_ORIENTATION_1 (GUI_ROTATION_CCW)
#define DISPLAY_ORIENTATION_2 (GUI_ROTATION_0)
#define DISPLAY_ORIENTATION_3 (GUI_ROTATION_CW)
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CCW)
#define DISPLAY_ORIENTATION_0 (GUI_ROTATION_CCW)
#define DISPLAY_ORIENTATION_1 (GUI_ROTATION_0)
#define DISPLAY_ORIENTATION_2 (GUI_ROTATION_CW)
#define DISPLAY_ORIENTATION_3 (GUI_ROTATION_180)
#else
#error "No display driver orientation defined!"
#endif

/* Setting the base point of the LCD */
#define DISPLAY_ORIENTATION (0)

/* LCD controller register addresses */
#define ST7715_SWRESET  (0x01)
#define ST7715_SLPOUT   (0x11)
#define ST7715_NORON    (0x13)
#define ST7715_DISPON   (0x29)
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
#define DISPLAY_ORIENTATION_0 (GUI_ROTATION_0)
#define DISPLAY_ORIENTATION_1 (GUI_ROTATION_CW)
#define DISPLAY_ORIENTATION_2 (GUI_ROTATION_180)
#define DISPLAY_ORIENTATION_3 (GUI_ROTATION_CCW)
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CW)
#define DISPLAY_ORIENTATION_0 (GUI_ROTATION_CW)
#define DISPLAY_ORIENTATION_1 (GUI_ROTATION_180)
#define DISPLAY_ORIENTATION_2 (GUI_ROTATION_CCW)
#define DISPLAY_ORIENTATION_3 (GUI_ROTATION_0)
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_180)
#define DISPLAY_ORIENTATION_0 (GUI_ROTATION_180)
#define DISPLAY_ORIENTATION_1 (GUI_ROTATION_CCW)
#define DISPLAY_ORIENTATION_2 (GUI_ROTATION_0)
#define DISPLAY_ORIENTATION_3 (GUI_ROTATION_CW)
#elif (EMWIN_DISPLAY_ORIENTATION == ORIENTATION_CCW)
#define DISPLAY_ORIENTATION_0 (GUI_ROTATION_CCW)
#define DISPLAY_ORIENTATION_1 (GUI_ROTATION_0)
#define DISPLAY_ORIENTATION_2 (GUI_ROTATION_CW)
#define DISPLAY_ORIENTATION_3 (GUI_ROTATION_180)
#else
#error "No display driver orientation defined!"
#endif

/* Setting the base point of the LCD */
#define DISPLAY_ORIENTATION (GUI_MIRROR_X)

/* LCD controller register addresses */
#define ILI9341_SWRESET  (0x01)
#define ILI9341_SLPOUT   (0x11)
#define ILI9341_NORON    (0x13)
#define ILI9341_DISPON   (0x29)
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


#if (EMWIN_LCD_IF == LCD_IF_RSPI)
#if (EMWIN_SELECT_DMAC_DTC == 1)

static dtc_transfer_data_cfg_t s_data_cfg_w;
static dtc_transfer_data_cfg_t s_data_cfg_r;
static dtc_cmd_arg_t           s_args_w;
static dtc_cmd_arg_t           s_args_r;
static uint32_t                s_reg_buff;
static uint32_t                s_rev_dummy;
static volatile uint32_t s_transfer_data[4];
static volatile uint32_t s_receive_data[4];

#if (RSPI_VERSION == 1)
static uint16_t rspi_word_buffer[RSPI_WRITE_BUF_SIZE];
#endif

#elif (EMWIN_SELECT_DMAC_DTC == 2)

static dmaca_transfer_data_cfg_t   s_data_cfg_dmac_w;
static dmaca_transfer_data_cfg_t   s_data_cfg_dmac_r;
static dmaca_stat_t                s_stat_dmaca_w;
static dmaca_stat_t                s_stat_dmaca_r;
static uint32_t                    s_reg_buff;
static uint32_t                    s_rev_dummy;

#if (RSPI_VERSION == 1)
static uint16_t rspi_word_buffer[RSPI_WRITE_BUF_SIZE];
#endif

#endif /* EMWIN_SELECT_DMAC_DTC */
#endif /* EMWIN_LCD_IF == LCD_IF_RSPI */


#if (EMWIN_LCD_IF == LCD_IF_RSPI)

#if (RSPI_VERSION == 1) && (EMWIN_SELECT_DMAC_DTC > 0)
/**********************************************************************************************************************
 * Function Name: copy_to_word_buffer
 * Description  : Copy byte size data to word size buffer.
 * Arguments    : uint8_t *  p_src     ; Source address
 *              : uint16_t * p_dst     ; Destination address
 *              : int        num_bytes ; Source data length
 * Return Value : none
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(copy_to_word_buffer)
void copy_to_word_buffer(uint8_t * p_src, uint16_t * p_dst, int num_bytes)
{
    int i;
    for (i = 0; i < num_bytes; i++)
    {
        p_dst[i] = (uint16_t) p_src[i];
    }
}
/**********************************************************************************************************************
 * End of function copy_to_word_buffer
 *********************************************************************************************************************/
#endif /* (RSPI_VERSION == 1) && (EMWIN_SELECT_DMAC_DTC > 0) */

#if (EMWIN_SELECT_DMAC_DTC == 1)
/**********************************************************************************************************************
 * Function Name: enable_dtc
 * Description  : Enables DTC for RSPI transmission.
 * Arguments    : void * p_data    ; Source data address
 *              : int    num_bytes ; Source data length
 * Return Value : none
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(enable_dtc)
void enable_dtc(void * p_data, int num_bytes)
{
    /* Transfer setting */
    s_data_cfg_w.source_addr    = (uint32_t) p_data;
    s_data_cfg_w.dest_addr      = (uint32_t) (s_reg_buff);
    s_data_cfg_w.transfer_count = (uint32_t) num_bytes;

    s_data_cfg_r.source_addr    = (uint32_t) (s_reg_buff);
    s_data_cfg_r.dest_addr      = (uint32_t) &s_rev_dummy;
    s_data_cfg_r.transfer_count = (uint32_t) num_bytes;

    /* Create write/read setting */
    R_DTC_Create(DTCE_RSPI_SPTI,
            (dtc_transfer_data_t*) &s_transfer_data[0],
            (dtc_transfer_data_cfg_t*) &s_data_cfg_w,
            0);

    R_DTC_Create(DTCE_RSPI_SPRI,
            (dtc_transfer_data_t*) &s_receive_data[0],
            (dtc_transfer_data_cfg_t*) &s_data_cfg_r,
            0);

    /* Enable write/read setting */
    R_DTC_Control(DTC_CMD_ACT_SRC_ENABLE, NULL, &s_args_w);

    R_DTC_Control(DTC_CMD_ACT_SRC_ENABLE, NULL, &s_args_r);

    /* Start DTC */
    R_DTC_Control(DTC_CMD_DTC_START, NULL, NULL);
}
/**********************************************************************************************************************
 * End of function enable_dtc
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: disable_dtc
 * Description  : Disables DTC for RSPI transmission.
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(disable_dtc)
void disable_dtc(void)
{
    /* Stop DTC */
    R_DTC_Control(DTC_CMD_DTC_STOP, NULL, NULL);

    /* Disable write setting */
    R_DTC_Control(DTC_CMD_ACT_SRC_DISABLE, NULL, &s_args_w);

    /* Disable read setting */
    R_DTC_Control(DTC_CMD_ACT_SRC_DISABLE, NULL, &s_args_r);
}
/**********************************************************************************************************************
 * End of function disable_dtc
 *********************************************************************************************************************/

#elif (EMWIN_SELECT_DMAC_DTC == 2)
/**********************************************************************************************************************
 * Function Name: dma_callback_w
 * Description  : the RSPI transmit end callback function via DMAC.
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
void dma_callback_w(void)
{
    dmaca_stat_t   stat_dmaca;

    /* DMACA transfer end status clear */
    R_DMACA_Control(EMWIN_DMAC_NUMBER, DMACA_CMD_DTIF_STATUS_CLR, &stat_dmaca);

    /* SPI transmit end process */
    R_RSPI_DisableSpti(s_lcd_rspi_handle);
    R_RSPI_IntSptiIerClear(s_lcd_rspi_handle);

    return;
}
/**********************************************************************************************************************
 * End of function dma_callback_w
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: dma_callback_r
 * Description  : the RSPI receive end callback function via DMAC.
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
void dma_callback_r(void)
{
    dmaca_stat_t   stat_dmaca;

    /* DMACA transfer end status clear */
    R_DMACA_Control(EMWIN_DMAC_NUMBER2, DMACA_CMD_DTIF_STATUS_CLR, &stat_dmaca);

    /* SPI receive end process */
    R_RSPI_DisableRSPI(s_lcd_rspi_handle);
    R_RSPI_IntSpriIerClear(s_lcd_rspi_handle);

    /* Set transfer completion flag */
    s_transfer_busy = false;

    return;
}
/**********************************************************************************************************************
 * End of function dma_callback_r
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: enable_dmac
 * Description  : Enables DMAC for RSPI transmission.
 * Arguments    : void * p_data    ; Source data address
 *              : int_t  num_bytes ; Source data length
 * Return Value : none
 **********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(enable_dmac)
void enable_dmac(void * p_data, int num_bytes)
{
    /* Transfer setting */
    s_data_cfg_dmac_w.p_src_addr     = p_data;
    s_data_cfg_dmac_w.p_des_addr     = (void *) s_reg_buff;
    s_data_cfg_dmac_w.transfer_count = (uint32_t) num_bytes;

    s_data_cfg_dmac_r.p_src_addr     = (void *) s_reg_buff;
    s_data_cfg_dmac_r.p_des_addr     = &s_rev_dummy;
    s_data_cfg_dmac_r.transfer_count = (uint32_t) num_bytes;

    /* Create write/read setting */
    R_DMACA_Create(EMWIN_DMAC_NUMBER, &s_data_cfg_dmac_w);
    R_DMACA_Create(EMWIN_DMAC_NUMBER2, &s_data_cfg_dmac_r);

    /* Enable write/read setting */
    R_DMACA_Control(EMWIN_DMAC_NUMBER, DMACA_CMD_ENABLE, &s_stat_dmaca_w);
    R_DMACA_Control(EMWIN_DMAC_NUMBER2, DMACA_CMD_ENABLE, &s_stat_dmaca_r);
}
/**********************************************************************************************************************
 * End of function enable_dmac
 *********************************************************************************************************************/
#else

/* Transfer method is interrupt */

#endif /* EMWIN_SELECT_DMAC_DTC */

#endif /* EMWIN_LCD_IF == LCD_IF_RSPI */
/**********************************************************************************************************************
 * Function Name: write_command
 * Description  : Command part transmission processing.
 * Arguments    : uint8_t * p_cmd     ; Source data address
 *              : int_t     num_bytes ; Source data length
 * Return Value : none
 *********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(write_command)
void write_command(uint8_t * p_cmd, int num_bytes)
{

#if (EMWIN_LCD_IF == LCD_IF_RSPI)
#if (EMWIN_SELECT_DMAC_DTC == 1)

#if (RSPI_VERSION == 1)
    copy_to_word_buffer(p_cmd, rspi_word_buffer, num_bytes);
    enable_dtc(rspi_word_buffer, num_bytes);
#else
    enable_dtc(p_cmd, num_bytes);
#endif

#elif (EMWIN_SELECT_DMAC_DTC == 2)

#if (RSPI_VERSION == 1)
    copy_to_word_buffer(p_cmd, rspi_word_buffer, num_bytes);
    enable_dmac(rspi_word_buffer, num_bytes);
#else
    enable_dmac(p_cmd, num_bytes);
#endif

#else
    /* Transfer method is interrupt */
#endif
#endif

#if (EMWIN_USE_DATA_CMD_PIN == 1)
    /* DATA/CMD pin is Low */
    R_GPIO_PinWrite(EMWIN_DATA_CMD_PIN, GPIO_LEVEL_LOW);
#endif

#if (EMWIN_USE_LCD_CS_PIN == 1)
    /* Chip select pin is Low */
    R_GPIO_PinWrite(EMWIN_LCD_CS_PIN, GPIO_LEVEL_LOW);
#endif

    s_transfer_busy = true;

#if (EMWIN_LCD_IF == LCD_IF_SCI_SPI)

#if (EMWIN_SELECT_DMAC_DTC == 1)
    R_BSP_InterruptsDisable();
#endif

    s_lcd_sci_spi_err = R_SCI_Send(s_lcd_sci_spi_handle, p_cmd, num_bytes);

#if (EMWIN_SELECT_DMAC_DTC == 1)
    R_BSP_InterruptsEnable();
#endif

    if (SCI_SUCCESS != s_lcd_sci_spi_err)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP();  /* no operation */
        }
    }

#else /* EMWIN_LCD_IF == LCD_IF_RSPI */
#if (RSPI_VERSION == 1) && (EMWIN_SELECT_DMAC_DTC > 0)
    s_lcd_rspi_err = R_RSPI_Write(s_lcd_rspi_handle, s_lcd_rspi_command, rspi_word_buffer, num_bytes);
#else
    s_lcd_rspi_err = R_RSPI_Write(s_lcd_rspi_handle, s_lcd_rspi_command, p_cmd, num_bytes);
#endif

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
    R_GPIO_PinWrite(EMWIN_LCD_CS_PIN, GPIO_LEVEL_HIGH);
#endif

#if (EMWIN_USE_DATA_CMD_PIN == 1)
    /* DATA/CMD pin is High */
    R_GPIO_PinWrite(EMWIN_DATA_CMD_PIN, GPIO_LEVEL_HIGH);
#endif

#if (EMWIN_LCD_IF == LCD_IF_RSPI)
#if (EMWIN_SELECT_DMAC_DTC == 1)
    disable_dtc();
#elif (EMWIN_SELECT_DMAC_DTC == 2)
    /* Disable processing not required when using DMAC */
#else
    /* Transfer method is interrupt */
#endif
#endif

}
/**********************************************************************************************************************
 * End of function write_command
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: write_data
 * Description  : Data part transmission processing.
 * Arguments    : uint8_t * p_data    ; Source data address
 *              : int_t     num_bytes ; Source data length
 * Return Value : none
 *********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE(write_data)
void write_data(uint8_t * p_data, int num_bytes)
{

#if (EMWIN_LCD_IF == LCD_IF_RSPI)
#if (EMWIN_SELECT_DMAC_DTC == 1)


#if (RSPI_VERSION == 1)
    copy_to_word_buffer(p_data, rspi_word_buffer, num_bytes);
    enable_dtc(rspi_word_buffer, num_bytes);
#else
    enable_dtc(p_data, num_bytes);
#endif

#elif (EMWIN_SELECT_DMAC_DTC == 2)

#if (RSPI_VERSION == 1)
    copy_to_word_buffer(p_data, rspi_word_buffer, num_bytes);
    enable_dmac(rspi_word_buffer, num_bytes);
#else
    enable_dmac(p_data, num_bytes);
#endif

#else
    /* Transfer method is interrupt */
#endif
#endif


#if (EMWIN_USE_DATA_CMD_PIN == 1)
    /* DATA/CMD pin is High */
    R_GPIO_PinWrite(EMWIN_DATA_CMD_PIN, GPIO_LEVEL_HIGH);
#endif

#if (EMWIN_USE_LCD_CS_PIN == 1)
    /* Chip select pin is Low */
    R_GPIO_PinWrite(EMWIN_LCD_CS_PIN, GPIO_LEVEL_LOW);
#endif

    s_transfer_busy = true;

#if (EMWIN_LCD_IF == LCD_IF_SCI_SPI)

#if (EMWIN_SELECT_DMAC_DTC == 1)
    R_BSP_InterruptsDisable();
#endif

    s_lcd_sci_spi_err = R_SCI_Send(s_lcd_sci_spi_handle, p_data, num_bytes);

#if (EMWIN_SELECT_DMAC_DTC == 1)
    R_BSP_InterruptsEnable();
#endif

    if (SCI_SUCCESS != s_lcd_sci_spi_err)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP();  /* no operation */
        }
    }

#else /* EMWIN_LCD_IF == LCD_IF_RSPI */
#if (RSPI_VERSION == 1) && (EMWIN_SELECT_DMAC_DTC > 0)
    s_lcd_rspi_err = R_RSPI_Write(s_lcd_rspi_handle, s_lcd_rspi_command, rspi_word_buffer, num_bytes);
#else
    s_lcd_rspi_err = R_RSPI_Write(s_lcd_rspi_handle, s_lcd_rspi_command, p_data, num_bytes);
#endif

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
    R_GPIO_PinWrite(EMWIN_LCD_CS_PIN, GPIO_LEVEL_HIGH);
#endif

#if (EMWIN_LCD_IF == LCD_IF_RSPI)
#if (EMWIN_SELECT_DMAC_DTC == 1)
    disable_dtc();
#elif (EMWIN_SELECT_DMAC_DTC == 2)
    /* Disable processing not required when using DMAC */
#else
    /* Transfer method is interrupt */
#endif
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
static void write8_a0(uint8_t cmd)
{
    write_command(&cmd, 1);
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
static void writem8_a0(uint8_t * p_cmd, int num_items)
{
    write_command(p_cmd, num_items);
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
static void write8_a1(uint8_t data)
{
    write_data(&data, 1);
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
static void writem8_a1(uint8_t * p_data, int num_items)
{
    write_data(p_data, num_items);
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
static void readm8_a0(uint8_t * p_data, int num_items)
{
    GUI_USE_PARA(p_data);
    GUI_USE_PARA(num_items);
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
static void readm8_a1(uint8_t * p_data, int num_items)
{
    GUI_USE_PARA(p_data);
    GUI_USE_PARA(num_items);
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
static void com_end_callback(void * p_data)
{
    GUI_USE_PARA(p_data);
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

#if (EMWIN_SELECT_DMAC_DTC == 1) && (EMWIN_INIT_DMAC == 1)

    if (DTC_SUCCESS != R_DTC_Open())
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }

#elif (EMWIN_SELECT_DMAC_DTC == 2) && (EMWIN_INIT_DMAC == 1)

    R_DMACA_Init();

#else
    /* Transfer method is interrupt */
#endif

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
#if (EMWIN_SELECT_DMAC_DTC == 1)
    s_lcd_rspi_setting.tran_mode         = RSPI_TRANS_MODE_DTC;  /* Set transfer mode to DTC.         */
#elif (EMWIN_SELECT_DMAC_DTC == 2)
    s_lcd_rspi_setting.tran_mode         = RSPI_TRANS_MODE_DMAC; /* Set transfer mode to DMAC.        */
#else
    s_lcd_rspi_setting.tran_mode         = RSPI_TRANS_MODE_SW;   /* Set transfer mode to Software.    */
#endif

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

#if (EMWIN_SELECT_DMAC_DTC == 1)

    /* === DTC Enable === */
    s_lcd_rspi_err = R_RSPI_GetBuffRegAddress(s_lcd_rspi_handle, &s_reg_buff);
    if (RSPI_SUCCESS != s_lcd_rspi_err)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }

    /* Send */
    s_data_cfg_w.transfer_mode         = DTC_TRANSFER_MODE_NORMAL;
    s_data_cfg_w.data_size             = RSPI_DMA_TRANS_SIZE;
    s_data_cfg_w.src_addr_mode         = DTC_SRC_ADDR_INCR;
    s_data_cfg_w.chain_transfer_enable = DTC_CHAIN_TRANSFER_DISABLE;
    s_data_cfg_w.chain_transfer_mode   = DTC_CHAIN_TRANSFER_CONTINUOUSLY;
    s_data_cfg_w.response_interrupt    = DTC_INTERRUPT_AFTER_ALL_COMPLETE;
    s_data_cfg_w.repeat_block_side     = DTC_REPEAT_BLOCK_DESTINATION;
    s_data_cfg_w.dest_addr_mode        = DTC_DES_ADDR_FIXED;

    /* Receive */
    s_data_cfg_r.transfer_mode          = DTC_TRANSFER_MODE_NORMAL;
    s_data_cfg_r.data_size              = RSPI_DMA_TRANS_SIZE;
    s_data_cfg_r.src_addr_mode          = DTC_SRC_ADDR_FIXED;
    s_data_cfg_r.chain_transfer_enable  = DTC_CHAIN_TRANSFER_DISABLE;
    s_data_cfg_r.chain_transfer_mode    = DTC_CHAIN_TRANSFER_CONTINUOUSLY;
    s_data_cfg_r.response_interrupt     = DTC_INTERRUPT_AFTER_ALL_COMPLETE;
    s_data_cfg_r.repeat_block_side      = DTC_REPEAT_BLOCK_DESTINATION;
    s_data_cfg_r.dest_addr_mode         = DTC_DES_ADDR_FIXED;

    s_args_w.chain_transfer_nr = 0;
    s_args_r.chain_transfer_nr = 0;
    s_args_w.act_src           = DTCE_RSPI_SPTI;
    s_args_r.act_src           = DTCE_RSPI_SPRI;


#elif (EMWIN_SELECT_DMAC_DTC == 2)

    s_lcd_rspi_err = R_RSPI_GetBuffRegAddress(s_lcd_rspi_handle, &s_reg_buff);
    if (RSPI_SUCCESS != s_lcd_rspi_err)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }

    dmaca_return_t ret_dmaca;

    ret_dmaca = R_DMACA_Open(EMWIN_DMAC_NUMBER);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }

    ret_dmaca = R_DMACA_Open(EMWIN_DMAC_NUMBER2);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }

    ret_dmaca = R_DMACA_Int_Callback(EMWIN_DMAC_NUMBER, (void *) dma_callback_w);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }

    ret_dmaca = R_DMACA_Int_Callback(EMWIN_DMAC_NUMBER2, (void *) dma_callback_r);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }
    ret_dmaca = R_DMACA_Int_Enable(EMWIN_DMAC_NUMBER, 10);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }
    ret_dmaca = R_DMACA_Int_Enable(EMWIN_DMAC_NUMBER2, 10);
    if (DMACA_SUCCESS != ret_dmaca)
    {
        /* WAIT_LOOP */
        while (1)
        {
            R_BSP_NOP(); /* no operation */
        }
    }

    /* Send */
    s_data_cfg_dmac_w.transfer_mode         = DMACA_TRANSFER_MODE_NORMAL;
    s_data_cfg_dmac_w.repeat_block_side     = DMACA_REPEAT_BLOCK_DESTINATION;
    s_data_cfg_dmac_w.data_size             = RSPI_DMA_TRANS_SIZE;

    s_data_cfg_dmac_w.act_source            = IR_RSPI_SPTI;
    s_data_cfg_dmac_w.request_source        = DMACA_TRANSFER_REQUEST_PERIPHERAL;
    s_data_cfg_dmac_w.dtie_request          = DMACA_TRANSFER_END_INTERRUPT_ENABLE;
    s_data_cfg_dmac_w.esie_request          = DMACA_TRANSFER_ESCAPE_END_INTERRUPT_DISABLE;
    s_data_cfg_dmac_w.rptie_request         = DMACA_REPEAT_SIZE_END_INTERRUPT_DISABLE;
    s_data_cfg_dmac_w.sarie_request         = DMACA_SRC_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    s_data_cfg_dmac_w.darie_request         = DMACA_DES_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;

    s_data_cfg_dmac_w.src_addr_mode         = DMACA_SRC_ADDR_INCR;
    s_data_cfg_dmac_w.des_addr_mode         = DMACA_DES_ADDR_FIXED;

    s_data_cfg_dmac_w.src_addr_repeat_area  = DMACA_SRC_ADDR_EXT_REP_AREA_NONE;
    s_data_cfg_dmac_w.des_addr_repeat_area  = DMACA_DES_ADDR_EXT_REP_AREA_NONE;
    s_data_cfg_dmac_w.interrupt_sel         = DMACA_CLEAR_INTERRUPT_FLAG_BEGINNING_TRANSFER;

    /* Receive */
    s_data_cfg_dmac_r.transfer_mode         = DMACA_TRANSFER_MODE_NORMAL;
    s_data_cfg_dmac_r.repeat_block_side     = DMACA_REPEAT_BLOCK_DESTINATION;
    s_data_cfg_dmac_r.data_size             = RSPI_DMA_TRANS_SIZE;

    s_data_cfg_dmac_r.act_source            = IR_RSPI_SPRI;
    s_data_cfg_dmac_r.request_source        = DMACA_TRANSFER_REQUEST_PERIPHERAL;
    s_data_cfg_dmac_r.dtie_request          = DMACA_TRANSFER_END_INTERRUPT_ENABLE;
    s_data_cfg_dmac_r.esie_request          = DMACA_TRANSFER_ESCAPE_END_INTERRUPT_DISABLE;
    s_data_cfg_dmac_r.rptie_request         = DMACA_REPEAT_SIZE_END_INTERRUPT_DISABLE;
    s_data_cfg_dmac_r.sarie_request         = DMACA_SRC_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;
    s_data_cfg_dmac_r.darie_request         = DMACA_DES_ADDR_EXT_REP_AREA_OVER_INTERRUPT_DISABLE;

    s_data_cfg_dmac_r.src_addr_mode         = DMACA_SRC_ADDR_FIXED;
    s_data_cfg_dmac_r.des_addr_mode         = DMACA_DES_ADDR_FIXED;

    s_data_cfg_dmac_r.src_addr_repeat_area  = DMACA_SRC_ADDR_EXT_REP_AREA_NONE;
    s_data_cfg_dmac_r.des_addr_repeat_area  = DMACA_DES_ADDR_EXT_REP_AREA_NONE;
    s_data_cfg_dmac_r.interrupt_sel         = DMACA_CLEAR_INTERRUPT_FLAG_BEGINNING_TRANSFER;

#else
    /* Transfer method is interrupt */
#endif /* EMWIN_SELECT_DMAC_DTC */

#endif /* EMWIN_LCD_IF */

    /* Initialize display */
    init_display();
}
/**********************************************************************************************************************
 * End of function init_controller
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: lcd_driver_setting
 * Description  : Called during configuration changes such as display rotation.
 * Arguments    : .
 * Return Value : .
 * Note         : This call back function is called from the emWin library.
 *********************************************************************************************************************/
void lcd_driver_setting(GUI_DEVICE * p_device, int rotateindex, int layerindex)
{
    GUI_PORT_API     port_api = { 0 };
    CONFIG_FLEXCOLOR config   = { 0 };
    int32_t          swap_xy_ex;
    int              temp;
    int              orientation[] = {
                         DISPLAY_ORIENTATION_0,
                         DISPLAY_ORIENTATION_1,
                         DISPLAY_ORIENTATION_2,
                         DISPLAY_ORIENTATION_3
                     };

    GUI_USE_PARA(layerindex);

    /* Display driver configuration */
    swap_xy_ex = LCD_GetSwapXYEx(0);
    if (swap_xy_ex)
    {
        LCD_SetSizeEx(0, EMWIN_YSIZE_PHYS, EMWIN_XSIZE_PHYS);
        LCD_SetVSizeEx(0, EMWIN_YSIZE_PHYS, EMWIN_XSIZE_PHYS);
    }
    else
    {
        LCD_SetSizeEx(0, EMWIN_XSIZE_PHYS, EMWIN_YSIZE_PHYS);
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
    config.Orientation   = orientation[rotateindex] ^ DISPLAY_ORIENTATION;
    config.NumDummyReads = 0;

    config.FirstCOM = FIRST_COM;
    config.FirstSEG = FIRST_SEG;

    if (config.Orientation & GUI_SWAP_XY)
    {
        temp            = config.FirstCOM;
        config.FirstCOM = config.FirstSEG;
        config.FirstSEG = temp;
    }

    GUIDRV_FlexColor_Config(p_device, &config);
}
/**********************************************************************************************************************
 * End of function lcd_driver_setting
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
    GUI_DEVICE * p_device;

    /* Set display driver and color conversion for 1st layer */
    p_device = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, COLOR_CONVERSION, 0, 0);

    /* Display size and frame buffer setting */
    lcd_driver_setting(p_device, 0, 0);

#if (EMWIN_USE_RUNTIME_ORIENTATION == 1)
    /* Create additional drivers and add them to the display rotation module */
    LCD_ROTATE_AddDriverExOrientation(GUIDRV_FLEXCOLOR, 0, GUI_ROTATION_CW);
    LCD_ROTATE_AddDriverExOrientation(GUIDRV_FLEXCOLOR, 0, GUI_ROTATION_180);
    LCD_ROTATE_AddDriverExOrientation(GUIDRV_FLEXCOLOR, 0, GUI_ROTATION_CCW);

    /* Set call back function */
    LCD_ROTATE_SetCallback(lcd_driver_setting, 0);
#endif
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
    static bool init_f = false;

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
		    if (false == init_f)
		    {
			    init_controller();
                init_f = true;
            }
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

