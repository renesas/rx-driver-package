/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_qetouch_rx_if.h
* Version      : 1.00
* Device(s)    : R5F5113xAxFP,R5F5231xAxFP,R5F51305AxFN
* Description  : This file contains the TOUCH API and should be included by the application which uses this API.
***********************************************************************************************************************/

/***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 04.10.2018 1.00    First Release
***********************************************************************************************************************/
#ifndef QETOUCH_RX_IF_H_FILE
#define QETOUCH_RX_IF_H_FILE

#include "r_typedefs_qe.h"
#include "r_touch_qe_config.h"


/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/
/* Driver Version Number. */
#define QETOUCH_RX_VERSION_MAJOR    (1)
#define QETOUCH_RX_VERSION_MINOR    (00)


/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/

/* R_TOUCH_Control() commands */
typedef enum e_touch_cmd
{
    TOUCH_CMD_SET_METHOD,           // scan specified method only
    TOUCH_CMD_CYCLE_ALL_METHODS,    // scan through all methods in order used in Open()
    TOUCH_CMD_CYCLE_METHOD_LIST,    // scan/cycle through specified method list
    TOUCH_CMD_END_ENUM
} touch_cmd_t;

/* TOUCH_CMD_CYCLE_METHOD_LIST argument. */
typedef struct
{
    uint8_t     num_methods;                // number of methods to scan
    uint8_t     methods[QE_MAX_METHODS];    // methods in scan order
    uint8_t     cur_index;                  // (unused by application)
} touch_mlist_t;


/***********************************************************************************************************************
* Exported global variables
***********************************************************************************************************************/


/***********************************************************************************************************************
* Exported global functions
***********************************************************************************************************************/
qe_err_t R_TOUCH_Open(ctsu_cfg_t *p_ctsu_cfgs[], touch_cfg_t *p_touch_cfgs[], uint8_t num_methods, qe_trig_t trigger);
qe_err_t R_TOUCH_UpdateDataAndStartScan(void);                                  // for software triggers
qe_err_t R_TOUCH_UpdateData(void);                                              // for external triggers
qe_err_t R_TOUCH_Control(touch_cmd_t cmd, void *p_arg);
qe_err_t R_TOUCH_GetRawData(uint8_t method, uint16_t *p_buf, uint8_t *p_cnt);   // for advanced debugging
qe_err_t R_TOUCH_GetData(uint8_t method, uint16_t *p_buf, uint8_t *p_cnt);      // for advanced debugging
qe_err_t R_TOUCH_GetAllBtnStates(uint8_t method, uint64_t *p_mask);
qe_err_t R_TOUCH_GetSliderPosition(uint16_t slider_id, uint16_t *p_position);
qe_err_t R_TOUCH_GetWheelPosition(uint16_t wheel_id, uint16_t *p_position);
qe_err_t R_TOUCH_Close(void);
uint32_t R_TOUCH_GetVersion (void);

#endif // QETOUCH_RX_IF_H_FILE
