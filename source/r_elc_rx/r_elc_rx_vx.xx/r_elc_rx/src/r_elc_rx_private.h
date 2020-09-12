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
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_elc_rx_private.h
 * Version      : 1.0
 * Description  : Functions for using Event Link Controller module
 ************************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           01.07.2016 1.0     Initial Release
 *           16.06.2017 1.20    Added support for RX65N(2MB), RX130(512KB)
 ***********************************************************************************************************************/
#ifndef R_ELC_RX_PRIVATE_H
    #define R_ELC_RX_PRIVATE_H

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
#include "r_elc_rx_if.h"

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
#define ELC_IPR_MAX         (15)

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/
typedef enum
{
    ELC_MODULE_STOP_RELEASE = 0, 
    ELC_MODULE_STOP_TRANSITION = 1
} elc_module_stop_t;

/***********************************************************************************************************************
 Public Functions
 ***********************************************************************************************************************/
void elc_module_control (elc_module_stop_t select);
void elc_open_init_register (void);
void elc_set_eventlink_setting (elc_event_signal_t * const p_elc_event_signal, elc_link_module_t * const p_elc_module);
void elc_control_cmd_event_clear (const elc_module_t clear_select);
void elc_control_cmd_soft_trigger (void);
void elc_start_all_eventlink (void);
void elc_stop_all_eventlink (void);
void elc_control_cmd_write_pdbf (const elc_portbuffer_t portbuffer_select, const uint8_t value);
uint8_t elc_control_cmd_read_pdbf (const elc_portbuffer_t portbuffer_select);
elc_err_t elc_check_validate_module (const elc_module_t link_module);
elc_err_t elc_set_err_check_um_note (elc_event_signal_t * const p_elc_event_signal,
elc_link_module_t * const p_elc_peripheral_module);

#endif /* R_ELC_RX_PRIVATE_H */
/***********************************************************************************************************************
 * End of File
 ***********************************************************************************************************************/
