/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014(2015-2016) Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_dmaca_rx_target.h
* Description  : Functions for DMACA driver
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 30.06.2014 1.00    Initial revision
*         : 27.08.2014 1.01    Not change program due to updating application note only.
*         : 30.01.2015 1.02    Added RX71M.
*         : 13.04.2015 1.03    Added RX231.
*         : 30.09.2016 1.04    Added #define DMACA_INVALID_LOWER_BIT_MASK.
*         :                    Comment out #define DMACA_NUM_INTERRUPT_SRC
*         :                    because this FIT module not use it.
*         : 31.07.2017 1.05    Fixed to correspond to Renesas coding rule.
*         : 20.05.2019 2.00    Added support for GNUC and ICCRX.
*******************************************************************************/
#ifndef DMACA_RX_TARGET_H
#define DMACA_RX_TARGET_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* Number of activation sources */
/* #define DMACA_NUM_INTERRUPT_SRC      (194) */

/* Maximum value of 16bit count value (normal transfer mode) */
#define DMACA_MAX_16BITS_COUNT_VAL_A    (65535)
/* Maximum value of 16bit count value (repeat or block transfer mode) */
#define DMACA_MAX_16BITS_COUNT_VAL_B    (65536)
/* Maximum value of 10bit count value */
#define DMACA_MAX_10BITS_COUNT_VAL      (1024)
/* Minimum of count value and block size */
#define DMACA_MIN_COUNT_VAL             (1)

/* Definition of number of DMACA channel */
#define DMACA_NUM_CHANNELS              (8)

/* DMACA source address must be in 0x00000000h to 0x0FFFFFFF and 0xF0000000 to 0xFFFFFFFF. */
#define DMACA_INVALID_SRC_ADDR_MASK     (0xf0000000u)
/* DMACA destination address must be in 0x00000000h to 0x0FFFFFFF and 0xF0000000 to 0xFFFFFFFF. */
#define DMACA_INVALID_DES_ADDR_MASK     (0xf0000000u)

/* DMACA channel0 offset address must be in 0x00000000h to 0x00FFFFFF and 0xFF000000 to 0xFFFFFFFF. */
#define DMAC0_INVALID_OFFSET_VALUE_MASK (0xff000000u)

/* DMACA lower bits of transfer count bit mask */
#define DMACA_INVALID_LOWER_BIT_MASK    (0x0000ffffu)

/* ---- DMACA register base address ---- */
#define DMACA_BASE_ADDR        ((uint32_t)&DMAC0)

/* DMACA ch4 Interrupt request bit mask */
#define DMACA_DMIS4_BIT_MASK   (0x10u)
/* DMACA ch5 Interrupt request bit mask */
#define DMACA_DMIS5_BIT_MASK   (0x20u)
/* DMACA ch6 Interrupt request bit mask */
#define DMACA_DMIS6_BIT_MASK   (0x40u)
/* DMACA ch7 Interrupt request bit mask */
#define DMACA_DMIS7_BIT_MASK   (0x80u)

/* ---- DMACA register address offset ---- */
/* DMA Source Address Register address offset */
#define DMACA_DMSAR_OFST   (0x00u)
/* DMA Destination Address Register address offset */
#define DMACA_DMDAR_OFST   (0x04u)
/* DMA Transfer Count Register address offset */
#define DMACA_DMCRA_OFST   (0x08u)
/* DMA Block Transfer Count Register address offset */
#define DMACA_DMCRB_OFST   (0x0Cu)
/* DMA Transfer Mode Register address offset */
#define DMACA_DMTMD_OFST   (0x10u)
/* DMA Interrupt Setting Register address offset */
#define DMACA_DMINT_OFST   (0x13u)
/* DMA Address Mode Register address offset */
#define DMACA_DMAMD_OFST   (0x14u)
/* DMA Offset Register address offset */
#define DMACA_DMOFR_OFST   (0x18u)
/* DMA Transfer Enable Register address offset */
#define DMACA_DMCNT_OFST   (0x1Cu)
/* DMA Software Start Register address offset */
#define DMACA_DMREQ_OFST   (0x1Du)
/* DMA Status Register address offset */
#define DMACA_DMSTS_OFST   (0x1Eu)
/* DMA Activation Source Flag Control Register address offset */
#define DMACA_DMCSL_OFST   (0x1Fu)
/* DMACA Module Activation Register address offset */
#define DMACA_DMAST_OFST   (0x200u)
/* DMAC74 Interrupt Status Monitor Register address offset */
#define DMACA_DMIST_OFST   (0x204u)


/* ---- Registers and bits for ICU ---- */
/* DMACA Activation Request Select Register */
#define ICU_DMRSR(dmaca_channel)       (*g_icu_dmrsr[dmaca_channel])

/* ---- Registers and bits for DMACA ---- */
/* DMA Source Address Register */
#define DMACA_DMSAR(dmaca_channel)     (*(volatile void R_BSP_EVENACCESS_SFR   **)(DMACA_BASE_ADDR + DMACA_DMSAR_OFST \
                                                                               + (0x40 * dmaca_channel)))
/* DMA Destination Address Register */
#define DMACA_DMDAR(dmaca_channel)     (*(volatile void R_BSP_EVENACCESS_SFR   **)(DMACA_BASE_ADDR + DMACA_DMDAR_OFST \
                                                                               + (0x40 * dmaca_channel)))
/* DMA Transfer Count Register */
#define DMACA_DMCRA(dmaca_channel)     (*(volatile uint32_t R_BSP_EVENACCESS_SFR *)(DMACA_BASE_ADDR + DMACA_DMCRA_OFST \
                                                                                + (0x40 * dmaca_channel)))
/* DMA Block Transfer Count Register */
#define DMACA_DMCRB(dmaca_channel)     (*(volatile uint16_t R_BSP_EVENACCESS_SFR *)(DMACA_BASE_ADDR + DMACA_DMCRB_OFST \
                                                                                + (0x40 * dmaca_channel)))
/* DMA Transfer Mode Register */
#define DMACA_DMTMD(dmaca_channel)     (*(volatile uint16_t R_BSP_EVENACCESS_SFR *)(DMACA_BASE_ADDR + DMACA_DMTMD_OFST \
                                                                                + (0x40 * dmaca_channel)))
/* DMA Interrupt Setting Register */
#define DMACA_DMINT(dmaca_channel)     (*(volatile uint8_t R_BSP_EVENACCESS_SFR  *)(DMACA_BASE_ADDR + DMACA_DMINT_OFST \
                                                                                + (0x40 * dmaca_channel)))
/* DMA Address Mode Register */
#define DMACA_DMAMD(dmaca_channel)     (*(volatile uint16_t R_BSP_EVENACCESS_SFR *)(DMACA_BASE_ADDR + DMACA_DMAMD_OFST \
                                                                                + (0x40 * dmaca_channel)))
/* DMA Offset Register (only for DMACA channe0)*/
#define DMACA_DMOFR                       (*(volatile uint32_t R_BSP_EVENACCESS_SFR *)(DMACA_BASE_ADDR + DMACA_DMOFR_OFST))
/* DMA Transfer Enable Register */
#define DMACA_DMCNT(dmaca_channel)     (*(volatile uint8_t R_BSP_EVENACCESS_SFR  *)(DMACA_BASE_ADDR + DMACA_DMCNT_OFST \
                                                                                + (0x40 * dmaca_channel)))
/* DMA Software Start Register */
#define DMACA_DMREQ(dmaca_channel)     (*(volatile uint8_t R_BSP_EVENACCESS_SFR  *)(DMACA_BASE_ADDR + DMACA_DMREQ_OFST \
                                                                                + (0x40 * dmaca_channel)))
/* DMA Status Register */
#define DMACA_DMSTS(dmaca_channel)     (*(volatile uint8_t R_BSP_EVENACCESS_SFR  *)(DMACA_BASE_ADDR + DMACA_DMSTS_OFST \
                                                                                + (0x40 * dmaca_channel)))
/* DMA Activation Source Flag Control Register */
#define DMACA_DMCSL(dmaca_channel)     (*(volatile uint8_t R_BSP_EVENACCESS_SFR  *)(DMACA_BASE_ADDR + DMACA_DMCSL_OFST \
                                                                                + (0x40 * dmaca_channel)))
/* DMACA Module Activation Register (for DMACA All channel) */
#define DMACA_DMAST                       (*(volatile uint8_t R_BSP_EVENACCESS_SFR  *)(DMACA_BASE_ADDR + DMACA_DMAST_OFST))
/* DMAC74 Interrupt Status Monitor Register (for DMACA channel 4 to 7) */
#define DMACA_DMIST                       (*(volatile uint8_t R_BSP_EVENACCESS_SFR  *)(DMACA_BASE_ADDR + DMACA_DMIST_OFST))


#endif /* DMACA_RX_TARGET_H */

/* End of File */

