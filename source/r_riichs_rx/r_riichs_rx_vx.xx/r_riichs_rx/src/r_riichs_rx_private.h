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
 * File Name    : r_riichs_rx_private.h
 * Description  : Functions for using RIICHS on RX devices. 
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 30.06.2021 1.00     First Release
 **********************************************************************************************************************/
/* Guards against multiple inclusion */
#ifndef RIICHS_PRIVATE_H
    #define RIICHS_PRIVATE_H
/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
/* Fixed width integer support. */
    #include "r_riichs_rx_if.h"
    #include "r_riichs_rx_pin_config.h"
    #include "r_riichs_rx_config.h"

/* Include specifics for chosen MCU.  */
    #if defined(BSP_MCU_RX671)
        #include "./targets/rx671/r_riichs_rx671_private.h"
    #else
        #error "This MCU is not supported by the current r_riichs_rx module."
    #endif

/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/
/*----------------------------------------------------------------------------*/
/*   Define for calculation processing.                                       */
/*----------------------------------------------------------------------------*/
/* Bit position masks */
    #define BIT0                      ((uint32_t)(0x00000001))
    #define BIT1                      ((uint32_t)(0x00000002))
    #define BIT2                      ((uint32_t)(0x00000004))
    #define BIT3                      ((uint32_t)(0x00000008))
    #define BIT4                      ((uint32_t)(0x00000010))
    #define BIT5                      ((uint32_t)(0x00000020))
    #define BIT6                      ((uint32_t)(0x00000040))
    #define BIT7                      ((uint32_t)(0x00000080))

    #define RIICHS_MAX_DIV              ((uint8_t)(0x08))
    #define RIICHS_STAD_SPPED_MAX       ((uint8_t)(100))
    #define RIICHS_FAST_SPPED_MAX       ((uint16_t)(400))
    #define RIICHS_FASTPLUS_SPPED_MAX   ((uint16_t)(1000))
    #define RIICHS_HIGH_SPPED_MAX       ((uint16_t)(3400))
    #define RIICHS_ICBR_MAX             ((uint16_t)(256))

/*----------------------------------------------------------------------------*/
/*   Define send data of blank.                                               */
/*----------------------------------------------------------------------------*/
    #define BLANK                     ((uint8_t)(0xFF))

/*----------------------------------------------------------------------------*/
/*   Define R/W code for slave address.                                       */
/*----------------------------------------------------------------------------*/
    #define W_CODE                    ((uint8_t)(0xFE))       /* Write code for slave address */
    #define R_CODE                    ((uint8_t)(0x01))       /* Read code for slave address */

/*----------------------------------------------------------------------------*/
/*   Define of port control.                                                  */
/*----------------------------------------------------------------------------*/
    #define RIICHS_OUT                ((uint8_t)(0x01))       /* Port Output */
    #define RIICHS_IN                 ((uint8_t)(0x00))       /* Port Input */

/*----------------------------------------------------------------------------*/
/*   Define bool type.                                                        */
/*----------------------------------------------------------------------------*/
    #define RIICHS_FALSE              ((bool)(0))             /* False */
    #define RIICHS_TRUE               ((bool)(1))             /* True */

/* RIICHS port (ascii to number) */
/* RIICHS0 */
    #if ((('0') <= R_RIICHS_CFG_RIICHS0_SCL0_PORT) && (('9') >= R_RIICHS_CFG_RIICHS0_SCL0_PORT))
        #define RIICHS_CH0_SCL0_GP (R_RIICHS_CFG_RIICHS0_SCL0_PORT - 0x30)
    #elif ((('A') <= R_RIICHS_CFG_RIICHS0_SCL0_PORT) && (('H') >= R_RIICHS_CFG_RIICHS0_SCL0_PORT))
        #define RIICHS_CH0_SCL0_GP (R_RIICHS_CFG_RIICHS0_SCL0_PORT - 0x37)
    #elif ((('J') <= R_RIICHS_CFG_RIICHS0_SCL0_PORT) && (('K') >= R_RIICHS_CFG_RIICHS0_SCL0_PORT))
/* PORTI does not exist in the PORT group. PORTJ register is placed at the position of PORTI register. */
        #define RIICHS_CH0_SCL0_GP (R_RIICHS_CFG_RIICHS0_SCL0_PORT - 0x38)
    #else
        #error "Error! Invalid setting for R_RIICHS_CFG_RIICHS0_SCL0_PORT in r_riichs_rx_pin_config.h"
    #endif
    #if ((('0') <= R_RIICHS_CFG_RIICHS0_SCL0_BIT) && (('7') >= R_RIICHS_CFG_RIICHS0_SCL0_BIT))
        #define RIICHS_CH0_SCL0_PIN    (R_RIICHS_CFG_RIICHS0_SCL0_BIT - 0x30)
    #else
        #error "Error! Invalid setting for R_RIICHS_CFG_RIICHS0_SCL0_BIT in r_riichs_rx_pin_config.h"
    #endif

    #if ((('0') <= R_RIICHS_CFG_RIICHS0_SDA0_PORT) && (('9') >= R_RIICHS_CFG_RIICHS0_SDA0_PORT))
        #define RIICHS_CH0_SDA0_GP (R_RIICHS_CFG_RIICHS0_SDA0_PORT - 0x30)
    #elif ((('A') <= R_RIICHS_CFG_RIICHS0_SDA0_PORT) && (('H') >= R_RIICHS_CFG_RIICHS0_SDA0_PORT))
        #define RIICHS_CH0_SDA0_GP (R_RIICHS_CFG_RIICHS0_SDA0_PORT - 0x37)
    #elif ((('J') <= R_RIICHS_CFG_RIICHS0_SDA0_PORT) && (('K') >= R_RIICHS_CFG_RIICHS0_SDA0_PORT))
/* PORTI does not exist in the PORT group. PORTJ register is placed at the position of PORTI register. */
        #define RIICHS_CH0_SDA0_GP (R_RIICHS_CFG_RIICHS0_SDA0_PORT - 0x38)
    #else
        #error "Error! Invalid setting for R_RIICHS_CFG_RIICHS0_SDA0_PORT in r_riichs_rx_pin_config.h"
    #endif
    #if ((('0') <= R_RIICHS_CFG_RIICHS0_SDA0_BIT) && (('7') >= R_RIICHS_CFG_RIICHS0_SDA0_BIT))
        #define RIICHS_CH0_SDA0_PIN    (R_RIICHS_CFG_RIICHS0_SDA0_BIT - 0x30)
    #else
        #error "Error! Invalid setting for R_RIICHS_CFG_RIICHS0_SDA0_BIT in r_riichs_rx_pin_config.h"
    #endif

/*============================================================================*/
/*  Configuration Options of non support                                      */
/*============================================================================*/
/* Setting to */
/* 0 = disable, 1 = enable */
    #define RIICHS_CFG_NACK_AL_ENABLE         (0)
    #define RIICHS_CFG_SLAVE_AL_ENABLE        (0)
    #define RIICHS_CFG_NACK_SUSPEND_ENABLE    (1)
    #define RIICHS_CFG_SCL_SYNC_ENABLE        (1)

/*============================================================================*/
/*  Configuration Options                                                     */
/*============================================================================*/
/* Setting to */
    #define RIICHS_CFG_CH0_STD_ENABLE         (1)  /* Select START Condition Detection Enable or disable */
    #define RIICHS_CFG_CH0_SPD_ENABLE         (1)  /* Select STOP Condition Detection enable or disable */
    #define RIICHS_CFG_CH0_TED_ENABLE         (1)  /* Select Transmit End Detection enable or disable */
    #define RIICHS_CFG_CH0_TD_ENABLE          (1)  /* Select Transmit Data Empty Detection enable or disable */
    #define RIICHS_CFG_CH0_RD_ENABLE          (1)  /* Select Receive Data Full Detection enable or disable */

/*============================================================================*/
/*  Parameter check of Configuration Options                                  */
/*============================================================================*/
    #if ((0 != RIICHS_CFG_PARAM_CHECKING_ENABLE) && (1 != RIICHS_CFG_PARAM_CHECKING_ENABLE))
        #error "ERROR - RIICHS_CFG_PARAM_CHECKING_ENABLE - Parameter error in configures file.  "
    #endif

    #if ((0 != RIICHS_CFG_PORT_SET_PROCESSING) && (1 != RIICHS_CFG_PORT_SET_PROCESSING))
        #error "ERROR - RIICHS_CFG_PORT_SET_PROCESSING - Parameter error in configures file.  "
    #endif

    #if ((0 != RIICHS_CFG_NACK_AL_ENABLE) && (1 != RIICHS_CFG_NACK_AL_ENABLE))
        #error "ERROR - RIICHS_CFG_NACK_AL_ENABLE - Parameter error in configures file.  "
    #endif

    #if ((0 != RIICHS_CFG_SLAVE_AL_ENABLE) && (1 != RIICHS_CFG_SLAVE_AL_ENABLE))
        #error "ERROR - RIICHS_CFG_SLAVE_AL_ENABLE - Parameter error in configures file.  "
    #endif

    #if ((0 != RIICHS_CFG_NACK_SUSPEND_ENABLE) && (1 != RIICHS_CFG_NACK_SUSPEND_ENABLE))
        #error "ERROR - RIICHS_CFG_NACK_SUSPEND_ENABLE - Parameter error in configures file.  "
    #endif

    #if ((0 != RIICHS_CFG_SCL_SYNC_ENABLE) && (1 != RIICHS_CFG_SCL_SYNC_ENABLE))
        #error "ERROR - RIICHS_CFG_SCL_SYNC_ENABLE - Parameter error in configures file.  "
    #endif

/*============================================================================*/
/*   Define for register setting.                                             */
/*============================================================================*/
/* Interrupt register setting */
    #define RIICHS_IR_CLR             ((uint8_t)(0x00))       /* Clears interrupt request register. */
    #define RIICHS_IR_SET             ((uint8_t)(0x01))       /* Sets interrupt request register. */
    #define RIICHS_IER_DISABLE        (0U)                    /* Disables interrupt request enable register. */
    #define RIICHS_IER_ENABLE         (1U)                    /* Enables interrupt request enable register. */

/* RIICHS register setting */
    #define RIICHS_ICCR_NOT_DRIVEN    ((uint32_t)(0x00000000))       /* Clears ICCR.ICE bit. */
    #define RIICHS_ICCR_ICE           ((uint32_t)(0x80000000))       /* ICCR.ICE bit. */
    #define RIICHS_ICCR_ICE_SET       ((uint32_t)(0x80000000))       /* Sets ICCR.ICE bit. */
    #define RIICHS_ICCR_ICE_CLR       ((uint32_t)(0x7FFFFFFF))       /* Clears ICCR.ICE bit. */
    #define RIICHS_ICRCR_RIICHS_RESET ((uint32_t)(0x00000001))       /* Sets ICRCR.MRST bit. */
    #define RIICHS_ICRCR_ENABLE       ((uint32_t)(0xFFFFFFFE))       /* Clears ICRCR.MRST bit. */
    #define RIICHS_ICRCR_ISRST_SET    ((uint32_t)(0x00010000))       /* Sets ICRCR.ISRST bit */
    #define RIICHS_ICRCR_ISRST_CLR    ((uint32_t)(0xFFFEFFFF))       /* Clear ICRCR.ISRST bit */
    #define RIICHS_ICOCR_CLO_SET      ((uint32_t)(0x00000010))       /* Sets ICOCR.CLO bit. */
    #define RIICHS_ICOCR_SOWP         ((uint32_t)(0x00000004))       /* ICOCR.SOWP bit. */
    #define RIICHS_ICOCR_SDAO_SET     ((uint32_t)(0x00000001))       /* Sets ICOCR.SDAO bit. */
    #define RIICHS_ICOCR_SDAO_CLR     ((uint32_t)(0xFFFFFFFE))       /* Clears ICOCR.SDAO bit. */
    #define RIICHS_ICOCR_SCLO_SET     ((uint32_t)(0x00000002))       /* Sets ICOCR.SCLO bit. */
    #define RIICHS_ICOCR_SCLO_CLR     ((uint32_t)(0xFFFFFFFD))       /* Clears ICOCR.SCLO bit. */
    #define RIICHS_ICOCR_SOWP_SET     ((uint32_t)(0x00000004))       /* Sets ICOCR.SOWP bit. */
    #define RIICHS_ICOCR_SOWP_CLR     ((uint32_t)(0xFFFFFFFB))       /* Clears ICOCR.SOWP bit. */
    #define RIICHS_ICIMR_SDAI         ((uint32_t)(0x00000002))       /* Mask for ICIMR.SDAI bit. */
    #define RIICHS_ICIMR_SDAI_SET     ((uint32_t)(0x00000002))       /* Sets for ICIMR.SDAI bit. */
    #define RIICHS_ICIMR_SCLI         ((uint32_t)(0x00000001))       /* Mask for ICIMR.SCLI bit. */
    #define RIICHS_ICIMR_SCLI_SET     ((uint32_t)(0x00000001))       /* Sets for ICIMR.SCLI bit. */
    #define RIICHS_ICIMR_SDAO         ((uint32_t)(0x00000008))       /* Mask for ICIMR.SDAO bit. */
    #define RIICHS_ICIMR_SDAO_SET     ((uint32_t)(0x00000008))       /* Sets for ICIMR.SDAO bit. */
    #define RIICHS_ICIMR_SCLO         ((uint32_t)(0x00000004))       /* Mask for ICIMR.SCLO bit. */
    #define RIICHS_ICIMR_SCLO_SET     ((uint32_t)(0x00000004))       /* Sets for ICIMR.SCLO bit. */
    #define RIICHS_SAR0_INIT          ((uint32_t)(0x00000000))       /* Sets SARy. (y=0,1,2) */
    #define RIICHS_SAR1_INIT          ((uint32_t)(0x00000000))       /* Sets SARy. (y=0,1,2) */
    #define RIICHS_SAR2_INIT          ((uint32_t)(0x00000000))       /* Sets SARy. (y=0,1,2) */
    #define RIICHS_SAR_10BIT_FORMAT_SET ((uint32_t)(0x00000400))     /* Sets SARy.FS bit. */
    #define RIICHS_ICSCR_INIT         ((uint32_t)(0x00000000))       /* Sets ICSCR */
    #define RIICHS_ICSCR_SAR_DISABLE  ((uint32_t)(0xFFF8FFFF))       /* Clears ICSCR.SAR0E,SAR1E,SAR2E bit */
    #define RIICHS_ICSCR_SAR0E_SET    ((uint32_t)(0x00010000))       /* Sets ICSCR.SAR0E bit */
    #define RIICHS_ICSCR_SAR0E_CLR    ((uint32_t)(0xFFFEFFFF))       /* Clears ICSCR.SAR0E bit */
    #define RIICHS_ICSCR_SAR1E_SET    ((uint32_t)(0x00020000))       /* Sets ICSCR.SAR1E bit */
    #define RIICHS_ICSCR_SAR1E_CLR    ((uint32_t)(0xFFFDFFFF))       /* Clears ICSCR.SAR1E bit */
    #define RIICHS_ICSCR_SAR2E_SET    ((uint32_t)(0x00040000))       /* Sets ICSCR.SAR2E bit */
    #define RIICHS_ICSCR_SAR2E_CLR    ((uint32_t)(0xFFFBFFFF))       /* Clears ICSCR.SAR2E bit */
    #define RIICHS_ICSCR_GCAE_SET     ((uint32_t)(0x00000001))       /* Sets ICSCR.GCAE bit */
    #define RIICHS_ICSCR_GCAE_CLR     ((uint32_t)(0xFFFFFFFE))       /* Clears ICSCR.GCAE bit */
    #define RIICHS_ICSCR_DIDE_SET     ((uint32_t)(0x00000040))       /* Sets ICSCR.DIDE bit */
    #define RIICHS_ICSCR_DIDE_CLR     ((uint32_t)(0xFFFFFFBF))       /* Clears ICSCR.DIDE bit */
    #define RIICHS_ICSCR_HSMCE_SET     ((uint32_t)(0x00000020))       /* Sets ICSCR.HSMCE bit */

    #define RIICHS_ICFER_SCLE         ((uint32_t)(0x00000100))       /* Mask for ICFER.SCLE bit. */
    #define RIICHS_ICICR_NFE          ((uint32_t)(0x00000010))       /* Mask for ICICR.NFE bit. */
    #define RIICHS_ICRCCR_CKS         ((uint32_t)(0x00000007))       /* Mask for ICRCCR.CKS bit. */

    #define RIICHS_ICTOR_INIT         ((uint32_t)(0x00000000))       /* Sets ICTOR */

    #if RIICHS_CFG_NACK_AL_ENABLE == 0
        #define RIICHS_NALE           ((uint32_t)(0x00000000))       /* Mask for ICFER.NALE bit. */
    #elif RIICHS_CFG_NACK_AL_ENABLE == 1
        #define RIICHS_NALE           ((uint32_t)(0x00000002))       /* Mask for ICFER.NALE bit. */
    #endif

    #if RIICHS_CFG_SLAVE_AL_ENABLE == 0
        #define RIICHS_SALE           ((uint32_t)(0x00000000))       /* Mask for ICFER.SALE bit. */
    #elif RIICHS_CFG_SLAVE_AL_ENABLE == 1
        #define RIICHS_SALE           ((uint32_t)(0x00000004))       /* Mask for ICFER.SALE bit. */
    #endif

    #if RIICHS_CFG_SCL_SYNC_ENABLE == 0
        #define RIICHS_SCLE           ((uint32_t)(0x00000000))       /* Mask for ICFER.SCLE bit. */
    #elif RIICHS_CFG_SCL_SYNC_ENABLE == 1
        #define RIICHS_SCLE           ((uint32_t)(0x00000100))       /* Mask for ICFER.SCLE bit. */
    #endif

    #define RIICHS_ICFER_MALE_SET     ((uint32_t)(0x00000001))       /* Sets RIICHS0.ICFER.MALE bit. */
    #define RIICHS_ICFER_MALE_CLR     ((uint32_t)(0xFFFFFFFE))       /* Clears RIICHS0.ICFER.MALE bit. */
    #define RIICHS_ICFER_FMPE_SET     ((uint32_t)(0x00004000))       /* Sets RIICHS0.ICFER.FMPE bit. */
    #define RIICHS_ICFER_FMPE_CLR     ((uint32_t)(0xFFFFBFFF))       /* Clears RIICHS0.ICFER.FMPE bit. */
    #define RIICHS_ICFER_HSME_SET     ((uint32_t)(0x00008000))       /* Sets RIICHS0.ICFER.HSME bit. */
    #define RIICHS_ICFER_HSME_CLR     ((uint32_t)(0xFFFF7FFF))       /* Clears RIICHS0.ICFER.HSME bit. */

    #define RIICHS_ICICR_NF_SET       ((uint32_t)(0x00000010))       /* Sets RIICHS0.ICICR.NF bit. */
    #define RIICHS_ICICR_NF_CLR       ((uint32_t)(0xFFFFFFEF))       /* Clears RIICHS0.ICICR.NF bit. */

    #define RIICHS_ICSER_STDE         ((uint32_t)(0x00000001))       /* Mask for ICSER.STDE bit. */
    #define RIICHS_ICSER_SPDE         ((uint32_t)(0x00000002))       /* Mask for ICSER.SPDE bit. */
    #define RIICHS_ICSER_TEDE         ((uint32_t)(0x00000100))       /* Mask for ICSER.TEDE bit. */
    #define RIICHS_ICSER_TMOE_SET     ((uint32_t)(0x00100000))       /* Sets RIICHS0.ICSER.TMOE bit. */
    #define RIICHS_ICSER_TMOE_CLR     ((uint32_t)(0xFFEFFFFF))       /* Clears RIICHS0.ICSER.TMOE bit. */
    #define RIICHS_ICSER_NAKDE_SET    ((uint32_t)(0x00000010))       /* Sets RIICHS0.ICSER.NAKDE bit. */
    #define RIICHS_ICSER_NAKDE_CLR    ((uint32_t)(0xFFFFFFEF))       /* Clears RIICHS0.ICSER.NAKDE bit. */
    #define RIICHS_ICSER_ALE_SET      ((uint32_t)(0x00010000))       /* Sets RIICHS0.ICSER.ALE bit. */
    #define RIICHS_ICSER_ALE_CLR      ((uint32_t)(0xFFFEFFFF))       /* Clears RIICHS0.ICSER.ALE bit. */

    #define RIICHS_ICTOR_TMOL_SET     ((uint32_t)(0x00000010))       /* Sets RIICHS0.ICTOR.TMOL bit. */
    #define RIICHS_ICTOR_TMOL_CLR     ((uint32_t)(0xFFFFFFEF))       /* Clears RIICHS0.ICTOR.TMOL bit. */
    #define RIICHS_ICTOR_TMOH_SET     ((uint32_t)(0x00000020))       /* Sets RIICHS0.ICTOR.TMOH bit. */
    #define RIICHS_ICTOR_TMOH_CLR     ((uint32_t)(0xFFFFFFDF))       /* Clears RIICHS0.ICTOR.TMOH bit. */

    #define RIICHS_ICCSER_TDE_SET     ((uint32_t)(0x00000001))       /* Sets RIICHS0.ICCSER.TDE bit. */
    #define RIICHS_ICCSER_TDE_CLR     ((uint32_t)(0x00000000))       /* Clears RIICHS0.ICCSER.TDE bit. */
    #define RIICHS_ICCSER_RDE_SET     ((uint32_t)(0x00000002))       /* Sets RIICHS0.ICCSER.RDE bit. */
    #define RIICHS_ICCSER_RDE_CLR     ((uint32_t)(0x00000000))       /* Clears RIICHS0.ICCSER.RDE bit. */

    #define RIICHS_CH0_ICACKR_INIT    ((uint32_t)(0x00000000))       /* Sets ICACKR */
    #define RIICHS_CH0_ICCSCR_INIT    ((uint32_t)(0x00000000))       /* Sets ICCSCR */
    #define RIICHS_CH0_ICOCR_INIT     ((uint32_t)(0x00000003))       /* Sets ICOCR */

    #define RIICHS_ICCGR_ST           ((uint32_t)(0x00000001))       /* Mask for ICCGR.ST bit. */
    #define RIICHS_ICCGR_RS           ((uint32_t)(0x00000002))       /* Mask for ICCGR.RS bit. */
    #define RIICHS_ICCGR_SP           ((uint32_t)(0x00000004))       /* Mask for ICCGR.SP bit. */
    #define RIICHS_ICMMR_TRS          ((uint32_t)(0x00000010))       /* Mask for ICCR2.TRS bit. */
    #define RIICHS_ICMMR_TRS_SET      ((uint32_t)(0x00000010))       /* Sets ICMMR.TRS bit. */
    #define RIICHS_ICMMR_TRS_CLR      ((uint32_t)(0xFFFFFFEF))       /* Clears ICMMR.TRS bit. */
    #define RIICHS_ICMMR_MST          ((uint32_t)(0x00000004))       /* Mask for ICMMR.MST bit. */
    #define RIICHS_ICMMR_MST_SET      ((uint32_t)(0x00000004))       /* Sets ICMMR.MST bit. */
    #define RIICHS_ICBSR_BFREE        ((uint32_t)(0x00000001))       /* Mask for ICBSR.BFREE bit. */
    #define RIICHS_ICBSR_BFREE_SET    ((uint32_t)(0x00000000))       /* Sets ICBSR.BFREE bit. */
    #define RIICHS_ICCSCR_WAITRE_SET  ((uint32_t)(0x00000002))       /* Sets ICCSCR.WAIT bit. */
    #define RIICHS_ICCSCR_WAITRE_CLR  ((uint32_t)(0xFFFFFFFD))       /* Clears ICCSCR.WAIT bit. */
    #define RIICHS_ICCSCR_WAITAE_SET  ((uint32_t)(0x00000001))       /* Sets ICCSCR.WAITAE bit. */
    #define RIICHS_ICCSCR_WAITAE_CLR  ((uint32_t)(0xFFFFFFFE))       /* Clears ICCSCR.WAITAE bit. */
    #define RIICHS_ICACKR_ACKWP_SET   ((uint32_t)(0x00000004))       /* Sets ICACKR.ACKWP bit. */
    #define RIICHS_ICACKR_ACKWP_CLR   ((uint32_t)(0xFFFFFFFB))       /* Clears ICACKR.ACKWP bit. */
    #define RIICHS_ICACKR_ACKBT_SET   ((uint32_t)(0x00000002))       /* Sets ICACKR.ACKBT bit. */
    #define RIICHS_ICACKR_ACKBT_CLR   ((uint32_t)(0xFFFFFFFD))       /* Clears ICACKR.ACKBT bit. */
    #define RIICHS_ICSSR_AAS0         ((uint32_t)(0x00010000))       /* Mask for ICSSR.AAS0 bit. */
    #define RIICHS_ICSSR_AAS0_SET     ((uint32_t)(0x00010000))       /* Sets ICSSR.AAS0 bit. */
    #define RIICHS_ICSSR_AAS1         ((uint32_t)(0x00020000))       /* Mask for ICSSR.AAS1 bit. */
    #define RIICHS_ICSSR_AAS1_SET     ((uint32_t)(0x00020000))       /* Sets ICSSR.AAS1 bit. */
    #define RIICHS_ICSSR_AAS2         ((uint32_t)(0x00040000))       /* Mask for ICSSR.AAS2 bit. */
    #define RIICHS_ICSSR_AAS2_SET     ((uint32_t)(0x00040000))       /* Sets ICSSR.AAS2 bit. */
    #define RIICHS_ICSSR_GCA          ((uint32_t)(0x00000001))       /* Mask for ICSSR.GCA bit. */
    #define RIICHS_ICSSR_GCA_SET      ((uint32_t)(0x00000001))       /* Sets ICSSR.GCA bit. */
    #define RIICHS_ICSSR_HSMC         ((uint32_t)(0x00000020))       /* Mask for ICSSR.HSMC bit. */
    #define RIICHS_ICSSR_HSMC_SET     ((uint32_t)(0x00000020))       /* Sets ICSSR.HSMC bit. */
    #define RIICHS_ICSSR_DID          ((uint32_t)(0x00000040))       /* Mask for ICSSR.DID bit. */
    #define RIICHS_ICSSR_DID_SET      ((uint32_t)(0x00000040))       /* Sets ICSSR.DID bit. */
    #define RIICHS_ICSSR_HOA          ((uint32_t)(0x00008000))       /* Mask for ICSSR.HOA bit. */
    #define RIICHS_ICSSR_HOA_SET      ((uint32_t)(0x00008000))       /* Sets ICSSR.HOA bit. */
    #define RIICHS_ICSR2_TMOF         ((uint32_t)(0x00100000))       /* Mask for ICSR2.TMOF bit. */
    #define RIICHS_ICSR2_TMOF_SET     ((uint32_t)(0x00100000))       /* Sets ICSR2.TMOF bit. */
    #define RIICHS_ICSR2_AL           ((uint32_t)(0x00010000))       /* Mask for ICSR2.AL bit. */
    #define RIICHS_ICSR2_AL_SET       ((uint32_t)(0x00010000))       /* Sets ICSR2.AL bit. */
    #define RIICHS_ICSR2_AL_CLR       ((uint32_t)(0xFFFEFFFF))       /* Clears ICSR2.AL bit. */
    #define RIICHS_ICSR2_START        ((uint32_t)(0x00000001))       /* Mask for ICSR2.START bit. */
    #define RIICHS_ICSR2_START_SET    ((uint32_t)(0x00000001))       /* Sets ICSR2.START bit. */
    #define RIICHS_ICSR2_START_CLR    ((uint32_t)(0xFFFFFFFE))       /* Clears ICSR2.START bit. */
    #define RIICHS_ICSR2_STOP         ((uint32_t)(0x00000002))       /* Mask for ICSR2.STOP bit. */
    #define RIICHS_ICSR2_STOP_SET     ((uint32_t)(0x00000002))       /* Sets ICSR2.STOP bit. */
    #define RIICHS_ICSR2_STOP_CLR     ((uint32_t)(0xFFFFFFFD))       /* Clears ICSR2.STOP bit. */
    #define RIICHS_ICSR2_NACKF        ((uint32_t)(0x00000010))       /* Mask for ICSR2.NACKF bit. */
    #define RIICHS_ICSR2_NACKF_SET    ((uint32_t)(0x00000010))       /* Sets ICSR2.NACKF bit. */
    #define RIICHS_ICSR2_NACKF_CLR    ((uint32_t)(0xFFFFFFEF))       /* Clears ICSR2.NACKF bit. */
    #define RIICHS_ICCSR_RDRF         ((uint32_t)(0x00000002))       /* Mask for ICCSR.RDRF bit. */
    #define RIICHS_ICCSR_RDRF_SET     ((uint32_t)(0x00000002))       /* Sets ICCSR.RDRF bit. */
    #define RIICHS_ICCSR_TDRE         ((uint32_t)(0x00000001))       /* Mask for ICCSR.TDRE bit. */
    #define RIICHS_ICCSR_TDRE_SET     ((uint32_t)(0x00000001))       /* Sets ICCSR.TDRE bit. */
    #define RIICHS_ICSR2_DISABLE      ((uint32_t)(0xFFFFFFED))       /* Clears ICSR2.NACKF bit and STOP bit. */
    #define RIICHS_ICRCR_INIT         ((uint32_t)(0x00000000))       /* Initializes ICICR. */
    #define RIICHS_ICSIER_INIT        ((uint32_t)(0x00000000))       /* Initializes ICSIER. */
    #define RIICHS_ICCSIER_INIT       ((uint32_t)(0x00000000))       /* Initializes ICCSIER. */
    #define ICHBR_ICFBR_HIGH16_MASK   ((uint32_t)(0x00000000))       /* Mask for ICFBR HIGH 16 bits */

/* Enable bit  : ALIE, SIE, NAKIE           */
/* Disable bit : TMOIE, SPIE, TEIE          */
    #define RIICHS_ICSIER_ST_NAK_AL   ((uint32_t)(0x00010011))       /* Sets ICSIER.STIE ALIE and NAKIE. */

/* Enable bit  : ALIE, SPIE, NAKIE          */
/* Disable bit : TMOIE, STIE, TEIE          */
    #define RIICHS_ICSIER_SP_NAK_AL   ((uint32_t)(0x00010012))       /* Sets ICSIER.SPIE ALIE and NAKIE. */

/* Enable bit  : ALIE, NAKIE, TEIE          */
/* Disable bit : TMOIE, STIE, SPIE          */
    #define RIICHS_ICSIER_TEND_NAK_AL ((uint32_t)(0x00010110))       /* Sets ICSIER.TEIE ALIE and NAKIE bit. */

/* Enable bit  : ALIE, NAKIE                */
/* Disable bit : TMOIE, STIE, SPIE, TEIE    */
    #define RIICHS_ICSIER_NAK_AL      ((uint32_t)(0x00010010))       /* Sets ICSIER.NAKIE bit and ALIE bit. */

/* Enable bit  : ALIE, SPIE                 */
/* Disable bit : TMOIE, STIE, NAKIE, TEIE   */
    #define RIICHS_ICSIER_SP_AL       ((uint32_t)(0x00010002))       /* Sets ICSIER.SPIE bit and ALIE bit. */

/* Enable bit  : ALIE                       */
/* Disable bit : TMOIE, STIE, SPIE, NAKIE, TEIE */
    #define RIICHS_ICSIER_AL          ((uint32_t)(0x00010000))       /* Sets ICSIER.ALIE bit. */

/* Enable bit  : SPIE                       */
/* Disable bit : TMOIE, ALIE, STIE, NAKIE, TEIE */
    #define RIICHS_ICSIER_SP          ((uint32_t)(0x00000002))       /* Sets ICSIER.SPIE bit. */

/* Disable bit : ALIE, TMOIE, STIE, TEIE, SPIE, NAKIE */
    #define RIICHS_ICSIER_ICCSIER_NULL ((uint32_t)(0x00000000))      /* Sets ICSIER or ICCSIER register. */

/* Enable bit  : SPIE, NAKIE                */
/* Disable bit : ALIE, TMOIE, STIE, TEIE    */
    #define RIICHS_ICSIER_SP_NAK      ((uint32_t)(0x00000012))       /* Sets ICSIER register. */

/* Enable bit  : RIE, TIE                   */
    #define RIICHS_ICCSIER_TX_RX      ((uint32_t)(0x00000003))       /* Sets ICCSIER register. */

/* Enable bit  : TIE */
/* Disable bit : RIE */
    #define RIICHS_ICCSIER_TX         ((uint32_t)(0x00000001))       /* Sets ICCSIER register. */

/* Enable bit  : RIE */
/* Disable bit : TIE */
    #define RIICHS_ICCSIER_RX         ((uint32_t)(0x00000002))       /* Sets ICCSIER register. */

/* Enable bit  : TMOIE */
    #define RIICHS_ICSIER_TMO         ((uint32_t)(0x00100000))       /* Sets ICSIER register. */

    #define RIICHS_MSK_BFREE          ((uint32_t)(0x00000001))       /* Mask ICBSR.BFREE bit  */
    #define RIICHS_MSK_SCLSDA         ((uint32_t)(0x00000003))       /* Mask ICIMR.SDAI bit and SCLI bit */

/* Common registers setting */
    #define RIICHS_PWPR_BOWI          (MPC.PWPR.BIT.B0WI)            /* PWPR PFSWE Bit Write Disable */

/* Control registers address defines */
    #define RIICHS_ICCR_ADR        ( (volatile uint32_t *)&RIICHS0 + 0x0000)
    #define RIICHS_ICRCR_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x0003)
    #define RIICHS_ICMMR_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x0004)
    #define RIICHS_ICFER_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x0013)
    #define RIICHS_ICSCR_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x0014)
    #define RIICHS_ICRCCR_ADR      ( (volatile uint32_t *)&RIICHS0 + 0x0017)
    #define RIICHS_ICFBR_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x0018)
    #define RIICHS_ICHBR_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x0019)
    #define RIICHS_ICBFTR_ADR      ( (volatile uint32_t *)&RIICHS0 + 0x001A)
    #define RIICHS_ICOCR_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x001D)
    #define RIICHS_ICICR_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x001E)
    #define RIICHS_ICTOR_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x001F)
    #define RIICHS_ICACKR_ADR      ( (volatile uint32_t *)&RIICHS0 + 0x0023)
    #define RIICHS_ICCSCR_ADR      ( (volatile uint32_t *)&RIICHS0 + 0x0024)
    #define RIICHS_ICCGR_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x004B)
    #define RIICHS_ICDR_ADR        ( (volatile uint32_t *)&RIICHS0 + 0x0051)
    #define RIICHS_ICSR2_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x006F)
    #define RIICHS_ICSER_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x0070)
    #define RIICHS_ICSIER_ADR      ( (volatile uint32_t *)&RIICHS0 + 0x0071)
    #define RIICHS_ICCSR_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x0073)
    #define RIICHS_ICCSER_ADR      ( (volatile uint32_t *)&RIICHS0 + 0x0074)
    #define RIICHS_ICCSIER_ADR     ( (volatile uint32_t *)&RIICHS0 + 0x0075)
    #define RIICHS_ICBSR_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x007F)
    #define RIICHS_ICSSR_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x0080)
    #define RIICHS_SAR0_ADR        ( (volatile uint32_t *)&RIICHS0 + 0x00A7)
    #define RIICHS_SAR1_ADR        ( (volatile uint32_t *)&RIICHS0 + 0x00A8)
    #define RIICHS_SAR2_ADR        ( (volatile uint32_t *)&RIICHS0 + 0x00A9)
    #define RIICHS_SAMR0_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x00C7)
    #define RIICHS_SAMR1_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x00C8)
    #define RIICHS_SAMR2_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x00C9)
    #define RIICHS_ICBCR_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x00DB)
    #define RIICHS_ICIMR_ADR       ( (volatile uint32_t *)&RIICHS0 + 0x00EE)

/* Base register of PCR used to calculate all PCR register addresses. This is constant for all supported MCUs. */
    #define RIICHS_PRV_PCR_BASE_REG ((uint8_t volatile *)(&MPC.PWPR.BYTE-95))
/* Base register of PDR used to calculate all PDR register addresses. This is constant for all supported MCUs. */
    #define RIICHS_PRV_PDR_BASE_REG ((uint8_t volatile *)(&MPC.PWPR.BYTE-287))
/* Base register of PMR used to calculate all PMR register addresses. This is constant for all supported MCUs. */
    #define RIICHS_PRV_PMR_BASE_REG ((uint8_t volatile *)(&MPC.PWPR.BYTE-191))
/* Base register of PFS used to calculate all PFS register addresses. This is constant for all supported MCUs. */
    #define RIICHS_PRV_PFS_BASE_REG ((uint8_t volatile *)(&MPC.PWPR.BYTE+33))

/***********************************************************************************************************************
 Typedef definitions
 **********************************************************************************************************************/
/*----------------------------------------------------------------------------*/
/*   Define internal iic information structure type.                          */
/*----------------------------------------------------------------------------*/
/* ---- Internal Mode. ---- */
typedef enum
{
    RIICHS_MODE_NONE = 0U, /* Non-communication mode */
    RIICHS_MODE_M_SEND, /* Master transmission mode */
    RIICHS_MODE_M_RECEIVE, /* Master reception mode */
    RIICHS_MODE_M_SEND_RECEIVE, /* Master transmission reception mode */
    RIICHS_MODE_S_READY, /* Multi master transfer mode */
    RIICHS_MODE_S_SEND, /* Slave transmission mode */
    RIICHS_MODE_S_RECEIVE /* Slave reception mode */
} riichs_api_mode_t;

/* ---- Internal Status. ---- */
typedef enum
{
    RIICHS_STS_NO_INIT = 0U, /* None initialization state */
    RIICHS_STS_IDLE, /* Idle state */
    RIICHS_STS_IDLE_EN_SLV, /* Idle state on enable slave transfer */
    RIICHS_STS_ST_COND_WAIT, /* Start condition generation completion wait state */
    RIICHS_STS_MASTER_CODE_WAIT, /* Send master code state */
    RIICHS_STS_SEND_SLVADR_W_WAIT, /* Slave address [Write] transmission completion wait state */
    RIICHS_STS_SEND_SLVADR_R_WAIT, /* Slave address [Read] transmission completion wait state */
    RIICHS_STS_SEND_DATA_WAIT, /* Data transmission completion wait state */
    RIICHS_STS_RECEIVE_DATA_WAIT, /* Data reception completion wait state */
    RIICHS_STS_SP_COND_WAIT, /* Stop condition generation completion wait state */
    RIICHS_STS_AL, /* Detect Arbitration Lost */
    RIICHS_STS_TMO, /* Detect Time out */
    RIICHS_STS_MAX /* Prohibition of setup above here */
} riichs_api_status_t;

/* ---- Internal Event. ---- */
typedef enum
{
    RIICHS_EV_INIT = 0U, /* Called function of Initializes IIC driver */
    RIICHS_EV_EN_SLV_TRANSFER, /* Enable Slave Transfer */
    RIICHS_EV_GEN_START_COND, /* Called function of Start condition generation */
    RIICHS_EV_INT_START, /* Interrupted start condition generation */
    RIICHS_EV_INT_ADD, /* Interrupted address sending */
    RIICHS_EV_INT_SEND, /* Interrupted data sending */
    RIICHS_EV_INT_RECEIVE, /* Interrupted data receiving */
    RIICHS_EV_INT_STOP, /* Interrupted Stop condition generation */
    RIICHS_EV_INT_AL, /* Interrupted Arbitration-Lost */
    RIICHS_EV_INT_NACK, /* Interrupted No Acknowledge */
    RIICHS_EV_INT_TMO, /* Interrupted Time out */
    RIICHS_EV_MAX /* Prohibition of setup above here */
} riichs_api_event_t;

/* ---- Internal Information structure. ---- */
typedef struct
{
    riichs_api_mode_t   N_Mode; /* Internal mode of control protocol */
    riichs_api_mode_t   B_Mode; /* Internal mode of control protocol */
    riichs_api_status_t N_status; /* Internal Status of Now */
    riichs_api_status_t B_status; /* Internal Status of Before */
} riichs_api_info_t;

/* ---- Internal state transition structure ---- */
typedef struct
{
    riichs_api_event_t event_type; /* Event */
    riichs_return_t    (*proc) (riichs_info_t *); /* handler */
} riichs_mtx_t;

/***********************************************************************************************************************
 Exported global variables
 **********************************************************************************************************************/
extern uint8_t g_riichs_rxi_int_priority[];
extern uint8_t g_riichs_txi_int_priority[];
extern uint8_t g_riichs_eei_int_priority[];
extern uint8_t g_riichs_tei_int_priority[];
extern uint8_t g_riichs_timeout_enable[];

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/
bool riichs_param_check (riichs_info_t * p_riichs_info);
bool riichs_slave_address_check (uint8_t format, uint16_t address);
bool riichs_mcu_check_channel (uint8_t channe);
void riichs_mcu_int_init (uint8_t channe);
uint8_t riichs_mcu_check_ir_txi (uint8_t channel);
uint8_t riichs_mcu_check_ir_rxi (uint8_t channel);
void riichs_mcu_clear_ir_txi (uint8_t channel);
void riichs_mcu_clear_ir_rxi (uint8_t channel);
void riichs_mcu_int_enable (uint8_t channe);
void riichs_mcu_int_disable (uint8_t channe);
void riichs_mcu_power_on (uint8_t channel);
void riichs_mcu_power_off (uint8_t channel);
bool riichs_mcu_hardware_lock (uint8_t channel);
void riichs_mcu_hardware_unlock (uint8_t channel);
double riichs_mcu_check_freq (void);
void riichs_mcu_int_icsier_iccsier_setting (uint8_t channel, uint32_t New_icsier,uint32_t New_iccsier);

    #if (1U == RIICHS_CFG_PORT_SET_PROCESSING)
void riichs_mcu_io_open (uint8_t channe);
void riichs_mcu_mpc_enable (uint8_t channe);
void riichs_mcu_mpc_disable (uint8_t channe);
    #endif

    #if (1U == RIICHS_CFG_CH0_INCLUDED)
void riichs0_eei_sub (void);
void riichs0_txi_sub (void);
void riichs0_rxi_sub (void);
void riichs0_tei_sub (void);
    #endif

#endif /* RIICHS_PRIVATE_H */

