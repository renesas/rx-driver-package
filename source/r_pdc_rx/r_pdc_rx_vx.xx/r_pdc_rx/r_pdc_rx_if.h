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
 * Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_pdc_rx_if.h
 * Version      : 2.05
 * Description  : PDC module device driver
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.10.2016 2.00     First Release
 *         : 02.10.2017 2.01     Added changes for RX65N-2MB.
 *         : 01.02.2019 2.02     Changed Minor version to 2.02.
 *         : 20.05.2019 2.03     Changed Minor version to 2.03.
 *         : 30.07.2019 2.04     Changed Minor version to 2.04.
 *                               Added changes for RX72M.
 *         : 22.11.2019 2.05     Changed Minor version to 2.05.
 ***********************************************************************************************************************/
/* Guards against multiple inclusion. */
#ifndef PDC_RX_IF_H
    #define PDC_RX_IF_H

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
/* Integer support. */
    #include <stdint.h>
/* Bool support. */
    #include <stdbool.h>

/* Configuration for this package. */
    #include "r_pdc_rx_config.h"

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/* Version Number of API. */
    #define PDC_RX_VERSION_MAJOR  (2)
    #define PDC_RX_VERSION_MINOR  (5)

/* Selects the active sense of the VSYNC signal. */
    #define PDC_VSYNC_SIGNAL_POLARITY_HIGH    (0)
    #define PDC_VSYNC_SIGNAL_POLARITY_LOW     (1)

/* Selects the active sense of the HSYNC signal. */
    #define PDC_HSYNC_SIGNAL_POLARITY_HIGH    (0)
    #define PDC_HSYNC_SIGNAL_POLARITY_LOW     (1)

/* Select the interrupt set to be updated. */
    #define PDC_PCDFI_IPR_UPDATE              (0x0001)
    #define PDC_GROUPBL0_IPR_UPDATE           (0x0002)
    #define PDC_PCFEI_IEN_UPDATE              (0x0004)
    #define PDC_PCERI_IEN_UPDATE              (0x0008)
    #define PDC_PCDFI_IEN_UPDATE              (0x0010)
    #define PDC_DFIE_IEN_UPDATE               (0x0020)
    #define PDC_FEIE_IEN_UPDATE               (0x0040)
    #define PDC_OVIE_IEN_UPDATE               (0x0080)
    #define PDC_UDRIE_IEN_UPDATE              (0x0100)
    #define PDC_VERIE_IEN_UPDATE              (0x0200)
    #define PDC_HERIE_IEN_UPDATE              (0x0400)
    #define PDC_ALL_INT_UPDATE                (0x07FF)

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/
/* R_PDC_Control control codes */
typedef enum e_pdc_command
{
    PDC_CMD_CAPTURE_START = 0, /* Start PDC capture */
    PDC_CMD_CHANGE_POS_AND_SIZE, /* Change PDC capture position and capture size */
    PDC_CMD_STATUS_GET, /* Get PDC status */
    PDC_CMD_STATUS_CLR, /* Clear PDC status */
    PDC_CMD_SET_INTERRUPT, /* PDC interrupt setting */
    PDC_CMD_DISABLE, /* Disable PDC receive operation */
    PDC_CMD_ENABLE, /* Enable PDC receive operation */
    PDC_CMD_RESET /* PDC reset */
} pdc_command_t;

/* Function return values */
typedef enum e_pdc_return /* PDC API error codes */
{
    PDC_SUCCESS = 0, /* Processing finished successfully. */
    PDC_ERR_OPENED, /* PDC module initialized. Initialization function
     R_PDC_Open has been run. */
    PDC_ERR_NOT_OPEN, /* PDC module uninitialized. R_PDC_Open has not been run. */
    PDC_ERR_INVALID_ARG, /* Invalid argument input. */
    PDC_ERR_INVALID_COMMAND, /* Command is invalid. */
    PDC_ERR_NULL_PTR, /* Argument pointer value was NULL. */
    PDC_ERR_LOCK_FUNC, /* PDC resources is in use by another process. */
    PDC_ERR_INTERNAL, /* Module internal error is detected. */
    PDC_ERR_RST_TIMEOUT /* PDC is resetting after constant time is passed */
} pdc_return_t;

/* Pointer to callback functions of PDC */
typedef struct
{
    void (*pcb_receive_data_ready) (void *); /* Pointer to callback function for receive data-ready interrupt */
    void (*pcb_frame_end) (void *); /* Pointer to callback function for FIFO empty after frame-end interrupt */
    void (*pcb_error) (void *); /* Pointer to callback function for occurred each error */
} pdc_cb_t;

/* Event ID code for call factor of callback functions */
typedef enum
{
    PDC_EVT_ID_DATAREADY = 0, /* Event for receive data-ready interrupt */
    PDC_EVT_ID_FRAMEEND, /* Event for frame end interrupt */
    PDC_EVT_ID_TIMEOUT, /* Event for FIFO is not empty after constant time is passed */
    PDC_EVT_ID_ERROR, /* Event for error interrupt */
    PDC_EVT_ID_OVERRUN, /* Event for overrun */
    PDC_EVT_ID_UNDERRUN, /* Event for underrun */
    PDC_EVT_ID_VERTICALLINE, /* Event for vertical line setting error */
    PDC_EVT_ID_HORIZONTALBYTE /* Event for horizontal line setting error */
} pdc_cb_event_t;

/* Argument for callback functions */
typedef struct
{
    pdc_cb_event_t event_id; /* Event for call factor of callback function */
} pdc_cb_arg_t;

/* Interrupt priority level control */
typedef struct st_pdc_int_priority_data_cfg
{
    uint8_t pcdfi_level; /* PCDFI interrupt priority level */
    uint8_t groupbl0_level; /* GROUPBL0 interrupt priority level */
} pdc_ipr_dcfg_t;

/* Interrupt controller (ICUA) PDC interrupt enable/disable */
typedef struct st_pdc_inticu_data_cfg
{
    bool pcfei_ien; /* Frame-end interrupt enabled */
    bool pceri_ien; /* Error interrupt enabled */
    bool pcdfi_ien; /* Receive data-ready interrupt enabled */
} pdc_inticu_dcfg_t;

/* PDC interrupt enable/disable */
typedef struct st_pdc_intpdc_data_cfg
{
    bool dfie_ien; /* Receive data-ready interrupt enabled */
    bool feie_ien; /* Frame-end interrupt enabled */
    bool ovie_ien; /* Overrun interrupt enabled */
    bool udrie_ien; /* Underrun interrupt enabled */
    bool verie_ien; /* Vertical line count setting error interrupt enabled */
    bool herie_ien; /* Horizontal byte count setting error interrupt enabled */
} pdc_intpdc_dcfg_t;

/* Capture position specification */
typedef struct st_pdc_position_data_cfg
{
    uint16_t vst_position; /* Vertical capture start line position */
    uint16_t hst_position; /* Horizontal capture start byte position */
} pdc_pos_dcfg_t;

/* Capture size specification */
typedef struct st_pdc_size_data_cfg
{
    uint16_t vsz_size; /* Vertical capture size */
    uint16_t hsz_size; /* Horizontal capture size */
} pdc_size_dcfg_t;

/* PDC settings */
typedef struct st_pdc_data_cfg
{
    uint16_t          iupd_select; /* Interrupt setting update select */
    pdc_ipr_dcfg_t    priority; /* Interrupt priority level */
    pdc_inticu_dcfg_t inticu_req; /* ICU interrupt setting */
    pdc_intpdc_dcfg_t intpdc_req; /* PDC interrupt setting */
                  bool vps_select; /* VSYNC signal polarity setting */
                  bool hps_select; /* HSYNC signal polarity setting */
    pdc_pos_dcfg_t    capture_pos; /* Capture position setting */
    pdc_size_dcfg_t   capture_size; /* Capture size setting */
    pdc_cb_t          p_callback; /* Pointer to callback functions */
} pdc_data_cfg_t;

/* Copy of PDC status register (PCSR) */
typedef struct st_pdc_pcsr_stat
{
    bool frame_busy; /* PDC operating status (FBSY flag) */
    bool fifo_empty; /* FIFO status (FEMPF flag) */
    bool frame_end; /* Frame-end (FEF flag) */
    bool overrun; /* Overrun (OVRF flag) */
    bool underrun; /* Underrun (UDRF flag) */
    bool verf_error; /* Vertical line count setting error (VERF flag) */
    bool herf_error; /* Horizontal byte count setting error (HERF flag) */
} pdc_pcsr_stat_t;

/* Copy of PDC pin monitor status register (PCMONR) */
typedef struct st_pdc_pcmonr_stat
{
    bool vsync; /* VSYNC signal status (VSYNC flag) */
    bool hsync; /* HSYNC signal status (HSYNC flag) */
} pdc_pcmonr_stat_t;

/* PDC status */
typedef struct st_pdc_stat
{
    pdc_pcsr_stat_t   pcsr_stat; /* PDC status register(PCSR) information */
    pdc_pcmonr_stat_t pcmonr_stat; /* PDC pin monitor status (PCMONR) information */
} pdc_stat_t;

/***********************************************************************************************************************
 Exported global variables and functions (to be accessed by other files)
 ***********************************************************************************************************************/
pdc_return_t R_PDC_Open (pdc_data_cfg_t *p_data_cfg);
pdc_return_t R_PDC_Close (void);
pdc_return_t R_PDC_Control (pdc_command_t command, pdc_data_cfg_t *p_data_cfg, pdc_stat_t *p_stat);
pdc_return_t R_PDC_GetFifoAddr (uint32_t *p_fifo_addr);
uint32_t R_PDC_GetVersion (void);

#endif /* PDC_RX_IF_H */

/* End of File */
