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
 * File Name    : r_lpc_rx71m_private.h
 * Description  : Definitions used by the r_lpc_rx71m.c module.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description           
 *         : 01.10.2016 1.00    First Release
 ***********************************************************************************************************************/

#ifndef R_LPC_RX71M_PRIVATE_H
    #define R_LPC_RX71M_PRIVATE_H

/***********************************************************************************************************************
 Error checking for user defined macros
***********************************************************************************************************************/

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
/* Internally used definitions */
    #define LPC_TRANSITION_ONGOING (1)
    #define LPC_TRANSITION_COMPLETE (0)

    #define LPC_OSC_STOP_ENABLED (1)

    #define LPC_SUB_CLOCK (0x0300)
    #define LPC_LOCO      (0x0000)

    #define LPC_CLOCK_SWITCH_ENABLE (0x01)
    #define LPC_CLOCK_SWITCH_DISABLE (0x00)

    #define LPC_CLOCK_INACTIVE    (0x01)
    #define LPC_CLOCK_ACTIVE      (0x00)

    #define LPC_MAX_BCK_HIGH_SPEED_FREQ_HZ    (120000000)
    #define LPC_MAX_FCK_HIGH_SPEED_FREQ_HZ    ( 60000000)
    #define LPC_MAX_ICK_HIGH_SPEED_FREQ_HZ    (240000000)
    #define LPC_MAX_PCKA_HIGH_SPEED_FREQ_HZ   (120000000)
    #define LPC_MAX_PCKB_HIGH_SPEED_FREQ_HZ   ( 60000000)
    #define LPC_MAX_PCKC_HIGH_SPEED_FREQ_HZ   ( 60000000)
    #define LPC_MAX_PCKD_HIGH_SPEED_FREQ_HZ   ( 60000000)
    
    #define LPC_MAX_BCK_LOW1_SPEED_FREQ_HZ    (1000000)
    #define LPC_MAX_FCK_LOW1_SPEED_FREQ_HZ    (1000000)
    #define LPC_MAX_ICK_LOW1_SPEED_FREQ_HZ    (1000000)
    #define LPC_MAX_PCKA_LOW1_SPEED_FREQ_HZ   (1000000)
    #define LPC_MAX_PCKB_LOW1_SPEED_FREQ_HZ   (1000000)
    #define LPC_MAX_PCKC_LOW1_SPEED_FREQ_HZ   (1000000)
    #define LPC_MAX_PCKD_LOW1_SPEED_FREQ_HZ   (1000000)
    
    #define LPC_MAX_BCK_LOW2_SPEED_FREQ_HZ    (264000)
    #define LPC_MAX_FCK_LOW2_SPEED_FREQ_HZ    (264000)
    #define LPC_MAX_ICK_LOW2_SPEED_FREQ_HZ    (264000)
    #define LPC_MAX_PCKA_LOW2_SPEED_FREQ_HZ   (264000)
    #define LPC_MAX_PCKB_LOW2_SPEED_FREQ_HZ   (264000)
    #define LPC_MAX_PCKC_LOW2_SPEED_FREQ_HZ   (264000)
    #define LPC_MAX_PCKD_LOW2_SPEED_FREQ_HZ   (264000)

    #define LPC_MIN_BCK_LOW2_SPEED_FREQ_HZ    (1)
    #define LPC_MIN_FCK_LOW2_SPEED_FREQ_HZ    (32000)
    #define LPC_MIN_ICK_LOW2_SPEED_FREQ_HZ    (32000)
    #define LPC_MIN_PCKA_LOW2_SPEED_FREQ_HZ   (1)
    #define LPC_MIN_PCKB_LOW2_SPEED_FREQ_HZ   (1)
    #define LPC_MIN_PCKC_LOW2_SPEED_FREQ_HZ   (1)
    #define LPC_MIN_PCKD_LOW2_SPEED_FREQ_HZ   (1)

    #define LPC_MEDIUM_SPD (0x02)
    #define LPC_HIGH_SPD (0x00)

    #define LPC_CLOCK_INACTIVE                  (0x01)
    #define LPC_CLOCK_ACTIVE                    (0x00)
    
    #define LPC_ACS_CHK_MSTPCRA                 (0xFFFFFFCF)
    #define LPC_ACS_CHK_MSTPCRB                 (0xFFFFFFFF)
    #define LPC_ACS_CHK_MSTPCRC                 (0xFFFF0000)
    #define LPC_ACS_CHK_MSTPCRD                 (0xFFFFFFFF)
    
    #define LPC_OFS0_REG_VALUE                  (*(volatile uint32_t *)0x00120068)

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global variables
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/
#endif /* R_LPC_RX71M_PRIVATE_H */
