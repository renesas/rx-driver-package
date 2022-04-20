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
 * File Name    : wrap_sci.h
 * Description  :
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <string.h>
#include <stdint.h>
#include "platform.h"
#include "r_sci_rx_if.h"
#include "r_sci_rx_pinset.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef R_RYZ012_RX_WRAP_SCI_H_
#define R_RYZ012_RX_WRAP_SCI_H_

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
/* SCI configuration */
typedef struct
{
    sci_ch_t ch;           /* SCI channel */
    void (*func)(void);    /* Function name of SCI Port configuration */
    uint16_t tx_size;      /* TX buffer size */
    uint16_t rx_size;      /* RX buffer size */
} st_sci_conf_t;

/* SCI table information */
typedef struct
{
    sci_hdl_t   sci_hdl;
    sci_cfg_t   sci_config;
    volatile uint8_t tx_end_flag;
} st_sci_tbl_t;

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: wrap_sci_open
 * Description  : Open SCI channel.
 * Arguments    : ch
 *                p_cb
 * Return Value :
 *********************************************************************************************************************/
uint8_t   wrap_sci_open (sci_ch_t ch, void (* const p_cb)(void *p_args));

/**********************************************************************************************************************
 * Function Name: wrap_sci_send
 * Description  : Send data on SCI channel.
 * Arguments    : p_src
 *                length
 * Return Value : same as R_SCI_Send()
 *********************************************************************************************************************/
sci_err_t wrap_sci_send (uint8_t *p_src, uint16_t const length);

/**********************************************************************************************************************
 * Function Name: wrap_sci_recv
 * Description  : receive data from SCI channel.
 * Arguments    : p_dst
 *                length
 * Return Value : same as R_SCI_Receive()
 *********************************************************************************************************************/
sci_err_t wrap_sci_recv (uint8_t * p_dst, uint16_t const length);

/**********************************************************************************************************************
 * Function Name: wrap_sci_close
 * Description  : close SCI channel.
 * Arguments    : none
 * Return Value : same as R_SCI_Close()
 *********************************************************************************************************************/
sci_err_t wrap_sci_close (void);

/**********************************************************************************************************************
 * Function Name: wrap_sci_ctrl
 * Description  : configure SCI channel.
 * Arguments    : cmd
 *                p_args
 * Return Value : same as R_SCI_Control()
 *********************************************************************************************************************/
sci_err_t wrap_sci_ctrl (sci_cmd_t const cmd, void * p_args);

#endif /* R_RYZ012_RX_WRAP_SCI_H_ */
