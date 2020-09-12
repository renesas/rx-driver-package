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
* File Name    : r_qspi_smstr_target_dev_port.c
* Version      : 1.14
* Device       : RX
* Abstract     : Source file dedicated to RX72N for QSPI single master driver
* Tool-Chain   : Renesas RXC Toolchain v3.01.00
* OS           : not use
* H/W Platform : not use
* Description  : Functions for QSPI single master driver
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 22.11.2019 1.14     First Release
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
#if defined(BSP_MCU_RX72N)


/*******************************************************************************
Macro definitions
*******************************************************************************/
#define QSPI_TIMER_CH_FLG          (0)
#define QSPI_TIMER_CH_COUNT        (1)
#define QSPI_TIMER_CH_MAX_COUNT    (2)
#define QSPI_TIMER_MIN_TIME        (100)       /* 100ms                    */
#define QSPI_SECTOR_SIZE           (512)       /* 1 sector size            */


/*******************************************************************************
Private variables and functions
*******************************************************************************/
static volatile uint8_t  gs_qspi_int_spti_dmacdtc_flg[QSPI_NUM_CHANNELS];
static volatile uint8_t  gs_qspi_int_spri_dmacdtc_flg[QSPI_NUM_CHANNELS];
static volatile uint32_t gs_qspi_timer_cnt_out[QSPI_NUM_CHANNELS][QSPI_TIMER_CH_MAX_COUNT + 1];

static void                 r_qspi_smstr_start_timer(uint8_t channel, uint32_t msec);
static qspi_smstr_status_t  r_qspi_smstr_check_timer(uint8_t channel);
static void                 r_qspi_smstr_end_timer(uint8_t channel);

#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
R_BSP_PRAGMA_STATIC_INTERRUPT(r_qspi_smstr_spti_isr0, VECT(QSPI, SPTI))
R_BSP_ATTRIB_STATIC_INTERRUPT void r_qspi_smstr_spti_isr0(void)
{
    R_QSPI_SMstr_Int_Spti_Ier_Clear(0);
    R_QSPI_SMstr_Int_Spti_Dmacdtc_Flag_Set(0, QSPI_SET_TRANS_STOP);
}

R_BSP_PRAGMA_STATIC_INTERRUPT(r_qspi_smstr_spri_isr0, VECT(QSPI, SPRI))
R_BSP_ATTRIB_STATIC_INTERRUPT void r_qspi_smstr_spri_isr0(void)
{
    R_QSPI_SMstr_Int_Spri_Ier_Clear(0);
    R_QSPI_SMstr_Int_Spri_Dmacdtc_Flag_Set(0, QSPI_SET_TRANS_STOP);
}
#endif  /* defined(QSPI_SMSTR_CFG_CH0_INCLUDED) */


/*******************************************************************************
* Function Name: r_qspi_smstr_io_init
* Description  : Initializes setting of ports used for QSPI.
* Arguments    : channel -
*                    Which channel to use.
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_io_init(uint8_t channel)
{
    r_qspi_smstr_mpc_disable(channel);    /* MPC : use as general I/O port.   */
    r_qspi_smstr_clk_init(channel);       /* Initialize port for QSPCLK.      */
    r_qspi_smstr_dataio0_init(channel);   /* Initialize port for QMO/QIO0.    */
    r_qspi_smstr_dataio1_init(channel);   /* Initialize port for QMI/QIO1.    */
    r_qspi_smstr_dataio2_init(channel);   /* Initialize port for QIO2.        */
    r_qspi_smstr_dataio3_init(channel);   /* Initialize port for QIO3.        */
}


/*******************************************************************************
* Function Name: r_qspi_smstr_io_reset
* Description  : Resets setting of ports used for QSPI.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_io_reset(uint8_t channel)
{
    r_qspi_smstr_mpc_disable(channel);    /* MPC : use as general I/O port.   */
}


/*******************************************************************************
* Function Name: r_qspi_smstr_mpc_enable
* Description  : Sets MPC registers of ports for QSPI.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_mpc_enable(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            /* ---- Setting of ports for QSPI channel 0 ---- */
            QSPI_CH0_PMR_DATAIO0    = 0;                        /* Use as general I/O port.                 */
            QSPI_CH0_PMR_DATAIO1    = 0;                        /* Use as general I/O port.                 */
            QSPI_CH0_PMR_DATAIO2    = 0;                        /* Use as general I/O port.                 */
            QSPI_CH0_PMR_DATAIO3    = 0;                        /* Use as general I/O port.                 */
            QSPI_CH0_PMR_CLK        = 0;                        /* Use as general I/O port.                 */
            if (0 == QSPI_CH0_PMR_CLK)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }

#ifdef QSPI_SMSTR_CFG_USE_FIT
            R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);
#else
            QSPI_PWPR_BOWI          = 0;                        /* Enable writing PFSWE bit.                */
            QSPI_PWPR_PFSWE         = 1;                        /* Enable writing PFS registers.            */
            if (1 == QSPI_PWPR_PFSWE)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif /* QSPI_SMSTR_CFG_USE_FIT */

            QSPI_CH0_MPC_DATAIO0    = QSPI_MPCDATAIO0_ENABLE;   /* Set pin function to QSPI QIO0 pin.       */
            QSPI_CH0_MPC_DATAIO1    = QSPI_MPCDATAIO1_ENABLE;   /* Set pin function to QSPI QIO1 pin.       */
            QSPI_CH0_MPC_DATAIO2    = QSPI_MPCDATAIO2_ENABLE;   /* Set pin function to QSPI QIO2 pin.       */
            QSPI_CH0_MPC_DATAIO3    = QSPI_MPCDATAIO3_ENABLE;   /* Set pin function to QSPI QIO3 pin.       */
            QSPI_CH0_MPC_CLK        = QSPI_MPCCLK_ENABLE;       /* Set pin function to QSPI QSPCLK pin.     */
            if (QSPI_MPCCLK_ENABLE == QSPI_CH0_MPC_CLK)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }

#ifdef QSPI_SMSTR_CFG_USE_FIT
            R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
#else
            QSPI_PWPR_PFSWE         = 0;                        /* Disable writing PFS registers.           */
            QSPI_PWPR_BOWI          = 1;                        /* Disable writing PFSWE bit.               */
            if (1 == QSPI_PWPR_BOWI)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif /* QSPI_SMSTR_CFG_USE_FIT */

            QSPI_CH0_PMR_DATAIO0    = 1;                        /* Use as I/O port for peripheral function. */
            QSPI_CH0_PMR_DATAIO1    = 1;                        /* Use as I/O port for peripheral function. */
            QSPI_CH0_PMR_DATAIO2    = 1;                        /* Use as I/O port for peripheral function. */
            QSPI_CH0_PMR_DATAIO3    = 1;                        /* Use as I/O port for peripheral function. */
            QSPI_CH0_PMR_CLK        = 1;                        /* Use as I/O port for peripheral function. */
            if (1 == QSPI_CH0_PMR_CLK)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_mpc_disable
* Description  : Clears MPC registers of ports for QSPI.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_mpc_disable(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            /* ---- Setting of ports for QSPI channel 0 ---- */
            QSPI_CH0_PMR_DATAIO0    = 0;                        /* Use as general I/O port.                 */
            QSPI_CH0_PMR_DATAIO1    = 0;                        /* Use as general I/O port.                 */
            QSPI_CH0_PMR_DATAIO2    = 0;                        /* Use as general I/O port.                 */
            QSPI_CH0_PMR_DATAIO3    = 0;                        /* Use as general I/O port.                 */
            QSPI_CH0_PMR_CLK        = 0;                        /* Use as general I/O port.                 */
            if (0 == QSPI_CH0_PMR_CLK)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }

#ifdef QSPI_SMSTR_CFG_USE_FIT
            R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);
#else
            QSPI_PWPR_BOWI          = 0;                        /* Enable writing PFSWE bit.                */
            QSPI_PWPR_PFSWE         = 1;                        /* Enable writing PFS registers.            */
            if (1 == QSPI_PWPR_PFSWE)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif /* QSPI_SMSTR_CFG_USE_FIT */

            QSPI_CH0_MPC_DATAIO0    = QSPI_MPCDATAIO0_INIT;     /* Set pin function to Hi-Z.                */
            QSPI_CH0_MPC_DATAIO1    = QSPI_MPCDATAIO1_INIT;     /* Set pin function to Hi-Z.                */
            QSPI_CH0_MPC_DATAIO2    = QSPI_MPCDATAIO2_INIT;     /* Set pin function to Hi-Z.                */
            QSPI_CH0_MPC_DATAIO3    = QSPI_MPCDATAIO3_INIT;     /* Set pin function to Hi-Z.                */
            QSPI_CH0_MPC_CLK        = QSPI_MPCCLK_INIT;         /* Set pin function to Hi-Z.                */
            if (QSPI_MPCCLK_INIT == QSPI_CH0_MPC_CLK)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }

#ifdef QSPI_SMSTR_CFG_USE_FIT
            R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
#else
            QSPI_PWPR_PFSWE         = 0;                        /* Disable writing PFS registers.           */
            QSPI_PWPR_BOWI          = 1;                        /* Disable writing PFSWE bit.               */
            if (1 == QSPI_PWPR_BOWI)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif /* QSPI_SMSTR_CFG_USE_FIT */

#endif
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_dataio0_init
* Description  : Initializes setting of port for QMO/QIO0.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_dataio0_init(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            /* ---- Setting of port for QMO/QIO0 ---- */
            QSPI_CH0_ODR_DATAIO0    = 0;                /* Output type is CMOS output.              */
            QSPI_CH0_PCR_DATAIO0    = 0;                /* Disable input pull-up resistor.          */
    #if (('J' != R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT) && ('3' != R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT))
            QSPI_CH0_DSCR_DATAIO0   = 1;                /* Drive capacity is High-drive output.     */
    #endif
            QSPI_CH0_PDR_DATAIO0    = QSPI_SMSTR_IN;    /* Select input direction.                  */
            if (QSPI_SMSTR_IN == QSPI_CH0_PDR_DATAIO0)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_dataio0_reset
* Description  : Resets setting of port for QMO/QIO0.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_dataio0_reset(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            /* ---- Setting of port for QMO/QIO0 ---- */
            QSPI_CH0_PDR_DATAIO0    = QSPI_SMSTR_IN;  /* Select input direction.                  */
    #if (('J' != R_QSPI_SMSTR_CFG_QSPI_QIO0_PORT) && ('3' != R_QSPI_SMSTR_CFG_QSPI_QIO0_BIT))
            QSPI_CH0_DSCR_DATAIO0   = 0;              /* Drive capacity is normal drive output.   */
    #endif
            QSPI_CH0_PCR_DATAIO0    = 0;              /* Disable input pull-up resistor.          */
            QSPI_CH0_ODR_DATAIO0    = 0;              /* Output type is CMOS output.              */
            if (0 == QSPI_CH0_ODR_DATAIO0)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_dataio1_init
* Description  : Initializes setting of port for QMI/QIO1.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_dataio1_init(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            /* ---- Setting of port for QMI/QIO1 ---- */
            QSPI_CH0_ODR_DATAIO1    = 0;              /* Output type is CMOS output.              */
            QSPI_CH0_PCR_DATAIO1    = 0;              /* Disable input pull-up resistor.          */
    #if (('J' != R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT) && ('5' != R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT))
            QSPI_CH0_DSCR_DATAIO1   = 1;              /* Drive capacity is High-drive output.     */
    #endif
            QSPI_CH0_PDR_DATAIO1    = QSPI_SMSTR_IN;  /* Select input direction.                  */
            if (QSPI_SMSTR_IN == QSPI_CH0_PDR_DATAIO1)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_dataio1_reset
* Description  : Resets setting of port for QMI/QIO1.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_dataio1_reset(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            /* ---- Setting of port for QMI/QIO1 ---- */
            QSPI_CH0_PDR_DATAIO1    = QSPI_SMSTR_IN;  /* Select input direction.                  */
    #if (('J' != R_QSPI_SMSTR_CFG_QSPI_QIO1_PORT) && ('5' != R_QSPI_SMSTR_CFG_QSPI_QIO1_BIT))
            QSPI_CH0_DSCR_DATAIO1   = 0;              /* Drive capacity is normal drive output.   */
    #endif
            QSPI_CH0_PCR_DATAIO1    = 0;              /* Disable input pull-up resistor.          */
            QSPI_CH0_ODR_DATAIO1    = 0;              /* Output type is CMOS output.              */
            if (0 == QSPI_CH0_ODR_DATAIO1)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_dataio2_init
* Description  : Initializes setting of port for QIO2.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_dataio2_init(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            /* ---- Setting of port for QIO2 ---- */
            QSPI_CH0_ODR_DATAIO2    = 0;              /* Output type is CMOS output.              */
            QSPI_CH0_PCR_DATAIO2    = 0;              /* Disable input pull-up resistor.          */
            QSPI_CH0_DSCR_DATAIO2   = 1;              /* Drive capacity is High-drive output.     */
            QSPI_CH0_PDR_DATAIO2    = QSPI_SMSTR_IN;  /* Select input direction.                  */
            if (QSPI_SMSTR_IN == QSPI_CH0_PDR_DATAIO2)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_dataio2_reset
* Description  : Resets setting of port for QIO2.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_dataio2_reset(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            /* ---- Setting of port for QIO2 ---- */
            QSPI_CH0_PDR_DATAIO2    = QSPI_SMSTR_IN;  /* Select input direction.                  */
            QSPI_CH0_DSCR_DATAIO2   = 0;              /* Drive capacity is normal drive output.   */
            QSPI_CH0_PCR_DATAIO2    = 0;              /* Disable input pull-up resistor.          */
            QSPI_CH0_ODR_DATAIO2    = 0;              /* Output type is CMOS output.              */
            if (0 == QSPI_CH0_ODR_DATAIO2)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_dataio3_init
* Description  : Initializes setting of port for QIO3.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_dataio3_init(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            /* ---- Setting of port for QIO3 ---- */
            QSPI_CH0_ODR_DATAIO3    = 0;              /* Output type is CMOS output.              */
            QSPI_CH0_PCR_DATAIO3    = 0;              /* Disable input pull-up resistor.          */
            QSPI_CH0_DSCR_DATAIO3   = 1;              /* Drive capacity is High-drive output.     */
            QSPI_CH0_PDR_DATAIO3    = QSPI_SMSTR_IN;  /* Select input direction.                  */
            if (QSPI_SMSTR_IN == QSPI_CH0_PDR_DATAIO3)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_dataio3_reset
* Description  : Resets setting of port for QIO3.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_dataio3_reset(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            /* ---- Setting of port for QIO3 ---- */
            QSPI_CH0_PDR_DATAIO3    = QSPI_SMSTR_IN;  /* Select input direction.                  */
            QSPI_CH0_DSCR_DATAIO3   = 0;              /* Drive capacity is normal drive output.   */
            QSPI_CH0_PCR_DATAIO3    = 0;              /* Disable input pull-up resistor.          */
            QSPI_CH0_ODR_DATAIO3    = 0;              /* Output type is CMOS output.              */
            if (0 == QSPI_CH0_ODR_DATAIO3)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_clk_init
* Description  : Initializes setting of port for QSPCLK.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_clk_init(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            /* ---- Setting of port for QSPCLK ---- */
            QSPI_CH0_ODR_CLK    = 0;                /* Output type is CMOS output.              */
            QSPI_CH0_DSCR_CLK   = 1;                /* Drive capacity is High-drive output.     */
            QSPI_CH0_PODR_CLK   = QSPI_SMSTR_HI;    /* Output data is High.                     */
            QSPI_CH0_PDR_CLK    = QSPI_SMSTR_OUT;   /* Select output direction.                 */
            if (QSPI_SMSTR_OUT == QSPI_CH0_PDR_CLK)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_clk_reset
* Description  : Resets setting of port for QSPCLK.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_clk_reset(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            /* ---- Setting of port for QSPCLK ---- */
            QSPI_CH0_PDR_CLK    = QSPI_SMSTR_IN;    /* Select input direction.                  */
            QSPI_CH0_PODR_CLK   = QSPI_SMSTR_LOW;   /* Output data is Low.                      */
            QSPI_CH0_DSCR_CLK   = 0;                /* Drive capacity is normal drive output.   */
            QSPI_CH0_ODR_CLK    = 0;                /* Output type is CMOS output.              */
            if (0 == QSPI_CH0_ODR_CLK)
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_module_enable
* Description  : Releases module stop state.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_module_enable(uint8_t channel)
{
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif
#endif
    /* Enable writing to registers. */
#ifdef QSPI_SMSTR_CFG_USE_FIT
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
#else
    QSPI_PRCR = QSPI_PRCR_ENABLE;
    if (QSPI_PRCR_ENABLE == QSPI_PRCR)
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
#endif /* QSPI_SMSTR_CFG_USE_FIT */
    
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
        #endif
            MSTP(QSPI) = 0;
        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
        #endif
            if (0 == MSTP(QSPI))
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
    
    /* Disable writing to registers. */
#ifdef QSPI_SMSTR_CFG_USE_FIT
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
#else
    QSPI_PRCR = QSPI_PRCR_DISABLE;
    if (QSPI_PRCR_DISABLE == QSPI_PRCR)
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
#endif /* QSPI_SMSTR_CFG_USE_FIT */
}


/*******************************************************************************
* Function Name: r_qspi_smstr_module_disable
* Description  : Sets the Module Stop of QSPI.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_module_disable(uint8_t channel)
{
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif
#endif
    /* Enable writing to registers. */
    QSPI_PRCR = QSPI_PRCR_ENABLE;
    if (QSPI_PRCR_ENABLE == QSPI_PRCR)
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
    
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
        #endif
            MSTP(QSPI) = 1;
        #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
        #endif
            if (1 == MSTP(QSPI))
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
    
    /* Disable writing to registers. */
    QSPI_PRCR = QSPI_PRCR_DISABLE;
    if (QSPI_PRCR_DISABLE == QSPI_PRCR)
    {
        /* Wait for the write completion. */
        R_BSP_NOP();
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_tx_dmacdtc_wait
* Description  : Waits for transmission completion.
* Arguments    : channel -
*                    Which channel to use
*                size -
*                    Size of data
* Return Value : QSPI_SMSTR_SUCCESS -
*                    Successful operation
*                QSPI_SMSTR_ERR_PARAM -
*                    Parameter error
*                QSPI_SMSTR_ERR_HARD -
*                    Hardware error
*******************************************************************************/
qspi_smstr_status_t r_qspi_smstr_tx_dmacdtc_wait(uint8_t channel, uint32_t size)
{
    qspi_smstr_status_t    ret  = QSPI_SMSTR_SUCCESS;
    uint32_t               time = QSPI_TIMER_MIN_TIME;     /* 100ms        */

    /* ---- Check DMAC/DTC transfer end.  --- */
    /* Timeout value depends on transfer size. 
       Minimum time is 100ms. When the data count exceeds 10 sectors (1 sector = 512 bytes),
       increase 10ms time for 1 sector increase.*/
    if ((QSPI_SECTOR_SIZE * 11) <= size)
    {
        time = (time + (((size / QSPI_SECTOR_SIZE) - 10) * 10));
        if (0x0fffffff < time)
        {
            time = 0x0fffffff;
        }
    }

    /* Start timer. */
    r_qspi_smstr_start_timer(channel, time);

    /* WAIT_LOOP */
    while (1)
    {
        /* Check timeout. */
        if (QSPI_SMSTR_ERR_HARD == r_qspi_smstr_check_timer(channel))
        {
            R_QSPI_SMSTR_LOG_FUNC(QSPI_SMSTR_DEBUG_ERR_ID, (uint32_t)QSPI_SMSTR_TARGET_DEV_PORT, __LINE__);
            ret = QSPI_SMSTR_ERR_HARD;
            break;
        }
        
        /* Check DMAC/DTC transfer end */
        if (QSPI_SET_TRANS_STOP == gs_qspi_int_spti_dmacdtc_flg[channel])
        {
            break;
        }
    }

    r_qspi_smstr_end_timer(channel);

    return ret;
}


/*******************************************************************************
* Function Name: r_qspi_smstr_rx_dmacdtc_wait
* Description  : Waits for reception completion.
* Arguments    : channel -
*                    Which channel to use
*                size -
*                    Size of data
* Return Value : QSPI_SMSTR_SUCCESS -
*                    Successful operation
*                QSPI_SMSTR_ERR_PARAM -
*                    Parameter error
*                QSPI_SMSTR_ERR_HARD -
*                    Hardware error
*******************************************************************************/
qspi_smstr_status_t r_qspi_smstr_rx_dmacdtc_wait(uint8_t channel, uint32_t size)
{
    qspi_smstr_status_t    ret  = QSPI_SMSTR_SUCCESS;
    uint32_t               time = QSPI_TIMER_MIN_TIME;     /* 100ms        */

    /* ---- Check DMAC/DTC transfer end.  --- */
    /* Timeout value depends on transfer size. 
       Minimum time is 100ms. When the data count exceeds 10 sectors (1 sector = 512 bytes),
       increase 10ms time for 1 sector increase.*/
    if ((QSPI_SECTOR_SIZE * 11) <= size)
    {
        time = (time + (((size / QSPI_SECTOR_SIZE) - 10) * 10));
        if (0x0fffffff < time)
        {
            time = 0x0fffffff;
        }
    }

    /* Start timer. */
    r_qspi_smstr_start_timer(channel, time);

    /* WAIT_LOOP */
    while (1)
    {
        /* Check timeout. */
        if (QSPI_SMSTR_ERR_HARD == r_qspi_smstr_check_timer(channel))
        {
            R_QSPI_SMSTR_LOG_FUNC(QSPI_SMSTR_DEBUG_ERR_ID, (uint32_t)QSPI_SMSTR_TARGET_DEV_PORT, __LINE__);
            ret = QSPI_SMSTR_ERR_HARD;
            break;
        }
        
        /* Check DMAC/DTC transfer end */
        if (QSPI_SET_TRANS_STOP == gs_qspi_int_spri_dmacdtc_flg[channel])
        {
            break;
        }
    }

    r_qspi_smstr_end_timer(channel);

    return ret;
}


/*******************************************************************************
* Function Name: r_qspi_smstr_int_spti_init
* Description  : Initializes the interrupt registers of SPTI.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_int_spti_init(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            R_BSP_InterruptRequestDisable(VECT(QSPI, SPTI));
            IR(QSPI, SPTI)    = 0;
            IPR(QSPI, SPTI)   = QSPI_SMSTR_CFG_CH0_INT_SPTI_LEVEL;
            if (0 == IPR(QSPI, SPTI))
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif  /* defined(QSPI_SMSTR_CFG_CH0_INCLUDED) */
        break;
        
        case 1:
#if defined(QSPI_SMSTR_CFG_CH1_INCLUDED)
            R_BSP_InterruptRequestDisable(VECT(QSPI, SPTI));
            IR(QSPI, SPTI)    = 0;
            IPR(QSPI, SPTI)   = QSPI_SMSTR_CFG_CH1_INT_SPTI_LEVEL;
            if (0 == IPR(QSPI, SPTI))
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif  /* defined(QSPI_SMSTR_CFG_CH1_INCLUDED) */
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_int_spti_init
* Description  : Initializes the interrupt registers of SPTI.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_int_spri_init(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            R_BSP_InterruptRequestDisable(VECT(QSPI, SPRI));
            IR(QSPI, SPRI)    = 0;
            IPR(QSPI, SPRI)   = QSPI_SMSTR_CFG_CH0_INT_SPRI_LEVEL;
            if (0 == IPR(QSPI, SPRI))
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif  /* defined(QSPI_SMSTR_CFG_CH0_INCLUDED) */
        break;
        
        case 1:
#if defined(QSPI_SMSTR_CFG_CH1_INCLUDED)
            R_BSP_InterruptRequestDisable(VECT(QSPI, SPRI));
            IR(QSPI, SPTR)    = 0;
            IPR(QSPI, SPRI)   = QSPI_SMSTR_CFG_CH1_INT_SPRI_LEVEL;
            if (0 == IPR(QSPI, SPRI))
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif  /* defined(QSPI_SMSTR_CFG_CH1_INCLUDED) */
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_int_spti_ier_set
* Description  : Sets the ICU.IERm.IENj bit of SPTI to 1.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_int_spti_ier_set(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            R_BSP_InterruptRequestEnable(VECT(QSPI, SPTI));                /* Enable QSPI0.SPTI0 interrupt request.    */
            if (1 == IEN(QSPI, SPTI))
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif  /* defined(QSPI_SMSTR_CFG_CH0_INCLUDED) */
        break;
        
        case 1:
#if defined(QSPI_SMSTR_CFG_CH1_INCLUDED)
            R_BSP_InterruptRequestEnable(VECT(QSPI, SPTI));                /* Enable QSPI1.SPTI1 interrupt request.    */
            if (1 == IEN(QSPI, SPTI))
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif  /* defined(QSPI_SMSTR_CFG_CH1_INCLUDED) */
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/*******************************************************************************
* Function Name: r_qspi_smstr_int_spri_ier_set
* Description  : Sets the ICU.IERm.IENj bit of SPRI to 1.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
void r_qspi_smstr_int_spri_ier_set(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            R_BSP_InterruptRequestEnable(VECT(QSPI, SPRI));                /* Enable QSPI0.SPRI0 interrupt request.    */
            if (1 == IEN(QSPI, SPRI))
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif  /* defined(QSPI_SMSTR_CFG_CH0_INCLUDED) */
        break;
        
        case 1:
#if defined(QSPI_SMSTR_CFG_CH1_INCLUDED)
            R_BSP_InterruptRequestEnable(VECT(QSPI, SPRI));                /* Enable QSPI1.SPRI1 interrupt request.    */
            if (1 == IEN(QSPI, SPRI))
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif  /* defined(QSPI_SMSTR_CFG_CH1_INCLUDED) */
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/**********************************************************************************************************************
 * Function Name: R_QSPI_SMstr_Int_Spti_Ier_Clear
 *****************************************************************************************************************/ /**
 * @brief This function is used to clear the ICU.IERm.IENj bit of the transmit buffer-empty interrupt (SPTI).
 * @param[in] channel
 *             QSPI channel number
 * @details   Use this function when disabling interrupts from within the handler of the SPTI interrupt generated at 
 *            DMAC transfer-end. \n
 * @note      Do not use this function for software transfers or DTC transfers. Doing so could disrupt the transfer.
 */
void R_QSPI_SMstr_Int_Spti_Ier_Clear(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            R_BSP_InterruptRequestDisable(VECT(QSPI, SPTI));                /* Disable QSPI0.SPTI0 interrupt request.   */
            IR(QSPI,SPTI)     = 0;                /* Clear QSPI0.SPTI0 interrupt status flag. */
            if (0 == IR(QSPI, SPTI))
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif  /* defined(QSPI_SMSTR_CFG_CH0_INCLUDED) */
        break;
        
        case 1:
#if defined(QSPI_SMSTR_CFG_CH1_INCLUDED)
            R_BSP_InterruptRequestDisable(VECT(QSPI, SPTI));                /* Disable QSPI1.SPTI1 interrupt request.   */
            IR(QSPI,SPTI)     = 0;                /* Clear QSPI1.SPTI1 interrupt status flag. */
            if (0 == IR(QSPI, SPTI))
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif  /* defined(QSPI_SMSTR_CFG_CH1_INCLUDED) */
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/**********************************************************************************************************************
 * Function Name: R_QSPI_SMstr_Int_Spri_Ier_Clear
 *****************************************************************************************************************/ /**
 * @brief This function is used to clear the ICU.IERm.IENj bit of the receive buffer-full interrupt (SPRI).
 * @param[in] channel
 *             QSPI channel number
 * @details   Use this function when disabling interrupts from within the handler of the SPRI interrupt generated at 
 *            DMAC transfer-end. \n
 * @note      Do not use this function for software transfers or DTC transfers. Doing so could disrupt the transfer.
 */
void R_QSPI_SMstr_Int_Spri_Ier_Clear(uint8_t channel)
{
    switch (channel)
    {
        case 0:
#if defined(QSPI_SMSTR_CFG_CH0_INCLUDED)
            R_BSP_InterruptRequestDisable(VECT(QSPI, SPRI));                /* Disable QSPI0.SPRI0 interrupt request.   */
            IR(QSPI,SPRI)     = 0;                /* Clear QSPI0.SPRI0 interrupt status flag. */
            if (0 == IR(QSPI, SPRI))
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif  /* defined(QSPI_SMSTR_CFG_CH0_INCLUDED) */
        break;
        
        case 1:
#if defined(QSPI_SMSTR_CFG_CH1_INCLUDED)
            R_BSP_InterruptRequestDisable(VECT(QSPI, SPRI));                /* Disable QSPI1.SPRI1 interrupt request.   */
            IR(QSPI,SPRI)     = 0;                /* Clear QSPI1.SPRI1 interrupt status flag. */
            if (0 == IR(QSPI, SPRI))
            {
                /* Wait for the write completion. */
                R_BSP_NOP();
            }
#endif  /* defined(QSPI_SMSTR_CFG_CH1_INCLUDED) */
        break;
        
        default:
            /* Should never get here. Valid channel number is checked above. */
        break;
    }
}


/**********************************************************************************************************************
 * Function Name: R_QSPI_SMstr_Int_Spti_Dmacdtc_flag_Set
 *****************************************************************************************************************/ /**
 * @brief This function is used to set the DMAC or DTC transfer-end flag for data transmission.
 * @param[in] channel
 *             QSPI channel number
 * @param[in] flg
 *             Flag. The settings are as follows.\n
 *             QSPI_SET_TRANS_STOP : DMAC or DTC transfer-end\n
 *             (QSPI_SET_TRANS_START : DMAC or DTC transfer-start: Setting by the user is prohibited.)
 * @retval    QSPI_SMSTR_SUCCESS     Successful operation
 * @retval    QSPI_SMSTR_ERR_PARAM   Parameter error
 * @details   Set QSPI_SET_TRANS_STOP from within the handler of the SPTI interrupt generated at DMAC transfer-end.
 * @note      Do not use this function for software transfers or DTC transfers. Doing so could disrupt the transfer.
 */
qspi_smstr_status_t R_QSPI_SMstr_Int_Spti_Dmacdtc_Flag_Set(uint8_t channel, qspi_smstr_trans_flg_t flg)
{
    if ((QSPI_SET_TRANS_STOP != flg) && (QSPI_SET_TRANS_START != flg))
    {
        return QSPI_SMSTR_ERR_PARAM;
    }
    
    gs_qspi_int_spti_dmacdtc_flg[channel] = flg;
    
    return QSPI_SMSTR_SUCCESS;
}


/**********************************************************************************************************************
 * Function Name: R_QSPI_SMstr_Int_Spri_Dmacdtc_flag_Set
 *****************************************************************************************************************/ /**
 * @brief This function is used to set the DMAC or DTC transfer-end flag for data reception.
 * @param[in] channel
 *             QSPI channel number
 * @param[in] flg
 *             Flag. The settings are as follows.\n
 *             QSPI_SET_TRANS_STOP : DMAC or DTC transfer-end\n
 *             (QSPI_SET_TRANS_START  : DMAC or DTC transfer-start: Setting by the user is prohibited.)
 * @retval    QSPI_SMSTR_SUCCESS    Successful operation
 * @retval    QSPI_SMSTR_ERR_PARAM  Parameter error
 * @details   Set QSPI_SET_TRANS_STOP from within the handler of the SPRI interrupt generated at DMAC transfer-end.
 * @note      Do not use this function for software transfers or DTC transfers. Doing so could disrupt the transfer.
 */
qspi_smstr_status_t R_QSPI_SMstr_Int_Spri_Dmacdtc_Flag_Set(uint8_t channel, qspi_smstr_trans_flg_t flg)
{
    if ((QSPI_SET_TRANS_STOP != flg) && (QSPI_SET_TRANS_START != flg))
    {
        return QSPI_SMSTR_ERR_PARAM;
    }
    
    gs_qspi_int_spri_dmacdtc_flg[channel] = flg;
    
    return QSPI_SMSTR_SUCCESS;
}


/**********************************************************************************************************************
 * Function Name: R_QSPI_SMstr_1ms_Interval
 *****************************************************************************************************************/ /**
 * @brief This function increments the internal timer counter each time it is called.
 * @details   Increments the internal timer counter while waiting for the DMAC transfer or DTC transfer to finish.
 * @note      Use a timer or the like to call this function at 1 millisecond (ms) intervals.
 */
void R_QSPI_SMstr_1ms_Interval(void)
{
    uint8_t channel = 0;

    /* WAIT_LOOP */
    for (channel = 0; channel < QSPI_NUM_CHANNELS; channel++)
    {
        if (gs_qspi_timer_cnt_out[channel][QSPI_TIMER_CH_FLG] != 0)
        {
            gs_qspi_timer_cnt_out[channel][QSPI_TIMER_CH_COUNT]++;
        }
    }

    return;
}


/*******************************************************************************
* Function Name: r_qspi_smstr_start_timer
* Description  : Sets timer for checking timeout and start timer.
* Arguments    : channel -
*                    Which channel to use
*              : msec -
*                    timeout(msec order)
* Return Value : none
*******************************************************************************/
static void r_qspi_smstr_start_timer(uint8_t channel, uint32_t msec)
{
    gs_qspi_timer_cnt_out[channel][QSPI_TIMER_CH_FLG] = 1;
    gs_qspi_timer_cnt_out[channel][QSPI_TIMER_CH_COUNT] = 0;
    gs_qspi_timer_cnt_out[channel][QSPI_TIMER_CH_MAX_COUNT] = (uint32_t)(msec);
}


/*******************************************************************************
* Function Name: r_qspi_smstr_check_timer
* Description  : Checks timeout to set r_qspi_smstr_start_timer function.
*              : If timeout,return QSPI_SMSTR_ERR_OTHER. 
*              : In the case of others, return QSPI_SMSTR_SUCCESS.
* Arguments    : channel -
*                    Which channel to use
* Return Value : QSPI_SMSTR_SUCCESS -
*                    Successful operation
*                QSPI_SMSTR_ERR_HARD -
*                    Hardware error
*******************************************************************************/
static qspi_smstr_status_t r_qspi_smstr_check_timer(uint8_t channel)
{
    qspi_smstr_status_t ret = QSPI_SMSTR_SUCCESS;
    uint32_t gs_qspi_timer_cnt_out1 = gs_qspi_timer_cnt_out[channel][QSPI_TIMER_CH_COUNT];
    uint32_t gs_qspi_timer_cnt_out2 = gs_qspi_timer_cnt_out[channel][QSPI_TIMER_CH_MAX_COUNT];

    /* ---- Check compare ---- */
    if (gs_qspi_timer_cnt_out1 >= gs_qspi_timer_cnt_out2)
    {
        ret = QSPI_SMSTR_ERR_HARD;
    }
    else
    {
        ret = QSPI_SMSTR_SUCCESS;
    }

    return ret;
}


/*******************************************************************************
* Function Name: r_qspi_smstr_end_timer
* Description  : Stops timer to set r_qspi_smstr_start_timer function.
* Arguments    : channel -
*                    Which channel to use
* Return Value : none
*******************************************************************************/
static void r_qspi_smstr_end_timer(uint8_t channel)
{
    gs_qspi_timer_cnt_out[channel][QSPI_TIMER_CH_FLG] = 0;
}


#endif /* defined(BSP_MCU_RX72N) */

/* End of File */

