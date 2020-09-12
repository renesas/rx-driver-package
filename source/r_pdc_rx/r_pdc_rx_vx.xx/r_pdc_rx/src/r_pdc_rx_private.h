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
 * File Name    : r_pdc_rx_private.h
 * Version      : 2.05
 * Device       : RX64M/RX71M/RX651/RX65N/RX72M/RX66N/RX72N
 * Tool-Chain   : RX Family C Compiler
 * H/W Platform : 
 * Description  : File that defines macro and structure seen only in "r_pdc_rx.c" file.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.10.2016 2.00     First Release.
 *         : 02.10.2017 2.01     Removed BSP version error.
 *         :                     Change loop values of PDC reset wating process "219" to "10000000".
 *         : 30.07.2019 2.04     Added changes for RX72M.
 *         : 22.11.2019 2.05     Added changes for RX66N and RX72N.
 ***********************************************************************************************************************/
/* Guards against multiple inclusion */
#ifndef PDC_RX_PRIVATE_H
    #define PDC_RX_PRIVATE_H

/* This checks that the module of the PDC is supported to the MCU that has been selected for sure. */
    #if (defined(BSP_MCU_RX64M) || defined(BSP_MCU_RX71M) || defined(BSP_MCU_RX65N) || defined(BSP_MCU_RX72M) || defined(BSP_MCU_RX66N) || defined(BSP_MCU_RX72N))

    #else
        #error "This MCU is not supported by the current r_pdc_rx module."
    #endif

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
    #if ((PDC_CFG_PCKO_DIV == 2) || (PDC_CFG_PCKO_DIV == 4) || (PDC_CFG_PCKO_DIV == 6) || (PDC_CFG_PCKO_DIV == 8) || (PDC_CFG_PCKO_DIV == 10) || (PDC_CFG_PCKO_DIV == 12) || (PDC_CFG_PCKO_DIV == 14) || (PDC_CFG_PCKO_DIV == 16))
    #else
        #error "ERROR- PDC_CFG_PCKO_DIV - PCKO division values is out of range defined in r_pdc_rx_config.h."
    #endif

/* Defines for the PCE bit. (PDC Operation Enable) */
    #define PDC_DISABLE_OPERATION       (0)
    #define PDC_ENABLE_OPERATION        (1)

/* Defines for the PCKE bit. (PIXCLK Input Enable) */
    #define PDC_DISABLE_PIXCLK_INPUT    (0)
    #define PDC_ENABLE_PIXCLK_INPUT     (1)

/* Defines for the PRST bit. (PDC Reset) */
    #define PDC_RESET_RELEASE           (0)
    #define PDC_RESET                   (1)

/* Defines for the PCKOE bit. (PCKO Output Enable) */
    #define PDC_DISABLE_PCKO_OUTPUT     (0)
    #define PDC_ENABLE_PCKO_OUTPUT      (1)

/* Defines for PDC status flag */
    #define PDC_CLEAR_STATUS            (0)
    #define PDC_SET_STATUS              (1)

/* Defines for PDC FIFO status */
    #define PDC_FIFO_NOT_EMPTY          (0)
    #define PDC_FIFO_EMPTY              (1)

/* Defines for value of the VST bit. (Vertical Capture Start Line Position) */
    #define PDC_VST_UPPER_LIMIT         (0x0FFE)

/* Defines for value of the HST bit. (Horizontal Capture Start Byte Position) */
    #define PDC_HST_UPPER_LIMIT         (0x0FFB)

/* Defines for value of the VSZ bit. (Vertical Capture Size) */
    #define PDC_VSZ_LOWER_LIMIT         (0x0001)
    #define PDC_VSZ_UPPER_LIMIT         (0x0FFF)

/* Defines for value of the HSZ bit. (Horizontal Capture Size) */
    #define PDC_HSZ_LOWER_LIMIT         (0x0004)
    #define PDC_HSZ_UPPER_LIMIT         (0x0FFF)

/* Defines for mix value of the VST bit and VSZ bit. */
    #define PDC_VSTVSZ_MIX_UPPER_LIMIT  (0x0FFF)
    #define PDC_HSTHSZ_MIX_UPPER_LIMIT  (0x0FFF)

/* Defines for the EDS bit. (Endian Select) */
    #define PDC_EDS_LITTLE_ENDIAN       (0)
    #define PDC_EDS_BIG_ENDIAN          (1)

/* Defines for value of IPR bit. (Interrupt Priority Level) */
    #define PDC_IPR_LV_UPPER_LIMIT      (0x0F)

/* Defines for loop values of PDC reset waiting process */
    #define PDC_RESET_LOOP_VALUES       (10000000)

/* Defines for loop values of FIFO empty waiting process */
    #define PDC_EMPTY_LOOP_VALUES       (300)
    
/* Defines for loop values of continuous reset limitation waiting process */
    #define PDC_CONT_RESET_LOOP_VALUES  (72)

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/
/* Defines for interrupt setup step */
typedef enum e_pdc_setup_step
{
    PDC_INITIALIZE = 0, /* Procedure 1, which is run by within R_PDC_Open. */
    PDC_CHANGE_SETTING /* Procedure, which is run by within R_PDC_Control. */
} pdc_step_t;

/***********************************************************************************************************************
 Exported global variables
 ***********************************************************************************************************************/

#endif /* PDC_RX_PRIVATE_H */

/* End of File */
