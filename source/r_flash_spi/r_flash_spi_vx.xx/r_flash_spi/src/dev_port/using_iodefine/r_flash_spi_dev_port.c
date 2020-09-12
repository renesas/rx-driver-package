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
* Copyright (C) 2011(2012-2019) Renesas Electronics Corporation. All rights reserved.
*************************************************************************************************/
/************************************************************************************************
* System Name  : FLASH SPI driver software
* File Name    : r_flash_spi_dev_port.c
* Version      : 3.01
* Device       : -
* Abstract     : Device port file
* Tool-Chain   : -
* OS           : not use
* H/W Platform : -
* Description  : SPI FLASH driver device port source file
* Limitation   : None
*************************************************************************************************/
/************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 23.07.2014 2.21     Created
*              : 29.05.2015 2.32     Revised functions of same as Ver.2.32 of EEPROM SPI FIT module.
*              : 04.04.2019 3.01     Added support for GNUC and ICCRX.
*                                    Fixed coding style.
*************************************************************************************************/


/************************************************************************************************
Includes <System Includes> , "Project Includes"
*************************************************************************************************/
#include "r_flash_spi_if.h"                      /* FLASH driver I/F definitions                 */
#include "r_flash_spi_config.h"                  /* FLASH driver Configuration definitions       */
#include "./src/r_flash_spi_private.h"           /* FLASH driver Private module definitions      */

/* Check MCU Group. */
#if ((defined(FLASH_SPI_CFG_USE_FIT) && (FLASH_SPI_CFG_USE_GPIO_MPC_FIT == 0)) || \
     (!defined(FLASH_SPI_CFG_USE_FIT)))


/************************************************************************************************
Macro definitions
*************************************************************************************************/
#define FLASH_SPI_OVERHEAD_LOOP_COUNT   (4)
        /* Overhead of 20 cycles or 4 loops to call/return from r_scifa_smstr_delaywait() function. */
#define FLASH_SPI_CPU_CYCLES_PER_LOOP   (5)
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
* Function Name: r_flash_spi_cs_init
* Description  : Initializes the ports related to the specified device.
* Arguments    : uint8_t           devno               ;   Device No. (FLASH_SPI_DEVn)
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_flash_spi_cs_init(uint8_t devno)
{
    switch (devno)
    {
        case FLASH_SPI_DEV0:
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
            FLASH_SPI_DEV0_PMR_CS     = 0;                /* Use as a general I/O port.           */
            if (0 == FLASH_SPI_DEV0_PMR_CS)
            {
                /* Wait for the write completion. */
            }
            FLASH_SPI_PWPR_BOWI       = 0;                /* Enable to write the PFSWE bit.       */
            FLASH_SPI_PWPR_PFSWE      = 1;                /* Enable to write the PFS registers.   */
            if (1 == FLASH_SPI_PWPR_PFSWE)
            {
                /* Wait for the write completion. */
            }
            FLASH_SPI_DEV0_MPC_CS     = 0x00;             /* Pin Function Select : Hi-Z           */
            if (0x00 == FLASH_SPI_DEV0_MPC_CS)
            {
                /* Wait for the write completion. */
            }
            FLASH_SPI_PWPR_PFSWE      = 0;                /* Disable to write the PFS registers.  */
            FLASH_SPI_PWPR_BOWI       = 1;                /* Disable to write the PFSWE bit.      */
            if (1 == FLASH_SPI_PWPR_BOWI)
            {
                /* Wait for the write completion. */
            }
            FLASH_SPI_DEV0_PODR_CS    = FLASH_SPI_HI;    /* Output data is High.                 */
            FLASH_SPI_DEV0_PDR_CS     = FLASH_SPI_OUT;   /* Select output direction.             */
            if (FLASH_SPI_OUT == FLASH_SPI_DEV0_PDR_CS)
            {
                /* Wait for the write completion. */
            }
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
        break;
        case FLASH_SPI_DEV1:
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
            FLASH_SPI_DEV1_PMR_CS     = 0;                /* Use as a general I/O port.           */
            if (0 == FLASH_SPI_DEV1_PMR_CS)
            {
                /* Wait for the write completion. */
            }
            FLASH_SPI_PWPR_BOWI       = 0;                /* Enable to write the PFSWE bit.       */
            FLASH_SPI_PWPR_PFSWE      = 1;                /* Enable to write the PFS registers.   */
            if (1 == FLASH_SPI_PWPR_PFSWE)
            {
                /* Wait for the write completion. */
            }
            FLASH_SPI_DEV1_MPC_CS     = 0x00;             /* Pin Function Select : Hi-Z           */
            if (0x00 == FLASH_SPI_DEV1_MPC_CS)
            {
                /* Wait for the write completion. */
            }
            FLASH_SPI_PWPR_PFSWE      = 0;                /* Disable to write the PFS registers.  */
            FLASH_SPI_PWPR_BOWI       = 1;                /* Disable to write the PFSWE bit.      */
            if (1 == FLASH_SPI_PWPR_BOWI)
            {
                /* Wait for the write completion. */
            }
            FLASH_SPI_DEV1_PODR_CS    = FLASH_SPI_HI;    /* Output data is High.                 */
            FLASH_SPI_DEV1_PDR_CS     = FLASH_SPI_OUT;   /* Select output direction.             */
            if (FLASH_SPI_OUT == FLASH_SPI_DEV1_PDR_CS)
            {
                /* Wait for the write completion. */
            }
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
        break;
        default:
            /* Do nothing. */
        break;
    }
}


/************************************************************************************************
* Function Name: r_flash_spi_cs_reset
* Description  : Resets the ports related to the specified device.
* Arguments    : uint8_t           devno               ;   Device No. (FLASH_SPI_DEVn)
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_flash_spi_cs_reset(uint8_t devno)
{
    switch (devno)
    {
        case FLASH_SPI_DEV0:
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
            FLASH_SPI_DEV0_PMR_CS     = 0;                /* Use as a general I/O port.           */
            if (0 == FLASH_SPI_DEV0_PMR_CS)
            {
                /* Wait for the write completion. */
            }
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
        break;
        case FLASH_SPI_DEV1:
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
            FLASH_SPI_DEV1_PMR_CS     = 0;                /* Use as a general I/O port.           */
            if (0 == FLASH_SPI_DEV1_PMR_CS)
            {
                /* Wait for the write completion. */
            }
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
        break;
        default:
            /* Do nothing. */
        break;
    }
}


/************************************************************************************************
* Function Name: r_flash_spi_set_cs
* Description  : Sets the state of CS pin.
* Arguments    : uint8_t           devno               ;   Device No. (FLASH_SPI_DEVn)
*              : uint8_t           lv                  ;   CS output level
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
void r_flash_spi_set_cs(uint8_t devno, uint8_t lv)
{
    switch (devno)
    {
        case FLASH_SPI_DEV0:
#if (FLASH_SPI_CFG_DEV0_INCLUDED == 1)
            FLASH_SPI_DEV0_PODR_CS    = lv;
            if (lv == FLASH_SPI_DEV0_PODR_CS)
            {
                /* Wait for the write completion. */
            }
#endif  /* #if (FLASH_SPI_CFG_DEV0_INCLUDED == 1) */
        break;
        case FLASH_SPI_DEV1:
#if (FLASH_SPI_CFG_DEV1_INCLUDED == 1)
            FLASH_SPI_DEV1_PODR_CS    = lv;
            if (lv == FLASH_SPI_DEV1_PODR_CS)
            {
                /* Wait for the write completion. */
            }
#endif  /* #if (FLASH_SPI_CFG_DEV1_INCLUDED == 1) */
        break;
        default:
            /* Do nothing. */
        break;
    }
}


/************************************************************************************************
* Function Name: r_flash_spi_delaywait
* Description  : This asm loop executes a known number (5) of CPU cycles.
*              ; If a value of '4' is passed in as an argument,
*              ; then this function would consume 20 CPU cycles before returning.
* Arguments    : unsigned long      loop_cnt            ;   The number of 'units' to delay.
* Return Value : None
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE_ASM(r_flash_spi_delaywait)
void r_flash_spi_delaywait (unsigned long loop_cnt)
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
* Function Name: r_flash_spi_softwaredelay
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
bool r_flash_spi_softwaredelay(uint32_t delay, bsp_delay_units_t units)
{
    uint64_t loop_cnt;

    /* Calculate the number of loops, accounting for the overhead of 20 cycles (4 loops at 5 cycles/loop) */
    loop_cnt = (((uint64_t)delay * (uint64_t)BSP_ICLK_HZ) / (FLASH_SPI_CPU_CYCLES_PER_LOOP * units))
                - FLASH_SPI_OVERHEAD_LOOP_COUNT;

    /* Make sure the request is valid and did not result in an overflow. */
    if ((loop_cnt > 0xFFFFFFFF) || (loop_cnt == 0) ||
        ((units != BSP_DELAY_MICROSECS) && (units != BSP_DELAY_MILLISECS) && (units != BSP_DELAY_SECS)))
    {
        return (false);
    }
    r_flash_spi_delaywait((uint32_t)loop_cnt);
    return (true);
}


/************************************************************************************************
* Function Name: r_flash_spi_wait_lp
* Description  : Loop Timer Processing
* Arguments    : uint8_t            unit                  ;   Timer unit
* Return Value : FLASH_SPI_SUCCESS                        ;   Successful operation
*              : FLASH_SPI_ERR_OTHER                      ;   Other error
*------------------------------------------------------------------------------------------------
* Notes        : None
*************************************************************************************************/
flash_spi_status_t r_flash_spi_wait_lp(uint8_t unit)
{
    if (FLASH_SPI_UINT_MICROSECS == unit)
    {
        /* Wait timer of us. */
        if (true != r_flash_spi_softwaredelay(1, BSP_DELAY_MICROSECS))
        {
            return FLASH_SPI_ERR_OTHER;
        }
    }
    else
    {
        /* Wait timer of ms. */
        if (true != r_flash_spi_softwaredelay(1, BSP_DELAY_MILLISECS))
        {
            return FLASH_SPI_ERR_OTHER;
        }
    }
    return FLASH_SPI_SUCCESS;
}


#endif  /* #if (defined(FLASH_SPI_CFG_USE_FIT) && (FLASH_SPI_CFG_USE_GPIO_MPC_FIT == 0)) || \
                (!defined(FLASH_SPI_CFG_USE_FIT))) */

/* End of File */
