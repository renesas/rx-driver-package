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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_qectsu_rx_private.h
* Description  : This file includes definitions internal to the QE CTSU driver.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*           04.10.2018 1.00    First Release
*           27.03.2019 1.10    Added ctsu_pcmd_t.
*                              Moved ctsu_mode_t to r_ctsu_qe_if.h.
*                              Added extern for g_pvt_scan_info[].
*                              Added #define CTSU_SENSOR_SATURATION.
***********************************************************************************************************************/

#ifndef R_CTSU_QE_PRIVATE_H
#define R_CTSU_QE_PRIVATE_H

#include "r_ctsu_qe_if.h"


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define CTSU_SENSOR_SATURATION      (52000)     /* top of linear portion of correction curve */

#if (defined(BSP_MCU_RX113) || defined(BSP_MCU_RX130))
#define CTSU_HAS_TRIMMER_REG	(1)
#endif

#if (defined(BSP_MCU_RX130))
#define CTSU_HAS_TXVSEL         (1)
#endif

/* For parts with CTSUCHAC2/3/4 and CTSUTRC2/3/4 registers (more than 16 TS pins) */
#if (defined(BSP_MCU_RX23_ALL) || defined(BSP_MCU_RX130))
#define CTSU_HAS_LARGE_REG_SET  (1)
#endif


/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/* CTSU Driver Control Structure */
typedef struct st_ctsu_ctrl
{
    ctsu_cfg_t              **p_methods;    // pointer to array of pointers of all methods
    uint8_t                 num_methods;    // number of methods
    uint8_t                 cur_method;     // index into p_methods[]
    bsp_lock_t              lock;           // scan (or other critical operation) in progress
    uint8_t                 wr_index;       // index into elem_regs[]
    uint8_t                 rd_index;       // word index into scan data buffer
    void                    (*p_callback)(ctsu_isr_evt_t event, void *p_args);    // callback function pointer
    bool                    open;           // true = Open() completed
    ctsu_mode_t             mode;           // for active method (SELF or MUTUAL)
    qe_trig_t               trigger;        // for all methods
    volatile ctsu_state_t   state;          // for legacy touch driver; former g_ctsu_soft_mode
} ctsu_ctrl_t;


/* CTSU Private Control() Commands */
typedef enum e_ctsu_pcmd
{
    CTSU_PCMD_SET_OFFSET_TUNING_FLG,        // indicate Touch has started offset tuning
    CTSU_PCMD_CLR_OFFSET_TUNING_FLG,        // indicate Touch has finished offset tuning
    CTSU_PCMD_DISABLE_CTSU_INTS,            // disable CTSU interrupts
    CTSU_PCMD_ENABLE_CTSU_INTS,             // enable CTSU interrupts
    CTSU_PCMD_END_ENUM
} ctsu_pcmd_t;


/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
extern ctsu_ctrl_t  g_ctsu_ctrl;
extern ctsu_cfg_t   *gp_cur_cfg;                // ptr to current configuration/method
extern uint8_t      g_correction_mode;              // for StartScan() lock checking
extern uint16_t     g_correction_dtc_txd[3];            // for write int routine
extern volatile scan_info_t     g_pvt_scan_info[];

qe_err_t        r_ctsu_control_private(ctsu_pcmd_t pcmd, uint8_t method, void *p_arg);
extern qe_err_t correction_CTSU_sensor_ico(void);   // performs sensor gain correction
extern void     CTSUInterrupt(void);                // scan done int from legacy driver (handles correction, more)


#endif /* R_CTSU_QE_PRIVATE_H */
