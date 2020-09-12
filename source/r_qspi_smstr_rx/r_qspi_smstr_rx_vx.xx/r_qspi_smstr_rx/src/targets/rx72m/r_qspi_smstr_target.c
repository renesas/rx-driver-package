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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* System Name  : QSPI single master driver
* File Name    : r_qspi_smstr_target.c
* Version      : 1.13
* Device       : RX
* Abstract     : Main processing source file for QSPI single master driver
* Tool-Chain   : Renesas RXC Toolchain v3.01.00
* OS           : not use
* H/W Platform : not use
* Description  : Functions for QSPI single master driver
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 30.07.2019 1.13     First Release
*******************************************************************************/

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
/* Public interface header file for this package. */
#include "r_qspi_smstr_rx_if.h"
/* Configuration for this package. */
#include "r_qspi_smstr_rx_config.h"
/* Private header file for this package. */
#include "./src/r_qspi_smstr_private.h"

/* Check MCU Group */
#if defined(BSP_MCU_RX72M)


/*******************************************************************************
Macro definitions
*******************************************************************************/
/* ---- Definitions of mask data for I/O registers. ---- */
/* Casting the macro definitions to uint16_t type is valid. 
   Because match the type of the processing. */
#define SPCMD_MASK_CLK_MODE     (uint16_t)(0x0003)  /* SPI clock mode of SPCMD.             */

/* ---- Definitions of value of transmit data length. ---- */
/* Casting the macro definitions to uint8_t type is valid. 
   Because match the type of the processing. */
#define SPCMD_SPB_LONG          (uint8_t)(2)        /* Transfer data length : 32 bits       */
#define SPCMD_SPB_BYTE          (uint8_t)(0)        /* Transfer data length :  8 bits       */

/* ---- Definition to check data size. ---- */
/* Casting the macro definitions to uint32_t type is valid. 
   Because match the type of the processing. */
#define QSPI_FIFO_CHK           (uint32_t)(0x000f)

/*******************************************************************************
Private variables and functions
*******************************************************************************/
static void r_qspi_smstr_spsr_clear(uint8_t channel);
static void r_qspi_smstr_buffer_reset(uint8_t channel);
static void r_qspi_smstr_datasize_set(uint8_t channel, uint32_t size);

/* Array of address of SPSR. */
static volatile uint8_t R_BSP_EVENACCESS_SFR * g_qspi_spsr[] =
{
#if (1 == QSPI_NUM_CHANNELS)
    &(QSPI.SPSR.BYTE),
#endif
};

/* Array of base address of I/O registers of QSPI. */
static volatile struct st_qspi R_BSP_EVENACCESS_SFR * g_qspi_base_adr[] =
{
#if (1 == QSPI_NUM_CHANNELS)
    &QSPI
#endif
};


/*******************************************************************************
* Function Name: r_qspi_smstr_ch_check
* Description  : Checks whether channel is valid or not.
* Arguments    : channel -
*                    Which channel to use
* Return Value : QSPI_SMSTR_SUCCESS -
*                    The channel is valid.
*                QSPI_SMSTR_ERR_PARAM -
*                    Parameter error
*******************************************************************************/
qspi_smstr_status_t r_qspi_smstr_ch_check(uint8_t channel)
{
    qspi_smstr_status_t ret = QSPI_SMSTR_SUCCESS;
    
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            /* Do nothing */
#else
            /* QSPI channel 0 is invalid. */
            ret = QSPI_SMSTR_ERR_PARAM;
#endif
        break;
        
        default:
            /* The channel number is invalid. */
            ret = QSPI_SMSTR_ERR_PARAM;
        break;
    }
    
    return ret;
}


/*******************************************************************************
* Function Name: r_qspi_smstr_enable
* Description  : Enables QSPI and initializes registers.
* Arguments    : channel -
*                    Which channel to use
*                br_data -
*                    Setting of QSPI Bit Rate Register (SPBR)
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_enable(uint8_t channel, uint8_t br_data)
{
    r_qspi_smstr_module_enable(channel);            /* Cancel the Module Stop of QSPI.  */
    QSPI_SPCR(channel)      = QSPI_SPCR_INIT;
                                        /* Disable QSPI function and set master mode.   */
    if (QSPI_SPCR_INIT == QSPI_SPCR(channel))
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
    QSPI_SSLP(channel)      = QSPI_SSLP_INIT;       /* Initialize SSLP.                 */
    QSPI_SPPCR(channel)     = QSPI_SPPCR_INIT;      /* Initialize SPPCR.                */
    QSPI_SPBR(channel)      = br_data;              /* Set bit rate.                    */
    QSPI_SPCKD(channel)     = QSPI_SPCKD_INIT;      /* Initialize SPCKD.                */
    QSPI_SSLND(channel)     = QSPI_SSLND_INIT;      /* Initialize SSLND.                */
    QSPI_SPND(channel)      = QSPI_SPND_INIT;       /* Initialize SPND.                 */
    QSPI_SPDCR(channel)     = QSPI_SPDCR_INIT;      /* Initialize SPDCR.                */
    r_qspi_smstr_spsr_clear(channel);               /* Clear status flag of QSPI.       */
    QSPI_SPSCR(channel)     = QSPI_SPSCR_INIT;      /* Initialize SPSCR.                */
    QSPI_SPBFCR(channel)    = QSPI_SPBFCR_INIT;     /* Initialize SPBFCR.               */
    if (QSPI_SPBFCR_INIT == QSPI_SPBFCR(channel))
    {
        /* According to specifications of QSPI,
           if write SPBFCR, must execute dummy reading. */
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
    QSPI_SPCMD0(channel)    = QSPI_SPCMD_INIT;      /* Initialize SPCMD0.               */
    QSPI_SPCMD1(channel)    = QSPI_SPCMD_INIT;      /* Initialize SPCMD1.               */
    QSPI_SPCMD2(channel)    = QSPI_SPCMD_INIT;      /* Initialize SPCMD2.               */
    if (QSPI_SPCMD_INIT  == QSPI_SPCMD2(channel))
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_disable
* Description  : Stops QSPI.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_disable(uint8_t channel)
{
    r_qspi_smstr_module_enable(channel);            /* Cancel the Module Stop of QSPI.  */
    r_qspi_smstr_spsr_clear(channel);               /* Clear status flags of QSPI.      */
    r_qspi_smstr_module_disable(channel);           /* Set the Module Stop of QSPI.     */
}


/*******************************************************************************
* Function Name: r_qspi_smstr_change
* Description  : Changes setting of SPI clock mode and bit rate of QSPI.
* Arguments    : channel -
*                    Which channel to use
*                mode_num -
*                    Number of SPI clock mode
*                        0: CPOL=0, CPHA=0
*                        1: CPOL=0, CPHA=1
*                        2: CPOL=1, CPHA=0
*                        3: CPOL=1, CPHA=1
*                br_data -
*                    Setting of QSPI Bit Rate Register (SPBR)
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_change(uint8_t channel, uint8_t mode_num, uint8_t br_data)
{
    uint16_t tmp_mode;
    
    /* Cast the variable to a uint16_t type. */
    tmp_mode = (uint16_t)(SPCMD_MASK_CLK_MODE & ((uint16_t)mode_num));
    
    /* Set bit rate. */
    QSPI_SPBR(channel)      = br_data;
    
    /* ---- Change SPI clock mode. ---- */
    /* Cast the variable to a uint16_t type. */
    QSPI_SPCMD0(channel)    = (uint16_t)((QSPI_SPCMD0(channel) & ~SPCMD_MASK_CLK_MODE) | tmp_mode);
    QSPI_SPCMD1(channel)    = (uint16_t)((QSPI_SPCMD1(channel) & ~SPCMD_MASK_CLK_MODE) | tmp_mode);
    QSPI_SPCMD2(channel)    = (uint16_t)((QSPI_SPCMD2(channel) & ~SPCMD_MASK_CLK_MODE) | tmp_mode);
    if (0 == QSPI_SPCMD2(channel))
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_data_set_long
* Description  : Writes longword data to QSPI Data Register.
* Arguments    : channel -
*                    Which channel to use
*                set_data -
*                    Data to be written
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_data_set_long(uint8_t channel, uint32_t set_data)
{
    QSPI_SPDR_LONG(channel) = set_data;
}


/*******************************************************************************
* Function Name: r_qspi_smstr_data_set_byte
* Description  : Writes byte data to QSPI Data Register.
* Arguments    : channel -
*                    Which channel to use
*                set_data -
*                    Data to be written
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_data_set_byte(uint8_t channel, uint8_t set_data)
{
    QSPI_SPDR_BYTE(channel) = set_data;
}


/*******************************************************************************
* Function Name: r_qspi_smstr_data_get_long
* Description  : Reads longword data from QSPI Data Register.
* Arguments    : channel -
*                    Which channel to use
* Return Value : Readout data
*******************************************************************************/
uint32_t r_qspi_smstr_data_get_long(uint8_t channel)
{
    return QSPI_SPDR_LONG(channel);
}


/*******************************************************************************
* Function Name: r_qspi_smstr_data_get_byte
* Description  : Reads byte data from QSPI Data Register.
* Arguments    : channel -
*                    Which channel to use
* Return Value : Readout data
*******************************************************************************/
uint8_t r_qspi_smstr_data_get_byte(uint8_t channel)
{
    return QSPI_SPDR_BYTE(channel);
}


/*******************************************************************************
* Function Name: r_qspi_smstr_spsr_clear
* Description  : Clears status flags of QSPI.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
static void r_qspi_smstr_spsr_clear(uint8_t channel)
{
    uint8_t     uc_tmp = 0;

    if (0x00 != (QSPI_SPSR(channel) & (QSPI_SMSTR_MASK_SPSSLF | QSPI_SMSTR_MASK_SPTEF | QSPI_SMSTR_MASK_SPRFF)))
    {
        uc_tmp |= ~(QSPI_SMSTR_MASK_SPSSLF | QSPI_SMSTR_MASK_SPTEF | QSPI_SMSTR_MASK_SPRFF);
        QSPI_SPSR(channel) &= uc_tmp;
        if (QSPI_SPSR_INIT == QSPI_SPSR(channel))
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_sptef_clear
* Description  : Clears Transmit Buffer Empty Flag of SPSR.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_sptef_clear(uint8_t channel)
{
    /* ---- Clear SPTEF. ---- */
    if (0 != (QSPI_SPSR(channel) & QSPI_SMSTR_MASK_SPTEF))
    {
        QSPI_SPSR(channel) &= ~(QSPI_SMSTR_MASK_SPTEF);
        if (0 == QSPI_SPSR(channel))
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_sprff_clear
* Description  : Clears Receive Buffer Full Flag of SPSR.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_sprff_clear(uint8_t channel)
{
    /* ---- Clear SPRFF. ---- */
    if (0 != (QSPI_SPSR(channel) & QSPI_SMSTR_MASK_SPRFF))
    {
        QSPI_SPSR(channel) &= ~(QSPI_SMSTR_MASK_SPRFF);
        if (0 == QSPI_SPSR(channel))
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_spsslf_clear
* Description  : Clears QSPI Negation Flag of SPSR.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_spsslf_clear(uint8_t channel)
{
    /* ---- Clear SPSSLF. ---- */
    if (0 != (QSPI_SPSR(channel) & QSPI_SMSTR_MASK_SPSSLF))
    {
        QSPI_SPSR(channel) &= ~(QSPI_SMSTR_MASK_SPSSLF);
        if (0 == QSPI_SPSR(channel))
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_spsr_addr
* Description  : Gets address of SPSR.
* Arguments    : channel -
*                    Which channel to use
* Return Value : Address of SPSR.
*******************************************************************************/
volatile uint8_t R_BSP_EVENACCESS_SFR * r_qspi_smstr_spsr_addr(uint8_t channel)
{
    return g_qspi_spsr[channel];
}


/*******************************************************************************
* Function Name: r_qspi_smstr_trx_enable_single
* Description  : Enables QSPI transmission and reception in Single-SPI mode.
* Arguments    : channel -
*                    Which channel to use
*                size -
*                    Size of data
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_trx_enable_single(uint8_t channel, uint32_t size)
{
    r_qspi_smstr_spsr_clear(channel);               /* Clear status flags of QSPI.                              */
    r_qspi_smstr_buffer_reset(channel);             /* Reset transmit and receive buffer.                       */
    r_qspi_smstr_datasize_set(channel, size);       /* Set data size of transmission.                           */
    
    /* ==== Set sequence control and SPI operation mode. ==== */
    if ((QSPI_FIFO_HALF_SIZE <= size) && (0 != (QSPI_FIFO_CHK & size)))
    {
        /* ---- Size of data : (16 * m) + n bytes (m=0x00000001~0x0FFFFFFF, n=0x00000001~0x0000000F) ---- */
        QSPI_SPSCR(channel)     = QSPI_SPSCR_SEQ1;  /* Sequence length = 2                                      */
        QSPI_SPIMOD0(channel)   = 0;                /* SPCMD0 : Single-SPI                                      */
        QSPI_SSLKP0(channel)    = 1;                /* Keeps the QSSL signal level from the end of the transfer
                                                       to the beginning of the next access.                     */
        QSPI_SPIMOD1(channel)   = 0;                /* SPCMD1 : Single-SPI                                      */
        QSPI_SSLKP1(channel)    = 0;                /* Negate QSSL signal upon completion of transfer.          */
        if (0 == QSPI_SSLKP1(channel))
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }
    else
    {
        /* ---- Size of data : (16 * m) bytes or n bytes (m=0x00000001~0x0FFFFFFF, n=0x00000001~0x0000000F) ----*/
        QSPI_SPSCR(channel)     = QSPI_SPSCR_SEQ0;  /* Sequence length = 1                                      */
        QSPI_SPIMOD0(channel)   = 0;                /* SPCMD0 : Single-SPI                                      */
        QSPI_SSLKP0(channel)    = 0;                /* Negate QSSL signal upon completion of transfer.          */
        if (0 == QSPI_SSLKP0(channel))
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }
    
    r_qspi_smstr_mpc_enable(channel);               /* MPC : use as I/O port for QSPI.                          */
    QSPI_SPCR(channel)  = QSPI_SPCR_TRX_SINGLE;     /* Enable transmission and reception.                       */
    if (QSPI_SPCR_TRX_SINGLE == QSPI_SPCR(channel))
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_trx_disable
* Description  : Disables QSPI transmission and reception.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_trx_disable(uint8_t channel)
{
    r_qspi_smstr_mpc_disable(channel);              /* MPC : use as general I/O port.                           */
    QSPI_SPCR(channel)  = QSPI_SPCR_INIT;           /* Disable reception.                                       */
    if (QSPI_SPCR_INIT == QSPI_SPCR(channel))
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
    r_qspi_smstr_spsr_clear(channel);               /* Clear status flag of QSPI.                               */
    QSPI_SPSCR(channel) = QSPI_SPSCR_INIT;          /* Initialize sequence length.                              */
    if (QSPI_SPSCR_INIT == QSPI_SPSCR(channel))
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_tx_enable_dual
* Description  : Enables QSPI transmission in Dual-SPI mode.
* Arguments    : channel -
*                    Which channel to use
*                size -
*                    Size of data
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_tx_enable_dual(uint8_t channel, uint32_t size)
{
    r_qspi_smstr_spsr_clear(channel);               /* Clear status flags of QSPI.                              */
    r_qspi_smstr_buffer_reset(channel);             /* Reset transmit and receive buffer.                       */
    r_qspi_smstr_datasize_set(channel, size);       /* Set data size of transmission for dual or quad.          */
    
    /* ==== Set sequence control and SPI operation mode. ==== */
    if ((QSPI_FIFO_HALF_SIZE <= size) && (0 != (QSPI_FIFO_CHK & size)))
    {
        /* ---- Size of data : (16 * m) + n bytes (m=0x00000001~0x0FFFFFFF, n=0x00000001~0x0000000F) ---- */
        QSPI_SPSCR(channel)     = QSPI_SPSCR_SEQ1;  /* Sequence length = 2                                      */
        QSPI_SPRW0(channel)     = 0;                /* SPCMD0 : Write operation                                 */
        QSPI_SPIMOD0(channel)   = 1;                /* SPCMD0 : Dual-SPI                                        */
        QSPI_SSLKP0(channel)    = 1;                /* Keeps the QSSL signal level from the end of the transfer
                                                       to the beginning of the next access.                     */
        QSPI_SPRW1(channel)     = 0;                /* SPCMD1 : Write operation                                 */
        QSPI_SPIMOD1(channel)   = 1;                /* SPCMD1 : Dual-SPI                                        */
        QSPI_SSLKP1(channel)    = 0;                /* Negate QSSL signal upon completion of transfer.          */
        if (0 == QSPI_SSLKP1(channel))
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }
    else
    {
        /* ---- Size of data : (16 * m) bytes or n bytes (m=0x00000001~0x0FFFFFFF, n=0x00000001~0x0000000F) ----*/
        QSPI_SPSCR(channel)     = QSPI_SPSCR_SEQ0;  /* Sequence length = 1                                      */
        QSPI_SPRW0(channel)     = 0;                /* SPCMD0 : Write operation                                 */
        QSPI_SPIMOD0(channel)   = 1;                /* SPCMD0 : Dual-SPI                                        */
        QSPI_SSLKP0(channel)    = 0;                /* Negate QSSL signal upon completion of transfer.          */
        if (0 == QSPI_SSLKP0(channel))
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }
    
    r_qspi_smstr_mpc_enable(channel);               /* MPC : use as I/O port for QSPI.                          */
    QSPI_SPCR(channel)  = QSPI_SPCR_TX_DUALQUAD;    /* Enable transmission.                                     */
    if (QSPI_SPCR_TX_DUALQUAD == QSPI_SPCR(channel))
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_tx_enable_quad
* Description  : Enables QSPI transmission in Quad-SPI mode.
* Arguments    : channel -
*                    Which channel to use
*                size -
*                    Size of data
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_tx_enable_quad(uint8_t channel, uint32_t size)
{
    r_qspi_smstr_spsr_clear(channel);               /* Clear status flags of QSPI.                              */
    r_qspi_smstr_buffer_reset(channel);             /* Reset transmit and receive buffer.                       */
    r_qspi_smstr_datasize_set(channel, size);       /* Set data size of transmission for dual or quad.          */
    
    /* ==== Set sequence control and SPI operation mode. ==== */
    if ((QSPI_FIFO_HALF_SIZE <= size) && (0 != (QSPI_FIFO_CHK & size)))
    {
        /* ---- Size of data : (16 * m) + n bytes (m=0x00000001~0x0FFFFFFF, n=0x00000001~0x0000000F) ---- */
        QSPI_SPSCR(channel)     = QSPI_SPSCR_SEQ1;  /* Sequence length = 2                                      */
        QSPI_SPRW0(channel)     = 0;                /* SPCMD0 : Write operation                                 */
        QSPI_SPIMOD0(channel)   = 2;                /* SPCMD0 : Quad-SPI                                        */
        QSPI_SSLKP0(channel)    = 1;                /* Keeps the QSSL signal level from the end of the transfer
                                                       to the beginning of the next access.                     */
        QSPI_SPRW1(channel)     = 0;                /* SPCMD1 : Write operation                                 */
        QSPI_SPIMOD1(channel)   = 2;                /* SPCMD1 : Quad-SPI                                        */
        QSPI_SSLKP1(channel)    = 0;                /* Negate QSSL signal upon completion of transfer.          */
        if (0 == QSPI_SSLKP1(channel))
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }
    else
    {
        /* ---- Size of data : (16 * m) bytes or n bytes (m=0x00000001~0x0FFFFFFF, n=0x00000001~0x0000000F) ----*/
        QSPI_SPSCR(channel)     = QSPI_SPSCR_SEQ0;  /* Sequence length = 1                                      */
        QSPI_SPRW0(channel)     = 0;                /* SPCMD0 : Write operation                                 */
        QSPI_SPIMOD0(channel)   = 2;                /* SPCMD0 : Quad-SPI                                        */
        QSPI_SSLKP0(channel)    = 0;                /* Negate QSSL signal upon completion of transfer.          */
        if (0 == QSPI_SSLKP0(channel))
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }
    
    r_qspi_smstr_mpc_enable(channel);               /* MPC : use as I/O port for QSPI.                          */
    QSPI_SPCR(channel)  = QSPI_SPCR_TX_DUALQUAD;    /* Enable transmission.                                     */
    if (QSPI_SPCR_TX_DUALQUAD == QSPI_SPCR(channel))
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_tx_disable
* Description  : Disables QSPI transmission.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_tx_disable(uint8_t channel)
{
    r_qspi_smstr_mpc_disable(channel);              /* MPC : use as general I/O port.                           */
    QSPI_SPCR(channel)  = QSPI_SPCR_INIT;           /* Disable transmission.                                    */
    if (QSPI_SPCR_INIT == QSPI_SPCR(channel))
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
    r_qspi_smstr_spsr_clear(channel);               /* Clear status flag of QSPI.                               */
    QSPI_SPSCR(channel) = QSPI_SPSCR_INIT;          /* Initialize sequence length.                              */
    if (QSPI_SPSCR_INIT == QSPI_SPSCR(channel))
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_rx_enable_dual
* Description  : Enables QSPI reception in Dual-SPI mode.
* Arguments    : channel -
*                    Which channel to use
*                size -
*                    Size of data
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_rx_enable_dual(uint8_t channel, uint32_t size)
{
    r_qspi_smstr_spsr_clear(channel);               /* Clear status flags of QSPI.                              */
    r_qspi_smstr_buffer_reset(channel);             /* Reset transmit and receive buffer.                       */
    r_qspi_smstr_datasize_set(channel, size);       /* Set data size of transmission for dual or quad.          */
    
    /* ==== Set sequence control and SPI operation mode. ==== */
    /* NOTE: When all the commands configuring sequence are dual-/quad-SPI read operations, the sequential operation
             is continued as long as the receive buffer is not empty for the receive data.
             To terminate read operation, execute write operation with transmission buffer empty
             as the last sequence. */
    if ((QSPI_FIFO_HALF_SIZE <= size) && (0 != (QSPI_FIFO_CHK & size)))
    {
        /* ---- Size of data : (16 * m) + n bytes (m=0x00000001~0x0FFFFFFF, n=0x00000001~0x0000000F) ---- */
        QSPI_SPSCR(channel)     = QSPI_SPSCR_SEQ2;  /* Sequence length = 3                                      */
        QSPI_SPRW0(channel)     = 1;                /* SPCMD0 : Read operation                                  */
        QSPI_SPIMOD0(channel)   = 1;                /* SPCMD0 : Dual-SPI                                        */
        QSPI_SSLKP0(channel)    = 1;                /* Keeps the QSSL signal level from the end of the transfer
                                                       to the beginning of the next access.                     */
        QSPI_SPRW1(channel)     = 1;                /* SPCMD1 : Read operation                                  */
        QSPI_SPIMOD1(channel)   = 1;                /* SPCMD1 : Dual-SPI                                        */
        QSPI_SSLKP1(channel)    = 0;                /* Negate QSSL signal upon completion of transfer.          */
        QSPI_SPRW2(channel)     = 0;                /* SPCMD2 : Write operation (To terminate read operation.)  */
        QSPI_SPIMOD2(channel)   = 2;                /* SPCMD2 : Quad-SPI                                        */
        if (0 == QSPI_SPIMOD2(channel))
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }
    else
    {
        /* ---- Size of data : (16 * m) bytes or n bytes (m=0x00000001~0x0FFFFFFF, n=0x00000001~0x0000000F) ----*/
        QSPI_SPSCR(channel)     = QSPI_SPSCR_SEQ1;  /* Sequence length = 2                                      */
        QSPI_SPRW0(channel)     = 1;                /* SPCMD0 : Read operation                                  */
        QSPI_SPIMOD0(channel)   = 1;                /* SPCMD0 : Dual-SPI                                        */
        QSPI_SSLKP0(channel)    = 0;                /* Negate QSSL signal upon completion of transfer.          */
        QSPI_SPRW1(channel)     = 0;                /* SPCMD1 : Write operation (To terminate read operation.)  */
        QSPI_SPIMOD1(channel)   = 2;                /* SPCMD1 : Quad-SPI                                        */
        if (0 == QSPI_SPIMOD1(channel))
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }

    r_qspi_smstr_mpc_enable(channel);               /* MPC : use as I/O port for QSPI.                          */
    QSPI_SPCR(channel)  = QSPI_SPCR_RX_DUALQUAD;    /* Enable reception.                                        */
    if (QSPI_SPCR_RX_DUALQUAD == QSPI_SPCR(channel))
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_rx_enable_quad
* Description  : Enables QSPI reception in Quad-SPI mode.
* Arguments    : channel -
*                    Which channel to use
*                size -
*                    Size of data
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_rx_enable_quad(uint8_t channel, uint32_t size)
{
    r_qspi_smstr_spsr_clear(channel);               /* Clear status flags of QSPI.                              */
    r_qspi_smstr_buffer_reset(channel);             /* Reset transmit and receive buffer.                       */
    r_qspi_smstr_datasize_set(channel, size);       /* Set data size of transmission for dual or quad.          */
    
    /* ==== Set sequence control and SPI operation mode. ==== */
    /* NOTE: When all the commands configuring sequence are dual-/quad-SPI read operations, the sequential operation
             is continued as long as the receive buffer is not empty for the receive data.
             To terminate read operation, execute write operation with transmission buffer empty
             as the last sequence. */
    if ((QSPI_FIFO_HALF_SIZE <= size) && (0 != (QSPI_FIFO_CHK & size)))
    {
        /* ---- Size of data : (16 * m) + n bytes (m=0x00000001~0x0FFFFFFF, n=0x00000001~0x0000000F) ---- */
        QSPI_SPSCR(channel)     = QSPI_SPSCR_SEQ2;  /* Sequence length = 3                                      */
        QSPI_SPRW0(channel)     = 1;                /* SPCMD0 : Read operation                                  */
        QSPI_SPIMOD0(channel)   = 2;                /* SPCMD0 : Quad-SPI                                        */
        QSPI_SSLKP0(channel)    = 1;                /* Keeps the QSSL signal level from the end of the transfer
                                                       to the beginning of the next access.                     */
        QSPI_SPRW1(channel)     = 1;                /* SPCMD1 : Read operation                                  */
        QSPI_SPIMOD1(channel)   = 2;                /* SPCMD1 : Quad-SPI                                        */
        QSPI_SSLKP1(channel)    = 0;                /* Negate QSSL signal upon completion of transfer.          */
        QSPI_SPRW2(channel)     = 0;                /* SPCMD2 : Write operation (To terminate read operation.)  */
        QSPI_SPIMOD2(channel)   = 2;                /* SPCMD2 : Quad-SPI                                        */
        if (0 == QSPI_SPIMOD2(channel))
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }
    else
    {
        /* ---- Size of data : (16 * m) bytes or n bytes (m=0x00000001~0x0FFFFFFF, n=0x00000001~0x0000000F) ----*/
        QSPI_SPSCR(channel)     = QSPI_SPSCR_SEQ1;  /* Sequence length = 2                                      */
        QSPI_SPRW0(channel)     = 1;                /* SPCMD0 : Read operation                                  */
        QSPI_SPIMOD0(channel)   = 2;                /* SPCMD0 : Quad-SPI                                        */
        QSPI_SSLKP0(channel)    = 0;                /* Negate QSSL signal upon completion of transfer.          */
        QSPI_SPRW1(channel)     = 0;                /* SPCMD1 : Write operation (To terminate read operation.)  */
        QSPI_SPIMOD1(channel)   = 2;                /* SPCMD1 : Quad-SPI                                        */
        if (0 == QSPI_SPIMOD1(channel))
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }

    r_qspi_smstr_mpc_enable(channel);               /* MPC : use as I/O port for QSPI.                          */
    QSPI_SPCR(channel)  = QSPI_SPCR_RX_DUALQUAD;    /* Enable reception.                                        */
    if (QSPI_SPCR_RX_DUALQUAD == QSPI_SPCR(channel))
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_rx_disable
* Description  : Disables QSPI reception.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_rx_disable(uint8_t channel)
{
    r_qspi_smstr_mpc_disable(channel);              /* MPC : use as general I/O port.                           */
    QSPI_SPCR(channel)  = QSPI_SPCR_INIT;           /* Disable reception.                                       */
    if (QSPI_SPCR_INIT == QSPI_SPCR(channel))
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
    r_qspi_smstr_spsr_clear(channel);               /* Clear status flag of QSPI.                               */
    QSPI_SPSCR(channel) = QSPI_SPSCR_INIT;          /* Initialize sequence length.                              */
    if (QSPI_SPSCR_INIT == QSPI_SPSCR(channel))
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_buffer_reset
* Description  : Resets transmit and receive buffer of QSPI.
* Arguments    : channel -
*                    Which channel to use.
* Return Value : none
*******************************************************************************/
static void r_qspi_smstr_buffer_reset(uint8_t channel)
{
    QSPI_SPBFCR(channel)    = QSPI_SPBFCR_RST;      /* Reset transmit and receive buffer.                       */
    if (QSPI_SPBFCR_RST == QSPI_SPBFCR(channel))
    {
        /* According to specifications of QSPI,
           if write SPBFCR, must execute dummy reading. */
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
    QSPI_SPBFCR(channel)    = QSPI_SPBFCR_INIT;     /* Allow transmit and receive buffer normal operation.      */
    if (QSPI_SPBFCR_INIT == QSPI_SPBFCR(channel))
    {
        /* According to specifications of QSPI,
           if write SPBFCR, must execute dummy reading. */
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_datasize_set
* Description  : Sets data size to the SFR for QSPI.
* Arguments    : channel -
*                    Which channel to use
*                size -
*                    Size of data.
* Return Value : none
*******************************************************************************/
static void r_qspi_smstr_datasize_set(uint8_t channel, uint32_t size)
{
    if ((QSPI_FIFO_HALF_SIZE <= size) && (0 != (QSPI_FIFO_CHK & size)))
    {
        /* ---- Size of data : (16 * m) + n bytes (m=0x00000001~0x0FFFFFFF, n=0x00000001~0x0000000F) ---- */
        QSPI_SPBFCR(channel)    = QSPI_SPBFCR_FIFO_HALF_SIZE;           /* Buffer triggering number : 16 bytes  */
        if (QSPI_SPBFCR_FIFO_HALF_SIZE == QSPI_SPBFCR(channel))
        {
            /* According to specifications of QSPI,
               if write SPBFCR, must execute dummy reading. */
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
        QSPI_SPB0(channel)      = SPCMD_SPB_LONG;                       /* Transfer data length : 32 bits       */

        /* Cast the variable to a uint32_t type. */
        QSPI_SPBMUL0(channel) = (uint32_t)((size / QSPI_FIFO_HALF_SIZE) * (QSPI_FIFO_HALF_SIZE / QSPI_SMSTR_TRAN_SIZE));
                                                                        /* Number of times to transfer 32 bits. */
        QSPI_SPB1(channel)      = SPCMD_SPB_BYTE;                       /* Transfer data length : 8 bits        */

        /* Cast the variable to a uint32_t type. */
        QSPI_SPBMUL1(channel)   = (uint32_t)(size % QSPI_FIFO_HALF_SIZE);
                                                                        /* Number of times to transfer 8 bits.  */
        if (0 == QSPI_SPBMUL1(channel))
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }
    else if ((QSPI_FIFO_HALF_SIZE <= size) && (0 == (QSPI_FIFO_CHK & size)))
    {
        /* ---- Size of data : (16 * m) bytes (m=0x00000001~0x0FFFFFFF) ---- */
        QSPI_SPBFCR(channel)    = QSPI_SPBFCR_FIFO_HALF_SIZE;           /* Buffer triggering number : 16 bytes  */
        if (QSPI_SPBFCR_FIFO_HALF_SIZE == QSPI_SPBFCR(channel))
        {
            /* According to specifications of QSPI,
               if write SPBFCR, must execute dummy reading. */
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
        QSPI_SPB0(channel)      = SPCMD_SPB_LONG;                       /* Transfer data length : 32 bits       */

        /* Cast the variable to a uint32_t type. */
        QSPI_SPBMUL0(channel) = (uint32_t)((size / QSPI_FIFO_HALF_SIZE) * (QSPI_FIFO_HALF_SIZE / QSPI_SMSTR_TRAN_SIZE));
                                                                        /* Number of times to transfer 32 bits. */
        if (0 == QSPI_SPBMUL0(channel))
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }
    else
    {
        /* ---- Size of data : n bytes (n=0x00000001~0x0000000F) ---- */
        QSPI_SPBFCR(channel)    = QSPI_SPBFCR_BYTE;                     /* Buffer triggering number : 1 byte    */
        if (QSPI_SPBFCR_BYTE == QSPI_SPBFCR(channel))
        {
            /* According to specifications of QSPI,
               if write SPBFCR, must execute dummy reading. */
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
        QSPI_SPB0(channel)      = SPCMD_SPB_BYTE;                       /* Transfer data length : 8 bits        */

        /* Cast the variable to a uint32_t type */
        QSPI_SPBMUL0(channel)   = (uint32_t)(size);                     /* Number of times to transfer 8 bits.  */
        if (0 == QSPI_SPBMUL0(channel))
        {
            /* Wait for the write completion. */
            R_BSP_NOP();
        }
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_get_buffregaddress
* Description  : Gets address of QSPI data register (SPDR).
* Arguments    : channel -
*                    Which channel to use
*                p_spdr_adr -
*                    Address of QSPI data register (SPDR)
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_get_buffregaddress(uint8_t channel, uint32_t *p_spdr_adr)
{
    *p_spdr_adr = (uint32_t)(&QSPI_SPDR_LONG(channel));
}


#endif /* defined(BSP_MCU_RX72M) */

/* End of File */

