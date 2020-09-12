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
* Copyright (C) 2014(2015-2017) Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_dtc_rx_target.c
* Device       : RX24T
* Tool-Chain   : Renesas RXC Toolchain v2.06.00
* OS           : not use
* H/W Platform : not use
* Description  : Functions for using DTC on RX24T devices.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 17.03.2014 1.00    Initial revision
*         : 17.07.2014 2.00    Second  revision
*         : 12.11.2014 2.01    Added RX113.
*         : 30.01.2015 2.02    Added RX71M.
*         : 24.12.2015 2.04    Added RX130, RX23T and RX24T.
*         : 31.03.2017 2.07    Added RX24T-512KB.
*         : 31.07.2017 2.08    Fixed to correspond to Renesas coding rule.
*         : 25.11.2019 3.30    Added support for atomic control.
*******************************************************************************/

/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Defines for DTC support */
#include "r_dtc_rx_if.h"
#include ".\src\r_dtc_rx_private.h"

/* Check MCU Group */
#if defined(BSP_MCU_RX24T)

/*******************************************************************************
Exported global variables (to be accessed by other files)
*******************************************************************************/
/* The array of all interrupt source */
const dtc_activation_source_t g_source_array[DTC_NUM_INTERRUPT_SRC] =
{
#if (BSP_CFG_MCU_PART_MEMORY_SIZE == 0xB) || \
    (BSP_CFG_MCU_PART_MEMORY_SIZE == 0xC) || \
    (BSP_CFG_MCU_PART_MEMORY_SIZE == 0xE)
    /* RX24T-512KB */
    DTCE_ICU_SWINT,DTCE_CMT0_CMI0,DTCE_CMT1_CMI1,DTCE_CMT2_CMI2,
    DTCE_CMT3_CMI3,DTCE_RSPI0_SPRI0,DTCE_RSPI0_SPTI0,
    DTCE_GPT0_GTCIA0,DTCE_GPT0_GTCIB0,DTCE_GPT0_GTCIC0,DTCE_GPT0_GTCID0,
    DTCE_GPT0_GTCIE0,DTCE_GPT0_GTCIF0,DTCE_GPT0_GTCIV0,DTCE_GPT0_GTCIU0,
    DTCE_RSCAN_COMFRXINT,DTCE_ICU_IRQ0,DTCE_ICU_IRQ1,DTCE_ICU_IRQ2,DTCE_ICU_IRQ3,
    DTCE_ICU_IRQ4,DTCE_ICU_IRQ5,DTCE_ICU_IRQ6,DTCE_ICU_IRQ7,
    DTCE_GPT1_GTCIA1,DTCE_GPT1_GTCIB1,DTCE_GPT1_GTCIC1,DTCE_GPT1_GTCID1,
    DTCE_S12AD_S12ADI,DTCE_S12AD_GBADI,DTCE_S12AD_GCADI,
    DTCE_S12AD1_S12ADI1,DTCE_S12AD1_GBADI1,DTCE_S12AD1_GCADI1,
    DTCE_CMPC0_CMPC0,DTCE_CMPC1_CMPC1,DTCE_CMPC2_CMPC2,
    DTCE_S12AD2_S12ADI2,DTCE_S12AD2_GBADI2,DTCE_S12AD2_GCADI2,
    DTCE_MTU0_TGIA0,DTCE_MTU0_TGIB0,DTCE_MTU0_TGIC0,DTCE_MTU0_TGID0,
    DTCE_MTU1_TGIA1,DTCE_MTU1_TGIB1,DTCE_MTU2_TGIA2,DTCE_MTU2_TGIB2,
    DTCE_MTU3_TGIA3,DTCE_MTU3_TGIB3,DTCE_MTU3_TGIC3,DTCE_MTU3_TGID3,
    DTCE_MTU4_TGIA4,DTCE_MTU4_TGIB4,DTCE_MTU4_TGIC4,DTCE_MTU4_TGID4,DTCE_MTU4_TCIV4,
    DTCE_MTU5_TGIU5,DTCE_MTU5_TGIV5,DTCE_MTU5_TGIW5,
    DTCE_MTU6_TGIA6,DTCE_MTU6_TGIB6,DTCE_MTU6_TGIC6,DTCE_MTU6_TGID6,
    DTCE_MTU7_TGIA7,DTCE_MTU7_TGIB7,DTCE_MTU7_TGIC7,DTCE_MTU7_TGID7,DTCE_MTU7_TCIV7,
    DTCE_MTU9_TGIA9,DTCE_MTU9_TGIB9,DTCE_MTU9_TGIC9,DTCE_MTU9_TGID9,
    DTCE_CMPC3_CMPC3,DTCE_TMR0_CMIA0,DTCE_TMR0_CMIB0,
    DTCE_TMR1_CMIA1,DTCE_TMR1_CMIB1,DTCE_TMR2_CMIA2,DTCE_TMR2_CMIB2,
    DTCE_TMR3_CMIA3,DTCE_TMR3_CMIB3,DTCE_TMR4_CMIA4,DTCE_TMR4_CMIB4,
    DTCE_TMR5_CMIA5,DTCE_TMR5_CMIB5,DTCE_TMR6_CMIA6,DTCE_TMR6_CMIB6,
    DTCE_TMR7_CMIA7,DTCE_TMR7_CMIB7,
    DTCE_GPT1_GTCIE1,DTCE_GPT1_GTCIF1,DTCE_GPT1_GTCIV1,DTCE_GPT1_GTCIU1,
    DTCE_GPT2_GTCIA2,DTCE_GPT2_GTCIB2,DTCE_GPT2_GTCIC2,DTCE_GPT2_GTCID2,
    DTCE_GPT2_GTCIE2,DTCE_GPT2_GTCIF2,DTCE_GPT2_GTCIV2,DTCE_GPT2_GTCIU2,
    DTCE_GPT3_GTCIA3,DTCE_GPT3_GTCIB3,
    DTCE_SCI1_RXI1,DTCE_SCI1_TXI1,DTCE_SCI5_RXI5,DTCE_SCI5_TXI5,
    DTCE_SCI6_RXI6,DTCE_SCI6_TXI6,DTCE_SCI8_RXI8,DTCE_SCI8_TXI8,
    DTCE_SCI9_RXI9,DTCE_SCI9_TXI9,DTCE_GPT3_GTCIC3,DTCE_GPT3_GTCID3,
    DTCE_GPT3_GTCIE3,DTCE_GPT3_GTCIF3,DTCE_GPT3_GTCIV3,DTCE_GPT3_GTCIU3,
    DTCE_RIIC0_RXI0,DTCE_RIIC0_TXI0,
    DTCE_SCI11_RXI11,DTCE_SCI11_TXI11
#else
    /* RX24T-256KB */
    DTCE_ICU_SWINT, DTCE_CMT0_CMI0,DTCE_CMT1_CMI1, DTCE_CMT2_CMI2,DTCE_CMT3_CMI3, 
    DTCE_RSPI0_SPRI0, DTCE_RSPI0_SPTI0, DTCE_ICU_IRQ0, DTCE_ICU_IRQ1, DTCE_ICU_IRQ2,
    DTCE_ICU_IRQ3, DTCE_ICU_IRQ4, DTCE_ICU_IRQ5, DTCE_ICU_IRQ6, DTCE_ICU_IRQ7, 
    DTCE_S12AD_S12ADI, DTCE_S12AD_GBADI, DTCE_S12AD_GCADI, DTCE_S12AD1_S12ADI1,
    DTCE_S12AD1_GBADI1, DTCE_S12AD1_GCADI1, DTCE_CMPC0_CMPC0, DTCE_CMPC1_CMPC1,
    DTCE_CMPC2_CMPC2, DTCE_S12AD2_S12ADI2, DTCE_S12AD2_GBADI2, DTCE_S12AD2_GCADI2,
    DTCE_MTU0_TGIA0, DTCE_MTU0_TGIB0, DTCE_MTU0_TGIC0, DTCE_MTU0_TGID0, DTCE_MTU1_TGIA1,
    DTCE_MTU1_TGIB1, DTCE_MTU2_TGIA2, DTCE_MTU2_TGIB2, DTCE_MTU3_TGIA3, DTCE_MTU3_TGIB3,
    DTCE_MTU3_TGIC3, DTCE_MTU3_TGID3, DTCE_MTU4_TGIA4, DTCE_MTU4_TGIB4, DTCE_MTU4_TGIC4,
    DTCE_MTU4_TGID4, DTCE_MTU4_TCIV4, DTCE_MTU5_TGIU5, DTCE_MTU5_TGIV5, DTCE_MTU5_TGIW5,
    DTCE_MTU6_TGIA6, DTCE_MTU6_TGIB6, DTCE_MTU6_TGIC6, DTCE_MTU6_TGID6, DTCE_MTU7_TGIA7,
    DTCE_MTU7_TGIB7, DTCE_MTU7_TGIC7, DTCE_MTU7_TGID7, DTCE_MTU7_TCIV7, DTCE_MTU9_TGIA9,
    DTCE_MTU9_TGIB9, DTCE_MTU9_TGIC9, DTCE_MTU9_TGID9, DTCE_CMPC3_CMPC3, DTCE_TMR0_CMIA0,
    DTCE_TMR0_CMIB0, DTCE_TMR1_CMIA1, DTCE_TMR1_CMIB1, DTCE_TMR2_CMIA2, DTCE_TMR2_CMIB2,
    DTCE_TMR3_CMIA3, DTCE_TMR3_CMIB3, DTCE_TMR4_CMIA4, DTCE_TMR4_CMIB4, DTCE_TMR5_CMIA5,
    DTCE_TMR5_CMIB5, DTCE_TMR6_CMIA6, DTCE_TMR6_CMIB6, DTCE_TMR7_CMIA7, DTCE_TMR7_CMIB7,
    DTCE_SCI1_RXI1, DTCE_SCI1_TXI1, DTCE_SCI5_RXI5, DTCE_SCI5_TXI5, DTCE_SCI6_RXI6,
    DTCE_SCI6_TXI6, DTCE_RIIC0_RXI0, DTCE_RIIC0_TXI0
#endif
};


#if ((0 != BSP_CFG_USER_LOCKING_ENABLED) || (bsp_lock_t != BSP_CFG_USER_LOCKING_TYPE) \
      || (DTC_ENABLE != DTC_CFG_USE_DMAC_FIT_MODULE))
/*******************************************************************************
* Function Name: r_dtc_check_DMAC_locking_byUSER
* Description  : Checks all DMAC channel locking.
* Arguments    : none -
* Return Value : true -
*                    All DMAC channels are unlocked. 
*                false -
*                    One or some DMAC channels are locked.
*
*******************************************************************************/
bool r_dtc_check_DMAC_locking_byUSER(void)
{
    bool ret = true;

    /* User has to check the locking of DMAC by themselves. */
    /* do something */

    return ret;
}
#endif


/*******************************************************************************
* Function Name: r_dtc_module_enable
* Description  : Releases module stop state.
* Arguments    : None
* Return Value : None
*******************************************************************************/
void r_dtc_module_enable(void)
{
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
bsp_int_ctrl_t int_ctrl;
#endif
    /* Enable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    /* Release from module stop state. */
    MSTP(DTC) = 0;

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    /* Disable writing to MSTP registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    return;
}
/******************************************************************************
 End of function r_dtc_module_enable
 *****************************************************************************/

/*******************************************************************************
* Function Name: r_dtc_module_disable
* Description  : Sets to module stop state.
* Arguments    : None
* Return Value : None
*******************************************************************************/
void r_dtc_module_disable(void)
{
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
bsp_int_ctrl_t int_ctrl;
#endif
    /* Enable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    /* Set to module stop state. */
    MSTP(DTC) = 1;

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    /* Disable writing to MSTP registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    return;
}
/******************************************************************************
 End of function r_dtc_module_disable
 *****************************************************************************/


#endif /* defined(BSP_MCU_RX24T) */

/* End of File */

