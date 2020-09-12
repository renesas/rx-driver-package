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
* Copyright (C) 2013-2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_irq_rx_config.h
* Device(s)    : Renesas RX Family
* Tool-Chain   : Renesas RX Standard Toolchain
* H/W Platform :
* Description  : User configurable options and setup definitions for the RX IRQ support module.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY  Version Description
*         : 15.07.2013  1.00    First Release
*         : 04.04.2014  1.20    Using the definition of VECT_ICU_IRQ15 to check the number of IRQs.
*         : 30.09.2015  1.70    Using the definition of VECT_ICU_IRQ7  to check the number of IRQs.
*         : 28.09.2018  2.30    Updated the coding rules of Renesas.
***********************************************************************************************************************/
#ifndef IRQ_CONFIG_H
#define IRQ_CONFIG_H
/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"
#include "r_irq_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/* Set this value to 1 to enable locking functions for the IRQ APIs, otherwise set to 0 to ignore locking.
 * If locking is enabled, the application must obtain a lock on the selected IRQ before making calls to the API
 * for that IRQ. If locking is disabled, any caller has unrestricted access to any IRQ function. Disabling is
 * provided for systems that require faster and smaller code, however care must be taken to prevent reentrancy or
 * conflict over resource usage.
 */
#define IRQ_CFG_REQUIRE_LOCK    (1)

/* Checking of arguments passed to IRQ API functions can be enable or disabled.
 * Disabling argument checking is provided for systems that absolutely require faster and smaller code.
 * By default the module is configured to use the setting of the system-wide BSP_CFG_PARAM_CHECKING_ENABLE macro.
 * This can be overridden for the local module by redefining IRQ_CFG_PARAM_CHECKING.
 * To control parameter checking locally, set IRQ_CFG_PARAM_CHECKING to 1 to enable it, otherwise set to 0 skip checking.
 *
 * Use this option with caution.
 */

#define IRQ_CFG_PARAM_CHECKING  (BSP_CFG_PARAM_CHECKING_ENABLE)


/* *********************************************************************************************************************
 * IRQ enabling and port assignments
 *
 * If you are using Smart Configurator for pin assignment, use the following:
 *   #include "r_irq_rx_pinset.h"
 *   #ifndef R_IRQ_RX_PINSET_H
 *   #error "Please check IRQ port assignments in Smart Configurator"
 *   #endif
 * and comment out the following:
 *   #define IRQ_PORT_IRQ0_PORT     ('m') 
 *   #define IRQ_PORT_IRQ0_BIT      ('n')
 *          ...
 *   #define IRQ_PORT_IRQ15_PORT     ('m') 
 *   #define IRQ_PORT_IRQ15_BIT      ('n')
 * --------------------------------------------------------------------------------------------
 * Conversely, if you are using FIT Configurator for pin assignment, comment out the following:
 *   #include "r_irq_rx_pinset.h"
 *   #ifndef R_IRQ_RX_PINSET_H
 *   #error "Please check IRQ port assignments in Smart Configurator"
 *   #endif
 * and use the following:
 *   #define IRQ_PORT_IRQ0_PORT     ('m') 
 *   #define IRQ_PORT_IRQ0_BIT      ('n')
 *          ...
 *   #define IRQ_PORT_IRQ15_PORT     ('m') 
 *   #define IRQ_PORT_IRQ15_BIT      ('n')
 * Use these defines to select IRQs that are to be used in your system.
 * This will eliminate the generation of code for unused IRQs.
 *
 * IRQs intended for use must have an I/O port and bit assigned. Use these definitions to make the assignments.
 * These are decoded by the API to map to the port input data registers.
 * Set these as required according to the following format:
 * #define IRQ_PORT_IRQ0_PORT     ('m') where m is the port number and the IRQ number replaces *
 * ##define IRQ_PORT_IRQ0_BIT     ('n') where n is the bit number and the IRQ number replaces *
 * Port assignments here must match the port configuration settings performed externally for them.
***********************************************************************************************************************/
#include "r_irq_rx_pinset.h"
#ifndef R_IRQ_RX_H
#error "Please add IRQ Pin setting in Smart Configurator, or if you are using FIT Configurator remove the comment of following macros"
#endif
/*
#define IRQ_PORT_IRQ0_PORT      ('m')
#define IRQ_PORT_IRQ0_BIT       ('n')

#define IRQ_PORT_IRQ1_PORT      ('m')
#define IRQ_PORT_IRQ1_BIT       ('n')

#define IRQ_PORT_IRQ2_PORT      ('m')
#define IRQ_PORT_IRQ2_BIT       ('n')

#define IRQ_PORT_IRQ3_PORT      ('m')
#define IRQ_PORT_IRQ3_BIT       ('n')

#define IRQ_PORT_IRQ4_PORT      ('m')
#define IRQ_PORT_IRQ4_BIT       ('n')

#define IRQ_PORT_IRQ5_PORT      ('m')
#define IRQ_PORT_IRQ5_BIT       ('n')

#if defined(VECT_ICU_IRQ7)               // For MCUs with 8 IRQs
#define IRQ_PORT_IRQ6_PORT      ('m')
#define IRQ_PORT_IRQ6_BIT       ('n')

#define IRQ_PORT_IRQ7_PORT      ('m')
#define IRQ_PORT_IRQ7_BIT       ('n')
#endif // #if defined(VECT_ICU_IRQ7)

#if defined(VECT_ICU_IRQ15)              // For MCUs with 16 IRQs
#define IRQ_PORT_IRQ8_PORT      ('m')
#define IRQ_PORT_IRQ8_BIT       ('n')

#define IRQ_PORT_IRQ9_PORT      ('m')
#define IRQ_PORT_IRQ9_BIT       ('n')

#define IRQ_PORT_IRQ10_PORT     ('m')
#define IRQ_PORT_IRQ10_BIT      ('n')

#define IRQ_PORT_IRQ11_PORT     ('m')
#define IRQ_PORT_IRQ11_BIT      ('n')

#define IRQ_PORT_IRQ12_PORT     ('m')
#define IRQ_PORT_IRQ12_BIT      ('n')

#define IRQ_PORT_IRQ13_PORT     ('m')
#define IRQ_PORT_IRQ13_BIT      ('n')

#define IRQ_PORT_IRQ14_PORT     ('m')
#define IRQ_PORT_IRQ14_BIT      ('n')

#define IRQ_PORT_IRQ15_PORT     ('m')
#define IRQ_PORT_IRQ15_BIT      ('n')
#endif // #if defined(VECT_ICU_IRQ15)
*/

/***********************************************************************************************************************
Digital Filtering
***********************************************************************************************************************/
/* To enable digital noise filtering with the selected IRQ.
 * Set the value to 1 to enable the filter or 0 to disable it. */
#define IRQ_CFG_FILT_EN_IRQ0       (0)   /* Filtering disabled on IRQ 0. */
#define IRQ_CFG_FILT_EN_IRQ1       (0)   /* Filtering disabled on IRQ 1. */
#define IRQ_CFG_FILT_EN_IRQ2       (0)   /* Filtering disabled on IRQ 2. */
#define IRQ_CFG_FILT_EN_IRQ3       (0)   /* Filtering disabled on IRQ 3. */
#define IRQ_CFG_FILT_EN_IRQ4       (0)   /* Filtering disabled on IRQ 4. */
#define IRQ_CFG_FILT_EN_IRQ5       (0)   /* Filtering disabled on IRQ 5. */
#define IRQ_CFG_FILT_EN_IRQ6       (0)   /* Filtering disabled on IRQ 6. */
#define IRQ_CFG_FILT_EN_IRQ7       (0)   /* Filtering disabled on IRQ 7. */
#define IRQ_CFG_FILT_EN_IRQ8       (0)   /* Filtering disabled on IRQ 8. */
#define IRQ_CFG_FILT_EN_IRQ9       (0)   /* Filtering disabled on IRQ 9. */
#define IRQ_CFG_FILT_EN_IRQ10      (0)   /* Filtering disabled on IRQ 10. */
#define IRQ_CFG_FILT_EN_IRQ11      (0)   /* Filtering disabled on IRQ 11. */
#define IRQ_CFG_FILT_EN_IRQ12      (0)   /* Filtering disabled on IRQ 12. */
#define IRQ_CFG_FILT_EN_IRQ13      (0)   /* Filtering disabled on IRQ 13. */
#define IRQ_CFG_FILT_EN_IRQ14      (0)   /* Filtering disabled on IRQ 14. */
#define IRQ_CFG_FILT_EN_IRQ15      (0)   /* Filtering disabled on IRQ 15. */

/* Example settings for IRQ input pin digital filtering sample clock divisors for each IRQ.
 * Change as required by selecting a divisor value from the IRQ_CFG_PCLK_DIVxx definitions.
 * Filtering must be enabled for the corresponding IRQ for these settings to be effective.  */
#define IRQ_CFG_FILT_PLCK_IRQ0    (IRQ_CFG_PCLK_DIV64)  /* Filter sample clock divisor for IRQ 0 = PCLK/64. */
#define IRQ_CFG_FILT_PLCK_IRQ1    (IRQ_CFG_PCLK_DIV64)  /* Filter sample clock divisor for IRQ 1 = PCLK/64. */
#define IRQ_CFG_FILT_PLCK_IRQ2    (IRQ_CFG_PCLK_DIV64)  /* Filter sample clock divisor for IRQ 2 = PCLK/64. */
#define IRQ_CFG_FILT_PLCK_IRQ3    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 3 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ4    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 4 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ5    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 5 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ6    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 6 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ7    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 7 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ8    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 8 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ9    (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 9 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ10   (IRQ_CFG_PCLK_DIV64)  /* Filter sample clock divisor for IRQ 10 = PCLK/64. */
#define IRQ_CFG_FILT_PLCK_IRQ11   (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 11 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ12   (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 12 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ13   (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 13 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ14   (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 14 = PCLK. */
#define IRQ_CFG_FILT_PLCK_IRQ15   (IRQ_CFG_PCLK_DIV1)   /* Filter sample clock divisor for IRQ 15 = PCLK. */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

      
#endif /* IRQ_CONFIG_H */
