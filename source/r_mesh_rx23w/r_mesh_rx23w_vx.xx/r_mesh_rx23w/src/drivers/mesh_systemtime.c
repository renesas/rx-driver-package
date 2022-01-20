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
* Copyright (C) 2020-2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/*******************************************************************************
* Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "platform.h"
#include "mesh_systemtime.h"
#include "MS_systemtime_pl.h"

#if SYSTEMTIME_EN

/*******************************************************************************
* Global Variables and Private Functions declaration
*******************************************************************************/
static uint32_t gs_systemtime_ms;
static uint16_t gs_tmr_timeout_ms;
static uint16_t gs_tmr_timeout_clk;
static uint16_t gs_tmr_conv_ms;
static uint16_t gs_tmr_conv_clk;

/*******************************************************************************
* Functions
*******************************************************************************/
/***************************************************************************//**
* @brief Initialize 8bit-timer TMR2 and TMR3 for generating System Time
*******************************************************************************/
uint8_t mesh_systemtime_init(void)
{
    /* TMR2 and TMR3 are cascaded-connected and used as a 16-bit count timer */
    /* PCLKB for TMR2 and TMR3 is divided by '8192' */

    /* Lock TMR2 and TMR3 */
    if (false == R_BSP_HardwareLock(BSP_LOCK_TMR2))
    {
        return 1;
    }
    if (false == R_BSP_HardwareLock(BSP_LOCK_TMR3))
    {
        R_BSP_HardwareUnlock(BSP_LOCK_TMR2);
        return 2;
    }

    gs_systemtime_ms = 0;

    /* NOTE: PCLKB must be lower than 65.535MHz */
    {
        const uint16_t clk_div = 8192; /* clock divisor of PCLKB, i.e. multiplier for time */
        uint16_t clk_per_ms = (uint16_t)(BSP_PCLKB_HZ / 1000); /* [clk/ms] */
        uint16_t grt_cmn_div = 0; /* greatest common divisor */

        /* calculate log_2 of a greatest common divisor of (BSP_PCLKB_HZ / 1000) and 8192 */
        while (grt_cmn_div <= 13) /* 13=log_2(8192) */
        {
            grt_cmn_div++;
            /* equivalent to "(clk_per_ms % (2^grt_cmn_div)) != 0" */
            if (clk_per_ms != ((clk_per_ms >> grt_cmn_div) << grt_cmn_div))
            {
                grt_cmn_div--;
                break;
            }
        }

        /* calculate a greatest common divisor */
        grt_cmn_div = 1 << grt_cmn_div;

        /* calculate [ms] vs [clk] conversion parameters */
        gs_tmr_conv_clk = clk_per_ms / grt_cmn_div;                        /* 0x7D[clk]   at PCLKB=32MHz */
        gs_tmr_conv_ms  = clk_div    / grt_cmn_div;                        /* 32[ms]      at PCLKB=32MHz */
        gs_tmr_timeout_clk = gs_tmr_conv_clk * (0xFFFF / gs_tmr_conv_clk); /* 0xFFDC[clk] at PCLKB=32MHz */
        gs_tmr_timeout_ms  = gs_tmr_conv_ms  * (0xFFFF / gs_tmr_conv_clk); /* 16768[ms]   at PCLKB=32MHz */
    }

    /* Enable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Enable Module Clock */
    MSTP(TMR2) = 0;

    /* Set 16-bit Count Mode */
    TMR2.TCCR.BYTE = 0x18;

    /* Set Compare Match A Value */
    /* e.g. 0xFFDC[clk] is equivalent to 16,768[ms] at PCLKB=32MHz */
    TMR23.TCORA = gs_tmr_timeout_clk - 1;

    /* Clear Count by Compare Match A */
    TMR2.TCR.BIT.CCLR = 0x1;

    /* Enable Compare Match A Interrupt */
    TMR2.TCR.BIT.CMIEA = 1;

    /* Configure and Enable Interrupt */
    IR(TMR2, CMIA2)  = 0;
    IPR(TMR2, CMIA2) = 4;
    R_BSP_InterruptRequestEnable(VECT(TMR2, CMIA2));

    /* Start Counting with PCLKB/8192 Clock */
    TMR3.TCCR.BYTE = 0x0E;

    /* Disable writing to MSTP registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Register Sytemtime Read Function */
    MS_systemtime_init_pl(mesh_systemtime_read);

    return 0;
}

/***************************************************************************//**
* @brief Check if 8bit Timers for System Time are active
*******************************************************************************/
uint8_t mesh_systemtime_is_active(void)
{
    return (TMR3.TCCR.BYTE != 0);
}

/***************************************************************************//**
* @brief Read System Time
*******************************************************************************/
uint32_t mesh_systemtime_read(void)
{
    /* Convert clocks to milliseconds */
    /* e.g. 125[clk] is equivalent to 32[ms] at PCLKB=32MHz */
    uint32_t timer_ms = (TMR23.TCNT * gs_tmr_conv_ms) / gs_tmr_conv_clk;

    return (timer_ms + gs_systemtime_ms);
}

#if SYSTEMTIME_STRING_EN
/***************************************************************************//**
* @brief Return String of System Time
*******************************************************************************/
char * mesh_systemtime_read_string(void)
{
    static char str[16];
    uint32_t time = mesh_systemtime_read();
    uint32_t time_ms, time_s, time_m, time_h;

    time_s = time / 1000;
    time_ms = time % 1000;
    if (time_s < 60UL)
    {
        sprintf(str, "0:00:%02ld.%03ld", time_s, time_ms);
    }
    else
    {
        time_m = time_s / 60;
        time_s = time_s % 60;
        if (time_m < 60UL)
        {
            sprintf(str, "0:%02ld:%02ld.%03ld", time_m, time_s, time_ms);
        }
        else
        {
            time_h = time_m / 60;
            time_m = time_m % 60;
            sprintf(str, "%ld:%02ld:%02ld.%03ld", time_h, time_m, time_s, time_ms);
        }
    }
    return str;
}
#endif /* SYSTEMTIME_STRING_EN */

/***************************************************************************//**
* @brief Interrupt Service Routine to catch wrap-around of timer
*******************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(mesh_tmr2_isr, VECT(TMR2, CMIA2))
R_BSP_ATTRIB_STATIC_INTERRUPT void mesh_tmr2_isr(void)
{
    /* Add Timeout time when Compare Match Interrupt occurs */
    /* e.g. Interrupt Cycle is 0xFFDC[clk]=16,768[ms] at PCLKB=32MHz */
    gs_systemtime_ms += gs_tmr_timeout_ms;
}

#endif /* SYSTEMTIME_EN */
