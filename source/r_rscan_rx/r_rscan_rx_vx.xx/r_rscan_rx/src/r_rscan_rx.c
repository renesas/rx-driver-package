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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_rscan_rx.c
* Description  : Primary source code for the RSCAN driver.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           22.03.2015 1.00    Initial Release
*           20.05.2019 2.00    Add support for GNUC and ICCRX.
*           15.10.2019 2.20    Added support for RX23E-A
*                              Added support for atomic control
*                              Fixed warning in IAR
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <string.h>         /* memset()*/

#include "platform.h"
/* Interface file for this package. */
#include "r_rscan_rx_if.h"
/* Configuration file for this package. */
#include "r_rscan_rx_config.h"
/* Private header file for this package. */
#include "r_rscan_rx_private.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define CAN_BOX_TYPE_MASK           (0xF0000000)
#define CAN_BOX_BUFNUM_MASK         (0x000001FF)    /* tx and rx buffer number*/
#define CAN_BOX_RXFIFO_MASK         (0x000000FF)

#define CAN_NUM_RXFIFOS             (2)
#define CAN_MAX_CH_RULES            (16)


/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/* driver control block */
static can_dcb_t    g_dcb = {FALSE, 0, 0, NULL};

/* channel control block */
static can_ccb_t    g_ccb = {FALSE, NULL};


/* channel registers */
/*static can_chan_t       *g_ch_regs = (can_chan_t *)&RSCAN0C0CFG;*/

/* local functions */
static void     can_enable_ints(void);
static void     can_enable_rxfifos(void);
static void     can_enable_chan_fifos(void);

/* interrupts */
R_BSP_ATTRIB_STATIC_INTERRUPT void can_global_err_isr(void);
R_BSP_ATTRIB_STATIC_INTERRUPT void can_rxfifo_isr(void);
R_BSP_ATTRIB_STATIC_INTERRUPT void can_ch0_error_isr(void);
R_BSP_ATTRIB_STATIC_INTERRUPT void can_ch0_tx_isr(void);


/******************************************************************************
* Function Name: R_CAN_Open
* Description  : This function applies clock to the CAN peripheral, sets the
*                mode to RESET, configures the timestamp and all receive
*                mailboxes.
*
* Arguments    : p_cfg-
*                    Pointer to configuration structure (see below)
*                p_callback-
*                    Optional pointer to function called from RXIF or global
*                    error interrupts.
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_OPENED-
*                    Call to Open already made
*                CAN_ERR_INVALID_ARG-
*                    An element of the p_cfg structure contains an invalid value.
*                CAN_ERR_MISSING_CALLBACK-
*                    A callback function was not provided and a main callback
*                    interrupt is enabled in config.h
*******************************************************************************/
can_err_t R_CAN_Open(can_cfg_t  *p_cfg, void(* const p_callback)(can_cb_evt_t   event,void *p_args))
{
can_err_t   err = CAN_SUCCESS;
#if (R_BSP_VERSION_MAJOR >= 5) && (R_BSP_VERSION_MINOR >= 30)
bsp_int_ctrl_t int_ctrl;
#endif

#if CAN_CFG_PARAM_CHECKING_ENABLE
    if ((NULL == p_cfg)
     || (p_cfg->timestamp_src >= CAN_TIMESTAMP_SRC_END_ENUM)
     || (p_cfg->timestamp_div >= CAN_TIMESTAMP_DIV_END_ENUM))
    {
        return CAN_ERR_INVALID_ARG;
    }

    if ((NULL == p_callback) && ((CAN_CFG_INT_RXFIFO_THRESHOLD != 0)
                              || (CAN_CFG_INT_DLC_ERR != 0)
                              || (CAN_CFG_INT_FIFO_OVFL != 0)
                              || (CAN_CFG_INT_HIST_FIFO_OVFL != 0)))
    {
        return CAN_ERR_MISSING_CALLBACK;
    }

    if (TRUE == g_dcb.opened)
    {
        return CAN_ERR_OPENED;
    }
#endif
    g_dcb.opened = TRUE;

    /* Apply clock to the CAN interface */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
#if (R_BSP_VERSION_MAJOR >= 5) && (R_BSP_VERSION_MINOR >= 30)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    MSTP(RSCAN) = 0;
#if (R_BSP_VERSION_MAJOR >= 5) && (R_BSP_VERSION_MINOR >= 30)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);



    /* Wait for CAN RAM initialization to completed */
    while (0 != RSCAN.GSTS.BIT.GRAMINIT)
    {
        R_BSP_NOP();
    }

    /* Apply clock to RSCAN */
    RSCAN.GCTRL.BIT.GSLPR = 0;

    /* Enter global reset mode */
    RSCAN.GCTRL.BIT.GMDC = CAN_GLOBAL_MODE_RESET;

    /* Set register page/bank to NOT receive rules */
    RSCAN.GRWCR.BIT.RPAGE = 1;


    /* Set main clock and timestamp source */

    RSCAN.GCFGL.BIT.DCS = CAN_CFG_CLOCK_SOURCE;

    RSCAN.GCFGL.BIT.TSSS = p_cfg->timestamp_src;
    RSCAN.GCFGL.BIT.TSP = p_cfg->timestamp_div;
    RSCAN.GCTRH.BIT.TSRST = 1;  // reset timestamp counter


    /* Setup rest of global configuration */
    RSCAN.GCFGL.BIT.TPRI = 1;   // transmit priority mailbox number based, not msg ID
    RSCAN.GCFGL.BIT.DCE = 1;    // enable receive rule check DLC
    RSCAN.GCFGL.BIT.DRE = 0;    // DLC replacement disabled
    RSCAN.GCFGL.BIT.MME = 0;    // mirror function disabled

    /* interval timer prescaler (BIT.ITRCP) not used*/

    /* Setup 4 receive mailboxes */
    RSCAN.RMNB.WORD = 4;

    /* Configure global interrupts */
    RSCAN.GCTRL.BIT.DEIE = CAN_CFG_INT_DLC_ERR;
    RSCAN.GCTRL.BIT.MEIE = CAN_CFG_INT_FIFO_OVFL;
    RSCAN.GCTRL.BIT.THLEIE = CAN_CFG_INT_HIST_FIFO_OVFL;

    /* Save callback function pointer (may be NULL) */
    g_dcb.p_callback = p_callback;


    /* Set priority for all interrupt handlers, but enable later as needed */
    IPR(RSCAN, COMFRXINT) = CAN_CFG_INT_PRIORITY;       
    IPR(RSCAN, RXFINT) = CAN_CFG_INT_PRIORITY;
    IPR(RSCAN, TXINT) = CAN_CFG_INT_PRIORITY;
    IPR(RSCAN, CHERRINT) = CAN_CFG_INT_PRIORITY;
    IPR(RSCAN, GLERRINT) = CAN_CFG_INT_PRIORITY;

    return err;
}
/******************************************************************************
* End of function R_CAN_Open
*******************************************************************************/

/******************************************************************************
* Function Name: R_CAN_InitChan
* Description  : This function applies clock to the channel, puts the channel
*                in Reset mode, and sets the bit rate registers.
*
* Arguments    : chan -
*                    Channel to initialize (ignored)
*                p_baud-
*                    Pointer to bit rate structure
*                p_chcallback-
*                    Optional pointer to function called for transmit,
*                    gateway receive, and channel error interrupts
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_ILLEGAL_MODE-
*                    Not in global reset mode (results from Open())
*                CAN_ERR_INVALID_ARG-
*                    An invalid argument was provided
*                CAN_ERR_MISSING_CALLBACK-
*                    A callback function was not provided and a channel
*                    interrupt is enabled in config.h
*******************************************************************************/
can_err_t R_CAN_InitChan(uint8_t        chan,
                         can_bitrate_t  *p_baud,
                         void          (* const p_chcallback)(uint8_t      chan,
                                                              can_cb_evt_t event,
                                                              void         *p_args))
{
can_err_t   err         = CAN_SUCCESS;

#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((0 != RSCAN.GCTRL.BIT.GSLPR)
        || (CAN_GLOBAL_MODE_RESET != RSCAN.GCTRL.BIT.GMDC))
    {
        return CAN_ERR_ILLEGAL_MODE;
    }

    if ((NULL == p_baud)
     || (p_baud->prescaler < 1) || (p_baud->prescaler > 1024)
     || (p_baud->tseg1 < 4) || (p_baud->tseg1 > 16)
     || (p_baud->tseg2 < 2) || (p_baud->tseg2 > 8)
     || (p_baud->sjw < 1) || (p_baud->sjw > 4))
    {
        return CAN_ERR_INVALID_ARG;
    }

    if ((NULL == p_chcallback) && ((CAN_CFG_INT_TXFIFO_THRESHOLD != 0)
                                || (CAN_CFG_INT_HIST_FIFO_THRESHOLD != 0)
                                || (CAN_CFG_INT_MBX_TX_COMPLETE != 0)
                                || (CAN_CFG_INT_MBX_TX_ABORTED != 0)
                                || (CAN_CFG_INT_BUS_ERROR != 0)
                                || (CAN_CFG_INT_ERR_WARNING != 0)
                                || (CAN_CFG_INT_ERR_PASSIVE != 0)
                                || (CAN_CFG_INT_BUS_OFF_ENTRY != 0)
                                || (CAN_CFG_INT_BUS_OFF_RECOVERY != 0)
                                || (CAN_CFG_INT_OVERLOAD_FRAME_TX != 0)
                                || (CAN_CFG_INT_BUS_LOCK != 0)
                                || (CAN_CFG_INT_ARB_LOST != 0)))
    {
        return CAN_ERR_MISSING_CALLBACK;
    }

    if (TRUE == g_ccb.initialized)
    {
        return CAN_ERR_INIT_DONE;
    }
#endif /* end of #if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)*/
    g_ccb.initialized = TRUE;


    /* Apply clock to channel */
    RSCAN0.CTRL.BIT.CSLPR = 0;

    /* Enter channel reset mode */
    RSCAN0.CTRL.BIT.CHMDC = CAN_CHMODE_RESET;       // also resets all transmit buffers

    /* Set bit rate */
    RSCAN0.CFGL.BIT.BRP = p_baud->prescaler - 1;
    RSCAN0.CFGH.BIT.TSEG1 = p_baud->tseg1 - 1;
    RSCAN0.CFGH.BIT.TSEG2 = p_baud->tseg2 - 1;
    RSCAN0.CFGH.BIT.SJW = p_baud->sjw - 1;

    /* Configure channel interrupts */
    RSCAN0.CTRL.BIT.BEIE = CAN_CFG_INT_BUS_ERROR;
    RSCAN0.CTRL.BIT.EWIE = CAN_CFG_INT_ERR_WARNING;
    RSCAN0.CTRL.BIT.EPIE = CAN_CFG_INT_ERR_PASSIVE;
    RSCAN0.CTRL.BIT.BOEIE = CAN_CFG_INT_BUS_OFF_ENTRY;
    RSCAN0.CTRL.BIT.BORIE = CAN_CFG_INT_BUS_OFF_RECOVERY;
    RSCAN0.CTRL.BIT.OLIE = CAN_CFG_INT_OVERLOAD_FRAME_TX;
    RSCAN0.CTRL.BIT.BLIE = CAN_CFG_INT_BUS_LOCK;
    RSCAN0.CTRL.BIT.ALIE = CAN_CFG_INT_ARB_LOST;
    RSCAN0.CTRH.BIT.TAIE = CAN_CFG_INT_MBX_TX_ABORTED;

    /* Configure transmit complete interrupts for all 4 transmit mailboxes for channel */
    RSCAN0.TMIEC.WORD = (CAN_CFG_INT_MBX_TX_COMPLETE == 0) ? 0x0000 : 0x000F;

    /* Save channel callback function (may be NULL) */
    g_ccb.p_chcallback = p_chcallback;

    /* Configure other channel settings */
    RSCAN0.CTRH.BIT.BOM = CAN_BO_RECOVERY_ISO;      // bus off recovery mode
    RSCAN0.CTRH.BIT.ERRD = 1;                       // error flags are accumulative (0/1)


    return err;
}
/******************************************************************************
 End of function R_CAN_InitChan
 *****************************************************************************/


/******************************************************************************
* Function Name: R_CAN_ConfigFIFO
* Description  : This function configures the FIFO specified by the box id
*                passed in. It does not become enabled until the channel
*                mode has changed.
*                RFCCx.RFE - enable in global operating or global test mode
*                CFCCk.CFE - enable in chan comm or chan halt mode
*                THLCCm.THLE - enable in chan comm or chan halt mode
*
* Arguments    : fifo_id -
*                    Box ID for FIFO to configure.
*                threshold -
*                    Number of entries in FIFO to set interrupt flag
*                txmbx_id -
*                    Associated tx mailbox for TXFIFOs.
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_ILLEGAL_MODE-
*                    Not in global reset mode (results from Open())
*                CAN_ERR_CH_NO_INIT-
*                    Channel not initialized, and non-RX FIFO is requested
*                CAN_ERR_INVALID_ARG-
*                    An argument contains an invalid value.
*******************************************************************************/
can_err_t R_CAN_ConfigFIFO(can_box_t            fifo_id,
                           can_fifo_threshold_t threshold,
                           can_box_t            txmbx_id)
{
uint32_t    i;
can_err_t   err = CAN_SUCCESS;


#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((0 != RSCAN.GCTRL.BIT.GSLPR)
        || (CAN_GLOBAL_MODE_RESET != RSCAN.GCTRL.BIT.GMDC))
    {
        return CAN_ERR_ILLEGAL_MODE;
    }

    if ((CAN_BOX_TXFIFO == fifo_id)
        && (CAN_CHMODE_RESET != RSCAN0.CTRL.BIT.CHMDC))
    {
        return CAN_ERR_ILLEGAL_MODE;
    }

    /* Verify box id is for FIFO and threshold is valid */
    if ((fifo_id & CAN_BOX_TYPE_MASK) != CAN_FLG_FIFO)
    {
        return CAN_ERR_INVALID_ARG;
    }
    else if (CAN_BOX_HIST_FIFO == fifo_id)
    {
        if ((CAN_FIFO_THRESHOLD_1 != threshold) && (CAN_FIFO_THRESHOLD_6 != threshold))
        {
            return CAN_ERR_INVALID_ARG;
        }
    }
    else
    {
        if ((CAN_FIFO_THRESHOLD_1 != threshold)
         && (CAN_FIFO_THRESHOLD_2 != threshold)
         && (CAN_FIFO_THRESHOLD_3 != threshold)
         && (CAN_FIFO_THRESHOLD_FULL != threshold))
        {
            return CAN_ERR_INVALID_ARG;
        }
    }

    /* Check if FIFO already configured */
    if ((g_dcb.fifo_mask & fifo_id) == fifo_id)
    {
        return CAN_ERR_INVALID_ARG;
    }

    /* Check txmbx argument (only for TXFIFO) */
    if (CAN_BOX_TXFIFO == fifo_id)
    {
        /* verify a txmbx box id was passed */
        if ((txmbx_id & CAN_FLG_TXMBX) == 0)
        {
            return CAN_ERR_INVALID_ARG;
        }

        /* chan must be initialized so removal of txmbx interrupt is not overwritten later */
        if (FALSE == g_ccb.initialized)
        {
            return CAN_ERR_CH_NO_INIT;
        }
    }
#endif /* end of (CAN_CFG_PARAM_CHECKING_ENABLE == 1)*/


    if (fifo_id & CAN_BOX_RXFIFO_MASK)
    {
        if (CAN_BOX_RXFIFO_0 == fifo_id)
        {
            /* set FIFO depth, threshold, and threshold int */
            RSCAN.RFCC0.BIT.RFDC = CAN_FIFO_DEPTH_4;
            RSCAN.RFCC0.BIT.RFIE = CAN_CFG_INT_RXFIFO_THRESHOLD;
            if (CAN_FIFO_THRESHOLD_1 == threshold)
            {
                RSCAN.RFCC0.BIT.RFIM = 1;               // set int flag at every message
            }
            else
            {
                RSCAN.RFCC0.BIT.RFIM = 0;               // set int flag at threshold
                RSCAN.RFCC0.BIT.RFIGCV = threshold;
            }
        }
        else
        {
            /* set FIFO depth, threshold, and threshold int */
            RSCAN.RFCC1.BIT.RFDC = CAN_FIFO_DEPTH_4;
            RSCAN.RFCC1.BIT.RFIE = CAN_CFG_INT_RXFIFO_THRESHOLD;
            if (CAN_FIFO_THRESHOLD_1 == threshold)
            {
                RSCAN.RFCC1.BIT.RFIM = 1;               // set int flag at every message
            }
            else
            {
                RSCAN.RFCC1.BIT.RFIM = 0;               // set int flag at threshold
                RSCAN.RFCC1.BIT.RFIGCV = threshold;
            }
        }

        /* enable RXFIFO only in global operating mode or global test mode */
    }

    else if (CAN_BOX_TXFIFO == fifo_id)
    {
        /* set FIFO type, depth, threshold, and threshold int */

        RSCAN0.CFCCL0.BIT.CFDC = CAN_FIFO_DEPTH_4;

        RSCAN0.CFCCH0.BIT.CFM = CAN_FIFO_MODE_TRANSMIT;
        RSCAN0.CFCCH0.BIT.CFITT = 0;                // interval timer off
        RSCAN0.CFCCL0.BIT.CFTXIE = CAN_CFG_INT_TXFIFO_THRESHOLD;
        RSCAN0.CFCCL0.BIT.CFRXIE = 0;

        if (CAN_FIFO_THRESHOLD_1 == threshold)
        {
            RSCAN0.CFCCL0.BIT.CFIM = 1;             // set int flag at every message
        }
        else
        {
            RSCAN0.CFCCL0.BIT.CFIM = 0;             // set int flag at threshold
            RSCAN0.CFCCL0.BIT.CFIGCV = threshold;
        }

        /* link associated transmit mailbox and save it */
        RSCAN0.CFCCH0.BIT.CFTML = txmbx_id & CAN_BOX_BUFNUM_MASK;

        /* disable the interrupt associated with the mailbox (may or may not be set) */
        i = txmbx_id & CAN_BOX_BUFNUM_MASK;         // get buffer number
        RSCAN0.TMIEC.WORD &= (~(1 << i));             // reset bit
    }

    else // history FIFO
    {
        /* log all transmit sources */
        RSCAN0.THLCC0.BIT.THLDTE = 1;

        /* set threshold */
        RSCAN0.THLCC0.BIT.THLIM = (CAN_FIFO_THRESHOLD_1 == threshold) ? 1 : 0;

        /* set interrupt enable */
        RSCAN0.THLCC0.BIT.THLIE = CAN_CFG_INT_HIST_FIFO_THRESHOLD;
    }

    /* Show FIFO as being used */
    g_dcb.fifo_mask |= fifo_id;

    return err;
}
/******************************************************************************
 End of function R_CAN_ConfigFIFO
 *****************************************************************************/


/******************************************************************************
* Function Name: can_enable_ints
* Description  : This function enables all interrupts that are configured.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void can_enable_ints(void)
{

    if ((CAN_CFG_INT_DLC_ERR || CAN_CFG_INT_FIFO_OVFL || CAN_CFG_INT_HIST_FIFO_OVFL) != 0)
    {
        IR(RSCAN,GLERRINT) = 0;
        R_BSP_InterruptRequestEnable(VECT(RSCAN,GLERRINT));
    }

    if (CAN_CFG_INT_RXFIFO_THRESHOLD == 1)
    {
        IR(RSCAN,RXFINT) = 0;
        R_BSP_InterruptRequestEnable(VECT(RSCAN,RXFINT));
    }

    if ((CAN_CFG_INT_BUS_ERROR
     ||  CAN_CFG_INT_ERR_WARNING
     ||  CAN_CFG_INT_ERR_PASSIVE
     ||  CAN_CFG_INT_BUS_OFF_ENTRY
     ||  CAN_CFG_INT_BUS_OFF_RECOVERY
     ||  CAN_CFG_INT_OVERLOAD_FRAME_TX
     ||  CAN_CFG_INT_BUS_LOCK
     ||  CAN_CFG_INT_ARB_LOST) != 0)
    {
        IR(RSCAN,CHERRINT) = 0;
        R_BSP_InterruptRequestEnable(VECT(RSCAN,CHERRINT));
    }

    if ((CAN_CFG_INT_MBX_TX_COMPLETE
     ||  CAN_CFG_INT_MBX_TX_ABORTED
     ||  CAN_CFG_INT_TXFIFO_THRESHOLD
     ||  CAN_CFG_INT_HIST_FIFO_THRESHOLD) != 0)
    {
        IR(RSCAN,TXINT) = 0;
        R_BSP_InterruptRequestEnable(VECT(RSCAN,TXINT));
    }

    return;
}
/******************************************************************************
 End of function can_enable_ints
 *****************************************************************************/


/******************************************************************************
* Function Name: R_CAN_AddRxRule
* Description  : This function
*
* Arguments    : p_cfg-
*                    Pointer to configuration structure (see below)
*                p_callback-
*                    Optional pointer to function called from RXIF or global
*                    error interrupts.
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_ILLEGAL_MODE
*                    Not in global reset mode (results from Open())
*                CAN_ERR_INVALID_ARG-
*                    An element of the p_cfg structure contains an invalid value.
*                CAN_ERR_MISSING_CALLBACK-
*                    A callback function was not provided and a main callback
*                    interrupt is enabled in config.h
*******************************************************************************/
can_err_t R_CAN_AddRxRule(uint8_t       chan,
                          can_filter_t  *p_filter,
                          can_box_t     dst_box)
{
uint32_t        i;
can_err_t       err = CAN_SUCCESS;
can_rxrule_t    *rule_arr = (can_rxrule_t *)&RSCAN.GAFLIDL0.WORD;


    /* Must clear here. Not documented as such. */
    if (0 == g_dcb.rule_count)
    {
        /* clear rule count */
        RSCAN.GRWCR.BIT.RPAGE = 0;
        RSCAN.GAFLCFG.BIT.RNC0 = 0;
        RSCAN.GRWCR.BIT.RPAGE = 1;
    }

#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((0 != RSCAN.GCTRL.BIT.GSLPR)
     || (CAN_GLOBAL_MODE_RESET != RSCAN.GCTRL.BIT.GMDC))
    {
        return CAN_ERR_ILLEGAL_MODE;
    }

    if ((NULL == p_filter)
     || (p_filter->check_ide > 1)
     || (p_filter->ide > 1)
     || (p_filter->check_rtr > 1)
     || (p_filter->rtr > 1)
     || (p_filter->min_dlc > 8))
    {
        return CAN_ERR_INVALID_ARG;
    }

    /* verify RXMBX or RXFIFO for dst_box */
    if (((dst_box & CAN_FLG_RXMBX) == 0)
      && (((dst_box & CAN_FLG_FIFO) == 0) || ((dst_box & CAN_BOX_RXFIFO_MASK) == 0)))
    {
        return CAN_ERR_INVALID_ARG;
    }

    /* if RXIFO, verify configured */
    if ((dst_box & CAN_FLG_FIFO)
     && ((g_dcb.fifo_mask & dst_box) != dst_box))
    {
        return CAN_ERR_INVALID_ARG;
    }

    /* verify not already at max number rules */
    if (16 == RSCAN.GAFLCFG.BIT.RNC0)
    {
        return CAN_ERR_MAX_RULES;
    }
#endif /* end of (CAN_CFG_PARAM_CHECKING_ENABLE == 1)*/

    /* Set page/bank to receive rule registers */
    RSCAN.GRWCR.BIT.RPAGE = 0;


    /* load rule info */
    i = RSCAN.GAFLCFG.BIT.RNC0;

    rule_arr[i].GAFLIDL.BIT.GAFLID = p_filter->id & 0xFFFF;

    rule_arr[i].GAFLIDH.BIT.GAFLIDE = p_filter->ide;
    rule_arr[i].GAFLIDH.BIT.GAFLRTR = p_filter->rtr;
    rule_arr[i].GAFLIDH.BIT.GAFLID = (p_filter->id >> 16) & 0x1FFF;
    rule_arr[i].GAFLIDH.BIT.GAFLLB = 0;         // no mirrored messages

    rule_arr[i].GAFLML.BIT.GAFLIDM = p_filter->id_mask & 0xFFFF;

    rule_arr[i].GAFLMH.BIT.GAFLIDEM = p_filter->check_ide;
    rule_arr[i].GAFLMH.BIT.GAFLRTRM = p_filter->check_rtr;
    rule_arr[i].GAFLMH.BIT.GAFLIDM = (p_filter->id_mask >> 16) & 0x1FFF;

    rule_arr[i].GAFLPH.BIT.GAFLDLC = p_filter->min_dlc;
    rule_arr[i].GAFLPH.BIT.GAFLPTR = p_filter->label;

    if ((dst_box & CAN_FLG_RXMBX) != 0)
    {
        rule_arr[i].GAFLPL.BIT.GAFLRMV = 1;     // RXMBX
        rule_arr[i].GAFLPL.BIT.GAFLRMDP = dst_box & CAN_BOX_BUFNUM_MASK;
    }
    else /* RXFIFO*/
    {
        rule_arr[i].GAFLPL.BIT.GAFLRMV = 0;     // RXFIFO
        rule_arr[i].GAFLPL.BIT.GAFLFDP0 = (CAN_BOX_RXFIFO_0 == dst_box) ? 1 : 0;
        rule_arr[i].GAFLPL.BIT.GAFLFDP1 = (CAN_BOX_RXFIFO_1 == dst_box) ? 1 : 0;
    }

    /* increment the number of rules in use */
    RSCAN.GAFLCFG.BIT.RNC0++;
    g_dcb.rule_count++;


    /* Set page/bank to NOT receive rule registers */
    RSCAN.GRWCR.BIT.RPAGE = 1;

    return err;
}
/******************************************************************************
 End of function R_CAN_AddRxRule
 *****************************************************************************/


/******************************************************************************
* Function Name: R_CAN_Control
* Description  : This function
*
* Arguments    : cmd-
*                    Command to execute
*                arg1-
*                    for CAN_CMD_ABORT_TX, this is a transmit mailbox id
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_NOT_OPENED-
*                    Call to Open not made
*                CAN_ERR_INVALID_ARG-
*                    An invalid value passed for one of the parameters.
*                CAN_ERR_CH_NO_INIT-
*                    Channel must be initialized before calling command
*******************************************************************************/
can_err_t R_CAN_Control(can_cmd_t   cmd,
                        uint32_t    arg1)
{
can_err_t   err = CAN_SUCCESS;
can_tmcp_t  *tmcp_regs = (can_tmcp_t *)&RSCAN0.TMC0;


#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    if (FALSE == g_dcb.opened)
    {
        return CAN_ERR_NOT_OPENED;
    }

    if ((cmd >= CAN_CMD_END_ENUM)
            || ((CAN_CMD_ABORT_TX == cmd) && ((arg1 & CAN_FLG_TXMBX) == 0))) // arg1 must be txmbx
    {
        return CAN_ERR_INVALID_ARG;
    }

    if ((CAN_CMD_RESET_TIMESTAMP != cmd) && (FALSE == g_ccb.initialized))
    {
        return CAN_ERR_CH_NO_INIT;
    }
#endif

    switch (cmd)
    {
    case CAN_CMD_ABORT_TX:
    {
        /* get the transmit buffer number (in communications mode) */
        arg1 &= CAN_BOX_BUFNUM_MASK;

        /* request abort */
        tmcp_regs[arg1].BIT.TMTAR = 1;
        break;
    }
    case CAN_CMD_RESET_TIMESTAMP:
    {
        RSCAN.GCTRH.BIT.TSRST = 1;
        break;
    }
    case CAN_CMD_SET_MODE_COMM:
    {
        /* set global operating mode */
        RSCAN.GCTRL.BIT.GMDC = CAN_GLOBAL_MODE_OPERATING;
        while ((RSCAN.GSTS.WORD & 0x07) != 0)
        {
            R_BSP_NOP();
        }

        /* enable configured RXFIFOs */
        can_enable_rxfifos();

        /* enable interrupts */
        can_enable_ints();

        /* put initialized channels into communications mode */
        if (TRUE == g_ccb.initialized)
        {
            RSCAN0.CTRL.BIT.CHMDC = CAN_CHMODE_COMM;
            while ((RSCAN0.STSL.WORD & 0x07) != 0)
            {
                R_BSP_NOP();
            }

            /* enable channel FIFOs */
            can_enable_chan_fifos();

            while (0 == RSCAN0.STSL.BIT.COMSTS)
            {
                R_BSP_NOP();
            }
        }
        break;
    }
    default:
    {
        /* set global test mode */
        RSCAN.GCTRL.BIT.GMDC = CAN_GLOBAL_MODE_TEST;
        while (1 != RSCAN.GSTS.BIT.GHLTSTS)
        {
            R_BSP_NOP();
        }

        /* enable configured RXFIFOs */
        can_enable_rxfifos();

        /* enable interrupts */
        can_enable_ints();

        /* put channel into appropriate test mode */

        /* set to channel halt mode */
        RSCAN0.CTRL.BIT.CHMDC = CAN_CHMODE_HALT;
        while ( 1 != RSCAN0.STSL.BIT.CHLTSTS)
            ;

        /* enable channel FIFOs */
        can_enable_chan_fifos();

        /* enable test mode */
        RSCAN0.CTRH.BIT.CTME = 1;

        /* set test mode type */
        if (CAN_CMD_SET_MODE_TST_STANDARD == cmd)
        {
            RSCAN0.CTRH.BIT.CTMS = CAN_TEST_MODE_STD;
        }
        else if (CAN_CMD_SET_MODE_TST_LISTEN == cmd)
        {
            RSCAN0.CTRH.BIT.CTMS = CAN_TEST_MODE_LISTEN;
        }
        else if (CAN_CMD_SET_MODE_TST_INT_LOOPBACK == cmd)
        {
            RSCAN0.CTRH.BIT.CTMS = CAN_TEST_MODE_INT_LOOPBACK;
        }
        else /* (cmd == CAN_CMD_SET_MODE_TST_EXT_LOOPBACK) */
        {
            RSCAN0.CTRH.BIT.CTMS = CAN_TEST_MODE_EXT_LOOPBACK;
        }

        /* set global operating mode */
        RSCAN.GCTRL.BIT.GMDC = CAN_GLOBAL_MODE_OPERATING;
        while (0 != RSCAN.GSTS.WORD)
        {
            R_BSP_NOP();
        }

        /* go into channel communication mode */
        RSCAN0.CTRL.BIT.CHMDC = CAN_CHMODE_COMM;
        while ((RSCAN0.STSL.WORD & 0x07) != 0)
        {
            R_BSP_NOP();
        }

        while (0 == RSCAN0.STSL.BIT.COMSTS)
        {
            R_BSP_NOP();
        }

        break;
    }
    }

    return err;
}
/******************************************************************************
 End of function R_CAN_Control
 *****************************************************************************/


/******************************************************************************
* Function Name: can_enable_rxfifos
* Description  : This function enables configured RXFIFOs.
*                SYSTEM NUST BE IN GLOBAL OPERATING OR GLOBAL TEST MODE
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void can_enable_rxfifos(void)
{

    if ((g_dcb.fifo_mask & CAN_MASK_RXFIFO_0) != 0)
    {
        RSCAN.RFCC0.BIT.RFE = 1;
    }

    if ((g_dcb.fifo_mask & CAN_MASK_RXFIFO_1) != 0)
    {
        RSCAN.RFCC1.BIT.RFE = 1;
    }
}
/******************************************************************************
 End of function can_enable_rxfifos
 *****************************************************************************/


/******************************************************************************
* Function Name: can_enable_chan_fifos
* Description  : This function enables configured channel-specific FIFOs.
*                SYSTEM NUST BE IN CHAN COMM OR CHAN HALT MODE
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void can_enable_chan_fifos()
{

    /* enable TX FIFO */
    if ((g_dcb.fifo_mask & CAN_MASK_TXFIFO) != 0)
    {
        RSCAN0.CFCCL0.BIT.CFE = 1;
    }

    /* enable HIST FIFO */
    if ((g_dcb.fifo_mask & CAN_MASK_HIST_FIFO) != 0)
    {
        RSCAN0.THLCC0.BIT.THLE = 1;
    }
}
/******************************************************************************
 End of function can_enable_chan_fifos
 *****************************************************************************/


/******************************************************************************
* Function Name: R_CAN_SendMsg
* Description  : This function checks to see if the transmit mailbox or FIFO
*                is already full. If not, the appropriate registers are loaded
*                with the message and marked as ready to send. If the message
*                is for a transmit mailbox, this function will block until
*                transmission is complete. For TXFIFOs and interrupt driven
*                transmit mailboxes, this function does not wait for transmit
*                complete.
* Arguments    : box_id-
*                    TXMBX or TXFIFO
*                p_txmsg-
*                    Pointer to structure containing message info to send
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_BOX_FULL
*                    Cannot send at this time
*                CAN_ERR_INVALID_ARG-
*                    An invalid value passed for one of the parameters.
*                CAN_ERR_ILLEGAL_MODE-
*                    Channel not in proper mode
*******************************************************************************/
can_err_t R_CAN_SendMsg(can_box_t   box_id,
                        can_txmsg_t *p_txmsg)
{
uint32_t        i;
can_err_t       err = CAN_SUCCESS;
can_tmstsp_t    *tmstsp_regs = (can_tmstsp_t *)&RSCAN0.TMSTS0;
can_tmcp_t      *tmcp_regs = (can_tmcp_t *)&RSCAN0.TMC0;
can_msg_t       *msg_regs;


    if (box_id & CAN_FLG_TXMBX)
    {
        /* get pointer to base of message registers and index for this TXMBX */
        msg_regs = (can_msg_t *)&RSCAN0.TMIDL0;
        i = box_id & CAN_BOX_BUFNUM_MASK;       // buffer number
    }
    else if (CAN_BOX_TXFIFO == box_id)
    {
        /* get pointer to base of message registers and index for this TXFIFO */
        msg_regs = (can_msg_t *)&RSCAN0.CFIDL0;
        i = 0;
    }
#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    else
    {
        return CAN_ERR_INVALID_ARG;             // not a transmit box
    }

    /* check message pointer provided */
    if (NULL == p_txmsg)
    {
        return CAN_ERR_INVALID_ARG;
    }

    /* check if in communication mode */
    if (0 == RSCAN0.STSL.BIT.COMSTS)
    {
        return CAN_ERR_ILLEGAL_MODE;
    }
#endif

    if (box_id & CAN_FLG_TXMBX)
    {
        /* return if transmit or abort request already outstanding */
        if ((tmcp_regs[i].BYTE & CAN_TMCP_REQ_PRESENT) != 0)
        {
            return CAN_ERR_BOX_FULL;
        }
    }
    else  /* TXFIFO */
    {
        /* return if fifo is full */
        if (1 == RSCAN0.CFSTS0.BIT.CFFLL)
        {
            return CAN_ERR_BOX_FULL;
        }
    }


    /* load message into registers */

    msg_regs[i].IDL.BIT.ID = p_txmsg->id & 0xFFFF;
    msg_regs[i].IDH.BIT.IDE = p_txmsg->ide;
    msg_regs[i].IDH.BIT.RTR = p_txmsg->rtr;
    msg_regs[i].IDH.BIT.ID = (p_txmsg->id >> 16) & 0x1FFF;
    msg_regs[i].IDH.BIT.THLEN = (uint16_t) p_txmsg->log_history;
    msg_regs[i].PTR.BIT.PTR = (uint8_t) p_txmsg->label;
    msg_regs[i].PTR.BIT.DLC = p_txmsg->dlc;
    msg_regs[i].DF0.BIT.DB0 = p_txmsg->data[0];
    msg_regs[i].DF0.BIT.DB1 = p_txmsg->data[1];
    msg_regs[i].DF1.BIT.DB2 = p_txmsg->data[2];
    msg_regs[i].DF1.BIT.DB3 = p_txmsg->data[3];
    msg_regs[i].DF2.BIT.DB4 = p_txmsg->data[4];
    msg_regs[i].DF2.BIT.DB5 = p_txmsg->data[5];
    msg_regs[i].DF3.BIT.DB6 = p_txmsg->data[6];
    msg_regs[i].DF3.BIT.DB7 = p_txmsg->data[7];

    /* initiate transmit request */

    if (box_id & CAN_FLG_TXMBX)
    {
        tmstsp_regs[i].BIT.TMTRF = 0;               // clear last result
        tmcp_regs[i].BIT.TMOM = p_txmsg->one_shot;  // set one-shot option
        tmcp_regs[i].BIT.TMTR = 1;                  // request transmit

        /* wait for transmit complete if not using interrupts */
        if (CAN_CFG_INT_MBX_TX_COMPLETE == 0)
        {
            while (0 == tmstsp_regs[i].BIT.TMTRF)
            {
                R_BSP_NOP();
            }
        }
    }
    else /* TXFIFO */
    {
        RSCAN0.CFPCTR0.WORD = 0xFF;   // increment TXFIFO index
    }

    return err;
}
/******************************************************************************
* End of function R_CAN_SendMsg
*******************************************************************************/


/******************************************************************************
* Function Name: R_CAN_GetMsg
* Description  : This function checks to see if the receive mailbox or FIFO
*                contains a message. If it does, the p_rxmsg buffer is loaded
*                with the message info.
*
* Arguments    : box_id-
*                    RXMBX or RXFIFO
*                p_rxmsg-
*                    Pointer to structure to hold message info
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_BOX_EMPTY
*                    No message currently waiting in receive mailbox or FIFO
*                CAN_ERR_INVALID_ARG-
*                    An invalid value passed for one of the parameters.
*                CAN_ERR_ILLEGAL_MODE-
*                    System not in proper mode
*******************************************************************************/
can_err_t R_CAN_GetMsg(can_box_t   box_id,
                       can_rxmsg_t *p_rxmsg)
{
uint32_t            i;
can_err_t           err = CAN_SUCCESS;
can_fifo_stat_t     *rfstsx_regs = (can_fifo_stat_t *)&RSCAN.RFSTS0;
volatile uint16_t   *rfpctrx_regs = (uint16_t *)&RSCAN.RFPCTR0;
can_msg_t           *msg_regs;
uint16_t            tmp;


    if (box_id & CAN_FLG_RXMBX)
    {
        /* get pointer to base of message registers and index for this RXMBX */
        msg_regs = (can_msg_t *)&RSCAN.RMIDL0;
        i = box_id & CAN_BOX_BUFNUM_MASK;           // buffer number
    }
    else if (((box_id & CAN_FLG_FIFO) != 0) && ((box_id & CAN_BOX_RXFIFO_MASK) != 0))
    {
        /* get pointer to base of message registers and index for this RXFIFO */
        msg_regs = (can_msg_t *)&RSCAN.RFIDL0;
        i = (CAN_BOX_RXFIFO_0 == box_id) ? 0 : 1;     // RXFIFO number
    }
#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    else
    {
        return CAN_ERR_INVALID_ARG;                 // not a receive box
    }

    if (NULL == p_rxmsg)
    {
        return CAN_ERR_INVALID_ARG;
    }
#endif

    if (box_id & CAN_FLG_RXMBX)
    {
        /* return if receive mailbox is empty */
        if ((RSCAN.RMND0.WORD & (1 << i)) == 0)     /* receive buffer 0-15 */
        {
            return CAN_ERR_BOX_EMPTY;
        }
    }
    else /* RXFIFO*/
    {
        /* return if FIFO is empty */
        if (1 == rfstsx_regs[i].BIT.EMP)
        {
            return CAN_ERR_BOX_EMPTY;
        }
    }


    /* load message from registers */
    p_rxmsg->ide = msg_regs[i].IDH.BIT.IDE;
    p_rxmsg->rtr = msg_regs[i].IDH.BIT.RTR;
    tmp = msg_regs[i].IDL.BIT.ID;
    p_rxmsg->id = (msg_regs[i].IDH.BIT.ID << 16) | tmp;
    p_rxmsg->label = msg_regs[i].PTR.BIT.PTR;
    p_rxmsg->dlc = msg_regs[i].PTR.BIT.DLC;
    p_rxmsg->timestamp = msg_regs[i].TS.BIT.TS;
    p_rxmsg->data[0] = msg_regs[i].DF0.BIT.DB0;
    p_rxmsg->data[1] = msg_regs[i].DF0.BIT.DB1;
    p_rxmsg->data[2] = msg_regs[i].DF1.BIT.DB2;
    p_rxmsg->data[3] = msg_regs[i].DF1.BIT.DB3;
    p_rxmsg->data[4] = msg_regs[i].DF2.BIT.DB4;
    p_rxmsg->data[5] = msg_regs[i].DF2.BIT.DB5;
    p_rxmsg->data[6] = msg_regs[i].DF3.BIT.DB6;
    p_rxmsg->data[7] = msg_regs[i].DF3.BIT.DB7;

    /* clear message ready */

    if (box_id & CAN_FLG_RXMBX)
    {
        RSCAN.RMND0.WORD &= (~(1 << i));
    }
    else /* RXFIFO */
    {
        rfpctrx_regs[i] = 0xFF;         // increment RXFIFO index
    }

    return err;
}
/******************************************************************************
 End of function R_CAN_GetMsg
 *****************************************************************************/


/******************************************************************************
* Function Name: R_CAN_GetHistoryEntry
* Description  : This function determines if there is an entry in the History
*                FIFO, and if so, determines the box_id of the transmit source
*                and loads the label associated with the message sent.
*
* Arguments    : box_id-
*                    History FIFO
*                p_entry-
*                    Pointer to structure to hold message info
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_BOX_EMPTY
*                    No entries in FIFO
*                CAN_ERR_INVALID_ARG-
*                    An invalid value passed for one of the parameters.
*                CAN_ERR_ILLEGAL_MODE-
*                    Channel not in proper mode
*******************************************************************************/
can_err_t R_CAN_GetHistoryEntry(can_box_t       box_id,
                                can_history_t   *p_entry)
{
uint32_t    txbuf_num;
can_err_t   err = CAN_SUCCESS;


#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    if (CAN_BOX_HIST_FIFO != box_id)
    {
        return CAN_ERR_INVALID_ARG;             // not a History FIFO box
    }

    if (NULL == p_entry)
    {
        return CAN_ERR_INVALID_ARG;
    }
#endif

    /* return if FIFO is empty */
    if (1 == RSCAN0.THLSTS0.BIT.THLEMP)
    {
        return CAN_ERR_BOX_EMPTY;
    }


    /* get tx buffer number of log entry */
    txbuf_num = RSCAN0.THLACC0.BIT.BN;

    /* get box associated with tx buffer and create box_id */
    if (HIST_SRC_TX_BUF == RSCAN0.THLACC0.BIT.BT)
    {
        p_entry->box_id = (can_box_t)(CAN_FLG_TXMBX | txbuf_num);
    }
    else /* HIST_SRC_TXRX_FIFO */
    {
        p_entry->box_id = CAN_BOX_TXFIFO;
    }

    /* load message label */
    p_entry->label = RSCAN0.THLACC0.BIT.TID;

    /* increment HIST FIFO index */
    RSCAN0.THLPCTR0.WORD = 0xFF;

    return err;
}
/******************************************************************************
 End of function R_CAN_GetHistoryEntry
 *****************************************************************************/


/******************************************************************************
* Function Name: R_CAN_GetStatusMask
* Description  : This function returns various status masks based upon the
*                the type requested.
* Arguments    : type-
*                    Which type of status to return
*                chan-
*                    Optional channel number. Needed for CAN_STAT_CH_xxx types.
*                    Always 0 for RX231.
*                p_err-
*                    Pointer to hold error code
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_INVALID_ARG-
*                    An invalid value passed for one of the parameters.
*******************************************************************************/
uint32_t  R_CAN_GetStatusMask(can_stat_t    type,
                              uint8_t       chan,
                              can_err_t     *p_err)
{
can_fifo_mask_t stat;


#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((type >= CAN_STAT_END_ENUM) || (NULL == p_err))
    {
        *p_err = CAN_ERR_INVALID_ARG;
        return 0;
    }
#endif

    switch (type)
    {
    case CAN_STAT_FIFO_EMPTY:
    {
        stat.uint32 = 0;
        stat.bit.rxfifo_0 = RSCAN.RFSTS0.BIT.RFEMP;
        stat.bit.rxfifo_1 = RSCAN.RFSTS1.BIT.RFEMP;
        stat.bit.ch0_txfifo_0 = RSCAN0.CFSTS0.BIT.CFEMP;
        stat.bit.ch0_hist = RSCAN0.THLSTS0.BIT.THLEMP;
        break;
    }
    case CAN_STAT_FIFO_THRESHOLD:
    {
        stat.uint32 = 0;
        stat.bit.rxfifo_0 = RSCAN.RFSTS0.BIT.RFIF;
        stat.bit.rxfifo_1 = RSCAN.RFSTS1.BIT.RFIF;
        stat.bit.ch0_txfifo_0 = RSCAN0.CFSTS0.BIT.CFTXIF;
        stat.bit.ch0_hist = RSCAN0.THLSTS0.BIT.THLIF;

        /* clear threshold interrupt flags */
        RSCAN.RFSTS0.BIT.RFIF = 0;
        RSCAN.RFSTS1.BIT.RFIF = 0;
        RSCAN0.CFSTS0.BIT.CFTXIF = 0;
        RSCAN0.THLSTS0.BIT.THLIF = 0;
        break;
    }
    case CAN_STAT_FIFO_OVFL:
    {
        stat.uint32 = 0;
        stat.bit.rxfifo_0 = RSCAN.RFSTS0.BIT.RFMLT;
        stat.bit.rxfifo_1 = RSCAN.RFSTS1.BIT.RFMLT;
        stat.bit.ch0_hist = RSCAN0.THLSTS0.BIT.THLELT;

        /* clear overflow interrupt flags */
        RSCAN.RFSTS0.BIT.RFMLT = 0;
        RSCAN.RFSTS1.BIT.RFMLT = 0;
        RSCAN0.THLSTS0.BIT.THLELT = 0;
        break;
    }
    case CAN_STAT_RXMBX_FULL:
    {
        stat.uint32 = (uint32_t) RSCAN.RMND0.WORD;
        break;
    }
    case CAN_STAT_GLOBAL_ERR:
    {
        stat.uint32 = (uint32_t) RSCAN.GERFLL.BYTE;
        RSCAN.GERFLL.BIT.DEF = 0;       // reset DLC error flag
        break;
    }
    case CAN_STAT_CH_TXMBX_SENT:
    {
        stat.uint32 = (uint32_t) RSCAN0.TMTCSTS.WORD;

        /* reset bits */
        RSCAN0.TMSTS0.BIT.TMTRF = 0;
        RSCAN0.TMSTS1.BIT.TMTRF = 0;
        RSCAN0.TMSTS2.BIT.TMTRF = 0;
        RSCAN0.TMSTS3.BIT.TMTRF = 0;
        break;
    }
    case CAN_STAT_CH_TXMBX_ABORTED:
    {
        stat.uint32 = (uint32_t) RSCAN0.TMTASTS.WORD;

        /* reset bits */
        RSCAN0.TMSTS0.BIT.TMTRF = 0;
        RSCAN0.TMSTS1.BIT.TMTRF = 0;
        RSCAN0.TMSTS2.BIT.TMTRF = 0;
        RSCAN0.TMSTS3.BIT.TMTRF = 0;
        break;
    }
    case CAN_STAT_CH_ERROR:
    {
        stat.uint32 = (uint32_t) RSCAN0.ERFLL.WORD;

        /* reset bits */
        RSCAN0.ERFLL.WORD = 0;
        break;
    }
    }

    *p_err = CAN_SUCCESS;
    return stat.uint32;
}
/******************************************************************************
 End of function R_CAN_GetStatusMask
 *****************************************************************************/


/******************************************************************************
* Function Name: R_CAN_GetCountFIFO
* Description  : This function returns the number of entries used in a FIFO.
* Arguments    : box_id-
*                    FIFO box id
*                p_err-
*                    Pointer to hold error code
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_INVALID_ARG-
*                    An invalid value passed for one of the parameters.
*******************************************************************************/
uint32_t R_CAN_GetCountFIFO(can_box_t  box_id,
                            can_err_t  *p_err)
{

#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    if (((box_id & CAN_BOX_TYPE_MASK) != CAN_FLG_FIFO)
     || (NULL == p_err))
    {
        *p_err = CAN_ERR_INVALID_ARG;
        return 0;
    }
#endif

    *p_err = CAN_SUCCESS;

    if (CAN_BOX_RXFIFO_0 == box_id)
    {
        return RSCAN.RFSTS0.BIT.RFMC;
    }
    else if (CAN_BOX_RXFIFO_1 == box_id)
    {
        return RSCAN.RFSTS1.BIT.RFMC;
    }
    else if (CAN_BOX_TXFIFO == box_id)
    {
        return RSCAN0.CFSTS0.BIT.CFMC;
    }
    else  /* CAN_BOX_HIST_FIFO */
    {
        return RSCAN0.THLSTS0.BIT.THLMC;
    }
}
/******************************************************************************
 End of function R_CAN_GetCountFIFO
 *****************************************************************************/


/******************************************************************************
* Function Name: R_CAN_GetCountErr
* Description  : This function returns the number transmit or receive errors
*                on a channel.
* Arguments    : type-
*                    transmit or receive
*                chan-
*                    channel (always 0)
*                p_err-
*                    pointer to hold error code
* Return Value : CAN_SUCCESS-
*                    Successful
*                CAN_ERR_INVALID_ARG-
*                    An invalid value passed for one of the parameters.
*******************************************************************************/
uint32_t R_CAN_GetCountErr(can_count_t  type,
                           uint8_t      chan,
                           can_err_t    *p_err)
{
#if (CAN_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((type >= CAN_COUNT_END_ENUM)
     || (NULL == p_err))
    {
        *p_err = CAN_ERR_INVALID_ARG;
        return 0;
    }
#endif

    *p_err = CAN_SUCCESS;

    if (CAN_COUNT_RX_ERR == type)
    {
        return RSCAN0.STSH.BIT.REC;
    }
    else /* tx err */
    {
        return RSCAN0.STSH.BIT.TEC;
    }
}
/******************************************************************************
 End of function R_CAN_GetCountErr
 *****************************************************************************/


/******************************************************************************
* Function Name: R_CAN_Close
* Description  : This function
* Arguments    : None
* Return Value : None
*******************************************************************************/
void R_CAN_Close(void)
{
#if (R_BSP_VERSION_MAJOR >= 5) && (R_BSP_VERSION_MINOR >= 30)
bsp_int_ctrl_t int_ctrl;
#endif
    if (TRUE == g_dcb.opened)
    {
        /* enter global reset mode */
        RSCAN.GCTRL.BIT.GMDC = CAN_GLOBAL_MODE_RESET;

        /*
         * NOTE: If you manually clear rule counts anywhere in this function, they
         * will go to zero, but then magically go back to their previous value later.
         * Must clear in AddRxRule().
         */

        /* disable interrupts */
        R_BSP_InterruptRequestDisable(VECT(RSCAN,GLERRINT));
        R_BSP_InterruptRequestDisable(VECT(RSCAN,RXFINT));
        R_BSP_InterruptRequestDisable(VECT(RSCAN,CHERRINT));
        R_BSP_InterruptRequestDisable(VECT(RSCAN,TXINT));

        /* global stop mode */
        RSCAN.GCTRL.BIT.GSLPR = 1;
        while (1 != RSCAN.GCTRL.BIT.GSLPR)
            ;

        /* clear internal structures*/
        memset((void*)&g_dcb, 0, sizeof(g_dcb));
        memset((void*)&g_ccb, 0, sizeof(g_ccb));


        /* Remove clock from the CAN interface */
        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
#if (R_BSP_VERSION_MAJOR >= 5) && (R_BSP_VERSION_MINOR >= 30)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
        MSTP(RSCAN) = 1;
#if (R_BSP_VERSION_MAJOR >= 5) && (R_BSP_VERSION_MINOR >= 30)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
        R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    }
}
/******************************************************************************
 End of function R_CAN_Close
 *****************************************************************************/


/*****************************************************************************
* Function Name: R_CAN_GetVersion
* Description  : Returns the version of this module. The version number is
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
******************************************************************************/
uint32_t  R_CAN_GetVersion(void)
{
    uint32_t const version = (CAN_VERSION_MAJOR << 16) | CAN_VERSION_MINOR;

    return version;
}
/******************************************************************************
 End of function R_CAN_GetVersion
 *****************************************************************************/


/******************************************************************************
* Function Name: can_global_err_isr
* Description  : This function is executed when the INTCANGERR interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(can_global_err_isr,VECT(RSCAN,GLERRINT))
R_BSP_ATTRIB_STATIC_INTERRUPT void can_global_err_isr(void)
{

    g_dcb.p_callback(CAN_EVT_GLOBAL_ERR, NULL);

    /* reset interrupt flags */
    RSCAN0.THLSTS0.BIT.THLELT = 0;      // clear HIST FIFO overflow flags (for THLES error)
    RSCAN.RFSTS0.BIT.RFMLT = 0;         // clear RXFIFO overflow flags (for MES error)
    RSCAN.RFSTS1.BIT.RFMLT = 0;
    RSCAN.GERFLL.BIT.DEF = 0;           // clear dlc error flag (for DEF error)
}
/******************************************************************************
 End of function can_global_err_isr
 *****************************************************************************/


/******************************************************************************
* Function Name: can_rxfifo_isr
* Description  : This function is executed when the INTCANGRECC interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(can_rxfifo_isr, VECT(RSCAN,RXFINT))
R_BSP_ATTRIB_STATIC_INTERRUPT void can_rxfifo_isr(void)
{

    g_dcb.p_callback(CAN_EVT_RXFIFO_THRESHOLD, NULL);

    /* reset interrupt flags */
    RSCAN.RFSTS0.BIT.RFIF = 0;
    RSCAN.RFSTS1.BIT.RFIF = 0;
}
/******************************************************************************
 End of function can_rxfifo_isr
 *****************************************************************************/


/******************************************************************************
* Function Name: can_ch0_error_isr
* Description  : This function is executed when the INTCAN0ERR interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(can_ch0_error_isr, VECT(RSCAN,CHERRINT))
R_BSP_ATTRIB_STATIC_INTERRUPT void can_ch0_error_isr(void)
{

    g_ccb.p_chcallback(CAN_CH0, CAN_EVT_CHANNEL_ERR, NULL);

    /* reset interrupt flags */
    RSCAN0.ERFLL.WORD = 0;

}
/******************************************************************************
 End of function can_ch0_error_isr
 *****************************************************************************/


/******************************************************************************
* Function Name: can_ch0_tx_isr
* Description  : This function is executed when the INTCAN0TRX interrupt is
*                received.
* Arguments    : none
* Return Value : none
******************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(can_ch0_tx_isr, VECT(RSCAN,TXINT))
R_BSP_ATTRIB_STATIC_INTERRUPT void can_ch0_tx_isr(void)
{

    g_ccb.p_chcallback(CAN_CH0, CAN_EVT_TRANSMIT, NULL);

    /* reset mailbox transmit complete or abort flags */
    RSCAN0.TMSTS0.BIT.TMTRF = 0;
    RSCAN0.TMSTS1.BIT.TMTRF = 0;
    RSCAN0.TMSTS2.BIT.TMTRF = 0;
    RSCAN0.TMSTS3.BIT.TMTRF = 0;

    /* reset TXFIFO threshold flags */
    RSCAN0.CFSTS0.BIT.CFTXIF = 0;   // TODO: Need 16-bit write?

    /* reset HIST FIFO threshold flag */
    RSCAN0.THLSTS0.BIT.THLIF = 0;
}
/******************************************************************************
 End of function can_ch0_tx_isr
 *****************************************************************************/
