/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
 * No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
 * applicable laws, including copyright laws. 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO 
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the 
 * following link:
 * http://www.renesas.com/disclaimer 
 *
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_riic_rx_pin_config_reference.h
 * Description  : Pin configures the RIICHS drivers
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 30.06.2021 1.00     First Release
 **********************************************************************************************************************/
/* Guards against multiple inclusion */
#ifndef RIICHS_PIN_CONFIG_H
    #define RIICHS_PIN_CONFIG_H
/***********************************************************************************************************************
 Configuration Options
 **********************************************************************************************************************/
/*------------------------------------------------------------------------------*/
/*   Set using port as riic port                                                */
/*------------------------------------------------------------------------------*/
/* Set using port as riic port.                                                                 */
/* If you want to include the port configuration process(RIICHS_CFG_PORT_SET_PROCESSING is "1"),  */
/* please choose which ports to use for the SCL/SDA of RIICHS with the following setting.         */
/* Select the port group and pin used by setting
 "R_RIICHS_CFG_RIICHSx_SCLx_PORT (select from port group 0 to K)"
 and "R_RIICHS_CFG_RIICHSx_SCLx_BIT (select from pin number 0 to 7)"
 and "R_RIICHS_CFG_RIICHSx_SDAx_PORT (select from port group 0 to K)"
 and "R_RIICHS_CFG_RIICHSx_SDAx_BIT (select from pin number 0 to 7)",
 respectively. */

/* Select the ports (SCL0 and SDA0) to use in RIICHS0 */
    #define R_RIICHS_CFG_RIICHS0_SCL0_PORT   '1'     /* Port Number */
    #define R_RIICHS_CFG_RIICHS0_SCL0_BIT    '2'     /* Bit Number  */

    #define R_RIICHS_CFG_RIICHS0_SDA0_PORT   '1'     /* Port Number */
    #define R_RIICHS_CFG_RIICHS0_SDA0_BIT    '3'     /* Bit Number  */

#endif /* RIICHS_PIN_CONFIG_H */

