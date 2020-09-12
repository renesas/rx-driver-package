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
* History      : DD.MM.YYYY Version  Description
*              : 01.12.2014 1.00     First Release
*              : 27.01.2015 1.01     Chage symbol name of Arguments 
*                                    None change proceedures.
*******************************************************************************/
/*******************************************************************************
* System Name  : interface file for IrDA fit module.
* File Name    : r_irda_sci_rx.c
* Version      : 1.01
* Device       : RX
* Abstract     : 
* Tool-Chain   : Renesas RXC Toolchain v2.02.00
* OS           : not use
* H/W Platform : RSKRX113
* Description  : Functions for using IrDA on RX devices. 
* Limitation   : none
*******************************************************************************/

/*******************************************************************************
  Includes <System Includes> , "Project Includes"
*******************************************************************************/
/* Access to peripherals and board defines. */
#include "platform.h"

/* Include specifics for chosen MCU.  */
#include "r_irda_sci_rx_private.h"

/* Defines for IRDA support */
#include "r_irda_sci_rx_if.h"
#include "r_irda_sci_rx_config.h"

#include "r_byteq_if.h"

/*******************************************************************************
  Typedef definitions
*******************************************************************************/

/*******************************************************************************
  Macro definitions
*******************************************************************************/

/*******************************************************************************
  Private global variables and functions
*******************************************************************************/
irda_sci_err_t R_IRDA_SCI_Open      (uint8_t const            chan,
                                    irda_sci_t * const       p_cfg,
                                     void                    (* const p_callback)(void *p_args),
                                    irda_sci_hdl_t * const   p_hdl);

irda_sci_err_t R_IRDA_SCI_Send       (irda_sci_hdl_t const     hdl,
                                     uint8_t                 *p_src,
                                     uint16_t const           length);

irda_sci_err_t R_IRDA_SCI_Receive    (irda_sci_hdl_t const hdl,
                                     uint8_t         *p_dst,
                                     uint16_t const  length);

irda_sci_err_t R_IRDA_SCI_Control    (irda_sci_hdl_t const  hdl,
                                     irda_sci_cmd_t const  cmd,
                                     void                 *p_args);

irda_sci_err_t R_IRDA_SCI_Close      (irda_sci_hdl_t const hdl);

uint32_t       R_IRDA_SCI_GetVersion (void);

void           irda_txi_handler      (irda_sci_hdl_t const hdl);
void           irda_tei_handler      (irda_sci_hdl_t const hdl);

void           irda_rxi_handler      (irda_sci_hdl_t const hdl);
void           irda_eri_handler      (irda_sci_hdl_t const hdl);

/*****************************************************************************
* Function Name: R_IRDA_SCI_Open
* Description  : Initializes an SCI channel for the IrDA mode.
*
* Arguments    : chan -
*                    channel to initialize
*                p_cfg -
*                    ptr to configuration union; structure specific to mode
*                p_callback -
*                    ptr to function called from interrupt when a receiver 
*                    error is detected or for transmit end (TEI) condition
*                p_hdl -
*                     pointer to a handle for channel (value set here)
* Return Value : IRDA_SCI_SUCCESS -
*                    channel opened successfully
*                IRDA_SCI_ERR_BAD_CHAN -
*                    channel number invalid for part
*                IRDA_SCI_ERR_OMITTED_CHAN -
*                    channel not included in config.h
*                IRDA_SCI_ERR_CH_NOT_CLOSED -
*                    channel already in use
*                IRDA_SCI_ERR_BAD_MODE -
*                    unsupported mode
*                IRDA_SCI_ERR_NULL_PTR -
*                    missing required p_cfg argument
*                IRDA_SCI_ERR_INVALID_ARG -
*                    element of casted mode config structure (p_cfg) is invalid
*                IRDA_SCI_ERR_QUEUE_UNAVAILABLE -
*                    cannot open transmit or receive queue or both
******************************************************************************/
irda_sci_err_t R_IRDA_SCI_Open(uint8_t const      chan,
                               irda_sci_t * const  p_cfg,
                               void               (* const p_callback)(void *p_args),
                               irda_sci_hdl_t * const  p_hdl)
{
    irda_sci_err_t  err = IRDA_SCI_SUCCESS;
    bool            chk1, chk2;

    /* CHECK ARGUMENTS */
#if (IRDA_SCI_CFG_PARAM_CHECKING_ENABLE == 1)

    if (chan != IRDA_SCI_CH5)
    {
        return IRDA_SCI_ERR_BAD_CHAN;
    }
    if (g_irda_handles[chan] == NULL)
    {
        return IRDA_SCI_ERR_OMITTED_CHAN;
    }
    if ((p_cfg == NULL) || (p_hdl == NULL) || (p_callback == NULL))
    {
        return IRDA_SCI_ERR_NULL_PTR;
    }
    if (p_cfg->baud_rate == 0)
    {
        return (IRDA_SCI_ERR_INVALID_ARG);
    }
    if (p_cfg->clk_out_width > IRDA_SCI_OUT_WIDTH_128)
    {
        return IRDA_SCI_ERR_INVALID_ARG;
    }
    if ((p_cfg->int_priority < (BSP_MCU_IPL_MIN+1)) || (p_cfg->int_priority > BSP_MCU_IPL_MAX))
    {
        return IRDA_SCI_ERR_INVALID_ARG;
    }

#endif  /* IRDA_SCI_CFG_PARAM_CHECKING_ENABLE */

    /*  ---- SCI HARDWARE LOCK ---- */
    chk1 = R_BSP_HardwareLock(g_irda_handles[chan]->rom->hd_lock);
    chk2 = R_BSP_HardwareLock(g_irda_handles[chan]->rom->hd_lock_irda);
    if ((false == chk1)||(false == chk2))
    {
        /* Lock has already been acquired by another task. Needs to try again later. */
        err = IRDA_SCI_ERR_LOCK_FUNC;
        if ((true == chk1)&&(false == chk2))
        {
            chk1 = R_BSP_HardwareUnlock(g_irda_handles[chan]->rom->hd_lock);
        }
    }
	else
	{
		/* APPLY POWER TO CHANNEL */
		irda_power_setting(g_irda_handles[chan],TARGET_SETTING);

		err = irda_sci_open(chan,p_cfg,p_callback,p_hdl);

		/* set chanel handler pointer */
		*p_hdl = g_irda_handles[chan];

	}
    return (err);
}

/*****************************************************************************
* Function Name: R_IRDA_SCI_Send
* Description  : Sets the transmit data with the channel specified by the
*                handle. If transmit operation is not being performed,
*                the first transmit data is set to the hardware transmit
*                data register. If transmit operation is being performed,
*                the transmit data is stored in the queue.
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
*                p_src -
*                    ptr to data to transmit
*                length - 
*                    number of bytes to send
* Return Value : SCI_SUCCESS -
*                    requested number of bytes sent/loaded into tx queue
*                SCI_ERR_NULL_PTR -
*                    hdl or p_src is NULL
*                SCI_ERR_BAD_MODE -
*                    channel mode not currently supported
*                SCI_ERR_INSUFFICIENT_SPACE - 
*                    not enough space in tx queue to store data (Async)
*                SCI_ERR_XCVR_BUSY -
*                    channel currently busy (SSPI/Sync)
******************************************************************************/
irda_sci_err_t R_IRDA_SCI_Send(irda_sci_hdl_t const  hdl,
                               uint8_t            *p_src,
                               uint16_t const     length)
{
    irda_sci_err_t  err = IRDA_SCI_SUCCESS;
    uint16_t        cnt;

    /* Check arguments */
#if (1 == IRDA_SCI_CFG_PARAM_CHECKING_ENABLE)
    if ((hdl == NULL) || (p_src == NULL))
    {
        return IRDA_SCI_ERR_NULL_PTR;
    }
#endif

    /* Determine amount of space left in tx queue */
    *hdl->rom->icu_txi &= (uint8_t)~hdl->rom->txi_en_mask;
    R_BYTEQ_Unused(hdl->u_tx_data, &cnt);
    *hdl->rom->icu_txi |= hdl->rom->txi_en_mask;

    if (cnt < length)
    {
        /* Free space in the buffer is insufficient */
        err = IRDA_SCI_ERR_INSUFFICIENT_SPACE;
    }
    else
    {
        /* Else load bytes into tx queue for transmission */
        for (cnt=0; cnt < length; cnt++)
        {
            if (hdl->tx_idle == true)
            {
                /* Enable transmit end interrupt */
                *hdl->rom->icu_tei |= hdl->rom->tei_en_mask;
                hdl->rom->regs->SCR.BIT.TEIE = 0;

                /* if buffer is idle, kick off transmit */
                hdl->tx_idle = false;
                hdl->rom->regs->TDR = *p_src++;
            }
            else
            {
                /* if buffer is not idle, load next byte into tx queue (space checked in calling func) */
                *hdl->rom->icu_txi &= (uint8_t)~hdl->rom->txi_en_mask;
                R_BYTEQ_Put(hdl->u_tx_data, *p_src++);
                *hdl->rom->icu_txi |= hdl->rom->txi_en_mask;
            }
        }
        err = IRDA_SCI_SUCCESS;
    }

    return err;
}

/*****************************************************************************
* Function Name: R_IRDA_SCI_Receive
* Description  : The data received with the channel specified by the handle
*                is stored in the queue within the interrupt handling.
*                This function obtains the received data for the specified
*                size from the queue. 
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : SCI_SUCCESS -
*                    requested number of byte loaded into p_dst
*                SCI_ERR_NULL_PTR -
*                    hdl or p_dst is NULL
*                SCI_ERR_BAD_MODE -
*                    channel mode not currently supported
*                SCI_ERR_INSUFFICIENT_DATA -
*                    rx queue does not contain requested amount of data
******************************************************************************/
irda_sci_err_t R_IRDA_SCI_Receive(irda_sci_hdl_t const hdl,
                                  uint8_t         *p_dst,
                                  uint16_t const  length)
{
    irda_sci_err_t   err = IRDA_SCI_SUCCESS;
    uint16_t         cnt;

    /* Check arguments */
#if (1 == IRDA_SCI_CFG_PARAM_CHECKING_ENABLE)
    if ((hdl == NULL) || (p_dst == NULL))
    {
        return IRDA_SCI_ERR_NULL_PTR;
    }
#endif

    /* Obtains the received data size in the queue. */
    *hdl->rom->icu_rxi &= (uint8_t)~hdl->rom->rxi_en_mask;
    R_BYTEQ_Used(hdl->u_rx_data, &cnt);
    *hdl->rom->icu_rxi |= hdl->rom->rxi_en_mask;

    if (cnt < length)
    {
        /* The received data in the queue is less than the specified size. */
        err = IRDA_SCI_ERR_INSUFFICIENT_DATA;
    }
    else
    {
        /* Get specified bytes data from queue */
        for (cnt=0; cnt < length; cnt++)
        {
            *hdl->rom->icu_rxi &= (uint8_t)~hdl->rom->rxi_en_mask;
            R_BYTEQ_Get(hdl->u_rx_data, p_dst++);
            *hdl->rom->icu_rxi |= hdl->rom->rxi_en_mask;
        }
        err = IRDA_SCI_SUCCESS;
    }
    return err;
}

/*****************************************************************************
* Function Name: irda_txi_handler
* Description  : TXI interrupt handler for SCI
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
void irda_txi_handler(irda_sci_hdl_t const hdl)
{
    byteq_err_t     ret;
    uint8_t         byte;

    /* Get byte from que and place in TDR for transmit */
    ret = R_BYTEQ_Get(hdl->u_tx_data, (uint8_t *)&byte);

    /* Next transmit data is not present in the transmit queue. */
    if (BYTEQ_SUCCESS != ret)
    {
       hdl->tx_idle = true;    /* set flag if queue empty */

       /* Transmit End Interrupt enable */
       *hdl->rom->icu_tei |= (uint8_t)hdl->rom->tei_en_mask;
       hdl->rom->regs->SCR.BIT.TEIE = 1;
    }
    else
    {
       hdl->rom->regs->TDR = byte;
    }

}

/*****************************************************************************
* Function Name: irda_tei_handler
* Description  : TEI interrupt handler for SCI
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
void irda_tei_handler(irda_sci_hdl_t const hdl)
{
    irda_sci_cb_args_t   args;

    /* Clear Transmit End Interrupt */
    *hdl->rom->icu_tei &= (uint8_t)~hdl->rom->tei_en_mask;

    /* Transmit End Interrupt Disable */
    hdl->rom->regs->SCR.BIT.TEIE = 0;

    /* Do callback for transmit interrupt */
    if ((hdl->callback != NULL) && (hdl->callback != FIT_NO_FUNC))
    {
        args.hdl = hdl;
        args.event = IRDA_SCI_EVT_TEI;

        /* Call the callback function that has been set */
        hdl->callback((void *)&args);
    }
}

/*****************************************************************************
* Function Name: irda_rxi_handler
* Description  : RXI interrupt handler for SCI
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
void irda_rxi_handler(irda_sci_hdl_t const hdl)
{
    irda_sci_cb_args_t   args;
    byteq_err_t          ret;

    /* Read byte */
    uint8_t              byte = hdl->rom->regs->RDR;

    /* Stores the received data in the queue. */
    ret = R_BYTEQ_Put(hdl->u_rx_data, byte);

    /* Do callback for receive interrupt */
    if ((hdl->callback != NULL) && (hdl->callback != FIT_NO_FUNC))
    {
        /* The received data has been stored in the queue. */
        if (BYTEQ_SUCCESS == ret)
        {
            args.event = IRDA_SCI_EVT_RX_CHAR;
        }
        /* There is not enough space to store the data. */
        else
        {
            args.event = IRDA_SCI_EVT_RXBUF_OVFL;
        }

        args.hdl = hdl;
        args.byte = byte;

        /* Call the callback function that has been set */
        hdl->callback((void *)&args);
    }
}

/*****************************************************************************
* Function Name: irda_eri_handler
* Description  : ERI interrupt handler for SCI
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
void irda_eri_handler(irda_sci_hdl_t const hdl)
{
    irda_sci_cb_args_t   args;

    /* Read the receive data */
    uint8_t              byte = hdl->rom->regs->RDR;

    /* Do callback for error interrupt */
    if ((hdl->callback != NULL) && (hdl->callback != FIT_NO_FUNC))
    {
        args.hdl = hdl;
        args.byte = byte;

        /* Overrun error has occurred */
        if (hdl->rom->regs->SSR.BIT.ORER == 1)
        {
            args.event = IRDA_SCI_EVT_OVFL_ERR;
        }
        /* Framing error has occurred */
        else if (hdl->rom->regs->SSR.BIT.FER == 1)
        {
            args.event = IRDA_SCI_EVT_FRAMING_ERR;
        }

        /* Call the callback function that has been set */
        hdl->callback((void *)&args);
    }

    /* Clear the ORER, FER, and PER bits */
    hdl->rom->regs->SSR.BYTE &= ~IRDA_SCI_RCVR_ERR_MASK;

    /* In overrun error release, Keep reading the RDR during overrun error clear. */
    while ((hdl->rom->regs->SSR.BYTE & IRDA_SCI_RCVR_ERR_MASK) != 0)
    {
        byte = hdl->rom->regs->RDR;
        hdl->rom->regs->SSR.BYTE &= ~IRDA_SCI_RCVR_ERR_MASK;
    }

}

/*****************************************************************************
* Function Name: R_IRDA_SCI_Control
* Description  : This function performs the control of the transmission and 
*                Receive queue.
*
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
*                cmd -
*                    command to run
*                p_args -
*                    pointer argument(s) specific to command
* Return Value : IRDA_SCI_SUCCESS -
*                    Command completed successfully.
*                IRDA_SCI_ERR_NULL_PTR -
*                    hdl or p_args is NULL
*                IRDA_SCI_ERR_INVALID_ARG -
*                    The cmd value or p_args contains an invalid value.
*                    May be due to mode channel is operating in.
******************************************************************************/
irda_sci_err_t R_IRDA_SCI_Control(irda_sci_hdl_t const     hdl,
                                  irda_sci_cmd_t const     cmd,
                                  void                    *p_args)
{
    irda_sci_err_t   err = IRDA_SCI_SUCCESS;

    /* CHECK ARGUMENTS */
#if (1 == IRDA_SCI_CFG_PARAM_CHECKING_ENABLE)
    if (NULL == hdl)
    {
        return IRDA_SCI_ERR_NULL_PTR;
    }
    if ((NULL == p_args)
     && ((IRDA_SCI_CMD_TX_Q_BYTES_FREE == cmd) || (IRDA_SCI_CMD_RX_Q_BYTES_AVAIL_TO_READ == cmd)))
    {
        return IRDA_SCI_ERR_NULL_PTR;
    }
#endif

    /* Command control */
    switch(cmd)
    {

    /* flush the Transmit queue */
    case (IRDA_SCI_CMD_TX_Q_FLUSH):
        *hdl->rom->icu_txi &= (uint8_t)~hdl->rom->txi_en_mask;
        R_BYTEQ_Flush(hdl->u_tx_data);
        *hdl->rom->icu_txi |= hdl->rom->txi_en_mask;
    break;

    /* flush the Receive queue */
    case (IRDA_SCI_CMD_RX_Q_FLUSH):
        *hdl->rom->icu_rxi &= (uint8_t)~hdl->rom->rxi_en_mask;
        R_BYTEQ_Flush(hdl->u_rx_data);
        *hdl->rom->icu_rxi |= hdl->rom->rxi_en_mask;
    break;

    /* Checks the size of space in the transmit queue. */
    case (IRDA_SCI_CMD_TX_Q_BYTES_FREE):
        R_BYTEQ_Unused(hdl->u_tx_data, (uint16_t *) p_args);
    break;

    /* Checks the size of the data stored in the receive queue. */
    case (IRDA_SCI_CMD_RX_Q_BYTES_AVAIL_TO_READ):
        R_BYTEQ_Used(hdl->u_rx_data, (uint16_t *) p_args);
    break;

    /* other */
    default:
        err = IRDA_SCI_ERR_INVALID_ARG;
    break;
    }

    return err;
}

/*****************************************************************************
* Function Name: R_IRDA_SCI_Close
* Description  : Initialize the channel used in the IrDA to make it back to
*                default.
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : IRDA_SCI_SUCCESS -
*                    channel closed
*                IRDA_SCI_ERR_NULL_PTR -
*                    hdl was NULL
*                IRDA_SCI_ERR_LOCK_FUNC -
*                    Tried to close the channel, but detected a hardware error.
******************************************************************************/
irda_sci_err_t R_IRDA_SCI_Close(irda_sci_hdl_t const hdl)
{
    bool            chk1,chk2;

    /* CHECK ARGUMENTS */
#if (1 == IRDA_SCI_CFG_PARAM_CHECKING_ENABLE)
    if (hdl == NULL)
    {
        return IRDA_SCI_ERR_NULL_PTR;
    }
#endif

    /* Initializes the channel used in the IrDA.  */
    irda_sci_close(hdl);

    /* Initialization of the queue of the sending and receiving */
    R_BYTEQ_Close(hdl->u_tx_data);
    R_BYTEQ_Close(hdl->u_rx_data);

    /* Stops operation of the specified module. */
    irda_power_setting(hdl,TARGET_STOP);

    /* ---- Release hardware locks for the SCI and the IrDA. ---- */
    chk1 = R_BSP_HardwareUnlock(hdl->rom->hd_lock);
    chk2 = R_BSP_HardwareUnlock(hdl->rom->hd_lock_irda);

    /*Verifies the result of two hardware unlock operations. */
    if ((false == chk1)|| (false == chk2))
    {
        return (IRDA_SCI_ERR_LOCK_FUNC);
    }

    return (IRDA_SCI_SUCCESS);
}


/*****************************************************************************
* Function Name: R_IRDA_SCI_GetVersion
* Description  : Returns the version of this module. The version number is 
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
******************************************************************************/
#pragma inline(R_IRDA_SCI_GetVersion)
uint32_t  R_IRDA_SCI_GetVersion(void)
{
uint32_t const version = (IRDA_SCI_VERSION_MAJOR << 16) | IRDA_SCI_VERSION_MINOR;

    return version;
}

/* End of File */
