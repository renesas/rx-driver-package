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
* File Name    : r_irda_sci_rx113.c
* Version      : 1.01
* Device       : RX
* Abstract     : 
* Tool-Chain   : Renesas RXC Toolchain v2.01.00
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
#include "./../r_irda_sci_rx_private.h"
#include "r_irda_sci_rx113_private.h"

/* Defines for IRDA support */
#include "r_irda_sci_rx_if.h"
#include "r_irda_sci_rx_config.h"

#include "r_byteq_if.h"

/*****************************************************************************
Macro definitions
******************************************************************************/

/*****************************************************************************
  Private variables and functions
******************************************************************************/
/*------- Data and information for each channel -------*/
#if IRDA_SCI_CFG_CH5_INCLUDED

/* Queue buffers */
uint8_t         irda_ch5_tx_buf[IRDA_SCI_CFG_CH5_TX_BUFSIZ];
uint8_t         irda_ch5_rx_buf[IRDA_SCI_CFG_CH5_RX_BUFSIZ];

/* Addresses and setting values for I/O registers, etc. */
//
const irda_sci_ch_rom_t  irda_ch5_rom =
{
    /* Hardware locks for the SCI and the IrDA in the BSP module */
    BSP_LOCK_SCI5,
    BSP_LOCK_IRDA,
    /* Structure for I/O registers in the SCI and IrDA modules */
    (volatile __evenaccess struct st_sci12 *)&SCI5,
    (volatile __evenaccess struct st_irda *)&IRDA,
    /* Module stop registers and bits in the registers */
    &SYSTEM.MSTPCRB.LONG,
    &SYSTEM.MSTPCRC.LONG,
    BIT26_MASK,
    BIT20_MASK,
    /* Interrupt Register */
    &ICU.IPR[IPR_SCI5_RXI5].BYTE,
    &ICU.IR[IR_SCI5_RXI5].BYTE,
    &ICU.IR[IR_SCI5_TXI5].BYTE,
    &ICU.IR[IR_SCI5_TEI5].BYTE,
    &ICU.IR[IR_SCI5_ERI5].BYTE,
    &ICU.IER[IER_SCI5_RXI5].BYTE,
    &ICU.IER[IER_SCI5_TXI5].BYTE,
    &ICU.IER[IER_SCI5_TEI5].BYTE,
    &ICU.IER[IER_SCI5_ERI5].BYTE,
    BIT7_MASK,
    BIT0_MASK,
    BIT1_MASK,
    BIT6_MASK
};

/* Port setting values for pins IRTXD and IRRXD */
const irda_sci_ch_port_rom_t g_irda_sci_ch5_portrom =
{
    /* IRTXD Pin Select */
    IRDA_SCI_CFG_CH5_IRTXD_GP,              /* 0: Port group of IRTXD pin */
    IRDA_SCI_CFG_CH5_IRTXD_PIN,             /* 1: Port bit of IRTXD pin */
    1,                                      /* 2: Pin direction control (IN/OUT) when not selecting the IRTXD pin */
    IRDA_SCI_CFG_CH5_IRTXD_INACTIVE_LEVEL,  /* 3: Pin level when  not selecting the IRTXD pin */
    IRDA_SCI_MPC_IRTXD5_ENABLE,             /* 4: MPC setting data of IRTXD pin */

    /* IRTXD Pin Select */
    IRDA_SCI_CFG_CH5_IRRXD_GP,              /* 0: Port group of IRRXD pin */
    IRDA_SCI_CFG_CH5_IRRXD_PIN,             /* 1: Port bit of IRRXD pin */
    0,                                      /* 2: Pin direction control (IN/OUT) when not selecting the IRRXD pin */
    IRDA_SCI_CFG_CH5_IRRXD_INACTIVE_LEVEL,  /* 3: Pin level when  not selecting the IRRXD pin */
    IRDA_SCI_MPC_IRRXD5_ENABLE              /* 4: MPC setting data of IRRXD pin */
};

/* Channel control handle block */
irda_sci_ch_ctrl_t   irda_ch5_ctrl =
{
    &irda_ch5_rom,                       /* rom info pointer          */
    &g_irda_sci_ch5_portrom,        /* port info pointer         */
    NULL,                           /* Callback function pointer */
    NULL,                           /* Transmit queue handle     */
    NULL,                           /* Receive queue handle      */
    true                            /* Transmit status           */
};
#endif

/*------- Information table for all SCI channels -------*/
/* Handles array */
const irda_sci_hdl_t g_irda_handles[IRDA_SCI_NUM_CH] =
{
            NULL,           /* ch0  */
            NULL,           /* ch1  */
            NULL,           /* ch2  */
            NULL,           /* ch3  */
            NULL,           /* ch4  */
#if IRDA_SCI_CFG_CH5_INCLUDED
            &irda_ch5_ctrl,      /* ch5  */
#else
            NULL,
#endif
            NULL,           /* ch6  */
            NULL,           /* ch7  */
            NULL,           /* ch8  */
            NULL,           /* ch9  */
            NULL,           /* ch10 */
            NULL,           /* ch11 */
            NULL            /* ch12 */
};

/*------- Information table for all SCI channels -------*/
/* ��Boud-Rate divisor info               */
/* BRR = (PCLK/(divisor * baud)) - 1    */
/* abcs=0, divisor = 64*pow(2,2n-1)     */

#define NUM_DIVISORS  4
const baud_divisor_t irda_async_baud[NUM_DIVISORS]=
{
    {32,   0},   /* divisor result, n */
    {128,  1},
    {512,  2},
    {2048, 3}
};

const out_width_divisor_t irda_ircks_div[8] =
{
    {16 ,3},        /* div B * 3/16 */
    {2  ,1},        /* div P /2     */
    {4  ,1},        /* div P /4     */
    {8  ,1},        /* div P /8     */
    {16 ,1},        /* div P /16    */
    {32 ,1},        /* div P /32    */
    {64 ,1},        /* div P /64    */
    {128,1}         /* div P /128   */
};

/*------- Global functions -------*/
void                  irda_power_setting(irda_sci_hdl_t const hdl, bool init_or_setting);
irda_sci_err_t        irda_sci_open(uint8_t const           chan,
                                    irda_sci_t * const      p_cfg,
                                    void(* const p_callback)(void *p_args),
                                    irda_sci_hdl_t * const  p_hdl);
void                  irda_sci_close(irda_sci_hdl_t const hdl);

/*------- Private functions -------*/
static irda_sci_err_t irda_sci_init_queues          (uint8_t const chan);
static void           irda_sci_clear_io_register_sci(irda_sci_hdl_t const hdl);
static void           irda_sci_io_setting           (uint8_t* port_sel);
static void           irda_sci_io_mode_setting      (uint8_t* port_sel, bool init_or_setting);
static void           select_ints                   (uint32_t* ints_sel, bool init_or_setting);
static irda_sci_err_t irda_sci_init_bit_rate        (irda_sci_hdl_t const    hdl,
                                                     uint32_t const     pclk,
                                                     irda_sci_t * const  p_cfg);

/*****************************************************************************
* Function Name: irda_power_setting
* Description  : Starts or stops operation of the specified module.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                init_or_setting -
*                    Information of the selected operation for 
*                    the module (start/stop)
* Return Value : none
******************************************************************************/
void irda_power_setting(irda_sci_hdl_t const hdl, bool init_or_setting)
{
    volatile  __evenaccess uint32_t *mstp_reg = hdl->rom->mstp;

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    if (TARGET_SETTING == init_or_setting)
    {
        *mstp_reg &= ~hdl->rom->stop_mask;
         mstp_reg++;
        *mstp_reg &= ~hdl->rom->stop_mask_irda;
    }
    else
    {
        *mstp_reg |= hdl->rom->stop_mask;
         mstp_reg++;
        *mstp_reg |= hdl->rom->stop_mask_irda;
    }
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    return;
}

/*****************************************************************************
* Function Name: irda_sci_open
* Description  : Initialization for using the IRDA
*
* NOTE: The associated port must be configured/initialized prior to
*       calling this function.
*
* Arguments    :  irda_sci_ch_ctrl_t -
*                    handle for channel (pointer to channel control block)
* Return Value : IRDA_SCI_SUCCESS -
*                    channel opened successfully
******************************************************************************/
irda_sci_err_t irda_sci_open(uint8_t const      chan,
                             irda_sci_t * const  p_cfg,
                             void               (* const p_callback)(void *p_args),
                             irda_sci_hdl_t * const  p_hdl)
{
    irda_sci_err_t  err = IRDA_SCI_SUCCESS;
    volatile __evenaccess const irda_sci_ch_rom_t * p_prom = g_irda_handles[chan]->rom;

    /* I/O port setting (Hi-Z -> Set the state it was in when you release the port) */
    irda_sci_io_setting((uint8_t*)&(g_irda_handles[chan]->port_rom->irrxd_port_gr));
    irda_sci_io_setting((uint8_t*)&(g_irda_handles[chan]->port_rom->irtxd_port_gr));

    /* Initialize transmit status flag */
    g_irda_handles[chan]->tx_idle = true;

    /* Set data polarity and Serial I/O pins are used for IrDA data communication. */
    p_prom->regs_irda->IRCR.BYTE = (uint8_t)(0x80 | (IRDA_SCI_CFG_CH5_IRTXD_INACTIVE_LEVEL << 3) |
                          (IRDA_SCI_CFG_CH5_IRRXD_INACTIVE_LEVEL <<2 ));

    /* Configure SMR for asynchronous mode, single processor, 8bit , Non-parity, and 2 stop-bit */
    p_prom->regs->SMR.BYTE = (uint8_t)0x08;

    /* Setup clock for Baud-Rate */
    err = irda_sci_init_bit_rate(g_irda_handles[chan], BSP_PCLKB_HZ, p_cfg);
    if (IRDA_SCI_SUCCESS != err)
    {
        return err;
    }

    /* Set Interrupt Source Priority for ERI, RXI, TXI, and TEI. */
    *p_prom->ipr = p_cfg->int_priority;

    /* Set the Callback function to be called when ERI, RXI, and TEI interrupt occurs */
    g_irda_handles[chan]->callback = p_callback;

    /* Configuration and initialization of the queue of the sending and receiving */
    err = irda_sci_init_queues(chan);
    if (err != IRDA_SCI_SUCCESS)
    {
        return err;
    }

    /* Transmit Enable */
    p_prom->regs->SCR.BIT.TE = 1;

    /* Set IRRXD mode of the I/O port */
    irda_sci_io_mode_setting((uint8_t*)&(g_irda_handles[chan]->port_rom->irrxd_port_gr),TARGET_SETTING);
    
    /* wait over[18 / (16 * bit-rate)] */
    R_BSP_SoftwareDelay(((1000000/p_cfg->baud_rate) * 18 / 16),BSP_DELAY_MICROSECS);

    p_prom->regs_irda->IRCR.BYTE = (uint8_t)(0x80 | (p_cfg->clk_out_width<<4) |
                     (IRDA_SCI_CFG_CH5_IRTXD_INACTIVE_LEVEL << 3) | (IRDA_SCI_CFG_CH5_IRRXD_INACTIVE_LEVEL <<2 ));

    /* Receive Interrupt Enable */
    select_ints((uint32_t*)&(p_prom->ir_rxi),TARGET_SETTING);
    select_ints((uint32_t*)&(p_prom->ir_eri),TARGET_SETTING);
    p_prom->regs->SCR.BIT.RIE = 1;           /* Receive Interrupt Enable */

    /* Receive Enable */
    p_prom->regs->SCR.BIT.RE = 1;

    /* Set IRTXD mode of the I/O port */
    irda_sci_io_mode_setting((uint8_t*)&(g_irda_handles[chan]->port_rom->irtxd_port_gr),TARGET_SETTING);

    /* Transmit Interrupt Enable */
    select_ints((uint32_t*)&(p_prom->ir_txi),TARGET_SETTING);
    p_prom->regs->SCR.BIT.TIE = 1;
    /* The TEI interrupt is enabled if the transmit queue is empty when the TXI interrupt occurs, */
    /* and the TEI interrupt is disabled in the TEI interrupt handling.  */
    return (err);
}

/*****************************************************************************
* Function Name: irda_sci_close
* Description  : Stops the IRDA function. This processing includes the 
*                initialization for SCI chX used.
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : None
******************************************************************************/
void irda_sci_close(irda_sci_hdl_t const hdl)
{

    volatile __evenaccess const irda_sci_ch_rom_t * p_prom = hdl->rom;

    /* I/O port Refresh (Set the state it was in when you release the port) */
    irda_sci_io_setting((uint8_t*)&(hdl->port_rom->irtxd_port_gr));
    irda_sci_io_setting((uint8_t*)&(hdl->port_rom->irrxd_port_gr));

    /* Set IN/OUT Port mode of the I/O port */
    irda_sci_io_mode_setting((uint8_t*)&(hdl->port_rom->irtxd_port_gr),TARGET_STOP);

    /* Transmit Interrupt Disable  & Transmit End Interrupt Disable */
    p_prom->regs->SCR.BIT.TEIE = 0;
    p_prom->regs->SCR.BIT.TIE = 0;
    select_ints((uint32_t*)&(p_prom->ir_tei),TARGET_STOP);
    select_ints((uint32_t*)&(p_prom->ir_txi),TARGET_STOP);

    /* Receive Disable */
    p_prom->regs->SCR.BIT.RE = 0;

    /* Receive Interrupt Disable */
    p_prom->regs->SCR.BIT.RIE = 0;
    select_ints((uint32_t*)&(p_prom->ir_rxi),TARGET_STOP);
    select_ints((uint32_t*)&(p_prom->ir_eri),TARGET_STOP);

    /* Initialize IrDA Clock Select*/
    p_prom->regs_irda->IRCR.BYTE &= 0x8F;

    /* Set IN/OUT Port mode of the I/O port */
    irda_sci_io_mode_setting((uint8_t*)&(hdl->port_rom->irrxd_port_gr),TARGET_STOP);

    /* Transmit Disable */
    p_prom->regs->SCR.BIT.TE = 0;

    /* Initialization of SCI Register */
    irda_sci_clear_io_register_sci(hdl);

    /* Clear Interrupt flags */
    *p_prom->ir_rxi = 0;
    *p_prom->ir_eri = 0;
    *p_prom->ir_txi = 0;
    *p_prom->ir_tei = 0;

    /* Clear Interrupt Source Priority for ERI, RXI, TXI, and TEI. */
    *p_prom->ipr = 0;

    /* Serial I/O pins are used for SCI. */
    p_prom->regs_irda->IRCR.BYTE = 0x00;

}

/*****************************************************************************
* Function Name: sci_enable_ints
* Description  : Clears the specified interrupt request flag and enables or 
*                disables the interrupt request.
*                interrupts.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                priority -
*                    priority for interrupts
* Return Value : none
******************************************************************************/
static void select_ints(uint32_t* ints_sel, bool init_or_setting)
{
    volatile  __evenaccess uint32_t  * select_ir;
    volatile  __evenaccess uint32_t  * select_ien;
    uint8_t select_ien_mask = *(ints_sel+8);

    select_ir = (volatile  __evenaccess uint32_t*)*ints_sel;
    select_ien = (volatile  __evenaccess uint32_t*)*(ints_sel+4);

    if (TARGET_SETTING == init_or_setting)
    {
        /* Clear Interrupt Request flag */
        (uint8_t)(*select_ir) = 0;
        /* Notifies the CPU of the interrupt request. */
        (uint8_t)(*select_ien) |= (uint8_t)select_ien_mask;
    }
    else
    {
        /* Clear Interrupt Request flag */
        (*select_ir) = 0;
        /* Not notify the CPU of the interrupt request. */
        (uint8_t)(*select_ien) &= (uint8_t)~ select_ien_mask;
    }
}

/*****************************************************************************
* Outline      : Sets the state when the specified pin is not active state.
* Function Name: irda_sci_mpc_setting
* Description  : Set IRDA_SCI function pin controller.
* Arguments    : uint8_t* port_sel ; Port select infomation
* Return Value : None
******************************************************************************/
static void irda_sci_io_setting(uint8_t* port_sel)
{
    volatile uint8_t __evenaccess * const p_pdr  = ((&PORT0.PDR.BYTE) + *(port_sel));
    volatile uint8_t __evenaccess * const p_podr = ((&PORT0.PDR.BYTE) + 0x20 + *(port_sel));

    (*p_podr) |= ((*(port_sel+3) << *(port_sel+1)));        /* Port holds output data. */
    (*p_pdr)  |= ((*(port_sel+2) << *(port_sel+1)));        /* Port Direction */

}

/************************************************************************************
* Outline      : Switches the specified pin mode.
* Function Name: irda_sci_io_mode_setting
* Description  : Set IRDA_SCI function pin controller.
* Arguments    : uint8_t* port_sel ; Port select infomation
*              : init_or_setting   : Specifies the mode for the pin to be selected.
* Return Value : None
************************************************************************************/
static void irda_sci_io_mode_setting(uint8_t* port_sel, bool init_or_setting)
{
    volatile uint8_t __evenaccess * const p_pmr  = ((&PORT0.PDR.BYTE) + 0x60 + *(port_sel));
    volatile uint8_t __evenaccess * const ppfs = ((((&MPC.PWPR.BYTE) + 0x21 + (*(port_sel) * 8)) + *(port_sel+1)));

    if (TARGET_SETTING == init_or_setting)
    {
        /* Select Port Function Control */
        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);  /* Enables the PFS register writing.        */
        (*ppfs) = *(port_sel+4);
        R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);   /* Disables the PFS register writing.       */

        /* Control Port Mode (Functions) */
        (*p_pmr)  |= ((1U << *(port_sel+1)));
        /*Reads the same I/O register (since the Delay function may be called in next processing).  */
        if ((*p_pmr & (1U << *(port_sel+1))) == 1)
        {
            nop();
        }
    }
    else
    {
        /* Select Port Function Control */
        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);  /* Enables the PFS register writing.        */
        (*ppfs) = IRDA_SCI_MPC_IRTXD_DISABLE;
        R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);   /* Disables the PFS register writing.       */

        /* Control Port Mode (Port) */
        (*p_pmr)  &= (~(1U << *(port_sel+1)));
    }

}

/************************************************************************************
* Outline      : Reset SCI I/O register
* Function Name: irda_sci_clear_io_register
* Description  : Reset SCI I/O register.
* Arguments    : irda_sci_ch_ctrl_t const hdl     ;   IrDA handler
* Return Value : None
************************************************************************************/
static void irda_sci_clear_io_register_sci(irda_sci_hdl_t const hdl)
{
    volatile  __evenaccess struct st_sci12 * p_prom = hdl->rom->regs;

    /* Initializes SCI I/O register */
    p_prom->SMR.BYTE     = IRDA_SCI_SMR_INIT;
    p_prom->SCR.BYTE     = IRDA_SCI_SCR_INIT;
    p_prom->SCMR.BYTE    = IRDA_SCI_SCMR_INIT;
    p_prom->BRR          = IRDA_SCI_BRR_INIT;
    p_prom->SEMR.BYTE    = IRDA_SCI_SEMR_INIT;
    p_prom->SNFR.BYTE    = IRDA_SCI_SNFR_INIT;
    p_prom->SIMR1.BYTE   = IRDA_SCI_SIMR1_INIT;
    p_prom->SIMR2.BYTE   = IRDA_SCI_SIMR2_INIT;
    p_prom->SIMR3.BYTE   = IRDA_SCI_SIMR3_INIT;
    p_prom->SISR.BYTE    = IRDA_SCI_SISR_INIT;
    p_prom->SPMR.BYTE    = IRDA_SCI_SPMR_INIT;
}

/*****************************************************************************
* Function Name: sci_init_queues
* Description  : Configuration and initialization of the queue of the sending
*                and receiving
* Arguments    : chan -
*                    channel (ptr to chan control block)
* Return Value : SCI_SUCCESS -
*                    channel initialized successfully
*                SCI_ERR_QUEUE_UNAVAILABLE -
*                    no queue control blocks available
******************************************************************************/
static irda_sci_err_t irda_sci_init_queues(uint8_t const  chan)
{
    byteq_err_t      ret1 = BYTEQ_ERR_INVALID_ARG;
    byteq_err_t      ret2 = BYTEQ_ERR_INVALID_ARG;
    irda_sci_err_t   err    = IRDA_SCI_SUCCESS;

    /* channel number verified as legal prior to calling this function */
    switch (chan)
    {
#if IRDA_SCI_CFG_CH5_INCLUDED
    case (IRDA_SCI_CH5):
        ret1 = R_BYTEQ_Open(irda_ch5_tx_buf, IRDA_SCI_CFG_CH5_TX_BUFSIZ, &g_irda_handles[IRDA_SCI_CH5]->u_tx_data);
        ret2 = R_BYTEQ_Open(irda_ch5_rx_buf, IRDA_SCI_CFG_CH5_RX_BUFSIZ, &g_irda_handles[IRDA_SCI_CH5]->u_rx_data);
        break;
#endif
    }

    if ((ret1 != BYTEQ_SUCCESS) || (ret2 != BYTEQ_SUCCESS))
    {
        err = IRDA_SCI_ERR_QUEUE_UNAVAILABLE;
    }

    return err;
}

/*****************************************************************************
* Function Name: irda_sci_init_bit_rate
* Description  :Configures settings based on the specified peripheral clock
*               and communication rate. Verifies if the high pulse width
*               is within the available range. 
* Arguments    : hdl -
*                    Handle for channel (ptr to chan control block)
*                pclk -
*                    Peripheral clock speed; e.g. 24000000 for 24MHz
*                baud -
*                    Baud rate; 19200, 57600, 115200, etc.
* Return Value : IRDA_SCI_SUCCESS -
*                    Baud-Rate was correctly set
*                IRDA_SCI_ERR_INVALID_ARG -
*                    Paramater is Invalid value or The baud rate is not set correctly
******************************************************************************/
static irda_sci_err_t irda_sci_init_bit_rate(irda_sci_hdl_t const  hdl,
                                             uint32_t const pclk,
                                             irda_sci_t * const  p_cfg)
{
    uint32_t i;
    uint32_t num_divisors = NUM_DIVISORS;
    uint32_t ratio;
    uint32_t tmp;
    uint32_t irda_tmp1;
    uint32_t irda_tmp2;
    uint32_t baud = p_cfg->baud_rate;
    irda_sci_err_t  err = IRDA_SCI_SUCCESS;

    baud_divisor_t const *p_baud_info = irda_async_baud;

#if (SCI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((pclk == 0) || (baud == 0))
    {
        return (IRDA_SCI_ERR_INVALID_ARG);
    }
#endif

    /* FIND the divisor ; CKS and ABCS Value is associated in the table. */
    /* BRR must be 255 or less                                */
    /* the "- 1" is ignored in some steps for approximations  */
    /* BRR = (PCLK/(divisor * baud)) - 1                      */
    /* BRR = (ratio / divisor) - 1                            */

    ratio = pclk/baud;
    for(i=0; i < num_divisors; i++)
    {
        if (ratio < (uint32_t)(p_baud_info[i].divisor * 256))
        {
            break;
        }
    }
    if (i == num_divisors)
    {
        return (IRDA_SCI_ERR_INVALID_ARG);
    }
    

    /* Adjusts the value set to the BRR register by rounding and carry. */
    /* divide ratio by only half the divisor and see if odd number */
    tmp = ratio/((uint32_t)p_baud_info[i].divisor/2);
    /* if odd, "round up" by ignoring -1; divide by 2 again for rest of divisor */
    if (0x01 == (tmp & 0x01))
    {
        tmp = (uint8_t)tmp/2;
    }
    else
    {
        tmp = (uint8_t)(tmp/2)-1;
    }

    /* Verifies whether the high pulse width, which is specified with the output pulse width,*/
    /* is within the available range for communiaction.                                      */
    if (IRDA_SCI_OUT_WIDTH_3_16 != p_cfg->clk_out_width)
    {
        irda_tmp1 = (pclk / 1000 / irda_ircks_div[p_cfg->clk_out_width].w_denominator);
        irda_tmp2 = baud / (0.2125 * 1000);
        if((709 <= irda_tmp1)||(irda_tmp1 <= irda_tmp2))
        {
            return (IRDA_SCI_ERR_INVALID_ARG);
        }
    }

    /* Set CSK,ABCS bit and BRR register */
    hdl->rom->regs->BRR = (uint8_t)tmp;
    hdl->rom->regs->SEMR.BIT.ABCS = 0;
    hdl->rom->regs->SMR.BIT.CKS = p_baud_info[i].cks;

    return (err);
}

/*****************************************************************************
* Function Name: sci5_txi5_isr
* Description  : TXI interrupt routine
* Arguments    : none
* Return Value : none
******************************************************************************/
#if IRDA_SCI_CFG_CH5_INCLUDED
#pragma interrupt sci5_txi5_isr(vect=VECT(SCI5,TXI5))
static void sci5_txi5_isr(void)
{
    irda_txi_handler(&irda_ch5_ctrl);
}
#endif

/*****************************************************************************
* Function Name: IRDA_SCI_CFG_CH5_INCLUDED
* Description  : TEI interrupt routine
* Arguments    : none
* Return Value : none
******************************************************************************/
#if IRDA_SCI_CFG_CH5_INCLUDED
#pragma interrupt sci5_tei5_isr(vect=VECT(SCI5,TEI5))
static void sci5_tei5_isr(void)
{
    irda_tei_handler(&irda_ch5_ctrl);
}
#endif


/*****************************************************************************
* Function Name: sci5_rxi5_isr
* Description  : RXI interrupt routine
* Arguments    : none
* Return Value : none
******************************************************************************/
#if IRDA_SCI_CFG_CH5_INCLUDED
#pragma interrupt sci5_rxi5_isr(vect=VECT(SCI5,RXI5))
static void sci5_rxi5_isr(void)
{
    irda_rxi_handler(&irda_ch5_ctrl);
}
#endif

/*****************************************************************************
* Function Name: sci5_eri5_isr
* Description  : ERI interrupt routine
* Arguments    : none
* Return Value : none
******************************************************************************/
#if IRDA_SCI_CFG_CH5_INCLUDED
#pragma interrupt sci5_eri5_isr(vect=VECT(SCI5,ERI5))
static void sci5_eri5_isr(void)
{
    irda_eri_handler(&irda_ch5_ctrl);
}
#endif

/* End of File */
