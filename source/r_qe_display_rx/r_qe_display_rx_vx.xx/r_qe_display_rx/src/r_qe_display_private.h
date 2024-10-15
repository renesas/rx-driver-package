/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_qe_display_private.h
 * Version      : 1.00
 * Description  : Header file of QE for Display module.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 19.07.2024 1.00      First Release
 **********************************************************************************************************************/

#ifndef R_QE_DISPLAY_PRIVATE_H
#define R_QE_DISPLAY_PRIVATE_H

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include <string.h>

#include "r_qe_display_rx_if.h"

#include "r_gpio_rx_if.h"

#include "r_glcdc_rx_if.h"
#include "r_glcdc_rx_pinset.h"

#include "r_sci_rx_if.h"
#include "r_sci_rx_pinset.h"

#include "r_cmt_rx_if.h"

/**********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/

#ifndef QE_DISPLAY_CONFIGURATION
#error "Error!! Please declare QE_DISPLAY_CONFIGURATION definition to the compiler."
#endif

/** LCD definitions *********************************************/
#if (LCD_CH0_CALLBACK_ENABLE == true)
/* callback function prototype declaration */
void LCD_CH0_PCALLBACK (void *);
#define GLCDC_PRV_PCALLBACK ((void (*)(void *))LCD_CH0_PCALLBACK)
#else
#define GLCDC_PRV_PCALLBACK (FIT_NO_FUNC)
#endif

/* Pixel color code */
#define PIXEL_COLOR_RED_ARGB8888    (0xFFFF0000)
#define PIXEL_COLOR_RED_RGB888      (0x00FF0000)
#define PIXEL_COLOR_RED_RGB565      (0xF800)
#define PIXEL_COLOR_RED_ARGB1555    (0xFC00)
#define PIXEL_COLOR_RED_ARGB4444    (0xFF00)
#define PIXEL_COLOR_RED_CLUT8       (0xE0)
#define PIXEL_COLOR_RED_CLUT4       (0x08)
#define PIXEL_COLOR_WHITE_CLUT1     (0x01)

/* Value for Calculate HSV color code */
#define FLT_EPSILON                 (1.1920928955078125e-07)


/** UART definitions **********************************************/

/* SCI pin setting */
#if (QE_DISPLAY_CFG_UART_CH == 0)
#define SCI_PIN_SET     R_SCI_PinSet_SCI0
#elif (QE_DISPLAY_CFG_UART_CH == 1)
#define SCI_PIN_SET     R_SCI_PinSet_SCI1
#elif (QE_DISPLAY_CFG_UART_CH == 2)
#define SCI_PIN_SET     R_SCI_PinSet_SCI2
#elif (QE_DISPLAY_CFG_UART_CH == 3)
#define SCI_PIN_SET     R_SCI_PinSet_SCI3
#elif (QE_DISPLAY_CFG_UART_CH == 4)
#define SCI_PIN_SET     R_SCI_PinSet_SCI4
#elif (QE_DISPLAY_CFG_UART_CH == 5)
#define SCI_PIN_SET     R_SCI_PinSet_SCI5
#elif (QE_DISPLAY_CFG_UART_CH == 6)
#define SCI_PIN_SET     R_SCI_PinSet_SCI6
#elif (QE_DISPLAY_CFG_UART_CH == 7)
#define SCI_PIN_SET     R_SCI_PinSet_SCI7
#elif (QE_DISPLAY_CFG_UART_CH == 8)
#define SCI_PIN_SET     R_SCI_PinSet_SCI8
#elif (QE_DISPLAY_CFG_UART_CH == 9)
#define SCI_PIN_SET     R_SCI_PinSet_SCI9
#elif (QE_DISPLAY_CFG_UART_CH == 10)
#define SCI_PIN_SET     R_SCI_PinSet_SCI10
#elif (QE_DISPLAY_CFG_UART_CH == 11)
#define SCI_PIN_SET     R_SCI_PinSet_SCI11
#elif (QE_DISPLAY_CFG_UART_CH == 12)
#define SCI_PIN_SET     R_SCI_PinSet_SCI12
#endif


/* Response data (MCU -> QE) */
#define RES_ACCEPT(x)                               (x | 0x10)

#define QE_DISPLAY_RES_ACK                          (0xA0)
#define QE_DISPLAY_RES_NACK                         (0xA5)

#define QE_DISPLAY_RES_BUSY                         (0x02)
#define QE_DISPLAY_RES_NEXT                         (0x04)

#define QE_DISPLAY_RES_SUCCES_SETTING               (0x10)
#define QE_DISPLAY_RES_ERR_INVALID_ARG              (0x11)
#define QE_DISPLAY_RES_ERR_INVALID_TIMING_SETTING   (0x12)
#define QE_DISPLAY_RES_ERR_INVALID_LAYER_SETTING    (0x13)
#define QE_DISPLAY_RES_ERR_INVALID_ALIGNMENT        (0x14)
#define QE_DISPLAY_RES_ERR_INVALID_GAMMA_SETTING    (0x15)
#define QE_DISPLAY_RES_ERR_INVALID_BLEND_SETTING    (0x16)


/* ID data (QE -> MCU) */
#define QE_DISPLAY_ID_COMMAND                       (0x0C)
#define QE_DISPLAY_ID_DATA                          (0x0D)


/* Command data (QE -> MCU) */
#define QE_DISPLAY_COMMAND_NULL                     (0x00)
#define QE_DISPLAY_COMMAND_CONNECTION               (0xF0)
#define QE_DISPLAY_COMMAND_MAX_COM_SIZE             (0xF5)

#define QE_DISPLAY_COMMAND_ALL                      (0x80)
#define QE_DISPLAY_COMMAND_TIMING                   (0x81)
#define QE_DISPLAY_COMMAND_OUTCTL                   (0x82)
#define QE_DISPLAY_COMMAND_LAYER1                   (0x83)
#define QE_DISPLAY_COMMAND_LAYER2                   (0x84)
#define QE_DISPLAY_COMMAND_PROCIDURE                (0x85)
#define QE_DISPLAY_COMMAND_DITHER                   (0x86)
#define QE_DISPLAY_COMMAND_BRIGHTNESS               (0x87)
#define QE_DISPLAY_COMMAND_CONTRAST                 (0x88)
#define QE_DISPLAY_COMMAND_GAMMA                    (0x89)


/* Status */
#define QE_DISPLAY_STS_CLOSE                        (0x00)
#define QE_DISPLAY_STS_IDLE                         (0x01)
#define QE_DISPLAY_STS_COMMAND                      (0x02)
#define QE_DISPLAY_STS_DATA                         (0x03)
#define QE_DISPLAY_STS_EXEC                         (0x04)

#define QE_DISPLAY_STS_LCD_REINIT                   (0x05)
#define QE_DISPLAY_STS_ERR_ID                       (0x06)
#define QE_DISPLAY_STS_ERR_COMMAND                  (0x07)
#define QE_DISPLAY_STS_ERR_RECEIVE_DATA             (0x08)
#define QE_DISPLAY_STS_ERR_TIMEOUT                  (0x09)
#define QE_DISPLAY_STS_UART_CONNECTION              (0x0A)

/* Data size */
#define QE_DISPLAY_SIZE_ID                          (1)
#define QE_DISPLAY_SIZE_COMMAND                     (1)
#define QE_DISPLAY_SIZE_RES_COMMAND                 (1)
#define QE_DISPLAY_SIZE_RES_MAX_DATA                (2)

#define QE_DISPLAY_SIZE_VERIFY_DATA                 (10)
#define QE_DISPLAY_SIZE_RES_VERIFY_DATA             (11)

#define QE_DISPLAY_SIZE_REV_COMMAND_ALL             (246)
#define QE_DISPLAY_SIZE_REV_COMMAND_TIMING          (13)
#define QE_DISPLAY_SIZE_REV_COMMAND_OUTCTL          (14)
#define QE_DISPLAY_SIZE_REV_COMMAND_LAYER1          (9)
#define QE_DISPLAY_SIZE_REV_COMMAND_LAYER2          (9)
#define QE_DISPLAY_SIZE_REV_COMMAND_PROC_ORDER      (1)
#define QE_DISPLAY_SIZE_REV_COMMAND_DITHER          (5)
#define QE_DISPLAY_SIZE_REV_COMMAND_BRIGHTNESS      (6)
#define QE_DISPLAY_SIZE_REV_COMMAND_CONTRAST        (3)
#define QE_DISPLAY_SIZE_REV_COMMAND_GAMMA           (186)


#if QE_DISPLAY_CFG_UART_CH == 0
#if SCI_CFG_CH0_RX_DTC_DMACA_ENABLE == 0
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (SCI_CFG_CH0_RX_BUFSIZ)
#else
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (65535)
    /* DMAC/DTC usage info */
    #define QE_DISPLAY_UART_DMA_ENABLE     (SCI_CFG_CH0_RX_DTC_DMACA_ENABLE)
    #define QE_DISPLAY_UART_DMA_TX_CH      (SCI_CFG_CH0_TX_DMACA_CH_NUM)
    #define QE_DISPLAY_UART_DMA_RX_CH      (SCI_CFG_CH0_RX_DMACA_CH_NUM)
#endif

#elif QE_DISPLAY_CFG_UART_CH == 1
#if SCI_CFG_CH1_RX_DTC_DMACA_ENABLE == 0
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (SCI_CFG_CH1_RX_BUFSIZ)
#else
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (65535)
    /* DMAC/DTC usage info */
    #define QE_DISPLAY_UART_DMA_ENABLE     (SCI_CFG_CH1_RX_DTC_DMACA_ENABLE)
    #define QE_DISPLAY_UART_DMA_TX_CH      (SCI_CFG_CH1_TX_DMACA_CH_NUM)
    #define QE_DISPLAY_UART_DMA_RX_CH      (SCI_CFG_CH1_RX_DMACA_CH_NUM)
#endif

#elif QE_DISPLAY_CFG_UART_CH == 2
#if SCI_CFG_CH2_RX_DTC_DMACA_ENABLE == 0
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (SCI_CFG_CH2_RX_BUFSIZ)
#else
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (65535)
    /* DMAC/DTC usage info */
    #define QE_DISPLAY_UART_DMA_ENABLE     (SCI_CFG_CH2_RX_DTC_DMACA_ENABLE)
    #define QE_DISPLAY_UART_DMA_TX_CH      (SCI_CFG_CH2_TX_DMACA_CH_NUM)
    #define QE_DISPLAY_UART_DMA_RX_CH      (SCI_CFG_CH2_RX_DMACA_CH_NUM)
#endif

#elif QE_DISPLAY_CFG_UART_CH == 3
#if SCI_CFG_CH3_RX_DTC_DMACA_ENABLE == 0
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (SCI_CFG_CH3_RX_BUFSIZ)
#else
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (65535)
    /* DMAC/DTC usage info */
    #define QE_DISPLAY_UART_DMA_ENABLE     (SCI_CFG_CH3_RX_DTC_DMACA_ENABLE)
    #define QE_DISPLAY_UART_DMA_TX_CH      (SCI_CFG_CH3_TX_DMACA_CH_NUM)
    #define QE_DISPLAY_UART_DMA_RX_CH      (SCI_CFG_CH3_RX_DMACA_CH_NUM)
#endif

#elif QE_DISPLAY_CFG_UART_CH == 4
#if SCI_CFG_CH4_RX_DTC_DMACA_ENABLE == 0
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (SCI_CFG_CH4_RX_BUFSIZ)
#else
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (65535)
    /* DMAC/DTC usage info */
    #define QE_DISPLAY_UART_DMA_ENABLE     (SCI_CFG_CH4_RX_DTC_DMACA_ENABLE)
    #define QE_DISPLAY_UART_DMA_TX_CH      (SCI_CFG_CH4_TX_DMACA_CH_NUM)
    #define QE_DISPLAY_UART_DMA_RX_CH      (SCI_CFG_CH4_RX_DMACA_CH_NUM)
#endif

#elif QE_DISPLAY_CFG_UART_CH == 5
#if SCI_CFG_CH5_RX_DTC_DMACA_ENABLE == 0
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (SCI_CFG_CH5_RX_BUFSIZ)
#else
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (65535)
    /* DMAC/DTC usage info */
    #define QE_DISPLAY_UART_DMA_ENABLE     (SCI_CFG_CH5_RX_DTC_DMACA_ENABLE)
    #define QE_DISPLAY_UART_DMA_TX_CH      (SCI_CFG_CH5_TX_DMACA_CH_NUM)
    #define QE_DISPLAY_UART_DMA_RX_CH      (SCI_CFG_CH5_RX_DMACA_CH_NUM)
#endif

#elif QE_DISPLAY_CFG_UART_CH == 6
#if SCI_CFG_CH6_RX_DTC_DMACA_ENABLE == 0
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (SCI_CFG_CH6_RX_BUFSIZ)
#else
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (65535)
    /* DMAC/DTC usage info */
    #define QE_DISPLAY_UART_DMA_ENABLE     (SCI_CFG_CH6_RX_DTC_DMACA_ENABLE)
    #define QE_DISPLAY_UART_DMA_TX_CH      (SCI_CFG_CH6_TX_DMACA_CH_NUM)
    #define QE_DISPLAY_UART_DMA_RX_CH      (SCI_CFG_CH6_RX_DMACA_CH_NUM)
#endif

#elif QE_DISPLAY_CFG_UART_CH == 7
#if SCI_CFG_CH7_RX_DTC_DMACA_ENABLE == 0
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (SCI_CFG_CH7_RX_BUFSIZ)
#else
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (65535)
    /* DMAC/DTC usage info */
    #define QE_DISPLAY_UART_DMA_ENABLE     (SCI_CFG_CH7_RX_DTC_DMACA_ENABLE)
    #define QE_DISPLAY_UART_DMA_TX_CH      (SCI_CFG_CH7_TX_DMACA_CH_NUM)
    #define QE_DISPLAY_UART_DMA_RX_CH      (SCI_CFG_CH7_RX_DMACA_CH_NUM)
#endif

#elif QE_DISPLAY_CFG_UART_CH == 8
#if SCI_CFG_CH8_RX_DTC_DMACA_ENABLE == 0
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (SCI_CFG_CH8_RX_BUFSIZ)
#else
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (65535)
    /* DMAC/DTC usage info */
    #define QE_DISPLAY_UART_DMA_ENABLE     (SCI_CFG_CH8_RX_DTC_DMACA_ENABLE)
    #define QE_DISPLAY_UART_DMA_TX_CH      (SCI_CFG_CH8_TX_DMACA_CH_NUM)
    #define QE_DISPLAY_UART_DMA_RX_CH      (SCI_CFG_CH8_RX_DMACA_CH_NUM)
#endif

#elif QE_DISPLAY_CFG_UART_CH == 9
#if SCI_CFG_CH9_RX_DTC_DMACA_ENABLE == 0
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (SCI_CFG_CH9_RX_BUFSIZ)
#else
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (65535)
    /* DMAC/DTC usage info */
    #define QE_DISPLAY_UART_DMA_ENABLE     (SCI_CFG_CH9_RX_DTC_DMACA_ENABLE)
    #define QE_DISPLAY_UART_DMA_TX_CH      (SCI_CFG_CH9_TX_DMACA_CH_NUM)
    #define QE_DISPLAY_UART_DMA_RX_CH      (SCI_CFG_CH9_RX_DMACA_CH_NUM)
#endif

#elif QE_DISPLAY_CFG_UART_CH == 10
#if SCI_CFG_CH10_RX_DTC_DMACA_ENABLE == 0
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (SCI_CFG_CH10_RX_BUFSIZ)
#else
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (65535)
    /* DMAC/DTC usage info */
    #define QE_DISPLAY_UART_DMA_ENABLE     (SCI_CFG_CH10_RX_DTC_DMACA_ENABLE)
    #define QE_DISPLAY_UART_DMA_TX_CH      (SCI_CFG_CH10_TX_DMACA_CH_NUM)
    #define QE_DISPLAY_UART_DMA_RX_CH      (SCI_CFG_CH10_RX_DMACA_CH_NUM)
#endif

#elif QE_DISPLAY_CFG_UART_CH == 11
#if SCI_CFG_CH11_RX_DTC_DMACA_ENABLE == 0
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (SCI_CFG_CH11_RX_BUFSIZ)
#else
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (65535)
    /* DMAC/DTC usage info */
    #define QE_DISPLAY_UART_DMA_ENABLE     (SCI_CFG_CH11_RX_DTC_DMACA_ENABLE)
    #define QE_DISPLAY_UART_DMA_TX_CH      (SCI_CFG_CH11_TX_DMACA_CH_NUM)
    #define QE_DISPLAY_UART_DMA_RX_CH      (SCI_CFG_CH11_RX_DMACA_CH_NUM)
#endif

#elif QE_DISPLAY_CFG_UART_CH == 12
#if SCI_CFG_CH12_RX_DTC_DMACA_ENABLE == 0
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (SCI_CFG_CH12_RX_BUFSIZ)
#else
    /* Maximum Receivable Data size */
    #define QE_DISPLAY_SIZE_MAX_RECEIVABLE (65535)
    /* DMAC/DTC usage info */
    #define QE_DISPLAY_UART_DMA_ENABLE     (SCI_CFG_CH12_RX_DTC_DMACA_ENABLE)
    #define QE_DISPLAY_UART_DMA_TX_CH      (SCI_CFG_CH12_TX_DMACA_CH_NUM)
    #define QE_DISPLAY_UART_DMA_RX_CH      (SCI_CFG_CH12_RX_DMACA_CH_NUM)
#endif

#else
#error "Error!! Non-existent SCI channel!!"
#endif /* QE_DISPLAY_CFG_UART_CH */


/* Receivable size of data part */
#define QE_DISPLAY_SIZE_DATA_RECEIVABLE     (QE_DISPLAY_SIZE_MAX_RECEIVABLE - 1)

/* Receive timeout */
#define QE_DISPLAY_UART_TIMEOUT             (10) /* 0.5s per count (5 seconds) */

/* Use DMA */
#define USE_INTERRUPT                       (0)
#define USE_DMAC                            (2)


/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/** LCD variables *********************************************/
#if (LCD_CH0_CLUT_GR2_ENABLE == true)
extern uint32_t LCD_CH0_CLUT_GR2_PBASE[];
#endif

#if (LCD_CH0_CLUT_GR1_ENABLE == true)
extern uint32_t LCD_CH0_CLUT_GR1_PBASE[];
#endif


/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/** LCD functions *********************************************/

qe_display_err_t r_display_glcdc_init (void);
qe_display_err_t r_display_glcdc_reinit (void);
void r_display_exec_ajustment (void);

/* GLCDC FIT module api */
void r_glcdc_config_mode_setting (bool value);

/** UART functions *********************************************/

qe_display_err_t r_uart_init (void);
void r_uart_send_response (uint8_t response);
uint8_t * r_get_uart_buffer_pointer (void);
uint8_t r_get_uart_command (void);
void r_set_uart_command (uint8_t command);
uint8_t r_get_uart_status (void);
void r_set_uart_status (uint8_t status);
qe_display_err_t r_display_status_check (void);
void r_debug_print (char * str);


#endif /* R_QE_DISPLAY_PRIVATE_H */

/* End of File */
