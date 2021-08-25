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
/**********************************************************************************************************************
* File Name    : r_sci_rx.c
* Description  : Functions for using SCI on the RX230 device.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           26.05.2016 1.00    Initial Release.
*           19.12.2016 1.10    FIT_NO_PTR check added to NULL check.
*                              SCI_CMD_EN_TEI was changed to ineffective, because it is meaningless command.
*           20.05.2019 3.00    Added support for GNUC and ICCRX.
*           15.08.2019 3.20    Fixed warnings in IAR.
*           25.08.2020 3.60    Added feature using DTC/DMAC in SCI transfer.
*                              Merged IrDA functionality to SCI FIT.
*           31.03.2021 3.80    Added support circular buffer in mode asynchronous.
***********************************************************************************************************************/

/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "platform.h"

#include "r_sci_rx230_private.h"

/*****************************************************************************
Typedef definitions
******************************************************************************/

/*****************************************************************************
Macro definitions
******************************************************************************/

/*****************************************************************************
Private global variables and functions
******************************************************************************/
#if SCI_CFG_CH0_INCLUDED
R_BSP_PRAGMA_STATIC_INTERRUPT(sci0_txi0_isr, VECT(SCI0,TXI0))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci0_txi0_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci0_rxi0_isr, VECT(SCI0,RXI0))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci0_rxi0_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci0_tei0_isr, VECT(SCI0,TEI0))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci0_tei0_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci0_eri0_isr, VECT(SCI0,ERI0))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci0_eri0_isr(void);

#endif

#if SCI_CFG_CH1_INCLUDED
R_BSP_PRAGMA_STATIC_INTERRUPT(sci1_txi1_isr, VECT(SCI1,TXI1))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci1_txi1_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci1_rxi1_isr, VECT(SCI1,RXI1))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci1_rxi1_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci1_tei1_isr, VECT(SCI1,TEI1))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci1_tei1_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci1_eri1_isr, VECT(SCI1,ERI1))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci1_eri1_isr(void);

#endif

#if SCI_CFG_CH5_INCLUDED
R_BSP_PRAGMA_STATIC_INTERRUPT(sci5_txi5_isr, VECT(SCI5,TXI5))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci5_txi5_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci5_rxi5_isr, VECT(SCI5,RXI5))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci5_rxi5_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci5_tei5_isr, VECT(SCI5,TEI5))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci5_tei5_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci5_eri5_isr, VECT(SCI5,ERI5))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci5_eri5_isr(void);


#endif

#if SCI_CFG_CH6_INCLUDED
R_BSP_PRAGMA_STATIC_INTERRUPT(sci6_txi6_isr, VECT(SCI6,TXI6))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci6_txi6_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci6_rxi6_isr, VECT(SCI6,RXI6))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci6_rxi6_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci6_tei6_isr, VECT(SCI6,TEI6))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci6_tei6_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci6_eri6_isr, VECT(SCI6,ERI6))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci6_eri6_isr(void);

#endif

#if SCI_CFG_CH8_INCLUDED
R_BSP_PRAGMA_STATIC_INTERRUPT(sci8_txi8_isr, VECT(SCI8,TXI8))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci8_txi8_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci8_rxi8_isr, VECT(SCI8,RXI8))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci8_rxi8_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci8_tei8_isr, VECT(SCI8,TEI8))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci8_tei8_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci8_eri8_isr, VECT(SCI8,ERI8))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci8_eri8_isr(void);

#endif

#if SCI_CFG_CH9_INCLUDED
R_BSP_PRAGMA_STATIC_INTERRUPT(sci9_txi9_isr, VECT(SCI9,TXI9))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci9_txi9_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci9_rxi9_isr, VECT(SCI9,RXI9))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci9_rxi9_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci9_tei9_isr, VECT(SCI9,TEI9))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci9_tei9_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci9_eri9_isr, VECT(SCI9,ERI9))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci9_eri9_isr(void);

#endif

#if SCI_CFG_CH12_INCLUDED
R_BSP_PRAGMA_STATIC_INTERRUPT(sci12_txi12_isr, VECT(SCI12,TXI12))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci12_txi12_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci12_rxi12_isr, VECT(SCI12,RXI12))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci12_rxi12_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci12_tei12_isr, VECT(SCI12,TEI12))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci12_tei12_isr(void);
R_BSP_PRAGMA_STATIC_INTERRUPT(sci12_eri12_isr, VECT(SCI12,ERI12))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci12_eri12_isr(void);

#endif

#if (SCI_CFG_IRDA_INCLUDED)
#if (SCI_CFG_CH5_IRDA_INCLUDED)
/* Queue buffers*/
static uint8_t         irda_ch5_tx_buf[SCI_CFG_CH5_TX_BUFSIZ];
static uint8_t         irda_ch5_rx_buf[SCI_CFG_CH5_RX_BUFSIZ];
#endif
/*------- Global functions -------*/
static sci_err_t      sci_irda_init_queues (uint8_t const  chan);
static void           sci_irda_clear_io_register (sci_hdl_t const hdl);
static void           sci_irda_io_setting (uint8_t* port_sel);
static void           sci_irda_io_mode_setting (uint8_t* port_sel, bool init_or_setting);
static sci_err_t      sci_irda_init_bit_rate (sci_hdl_t const     hdl,
                                              uint32_t const      pclk,
                                              sci_irda_t * const  p_cfg);
#endif

/*****************************************************************************
* Function Name: sci_mcu_param_check
* Description  : This function parameters check on MCU.
*                 (channel range, interrupt priority, etc...)
* Arguments    : chan -
*                    channel to check
* Return Value : SCI_SUCCESS - 
*                    parameter check all successfully
*                SCI_ERR_BAD_CHAN -
*                    channel number invalid for part
*                SCI_ERR_INVALID_ARG -
*                    interrupt priority out of range
******************************************************************************/
sci_err_t sci_mcu_param_check(uint8_t const chan)
{
    /* channel range parameter check */
    if ((chan != SCI_CH0) && (chan != SCI_CH1) 
     && (chan != SCI_CH5) && (chan != SCI_CH6) 
     && (chan != SCI_CH8) && (chan != SCI_CH9) && (chan != SCI_CH12))
    {
        return SCI_ERR_BAD_CHAN;
    }

    return SCI_SUCCESS;
} /* End of function sci_mcu_param_check() */

/*****************************************************************************
* Function Name: sci_init_register
* Description  : This function initializes the register for SCI.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
void sci_init_register(sci_hdl_t const hdl)
{
    /* SCI transmit enable bit and receive enable bit check & disable */
    while ((0 != hdl->rom->regs->SCR.BIT.TE) || (0 != hdl->rom->regs->SCR.BIT.RE))
    {
        if (0 != hdl->rom->regs->SCR.BIT.TE)
        {
            hdl->rom->regs->SCR.BIT.TE = 0;  // transmit disable
        }
        if (0 != hdl->rom->regs->SCR.BIT.RE)
        {
            hdl->rom->regs->SCR.BIT.RE = 0;  // receive disable
        }
    }

    /* SMR register initialize */
    hdl->rom->regs->SMR.BYTE = 0x00;

    /* SCR register initialize */
    hdl->rom->regs->SCR.BYTE = 0x00;
    
    /* SSR register initialize */
    if (1 == SCI_SSR_ORER)
    {
        SCI_SSR_ORER = 0;
    }
    if (1 == SCI_SSR_PER)
    {
        SCI_SSR_PER = 0;
    }
    if (1 == SCI_SSR_FER)
    {
        SCI_SSR_FER = 0;
    }

    /* SCMR register initialize */
    hdl->rom->regs->SCMR.BIT.SMIF = 0;
    hdl->rom->regs->SCMR.BIT.SINV = 0;
    hdl->rom->regs->SCMR.BIT.SDIR = 0;

    /* BRR register initialize */
    hdl->rom->regs->BRR = 0xFF;

    /* SEMR register initialize */
    hdl->rom->regs->SEMR.BIT.BRME    = 0;
    hdl->rom->regs->SEMR.BIT.ABCS    = 0;
    hdl->rom->regs->SEMR.BIT.NFEN    = 0;
    hdl->rom->regs->SEMR.BIT.BGDM    = 0;
    hdl->rom->regs->SEMR.BIT.RXDESEL = 0;

    /* SNFR register initialize */
    hdl->rom->regs->SNFR.BYTE = 0;

    /* SPMR register initialize */
    hdl->rom->regs->SPMR.BIT.CTSE  = 0;
    hdl->rom->regs->SPMR.BIT.CKPOL = 0;
    hdl->rom->regs->SPMR.BIT.CKPH  = 0;

    return;
} /* End of function sci_init_register() */

/*****************************************************************************
* Function Name: sci_init_bit_rate
* Description  : This function determines the best possible settings for the
*                baud rate registers for the specified peripheral clock speed
*                and baud rate. Note that this does not guarantee a low bit 
*                error rate, just the best possible one. The bit rate error is
*                returned in .1% increments. If the hardware cannot support
*                the specified combination, a value of 1000 (100% error) is
*                returned.
*
* NOTE: The transmitter and receiver (TE and RE bits in SCR) must be disabled 
*       prior to calling this function.
*
*       The application must pause for 1 bit time after the BRR register
*       is loaded before transmitting/receiving to allow time for the clock
*       to settle. 
*
* Arguments    : hdl -
*                    Handle for channel (ptr to chan control block)
*                    NOTE: mode element must be already set
*                pclk -
*                    Peripheral clock speed; e.g. 24000000 for 24MHz
*                baud -
*                    Baud rate; 19200, 57600, 115200, etc.
* Return Value : bit error in .1% increments; e.g. 16 = 1.6% bit rate error
*                a value of 1000 denotes 100% error; no registers set
******************************************************************************/
int32_t sci_init_bit_rate(sci_hdl_t const  hdl,
                          uint32_t const   pclk,
                          uint32_t const   baud)
{
    uint32_t i;
    uint32_t num_divisors = 0;
    uint32_t ratio;
    uint32_t tmp;
    baud_divisor_t const *p_baud_info=NULL;

    uint32_t divisor;
    uint32_t int_M;
    float    float_M;
    float    error;
    float    abs_error;

#if SCI_CFG_PARAM_CHECKING_ENABLE
    if ((0 == pclk) || (0 == baud))
    {
        return 1000;
    }
#endif

    /* SELECT PROPER TABLE BASED UPON MODE */
    if (hdl->mode == SCI_MODE_ASYNC)
    {
#if (SCI_CFG_ASYNC_INCLUDED)
        p_baud_info = async_baud;
        num_divisors = NUM_DIVISORS_ASYNC;
#endif
    }
    else
    {   /* SYNC or SSPI */
#if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
        p_baud_info = sync_baud;
        num_divisors = NUM_DIVISORS_SYNC;
#endif
    }

    /* FIND DIVISOR; table has associated ABCS, BGDM and CKS values */
    /* BRR must be 255 or less */
    /* the "- 1" is ignored in some steps for approximations */
    /* BRR = (PCLK/(divisor * baud)) - 1 */
    /* BRR = (ratio / divisor) - 1 */
    ratio = pclk/baud;
    for(i=0; i < num_divisors; i++)
    {
        if (ratio < (uint32_t)(p_baud_info[i].divisor * 256))
        {
            break;
        }
    }

    /* RETURN IF BRR WILL BE >255 OR LESS THAN 0 */
    if (i == num_divisors)
    {
        return(1000);           // impossible baud rate requested; return 100% error
    }

    divisor = (uint32_t)p_baud_info[i].divisor;
    tmp = ratio/(divisor);      // tmp = PCLK/(baud * divisor) = BRR+1 = N+1
    if(0 == tmp)
    {
        return(1000);            // illegal value; return 100% error
    }

    /* SET BRR, ABCS, BDGM, and CKS */
    tmp = ratio / (divisor/2);  // divide by half the divisor

    /* if odd, "round up" by ignoring -1; divide by 2 again for rest of divisor */
    hdl->rom->regs->BRR = (uint8_t)((tmp & 0x01) ? (tmp/2) : ((tmp/2)-1));
    hdl->rom->regs->SEMR.BIT.ABCS = p_baud_info[i].abcs;
    hdl->rom->regs->SEMR.BIT.BGDM = p_baud_info[i].bgdm;
    hdl->rom->regs->SMR.BIT.CKS = p_baud_info[i].cks;

    /* CALCULATE BIT RATE ERROR.
     * RETURN IF ERROR LESS THAN 1% OR IF IN SYNCHRONOUS/SSPI MODE.
     */
    tmp = ratio/(divisor);      // tmp = PCLK/(baud * divisor) = BRR+1 = N+1
    error = ( ((float)pclk / ((baud * divisor) * tmp)) - 1) * 100;
    abs_error  = (error < 0) ? (-error) : error;

    if ((abs_error <= 1.0) || (hdl->mode != SCI_MODE_ASYNC))
    {
        hdl->rom->regs->SEMR.BIT.BRME = 0;          // disable MDDR
        return (uint32_t)(error*10);
    }

    /* CALCULATE M ASSUMING A 0% ERROR then WRITE REGISTER */
    hdl->rom->regs->BRR = (uint8_t)(tmp-1);
    float_M = ((float)((baud * divisor) * 256) * tmp) / pclk;
    float_M *= 2;
    int_M = (uint32_t)float_M;
    int_M = (int_M & 0x01) ? ((int_M/2) + 1) : (int_M/2);

    hdl->rom->regs->MDDR = (uint8_t)int_M;      // write M
    hdl->rom->regs->SEMR.BIT.BRME = 1;          // enable MDDR
    error = (( (float)(pclk) / (((divisor * tmp) * baud) * ((float)(256)/int_M)) ) - 1) * 100;

    /* Casting float to int32_t */      
    return (int32_t)(error*10);
} /* End of function sci_init_bit_rate() */

/*****************************************************************************
* Function Name: sci_initialize_ints
* Description  : This function sets priority, clears flags, and sets 
*                interrupts in both the ICU and SCI peripheral. These include 
*                RXI, TXI, TEI, and ERI interrupts.
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
*                priority -
*                    priority for interrupts
* Return Value : none
******************************************************************************/
void sci_initialize_ints(sci_hdl_t const hdl,
                         uint8_t const   priority)
{
    /* SET PRIORITY FOR INTERRUPTS */
    *hdl->rom->ipr = priority;

    /* DISABLE INTERRUPTS */
    DISABLE_ERI_INT;
    DISABLE_RXI_INT;
    DISABLE_TXI_INT;
    DISABLE_TEI_INT;

    /* CLEAR INTERRUPT FLAGS */
    *hdl->rom->ir_rxi = 0;
    *hdl->rom->ir_txi = 0;
    *hdl->rom->ir_tei = 0;
    *hdl->rom->ir_eri = 0;

    /* ENABLE ERI AND RXI INTERRUPTS REQUESTS */
    ENABLE_ERI_INT;
    ENABLE_RXI_INT;

    /* ENABLE INTERRUPTS IN SCI PERIPHERAL */
    /* Note: Enable interrupts after xcvr or will get "extra" interrupt */
    hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;    // enable TE, RE, TXI, and RXI/ERI

    return;    
} /* End of function sci_initialize_ints() */

/*****************************************************************************
* Function Name: sci_disable_ints
* Description  : This function disable interrupts in both the ICU and SCI 
*                peripheral. These include RXI, TXI, TEI, and ERI interrupts.
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
void sci_disable_ints(sci_hdl_t const hdl)
{
    /* disable ICU interrupts */
    DISABLE_RXI_INT;
    DISABLE_TXI_INT;
    DISABLE_ERI_INT;
    DISABLE_TEI_INT;
    
    /* disable peripheral interrupts and xcvr (TE and RE) */
    hdl->rom->regs->SCR.BYTE = 0;

    return;
} /* End of function sci_disable_ints() */

#if ((SCI_CFG_ASYNC_INCLUDED) || (TX_DTC_DMACA_ENABLE | RX_DTC_DMACA_ENABLE) || (SCI_CFG_IRDA_INCLUDED))
/*****************************************************************************
* sciN_txiN_isr 
*  
* Description  : TXI interrupt routines for every SCI channel.
******************************************************************************/

#if SCI_CFG_CH0_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci0_txi0_isr(void)
{
    txi_handler(&ch0_ctrl);
} /* End of function sci0_txi0_isr() */

#endif

#if SCI_CFG_CH1_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci1_txi1_isr(void)
{
    txi_handler(&ch1_ctrl);
} /* End of function sci1_txi1_isr() */

#endif


#if SCI_CFG_CH5_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci5_txi5_isr(void)
{
    txi_handler(&ch5_ctrl);
} /* End of function sci5_txi5_isr() */

#endif

#if SCI_CFG_CH6_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci6_txi6_isr(void)
{
    txi_handler(&ch6_ctrl);
} /* End of function sci6_txi6_isr() */

#endif

#if SCI_CFG_CH8_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci8_txi8_isr(void)
{
    txi_handler(&ch8_ctrl);
} /* End of function sci8_txi8_isr() */

#endif

#if SCI_CFG_CH9_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci9_txi9_isr(void)
{
    txi_handler(&ch9_ctrl);
} /* End of function sci9_txi9_isr() */

#endif

#if SCI_CFG_CH12_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci12_txi12_isr(void)
{
    txi_handler(&ch12_ctrl);
} /* End of function sci12_txi12_isr() */

#endif

#endif /* End of ((SCI_CFG_ASYNC_INCLUDED) || (TX_DTC_DMACA_ENABLE | RX_DTC_DMACA_ENABLE) || (SCI_CFG_IRDA_INCLUDED)) */

#if SCI_CFG_TEI_INCLUDED
/*****************************************************************************
* sciN_teiN_isr 
*  
* Description  : TEI interrupt routines for every SCI channel.
******************************************************************************/

#if SCI_CFG_CH0_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci0_tei0_isr(void)
{
    tei_handler(&ch0_ctrl);
} /* End of function sci0_tei0_isr() */

#endif

#if SCI_CFG_CH1_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci1_tei1_isr(void)
{
    tei_handler(&ch1_ctrl);
} /* End of function sci1_tei1_isr() */

#endif

#if SCI_CFG_CH5_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci5_tei5_isr(void)
{
    tei_handler(&ch5_ctrl);
} /* End of function sci5_tei5_isr() */

#endif

#if SCI_CFG_CH6_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci6_tei6_isr(void)
{
    tei_handler(&ch6_ctrl);
} /* End of function sci6_tei6_isr() */

#endif

#if SCI_CFG_CH8_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci8_tei8_isr(void)
{
    tei_handler(&ch8_ctrl);
} /* End of function sci8_tei8_isr() */

#endif

#if SCI_CFG_CH9_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci9_tei9_isr(void)
{
    tei_handler(&ch9_ctrl);
} /* End of function sci9_tei9_isr() */

#endif

#if SCI_CFG_CH12_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci12_tei12_isr(void)
{
    tei_handler(&ch12_ctrl);
} /* End of function sci12_tei12_isr() */

#endif

#endif

/*****************************************************************************
* sciN_rxiN_isr
*  
* Description  : RXI interrupt routines for every SCI channel.
******************************************************************************/

#if SCI_CFG_CH0_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci0_rxi0_isr(void)
{
    rxi_handler(&ch0_ctrl);
} /* End of function sci0_rxi0_isr() */

#endif

#if SCI_CFG_CH1_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci1_rxi1_isr(void)
{
    rxi_handler(&ch1_ctrl);
} /* End of function sci1_rxi1_isr() */

#endif

#if SCI_CFG_CH5_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci5_rxi5_isr(void)
{
    rxi_handler(&ch5_ctrl);
} /* End of function sci5_rxi5_isr() */

#endif

#if SCI_CFG_CH6_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci6_rxi6_isr(void)
{
    rxi_handler(&ch6_ctrl);
} /* End of function sci6_rxi6_isr() */

#endif

#if SCI_CFG_CH8_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci8_rxi8_isr(void)
{
    rxi_handler(&ch8_ctrl);
} /* End of function sci8_rxi8_isr() */

#endif

#if SCI_CFG_CH9_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci9_rxi9_isr(void)
{
    rxi_handler(&ch9_ctrl);
} /* End of function sci9_rxi9_isr() */

#endif

#if SCI_CFG_CH12_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci12_rxi12_isr(void)
{
    rxi_handler(&ch12_ctrl);
} /* End of function sci12_rxi12_isr() */

#endif

/*****************************************************************************
* sciN_eriN_isr 
*  
* Description  : ERI interrupt routines for every SCI channel.
******************************************************************************/

#if SCI_CFG_CH0_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci0_eri0_isr(void)
{
    eri_handler(&ch0_ctrl);
} /* End of function sci0_eri0_isr() */

#endif

#if SCI_CFG_CH1_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci1_eri1_isr(void)
{
    eri_handler(&ch1_ctrl);
} /* End of function sci1_eri1_isr() */

#endif

#if SCI_CFG_CH5_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci5_eri5_isr(void)
{
    eri_handler(&ch5_ctrl);
} /* End of function sci5_eri5_isr() */
#endif

#if SCI_CFG_CH6_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci6_eri6_isr(void)
{
    eri_handler(&ch6_ctrl);
} /* End of function sci6_eri6_isr() */

#endif

#if SCI_CFG_CH8_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci8_eri8_isr(void)
{
    eri_handler(&ch8_ctrl);
} /* End of function sci8_eri8_isr() */

#endif

#if SCI_CFG_CH9_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci9_eri9_isr(void)
{
    eri_handler(&ch9_ctrl);
} /* End of function sci9_eri9_isr() */

#endif

#if SCI_CFG_CH12_INCLUDED
R_BSP_ATTRIB_STATIC_INTERRUPT void sci12_eri12_isr(void)
{
    eri_handler(&ch12_ctrl);
} /* End of function sci12_eri12_isr() */

#endif

#if (SCI_CFG_ASYNC_INCLUDED)
/*****************************************************************************
* Function Name: sci_async_cmds
* Description  : This function configures non-standard UART hardware and
*                performs special software operations.
*
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                cmd -
*                    command to run
*                p_args -
*                    pointer argument(s) specific to command
* Return Value : SCI_SUCCESS -
*                    Command completed successfully.
*                SCI_ERR_NULL_PTR -
*                    p_args is NULL when required for cmd
*                SCI_ERR_INVALID_ARG -
*                    The cmd value or p_args contains an invalid value.
******************************************************************************/
sci_err_t sci_async_cmds(sci_hdl_t const hdl,
                         sci_cmd_t const cmd,
                         void            *p_args)
{
    sci_err_t   err=SCI_SUCCESS;
    int32_t     bit_err;
    uint32_t    slow_baud;

#if SCI_CFG_PARAM_CHECKING_ENABLE
    if (((NULL == p_args) || (FIT_NO_PTR == p_args))
     && ((SCI_CMD_TX_Q_BYTES_FREE == cmd) || (SCI_CMD_RX_Q_BYTES_AVAIL_TO_READ == cmd)))
    {
        return SCI_ERR_NULL_PTR;
    }
#endif

    switch(cmd)
    {
    case (SCI_CMD_EN_NOISE_CANCEL):
        hdl->rom->regs->SCR.BYTE &= (~SCI_EN_XCVR_MASK);
        SCI_SCR_DUMMY_READ;
        hdl->rom->regs->SEMR.BIT.NFEN = 1;      /* enable noise filter */
        hdl->rom->regs->SNFR.BYTE = 0;          /* clock divided by 1 (default) */
        SCI_IR_TXI_CLEAR;
        hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
    break;

    case (SCI_CMD_OUTPUT_BAUD_CLK):
        hdl->rom->regs->SCR.BYTE &= (~SCI_EN_XCVR_MASK);
        SCI_SCR_DUMMY_READ;
        hdl->rom->regs->SCR.BIT.CKE = 0x01;     /* output baud clock on SCK pin */
        SCI_IR_TXI_CLEAR;
        hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
    break;

    case (SCI_CMD_START_BIT_EDGE):
        hdl->rom->regs->SCR.BYTE &= (~SCI_EN_XCVR_MASK);
        SCI_SCR_DUMMY_READ;
        hdl->rom->regs->SEMR.BIT.RXDESEL = 1;   /* detect start bit on falling edge */
        SCI_IR_TXI_CLEAR;
        hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
    break;

#if SCI_CFG_TEI_INCLUDED
    case (SCI_CMD_EN_TEI):  /* SCI_CMD_EN_TEI is obsolete command, but it exists only for compatibility with older version. */
    break;
#endif
#if TX_DTC_DMACA_ENABLE
        case (SCI_CMD_CHECK_TX_DONE):
        {
            if((SCI_DTC_ENABLE == hdl->rom->dtc_dmaca_tx_enable) || (SCI_DMACA_ENABLE == hdl->rom->dtc_dmaca_tx_enable))
            {
                if (false == hdl->tx_idle)
                {
                    err = SCI_ERR_XCVR_BUSY;
                }
            }
            break;
        }
#endif

#if RX_DTC_DMACA_ENABLE
        case (SCI_CMD_CHECK_RX_DONE):
        {
            if((SCI_DTC_ENABLE == hdl->rom->dtc_dmaca_rx_enable) || (SCI_DMACA_ENABLE == hdl->rom->dtc_dmaca_rx_enable))
            {
                if (0 != hdl->queue[0].rx_cnt)
                {
                    err = SCI_ERR_XCVR_BUSY;
                }
            }
            break;
        }
#endif
    case (SCI_CMD_TX_Q_FLUSH):
    {
#if (SCI_CFG_USE_CIRCULAR_BUFFER == 1)
        R_BYTEQ_Flush(hdl->u_tx_data.que);
#else
        /* Disable TXI interrupt */
        DISABLE_TXI_INT;
        R_BYTEQ_Flush(hdl->u_tx_data.que);
        ENABLE_TXI_INT;
#endif
    break;
    }

    case (SCI_CMD_RX_Q_FLUSH):
    {
#if (SCI_CFG_USE_CIRCULAR_BUFFER == 1)
        R_BYTEQ_Flush(hdl->u_rx_data.que);
#else
        /* Disable RXI interrupt */
        DISABLE_RXI_INT;
        R_BYTEQ_Flush(hdl->u_rx_data.que);
        ENABLE_RXI_INT;
#endif
    break;
    }

    case (SCI_CMD_TX_Q_BYTES_FREE):
        R_BYTEQ_Unused(hdl->u_tx_data.que, (uint16_t *) p_args);
    break;

    case (SCI_CMD_RX_Q_BYTES_AVAIL_TO_READ):
        R_BYTEQ_Used(hdl->u_rx_data.que, (uint16_t *) p_args);
    break;

    case (SCI_CMD_GENERATE_BREAK):

        /* flush transmit queue */
        DISABLE_TXI_INT;
        R_BYTEQ_Flush(hdl->u_tx_data.que);
#if(TX_DTC_DMACA_ENABLE)
        if((SCI_DTC_ENABLE == hdl->rom->dtc_dmaca_tx_enable) || (SCI_DMACA_ENABLE == hdl->rom->dtc_dmaca_tx_enable))
        {
            sci_fifo_ctrl_t        *p_tctrl = &hdl->queue[hdl->qindex_app_rx];
            p_tctrl->tx_cnt = 0;
            p_tctrl->tx_fraction = 0;
#if ((TX_DTC_DMACA_ENABLE & 0x01) || (RX_DTC_DMACA_ENABLE & 0x01))
            if(SCI_DTC_ENABLE == hdl->rom->dtc_dmaca_tx_enable)
            {
                dtc_cmd_arg_t           args_dtc;
                args_dtc.act_src = hdl->rom->dtc_tx_act_src;
                R_DTC_Control(DTC_CMD_ACT_SRC_DISABLE, NULL, &args_dtc);

            }
#endif
#if ((TX_DTC_DMACA_ENABLE & 0x02) || (RX_DTC_DMACA_ENABLE & 0x02))
            if(SCI_DMACA_ENABLE == hdl->rom->dtc_dmaca_tx_enable)
            {
                dmaca_stat_t   stat_dmaca;
                R_DMACA_Control(hdl->rom->dmaca_tx_channel, DMACA_CMD_DISABLE, &stat_dmaca);

            }
#endif
#if SCI_CFG_FIFO_INCLUDED
            if (true == hdl->fifo_ctrl)
            {

                /* reset TX FIFO */
                hdl->rom->regs->FCR.BIT.TFRST = 0x01;
            }
#endif
        }
#endif
        ENABLE_TXI_INT;

        /* NOTE: the following steps will abort anything being sent */

        /* set baud rate 1.5x slower */
        slow_baud = (hdl->baud_rate << 1) / 3;
        hdl->rom->regs->SCR.BYTE &= (~SCI_EN_XCVR_MASK);
        SCI_SCR_DUMMY_READ;
        bit_err = sci_init_bit_rate(hdl, hdl->pclk_speed, slow_baud);
        SCI_IR_TXI_CLEAR;
        hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
        if (1000 == bit_err)
        {
            err = SCI_ERR_INVALID_ARG;
        }
        else
        {   
            /* transmit "0" and wait for completion */
            SCI_TDR(0);
            while (0 == hdl->rom->regs->SSR.BIT.TEND)
            {
                R_BSP_NOP();
            }

            /* restore original baud rate */
            hdl->rom->regs->SCR.BYTE &= (~SCI_EN_XCVR_MASK);
            SCI_SCR_DUMMY_READ;
            sci_init_bit_rate(hdl, hdl->pclk_speed, hdl->baud_rate);
            SCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
        }
    break;

    default:
        err = SCI_ERR_INVALID_ARG;
    break;
    }

    return err;
} /* End of function sci_async_cmds() */
#endif /* End of SCI_CFG_ASYNC_INCLUDED */

#if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
/*****************************************************************************
* Function Name: sci_sync_cmds
* Description  : This function performs special software operations specific
*                to the SSPI and SYNC protocols.
*
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                cmd -
*                    command to run
*                p_args -
*                    pointer argument(s) specific to command
* Return Value : SCI_SUCCESS -
*                    Command completed successfully.
*                SCI_ERR_NULL_PTR -
*                    p_args is NULL when required for cmd
*                SCI_ERR_INVALID_ARG -
*                    The cmd value or p_args contains an invalid value.
*                    May be due to mode channel is operating in.
******************************************************************************/
sci_err_t sci_sync_cmds(sci_hdl_t const hdl,
                        sci_cmd_t const cmd,
                        void            *p_args)
{
    sci_spi_mode_t  spi_mode;
    sci_cb_args_t   args;
    sci_err_t       err=SCI_SUCCESS;

    switch (cmd)
    {
    case (SCI_CMD_CHECK_XFER_DONE):
        if (hdl->tx_idle == false)
        {
            err = SCI_ERR_XFER_NOT_DONE;
        }
    break;

    case (SCI_CMD_XFER_LSB_FIRST):
        hdl->rom->regs->SCR.BYTE &= ~SCI_EN_XCVR_MASK;
        SCI_SCR_DUMMY_READ;
        hdl->rom->regs->SCMR.BIT.SDIR = 0;
        SCI_IR_TXI_CLEAR;
        hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
    break;

    case (SCI_CMD_XFER_MSB_FIRST):
        hdl->rom->regs->SCR.BYTE &= ~SCI_EN_XCVR_MASK;
        SCI_SCR_DUMMY_READ;
        hdl->rom->regs->SCMR.BIT.SDIR = 1;
        SCI_IR_TXI_CLEAR;
        hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
    break;

    case (SCI_CMD_INVERT_DATA):
        hdl->rom->regs->SCR.BYTE &= ~SCI_EN_XCVR_MASK;
        SCI_SCR_DUMMY_READ;
        hdl->rom->regs->SCMR.BIT.SINV ^= 1;
        SCI_IR_TXI_CLEAR;
        hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
    break;

    case (SCI_CMD_ABORT_XFER):

        /* disable receive interrupts in ICU and peripheral */
        DISABLE_RXI_INT;
        DISABLE_ERI_INT;
#if(TX_DTC_DMACA_ENABLE)
        if((SCI_DTC_ENABLE == hdl->rom->dtc_dmaca_tx_enable) || (SCI_DMACA_ENABLE == hdl->rom->dtc_dmaca_tx_enable))
        {
            sci_fifo_ctrl_t        *p_tctrl = &hdl->queue[hdl->qindex_app_rx];
            p_tctrl->tx_cnt = 0;
            p_tctrl->tx_fraction = 0;
#if ((TX_DTC_DMACA_ENABLE & 0x01) || (RX_DTC_DMACA_ENABLE & 0x01))
            if((SCI_DTC_ENABLE == hdl->rom->dtc_dmaca_tx_enable) && (SCI_DTC_ENABLE == hdl->rom->dtc_dmaca_rx_enable))
            {
                // Set condition for reset TDFR to generate interrupt in next time
                hdl->qindex_int_tx = 1;
                dtc_cmd_arg_t           args_dtc;
                args_dtc.act_src               = hdl->rom->dtc_tx_act_src;
                R_DTC_Control(DTC_CMD_ACT_SRC_DISABLE, NULL, &args_dtc);

                args_dtc.act_src               = hdl->rom->dtc_rx_act_src;
                R_DTC_Control(DTC_CMD_ACT_SRC_DISABLE, NULL, &args_dtc);
            }
#endif
#if ((TX_DTC_DMACA_ENABLE & 0x02) || (RX_DTC_DMACA_ENABLE & 0x02))
            if((SCI_DMACA_ENABLE == hdl->rom->dtc_dmaca_tx_enable) && (SCI_DMACA_ENABLE == hdl->rom->dtc_dmaca_rx_enable))
            {
                R_DMACA_Close(hdl->rom->dmaca_tx_channel);
                R_DMACA_Close(hdl->rom->dmaca_rx_channel);
            }
#endif

        }
#endif
        hdl->rom->regs->SCR.BYTE &= ~(SCI_SCR_REI_MASK | SCI_SCR_RE_MASK | SCI_SCR_TE_MASK);

        hdl->tx_cnt = 0;
        hdl->tx_dummy = false;
        hdl->tx_idle = true;

        /* Do callback if available */
        if ((hdl->callback != NULL) && (hdl->callback != FIT_NO_FUNC))
        {
            args.hdl = hdl;
            args.event = SCI_EVT_XFER_ABORTED;
            hdl->callback((void *)&args);
        }

        *hdl->rom->ir_rxi = 0;                  /* clear rxi interrupt flag */
#if SCI_CFG_FIFO_INCLUDED
#if(TX_DTC_DMACA_ENABLE)
            if((SCI_DTC_DMACA_DISABLE != hdl->rom->dtc_dmaca_tx_enable) && (true != hdl->fifo_ctrl))
            {
                *hdl->rom->ir_txi = 0;
            }
#endif
#else
#if(TX_DTC_DMACA_ENABLE)
            if((SCI_DTC_DMACA_DISABLE != hdl->rom->dtc_dmaca_tx_enable))
            {
                *hdl->rom->ir_txi = 0;
            }
#endif
#endif
        *hdl->rom->ir_eri = 0;                  /* clear eri interrupt flag */

        ENABLE_ERI_INT;                         /* enable rx err interrupts in ICU */
        ENABLE_RXI_INT;                         /* enable receive interrupts in ICU */

        /* Enable receive interrupt in peripheral after rcvr or will get "extra" interrupt */
        hdl->rom->regs->SCR.BYTE |= SCI_SCR_RE_MASK | SCI_SCR_TE_MASK;
        hdl->rom->regs->SCR.BYTE |= SCI_SCR_REI_MASK;
    break;
#if RX_DTC_DMACA_ENABLE
        case (SCI_CMD_CHECK_RX_SYNC_DONE):
        {
            if((SCI_DTC_ENABLE == hdl->rom->dtc_dmaca_rx_enable) || (SCI_DMACA_ENABLE == hdl->rom->dtc_dmaca_rx_enable))
            {
                if (0 != hdl->queue[0].rx_cnt)
                {
                    err = SCI_ERR_XCVR_BUSY;
                }
            }
            break;
        }
#endif
    case (SCI_CMD_CHANGE_SPI_MODE):
#if SCI_CFG_PARAM_CHECKING_ENABLE

        if (hdl->mode != SCI_MODE_SSPI)
        {
            return SCI_ERR_INVALID_ARG;
        }

        /* Check parameters */
        if ((p_args == NULL) || (p_args == FIT_NO_PTR))
        {
            return SCI_ERR_NULL_PTR;
        }

        /* Casting pointer void* type is valid */
        spi_mode = *((sci_spi_mode_t *)p_args);

        if ((spi_mode != SCI_SPI_MODE_0) && (spi_mode != SCI_SPI_MODE_1)
          && (spi_mode != SCI_SPI_MODE_2) && (spi_mode != SCI_SPI_MODE_3))
        {
            return SCI_ERR_INVALID_ARG;
        }
#endif
        hdl->rom->regs->SCR.BYTE &= ~SCI_EN_XCVR_MASK;
        SCI_SCR_DUMMY_READ;
        hdl->rom->regs->SPMR.BYTE &= 0x3F;      /* clear previous mode */
        hdl->rom->regs->SPMR.BYTE |= *((uint8_t *)p_args);
        SCI_IR_TXI_CLEAR;
        hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
    break;

    default:
        err = SCI_ERR_INVALID_ARG;
    break;
    }

    return err;
} /* End of function sci_sync_cmds() */
#endif /* End of SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED */

#if (SCI_CFG_IRDA_INCLUDED)
/*****************************************************************************
* Function Name: sci_irda_cmds
* Description  : This function configures non-standard UART hardware and
*                performs special software operations.
*
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                cmd -
*                    command to run
*                p_args -
*                    pointer argument(s) specific to command
* Return Value : SCI_SUCCESS -
*                    Command completed successfully.
*                SCI_ERR_NULL_PTR -
*                    p_args is NULL when required for cmd
*                SCI_ERR_INVALID_ARG -
*                    The cmd value or p_args contains an invalid value.
******************************************************************************/
sci_err_t sci_irda_cmds(sci_hdl_t const hdl,
                        sci_cmd_t const cmd,
                        void            *p_args)
{
    sci_err_t   err=SCI_SUCCESS;

#if SCI_CFG_PARAM_CHECKING_ENABLE
    if (((NULL == p_args) || (FIT_NO_PTR == p_args))
     && ((SCI_CMD_TX_Q_BYTES_FREE == cmd) || (SCI_CMD_RX_Q_BYTES_AVAIL_TO_READ == cmd)))
    {
        return SCI_ERR_NULL_PTR;
    }
#endif

    /* Command control */
        switch(cmd)
        {

        /* flush the Transmit queue */
        case (SCI_CMD_TX_Q_FLUSH):
            DISABLE_TXI_INT;
            R_BYTEQ_Flush(hdl->u_tx_data.que);
            ENABLE_TXI_INT;
        break;

        /* flush the Receive queue */
        case (SCI_CMD_RX_Q_FLUSH):
            DISABLE_RXI_INT;
            R_BYTEQ_Flush(hdl->u_rx_data.que);
            ENABLE_RXI_INT;
        break;

        /* Checks the size of space in the transmit queue. */
        case (SCI_CMD_TX_Q_BYTES_FREE):
            R_BYTEQ_Unused(hdl->u_tx_data.que, (uint16_t *) p_args);
        break;

        /* Checks the size of the data stored in the receive queue. */
        case (SCI_CMD_RX_Q_BYTES_AVAIL_TO_READ):
            R_BYTEQ_Used(hdl->u_rx_data.que, (uint16_t *) p_args);
        break;

        /* other */
        default:
            err = SCI_ERR_INVALID_ARG;
        break;
        }

        return err;
} /* End of function sci_irda_cmds() */

/*****************************************************************************
* Function Name: sci_irda_open
* Description  : Initialization for using the IRDA
*
* NOTE: The associated port must be configured/initialized prior to
*       calling this function.
*
* Arguments    :  sci_ch_ctrl_t -
*                    handle for channel (pointer to channel control block)
* Return Value :  SCI_SUCCESS -
*                    channel opened successfully
******************************************************************************/
sci_err_t sci_irda_open(uint8_t const      chan,
                        sci_irda_t * const p_cfg,
                        uint8_t * const      p_priority,
                        sci_hdl_t  const   hdl)
{
    sci_err_t  err = SCI_SUCCESS;

    /* Check arguments */

#if SCI_CFG_PARAM_CHECKING_ENABLE
    /* Check channel has IrDA interface */
    if (chan != SCI_CH5)
    {
        return SCI_ERR_BAD_CHAN;
    }
    
    /* Check argument p_cfg, p_hdl */
    if (0 == p_cfg->baud_rate)
    {
        return SCI_ERR_INVALID_ARG;
    }
    if (p_cfg->clk_out_width > SCI_IRDA_OUT_WIDTH_128)
    {
        return SCI_ERR_INVALID_ARG;
    }
    if ((p_cfg->int_priority < (BSP_MCU_IPL_MIN+1)) || (p_cfg->int_priority > BSP_MCU_IPL_MAX))
    {
        return SCI_ERR_INVALID_ARG;
    }

#endif /* End of SCI_CFG_PARAM_CHECKING_ENABLE */

    /* I/O port setting (Hi-Z -> Set the state it was in when you release the port) */
    sci_irda_io_setting((uint8_t*)&(hdl->port_rom->irrxd_port_gr));
    sci_irda_io_setting((uint8_t*)&(hdl->port_rom->irtxd_port_gr));

    /* Initialize transmit status flag */
    hdl->tx_idle = true;

    /* Set data polarity and Serial I/O pins are used for IrDA data communication. */
    hdl->rom->regs_irda->IRCR.BYTE = (uint8_t)(0x80 | (SCI_CFG_CH5_IRDA_IRTXD_INACTIVE_LEVEL << 3) |
                          (SCI_CFG_CH5_IRDA_IRRXD_INACTIVE_LEVEL <<2 ));

    /* Configure SMR for asynchronous mode, single processor, 8bit , Non-parity, and 2 stop-bit */
    hdl->rom->regs->SMR.BYTE = (uint8_t)0x08;

    /* Setup clock for Baud-Rate */
    err = sci_irda_init_bit_rate(hdl, hdl->pclk_speed, p_cfg);
    if (SCI_SUCCESS != err)
    {
        return err;
    }

    /* Configuration and initialization of the queue of the sending and receiving */
    err = sci_irda_init_queues(chan);
    if (SCI_SUCCESS != err)
    {
        return err;
    }

    /* Set IRRXD mode of the I/O port */
    sci_irda_io_mode_setting((uint8_t*)&(hdl->port_rom->irrxd_port_gr),TARGET_SETTING);

    /* wait over[18 / (16 * bit-rate)] */
    R_BSP_SoftwareDelay((((1000000/p_cfg->baud_rate) * 18) / 16),BSP_DELAY_MICROSECS);

    hdl->rom->regs_irda->IRCR.BYTE = (uint8_t)(0x80 | (p_cfg->clk_out_width<<4) |
                     (SCI_CFG_CH5_IRDA_IRTXD_INACTIVE_LEVEL << 3) | (SCI_CFG_CH5_IRDA_IRRXD_INACTIVE_LEVEL <<2 ));

    /* Set IRTXD mode of the I/O port */
    sci_irda_io_mode_setting((uint8_t*)&(hdl->port_rom->irtxd_port_gr),TARGET_SETTING);

    *p_priority = p_cfg->int_priority;
    return (err);
} /* End of function sci_irda_open() */

/*****************************************************************************
* Function Name: sci_irda_close
* Description  : Stops the IRDA function. This processing includes the
*                initialization for SCI chX used.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
* Return Value : None
******************************************************************************/
void sci_irda_close(sci_hdl_t const hdl)
{

    /* I/O port Refresh (Set the state it was in when you release the port) */
    sci_irda_io_setting((uint8_t*)&(hdl->port_rom->irtxd_port_gr));
    sci_irda_io_setting((uint8_t*)&(hdl->port_rom->irrxd_port_gr));

    /* Set IN/OUT Port mode of the I/O port */
    sci_irda_io_mode_setting((uint8_t*)&(hdl->port_rom->irtxd_port_gr),TARGET_STOP);

    /* Initialize IrDA Clock Select*/
    hdl->rom->regs_irda->IRCR.BYTE &= 0x8F;

    /* Set IN/OUT Port mode of the I/O port */
    sci_irda_io_mode_setting((uint8_t*)&(hdl->port_rom->irrxd_port_gr),TARGET_STOP);

    /* Initialization of SCI Register */
    sci_irda_clear_io_register(hdl);

    /* Serial I/O pins are used for SCI. */
    hdl->rom->regs_irda->IRCR.BYTE = 0x00;

} /* End of function sci_irda_close() */

/*****************************************************************************
* Outline      : Sets the state when the specified pin is not active state.
* Function Name: sci_irda_io_setting
* Description  : Set IRDA_SCI function pin controller.
* Arguments    : uint8_t* port_sel ; Port select infomation
* Return Value : None
******************************************************************************/
static void sci_irda_io_setting(uint8_t* port_sel)
{
    volatile uint8_t R_BSP_EVENACCESS_SFR * const p_pdr  = ((&PORT0.PDR.BYTE) + *(port_sel));
    volatile uint8_t R_BSP_EVENACCESS_SFR * const p_podr = ((&PORT0.PDR.BYTE) + 0x20 + *(port_sel));

    (*p_podr) |= ((*(port_sel+3) << *(port_sel+1)));        /* Port holds output data. */
    (*p_pdr)  |= ((*(port_sel+2) << *(port_sel+1)));        /* Port Direction */

} /* End of function sci_irda_io_setting() */

/************************************************************************************
* Outline      : Switches the specified pin mode.
* Function Name: sci_irda_io_mode_setting
* Description  : Set IRDA_SCI function pin controller.
* Arguments    : uint8_t* port_sel ; Port select information
*              : init_or_setting   : Specifies the mode for the pin to be selected.
* Return Value : None
************************************************************************************/
static void sci_irda_io_mode_setting(uint8_t* port_sel, bool init_or_setting)
{
    volatile uint8_t R_BSP_EVENACCESS_SFR * const p_pmr  = ((&PORT0.PDR.BYTE) + 0x60 + *(port_sel));
    volatile uint8_t R_BSP_EVENACCESS_SFR * const ppfs = ((((&MPC.PWPR.BYTE) + 0x21 + (*(port_sel) * 8)) + *(port_sel+1)));

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
            R_BSP_NOP();
        }
    }
    else
    {
        /* Select Port Function Control */
        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);  /* Enables the PFS register writing.        */
        (*ppfs) = SCI_IRDA_MPC_IRTXD_DISABLE;
        R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);   /* Disables the PFS register writing.       */

        /* Control Port Mode (Port) */
        (*p_pmr)  &= (~(1U << *(port_sel+1)));
    }

} /* End of function sci_irda_io_mode_setting() */

/************************************************************************************
* Outline      : Reset SCI I/O register
* Function Name: sci_irda_clear_io_register
* Description  : Reset SCI I/O register.
* Arguments    : sci_hdl_t const hdl     ;   IrDA handler
* Return Value : None
************************************************************************************/
static void sci_irda_clear_io_register(sci_hdl_t const hdl)
{
    volatile  struct st_sci12 R_BSP_EVENACCESS_SFR * p_prom = hdl->rom->regs;

    /* Initializes SCI I/O register */
    p_prom->SMR.BYTE     = SCI_IRDA_SMR_INIT;
    p_prom->SCR.BYTE     = SCI_IRDA_SCR_INIT;
    p_prom->SCMR.BYTE    = SCI_IRDA_SCMR_INIT;
    p_prom->BRR          = SCI_IRDA_BRR_INIT;
    p_prom->SEMR.BYTE    = SCI_IRDA_SEMR_INIT;
    p_prom->SNFR.BYTE    = SCI_IRDA_SNFR_INIT;
    p_prom->SIMR1.BYTE   = SCI_IRDA_SIMR1_INIT;
    p_prom->SIMR2.BYTE   = SCI_IRDA_SIMR2_INIT;
    p_prom->SIMR3.BYTE   = SCI_IRDA_SIMR3_INIT;
    p_prom->SISR.BYTE    = SCI_IRDA_SISR_INIT;
    p_prom->SPMR.BYTE    = SCI_IRDA_SPMR_INIT;
} /*End of function sci_irda_clear_io_register */

/*****************************************************************************
* Function Name: sci_irda_init_queues
* Description  : Configuration and initialization of the queue of the sending
*                and receiving
* Arguments    : chan -
*                    channel (ptr to chan control block)
* Return Value : SCI_SUCCESS -
*                    channel initialized successfully
*                SCI_ERR_QUEUE_UNAVAILABLE -
*                    no queue control blocks available
******************************************************************************/
static sci_err_t sci_irda_init_queues(uint8_t const  chan)
{
    byteq_err_t      ret1 = BYTEQ_ERR_INVALID_ARG;
    byteq_err_t      ret2 = BYTEQ_ERR_INVALID_ARG;
    sci_err_t        err  = SCI_SUCCESS;

     /*channel number verified as legal prior to calling this function*/
    switch (chan)
    {
#if SCI_CFG_CH5_IRDA_INCLUDED
        case (SCI_CH5):
        {
            ret1 = R_BYTEQ_Open(irda_ch5_tx_buf, SCI_CFG_CH5_TX_BUFSIZ, &g_handles[SCI_CH5]->u_tx_data.que);
            ret2 = R_BYTEQ_Open(irda_ch5_rx_buf, SCI_CFG_CH5_RX_BUFSIZ, &g_handles[SCI_CH5]->u_rx_data.que);
            break;
        }
        default:
        {
            err = SCI_ERR_QUEUE_UNAVAILABLE;
            break;
        }
#endif
    }

    if ((BYTEQ_SUCCESS != ret1) || (BYTEQ_SUCCESS != ret2))
    {
        err = SCI_ERR_QUEUE_UNAVAILABLE;
    }

    return err;
} /* End of function sci_irda_init_queues() */

/*****************************************************************************
* Function Name: sci_irda_init_bit_rate
* Description  :Configures settings based on the specified peripheral clock
*               and communication rate. Verifies if the high pulse width
*               is within the available range.
* Arguments    : hdl -
*                    Handle for channel (ptr to chan control block)
*                pclk -
*                    Peripheral clock speed; e.g. 24000000 for 24MHz
*                baud -
*                    Baud rate; 19200, 57600, 115200, etc.
* Return Value : SCI_SUCCESS -
*                    Baud-Rate was correctly set
*                SCI_ERR_INVALID_ARG -
*                    Paramater is Invalid value or The baud rate is not set correctly
******************************************************************************/
static sci_err_t sci_irda_init_bit_rate(sci_hdl_t const  hdl,
                                        uint32_t const pclk,
                                        sci_irda_t * const  p_cfg)
{
    uint32_t i;
    uint32_t num_divisors = NUM_DIVISORS_IRDA;
    uint32_t ratio;
    uint32_t tmp;
    uint32_t irda_tmp1;
    uint32_t irda_tmp2;
    uint32_t baud = p_cfg->baud_rate;
    sci_err_t  err = SCI_SUCCESS;

    baud_divisor_t const *p_baud_info = irda_async_baud;

#if (SCI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((0 == pclk) || (0 == baud))
    {
        return (SCI_ERR_INVALID_ARG);
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
        return (SCI_ERR_INVALID_ARG);
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
    /* is within the available range for communication.                                      */
    if (SCI_IRDA_OUT_WIDTH_3_16 != p_cfg->clk_out_width)
    {
        irda_tmp1 = ((pclk / 1000) / irda_ircks_div[p_cfg->clk_out_width].w_denominator);
        irda_tmp2 = (uint32_t)(baud / (0.2125 * 1000));
        if((709 <= irda_tmp1)||(irda_tmp1 <= irda_tmp2))
        {
            return (SCI_ERR_INVALID_ARG);
        }
    }

    /* Set CSK,ABCS bit and BRR register */
    hdl->rom->regs->BRR = (uint8_t)tmp;
    hdl->rom->regs->SEMR.BIT.ABCS = p_baud_info[i].abcs;
    hdl->rom->regs->SEMR.BIT.BGDM = p_baud_info[i].bgdm;
    hdl->rom->regs->SMR.BIT.CKS = p_baud_info[i].cks;

    return (err);
} /* End of function sci_irda_init_bit_rate */

#endif /* End of SCI_CFG_IRDA_INCLUDED */

