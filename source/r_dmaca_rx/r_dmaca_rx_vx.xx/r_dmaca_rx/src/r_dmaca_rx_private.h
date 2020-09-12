/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_dmaca_rx_private.h
* Description  : Specific file for DMACA driver
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 30.06.2014 1.00    Initial revision
*         : 27.08.2014 1.01    Not change program due to updating application note only.
*         : 30.01.2015 1.02    Added RX71M.
*         : 13.04.2015 1.03    Added RX231 and RX230.
*         : 30.09.2016 1.04    Added RX65N.
*         : 31.07.2017 1.05    Supported RX65N-2MB.
*                              Fixed to correspond to Renesas coding rule.
*         : 28.09.2018 1.10    Supported RX66T.
*         : 01.02.2019 1.20    Supported RX72T, RX65N-64pin.
*         : 20.05.2019 2.00    Added support for GNUC and ICCRX.
*         : 28.06.2019 2.10    Supported RX23W.
*         : 15.08.2019 2.20    Supported RX72M.
*         : 30.12.2019 2.30    Added support RX66N, RX72N.
*                              Fixed to comply with GSCE Coding Standards Rev.6.00.
*******************************************************************************/
#ifndef DMACA_RX_PRIVATE_H
#define DMACA_RX_PRIVATE_H

/*******************************************************************************
Includes   <System Includes>, "Project Includes"
*******************************************************************************/
/* bool support */
#include <stdbool.h>
/* DMACA driver targets header file */
#if   defined(BSP_MCU_RX64M)
    #include "./src/targets/rx64m/r_dmaca_rx_target.h"
#elif defined(BSP_MCU_RX65N)
    #include "./src/targets/rx65n/r_dmaca_rx_target.h"
#elif defined(BSP_MCU_RX66N)
    #include "./src/targets/rx66n/r_dmaca_rx_target.h"
#elif defined(BSP_MCU_RX66T)
    #include "./src/targets/rx66t/r_dmaca_rx_target.h"
#elif defined(BSP_MCU_RX71M)
    #include "./src/targets/rx71m/r_dmaca_rx_target.h"
#elif defined(BSP_MCU_RX72T)
    #include "./src/targets/rx72t/r_dmaca_rx_target.h"
#elif defined(BSP_MCU_RX72M)
    #include "./src/targets/rx72m/r_dmaca_rx_target.h"
#elif defined(BSP_MCU_RX72N)
    #include "./src/targets/rx72n/r_dmaca_rx_target.h"
#elif defined(BSP_MCU_RX230)
    #include "./src/targets/rx230/r_dmaca_rx_target.h"
#elif defined(BSP_MCU_RX231)
    #include "./src/targets/rx231/r_dmaca_rx_target.h"
#elif defined(BSP_MCU_RX23W)
    #include "./src/targets/rx23w/r_dmaca_rx_target.h"
#else
    #error "This MCU is not supported by the current DMACA driver."
#endif

/*****************************************************************************
Macro definitions
******************************************************************************/


/*****************************************************************************
Typedef definitions
******************************************************************************/
typedef enum e_dmaca_chk_locking_sw
{
    DMACA_ALL_CH_UNLOCKED_AND_DTC_UNLOCKED = 0,             /* All DMAC channels and DTC are unlocked. */
    DMACA_ALL_CH_UNLOCKED_BUT_DTC_LOCKED,                   /* All DMAC channels are unlocked, but DTC is locked. */
    DMACA_LOCKED_CH_EXIST                                   /* Other DMAC channels are locked. */
 } dmaca_chk_locking_sw_t;

/* DMCNT.DTE(b0) : Configurable options for DMA Transfer Enable control for corresponding channel */
typedef enum e_dmaca_transfer_enable
{
    /* Disables DMA transfer. */
    DMACA_TRANSFER_DISABLE  = (0),

    /* Enables DMAC transfer. */
    DMACA_TRANSFER_ENABLE    = (1)
} dmaca_transfer_enable_t;

/* DMREQ.CLRS(b4) : Configurable options for DMA Software Start Bit Auto Clear Select */
typedef enum e_dmaca_swreq_auto_clear
{
    /* =(0 << 4) : SWREQ bit is cleared after DMA transfer is started by software. */
    DMACA_SOFTWARE_START_BIT_CLEAR      = (0),

    /* SWREQ bit is not cleared after DMA transfer is started by software. */

    DMACA_SOFTWARE_START_BIT_NOT_CLEAR  = (1 << 4)
} dmaca_swreq_auto_clear_t;

/* DMREQ.SWREQ(b0) : Configurable options for DMA Software Start */
typedef enum e_dmaca_swreq
{
    /* DMA transfer is not requested. */
    DMACA_SOFTWARE_START_NOT_REQUESTED  = (0),

    /* DMA transfer is requested. */
    DMACA_SOFTWARE_START_REQUESTED      = (1)
} dmaca_swreq_t;


/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/
extern void *g_pdmaci_handlers[];
extern volatile uint8_t R_BSP_EVENACCESS_SFR *g_icu_dmrsr[];
extern uint8_t                       g_locking_sw[DMACA_NUM_CHANNELS];

bool           r_dmaca_channel_valid_check(uint8_t channel);
void           r_dmaca_module_enable(void);
void           r_dmaca_module_disable(void);
dmaca_return_t r_dmaca_int_enable(uint8_t channel, uint8_t priority);
dmaca_return_t r_dmaca_int_disable(uint8_t channel);
#if ((0 == BSP_CFG_USER_LOCKING_ENABLED) && (bsp_lock_t == BSP_CFG_USER_LOCKING_TYPE))
  #if (1 != DMACA_CFG_USE_DTC_FIT_MODULE)
    dmaca_chk_locking_sw_t r_dmaca_check_dtc_locking_byuser(void);
  #endif
#else
    dmaca_chk_locking_sw_t r_dmaca_check_DMACA_DTC_locking_byUSER(void);
#endif

#endif /* DMACA_RX_PRIVATE_H */

/* End of File */

