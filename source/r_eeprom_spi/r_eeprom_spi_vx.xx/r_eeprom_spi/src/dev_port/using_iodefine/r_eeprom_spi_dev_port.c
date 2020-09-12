/************************************************************************************************
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
* Copyright (C) 2014(2019) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* System Name  : EEPROM driver software
* File Name    : r_eeprom_spi_dev_port.c
* Version      : 3.01
* Device       : -
* Abstract     : Device port file
* Tool-Chain   : -
* OS           : not use
* H/W Platform : -
* Description  : EEPROM driver device port source file
* Limitation   : None
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 28.11.2014 2.30     Revised functions of same as Ver.2.30 of other middleware.
*              : 30.01.2015 2.31     Added RX71M.
*              : 29.05.2015 2.32     Added RX231 and RX230.
*              : 04.04.2019 3.01     Added support for GNUC and ICCRX.
*                                    Fixed coding style.
*************************************************************************************************/


/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_eeprom_spi_if.h"                      /* EEPROM driver I/F definitions                 */
#include "r_eeprom_spi_config.h"                  /* EEPROM driver Configuration definitions       */
#include "./src/r_eeprom_spi_private.h"           /* EEPROM driver Private module definitions      */

/* Check MCU Group. */
#if ((defined(EEPROM_SPI_CFG_USE_FIT) && !defined(EEPROM_SPI_CFG_USE_GPIO_MPC_FIT)) || \
     (!defined(EEPROM_SPI_CFG_USE_FIT)))


/************************************************************************************************
Macro definitions
*************************************************************************************************/
#define EEPROM_SPI_OVERHEAD_LOOP_COUNT   (4)
        /* Overhead of 20 cycles or 4 loops to call/return from r_scifa_smstr_delaywait() function. */
#define EEPROM_SPI_CPU_CYCLES_PER_LOOP   (5)
        /* Known number (5) of CPU cycles required to execute the r_scifa_smstr_delaywait() loop. */


/************************************************************************************************
Typedef definitions
*************************************************************************************************/


/************************************************************************************************
Exported global variables (to be accessed by other files)
*************************************************************************************************/


/************************************************************************************************
Private global variables and functions
*************************************************************************************************/


/************************************************************************************************
* Function Name: r_eeprom_spi_cs_init
* Description  : Initializes the ports related to the specified device.
* Arguments    : uint8_t           devno               ;   Device No. (EEPROM_SPI_DEVn)
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_eeprom_spi_cs_init(uint8_t devno)
{
    switch (devno)
    {
        case EEPROM_SPI_DEV0:
#if defined(EEPROM_SPI_CFG_DEV0_INCLUDED)
            EEPROM_SPI_DEV0_PMR_CS     = 0;                /* Use as a general I/O port.           */
            if (0 == EEPROM_SPI_DEV0_PMR_CS)
            {
                /* Wait for the write completion. */
            }
            EEPROM_SPI_PWPR_BOWI       = 0;                /* Enable to write the PFSWE bit.       */
            EEPROM_SPI_PWPR_PFSWE      = 1;                /* Enable to write the PFS registers.   */
            if (1 == EEPROM_SPI_PWPR_PFSWE)
            {
                /* Wait for the write completion. */
            }
            EEPROM_SPI_DEV0_MPC_CS     = 0x00;             /* Pin Function Select : Hi-Z           */
            if (0x00 == EEPROM_SPI_DEV0_MPC_CS)
            {
                /* Wait for the write completion. */
            }
            EEPROM_SPI_PWPR_PFSWE      = 0;                /* Disable to write the PFS registers.  */
            EEPROM_SPI_PWPR_BOWI       = 1;                /* Disable to write the PFSWE bit.      */
            if (1 == EEPROM_SPI_PWPR_BOWI)
            {
                /* Wait for the write completion. */
            }
            EEPROM_SPI_DEV0_PODR_CS    = EEPROM_SPI_HI;    /* Output data is High.                 */
            EEPROM_SPI_DEV0_PDR_CS     = EEPROM_SPI_OUT;   /* Select output direction.             */
            if (EEPROM_SPI_OUT == EEPROM_SPI_DEV0_PDR_CS)
            {
                /* Wait for the write completion. */
            }
#endif  /* #if defined(EEPROM_SPI_CFG_DEV0_INCLUDED) */
        break;
        case EEPROM_SPI_DEV1:
#if defined(EEPROM_SPI_CFG_DEV1_INCLUDED)
            EEPROM_SPI_DEV1_PMR_CS     = 0;                /* Use as a general I/O port.           */
            if (0 == EEPROM_SPI_DEV1_PMR_CS)
            {
                /* Wait for the write completion. */
            }
            EEPROM_SPI_PWPR_BOWI       = 0;                /* Enable to write the PFSWE bit.       */
            EEPROM_SPI_PWPR_PFSWE      = 1;                /* Enable to write the PFS registers.   */
            if (1 == EEPROM_SPI_PWPR_PFSWE)
            {
                /* Wait for the write completion. */
            }
            EEPROM_SPI_DEV1_MPC_CS     = 0x00;             /* Pin Function Select : Hi-Z           */
            if (0x00 == EEPROM_SPI_DEV1_MPC_CS)
            {
                /* Wait for the write completion. */
            }
            EEPROM_SPI_PWPR_PFSWE      = 0;                /* Disable to write the PFS registers.  */
            EEPROM_SPI_PWPR_BOWI       = 1;                /* Disable to write the PFSWE bit.      */
            if (1 == EEPROM_SPI_PWPR_BOWI)
            {
                /* Wait for the write completion. */
            }
            EEPROM_SPI_DEV1_PODR_CS    = EEPROM_SPI_HI;    /* Output data is High.                 */
            EEPROM_SPI_DEV1_PDR_CS     = EEPROM_SPI_OUT;   /* Select output direction.             */
            if (EEPROM_SPI_OUT == EEPROM_SPI_DEV1_PDR_CS)
            {
                /* Wait for the write completion. */
            }
#endif  /* #if defined(EEPROM_SPI_CFG_DEV1_INCLUDED) */
        break;
        default:
            /* Do nothing. */
        break;
    }
}


/************************************************************************************************
* Function Name: r_eeprom_spi_cs_reset
* Description  : Resets the ports related to the specified device.
* Arguments    : uint8_t           devno               ;   Device No. (EEPROM_SPI_DEVn)
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_eeprom_spi_cs_reset(uint8_t devno)
{
    switch (devno)
    {
        case EEPROM_SPI_DEV0:
#if defined(EEPROM_SPI_CFG_DEV0_INCLUDED)
            EEPROM_SPI_DEV0_PMR_CS     = 0;                /* Use as a general I/O port.           */
            if (0 == EEPROM_SPI_DEV0_PMR_CS)
            {
                /* Wait for the write completion. */
            }
#endif  /* #if defined(EEPROM_SPI_CFG_DEV0_INCLUDED) */
        break;
        case EEPROM_SPI_DEV1:
#if defined(EEPROM_SPI_CFG_DEV1_INCLUDED)
            EEPROM_SPI_DEV1_PMR_CS     = 0;                /* Use as a general I/O port.           */
            if (0 == EEPROM_SPI_DEV1_PMR_CS)
            {
                /* Wait for the write completion. */
            }
#endif  /* #if defined(EEPROM_SPI_CFG_DEV1_INCLUDED) */
        break;
        default:
            /* Do nothing. */
        break;
    }
}


/************************************************************************************************
* Function Name: r_eeprom_spi_set_cs
* Description  : Sets the state of CS pin.
* Arguments    : uint8_t           devno               ;   Device No. (EEPROM_SPI_DEVn)
*              : uint8_t           lv                  ;   CS output level
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_eeprom_spi_set_cs(uint8_t devno, uint8_t lv)
{
    switch (devno)
    {
        case EEPROM_SPI_DEV0:
#if defined(EEPROM_SPI_CFG_DEV0_INCLUDED)
            EEPROM_SPI_DEV0_PODR_CS    = lv;
            if (lv == EEPROM_SPI_DEV0_PODR_CS)
            {
                /* Wait for the write completion. */
            }
#endif  /* #if defined(EEPROM_SPI_CFG_DEV0_INCLUDED) */
        break;
        case EEPROM_SPI_DEV1:
#if defined(EEPROM_SPI_CFG_DEV1_INCLUDED)
            EEPROM_SPI_DEV1_PODR_CS    = lv;
            if (lv == EEPROM_SPI_DEV1_PODR_CS)
            {
                /* Wait for the write completion. */
            }
#endif  /* #if defined(EEPROM_SPI_CFG_DEV1_INCLUDED) */
        break;
        default:
            /* Do nothing. */
        break;
    }
}


/************************************************************************************************
* Function Name: r_eeprom_spi_delaywait
* Description  : This asm loop executes a known number (5) of CPU cycles.
*              ; If a value of '4' is passed in as an argument,
*              ; then this function would consume 20 CPU cycles before returning.
* Arguments    : unsigned long      loop_cnt            ;   The number of 'units' to delay.
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE_ASM(r_eeprom_spi_delaywait)
void r_eeprom_spi_delaywait (unsigned long loop_cnt)
{
    R_BSP_ASM_INTERNAL_USED(loop_cnt)
    R_BSP_ASM_BEGIN
    R_BSP_ASM(    BRA.B   R_BSP_ASM_LAB_NEXT(0)     )
    R_BSP_ASM(    NOP                               )
    R_BSP_ASM_LAB(0:                                )
    R_BSP_ASM(    NOP                               )
    R_BSP_ASM(    SUB     #01H, R1                  )
    R_BSP_ASM(    BNE.B   R_BSP_ASM_LAB_PREV(0)     )
    R_BSP_ASM_END
}


/************************************************************************************************
* Function Name: r_eeprom_spi_softwaredelay
* Description  : Delay the specified duration in units and return.
* Arguments    : uint32_t           delay               ;   The number of 'units' to delay.
*              : bsp_delay_units_t  units               ;   the 'base' for the units specified.
*              :                                        ;   Valid values are: BSP_DELAY_MICROSECS,
*              :                                        ;                     BSP_DELAY_MILLISECS,
*              :                                        ;                     BSP_DELAY_SECS
* Return Value : true           ;   if delay executed.
*              : false          ;   if delay/units combination resulted in overflow/underflow.
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
bool r_eeprom_spi_softwaredelay(uint32_t delay, bsp_delay_units_t units)
{
    uint64_t loop_cnt;

    /* Calculate the number of loops, accounting for the overhead of 20 cycles (4 loops at 5 cycles/loop) */
    loop_cnt = (((uint64_t)delay * (uint64_t)BSP_ICLK_HZ) / (EEPROM_SPI_CPU_CYCLES_PER_LOOP * units))
                - EEPROM_SPI_OVERHEAD_LOOP_COUNT;

    /* Make sure the request is valid and did not result in an overflow. */
    if ((loop_cnt > 0xFFFFFFFF) || (loop_cnt == 0) ||
        ((units != BSP_DELAY_MICROSECS) && (units != BSP_DELAY_MILLISECS) && (units != BSP_DELAY_SECS)))
    {
        return (false);
    }
    r_eeprom_spi_delaywait((uint32_t)loop_cnt);
    return (true);
}


/************************************************************************************************
* Function Name: r_eeprom_spi_wait_lp
* Description  : Loop Timer Processing
* Arguments    : uint8_t            unit                   ;   Timer unit
* Return Value : EEPROM_SPI_SUCCESS                        ;   Successful operation
*              : EEPROM_SPI_ERR_OTHER                      ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
eeprom_status_t r_eeprom_spi_wait_lp(uint8_t unit)
{
    if (EEPROM_SPI_UINT_MICROSECS == unit)
    {
        /* Wait timer of us. */
        if (true != r_eeprom_spi_softwaredelay(1, BSP_DELAY_MICROSECS))
        {
            return EEPROM_SPI_ERR_OTHER;
        }
    }
    else
    {
        /* Wait timer of ms. */
        if (true != r_eeprom_spi_softwaredelay(1, BSP_DELAY_MILLISECS))
        {
            return EEPROM_SPI_ERR_OTHER;
        }
    }
    return EEPROM_SPI_SUCCESS;
}


#endif  /* #if (defined(EEPROM_SPI_CFG_USE_FIT) && defined(BSP_MCU_RX64M) && 
                !defined(EEPROM_SPI_CFG_USE_GPIO_MPC_FIT)) || \
                (!defined(EEPROM_SPI_CFG_USE_FIT) && defined(EEPROM_SPI_CFG_RX64M))) */

/* End of File */
