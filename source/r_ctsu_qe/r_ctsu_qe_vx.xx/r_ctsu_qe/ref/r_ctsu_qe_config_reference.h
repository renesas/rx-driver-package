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
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_qectsu_rx_config_reference.h
 * Description  : Configures the CTSU API module.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           04.10.2018 1.00    First Release
***********************************************************************************************************************/
#ifndef CTSU_CONFIG_HEADER_FILE
#define CTSU_CONFIG_HEADER_FILE


/***********************************************************************************************************************
 Configuration Options
 ***********************************************************************************************************************/
/* SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING
 * Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting
 * Setting to 1 includes parameter checking; 0 compiles out parameter checking
 */
#define CTSU_CFG_PARAM_CHECKING_ENABLE      (1)

/* Set value to 1 to use DTC instead of main processor for handling CTSU write
 * and read interrupts.
 * WARNINGS:
 * If DTC is used elsewhere in the application, there may be conflicts
 * with using this driver.
 * If DTC is used, cannot use external trigger for scan trigger type.
 */
#define CTSU_CFG_USE_DTC                    (0)

/* Interrupt priority level */
#define CTSU_CFG_INT_PRIORITY_LEVEL         (8)


#endif /* CTSU_CONFIG_HEADER_FILE */
