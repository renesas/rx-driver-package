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
* File Name    : r_rsci_rx671.c
* Description  : Functions for using RSCI on the RX671 device.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           31.03.2021 1.00    Initial Release
*                              Supported for RX671.
*           13.09.2021 1.10    Fixed setting of transfer data direction in async mode.
*           03.12.2021 2.00    Updated new features in Asynchronous mode
*                              and added support for Manchester mode.
***********************************************************************************************************************/

/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "platform.h"

#include "r_rsci_rx671_private.h"

/*****************************************************************************
Typedef definitions
******************************************************************************/

/*****************************************************************************
Macro definitions
******************************************************************************/

/*****************************************************************************
Private global variables and functions
******************************************************************************/

/*****************************************************************************
* Function Name: rsci_mcu_param_check
* Description  : This function parameters check on MCU.
*                 (channel range, interrupt priority, etc...)
* Arguments    : chan -
*                    channel to check
* Return Value : RSCI_SUCCESS - 
*                    parameter check all successfully
*                RSCI_ERR_BAD_CHAN -
*                    channel number invalid for part
*                RSCI_ERR_INVALID_ARG -
*                    interrupt priority out of range
******************************************************************************/
rsci_err_t rsci_mcu_param_check(uint8_t const chan)
{
    /* channel range parameter check */
    if ((10 > chan) || (11 < chan))
    {
        return RSCI_ERR_BAD_CHAN;
    }

    /* interrupt priority configuration parameter check */
    if ((1 > RSCI_CFG_ERI_TEI_PRIORITY) || (15 < RSCI_CFG_ERI_TEI_PRIORITY))
    {
        return RSCI_ERR_INVALID_ARG;
    }

    return RSCI_SUCCESS;
} /* End of function sci_mcu_param_check() */

/*****************************************************************************
* Function Name: rsci_init_register
* Description  : This function initializes the register for RSCI.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
void rsci_init_register(rsci_hdl_t const hdl)
{
    /* RSCI transmit enable bit and receive enable bit check & disable */
    /* WAIT_LOOP */
    while ((0 != hdl->rom->regs->SCR0.BIT.TE) || (0 != hdl->rom->regs->SCR0.BIT.RE))
    {
        if (0 != hdl->rom->regs->SCR0.BIT.TE)
        {
            hdl->rom->regs->SCR0.BIT.TE = 0;  // transmit disable
        }

        if (0 != hdl->rom->regs->SCR0.BIT.RE)
        {
            hdl->rom->regs->SCR0.BIT.RE = 0;  // receive disable
        }
    }

    /* SCRn register initialize */
    hdl->rom->regs->SCR2.BIT.CKS  = 0;
    hdl->rom->regs->SCR3.BIT.MP   = 0;
    hdl->rom->regs->SCR3.BIT.STOP = 0;
    hdl->rom->regs->SCR1.BIT.PM   = 0;
    hdl->rom->regs->SCR1.BIT.PE   = 0;
    hdl->rom->regs->SCR3.BIT.CHR  = 0;
    hdl->rom->regs->SCR3.BIT.MOD  = 0;
    hdl->rom->regs->SCR2.BIT.BCP  = 0;
    hdl->rom->regs->SCR3.BIT.BLK  = 0;
    hdl->rom->regs->SCR3.BIT.GM   = 0;

    /* SCR0 register initialize */
    hdl->rom->regs->SCR0.LONG = 0x00000000;

    /* SSR register initialize */
    if (1 == RSCI_SSR_ORER)
    {
       RSCI_SSCR_ORERC = 1;
    }

    if (1 == RSCI_SSR_APER)
    {
       RSCI_SSCR_APERC = 1;
    }

    if (1 == RSCI_SSR_AFER)
    {
       RSCI_SSCR_AFERC = 1;
    }

    /* SCR3 register initialize: Transfer Data Invert, Transfer Data Direction Select */
    hdl->rom->regs->SCR3.BIT.DINV = 0;
    hdl->rom->regs->SCR3.BIT.DDIR = 1;

    /* SCR1 register initialize */
    hdl->rom->regs->SCR1.BIT.SPB2DT = 0;
    hdl->rom->regs->SCR1.BIT.SPB2IO = 0;
    hdl->rom->regs->SCR4.BIT.RTADJ  = 0;
    hdl->rom->regs->SCR4.BIT.TTADJ  = 0;
    hdl->rom->regs->SCR1.BIT.RINV   = 0;
    hdl->rom->regs->SCR1.BIT.TINV   = 0;

    /* SCR4 register initialize */
    hdl->rom->regs->SCR4.BIT.RTMG = 0;
    hdl->rom->regs->SCR4.BIT.TTMG = 0;

    /* SCR2/BRR register initialize */
    hdl->rom->regs->SCR2.BIT.BRR = 0xFF;

    /* SCR2 register initialize */
    hdl->rom->regs->SCR2.BIT.BRME      = 0;
    hdl->rom->regs->SCR2.BIT.ABCS      = 0;
    hdl->rom->regs->SCR2.BIT.ABCSE     = 0;
    hdl->rom->regs->SCR1.BIT.NFEN      = 0;
    hdl->rom->regs->SCR2.BIT.BGDM      = 0;
    hdl->rom->regs->SCR3.BIT.RXDESEL   = 0;

    /* SNFR register initialize */
    hdl->rom->regs->SCR1.BIT.NFCS = 0;

    /* SCR1 & SCR3 register initialize: */
    hdl->rom->regs->SCR1.BIT.CTSE = 0;
    hdl->rom->regs->SCR3.LONG &= 0xFFFFFF00;

    /* MMCR register initialize */
    hdl->rom->regs->MMCR.LONG = 0x00000000;

    /* MMSR register initialize */
    hdl->rom->regs->MMSCR.LONG &= 0x00000017;

#if RSCI_CFG_FIFO_INCLUDED
    if (true == hdl->fifo_ctrl)
    {
        /* FCR register initialize */
        hdl->rom->regs->SCR3.BIT.FM   = 0;
        hdl->rom->regs->FCR.BIT.TFRST = 0;
        hdl->rom->regs->FCR.BIT.RFRST = 0;
        hdl->rom->regs->FCR.BIT.TTRG  = 0;
        hdl->rom->regs->FCR.BIT.RTRG  = 8;
    }
#endif /* End of RSCI_CFG_FIFO_INCLUDED */

#if RSCI_CFG_DATA_MATCH_INCLUDED
    /* SCRO & SSR register initialize */
    hdl->rom->regs->SCR0.BIT.DCME  = 0;
    hdl->rom->regs->SSR.BIT.DCMF  = 0;
    hdl->rom->regs->SSR.BIT.DFER  = 0;
    hdl->rom->regs->SSR.BIT.DPER  = 0;
    hdl->rom->regs->SCR0.BIT.IDSEL = 0;

    /* CDR register initialize */
    hdl->rom->regs->SCR4.BIT.CMPD = 0;

    /* Set initial value of receive in 8-bit data length */
    hdl->rom->regs->SCR3.BIT.CHR = 2;
#endif

    return;
} /* End of function rsci_init_register() */

/*****************************************************************************
* Function Name: rsci_init_bit_rate
* Description  : This function determines the best possible settings for the
*                baud rate registers for the specified peripheral clock speed
*                and baud rate. Note that this does not guarantee a low bit 
*                error rate, just the best possible one. The bit rate error is
*                returned in .1% increments. If the hardware cannot support
*                the specified combination, a value of 1000 (100% error) is
*                returned.
*
* NOTE: The transmitter and receiver (TE and RE bits in SCR0) must be disabled 
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
int32_t rsci_init_bit_rate(rsci_hdl_t const  hdl,
                          uint32_t const   pclk,
                          uint32_t const   baud)
{
    uint32_t i;
    uint32_t num_divisors = 0;
    uint32_t ratio;
    uint32_t tmp;
    baud_divisor_t const *p_baud_info = NULL;

    uint32_t divisor;
    uint32_t int_M;
    float    float_M;
    float    error;
    float    abs_error;

#if RSCI_CFG_FIFO_INCLUDED
    uint8_t brr;
#endif

#if RSCI_CFG_PARAM_CHECKING_ENABLE
    if ((0 == pclk) || (0 == baud))
    {
        return 1000;
    }
#endif

    /* SELECT PROPER TABLE BASED UPON MODE */
    if ((RSCI_MODE_ASYNC == hdl->mode) || (RSCI_MODE_MANC == hdl->mode))
    {
#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)
        p_baud_info = rsci_async_baud;
        num_divisors = NUM_DIVISORS_ASYNC;
#endif
    }
    else
    {
        /* SYNC or SSPI */
#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
        p_baud_info = rsci_sync_baud;
        num_divisors = NUM_DIVISORS_SYNC;
#endif
    }

    /* FIND DIVISOR; table has associated ABCS, BGDM, ABCSE and CKS values */
    /* BRR must be 255 or less */
    /* the "- 1" is ignored in some steps for approximations */
    /* BRR = (PCLK/(divisor * baud)) - 1 */
    /* BRR = (ratio / divisor) - 1 */
    ratio = pclk/baud;

    /* WAIT_LOOP */

    for(i = 0; i < num_divisors; i++)
        {
            /* Casting int16_t to uint32_t is valid. Because clock divisor is positive integer */
            if (ratio < (uint32_t)(p_baud_info[i].divisor * 256))
            {
                if ((0 == i) && (RSCI_MODE_MANC == hdl->mode))
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
        }

    /* RETURN IF BRR WILL BE >255 OR LESS THAN 0 */
    if (i == num_divisors)
    {
        return(1000);           // impossible baud rate requested; return 100% error
    }

    /* Casting int16_t to uint32_t is valid. Because clock divisor is a positive integer */
    divisor = (uint32_t)p_baud_info[i].divisor;
    tmp = ratio/(divisor);      // tmp = PCLK/(baud * divisor) = BRR+1 = N+1
    if(0 == tmp)
    {
        return(1000);            // illegal value; return 100% error
    }

    /* SET BRR, ABCS, BDGM, and CKS */
    tmp = ratio / (divisor/2);  // divide by half the divisor

#if RSCI_CFG_FIFO_INCLUDED
    /* Casting is valid. Because result of calculation is in range uint8_t type */
    brr = (uint8_t)((tmp & 0x01) ? (tmp/2) : ((tmp/2)-1));
    if (0 == brr)
    {
        if (true == hdl->fifo_ctrl)
        {
            if ((0x2U == hdl->rom->regs->SCR3.BIT.MOD) || (0x3U == hdl->rom->regs->SCR3.BIT.MOD))
            {
                if (0 == hdl->rom->regs->SCR2.BIT.CKS)
                {
                    return(1000);
                }
            }
        }
    }
#endif

    /* if odd, "round up" by ignoring -1; divide by 2 again for rest of divisor */
    hdl->rom->regs->SCR2.BIT.BRR = (uint8_t)((tmp & 0x01) ? (tmp/2) : ((tmp/2)-1));
    hdl->rom->regs->SCR2.BIT.ABCS = p_baud_info[i].abcs;
    hdl->rom->regs->SCR2.BIT.BGDM = p_baud_info[i].bgdm;
    hdl->rom->regs->SCR2.BIT.CKS = p_baud_info[i].cks;
    hdl->rom->regs->SCR2.BIT.ABCSE= p_baud_info[i].abcse;

    /* Configure internal clock for baud rate */
    hdl->rom->regs->SCR3.BIT.CKE = 0x00;

    /* CALCULATE BIT RATE ERROR.
     * RETURN IF ERROR LESS THAN 1% OR IF IN SYNCHRONOUS/SSPI MODE.
     */
    tmp = ratio/(divisor);      // tmp = PCLK/(baud * divisor) = BRR+1 = N+1

    /* Casting uint32_t to float is valid */
    error = ( ((float)pclk / ((baud * divisor) * tmp)) - 1) * 100;
    abs_error  = (error < 0) ? (-error) : error;

    /* Do not use Modulation Duty Setting in clock synchronous mode, simple SPI mode,
     * smart card Interface mode, manchester mode and extended serial mode.*/
    if ((abs_error <= 1.0) || (RSCI_MODE_ASYNC != hdl->mode))
    {
        hdl->rom->regs->SCR2.BIT.BRME = 0;          // disable MDDR

        /* Casting float to uint32_t */
        return (uint32_t)(error*10);
    }

    /* CALCULATE M ASSUMING A 0% ERROR then WRITE REGISTER */
    hdl->rom->regs->SCR2.BIT.BRR = (uint8_t)(tmp-1);

    /* Casting uint32_t to float is valid  */
    float_M = ((float)((baud * divisor) * 256) * tmp) / pclk;
    float_M *= 2;

    /* Casting float to uint32_t */
    int_M = (uint32_t)float_M;
    int_M = (int_M & 0x01) ? ((int_M/2) + 1) : (int_M/2);

    /* Casting uint32_t type to uint8_t type in this case is valid. Range value of m is not exceed uint8_t */
    hdl->rom->regs->SCR2.BIT.MDDR = (uint8_t)int_M;      // write M
    hdl->rom->regs->SCR2.BIT.BRME = 1;          // enable MDDR

    /* Casting uint32_t to float is valid*/
    error = (( (float)(pclk) / (((divisor * tmp) * baud) * ((float)(256)/int_M)) ) - 1) * 100;

    /* Casting float to int32_t */
    return (int32_t)(error*10);
} /* End of function rsci_init_bit_rate() */

/*****************************************************************************
* Function Name: rsci_initialize_ints
* Description  : This function sets priority, clears flags, and sets 
*                interrupts in both the ICU and SCI peripheral. These include 
*                RXI, TXI, TEI, and ERI/GROUP12 interrupts.
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
*                priority -
*                    priority for interrupts
* Return Value : none
******************************************************************************/
void rsci_initialize_ints(rsci_hdl_t const hdl,
                         uint8_t const   priority)
{
    volatile bsp_int_ctrl_t group_priority;

    /* SET PRIORITY FOR INTERRUPTS */
    *hdl->rom->ipr_rxi = priority;          // can set separately using Control()
    *hdl->rom->ipr_txi = priority;

    group_priority.ipl = 0x00000000;

#if ((RSCI_CFG_CH10_INCLUDED == 1) || (RSCI_CFG_CH11_INCLUDED == 1))
    /* Check interrupt priority */
    if (RSCI_CFG_ERI_TEI_PRIORITY > IPR(ICU, GROUPAL0))
    {
        /* Casting a positive integer to uint32_t is valid */
        group_priority.ipl = (uint32_t)RSCI_CFG_ERI_TEI_PRIORITY;
    }
#endif

    /* DISABLE ERI INTERRUPT */
    DISABLE_ERI_INT;

    /* DISABLE RXI INTERRUPT */
    DISABLE_RXI_INT;

    /* DISABLE TXI INTERRUPT */
    DISABLE_TXI_INT;

    /* DISABLE TEI INTERRUPT */
    DISABLE_TEI_INT;

    /* CLEAR INTERRUPT FLAGS */
    *hdl->rom->ir_rxi = 0;
    *hdl->rom->ir_txi = 0;
    (*hdl->rom->icu_grp) &= (~hdl->rom->tei_ch_mask);
    (*hdl->rom->icu_grp) &= (~hdl->rom->eri_ch_mask);

    /* REGISTER GROUP INTERRUPTS WITH BSP */
    #if RSCI_CFG_TEI_INCLUDED
    R_BSP_InterruptWrite(hdl->rom->tei_vector, hdl->rom->tei_isr);
    #endif
    R_BSP_InterruptWrite(hdl->rom->eri_vector, hdl->rom->eri_isr);

    /* ENABLE GROUP INTERRUPTS */
    R_BSP_InterruptControl(hdl->rom->eri_vector, BSP_INT_CMD_GROUP_INTERRUPT_ENABLE, (void *)&group_priority);

    /* ENABLE ERI AND RXI INTERRUPTS REQUESTS */
    ENABLE_ERI_INT;
    ENABLE_RXI_INT;

    /* ENABLE INTERRUPTS IN RSCI PERIPHERAL */
    /* Note: Enable interrupts after xcvr or will get "extra" interrupt */
    hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;    // enable TE, RE, TXI, and RXI/ERI

    return;    
} /* End of function rsci_initialize_ints() */

/*****************************************************************************
* Function Name: rsci_disable_ints
* Description  : This function disable interrupts in both the ICU and RSCI 
*                peripheral. These include RXI, TXI, TEI, ERI, and group
*                interrupts.
* Arguments    : hdl - 
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
void rsci_disable_ints(rsci_hdl_t const hdl)
{
    volatile bsp_int_ctrl_t group_priority;

    /* Disable ICU RXI interrupt */
    DISABLE_RXI_INT;

    /* Disable ICU TXI interrupt */
    DISABLE_TXI_INT;

    /* Disable ICU ERI interrupt */
    DISABLE_ERI_INT;

    /* Disable ICU TEI interrupt */
    DISABLE_TEI_INT;
    
    /* disable peripheral interrupts and xcvr (TE and RE) */
    hdl->rom->regs->SCR0.LONG = 0;

    /* disable group interrupts */
    group_priority.ipl = 0x00000000;

    /* Casting pointer to void* is valid */
    R_BSP_InterruptControl(hdl->rom->eri_vector, BSP_INT_CMD_GROUP_INTERRUPT_DISABLE, (void *)&group_priority);

    return;
} /* End of function rsci_disable_ints() */


#if (RSCI_CFG_ASYNC_INCLUDED)
/*****************************************************************************
* Function Name: rsci_async_cmds
* Description  : This function configures non-standard UART hardware and
*                performs special software operations.
*
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                cmd -
*                    command to run
*                p_args -
*                    pointer argument(s) specific to command
* Return Value : RSCI_SUCCESS -
*                    Command completed successfully.
*                RSCI_ERR_NULL_PTR -
*                    p_args is NULL when required for cmd
*                RSCI_ERR_INVALID_ARG -
*                    The cmd value or p_args contains an invalid value.
******************************************************************************/
rsci_err_t rsci_async_cmds(rsci_hdl_t const hdl,
                         rsci_cmd_t const cmd,
                         void            *p_args)
{
    rsci_err_t   err=RSCI_SUCCESS;
    int32_t     bit_err;
    uint32_t    slow_baud;

#if RSCI_CFG_PARAM_CHECKING_ENABLE

    /* Check parameters */
    if (((NULL == p_args) || (FIT_NO_PTR == p_args))
     && ((RSCI_CMD_TX_Q_BYTES_FREE == cmd) || (RSCI_CMD_RX_Q_BYTES_AVAIL_TO_READ == cmd)|| (RSCI_CMD_COMPARE_RECEIVED_DATA == cmd)))
    {
        return RSCI_ERR_NULL_PTR;
    }

#endif

    switch(cmd)
    {
        case (RSCI_CMD_EN_NOISE_CANCEL):
        {
            hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
            RSCI_SCR0_DUMMY_READ;
            hdl->rom->regs->SCR1.BIT.NFEN = 1;      /* enable noise filter */
            hdl->rom->regs->SCR1.BIT.NFCS = 0;          /* clock divided by 1 (default) */
            RSCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            break;
        }

        case (RSCI_CMD_OUTPUT_BAUD_CLK):
        {
            hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
            RSCI_SCR0_DUMMY_READ;
            hdl->rom->regs->SCR3.BIT.CKE = 0x01;     /* output baud clock on SCK pin */
            RSCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            break;
        }

        case (RSCI_CMD_START_BIT_EDGE):
        {
            hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
            RSCI_SCR0_DUMMY_READ;
            hdl->rom->regs->SCR3.BIT.RXDESEL = 1;   /* detect start bit on falling edge */
            RSCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            break;
        }

    #if RSCI_CFG_TEI_INCLUDED
        case (RSCI_CMD_EN_TEI):  /* SCI_CMD_EN_TEI is obsolete command, but it exists only for compatibility with older version. */
        {
            break;
        }
    #endif

        case (RSCI_CMD_TX_Q_FLUSH):
        {
            /* Disable TXI interrupt */
            DISABLE_TXI_INT;
            R_BYTEQ_Flush(hdl->u_tx_data.que);
            ENABLE_TXI_INT;
            break;
        }

        case (RSCI_CMD_RX_Q_FLUSH):
        {
            /* Disable RXI interrupt */
            DISABLE_RXI_INT;
            R_BYTEQ_Flush(hdl->u_rx_data.que);
            ENABLE_RXI_INT;
            break;
        }

        case (RSCI_CMD_TX_Q_BYTES_FREE):
        {
            /* Casting pointer void* to uint16_t* type is valid */
            R_BYTEQ_Unused(hdl->u_tx_data.que, (uint16_t *) p_args);
            break;
        }

        case (RSCI_CMD_RX_Q_BYTES_AVAIL_TO_READ):
        {
            /* Casting pointer void* type to uint16_t* type is valid  */
            R_BYTEQ_Used(hdl->u_rx_data.que, (uint16_t *) p_args);
            break;
        }

        case (RSCI_CMD_GENERATE_BREAK):
        {
            /* flush transmit queue */
            DISABLE_TXI_INT;
            R_BYTEQ_Flush(hdl->u_tx_data.que);
            
            ENABLE_TXI_INT;

            /* NOTE: the following steps will abort anything being sent */

            /* set baud rate 1.5x slower */
            slow_baud = (hdl->baud_rate << 1) / 3;
            hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
            RSCI_SCR0_DUMMY_READ;
            bit_err = rsci_init_bit_rate(hdl, hdl->pclk_speed, slow_baud);
            RSCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            if (1000 == bit_err)
            {
                err = RSCI_ERR_INVALID_ARG;
            }
            else
            {
                /* transmit "0" and wait for completion */
                RSCI_TDR(0);

                /* WAIT_LOOP */
                while (0 == hdl->rom->regs->SSR.BIT.TEND)
                {
                    R_BSP_NOP();
                }

                /* restore original baud rate */
                hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
                RSCI_SCR0_DUMMY_READ;
                rsci_init_bit_rate(hdl, hdl->pclk_speed, hdl->baud_rate);
                RSCI_IR_TXI_CLEAR;
                hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            }
            break;
        }

    #if RSCI_CFG_DATA_MATCH_INCLUDED
        case RSCI_CMD_COMPARE_RECEIVED_DATA:
        {
            hdl->rom->regs->SSR.BIT.DFER = 0; /* Clear Match Data Framing Error Flag */
            hdl->rom->regs->SSR.BIT.DPER = 0; /* Clear Match Data Parity Error Flag */
            hdl->rom->regs->SCR0.BIT.DCME = 1; /* Enable Data match function */
            hdl->rom->regs->SCR4.BIT.CMPD = *((unsigned char *)p_args); /* Comparison data */
            break;
        }
    #endif

    #if (RSCI_CFG_RX_DATA_SAMPLING_TIMING_INCLUDED || RSCI_CFG_TX_SIGNAL_TRANSITION_TIMING_INCLUDED)
        /* Enable receive data sampling timing adjust feature*/
        case RSCI_CMD_RX_SAMPLING_ENABLE:
        {
            hdl->rom->regs->SCR4.BIT.RTADJ = 1;
            break;
        }

        /* Disable receive data sampling timing adjust feature*/
        case RSCI_CMD_RX_SAMPLING_DISABLE:
        {
            hdl->rom->regs->SCR4.BIT.RTADJ = 0;
            break;
        }

        /* Enable transmit signal transition timing adjust feature*/
        case RSCI_CMD_TX_TRANSITION_TIMING_ENABLE:
        {
            hdl->rom->regs->SCR4.BIT.TTADJ = 1;
            break;
        }

        /* Disable transmit signal transition timing adjust feature*/
        case RSCI_CMD_TX_TRANSITION_TIMING_DISABLE:
        {
            hdl->rom->regs->SCR4.BIT.TTADJ = 0;
            break;
        }

        /* Set value for receive data sampling timing adjust feature*/
        case RSCI_CMD_SAMPLING_TIMING_ADJUST:
        {
            if ((0 == hdl->rom->regs->SCR2.BIT.ABCSE) && (0 == hdl->rom->regs->SCR2.BIT.ABCS))
            {
                /* Casting pointer void* to uint8_t* type is valid */
                if ((*(uint8_t *)p_args) <= 15)
                {
                    if (1 == hdl->rom->regs->SCR4.BIT.RTADJ)
                    {
                        /* Casting pointer void* to uint8_t* type is valid */
                        hdl->rom->regs->SCR4.BIT.RTMG = *(uint8_t *)p_args;
                    }
                }
                else
                {
                    err = RSCI_ERR_INVALID_ARG;
                }
            }
            else if ((0 == hdl->rom->regs->SCR2.BIT.ABCSE) && (1 == hdl->rom->regs->SCR2.BIT.ABCS))
            {
                /* Casting pointer void* to uint8_t* type is valid */
                if (((*(uint8_t *)p_args) <= 3) || ((8 <= (*(uint8_t *)p_args)) && ((*(uint8_t *)p_args) <= 11)))
                {
                    if (1 == hdl->rom->regs->SCR4.BIT.RTADJ)
                    {
                        /* Casting pointer void* to uint8_t* type is valid */
                        hdl->rom->regs->SCR4.BIT.RTMG = *(uint8_t *)p_args;
                    }
                }
                else
                {
                    err = RSCI_ERR_INVALID_ARG;
                }
            }
            else
            {
                /* Casting pointer void* to uint8_t* type is valid */
                if (((*(uint8_t *)p_args) <= 2) || ((8 <= (*(uint8_t *)p_args)) && ((*(uint8_t *)p_args) <= 10)))
                {
                    if (1 == hdl->rom->regs->SCR4.BIT.RTADJ)
                    {
                        /* Casting pointer void* to uint8_t* type is valid */
                        hdl->rom->regs->SCR4.BIT.RTMG = *(uint8_t *)p_args;
                    }
                }
                else
                {
                    err = RSCI_ERR_INVALID_ARG;
                }
            }
            break;
        }
        /*Set va transmit signal transition timing adjust feature*/
        case RSCI_CMD_TRANSITION_TIMING_ADJUST:
        {
           if (0 == hdl->rom->regs->SCR2.BIT.ABCSE)
           {
               /* Casting pointer void* to uint8_t* type is valid */
               if ((*(uint8_t *)p_args) <= 15)
               {
                   if (1 == hdl->rom->regs->SCR4.BIT.TTADJ)
                   {
                       /* Casting pointer void* to uint8_t* type is valid */
                        hdl->rom->regs->SCR4.BIT.TTMG = *(uint8_t *)p_args;
                   }
               }
               else
               {
                    err = RSCI_ERR_INVALID_ARG;
               }
           }
           else
           {
               /* Casting pointer void* to uint8_t* type is valid */
               if (((*(uint8_t *)p_args) <= 5) || ((8 <= (*(uint8_t *)p_args)) && ((*(uint8_t *)p_args) <= 13)))
               {
                   if (1 == hdl->rom->regs->SCR4.BIT.TTADJ)
                   {
                       /* Casting pointer void* to uint8_t* type is valid */
                        hdl->rom->regs->SCR4.BIT.TTMG = *(uint8_t *)p_args;
                   }
               }
               else
               {
                    err = RSCI_ERR_INVALID_ARG;
               }
           }

        break;
       }

    #endif
        default:
        {
            err = RSCI_ERR_INVALID_ARG;
            break;
        }
    }

    return err;
} /* End of function rsci_async_cmds() */
#endif /* End of RSCI_CFG_ASYNC_INCLUDED */

#if (RSCI_CFG_MANC_INCLUDED)
/*****************************************************************************
* Function Name: rsci_manc_cmds
* Description  : This function performs special software operations specific
*                to the Manchester code communication.
*
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                cmd -
*                    command to run
*                p_args -
*                    pointer argument(s) specific to command
* Return Value : RSCI_SUCCESS -
*                    Command completed successfully.
*                RSCI_ERR_NULL_PTR -
*                    p_args is NULL when required for cmd
*                RSCI_ERR_INVALID_ARG -
*                    The cmd value or p_args contains an invalid value.
******************************************************************************/
rsci_err_t rsci_manc_cmds(rsci_hdl_t const hdl,
                         rsci_cmd_t const cmd,
                         void            *p_args)
{
    rsci_err_t   err=RSCI_SUCCESS;

#if RSCI_CFG_PARAM_CHECKING_ENABLE

    /* Check parameters */
    if (((NULL == p_args) || (FIT_NO_PTR == p_args))
     && ((RSCI_CMD_TX_Q_BYTES_FREE == cmd) || (RSCI_CMD_RX_Q_BYTES_AVAIL_TO_READ == cmd)
        || (RSCI_CMD_SET_TRASMIT_PREFACE_LENGTH == cmd) || (RSCI_CMD_SET_RECEIVE_PREFACE_LENGTH == cmd)))
    {
        return RSCI_ERR_NULL_PTR;
    }
    if (RSCI_CMD_SET_TRASMIT_PREFACE_LENGTH == cmd)
    {
        /* Casting void* type is valid */
        if (15 < (*(uint8_t *)p_args))
        {
            return RSCI_ERR_INVALID_ARG;
        }
    }
    if (RSCI_CMD_SET_RECEIVE_PREFACE_LENGTH == cmd)
    {
        /* Casting void* type is valid */
        if (15 < (*(uint8_t *)p_args))
        {
            return RSCI_ERR_INVALID_ARG;
        }
    }
#endif

    switch(cmd)
    {
        case (RSCI_CMD_TX_Q_FLUSH):
        {
            /* Disable TXI interrupt */
            DISABLE_TXI_INT;
            R_BYTEQ_Flush(hdl->u_tx_data.que);
            ENABLE_TXI_INT;
            break;
        }

        case (RSCI_CMD_RX_Q_FLUSH):
        {
            /* Disable RXI interrupt */
            DISABLE_RXI_INT;
            R_BYTEQ_Flush(hdl->u_rx_data.que);
            ENABLE_RXI_INT;
            break;
        }

        case (RSCI_CMD_TX_Q_BYTES_FREE):
        {
            /* Casting pointer void* to uint16_t* type is valid */
            R_BYTEQ_Unused(hdl->u_tx_data.que, (uint16_t *) p_args);
            break;
        }

        case (RSCI_CMD_RX_Q_BYTES_AVAIL_TO_READ):
        {
            /* Casting pointer void* type to uint16_t* type is valid  */
            R_BYTEQ_Used(hdl->u_rx_data.que, (uint16_t *) p_args);
            break;
        }

        case (RSCI_CMD_SET_TRASMIT_PREFACE_LENGTH):
        {
            /* change TX preface length */
            hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
            RSCI_SCR0_DUMMY_READ;

            /* Casting void* type is valid */
            hdl->rom->regs->MMCR.BIT.TPLEN = *((uint8_t *)p_args);
            RSCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            break;
        }

        case (RSCI_CMD_SET_RECEIVE_PREFACE_LENGTH):
        {
            /* change RX preface length */
            hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
            RSCI_SCR0_DUMMY_READ;

            /* Casting void* type is valid */
            hdl->rom->regs->MMCR.BIT.RPLEN = *((uint8_t *)p_args);
            RSCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            break;
        }

        case (RSCI_CMD_START_BIT_PATTERN_LOW_TO_HIGH):
        {
            hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
            RSCI_SCR0_DUMMY_READ;
            hdl->rom->regs->MMCR.BIT.SBPTN = 0;
            RSCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            break;
        }

        case (RSCI_CMD_START_BIT_PATTERN_HIGH_TO_LOW):
        {
            hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
            RSCI_SCR0_DUMMY_READ;
            hdl->rom->regs->MMCR.BIT.SBPTN = 1;
            RSCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            break;
        }

        case (RSCI_CMD_EN_SYNC):
        {
            hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
            RSCI_SCR0_DUMMY_READ;
            hdl->rom->regs->MMCR.BIT.SYNCE = 1;

            /* Update start bit length to using sync pulse select
             * , regardless of the assigned initial value of this bit
             */
            hdl->rom->regs->MMCR.BIT.SBLEN = 1;
            RSCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            break;
        }

        default:
        {
            err = RSCI_ERR_INVALID_ARG;
            break;
        }
    }

    return err;
} /* End of function rsci_manc_cmds() */
#endif /* End of RSCI_CFG_MANC_INCLUDED */

#if (RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED)
/*****************************************************************************
* Function Name: rsci_sync_cmds
* Description  : This function performs special software operations specific
*                to the SSPI and SYNC protocols.
*
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                cmd -
*                    command to run
*                p_args -
*                    pointer argument(s) specific to command
* Return Value : RSCI_SUCCESS -
*                    Command completed successfully.
*                RSCI_ERR_NULL_PTR -
*                    p_args is NULL when required for cmd
*                RSCI_ERR_INVALID_ARG -
*                    The cmd value or p_args contains an invalid value.
*                    May be due to mode channel is operating in.
******************************************************************************/
rsci_err_t rsci_sync_cmds(rsci_hdl_t const hdl,
                        rsci_cmd_t const cmd,
                        void            *p_args)
{
    rsci_spi_mode_t  spi_mode;
    rsci_cb_args_t   args;
    rsci_err_t       err = RSCI_SUCCESS;

    switch (cmd)
    {
        case (RSCI_CMD_CHECK_XFER_DONE):
        {
            if (false == hdl->tx_idle)
            {
                err = RSCI_ERR_XFER_NOT_DONE;
            }
            break;
        }

        case (RSCI_CMD_ABORT_XFER):
        {
            /* Disable receive interrupts in ICU and peripheral */
            DISABLE_RXI_INT;
            DISABLE_ERI_INT;
            hdl->rom->regs->SCR0.LONG &= (~(RSCI_SCR0_RIE_MASK | RSCI_SCR0_RE_MASK | RSCI_SCR0_TE_MASK));

            hdl->tx_cnt = 0;
            hdl->tx_dummy = false;
            hdl->tx_idle = true;

            /* Do callback if available */
            if ((NULL != hdl->callback) && (FIT_NO_FUNC != hdl->callback))
            {
                args.hdl = hdl;
                args.event = RSCI_EVT_XFER_ABORTED;

                /* Casting pointer to void* is valid */
                hdl->callback((void *)&args);
            }

            *hdl->rom->ir_rxi = 0;                  /* clear rxi interrupt flag */


            (*hdl->rom->icu_grp) &= (~hdl->rom->eri_ch_mask);  /* clear eri interrupt flag */

            ENABLE_ERI_INT;                         /* enable rx err interrupts in ICU */
            ENABLE_RXI_INT;                         /* enable receive interrupts in ICU */

            /* Enable receive interrupt in peripheral after rcvr or will get "extra" interrupt */
            hdl->rom->regs->SCR0.LONG |= (RSCI_SCR0_RE_MASK | RSCI_SCR0_TE_MASK);
            hdl->rom->regs->SCR0.LONG |= RSCI_SCR0_RIE_MASK;
            break;
        }
        case (RSCI_CMD_CHANGE_SPI_MODE):
        {
    #if RSCI_CFG_PARAM_CHECKING_ENABLE

            if (RSCI_MODE_SSPI != hdl->mode)
            {
                return RSCI_ERR_INVALID_ARG;
            }

            /* Check parameters */
            if ((NULL == p_args ) || (FIT_NO_PTR == p_args))
            {
                return RSCI_ERR_NULL_PTR;
            }

            /* Casting pointer void* type is valid */
            spi_mode = *((rsci_spi_mode_t *)p_args);

            if ((RSCI_SPI_MODE_0 != spi_mode) && (RSCI_SPI_MODE_1 != spi_mode)
              && (RSCI_SPI_MODE_2 != spi_mode) && (RSCI_SPI_MODE_3 != spi_mode))
            {
                return RSCI_ERR_INVALID_ARG;
            }
    #endif
            hdl->rom->regs->SCR0.LONG &= (~RSCI_EN_XCVR_MASK);
            RSCI_SCR0_DUMMY_READ;
            hdl->rom->regs->SCR3.LONG &= 0xFFFFFF00;      /* clear previous mode */
            hdl->rom->regs->SCR3.LONG |= (*((uint8_t *)p_args));
            RSCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR0.LONG |= RSCI_EN_XCVR_MASK;
            break;
        }

        default:
        {
            err = RSCI_ERR_INVALID_ARG;
            break;
        }
    }

    return err;
} /* End of function rsci_sync_cmds() */
#endif /* End of RSCI_CFG_SSPI_INCLUDED || RSCI_CFG_SYNC_INCLUDED */

/*****************************************************************************
ISRs
******************************************************************************/


#if (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED)

/*****************************************************************************
* rsciN_txiN_isr
* Description  : TXI interrupt routines for every RSCI channel
******************************************************************************/


#if RSCI_CFG_CH10_INCLUDED
/*******************************************************************************
 * Function Name: rsci10_txi_isr
 * Description  : TXI interrupt routines for RSCI10 channel
 ******************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(rsci10_txi_isr, VECT(RSCI10,TXI))
R_BSP_ATTRIB_STATIC_INTERRUPT void rsci10_txi_isr(void)
{
    rsci_txi_handler(&g_rsci_ch10_ctrl);
} /* End of function rsci10_txi_isr() */
#endif /* End of RSCI_CFG_CH10_INCLUDED  */

#if RSCI_CFG_CH11_INCLUDED
/*******************************************************************************
 * Function Name: rsci11_txi_isr
 * Description  : TXI interrupt routines for RSCI11 channel
 ******************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(rsci11_txi_isr, VECT(RSCI11,TXI))
R_BSP_ATTRIB_STATIC_INTERRUPT void rsci11_txi_isr(void)
{
    rsci_txi_handler(&g_rsci_ch11_ctrl);
} /* End of function rsci11_txi_isr() */
#endif /* End of RSCI_CFG_CH11_INCLUDED  */

#endif /* End of (RSCI_CFG_ASYNC_INCLUDED || RSCI_CFG_MANC_INCLUDED) */

#if RSCI_CFG_TEI_INCLUDED
/*****************************************************************************
* rsciN_teiN_isr
*
* Description  : TEI interrupt routines for every RSCI channel.
*                BSP gets main group interrupt, then vectors to/calls these
*                "interrupts"/callbacks.
******************************************************************************/


#if RSCI_CFG_CH10_INCLUDED
/*****************************************************************************
* Function Name: rsci10_tei_isr
* Description  : TEI interrupt routines for RSCI10 channel.
******************************************************************************/
void rsci10_tei_isr(void *cb_args)
{
    rsci_tei_handler(&g_rsci_ch10_ctrl);
} /* End of function rsci10_tei_isr() */
#endif /* End of RSCI_CFG_CH10_INCLUDED */

#if RSCI_CFG_CH11_INCLUDED
/*****************************************************************************
* Function name: rsci11_tei_isr
* Description  : TEI interrupt routines for RSCI11 channel.
******************************************************************************/
void rsci11_tei_isr(void *cb_args)
{
    rsci_tei_handler(&g_rsci_ch11_ctrl);
} /* End of function rsci11_tei_isr() */
#endif /* End of RSCI_CFG_CH11_INCLUDED */

#endif /* RSCI_CFG_TEI_INCLUDED */

/*****************************************************************************
* rsciN_rxi_isr
* Description  : RXI interrupt routines for every RSCI channel
******************************************************************************/


#if RSCI_CFG_CH10_INCLUDED
/*******************************************************************************
 * Function Name: rsci10_rxi_isr
 * Description  : RXI interrupt routines for RSCI10 channel
 ******************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(rsci10_rxi_isr, VECT(RSCI10,RXI))
R_BSP_ATTRIB_STATIC_INTERRUPT void rsci10_rxi_isr(void)
{
    rsci_rxi_handler(&g_rsci_ch10_ctrl);
} /* End of function rsci10_rxi_isr() */
#endif /* End of RSCI_CFG_CH10_INCLUDED */

#if RSCI_CFG_CH11_INCLUDED
/*******************************************************************************
 * Function Name: rsci11_rxi_isr
 * Description  : RXI interrupt routines for RSCI11 channel
 ******************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(rsci11_rxi_isr, VECT(RSCI11,RXI))
R_BSP_ATTRIB_STATIC_INTERRUPT void rsci11_rxi_isr(void)
{
    rsci_rxi_handler(&g_rsci_ch11_ctrl);
} /* End of function rsci11_rxi_isr() */
#endif /* End of RSCI_CFG_CH11_INCLUDED */


/*****************************************************************************
* rsciN_eri_isr
*
* Description  : ERI interrupt routines for every RSCI channel.
*                BSP gets main group interrupt, then vectors to/calls these
*                "interrupts"/callbacks.
******************************************************************************/


#if RSCI_CFG_CH10_INCLUDED
/*****************************************************************************
* Function name: rsci10_eri_isr
* Description  : ERI interrupt routines for RSCI10 channel.
******************************************************************************/
void rsci10_eri_isr(void *cb_args)
{
    rsci_eri_handler(&g_rsci_ch10_ctrl);
} /* End of function rsci10_eri_isr() */
#endif /* End of RSCI_CFG_CH10_INCLUDED */

#if RSCI_CFG_CH11_INCLUDED
/*****************************************************************************
* Function name: rsci11_eri_isr
* Description  : ERI interrupt routines for RSCI11 channel.
******************************************************************************/
void rsci11_eri_isr(void *cb_args)
{
    rsci_eri_handler(&g_rsci_ch11_ctrl);
} /* End of function rsci11_eri_isr() */
#endif /* End of RSCI_CFG_CH11_INCLUDED */

