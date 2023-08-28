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
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/**********************************************************************************************************************
* File Name    : r_rsci_rx.c
* Description  : Functions for using RSCI on RX devices.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           31.03.2021 1.00    Initial Release
*                              Supported for RX671.
*           03.12.2021 2.00    Updated new features in Asynchronous mode
*                              and added support for Manchester mode.
*           31.03.2022 2.10    Supported for RX660.
*           15.08.2022 2.30    Supported for RX26T.
*                              Fixed to comply with GSCE Coding Standards Rev.6.5.0.
*           30.06.2023 2.40    Fixed to comply with GSCE Coding Standards Rev.6.5.0.
***********************************************************************************************************************/

/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "platform.h"

/* Defines for RSCI support */
#include "r_rsci_rx_private.h"

/* Include specifics for chosen MCU.  */
#include "r_rsci_rx_platform.h"


#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
#include "r_byteq_if.h"
#endif


/*****************************************************************************
Typedef definitions
******************************************************************************/

/*****************************************************************************
Macro definitions
******************************************************************************/

/*****************************************************************************
Private global variables and functions
******************************************************************************/
#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
static rsci_err_t rsci_init_queues (uint8_t const chan);

static rsci_err_t rsci_send_async_data (rsci_hdl_t const hdl,
                                        uint8_t * p_src,
                                        uint16_t const  length);

static byteq_err_t rsci_put_byte (rsci_hdl_t const    hdl,
                                uint8_t const      byte);

static void rsci_transfer (rsci_hdl_t const hdl);

static rsci_err_t rsci_receive_async_data (rsci_hdl_t const hdl,
                                        uint8_t         * p_dst,
                                        uint16_t const  length);
#endif

#if (RSCI_CFG_ASYNC_INCLUDED)
static rsci_err_t rsci_init_async (rsci_hdl_t const     hdl,
                                rsci_uart_t * const  p_cfg,
                                uint8_t * const     p_priority);
#if RSCI_CFG_FIFO_INCLUDED
static void rsci_fifo_transfer (rsci_hdl_t const hdl);
#endif
#endif

#if (RSCI_CFG_MANC_INCLUDED)
static rsci_err_t rsci_init_manc (rsci_hdl_t const     hdl,
                                rsci_manc_t * const  p_cfg,
                                uint8_t * const     p_priority);
#endif

#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
static rsci_err_t rsci_init_sync (rsci_hdl_t const         hdl,
                                rsci_sync_sspi_t * const p_cfg,
                                uint8_t * const         p_priority);

static rsci_err_t rsci_send_sync_data (rsci_hdl_t const hdl,
                                    uint8_t         * p_src,
                                    uint8_t         * p_dst,
                                    uint16_t const  length,
                                    bool            save_rx_data);

static rsci_err_t rsci_receive_sync_data (rsci_hdl_t const hdl,
                                        uint8_t         * p_dst,
                                        uint16_t const  length);
#if RSCI_CFG_FIFO_INCLUDED
static void rsci_fifo_receive_sync (rsci_hdl_t const hdl);
#endif

#endif /* #if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED) */

static void power_on (rsci_hdl_t const hdl);
static void power_off (rsci_hdl_t const hdl);

#if RSCI_CFG_FIFO_INCLUDED
static rsci_err_t rsci_init_fifo (rsci_hdl_t const hdl);

static void rsci_fifo_receive (rsci_hdl_t const hdl);

static void rsci_fifo_error (rsci_hdl_t const hdl);
#endif

static void rsci_receive (rsci_hdl_t const hdl);
static void rsci_error (rsci_hdl_t const hdl);

#if RSCI_CFG_DATA_MATCH_INCLUDED
static void rsci_receive_data_match(rsci_hdl_t const hdl);
#endif

/* queue buffers */
#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)

#if RSCI_CFG_CH8_INCLUDED
static uint8_t  ch8_tx_buf[RSCI_CFG_CH8_TX_BUFSIZ];
static uint8_t  ch8_rx_buf[RSCI_CFG_CH8_RX_BUFSIZ];
#endif

#if RSCI_CFG_CH9_INCLUDED
static uint8_t  ch9_tx_buf[RSCI_CFG_CH9_TX_BUFSIZ];
static uint8_t  ch9_rx_buf[RSCI_CFG_CH9_RX_BUFSIZ];
#endif

#if RSCI_CFG_CH10_INCLUDED
static uint8_t  ch10_tx_buf[RSCI_CFG_CH10_TX_BUFSIZ];
static uint8_t  ch10_rx_buf[RSCI_CFG_CH10_RX_BUFSIZ];
#endif

#if RSCI_CFG_CH11_INCLUDED
static uint8_t  ch11_tx_buf[RSCI_CFG_CH11_TX_BUFSIZ];
static uint8_t  ch11_rx_buf[RSCI_CFG_CH11_RX_BUFSIZ];
#endif

#endif /* #if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED) */

extern const rsci_hdl_t g_rsci_handles[RSCI_NUM_CH];


/***********************************************************************************************************************
* Function Name: R_RSCI_Open
********************************************************************************************************************//**
* @brief This function applies power to the RSCI channel, initializes the associated registers, enables interrupts, and
* provides the channel handle for use with other API functions. This function must be called before calling any
* other API functions
* @param[in]    chan  Channel to initialize.
*
* @param[in]    mode  Operational mode (see enumeration below)
* @code
typedef enum e_rsci_mode     // RSCI operational modes
{
    RSCI_MODE_OFF=0,         // channel not in use
    RSCI_MODE_ASYNC,         // Asynchronous
    RSCI_MODE_SSPI,          // Simple SPI
    RSCI_MODE_SYNC,          // Synchronous
    RSCI_MODE_MANC,          // Manchester
    RSCI_MODE_MAX            // End of modes currently supported
} rsci_mode_t;
* @endcode
* @param[in]    p_cfg  Pointer to configuration union, structure elements (see below) are specific to mode
* @code
typedef union
{
    rsci_uart_t      async;
    rsci_sync_sspi_t sync;
    rsci_sync_sspi_t sspi;
    rsci_manc_t      manc;
} rsci_cfg_t;
* @endcode
*
* @param[in]    p_callback Pointer to function called from interrupt when an RXI or receiver error is detected or
* for transmit end (TEI) condition.\n
* Refer to 2.12, Callback Function for details.
* @param[in]    p_hdl  Pointer to a handle for channel (value set here)
* Confirm the return value from R_RSCI_Open is “RSCI_SUCCESS” and then set the first parameter for the
* other APIs except R_RSCI_GetVersion(). Refer to 2.10, Parameters.
*
*
* @retval   RSCI_SUCCESS  Successful; channel initialized
*
* @retval   RSCI_ERR_BAD_CHAN  Channel number is invalid for part
*
* @retval   RSCI_ERR_OMITTED_CHAN  Corresponding RSCI_CHx_INCLUDED is invalid (0)
*
* @retval   RSCI_ERR_CH_NOT_CLOSED  Channel currently in operation; Perform R_RSCI_Close() first
*
* @retval   RSCI_ERR_BAD_MODE  Mode specified not currently supported
*
* @retval   RSCI_ERR_NULL_PTR  p_cfg pointer is NULL
*
* @retval   RSCI_ERR_INVALID_ARG  An element of the p_cfg structure contains an invalid value.
*
* @retval   RSCI_ERR_QUEUE_UNAVAILABLE  Cannot open transmit or receive queue or both (Asynchronous or Manchester mode).
* @details  Initializes an RSCI channel for a particular mode and provides a Handle in *p_hdl for use with other API
* functions. RXI and ERI interrupts are enabled in all modes. TXI is enabled in Asynchronous or Manchester mode
* @note  The driver calculates the optimum values for SCR2.BRR, SCR2.ABCS, and SCR2.CKS using BSP_PCLKA_HZ and
* BSP_PCLKB_HZ as defined in mcu_info.h of the board support package. This however does not guarantee
* a low bit error rate for all peripheral clock/baud rate combinations.
* If an external clock is used in Asynchronous mode, the pin direction must be selected before calling the
* R_RSCI_Open() function, and the pin function and mode must be selected after calling the R_RSCI_Open()
* function. See Section 3. R_RSCI_Open() in the application note for details.
*/
rsci_err_t R_RSCI_Open(uint8_t const      chan,
                        rsci_mode_t const   mode,
                        rsci_cfg_t * const  p_cfg,
                        void               (* const p_callback)(void *p_args),
                        rsci_hdl_t * const  p_hdl)
{
    rsci_err_t   err = RSCI_SUCCESS;
    uint8_t     priority = 1;

    /* CHECK ARGUMENTS */
#if RSCI_CFG_PARAM_CHECKING_ENABLE
    err = rsci_mcu_param_check(chan);
    if (RSCI_SUCCESS != err)
    {
        return err;
    }

    /* Check argument g_rsci_handles */
    if ((NULL == g_rsci_handles[chan]) || (FIT_NO_PTR == g_rsci_handles[chan]))
    {
        return RSCI_ERR_OMITTED_CHAN;
    }
    if (RSCI_MODE_OFF != g_rsci_handles[chan]->mode)
    {
        return RSCI_ERR_CH_NOT_CLOSED;
    }
    if ((RSCI_MODE_OFF == mode) || (RSCI_MODE_MAX <= mode))
    {
        return RSCI_ERR_BAD_MODE;
    }

    /* Check argument p_cfg, p_hdl */
    if (((NULL == p_cfg) || (NULL == p_hdl)) || ((FIT_NO_PTR == p_cfg) || (FIT_NO_PTR == p_hdl)))
    {
        return RSCI_ERR_NULL_PTR;
    }
#endif

    /* INITIALIZE MODE SPECIFIC FEATURES */
    g_rsci_handles[chan]->mode = mode;

    /* APPLY POWER TO CHANNEL */
    power_on(g_rsci_handles[chan]);

    /* INITIALIZE REGISTER */
    rsci_init_register(g_rsci_handles[chan]);

    /* INITIALIZE MODE SPECIFIC FEATURES */
    if (RSCI_MODE_ASYNC == mode)
    {
#if (RSCI_CFG_ASYNC_INCLUDED)
        /* Casting rsci_cfg_t type to rsci_uart_t type is valid */
        err = rsci_init_async(g_rsci_handles[chan], (rsci_uart_t *)p_cfg, &priority);
#endif
    }
    else if (RSCI_MODE_MANC == mode)
    {
#if (RSCI_CFG_MANC_INCLUDED)
        /* Casting rsci_cfg_t type to rsci_manc_t type is valid */
        err = rsci_init_manc(g_rsci_handles[chan], (rsci_manc_t *)p_cfg, &priority);
#endif
    }
    else
    {
#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
        /* Casting sci_cfg_t type to rsci_sync_sspi_t type is valid */
        err = rsci_init_sync(g_rsci_handles[chan], (rsci_sync_sspi_t *)p_cfg, &priority);
#endif
    }

    if (RSCI_SUCCESS != err)
    {
        g_rsci_handles[chan]->mode = RSCI_MODE_OFF;
        return err;
    }
    g_rsci_handles[chan]->callback = p_callback;

    /* INITIALIZE TX AND RX QUEUES ASYNC/MANC */
#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
    if ((RSCI_MODE_ASYNC == mode) || (RSCI_MODE_MANC == mode))
    {
        err = rsci_init_queues(chan);
        if (RSCI_SUCCESS != err)
        {
            g_rsci_handles[chan]->mode = RSCI_MODE_OFF;
            return err;
        }
    }
#endif

#if RSCI_CFG_FIFO_INCLUDED
    if ((true == g_rsci_handles[chan]->fifo_ctrl) && (RSCI_MODE_MANC != mode))
    {
        /* INITIALIZE TX AND RX FIFO */
        err = rsci_init_fifo(g_rsci_handles[chan]);
        if (RSCI_SUCCESS != err)
        {
#if (RSCI_CFG_ASYNC_INCLUDED)
            /* DE-INITIALIZE TX AND RX QUEUES */
            if (RSCI_MODE_ASYNC == mode)
            {
                R_BYTEQ_Close(g_rsci_handles[chan]->u_tx_data.que);
                R_BYTEQ_Close(g_rsci_handles[chan]->u_rx_data.que);
            }
#endif
            g_rsci_handles[chan]->mode = RSCI_MODE_OFF;
            return err;
        }
    }
#endif

    /* ENABLE INTERRUPTS */
    rsci_initialize_ints(g_rsci_handles[chan], priority);

    /* FINISH */
    *p_hdl = g_rsci_handles[chan];

    return RSCI_SUCCESS;
}  /* End of function R_RSCI_Open() */

/*****************************************************************************
* Function Name: power_on
* Description  : This function provides power to the channel referenced by
*                the handle by taking it out of the module stop state.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
static void power_on(rsci_hdl_t const hdl)
{
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif

    (*hdl->rom->mstp) &= (~hdl->rom->stop_mask);
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    return;
}  /* End of function power_on() */

/*****************************************************************************
* Function Name: power_off
* Description  : This function removes power to the channel referenced by
*                handle by putting it into the module stop state.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
static void power_off(rsci_hdl_t const hdl)
{
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif

    (*hdl->rom->mstp) |= (hdl->rom->stop_mask);
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    return;
}  /* End of function power_off() */

#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
/*****************************************************************************
* Function Name: rsci_init_queues
* Description  : This function attaches transmit and receive queues to the
*                channel.
*
* Arguments    : chan -
*                    channel (ptr to chan control block)
* Return Value : RSCI_SUCCESS -
*                    channel initialized successfully
*                RSCI_ERR_QUEUE_UNAVAILABLE -
*                    no queue control blocks available
******************************************************************************/
static rsci_err_t rsci_init_queues(uint8_t const chan)
{
    byteq_err_t q_err1 = BYTEQ_ERR_INVALID_ARG;
    byteq_err_t q_err2 = BYTEQ_ERR_INVALID_ARG;
    rsci_err_t err = RSCI_SUCCESS;

    /* channel number verified as legal prior to calling this function */
    switch (chan)
    {
#if RSCI_CFG_CH8_INCLUDED
        case (RSCI_CH8):
        {
            q_err1 = R_BYTEQ_Open(ch8_tx_buf, RSCI_CFG_CH8_TX_BUFSIZ, &g_rsci_handles[RSCI_CH8]->u_tx_data.que);
            q_err2 = R_BYTEQ_Open(ch8_rx_buf, RSCI_CFG_CH8_RX_BUFSIZ, &g_rsci_handles[RSCI_CH8]->u_rx_data.que);
            break;
        }
#endif
#if RSCI_CFG_CH9_INCLUDED
        case (RSCI_CH9):
        {
            q_err1 = R_BYTEQ_Open(ch9_tx_buf, RSCI_CFG_CH9_TX_BUFSIZ, &g_rsci_handles[RSCI_CH9]->u_tx_data.que);
            q_err2 = R_BYTEQ_Open(ch9_rx_buf, RSCI_CFG_CH9_RX_BUFSIZ, &g_rsci_handles[RSCI_CH9]->u_rx_data.que);
            break;
        }
#endif
#if RSCI_CFG_CH10_INCLUDED
        case (RSCI_CH10):
        {
            q_err1 = R_BYTEQ_Open(ch10_tx_buf, RSCI_CFG_CH10_TX_BUFSIZ, &g_rsci_handles[RSCI_CH10]->u_tx_data.que);
            q_err2 = R_BYTEQ_Open(ch10_rx_buf, RSCI_CFG_CH10_RX_BUFSIZ, &g_rsci_handles[RSCI_CH10]->u_rx_data.que);
            break;
        }
#endif
#if RSCI_CFG_CH11_INCLUDED
        case (RSCI_CH11):
        {
            q_err1 = R_BYTEQ_Open(ch11_tx_buf, RSCI_CFG_CH11_TX_BUFSIZ, &g_rsci_handles[RSCI_CH11]->u_tx_data.que);
            q_err2 = R_BYTEQ_Open(ch11_rx_buf, RSCI_CFG_CH11_RX_BUFSIZ, &g_rsci_handles[RSCI_CH11]->u_rx_data.que);
            break;
        }
#endif
        default:
        {
            err = RSCI_ERR_QUEUE_UNAVAILABLE;
            break;
        }
    }

    if ((BYTEQ_SUCCESS != q_err1) || (BYTEQ_SUCCESS != q_err2))
    {
        err = RSCI_ERR_QUEUE_UNAVAILABLE;
    }
    return err;
}  /* End of function rsci_init_queues() */
#endif /* End of RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED */

#if RSCI_CFG_FIFO_INCLUDED
/*****************************************************************************
* Function Name: rsci_init_fifo
* Description  : This function the setting of the FIFO mode, reset of the
*                TX/RX FIFO, and the threshold setting of the TX/RX FIFO.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
* Return Value : RSCI_SUCCESS -
*                    fifo initialized successfully
*                RSCI_ERR_INVALID_ARG -
*                    element of hdl contains illegal value
******************************************************************************/
static rsci_err_t rsci_init_fifo(rsci_hdl_t const hdl)
{
    /* CHECK ARGUMENTS */
#if RSCI_CFG_PARAM_CHECKING_ENABLE
    if (hdl->tx_dflt_thresh > 31)
    {
        return RSCI_ERR_INVALID_ARG;
    }
    if ((hdl->rx_dflt_thresh < 1) || (hdl->rx_dflt_thresh > 31))
    {
        return RSCI_ERR_INVALID_ARG;
    }
#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
    if (hdl->tx_dflt_thresh != hdl->rx_dflt_thresh)
    {
        return RSCI_ERR_INVALID_ARG;
    }
#endif
#endif

    /* FIFO Mode Select (1:FIFO mode) */
    hdl->rom->regs->SCR3.BIT.FM = 0x01;

    /* reset TX/RX FIFO */
    hdl->rom->regs->FCR.BIT.TFRST = 0x01;
    hdl->rom->regs->FCR.BIT.RFRST = 0x01;
    
    /* Clear SSR.BIT.RDRF flag to 0. */
    hdl->rom->regs->SSCR.BIT.RDRFC  = 1;

    /* set TX/RX FIFO threshold initial value */
    hdl->rom->regs->FCR.BIT.TTRG = hdl->tx_dflt_thresh;
    hdl->rom->regs->FCR.BIT.RTRG = hdl->rx_dflt_thresh;

    return RSCI_SUCCESS;
}  /* End of function rsci_init_fifo() */
#endif /* End of RSCI_CFG_FIFO_INCLUDED */

#if (RSCI_CFG_ASYNC_INCLUDED)
/*****************************************************************************
* Function Name: rsci_init_async
* Description  : This function initializes the control block and UART 
*                registers for an RSCI channel.
*
* NOTE: p_cfg is checked to be non-NULL prior to this function.
*       The TE and RE bits in SCR0 must be 0 prior to calling this function.
*
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
*                p_cfg -
*                    ptr to Uart configuration argument structure
*                p_priority -
*                    pointer to location to load interrupt priority into
* Return Value : RSCI_SUCCESS -
*                    channel initialized successfully
*                RSCI_ERR_INVALID_ARG -
*                    element of p_cfg contains illegal value
******************************************************************************/
static rsci_err_t rsci_init_async(rsci_hdl_t const      hdl,
                                rsci_uart_t * const   p_cfg,
                                uint8_t * const      p_priority)
{
    rsci_err_t   err=RSCI_SUCCESS;

    int32_t     bit_err;

    /* Check arguments */    
#if RSCI_CFG_PARAM_CHECKING_ENABLE
    if (((RSCI_DATA_8BIT != p_cfg->data_size) && (RSCI_DATA_7BIT != p_cfg->data_size))
            || ((RSCI_EVEN_PARITY != p_cfg->parity_type) && (RSCI_ODD_PARITY != p_cfg->parity_type)
                    && (RSCI_NONE_PARITY != p_cfg->parity_type))
            || ((RSCI_STOPBITS_1 != p_cfg->stop_bits) && (RSCI_STOPBITS_2 != p_cfg->stop_bits))
            || ((p_cfg->int_priority < (BSP_MCU_IPL_MIN + 1)) || (p_cfg->int_priority > BSP_MCU_IPL_MAX)))
    {
        return RSCI_ERR_INVALID_ARG;
    }

    if (RSCI_CLK_INT == p_cfg->clk_src)
    {
        if (0 == p_cfg->baud_rate)
        {
            return RSCI_ERR_INVALID_ARG;
        }
    }
    else if ((RSCI_CLK_EXT8X != p_cfg->clk_src) && (RSCI_CLK_EXT16X != p_cfg->clk_src))
    {
        return RSCI_ERR_INVALID_ARG;
    }
    else
    {
        /* Do Nothing */
        R_BSP_NOP();
    }
#endif /* End of RSCI_CFG_PARAM_CHECKING_ENABLE */


    /* Initialize channel control block flags */
    hdl->tx_idle = true;

        
    /* Configure for asynchronous mode, single processor, and user settings */
    /* Configure Parity Mode */
    if (RSCI_NONE_PARITY == p_cfg->parity_type)
    {
        hdl->rom->regs->SCR1.BIT.PE = 0;
    }
    else
    {
        hdl->rom->regs->SCR1.BIT.PE = 1;
		hdl->rom->regs->SCR1.BIT.PM = (uint8_t)((p_cfg->parity_type));
    }

    /* Configure Character Length, Stop Bit Length */
    hdl->rom->regs->SCR3.BIT.CHR  = (uint8_t)((p_cfg->data_size));
    hdl->rom->regs->SCR3.BIT.STOP = (uint8_t)((p_cfg->stop_bits));

    /* Configure data inversion */
    hdl->rom->regs->SCR3.BIT.DINV = (uint8_t)((true == p_cfg->invert_data) ? 1 : 0);

    /* Configure bit order */
    if (RSCI_DATA_7BIT != p_cfg->data_size)
    {
        hdl->rom->regs->SCR3.BIT.DDIR = (uint8_t)((true == p_cfg->msb_first) ? 0 : 1);
    }
    else
    {
        hdl->rom->regs->SCR3.BIT.DDIR = 1; /* LSB first is fixed in 7-bit data length*/
    }

    /* SETUP CLOCK FOR BAUD RATE */
    if (RSCI_CLK_INT == p_cfg->clk_src)
    {
        /* Use internal clock for baud rate */
        bit_err = rsci_init_bit_rate(hdl, hdl->pclk_speed, p_cfg->baud_rate);
        if (1000 == bit_err)
        {
            err = RSCI_ERR_INVALID_ARG;          // impossible baud rate; 100% error
        }
        else
        {
            hdl->baud_rate = p_cfg->baud_rate;   // save baud rate for break generation
        }
#if RSCI_CFG_RX_DATA_SAMPLING_TIMING_INCLUDED
        hdl->rom->regs->SCR4.BIT.RTADJ = 1;      /* Enable receive data sampling timing adjust*/
#endif

#if RSCI_CFG_TX_SIGNAL_TRANSITION_TIMING_INCLUDED
        hdl->rom->regs->SCR4.BIT.TTADJ = 1;      /* Enable transmit signal transition timing adjust*/
#endif
    }
    else
    {
        /* Use external clock for baud rate */
        hdl->rom->regs->SCR3.BIT.CKE = 0x02;
        hdl->rom->regs->SCR2.BIT.ABCS = (RSCI_CLK_EXT8X == p_cfg->clk_src) ? 1 : 0;

    }

    *p_priority = p_cfg->int_priority;
    return err;
}  /* End of function rsci_init_async() */
#endif /* End of RSCI_CFG_ASYNC_INCLUDED */

#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
/*****************************************************************************
* Function Name: rsci_init_sync
* Description  : This function initializes the control block and SYNC/SSPI
*                registers for an RSCI channel.
*
* NOTE: p_cfg is checked to be non-NULL prior to this function.
*       The TE and RE bits in SCR0 must be 0 prior to calling this function.
*
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                p_cfg -
*                    ptr to SSPI configuration argument structure
*                p_priority -
*                    pointer to location to load interrupt priority into
* Return Value : RSCI_SUCCESS -
*                    channel initialized successfully
*                RSCI_ERR_INVALID_ARG -
*                    element of p_cfg contains illegal value
******************************************************************************/
static rsci_err_t rsci_init_sync(rsci_hdl_t const         hdl,
                                rsci_sync_sspi_t * const p_cfg,
                                uint8_t * const         p_priority)
{
    rsci_err_t   err = RSCI_SUCCESS;
    int32_t     bit_err;


    /* Check arguments */

#if RSCI_CFG_PARAM_CHECKING_ENABLE
    if ((RSCI_MODE_SSPI == hdl->mode) && (RSCI_SPI_MODE_0 != p_cfg->spi_mode) && (RSCI_SPI_MODE_1 != p_cfg->spi_mode)
            && (RSCI_SPI_MODE_2 != p_cfg->spi_mode) && (RSCI_SPI_MODE_3 != p_cfg->spi_mode))
    {
        return RSCI_ERR_INVALID_ARG;
    }
    else if ((RSCI_MODE_SYNC == hdl->mode) && (RSCI_SPI_MODE_OFF != p_cfg->spi_mode))
    {
        return RSCI_ERR_INVALID_ARG;
    }
    else
    {
        /* Do Nothing */
        R_BSP_NOP();
    }

    if (0 == p_cfg->bit_rate)
    {
        return RSCI_ERR_INVALID_ARG;
    }

    if ((0 == p_cfg->int_priority) || (p_cfg->int_priority > BSP_MCU_IPL_MAX))
    {
        return RSCI_ERR_INVALID_ARG;
    }
#endif

    /* Initialize channel control block flags */
    hdl->tx_idle = true;
    hdl->tx_dummy = false;

    /* Configure SCR3 for SSPI/SYNC mode */

    /* Configure SPI register for clock polarity/phase and single master */
    if (RSCI_MODE_SSPI == hdl->mode)
    {
        hdl->rom->regs->SCR3.BIT.MOD = 0x03;
        hdl->rom->regs->SCR3.LONG   &= 0xFFFFFFFE; /* clear previous spi mode */
        hdl->rom->regs->SCR3.LONG   |= p_cfg->spi_mode;
    }
    else    /* synchronous operation */
    {
        hdl->rom->regs->SCR3.BIT.MOD = 0x02;
    }

    /* Configure data inversion */
    hdl->rom->regs->SCR3.BIT.DINV = (uint8_t)((true == p_cfg->invert_data) ? 1 : 0);

    /* Configure bit order */
    hdl->rom->regs->SCR3.BIT.DDIR = (uint8_t)((true == p_cfg->msb_first) ? 0 : 1);


    /* SETUP CLOCK FOR BIT RATE */
    /* Use internal clock for bit rate (master) */
    bit_err = rsci_init_bit_rate(hdl, hdl->pclk_speed, p_cfg->bit_rate);
    if (1000 == bit_err)
    {
        err = RSCI_ERR_INVALID_ARG;      /* impossible bit rate; 100% error */
    }

    *p_priority = p_cfg->int_priority;
    return err;
} /* End of function rsci_init_sync() */
#endif /* End of RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED */

#if (RSCI_CFG_MANC_INCLUDED)
/*****************************************************************************
* Function Name: rsci_init_manc
* Description  : This function initializes the control block and manchester
*                registers for an RSCI channel.
*
* NOTE: p_cfg is checked to be non-NULL prior to this function.
*       The TE and RE bits in SCR0 must be 0 prior to calling this function.
*
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                p_cfg -
*                    ptr to Manchester configuration argument structure
*                p_priority -
*                    pointer to location to load interrupt priority into
* Return Value : RSCI_SUCCESS -
*                    channel initialized successfully
*                RSCI_ERR_INVALID_ARG -
*                    element of p_cfg contains illegal value
******************************************************************************/
static rsci_err_t rsci_init_manc(rsci_hdl_t const      hdl,
                                rsci_manc_t * const   p_cfg,
                                uint8_t * const      p_priority)
{
    rsci_err_t   err=RSCI_SUCCESS;

    int32_t     bit_err;
    /* Check arguments */

#if RSCI_CFG_PARAM_CHECKING_ENABLE
    if (((RSCI_DATA_8BIT != p_cfg->data_size) && (RSCI_DATA_7BIT != p_cfg->data_size))
            || ((RSCI_EVEN_PARITY != p_cfg->parity_type) && (RSCI_ODD_PARITY != p_cfg->parity_type)
                    && (RSCI_NONE_PARITY != p_cfg->parity_type))
            || ((RSCI_STOPBITS_1 != p_cfg->stop_bits) && (RSCI_STOPBITS_2 != p_cfg->stop_bits))
            || ((RSCI_STARTBITS_1 != p_cfg->start_bits) && (RSCI_STARTBITS_3 != p_cfg->start_bits))
            || ((p_cfg->int_priority < (BSP_MCU_IPL_MIN + 1)) || (p_cfg->int_priority > BSP_MCU_IPL_MAX)))
    {
        return RSCI_ERR_INVALID_ARG;
    }
    if ((15 < hdl->rx_preface_length) || (15 < hdl->tx_preface_length))
    {
        return RSCI_ERR_INVALID_ARG;
    }
    if ((3 < hdl->rx_preface_pattern) || (3 < hdl->tx_preface_pattern))
    {
        return RSCI_ERR_INVALID_ARG;
    }
#endif /* End of RSCI_CFG_PARAM_CHECKING_ENABLE */


    /* Initialize channel control block flags */
    hdl->tx_idle = true;


    /* Configure for Manchester mode, single processor, and user settings */
    /* Configure Parity Mode */
    if (RSCI_NONE_PARITY == p_cfg->parity_type)
    {
        hdl->rom->regs->SCR1.BIT.PE = 0;
    }
    else
    {
        hdl->rom->regs->SCR1.BIT.PE = 1;
		hdl->rom->regs->SCR1.BIT.PM = (uint8_t)((p_cfg->parity_type));
    }

    /* Configure Character Length, Stop Bit Length, Start bit Length */
    hdl->rom->regs->SCR3.BIT.CHR = (uint8_t)((p_cfg->data_size));
    hdl->rom->regs->SCR3.BIT.STOP = (uint8_t)((p_cfg->stop_bits));
    hdl->rom->regs->MMCR.BIT.SBLEN = (uint8_t)((p_cfg->start_bits));

    /* Configure SCR3 for Manchester mode */
    hdl->rom->regs->SCR3.BIT.MOD = 0x05;

    /* Configure data inversion */
    hdl->rom->regs->SCR3.BIT.DINV = (uint8_t)((true == p_cfg->invert_data) ? 1 : 0);

    /* Configure bit order */
    if (RSCI_DATA_7BIT != p_cfg->data_size)
    {
        hdl->rom->regs->SCR3.BIT.DDIR = (uint8_t)((true == p_cfg->msb_first) ? 0 : 1);
    }
    else
    {
        hdl->rom->regs->SCR3.BIT.DDIR = 1; /* LSB first is fixed in 7-bit data length*/
    }

    /* Configure Decoding, Encoding, TX/RX Preface Length, TX/RX Preface Pattern */
    hdl->rom->regs->MMCR.BIT.DECS = hdl->rx_decoding_pol;
    hdl->rom->regs->MMCR.BIT.ENCS = hdl->tx_encoding_pol;
    hdl->rom->regs->MMCR.BIT.RPLEN = hdl->rx_preface_length;
    hdl->rom->regs->MMCR.BIT.TPLEN = hdl->tx_preface_length;
    hdl->rom->regs->MMCR.BIT.RPPAT = hdl->rx_preface_pattern;
    hdl->rom->regs->MMCR.BIT.TPPAT = hdl->tx_preface_pattern;

    /* SETUP CLOCK FOR BAUD RATE */
    /* Only use internal clock for baud rate in Manchester */
    bit_err = rsci_init_bit_rate(hdl, hdl->pclk_speed, p_cfg->baud_rate);
    if (1000 == bit_err)
    {
        err = RSCI_ERR_INVALID_ARG;          // impossible baud rate; 100% error
    }
    else
    {
        hdl->baud_rate = p_cfg->baud_rate;   // save baud rate for break generation
    }

    /* SETUP EXTRA ERROR INTERRUPT FOR MANCHESTER */
    hdl->rom->regs->MMCR.LONG |= RSCI_EN_MMCR_ERR_MASK;    // enable PFERIE, SYERIE, SBERIE

    *p_priority = p_cfg->int_priority;
    return err;
}  /* End of function rsci_init_async() */
#endif /* End of RSCI_CFG_MANC_INCLUDED */


/***********************************************************************************************************************
* Function Name: R_RSCI_Send
********************************************************************************************************************//**
* @brief  Initiates transmit if transmitter is not in use. Queues data for later transmit when in Asynchronous or Manchester mode.
*
* @param[in]    hdl  Handle for channel. Set hdl when R_RSCI_Open() is successfully processed.
*
* @param[in]    p_src  Pointer to data to transmit
*
* @param[in]    length  Number of bytes to send
*
* @retval   RSCI_SUCCESS  Transmit initiated or loaded into queue (Asynchronous/Manchester)
*
* @retval   RSCI_ERR_NULL_PTR  hdl value is NULL
*
* @retval   RSCI_ERR_BAD_MODE  Mode specified not currently supported
*
* @retval   RSCI_ERR_INSUFFICIENT_SPACE  Insufficient space in queue to load all data (Asynchronous/Manchester)
*
* @retval   RSCI_ERR_XCVR_BUSY  Channel currently busy (SSPI/Synchronous)
*
*
* @details  In asynchronous or manchester mode, this function places data into a transmit queue if the transmitter for the RSCI channel
* referenced by the handle is not in use. In SSPI and Synchronous modes, no data is queued and transmission begins immediately
* if the transceiver is not already in use. All transmissions are handled at the interrupt level.\n
* Note that the toggling of Slave Select lines when in SSPI mode is not handled by this driver. The Slave Select line
* for the target device must be enabled prior to calling this function.
* Also, toggling of the CTS/RTS pin in Synchronous/Asynchronous/Manchester mode is not handled by this driver.
* @note None
*/
rsci_err_t R_RSCI_Send(rsci_hdl_t const    hdl,
                        uint8_t            *p_src,
                        uint16_t const     length)
{
    rsci_err_t   err=RSCI_SUCCESS;

    /* Check arguments */

#if RSCI_CFG_PARAM_CHECKING_ENABLE
    /* Check argument hdl, p_src */
    if (((NULL == hdl)   || (FIT_NO_PTR == hdl)) || ((NULL == p_src) || (FIT_NO_PTR == p_src)))
    {
        return RSCI_ERR_NULL_PTR;
    }
    if ((RSCI_MODE_OFF == hdl->mode) || (RSCI_MODE_MAX <= hdl->mode))
    {
        return RSCI_ERR_BAD_MODE;
    }
    if (0 == length)
    {
        return RSCI_ERR_INVALID_ARG;
    }
#endif

    if ((RSCI_MODE_ASYNC == hdl->mode) || (RSCI_MODE_MANC == hdl->mode))
    {
        /* ASYNC or MANC */
#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
        err = rsci_send_async_data(hdl, p_src, length);
#endif
    }
    else
    {
        /* SSPI or SYNC */
#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
        err = rsci_send_sync_data(hdl, p_src, NULL, length, false);
#endif
    }

    return err;
} /* End of function R_RSCI_Send() */


#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
/*****************************************************************************
* Function Name: rsci_send_async_data
* Description  : This function determines if the tx byte queue of the channel
*                referenced by the handle is not full, and call the byte
*                transmission function.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                p_src -
*                    ptr to data to transmit
*                length -
*                    number of bytes to send and possibly receive
* Return Value : RSCI_SUCCESS -
*                    data transfer started
*                RSCI_ERR_XCVR_BUSY -
*                    channel currently busy
*                RSCI_ERR_INSUFFICIENT_SPACE -
*                    not enough space in tx queue to store data (Async/Manc)
******************************************************************************/
static rsci_err_t rsci_send_async_data(rsci_hdl_t const hdl,
                                        uint8_t         *p_src,
                                        uint16_t const  length)
{
    rsci_err_t   err = RSCI_SUCCESS;
    uint16_t    cnt;
    byteq_err_t byteq_err = BYTEQ_ERR_QUEUE_FULL;

    if (true != hdl->tx_idle)
    {
        return RSCI_ERR_XCVR_BUSY;
    }

#if RSCI_CFG_FIFO_INCLUDED
    if ((true == hdl->fifo_ctrl) && (RSCI_MODE_MANC != hdl->mode))
    {
        /* TX FIFO use check */
        if (0x00 < hdl->rom->regs->TFSR.BIT.T)
        {
            return RSCI_ERR_XCVR_BUSY;
        }

        /* reset TX FIFO */
        hdl->rom->regs->FCR.BIT.TFRST = 0x01;
        {
            {
            /* Determine amount of space left in tx queue */
                R_BYTEQ_Unused(hdl->u_tx_data.que, &cnt);
                if (cnt < length)
                {
                    /* If can't fit, return */
                    return RSCI_ERR_INSUFFICIENT_SPACE;
                }

                /* Else load bytes into tx queue for transmission */
                /* WAIT_LOOP */
                for (cnt = 0; cnt < length; cnt++)
                {
                    byteq_err = rsci_put_byte(hdl, *p_src++);
                    if (BYTEQ_SUCCESS != byteq_err)
                    {
                        /* If the return value is not BYTEQ_SUCCESS. */
                        err = RSCI_ERR_INSUFFICIENT_SPACE;
                        break;
                    }
                }
            }
        }
    }
    else /*that channel do not use FIFO but RSCI_CFG_FIFO_INCLUDED == true*/
#endif /* RSCI_CFG_FIFO_INCLUDED */
    {
        {
            {
            /* Determine amount of space left in tx queue */
                R_BYTEQ_Unused(hdl->u_tx_data.que, &cnt);

                if (cnt < length)
                {
                    /* If can't fit, return */
                    return RSCI_ERR_INSUFFICIENT_SPACE;
                }

                /* Else load bytes into tx queue for transmission */
                /* WAIT_LOOP */
                for (cnt = 0; cnt < length; cnt++)
                {
                    byteq_err = rsci_put_byte(hdl, *p_src++);
                    if (BYTEQ_SUCCESS != byteq_err)
                    {
                        /* If the return value is not BYTEQ_SUCCESS. */
                        err = RSCI_ERR_INSUFFICIENT_SPACE;
                        break;
                    }
                }
            }
        }
    }
    if (RSCI_SUCCESS == err)
    {
        hdl->tx_idle = false;
        ENABLE_TXI_INT;
    }

    return err;
} /* End of function rsci_send_async_data() */

/*****************************************************************************
* Function Name: rsci_put_byte
* Description  : Transmits byte if channel is not busy. Otherwise, byte is
*                stored in tx queue until can transmit. If buffer is full
*                and cannot store it, an error code is returned.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                byte -
*                    byte to transmit
* Return Value : RSCI_SUCCESS -
*                    data transfer started
*                RSCI_ERR_INSUFFICIENT_SPACE -
*                    not enough space in tx queue to store data (Async/Manc)
******************************************************************************/
static byteq_err_t rsci_put_byte(rsci_hdl_t const   hdl,
                                uint8_t const     byte)
{
    byteq_err_t err = BYTEQ_ERR_QUEUE_FULL;

    /* else load next byte into tx queue (space checked in calling func) */
    err = R_BYTEQ_Put(hdl->u_tx_data.que, byte);

    return err;
} /* End of function rsci_put_byte() */
#endif /* (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)*/


#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
/*****************************************************************************
* Function Name: rsci_send_sync_data
* Description  : This function determines if the channel referenced by the
*                handle is not busy, and begins the data transfer process
*                (both sending and receiving data).
*
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                p_src -
*                    ptr to data to transmit
*                p_dst -
*                    ptr to buffer to store receive data (optional)
*                length -
*                    number of bytes to send and possibly receive
*                save_rx_data -
*                    true if data clocked in should be saved to p_dst.
* Return Value : RSCI_SUCCESS -
*                    data transfer started
*                RSCI_ERR_XCVR_BUSY -
*                    channel currently busy
******************************************************************************/
static rsci_err_t rsci_send_sync_data(rsci_hdl_t const hdl,
                                    uint8_t         *p_src,
                                    uint8_t         *p_dst,
                                    uint16_t const  length,
                                    bool            save_rx_data)
{
#if RSCI_CFG_FIFO_INCLUDED
    uint8_t cnt;
    volatile uint8_t thresh_cnt;
#endif
    rsci_err_t   err = RSCI_SUCCESS;

    if (true == hdl->tx_idle)
    {

        if (true == save_rx_data)
        {
            hdl->u_rx_data.buf = p_dst;
        }
        hdl->save_rx_data  = save_rx_data;

        hdl->u_tx_data.buf = p_src;
        hdl->tx_cnt        = length;
        hdl->rx_cnt        = length;
        hdl->tx_idle       = false;
        hdl->tx_dummy      = false;

#if RSCI_CFG_FIFO_INCLUDED
        if (true == hdl->fifo_ctrl)
        {
            /* reset TX FIFO */
            hdl->rom->regs->FCR.BIT.TFRST = 0x01;

            /* reset RX FIFO */
            hdl->rom->regs->FCR.BIT.RFRST = 0x01;

            {
                /* If length is lower than RSCI_CFG_CHX_RX_FIFO_THRESH, FCR.BIT.RTRG register is set to length */
                if (length < hdl->rx_curr_thresh)
                {
                    hdl->rom->regs->FCR.BIT.RTRG = length;
                }

                thresh_cnt = hdl->rom->regs->FCR.BIT.RTRG;
                
                hdl->tx_cnt -= thresh_cnt;

                /* Repeated FIFO RX threshold count */
                /* WAIT_LOOP */
                for (cnt = 0; cnt < thresh_cnt; cnt++)
                {
                    if(0 != cnt)
                    {
                        hdl->u_tx_data.buf++;
                    }
                    RSCI_TDR(*hdl->u_tx_data.buf);    /* start transmit */
                }
            }
        }
        else
#endif /* RSCI_CFG_FIFO_INCLUDED */
        {
            {
                {
                    hdl->tx_cnt--;
                    RSCI_TDR(*hdl->u_tx_data.buf);    /* start transmit */
                }
            }
        }
        return err;
    }
    return RSCI_ERR_XCVR_BUSY;
} /* End of function sci_send_sync_data() */
#endif /* RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED */

#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
/***********************************************************************************************************************
* Function Name: R_RSCI_SendReceive
********************************************************************************************************************//**
* @brief  For Synchronous and SSPI modes only. Transmits and receives data simultaneously if the transceiver is not
* in use.
*
* @param[in]    hdl  Handle for channel. Set hdl when R_RSCI_Open() is successfully processed.
* @param[in]    p_src  Pointer to data to transmit
*
* @param[in]    p_dst  Pointer to buffer to load data into
*
* @param[in]    length  Number of bytes to send
*
* @retval   RSCI_SUCCESS  Data transfer initiated
*
* @retval   RSCI_ERR_NULL_PTR  hdl value is NULL
*
* @retval   RSCI_ERR_BAD_MODE  Channel mode not SSPI or Synchronous
*
* @retval   RSCI_ERR_XCVR_BUSY  Channel currently busy
* @details   If the transceiver is not in use, this function clocks out data from the p_src buffer while simultaneously
* clocking in data and placing it in the p_dst buffer.
* Note that the toggling of Slave Select lines for SSPI is not handled by this driver. The Slave Select line for
* the target device must be enabled prior to calling this function.
* Also, toggling of the CTS/RTS pin in Synchronous/Asynchronous mode is not handled by this driver.
*
* @note See section 2.12 Callback Function in application note for values passed to arguments of the callback function.
*/
rsci_err_t R_RSCI_SendReceive(rsci_hdl_t const hdl,
                            uint8_t         *p_src,
                            uint8_t         *p_dst,
                            uint16_t const  length)
{
    rsci_err_t   err;

#if RSCI_CFG_PARAM_CHECKING_ENABLE
    /* Check arguments */
    if ((NULL == hdl)  || (FIT_NO_PTR == hdl)       /* Check if hdl is available or not   */
    || (NULL == p_src) || (FIT_NO_PTR == p_src)     /* Check if p_src is available or not */
    || (NULL == p_dst) || (FIT_NO_PTR == p_dst))    /* Check if p_dst is available or not */
    {
        return RSCI_ERR_NULL_PTR;
    }

    if ((RSCI_MODE_SSPI != hdl->mode) && (RSCI_MODE_SYNC != hdl->mode))
    {
        return RSCI_ERR_BAD_MODE;
    }

    if (0 == length)
    {
        return RSCI_ERR_INVALID_ARG;
    }
#endif

    err = rsci_send_sync_data(hdl, p_src, p_dst, length, true);

    return err;
} /* End of function R_RSCI_SendReceive() */
#endif /* End of RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED */


#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
/*****************************************************************************
* Function Name: rsci_transfer
* Description  : Transfer for RSCI
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
static void rsci_transfer(rsci_hdl_t const hdl)
{
    uint16_t    num;
    uint8_t     byte;

    /* Get bytes from tx queue */
    (void)R_BYTEQ_Get(hdl->u_tx_data.que, (uint8_t *)&byte);

    /* TDR/FTDR register write access */
    RSCI_TDR(byte);

    /* Get data byte number from que and if the number of data bytes is 0, to disable the transfer */
    R_BYTEQ_Used(hdl->u_tx_data.que, &num);
    if (0 >= num)
    {
        /* Disable transmit interrupt */
        DISABLE_TXI_INT;
#if RSCI_CFG_TEI_INCLUDED
        /* Enable transmit end interrupt */
        hdl->rom->regs->SCR0.BIT.TEIE = 1;
        ENABLE_TEI_INT;
#endif
        hdl->tx_idle = true;    // set flag if queue empty
    }
} /* End of function rsci_transfer() */
#endif /* (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED) */

#if (RSCI_CFG_ASYNC_INCLUDED)
#if RSCI_CFG_FIFO_INCLUDED
/*****************************************************************************
* Function Name: rsci_fifo_transfer
* Description  : FIFO Transfer for RSCI
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
static void rsci_fifo_transfer(rsci_hdl_t const hdl)
{
    uint8_t cnt;
    uint8_t fifo_num;

    /* Repeated empty FIFO buffer count */
    fifo_num = RSCI_FIFO_FRAME_SIZE - hdl->rom->regs->TFSR.BIT.T;

    /* WAIT_LOOP */
    for (cnt = 0; cnt < fifo_num; cnt++)
    {
        /* RSCI Transfer */
        rsci_transfer(hdl);

        /* If the queue is empty(true == hdl->tx_idle), exit from FIFO transfer loop */
        if (true == hdl->tx_idle)
        {
            break;
        }
    }

    /* When the settings of transmit data are completed, set the SSR.BIT.TDFE flag to 0. */
    if (1 == hdl->rom->regs->SSR.BIT.TDRE)
    {
        /* write 1 to SSCR.TDREC bit. */
        hdl->rom->regs->SSCR.BIT.TDREC = 1;
    }
} /* End of function rsci_fifo_transfer() */
#endif /*End of RSCI_CFG_FIFO_INCLUDED */
#endif /* RSCI_CFG_ASYNC_INCLUDED */

#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
/*****************************************************************************
* Function Name: rsci_txi_handler
* Description  : TXI interrupt handler for RSCI
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
void rsci_txi_handler(rsci_hdl_t const hdl)
{
#if RSCI_CFG_FIFO_INCLUDED
    if ((true == hdl->fifo_ctrl) && (RSCI_MODE_MANC != hdl->mode))
    {
        {
#if (RSCI_CFG_ASYNC_INCLUDED)
            /* RSCI FIFO Transfer */
            rsci_fifo_transfer(hdl);
#endif
        }
    }
    else
#endif /* RSCI_CFG_FIFO_INCLUDED */
    {
        {
#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
            /* RSCI Transfer */
            rsci_transfer(hdl);
#endif
        }
    }
} /* End of function rsci_txi_handler() */
#endif /* (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED) */


#if RSCI_CFG_TEI_INCLUDED
/*****************************************************************************
* Function Name: rsci_tei_handler
* Description  : TEI interrupt handler for RSCI
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
void rsci_tei_handler(rsci_hdl_t const hdl)
{
    rsci_cb_args_t   args;

    /* Disable transmit end interrupt */
    DISABLE_TEI_INT;
    hdl->rom->regs->SCR0.BIT.TEIE = 0;

    /* Activate callback function if available */
    if ((NULL != hdl->callback) && (FIT_NO_FUNC != hdl->callback))
    {
        args.hdl = hdl;
        args.event = RSCI_EVT_TEI;

        /* Activate callback function */
        hdl->callback((void *)&args);
    }
} /* End of function rsci_tei_handler() */
#endif


/***********************************************************************************************************************
* Function Name: R_RSCI_Receive
********************************************************************************************************************//**
* @brief In Asynchronous or Manchester mode, fetches data from a queue which is filled by RXI interrupts. In other modes, initiates
* reception if transceiver is not in use.
* @param[in]    hdl Handle for channel. Set hdl when R_RSCI_Open() is successfully processed.
*
* @param[in]    p_dst  Pointer to buffer to load data into
*
* @param[in]    length  Number of bytes to read
*
* @retval RSCI_SUCCESS  Requested number of bytes were loaded into p_dst (Asynchronous/Manchester) Clocking in of data initiated
* (SSPI/Synchronous)
*
* @retval RSCI_ERR_NULL_PTR  hdl value is NULL
*
* @retval RSCI_ERR_BAD_MODE  Mode specified not currently supported
*
* @retval RSCI_ERR_INSUFFICIENT_DATA  Insufficient data in receive queue to fetch all data (Asynchronous/Manchester)
*
* @retval RSCI_ERR_XCVR_BUSY  Channel currently busy (SSPI/Synchronous)
*
* @details In Asynchronous or Manchester mode, this function gets data received on an RSCI channel referenced by the handle from its
* receive queue. This function will not block if the requested number of bytes is not available. In
* SSPI/Synchronous modes, the clocking in of data begins immediately if the transceiver is not already in use.
* The value assigned to RSCI_CFG_DUMMY_TX_BYTE in r_rsci_config.h is clocked out while the receive data is being clocked in.\n
* If any errors occurred during reception, the callback function specified in R_RSCI_Open() is executed. Check
* an event passed with the argument of the callback function to see if the reception has been successfully
* completed. See Section 2.12 Callback Function in application note for details.\n
* Note that the toggling of Slave Select lines when in SSPI mode is not handled by this driver. The Slave
* Select line for the target device must be enabled prior to calling this function.
* @note See section 2.12 Callback Function in application note for values passed to arguments of the callback function.
* In Asynchronous mode, when data match detected, received data stored in a queue and notify to user by callback function
* with event RSCI_EVT_RX_CHAR_MATCH.
*/
rsci_err_t R_RSCI_Receive(rsci_hdl_t const hdl,
                        uint8_t         *p_dst,
                        uint16_t const  length)
{
rsci_err_t   err = RSCI_SUCCESS;


    /* Check arguments */

#if RSCI_CFG_PARAM_CHECKING_ENABLE
    /* Check argument hdl, p_dst */
    if ((NULL == hdl) || (FIT_NO_PTR == hdl) || (NULL == p_dst) || (FIT_NO_PTR == p_dst))
    {
        return RSCI_ERR_NULL_PTR;
    }
    if ((RSCI_MODE_OFF == hdl->mode) || (RSCI_MODE_MAX <= hdl->mode))
    {
        return RSCI_ERR_BAD_MODE;
    }
    if (0 == length)
    {
        return RSCI_ERR_INVALID_ARG;
    }
#endif

    if ((RSCI_MODE_ASYNC == hdl->mode) || (RSCI_MODE_MANC == hdl->mode))
    {
        /* mode is ASYNC/MANC */
#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
        err = rsci_receive_async_data(hdl, p_dst, length);
#endif
    }
    else
    {
        /* mode is SSPI/SYNC */
#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
        err = rsci_receive_sync_data(hdl, p_dst, length);
#endif
    }

    return err;
} /* End of function R_RSCI_Receive() */

#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
/*****************************************************************************
* Function Name: rsci_receive_async_data
* Description  : This function determines if the rx byte queue of the channel 
*                referenced by the handle, the requested number of bytes 
*                is available, and get the data from the rx byte queue.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                p_dst -
*                    ptr to buffer to load data into
*                length - 
*                    number of bytes to read
* Return Value : RSCI_SUCCESS -
*                    requested number of byte loaded into p_dst
*                RSCI_ERR_INSUFFICIENT_DATA -
*                    rx queue does not contain requested amount of data
******************************************************************************/
static rsci_err_t rsci_receive_async_data(rsci_hdl_t const hdl,
                                        uint8_t         *p_dst,
                                        uint16_t const  length)
{
    rsci_err_t   err = RSCI_SUCCESS;
    uint16_t    cnt;
    byteq_err_t byteq_err = BYTEQ_SUCCESS;

    /* CHECK FOR SUFFICIENT DATA IN QUEUE, AND FETCH IF AVAILABLE */
    R_BYTEQ_Used(hdl->u_rx_data.que, &cnt);

    if (cnt < length)
    {
        return RSCI_ERR_INSUFFICIENT_DATA;
    }

    /* Get bytes from rx queue */
    /* WAIT_LOOP */
    for (cnt = 0; cnt < length; cnt++)
    {
        /* Disable RXI Interrupt */
        DISABLE_RXI_INT;
        byteq_err = R_BYTEQ_Get(hdl->u_rx_data.que, p_dst++);
        ENABLE_RXI_INT;
        if (BYTEQ_SUCCESS != byteq_err)
        {
            err = RSCI_ERR_INSUFFICIENT_DATA;
            break;
        }
    }
    return err;
} /* End of function rsci_receive_async_data() */
#endif /* (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED) */

#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
/*****************************************************************************
* Function Name: rsci_receive_sync_data
* Description  : This function determines if the channel referenced by the
*                handle is not busy, and dummy data send.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                p_dst -
*                    ptr to buffer to load data into
*                length - 
*                    number of bytes to read
* Return Value : RSCI_SUCCESS -
*                    requested number of byte loaded into p_dst
*                RSCI_ERR_XCVR_BUSY -
*                    channel currently busy
******************************************************************************/
static rsci_err_t rsci_receive_sync_data(rsci_hdl_t const hdl,
                                        uint8_t         *p_dst,
                                        uint16_t const  length)
{
#if RSCI_CFG_FIFO_INCLUDED
    uint8_t cnt;
    uint8_t thresh_cnt;
#endif

    /* IF TRANCEIVER NOT IN USE, START DUMMY TRANSMIT TO CLOCK IN DATA */
    if (true == hdl->tx_idle)
    {
        hdl->u_rx_data.buf = p_dst;
        hdl->save_rx_data  = true;               /* save the data clocked in */
        hdl->tx_idle       = false;
        hdl->tx_cnt        = length;
        hdl->rx_cnt        = length;
        hdl->tx_dummy      = true;

#if RSCI_CFG_FIFO_INCLUDED
        if (true == hdl->fifo_ctrl)
        {
            /* reset TX FIFO */
            hdl->rom->regs->FCR.BIT.TFRST = 0x01;

            /* reset RX FIFO */
            hdl->rom->regs->FCR.BIT.RFRST = 0x01;
            {
                if (length > RSCI_FIFO_FRAME_SIZE)
                {
                    thresh_cnt = RSCI_FIFO_FRAME_SIZE;
                }
                else
                {
                    /* If length is lower than SCI_CFG_CHXX_RX_FIFO_THRESH, FCR.BIT.RTRG register is set to length */
                    if (length < hdl->rx_curr_thresh)
                    {
                        hdl->rom->regs->FCR.BIT.RTRG = length;
                    }
                    thresh_cnt = length;
                }

                hdl->tx_cnt -= thresh_cnt;

                /* WAIT_LOOP */
                for (cnt = 0; cnt < thresh_cnt; cnt++)
                {
                    RSCI_TDR(RSCI_CFG_DUMMY_TX_BYTE);    /* start transmit */
                }
            }
        }
        else
#endif /* End of RSCI_CFG_FIFO_INCLUDED */
        {
            {
                hdl->tx_cnt--;
                RSCI_TDR(RSCI_CFG_DUMMY_TX_BYTE);    /* start transfer */
            }
        }
        return RSCI_SUCCESS;
    }

    return RSCI_ERR_XCVR_BUSY;
} /* End of function rsci_receive_sync_data() */
#endif /* End of RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED */

/*****************************************************************************
* Function Name: rsci_receive
* Description  : Receive for RSCI
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
static void rsci_receive(rsci_hdl_t const hdl)
{
    rsci_cb_args_t   args;
    uint8_t         byte;

    /* Read byte */
    RSCI_RDR(byte);
    if ((RSCI_MODE_ASYNC == hdl->mode) || (RSCI_MODE_MANC == hdl->mode))
    {
#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)

        /* Place byte in queue */
        if (R_BYTEQ_Put(hdl->u_rx_data.que, byte) == BYTEQ_SUCCESS)
        {
            args.event = RSCI_EVT_RX_CHAR;
        }
        else
        {
            args.event = RSCI_EVT_RXBUF_OVFL;
        }

        /* Do callback if available */
        if ((NULL != hdl->callback) && (FIT_NO_FUNC != hdl->callback))
        {
            args.hdl  = hdl;
            args.byte = byte;

            /* Casting to void type is valid */
            hdl->callback((void *)&args);
        }
#endif  /* End of RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED */
    }
    else
    {
#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
        hdl->rx_cnt--;

        /* Place byte in buffer if Receive() or SendReceive() */
        if (true == hdl->save_rx_data)
        {
            *hdl->u_rx_data.buf++ = byte;
        }

        /* See if more bytes to transfer */
        if (0 < hdl->rx_cnt)
        {
            if (0 < hdl->tx_cnt)
            {
                /* send another byte */
                if (true == hdl->tx_dummy)
                {
                    hdl->tx_cnt--;
                    RSCI_TDR(RSCI_CFG_DUMMY_TX_BYTE);
                }
                else
                {
                    hdl->tx_cnt--;
                    hdl->u_tx_data.buf++;
                    RSCI_TDR(*hdl->u_tx_data.buf);
                }
            }
        }
        else
        {
            hdl->tx_idle = true;

            /* Do callback if available */
            if ((NULL != hdl->callback) && (FIT_NO_FUNC != hdl->callback))
            {
                args.hdl = hdl;
                args.event = RSCI_EVT_XFER_DONE;

                /* Casting to void type is valid */
                hdl->callback((void *)&args);
            }
        }
#endif /* End of RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED */
    }
} /* End of function rsci_receive() */


#if RSCI_CFG_FIFO_INCLUDED
#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
/*****************************************************************************
* Function Name: rsci_fifo_receive_sync
* Description  : FIFO Receive for RSCI mode is SYNC and SSPI
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
static void rsci_fifo_receive_sync(rsci_hdl_t const hdl)
{
    uint8_t       cnt;
    uint8_t       fifo_num_rx;
    uint8_t       fifo_num_tx;
    rsci_cb_args_t args;
    uint8_t       byte_rx[RSCI_FIFO_FRAME_SIZE];

    fifo_num_rx = hdl->rom->regs->RFSR.BIT.R;
    /* WAIT_LOOP */
    for (cnt = 0; cnt < fifo_num_rx; cnt++)
    {
        RSCI_RDR(byte_rx[cnt]);
    }

    hdl->rx_cnt -= fifo_num_rx;

    /* Place byte in buffer if Receive() or SendReceive() */
    if (true == hdl->save_rx_data)
    {
        /* WAIT_LOOP */
        for (cnt = 0; cnt < fifo_num_rx; cnt++)
        {
            /* SCI Receive */
            *hdl->u_rx_data.buf++ = byte_rx[cnt];
        }
    }

    /* See if more bytes to transfer */
    if (0 < hdl->rx_cnt)
    {
        if (hdl->rom->regs->FCR.BIT.RTRG > hdl->rx_cnt)
        {
            hdl->rom->regs->FCR.BIT.RTRG = hdl->rx_cnt;
        }

        if (0 < hdl->tx_cnt)
        {
            if (hdl->tx_cnt > fifo_num_rx)
            {
                fifo_num_tx  = fifo_num_rx;
                hdl->tx_cnt -= fifo_num_rx;
            }
            else
            {
                fifo_num_tx  = hdl->tx_cnt;
                hdl->tx_cnt  = 0;
            }

            /* send another byte */
            if (true == hdl->tx_dummy)
            {
                /* WAIT_LOOP */
                for (cnt = 0; cnt < fifo_num_tx; cnt++)
                {
                    RSCI_TDR(RSCI_CFG_DUMMY_TX_BYTE);
                }
            }
            else
            {
                /* WAIT_LOOP */
                for (cnt = 0; cnt < fifo_num_tx; cnt++)
                {
                    hdl->u_tx_data.buf++;
                    RSCI_TDR(*hdl->u_tx_data.buf);
                }
            }
        }
    }
    else
    {
        hdl->rom->regs->FCR.BIT.RTRG = hdl->rx_curr_thresh;
        hdl->tx_idle = true;

        /* Do callback if available */
        if ((NULL != hdl->callback) && (FIT_NO_FUNC != hdl->callback))
        {
            args.hdl = hdl;
            args.event = RSCI_EVT_XFER_DONE;

            /* Casting pointer to void* type is valid */
            hdl->callback((void *)&args);
        }
    }

} /* End of function rsci_fifo_receive_sync() */
#endif /* End of RSCI_CFG_FIFO_INCLUDED */
#endif /* End of RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED */

#if RSCI_CFG_FIFO_INCLUDED
/*****************************************************************************
* Function Name: rsci_fifo_receive
* Description  : FIFO Receive for RSCI
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
static void rsci_fifo_receive(rsci_hdl_t const hdl)
{
#if (RSCI_CFG_ASYNC_INCLUDED)
    uint16_t        cnt;
    uint16_t        fifo_num;
    rsci_cb_args_t   args;
    uint8_t         byte_rx[RSCI_FIFO_FRAME_SIZE];
#endif
    if (RSCI_MODE_ASYNC == hdl->mode)
    {
#if (RSCI_CFG_ASYNC_INCLUDED)
        {
            /* Casting unsigned char type to uint16_t type is valid */
            fifo_num = (uint16_t)hdl->rom->regs->RFSR.BIT.R;

            /* RX FIFO flush */
            /* WAIT_LOOP */
            for (cnt = 0; cnt < fifo_num; cnt++)
            {
                /* Read byte */
                RSCI_RDR(byte_rx[cnt]);
            }

            /* Determine amount of space left in rx queue */
            (void)R_BYTEQ_Unused(hdl->u_rx_data.que, &cnt);
            if (cnt >= fifo_num)
            {
                /* free space is enough */
                args.event = RSCI_EVT_RX_CHAR;
            }
            else
            {
                /* insufficient free space, store as much as possible */
                fifo_num = cnt;
                args.event = RSCI_EVT_RXBUF_OVFL;
            }

            /* WAIT_LOOP */
            for (cnt = 0; cnt < fifo_num; cnt++)
            {
                /* store bytes to rx queue for R_SCI_Receive */
                (void)R_BYTEQ_Put(hdl->u_rx_data.que, byte_rx[cnt]);
            }

            /* Do callback if available */
            if ((NULL != hdl->callback) && (FIT_NO_FUNC != hdl->callback))
            {
                args.hdl = hdl;

                /* Number of bytes were stored to queue */
                args.num = (uint8_t)fifo_num;

                /* Casting pointer to void* type is valid */
                hdl->callback((void *)&args);
            }
        }
#endif /* End of RSCI_CFG_ASYNC_INCLUDED*/
    }
    else
    {
#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
        /* RSCI Receive */
        rsci_fifo_receive_sync(hdl);
#endif
    }

    /* When the readings of receive data are completed, set the SSR.BIT.RDRF flag to 0. */
    if (1 == hdl->rom->regs->SSR.BIT.RDRF)
    {
        /* write 1 to SSCR.RDRFC bit. */
        hdl->rom->regs->SSCR.BIT.RDRFC = 1;
    }

    if (RSCI_MODE_ASYNC == hdl->mode)
    {
        if (1 == hdl->rom->regs->RFSR.BIT.DR)
        {
            /* write 1 to RFSCR.DRC bit. */
            hdl->rom->regs->RFSCR.BIT.DRC = 1;
        }
    }
} /* End of function rsci_fifo_receive() */
#endif /* End of RSCI_CFG_FIFO_INCLUDED */

#if RSCI_CFG_DATA_MATCH_INCLUDED
/*****************************************************************************
* Function Name: rsci_receive_data_match
* Description  : RSCI receive data match
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
static void rsci_receive_data_match(rsci_hdl_t const hdl)
{
    volatile rsci_cb_args_t   args;
    volatile uint8_t         byte;

    if (RSCI_MODE_ASYNC == hdl->mode)
    {
#if (RSCI_CFG_ASYNC_INCLUDED)
        if (0 == hdl->rom->regs->SCR0.BIT.DCME) /* DCME automatically set 0 when data matched */
        {
            hdl->rom->regs->SSCR.BIT.DCMFC = 1; /* Clear Data Match Flag */

            if ((0 == hdl->rom->regs->SSR.BIT.DFER )  &&  (0 == hdl->rom->regs->SSR.BIT.DPER )) /* Check framing error and parity error */
            {
                /* Casting unsigned char type to unin8_t type is valid */
                byte = (uint8_t)(hdl->rom->regs->SCR4.BIT.CMPD); /* Read data from comparison data register */

                /* Place byte in queue */
                if (R_BYTEQ_Put(hdl->u_rx_data.que, byte) == BYTEQ_SUCCESS)
                {
                    args.event = RSCI_EVT_RX_CHAR_MATCH;
                }
                else
                {
                    args.event = RSCI_EVT_RXBUF_OVFL;
                }

                /* Do callback if available */
                if ((NULL != hdl->callback) && (FIT_NO_FUNC != hdl->callback))
                {
                    args.hdl = hdl;
                    args.byte = byte;

                    /* Casting to void* type is valid  */
                    hdl->callback((void *)&args);
                }
            }
        }
#endif /* End of RSCI_CFG_ASYNC_INCLUDED */
    }
} /* End of function rsci_receive_data_match() */
#endif /* End of RSCI_CFG_DATA_MATCH_INCLUDED */

/*****************************************************************************
* Function Name: rsci_rxi_handler
* Description  : RXI interrupt handler for RSCI
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
void rsci_rxi_handler(rsci_hdl_t const hdl)
{
#if RSCI_CFG_DATA_MATCH_INCLUDED
    if (1 == hdl->rom->regs->SSR.BIT.DCMF) /* Check Data match flag */
    {
        rsci_receive_data_match(hdl);
    }
    else
#endif
    {
#if RSCI_CFG_FIFO_INCLUDED
        if ((true == hdl->fifo_ctrl) && (RSCI_MODE_MANC != hdl->mode))
        {
            /* RSCI FIFO Receive */
            rsci_fifo_receive(hdl);
        }
        else
#endif
        {
            /* RSCI Receive */
            rsci_receive(hdl);
        }
    }
} /* End of function rsci_rxi_handler() */


/*****************************************************************************
* Function Name: rsci_error
* Description  : Error for RSCI
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
static void rsci_error(rsci_hdl_t const hdl)
{
    rsci_cb_args_t   args;
    uint8_t         byte;
    uint32_t reg_ssr = RSCI_SSR;
    uint32_t reg_mmsr = RSCI_MMSR;

    if ((0 != (reg_ssr & RSCI_RCVR_ERR_MASK)) || (0 != (reg_mmsr & RSCI_MMSR_ERR_MASK)))
    {
        if (0 != (reg_ssr & RSCI_SSR_ORER_MASK))
        {
            args.event = RSCI_EVT_OVFL_ERR;
        }
#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
        else if (0 != (reg_ssr & RSCI_SSR_APER_MASK))
        {
            args.event = RSCI_EVT_PARITY_ERR;
        }
        else if (0 != (reg_ssr & RSCI_SSR_AFER_MASK))
        {
            args.event = RSCI_EVT_FRAMING_ERR;
        }
#endif
#if RSCI_CFG_MANC_INCLUDED
        else if (0 != (reg_mmsr & RSCI_MMSR_MCER_MASK))
        {
            args.event = RSCI_EVT_MANCHESTER_CODE_ERR;
        }
        else if (0 != (reg_mmsr & RSCI_MMSR_SYER_MASK))
        {
            args.event = RSCI_EVT_RECEIVE_SYNC_ERR;
        }
        else if (0 != (reg_mmsr & RSCI_MMSR_SBER_MASK))
        {
            args.event = RSCI_EVT_START_BIT_ERR;
        }
        else if (0 != (reg_mmsr & RSCI_MMSR_PFER_MASK))
        {
            args.event = RSCI_EVT_PREFACE_ERR;
        }
#endif
        else
        {
            /* Do Nothing */
            R_BSP_NOP();
        }

        /* Flush register */
        RSCI_RDR(byte);

        /* Clear error condition */
        /* WAIT_LOOP */
        while ((0 != (RSCI_SSR & RSCI_RCVR_ERR_MASK)) || (0 != (RSCI_MMSR & RSCI_MMSR_ERR_MASK)))
        {
            RSCI_RDR(byte);

            RSCI_SSCR  |= RSCI_SSCR_ERR_CLEAR_MASK;      /* APER, AFER, ORER clear */
            RSCI_MMSCR |= RSCI_MMSCR_ERR_CLEAR_MASK;    /* PFER, SYER, SBER, MCER clear */

            if ((0 != (RSCI_SSR & RSCI_RCVR_ERR_MASK)) || (0 != (RSCI_MMSR & RSCI_MMSR_ERR_MASK)))
            {
                R_BSP_NOP(); /* read and Compare */
            }
        }

        /* Do callback for error */
        if ((NULL != hdl->callback) && (FIT_NO_FUNC != hdl->callback))
        {
            args.hdl  = hdl;
            args.byte = byte;

            /* Casting to void* type is valid */
            hdl->callback((void *)&args);
        }
    }

} /* End of function rsci_error() */

#if RSCI_CFG_FIFO_INCLUDED
/*****************************************************************************
* Function Name: rsci_fifo_error
* Description  : FIFO Error for RSCI
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
static void rsci_fifo_error(rsci_hdl_t const hdl)
{
    rsci_cb_args_t       args;
    uint32_t             reg;
    volatile uint32_t    ssrfifo_data;
    volatile uint32_t   dummy;

    reg = RSCI_SSR;
    if (0 != (reg & RSCI_RCVR_ERR_MASK))
    {
        if (0 != (reg & RSCI_SSR_ORER_MASK))
        {
            args.event = RSCI_EVT_OVFL_ERR;
        }
#if (RSCI_CFG_ASYNC_INCLUDED)
        else if (0 != (reg & RSCI_SSR_APER_MASK))
        {
            args.event = RSCI_EVT_PARITY_ERR;
        }
        else if (0 != (reg & RSCI_SSR_AFER_MASK))
        {
            args.event = RSCI_EVT_FRAMING_ERR;
        }
#endif
        else
        {
            /* Do Nothing */
        }

        /* Do callback for error */
        if ((NULL != hdl->callback) && (FIT_NO_FUNC != hdl->callback))
        {
            args.hdl = hdl;
            args.byte = 0;

            /* Casting pointer to void* type is valid */
            hdl->callback((void *)&args);
        }

        /* if error condition don't clear in callback when it clear at here */
        reg = RSCI_SSR;
        if (0 != (reg & RSCI_RCVR_ERR_MASK))
        {
            /* Flush register */
            /* WAIT_LOOP */
            while (0 != hdl->rom->regs->RFSR.BIT.R)
            {
                dummy = hdl->rom->regs->RDR.LONG;              /* FRDR dummy read */
            }

            /* Clear error condition */
            /* WAIT_LOOP */
            while (0x00 != (RSCI_SSR & RSCI_RCVR_ERR_MASK))   /* Check APER, AFER, ORER flags */
            {
                ssrfifo_data = RSCI_SSR;                     /* SSRFIFO dummy read */
                RSCI_SSCR |= RSCI_SSCR_ERR_CLEAR_MASK;      /* APER, AFER, ORER clear */
                if (0x00 != (RSCI_SSR & RSCI_RCVR_ERR_MASK))
                {
                    R_BSP_NOP();                                /* read and Compare */
                }
            }
        }
    }

    return;
} /* End of function rsci_fifo_error() */
#endif /* End of RSCI_CFG_FIFO_INCLUDED */

/*****************************************************************************
* Function Name: rsci_eri_handler
* Description  : ERI interrupt handler for SCI UART mode
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
void rsci_eri_handler(rsci_hdl_t const hdl)
{
#if RSCI_CFG_FIFO_INCLUDED
    if ((true == hdl->fifo_ctrl) && (RSCI_MODE_MANC != hdl->mode))
    {
        /* RSCI FIFO Error */
        rsci_fifo_error(hdl);
    }
    else
#endif
    {
        /* RSCI error */
        rsci_error(hdl);
    }
} /* End of function rsci_eri_handler() */

/***********************************************************************************************************************
* Function Name: R_RSCI_Control
********************************************************************************************************************//**
* @brief  This function configures and controls the operating mode for the RSCI channel.
*
* @param[in] hdl  Handle for channel. Set hdl when R_RSCI_Open() is successfully processed.
*
* @param[in] cmd  Command to run (see Section 3. R_RSCI_Control() in application note for details)
*
* @param[in] p_args   Pointer to arguments (see Section 3. R_RSCI_Control() in application note for details) specific to
* command, casted to void *
*
* @retval RSCI_SUCCESS  Successful; channel initialized.
*
* @retval RSCI_ERR_NULL_PTR  hdl or p_args pointer is NULL (when required)
*
* @retval RSCI_ERR_BAD_MODE  Mode specified not currently supported
*
* @retval RSCI_ERR_INVALID_ARG
*                    The cmd value or an element of p_args contains an invalid value.
* @details This function is used for configuring special hardware features such as changing driver configuration and
* obtaining driver status.
* The CTS/ RTS pin functions as RTS by default hardware control. By issuing an RSCI_CMD_EN_CTS_IN, the pin functions as CTS.
* @note When RSCI_CMD_CHANGE_BAUD is used, the optimum values for BRR, SEMR.ABCS, and SMR.CKS is calculated based on
* the bit rate specified. This however does not guarantee a low bit error rate for all peripheral clock/baud rate
* combinations.\n
* If the command RSCI_CMD_EN_CTS_IN is to be used, the pin direction must be selected before calling the
* R_RSCI_Open() function, and the pin function and mode must be selected after calling the R_RSCI_Open()
* function. See Section 3. R_RSCI_Control() for details.
*/
rsci_err_t R_RSCI_Control(rsci_hdl_t const     hdl,
                        rsci_cmd_t const     cmd,
                        void                *p_args)
{
    rsci_err_t   err = RSCI_SUCCESS;
    rsci_baud_t  *baud;
    int32_t     bit_err;


#if RSCI_CFG_PARAM_CHECKING_ENABLE
    /* Check argument hdl */
    if ((NULL == hdl) || (FIT_NO_PTR == hdl))
    {
        return RSCI_ERR_NULL_PTR;
    }

    /* Check argument p_args*/
    if ((NULL == p_args) || (FIT_NO_PTR == p_args))
    {
        if (RSCI_CMD_CHANGE_BAUD == cmd)
        {
            return RSCI_ERR_NULL_PTR;
        }
#if RSCI_CFG_FIFO_INCLUDED
        if ((RSCI_CMD_CHANGE_TX_FIFO_THRESH == cmd) || (RSCI_CMD_CHANGE_RX_FIFO_THRESH == cmd))
        {
            return RSCI_ERR_NULL_PTR;
        }
#endif
        if ((RSCI_CMD_SET_TXI_PRIORITY == cmd) || (RSCI_CMD_SET_RXI_PRIORITY == cmd))
        {
            return RSCI_ERR_NULL_PTR;
        }
    }
    if ((RSCI_MODE_OFF == hdl->mode) || (RSCI_MODE_MAX <= hdl->mode))
    {
        return RSCI_ERR_BAD_MODE;
    }
#if RSCI_CFG_FIFO_INCLUDED
    if (RSCI_CMD_CHANGE_TX_FIFO_THRESH == cmd)
    {
        /* Casting void* type is valid */
        if (31 < (*(uint8_t *)p_args))
        {
            return RSCI_ERR_INVALID_ARG;
        }
    }
    if (RSCI_CMD_CHANGE_RX_FIFO_THRESH == cmd)
    {
        /* Casting void* type is valid */
        if ((1 > (*(uint8_t *)p_args)) || (31 < (*(uint8_t *)p_args)))
        {
            return RSCI_ERR_INVALID_ARG;
        }
    }
#endif
    if ((RSCI_CMD_SET_TXI_PRIORITY == cmd) || (RSCI_CMD_SET_RXI_PRIORITY == cmd))
    {
        /* Casting void* type is valid */
        if ((1 > (*(uint8_t *)p_args)) || (BSP_MCU_IPL_MAX < (*(uint8_t *)p_args)))
        {
            return RSCI_ERR_INVALID_ARG;
        }
    }
#endif /* End of RSCI_CFG_PARAM_CHECKING_ENABLE */
    
    /* COMMANDS COMMON TO ALL MODES */

    switch (cmd)
    {
        case (RSCI_CMD_CHANGE_BAUD):
        {
            /* Casting void* type is valid */
            baud = (rsci_baud_t *)p_args;
#if (RSCI_CFG_ASYNC_INCLUDED)
            hdl->pclk_speed = baud->pclk;           // save for break generation
#endif
            hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
            RSCI_SCR0_DUMMY_READ;
            bit_err = rsci_init_bit_rate(hdl, baud->pclk, baud->rate);
            RSCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            if (1000 == bit_err)
            {
                err = RSCI_ERR_INVALID_ARG;      // impossible baud rate; 100% error
            }
            else
            {
                hdl->baud_rate = baud->rate;    // save for break generation
            }
            break;
        }

        case (RSCI_CMD_EN_CTS_IN):
        {
            if (RSCI_MODE_SSPI != hdl->mode)
            {
                /* PFS & port pins must be configured for CTS prior to calling this */
                hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
                RSCI_SCR0_DUMMY_READ;
                hdl->rom->regs->SCR1.BIT.CTSE = 1;     // enable CTS input
                RSCI_IR_TXI_CLEAR;
                hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            }
            else
            {
                /* Can not use CTS in smart card interface mode, simple SPI mode, and simple I2C mode */
                err = RSCI_ERR_INVALID_ARG;
            }
            break;
        }

        case (RSCI_CMD_XFER_LSB_FIRST):
        {
            hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
            RSCI_SCR0_DUMMY_READ;
            hdl->rom->regs->SCR3.BIT.DDIR = 1;
            RSCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            break;
        }

        case (RSCI_CMD_XFER_MSB_FIRST):
        {
            hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
            RSCI_SCR0_DUMMY_READ;
            hdl->rom->regs->SCR3.BIT.DDIR = 0;
            RSCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            break;
        }

        case (RSCI_CMD_INVERT_DATA):
        {
            hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
            RSCI_SCR0_DUMMY_READ;
            hdl->rom->regs->SCR3.BIT.DINV ^= 1;
            RSCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            break;
        }

#if RSCI_CFG_FIFO_INCLUDED
        case (RSCI_CMD_CHANGE_TX_FIFO_THRESH):
        {
            if (true == hdl->fifo_ctrl)
            {
                /* save current TX FIFO threshold */
                hdl->tx_curr_thresh = *((uint8_t *)p_args);

                /* change TX FIFO threshold */
                hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
                RSCI_SCR0_DUMMY_READ;

                /* Casting void* type is valid */
                hdl->rom->regs->FCR.BIT.TTRG = *((uint8_t *)p_args);
                RSCI_IR_TXI_CLEAR;
                hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            }
            else
            {
                err = RSCI_ERR_INVALID_ARG;
            }
            break;
        }

        case (RSCI_CMD_CHANGE_RX_FIFO_THRESH):
        {
            if (true == hdl->fifo_ctrl)
            {
                /* save current RX FIFO threshold */
                hdl->rx_curr_thresh = *((uint8_t *)p_args);

                /* change RX FIFO threshold */
                hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
                RSCI_SCR0_DUMMY_READ;

                /* Casting void* type is valid */
                hdl->rom->regs->FCR.BIT.RTRG = *((uint8_t *)p_args);
                RSCI_IR_TXI_CLEAR;
                hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            }
            else
            {
                err = RSCI_ERR_INVALID_ARG;
            }
            break;
        }
#endif /* End of RSCI_CFG_FIFO_INCLUDED */

        case (RSCI_CMD_SET_TXI_PRIORITY):
        {
            /* Casting void type to uint8_t type is valid */
            *hdl->rom->ipr_txi = *((uint8_t*)p_args);
            break;
        }

        case (RSCI_CMD_SET_RXI_PRIORITY):
        {
            /* Casting void type to uint8_t type is valid */
            *hdl->rom->ipr_rxi = *((uint8_t*)p_args);
            break;
        }

        default:
        {
            /* ASYNC-SPECIFIC COMMANDS */
            if (RSCI_MODE_ASYNC == hdl->mode)
            {
#if (RSCI_CFG_ASYNC_INCLUDED)
                err = rsci_async_cmds(hdl, cmd, p_args);
#endif
            }

            /* MANC-SPECIFIC COMMANDS */
            else if (RSCI_MODE_MANC == hdl->mode)
            {
#if (RSCI_CFG_MANC_INCLUDED)
                err = rsci_manc_cmds(hdl, cmd, p_args);
#endif
            }

            /* SSPI/SYNC-SPECIFIC COMMANDS */
            else
            {
#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
                err = rsci_sync_cmds(hdl, cmd, p_args);
#endif
            }
            break;
        }
    }

    return err;
} /* End of function R_RSCI_Control() */

/***********************************************************************************************************************
* Function Name: R_RSCI_Close
********************************************************************************************************************//**
* @brief This function removes power from the RSCI channel and disables the associated interrupts.
*
* @param[in] hdl  Handle for channel. Set hdl when R_RSCI_Open() is successfully processed.
*
* @retval RSCI_SUCCESS Successful; channel closed
*
* @retval RSCI_ERR_NULL_PTR hdl is NULL
*
* @details    Disables the RSCI channel designated by the handle and enters module-stop state.
* @note This function will abort any transmission or reception that may be in progress.
*/
rsci_err_t R_RSCI_Close(rsci_hdl_t const hdl)
{

#if RSCI_CFG_PARAM_CHECKING_ENABLE
    /* Check argument hdl */
    if ((NULL == hdl) || (FIT_NO_PTR == hdl))
    {
        return RSCI_ERR_NULL_PTR;
    }
#endif

    /* disable ICU interrupts */
        rsci_disable_ints(hdl);

    /* free tx and rx queues */
#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
    if ((RSCI_MODE_ASYNC == hdl->mode) || (RSCI_MODE_MANC == hdl->mode))
    {
        R_BYTEQ_Close(hdl->u_tx_data.que);
        R_BYTEQ_Close(hdl->u_rx_data.que);
    }
#endif
#if RSCI_CFG_FIFO_INCLUDED
    if ((true == hdl->fifo_ctrl) && (RSCI_MODE_MANC != hdl->mode))
    {
        /* reset FIFO threshold */
        hdl->rx_curr_thresh = hdl->rx_dflt_thresh;
        hdl->tx_curr_thresh = hdl->tx_dflt_thresh;
    }
#endif

    /* Mark the channel as not in use and power down */
    power_off(hdl);
    hdl->mode = RSCI_MODE_OFF;

    return RSCI_SUCCESS;
} /* End of function R_RSCI_Close() */


/***********************************************************************************************************************
* Function Name: R_RSCI_GetVersion
********************************************************************************************************************//**
* @brief This function returns the driver version number at runtime.
* @return Version number.
* @details  Returns the version of this module. The version number is encoded such that the top 2 bytes are the major
* version number and the bottom 2 bytes are the minor version number.
* @note None
*/
uint32_t  R_RSCI_GetVersion(void)
{
    uint32_t const version = (RSCI_VERSION_MAJOR << 16) | RSCI_VERSION_MINOR;

    return version;
} /* End of function R_RSCI_GetVersion() */

