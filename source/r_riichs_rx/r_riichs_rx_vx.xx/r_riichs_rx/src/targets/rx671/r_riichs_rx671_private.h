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
 * File Name    : r_riichs_rx671_private.h
 * Description  : Functions for using RIICHS on RX devices.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 30.06.2021 1.00     First Release
 **********************************************************************************************************************/
/* Guards against multiple inclusion */
#ifndef RIICHS_RX671_PRIVATE_H
    #define RIICHS_RX671_PRIVATE_H
/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/

/*----------------------------------------------------------------------------*/
/*   Define channel No.(max) + 1                                              */
/*----------------------------------------------------------------------------*/
    #define RIICHS_MAX_CH_NUM           (1U)                /* Set number of RIICHS channel */

/*----------------------------------------------------------------------------*/
/*   Define technical update[No.TN-RX*-A012A] process                         */
/*----------------------------------------------------------------------------*/
/* Unnecessary technical update [No.TN-RX*-A012A] process */

/*============================================================================*/
/*  Parameter check of Configuration Options                                  */
/*============================================================================*/

/*  RIICHS0 Parameter check of Configuration Options */
    #if ((0U != RIICHS_CFG_CH0_INCLUDED) && (1U != RIICHS_CFG_CH0_INCLUDED))
        #error "ERROR - RIICHS_CFG_CH0_INCLUDED - Parameter error in configures file."
    #endif

/*============================================================================*/
/*   PORT/ICU Channel setting.                                                */
/*============================================================================*/
/*----------------------------------------------------------------------------*/
/*   PORT/ICU register setting for RIICHS Channel 0                           */
/*----------------------------------------------------------------------------*/
/* EEI0/TEI0/EEI1/TEI1/EEI2/TEI2 Interrupt request register */
    #define RIICHS_IR_EEI_TEI        (ICU.IR[IR_ICU_GROUPAL1].BIT.IR)

/*----------------------------------------------------------------------------*/
/*   PORT/ICU register setting for RIICHS Channel 0.                            */
/*----------------------------------------------------------------------------*/
/* Sets interrupt source priority initialization. */
    #define RIICHS_IPR_CH0_RXI_INIT  ((uint8_t)(0x00)) /* RXI0 interrupt source priority initialization */
    #define RIICHS_IPR_CH0_TXI_INIT  ((uint8_t)(0x00)) /* TXI0 interrupt source priority initialization */
/* EEI0 interrupt source priority initialization*/
    #define RIICHS_IPR_CH0_EEI_SET   ((uint8_t)(RIICHS_CFG_CH0_EEI_INT_PRIORITY))
/* RXI0 interrupt source priority initialization*/
    #define RIICHS_IPR_CH0_RXI_SET   ((uint8_t)(RIICHS_CFG_CH0_RXI_INT_PRIORITY))
/* TXI0 interrupt source priority initialization*/
    #define RIICHS_IPR_CH0_TXI_SET   ((uint8_t)(RIICHS_CFG_CH0_TXI_INT_PRIORITY))
/* TEI0 interrupt source priority initialization*/
    #define RIICHS_IPR_CH0_TEI_SET   ((uint8_t)(RIICHS_CFG_CH0_TEI_INT_PRIORITY))

/* Sets to use multi-function pin controller. */
    #define RIICHS_MPC_SDA0_INIT     ((uint8_t)(0x00))                 /* Pin function select to Hi-Z */
    #define RIICHS_MPC_SCL0_INIT     ((uint8_t)(0x00))                 /* Pin function select to Hi-Z */
    #define RIICHS_MPC_SDA0_ENABLE   ((uint8_t)(0x2F))                 /* Pin function select to RIICHS0 SDA pin */
    #define RIICHS_MPC_SCL0_ENABLE   ((uint8_t)(0x2F))                 /* Pin function select to RIICHS0 SCL pin */

/* Define interrupt request registers */
    #define RIICHS_IR_RXI0         (ICU.IR[IR_RIICHS0_RXI].BIT.IR)      /* RXI0 Interrupt request register */
    #define RIICHS_IR_TXI0         (ICU.IR[IR_RIICHS0_TXI].BIT.IR)      /* TXI0 Interrupt request register */

/* Define interrupt request enable registers */
    #define RIICHS_IER_RXI0        (ICU.IER[IER_RIICHS0_RXI].BIT.IEN6)  /* RXI0 Interrupt request enable register */
    #define RIICHS_IER_TXI0        (ICU.IER[IER_RIICHS0_TXI].BIT.IEN7)  /* TXI0 Interrupt request enable register */

/* Define GroupAL1 interrupt request enable registers */
    #define RIICHS_IER_EEI0_GPAL1  (ICU.GENAL1.BIT.EN13)              /* EEI0 group Interrupt request enable register */
    #define RIICHS_IER_TEI0_GPAL1  (ICU.GENAL1.BIT.EN12)              /* TEI0 group Interrupt request enable register */

/* Define interrupt source priority registers */
    #define RIICHS_IPR_RXI0  (ICU.IPR[IPR_RIICHS0_RXI].BYTE)            /* RXI0 Interrupt source priority register */
    #define RIICHS_IPR_TXI0  (ICU.IPR[IPR_RIICHS0_TXI].BYTE)            /* TXI0 Interrupt source priority register */

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/
/*----------------------------------------------------------------------------*/
/*   interrupt function                                                       */
/*----------------------------------------------------------------------------*/
    #if (1U == RIICHS_CFG_CH0_INCLUDED)
/* ---- RIICHS0 EEI ---- */
void riichs0_eei_isr(void*);

/* ---- RIICHS0 TEI ---- */
void riichs0_tei_isr(void*);
    #endif

#endif /* RIICHS_RX671_PRIVATE_H */

