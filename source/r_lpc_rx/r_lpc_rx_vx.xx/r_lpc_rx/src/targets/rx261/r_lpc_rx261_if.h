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
 * Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_lpc_rx261_if.h
 * Description  : Definitions used by the r_lpc_rx261.c module.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description           
 *         : 28.06.2024 1.00    First Release
 ***********************************************************************************************************************/

#ifndef R_LPC_RX261_IF_H
    #define R_LPC_RX261_IF_H
/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
/* Valid function */
#define LPC_VALID_SNOOZE_MODE

/* Common definition used in snooze_operation and snooze_release */
#define LPC_SNZ_RESET               (0x0000)
/* Definition used in snooze_operation (SNZCR Register) */
#define LPC_SNZ_SCI5_REQ_EXIT       (0x0002)
#define LPC_SNZ_SCI5_DTC_REQ_EXIT   (0x0003)
#define LPC_SNZ_LPT_REQ             (0x0008)
#define LPC_SNZ_LPT_DTC_REQ_EXIT    (0x000c)
#define LPC_SNZ_S12AD_REQ           (0x0020)
#define LPC_SNZ_S12AD_DTC_REQ_EXIT  (0x0030)
#define LPC_SNZ_CTSU_REQ_EXIT       (0x0080)
#define LPC_SNZ_REMC_REQ_EXIT       (0x0200)
#define LPC_SNZ_S12AD_2SEL_REQ_EXIT (0x0800)
#define LPC_SNZ_S12AD_3SEL_REQ_EXIT (0x2000)
#define LPC_SNZ_DTC_ENABLE          (0x8000)
/* Definition used in snooze_release (SNZCR2 Register) */
#define LPC_SNZ_SCI5_ERROR_RELEASE  (0x0001)
#define LPC_SNZ_SCI5_FULL_RELEASE   (0x0004)
#define LPC_SNZ_SCI5_DTC_RELEASE    (0x0008)
#define LPC_SNZ_LPT_MATCH1_RELEASE  (0x0010)
#define LPC_SNZ_LPT_DTC_RELEASE     (0x0020)
#define LPC_SNZ_S12AD_RELEASE       (0x0040)
#define LPC_SNZ_S12AD_DTC_RELEASE   (0x0080)
#define LPC_SNZ_CTSU_RELEASE        (0x0100)
#define LPC_SNZ_S12AD2_RELEASE      (0x0400)
#define LPC_SNZ_S12AD3_RELEASE      (0x1000)

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
    LPC_OP_MIDDLE_SPEED_2 = 0x04,
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
    LPC_CKSEL_PLL = 0x04,
    LPC_CKSEL_IWDT,                    // cannot be used as a system clock source
    LPC_END_OF_CLOCKLIST
}lpc_clocks_t;

/* System Clocks. Don't change the order of this list. */
typedef enum lpc_system_clocks
{
    LPC_ICLK,
    LPC_FCLK,
    LPC_PCLKA,
    LPC_PCLKB,
    LPC_PCLKD,
}lpc_system_clocks_t;

/* PLL Dividers */
typedef enum lpc_pll_divider
{
    LPC_PLL_DIV_1 = 0x00,
    LPC_PLL_DIV_2 = 0x01,
    LPC_PLL_DIV_4 = 0x02
}lpc_pll_divider_t;

/* Please note that it is different from the callback function set by R_LPC_LowPowerModeActivate */
/**********************************************************************************************************************
 * Function Name: lpc_snooze_callback_set_t
 * Description  : .
 * Argument     : pdata
 * Return Value : .
 *********************************************************************************************************************/
typedef void (*lpc_snooze_callback_set_t) (void *pdata); /*Callback function type of the snooze mode*/

typedef struct st_lpc_snooze_interrupt
{
    uint8_t                   priority;  //Interrupt priority
    lpc_snooze_callback_set_t pcallback; //Setting the callback function of the snooze release interrupt
}lpc_snooze_interrupt_t;

typedef struct st_lpc_snooze_mode
{
    uint16_t               snooze_operation;  //SNZCR Register
    uint16_t               snooze_release;    //SNZCR2 Register
    lpc_snooze_interrupt_t snooze_interrupt;  //Snooze release interrupt
}lpc_snooze_mode_t;

/***********************************************************************************************************************
 Exported global variables
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/
#endif /* R_LPC_RX261_IF_H */
