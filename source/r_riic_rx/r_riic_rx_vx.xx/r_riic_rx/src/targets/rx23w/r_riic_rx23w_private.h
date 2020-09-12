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
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_riic_rx23w_private.h
 * Description  : Functions for using RIIC on RX devices. 
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 20.06.2019 2.42     First Release
 **********************************************************************************************************************/
/* Guards against multiple inclusion */
#ifndef RIIC_RX23W_PRIVATE_H
    #define RIIC_RX23W_PRIVATE_H
/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/

/*----------------------------------------------------------------------------*/
/*   Define channel No.(max) + 1                                              */
/*----------------------------------------------------------------------------*/
    #define MAX_RIIC_CH_NUM         (1U)                  /* Set number of RIIC channel */

/*----------------------------------------------------------------------------*/
/*   Define technical update[No.TN-RX*-A012A] process                         */
/*----------------------------------------------------------------------------*/
/* Unnecessary technical update [No.TN-RX*-A012A] process */

/*============================================================================*/
/*  Parameter check of Configuration Options                                  */
/*============================================================================*/
    #if (1U == RIIC_CFG_CH1_INCLUDED)
        #error "ERROR - RIIC_CFG_CH1_INCLUDED - RIIC1 is not supported in this device. "
    #endif
    #if (1U == RIIC_CFG_CH2_INCLUDED)
        #error "ERROR - RIIC_CFG_CH2_INCLUDED - RIIC2 is not supported in this device. "
    #endif

    #ifdef TN_RXA012A
        #error "ERROR - TN_RXA012A - Parameter error in configures file.  "
    #endif  /* TN_RXA012A */

    #if ((0U != RIIC_CFG_CH0_INCLUDED) && (1U != RIIC_CFG_CH0_INCLUDED))
        #error "ERROR - RIIC_CFG_CH0_INCLUDED - Parameter error in configures file.  "
    #endif

    #if ((0U >= RIIC_CFG_CH0_kBPS) || (400U < RIIC_CFG_CH0_kBPS))
        #error "ERROR - RIIC_CFG_CH0_kBPS - Parameter error in configures file.  "
    #endif

    #if ((0U > RIIC_CFG_CH0_DIGITAL_FILTER) || (4U < RIIC_CFG_CH0_DIGITAL_FILTER))
        #error "ERROR - RIIC_CFG_CH0_DIGITAL_FILTER - Parameter error in configures file.  "
    #endif

    #if ((0U != RIIC_CFG_CH0_MASTER_MODE) && (1U != RIIC_CFG_CH0_MASTER_MODE))
        #error "ERROR - RIIC_CFG_CH0_MASTER_MODE - Parameter error in configures file.  "
    #endif

    #if (0U == RIIC_CFG_CH0_SLV_ADDR0_FORMAT)
/* Do Nothing */
    #elif (1U == RIIC_CFG_CH0_SLV_ADDR0_FORMAT)
        #if ((0x0000 > RIIC_CFG_CH0_SLV_ADDR0) || (0x007F < RIIC_CFG_CH0_SLV_ADDR0))
            #error "ERROR - RIIC_CFG_CH0_SLV_ADDR0 - Parameter error in configures file.  "
        #endif
    #elif (2U == RIIC_CFG_CH0_SLV_ADDR0_FORMAT)
        #if ((0x0000 > RIIC_CFG_CH0_SLV_ADDR0) || (0x03FF < RIIC_CFG_CH0_SLV_ADDR0))
            #error "ERROR - RIIC_CFG_CH0_SLV_ADDR0 - Parameter error in configures file.  "
        #endif
    #else
        #error "ERROR - RIIC_CFG_CH0_SLV_ADDR0_FORMAT - Parameter error in configures file.  "
    #endif

    #if (0U == RIIC_CFG_CH0_SLV_ADDR1_FORMAT)
/* Do Nothing */
    #elif (1U == RIIC_CFG_CH0_SLV_ADDR1_FORMAT)
        #if ((0x0000 > RIIC_CFG_CH0_SLV_ADDR1) || (0x007F < RIIC_CFG_CH0_SLV_ADDR1))
            #error "ERROR - RIIC_CFG_CH0_SLV_ADDR1 - Parameter error in configures file.  "
        #endif
    #elif (2U == RIIC_CFG_CH0_SLV_ADDR1_FORMAT)
        #if ((0x0000 > RIIC_CFG_CH0_SLV_ADDR1) || (0x03FF < RIIC_CFG_CH0_SLV_ADDR1))
            #error "ERROR - RIIC_CFG_CH0_SLV_ADDR1 - Parameter error in configures file.  "
        #endif
    #else
        #error "ERROR - RIIC_CFG_CH0_SLV_ADDR1_FORMAT - Parameter error in configures file.  "
    #endif

    #if (0U == RIIC_CFG_CH0_SLV_ADDR2_FORMAT)
/* Do Nothing */
    #elif (1U == RIIC_CFG_CH0_SLV_ADDR2_FORMAT)
        #if ((0x0000 > RIIC_CFG_CH0_SLV_ADDR2) || (0x007F < RIIC_CFG_CH0_SLV_ADDR2))
            #error "ERROR - RIIC_CFG_CH0_SLV_ADDR2 - Parameter error in configures file.  "
        #endif
    #elif (2U == RIIC_CFG_CH0_SLV_ADDR2_FORMAT)
        #if ((0x0000 > RIIC_CFG_CH0_SLV_ADDR2) || (0x03FF < RIIC_CFG_CH0_SLV_ADDR2))
            #error "ERROR - RIIC_CFG_CH0_SLV_ADDR2 - Parameter error in configures file.  "
        #endif
    #else
        #error "ERROR - RIIC_CFG_CH0_SLV_ADDR2_FORMAT - Parameter error in configures file.  "
    #endif

    #if ((0U != RIIC_CFG_CH0_SLV_GCA_ENABLE) && (1U != RIIC_CFG_CH0_SLV_GCA_ENABLE))
        #error "ERROR - RIIC_CFG_CH0_SLV_GCA_ENABLE - Parameter error in configures file.  "
    #endif

    #if ((1U > RIIC_CFG_CH0_RXI_INT_PRIORITY) || (15U < RIIC_CFG_CH0_RXI_INT_PRIORITY))
        #error "ERROR - RIIC_CFG_CH0_RXI_INT_PRIORITY - Parameter error in configures file.  "
    #endif

    #if ((1U > RIIC_CFG_CH0_TXI_INT_PRIORITY) || (15U < RIIC_CFG_CH0_TXI_INT_PRIORITY))
        #error "ERROR - RIIC_CFG_CH0_TXI_INT_PRIORITY - Parameter error in configures file.  "
    #endif

    #if ((1U > RIIC_CFG_CH0_TEI_INT_PRIORITY) || (15U < RIIC_CFG_CH0_TEI_INT_PRIORITY) || \
     (RIIC_CFG_CH0_RXI_INT_PRIORITY > RIIC_CFG_CH0_TEI_INT_PRIORITY) || \
     (RIIC_CFG_CH0_TXI_INT_PRIORITY > RIIC_CFG_CH0_TEI_INT_PRIORITY))
        #error "ERROR - RIIC_CFG_CH0_TEI_INT_PRIORITY - Parameter error in configures file.  "
    #endif

    #if ((1U > RIIC_CFG_CH0_EEI_INT_PRIORITY) || (15U < RIIC_CFG_CH0_EEI_INT_PRIORITY) || \
     (RIIC_CFG_CH0_RXI_INT_PRIORITY > RIIC_CFG_CH0_EEI_INT_PRIORITY) || \
     (RIIC_CFG_CH0_TXI_INT_PRIORITY > RIIC_CFG_CH0_EEI_INT_PRIORITY))
        #error "ERROR - RIIC_CFG_CH0_EEI_INT_PRIORITY - Parameter error in configures file.  "
    #endif

    #if ((0U != RIIC_CFG_CH0_TMO_ENABLE) && (1U != RIIC_CFG_CH0_TMO_ENABLE))
        #error "ERROR - RIIC_CFG_CH0_TMO_ENABLE - Parameter error in configures file.  "
    #endif

    #if ((0U != RIIC_CFG_CH0_TMO_DET_TIME) && (1U != RIIC_CFG_CH0_TMO_DET_TIME))
        #error "ERROR - RIIC_CFG_CH0_TMO_DET_TIME - Parameter error in configures file.  "
    #endif

    #if ((0U != RIIC_CFG_CH0_TMO_LCNT) && (1U != RIIC_CFG_CH0_TMO_LCNT))
        #error "ERROR - RIIC_CFG_CH0_TMO_LCNT - Parameter error in configures file.  "
    #endif

    #if ((0U != RIIC_CFG_CH0_TMO_HCNT) && (1U != RIIC_CFG_CH0_TMO_HCNT))
        #error "ERROR - RIIC_CFG_CH0_TMO_HCNT - Parameter error in configures file.  "
    #endif

/*============================================================================*/
/*   Define Channel setting.                                                  */
/*============================================================================*/
/*----------------------------------------------------------------------------*/
/*   PORT/ICU register setting for RIIC Channel 0.                            */
/*----------------------------------------------------------------------------*/
/* Sets interrupt source priority initialization. */
    #define RIIC_IPR_CH0_EEI_INIT  ((uint8_t)(0x00))            /* EEI0 interrupt source priority initialization */
    #define RIIC_IPR_CH0_RXI_INIT  ((uint8_t)(0x00))            /* RXI0 interrupt source priority initialization */
    #define RIIC_IPR_CH0_TXI_INIT  ((uint8_t)(0x00))            /* TXI0 interrupt source priority initialization */
    #define RIIC_IPR_CH0_TEI_INIT  ((uint8_t)(0x00))            /* TEI0 interrupt source priority initialization */

/* EEI0 interrupt source priority initialization*/
    #define RIIC_IPR_CH0_EEI_SET   ((uint8_t)(RIIC_CFG_CH0_EEI_INT_PRIORITY))
/* RXI0 interrupt source priority initialization*/
    #define RIIC_IPR_CH0_RXI_SET   ((uint8_t)(RIIC_CFG_CH0_RXI_INT_PRIORITY))
/* TXI0 interrupt source priority initialization*/
    #define RIIC_IPR_CH0_TXI_SET   ((uint8_t)(RIIC_CFG_CH0_TXI_INT_PRIORITY))
/* TEI0 interrupt source priority initialization*/
    #define RIIC_IPR_CH0_TEI_SET   ((uint8_t)(RIIC_CFG_CH0_TEI_INT_PRIORITY))

/* Sets to use multi-function pin controller. */
    #define RIIC_MPC_SDA0_INIT     ((uint8_t)(0x00))           /* Pin function select to Hi-Z */
    #define RIIC_MPC_SCL0_INIT     ((uint8_t)(0x00))           /* Pin function select to Hi-Z */
    #define RIIC_MPC_SDA0_ENABLE   ((uint8_t)(0x0F))           /* Pin function select to RIIC0 SDA pin */
    #define RIIC_MPC_SCL0_ENABLE   ((uint8_t)(0x0F))           /* Pin function select to RIIC0 SCL pin */

/* Define interrupt request registers */
    #define RIIC_IR_RXI0   (ICU.IR[IR_RIIC0_RXI0].BIT.IR)      /* RXI0 Interrupt request register */
    #define RIIC_IR_TXI0   (ICU.IR[IR_RIIC0_TXI0].BIT.IR)      /* TXI0 Interrupt request register */

/* Define interrupt request enable registers */
    #define RIIC_IER_EEI0  (ICU.IER[IER_RIIC0_EEI0].BIT.IEN6)  /* EEI0 Interrupt request enable register */
    #define RIIC_IER_RXI0  (ICU.IER[IER_RIIC0_RXI0].BIT.IEN7)  /* RXI0 Interrupt request enable register */
    #define RIIC_IER_TXI0  (ICU.IER[IER_RIIC0_TXI0].BIT.IEN0)  /* TXI0 Interrupt request enable register */
    #define RIIC_IER_TEI0  (ICU.IER[IER_RIIC0_TEI0].BIT.IEN1)  /* TEI0 Interrupt request enable register */

/* Define interrupt source priority registers */
    #define RIIC_IPR_EEI0  (ICU.IPR[IPR_RIIC0_EEI0].BYTE)      /* EEI0 Interrupt source priority register */
    #define RIIC_IPR_RXI0  (ICU.IPR[IPR_RIIC0_RXI0].BYTE)      /* RXI0 Interrupt source priority register */
    #define RIIC_IPR_TXI0  (ICU.IPR[IPR_RIIC0_TXI0].BYTE)      /* TXI0 Interrupt source priority register */
    #define RIIC_IPR_TEI0  (ICU.IPR[IPR_RIIC0_TEI0].BYTE)      /* TEI0 Interrupt source priority register */

/*============================================================================*/
/*   Define for register setting.                                             */
/*============================================================================*/
/*----------------------------------------------------------------------------*/
/*   RIIC Channel 0 register setting.                                         */
/*----------------------------------------------------------------------------*/
    #if (0U == RIIC_CFG_CH0_TMO_ENABLE)
        #define RIIC_CH0_TMOE           ((uint8_t)(0x00))       /* Mask for ICFER.TMOE bit. */
    #elif (1U == RIIC_CFG_CH0_TMO_ENABLE)
        #define RIIC_CH0_TMOE           ((uint8_t)(0x01))       /* Mask for ICFER.TMOE bit. */
    #endif

    #if (0U == RIIC_CFG_CH0_MASTER_MODE)
        #define RIIC_CH0_MALE           ((uint8_t)(0x00))       /* Mask for RIIC0.ICFER.MALE bit. */
    #elif (1U == RIIC_CFG_CH0_MASTER_MODE)
        #define RIIC_CH0_MALE           ((uint8_t)(0x02))       /* Mask for RIIC0.ICFER.MALE bit. */
    #endif

    #if (0U == RIIC_CFG_CH0_DIGITAL_FILTER)
        #define RIIC_CH0_NFE            ((uint8_t)(0x00))       /* Mask for RIIC0.ICFER.NFE bit. */
    #elif (1U <= RIIC_CFG_CH0_DIGITAL_FILTER) && (4U >= RIIC_CFG_CH0_DIGITAL_FILTER)
        #define RIIC_CH0_NFE            ((uint8_t)(0x20))       /* Mask for RIIC0.ICFER.NFE bit. */
    #endif

/* Sets ICFER */
    #define RIIC_CH0_ICFER_INIT ((uint8_t)(RIIC_CH0_TMOE | RIIC_CH0_MALE | RIIC_NALE | \
                                       RIIC_SALE | RIIC_NACKE | RIIC_CH0_NFE | RIIC_SCLE))

    #if (0U == RIIC_CFG_CH0_TMO_DET_TIME)
        #define RIIC_CH0_TMOS           ((uint8_t)(0x00))       /* long mode for the timeout detection time */
    #elif (1U == RIIC_CFG_CH0_TMO_DET_TIME)
        #define RIIC_CH0_TMOS           ((uint8_t)(0x01))       /* short mode for the timeout detection time */
    #endif

    #if (0U == RIIC_CFG_CH0_TMO_LCNT)
        #define RIIC_CH0_TMOL           ((uint8_t)(0x00))       /* Count is disable while the SCL is held LOW */
    #elif (1U == RIIC_CFG_CH0_TMO_LCNT)
        #define RIIC_CH0_TMOL           ((uint8_t)(0x02))       /* Count is enable while the SCL is held LOW */
    #endif

    #if (0U == RIIC_CFG_CH0_TMO_HCNT)
        #define RIIC_CH0_TMOH           ((uint8_t)(0x00))       /* Count is disable while the SCL is held HIGH */
    #elif (1U == RIIC_CFG_CH0_TMO_HCNT)
        #define RIIC_CH0_TMOH           ((uint8_t)(0x04))       /* Count is enable while the SCL is held HIGH */
    #endif

/* Sets ICMR2 */
    #define RIIC_CH0_ICMR2_INIT     ((uint8_t)(RIIC_CH0_TMOS | RIIC_CH0_TMOL | RIIC_CH0_TMOH))

/* Sets ICMR3 */
    #if (0U == RIIC_CFG_CH0_DIGITAL_FILTER) || (1U == RIIC_CFG_CH0_DIGITAL_FILTER)
        #define RIIC_CH0_ICMR3_INIT ((uint8_t)(0x00)) /* Sets RIIC0.ICMR3(Noise Filter Stage: single-stage filter.) */
    #elif (2U == RIIC_CFG_CH0_DIGITAL_FILTER)
        #define RIIC_CH0_ICMR3_INIT ((uint8_t)(0x01)) /* Sets RIIC0.ICMR3(Noise Filter Stage: 2-stage filter.) */
    #elif (3U == RIIC_CFG_CH0_DIGITAL_FILTER)
        #define RIIC_CH0_ICMR3_INIT ((uint8_t)(0x02)) /* Sets RIIC0.ICMR3(Noise Filter Stage: 3-stage filter.) */
    #elif (4U == RIIC_CFG_CH0_DIGITAL_FILTER)
        #define RIIC_CH0_ICMR3_INIT ((uint8_t)(0x03)) /* Sets RIIC0.ICMR3(Noise Filter Stage: 4-stage filter.) */
    #endif

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/
extern const uint16_t g_riic_bps[MAX_RIIC_CH_NUM];
extern const uint16_t g_riic_slv_ad0_val[MAX_RIIC_CH_NUM];
extern const uint16_t g_riic_slv_ad1_val[MAX_RIIC_CH_NUM];
extern const uint16_t g_riic_slv_ad2_val[MAX_RIIC_CH_NUM];

extern const uint8_t g_riic_slv_ad0_format[MAX_RIIC_CH_NUM];
extern const uint8_t g_riic_slv_ad1_format[MAX_RIIC_CH_NUM];
extern const uint8_t g_riic_slv_ad2_format[MAX_RIIC_CH_NUM];
extern const uint8_t g_riic_gca_enable[MAX_RIIC_CH_NUM];

extern const uint8_t g_riic_icmr2_init[MAX_RIIC_CH_NUM];
extern const uint8_t g_riic_icmr3_init[MAX_RIIC_CH_NUM];
extern const uint8_t g_riic_icfer_init[MAX_RIIC_CH_NUM];

#endif /* RIIC_RX23W_PRIVATE_H */

