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
/***********************************************************************************************************************
 * File Name    : r_lpc_rx110_if.h
 * Description  : Definitions used by the r_lpc_rx110.c module.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description           
 *         : 01.10.2016 1.00    First Release
 ***********************************************************************************************************************/

#ifndef R_LPC_RX110_IF_H
    #define R_LPC_RX110_IF_H

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/
typedef enum lpc_low_power_mode
{
    LPC_LP_SLEEP,
    LPC_LP_DEEP_SLEEP,
    LPC_LP_SW_STANDBY,
    LPC_LP_INVALID_MODE
}lpc_low_power_mode_t;

typedef enum lpc_operating_mode
{
    LPC_OP_HIGH_SPEED = 0x00,
    LPC_OP_MIDDLE_SPEED = 0x02,
    LPC_OP_LOW_SPEED = 0x06,
    LPC_OP_INVALID_MODE
}lpc_operating_mode_t;

/* LPC Sleep Mode Return Clock Switching Sources*/
typedef enum lpc_clock_switch
{
    LPC_LOCO = 0x00,
    LPC_HOCO = 0x01,
    LPC_MAIN_OSC = 0x02,
}lpc_clock_switch_t;

/* Available system clock sources. Maps to CKSEL[2:0] */
typedef enum lpc_clocks
{
    LPC_CKSEL_LOCO = 0x00,
    LPC_CKSEL_HOCO = 0x01,
    LPC_CKSEL_MAIN_OSC = 0x02,
    LPC_CKSEL_SUBCLOCK = 0x03,
    LPC_CKSEL_IWDT = 0x05,                    // cannot be used as a system clock source
    LPC_END_OF_CLOCKLIST
}lpc_clocks_t;

/* System Clocks. Don't change the order of this list. */
typedef enum lpc_system_clocks
{
    LPC_ICLK,
    LPC_FCLK,
    LPC_PCLKB,
    LPC_PCLKD,
}lpc_system_clocks_t;


/***********************************************************************************************************************
 Exported global variables
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/
#endif /* R_LPC_RX110_IF_H */
