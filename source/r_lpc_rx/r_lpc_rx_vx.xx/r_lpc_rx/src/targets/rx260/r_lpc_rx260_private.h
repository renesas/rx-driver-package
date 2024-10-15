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
 * File Name    : r_lpc_rx260_private.h
 * Description  : Definitions used by the r_lpc_rx260.c module.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description           
 *         : 28.06.2024 1.00    First Release
 ***********************************************************************************************************************/

#ifndef R_LPC_RX260_PRIVATE_H
    #define R_LPC_RX260_PRIVATE_H

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
/* Internally used definitions */
    #define LPC_TRANSITION_ONGOING    (1)
    #define LPC_TRANSITION_COMPLETE   (0)

    #define LPC_OSC_STOP_ENABLED      (1)

    #define LPC_SUB_CLOCK             (0x0300)

    #define LPC_CLOCK_SWITCH_ENABLE   (0x01)
    #define LPC_CLOCK_SWITCH_DISABLE  (0x00)

    #define LPC_CLOCK_INACTIVE        (0x01)
    #define LPC_CLOCK_ACTIVE          (0x00)


    #if (BSP_CFG_MCU_VCC_MV > 2399)
        #define LPC_MAX_ICLK_HIGH_SPEED_FREQ_HZ               (64000000)
        #define LPC_MAX_FCLK_HIGH_SPEED_FREQ_HZ               (64000000)
        #define LPC_MAX_PCLKA_HIGH_SPEED_FREQ_HZ              (64000000)
        #define LPC_MAX_PCLKB_HIGH_SPEED_FREQ_HZ              (32000000)
        #define LPC_MAX_PCLKD_HIGH_SPEED_FREQ_HZ              (64000000)

        #define LPC_MAX_ICLK_MID_SPEED_FREQ_HZ                (24000000)
        #define LPC_MAX_FCLK_MID_SPEED_FREQ_HZ                (24000000)
        #define LPC_MAX_PCLKA_MID_SPEED_FREQ_HZ               (24000000)
        #define LPC_MAX_PCLKB_MID_SPEED_FREQ_HZ               (24000000)
        #define LPC_MAX_PCLKD_MID_SPEED_FREQ_HZ               (24000000)

        #define LPC_MAX_ICLK_MID_SPEED2_FREQ_HZ               (1000000)
        #define LPC_MAX_FCLK_MID_SPEED2_FREQ_HZ               (1000000)
        #define LPC_MAX_PCLKA_MID_SPEED2_FREQ_HZ              (1000000)
        #define LPC_MAX_PCLKB_MID_SPEED2_FREQ_HZ              (1000000)
        #define LPC_MAX_PCLKD_MID_SPEED2_FREQ_HZ              (1000000)

        #define LPC_MAX_ICLK_LOW_SPEED_FREQ_HZ                (32768)
        #define LPC_MAX_FCLK_LOW_SPEED_FREQ_HZ                (32768)
        #define LPC_MAX_PCLKA_LOW_SPEED_FREQ_HZ               (32768)
        #define LPC_MAX_PCLKB_LOW_SPEED_FREQ_HZ               (32768)
        #define LPC_MAX_PCLKD_LOW_SPEED_FREQ_HZ               (32768)

    #elif  (BSP_CFG_MCU_VCC_MV > 1799)
        #define LPC_MAX_ICLK_HIGH_SPEED_FREQ_HZ               (48000000)
        #define LPC_MAX_FCLK_HIGH_SPEED_FREQ_HZ               (48000000)
        #define LPC_MAX_PCLKA_HIGH_SPEED_FREQ_HZ              (48000000)
        #define LPC_MAX_PCLKB_HIGH_SPEED_FREQ_HZ              (32000000)
        #define LPC_MAX_PCLKD_HIGH_SPEED_FREQ_HZ              (48000000)
        #define LPC_MAX_RSIP_ENABLE_HIGH_SPEED_FREQ_HZ        (16000000)

        #define LPC_MAX_ICLK_MID_SPEED_FREQ_HZ                (24000000)
        #define LPC_MAX_FCLK_MID_SPEED_FREQ_HZ                (24000000)
        #define LPC_MAX_PCLKA_MID_SPEED_FREQ_HZ               (24000000)
        #define LPC_MAX_PCLKB_MID_SPEED_FREQ_HZ               (24000000)
        #define LPC_MAX_PCLKD_MID_SPEED_FREQ_HZ               (24000000)
        #define LPC_MAX_RSIP_ENABLE_MID_SPEED_FREQ_HZ         (16000000)

        #define LPC_MAX_ICLK_MID_SPEED2_FREQ_HZ               (1000000)
        #define LPC_MAX_FCLK_MID_SPEED2_FREQ_HZ               (1000000)
        #define LPC_MAX_PCLKA_MID_SPEED2_FREQ_HZ              (1000000)
        #define LPC_MAX_PCLKB_MID_SPEED2_FREQ_HZ              (1000000)
        #define LPC_MAX_PCLKD_MID_SPEED2_FREQ_HZ              (1000000)

        #define LPC_MAX_ICLK_LOW_SPEED_FREQ_HZ                (32768)
        #define LPC_MAX_FCLK_LOW_SPEED_FREQ_HZ                (32768)
        #define LPC_MAX_PCLKA_LOW_SPEED_FREQ_HZ               (32768)
        #define LPC_MAX_PCLKB_LOW_SPEED_FREQ_HZ               (32768)
        #define LPC_MAX_PCLKD_LOW_SPEED_FREQ_HZ               (32768)

    #elif  (BSP_CFG_MCU_VCC_MV > 1599)
        #define LPC_MAX_ICLK_MID_SPEED_FREQ_HZ                (4000000)
        #define LPC_MAX_FCLK_MID_SPEED_FREQ_HZ                (4000000)
        #define LPC_MAX_PCLKA_MID_SPEED_FREQ_HZ               (4000000)
        #define LPC_MAX_PCLKB_MID_SPEED_FREQ_HZ               (4000000)
        #define LPC_MAX_PCLKD_MID_SPEED_FREQ_HZ               (4000000)

        #define LPC_MAX_ICLK_MID_SPEED2_FREQ_HZ               (1000000)
        #define LPC_MAX_FCLK_MID_SPEED2_FREQ_HZ               (1000000)
        #define LPC_MAX_PCLKA_MID_SPEED2_FREQ_HZ              (1000000)
        #define LPC_MAX_PCLKB_MID_SPEED2_FREQ_HZ              (1000000)
        #define LPC_MAX_PCLKD_MID_SPEED2_FREQ_HZ              (1000000)

        #define LPC_MAX_ICLK_LOW_SPEED_FREQ_HZ                (32768)
        #define LPC_MAX_FCLK_LOW_SPEED_FREQ_HZ                (32768)
        #define LPC_MAX_PCLKA_LOW_SPEED_FREQ_HZ               (32768)
        #define LPC_MAX_PCLKB_LOW_SPEED_FREQ_HZ               (32768)
        #define LPC_MAX_PCLKD_LOW_SPEED_FREQ_HZ               (32768)
    #endif

    #define LPC_MEDIUM_SPD                                    (0x02)
    #define LPC_MEDIUM_SPD_2                                  (0x04)
    #define LPC_HIGH_SPD                                      (0x00)

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global variables
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/
#endif /* R_LPC_RX260_PRIVATE_H */
