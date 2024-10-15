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
* Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_dtc_rx_target.c
* Description  : Functions for using DTC on RX260 devices.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 28.06.2024 1.00    First Release
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Defines for DTC support */
#include "r_dtc_rx_if.h"
#include "./src/r_dtc_rx_private.h"

/* Check MCU Group */
#if defined(BSP_MCU_RX260)

/*******************************************************************************
Exported global variables (to be accessed by other files)
*******************************************************************************/
/* The array of all interrupt source */
const dtc_activation_source_t g_source_array[DTC_NUM_INTERRUPT_SRC] =
{
    DTCE_ICU_SWINT,
    DTCE_CMT0_CMI0,DTCE_CMT1_CMI1,DTCE_CMT2_CMI2,DTCE_CMT3_CMI3,
    DTCE_USB0_D0FIFO0,DTCE_USB0_D1FIFO0,
    DTCE_RSPI0_SPRI0,DTCE_RSPI0_SPTI0,
    DTCE_CMPB_CMPB0,DTCE_CMPB_CMPB1,
    DTCE_CTSU_CTSUWR,DTCE_CTSU_CTSURD,
    DTCE_ICU_IRQ0,DTCE_ICU_IRQ1,DTCE_ICU_IRQ2,DTCE_ICU_IRQ3,DTCE_ICU_IRQ4,
    DTCE_ICU_IRQ5,DTCE_ICU_IRQ6,DTCE_ICU_IRQ7,
    DTCE_S12AD_S12ADI0,DTCE_S12AD_GBADI,
    DTCE_ELC_ELSR18I,DTCE_ELC_ELSR19I,
    DTCE_GPTW0_GTCIA0,DTCE_GPTW0_GTCIB0,DTCE_GPTW0_GTCIC0,DTCE_GPTW0_GTCID0,
    DTCE_GPTW0_GTCIE0,DTCE_GPTW0_GTCIF0,DTCE_GPTW0_GTCIV0,DTCE_GPTW0_GTCIU0,
    DTCE_GPTW1_GTCIA1,DTCE_GPTW1_GTCIB1,DTCE_GPTW1_GTCIC1,DTCE_GPTW1_GTCID1,
    DTCE_GPTW1_GTCIE1,DTCE_GPTW1_GTCIF1,DTCE_GPTW1_GTCIV1,DTCE_GPTW1_GTCIU1,
    DTCE_GPTW2_GTCIA2,DTCE_GPTW2_GTCIB2,DTCE_GPTW2_GTCIC2,DTCE_GPTW2_GTCID2,
    DTCE_GPTW2_GTCIE2,DTCE_GPTW2_GTCIF2,DTCE_GPTW2_GTCIV2,DTCE_GPTW2_GTCIU2,
    DTCE_GPTW3_GTCIA3,DTCE_GPTW3_GTCIB3,DTCE_GPTW3_GTCIC3,DTCE_GPTW3_GTCID3,
    DTCE_GPTW3_GTCIE3,DTCE_GPTW3_GTCIF3,DTCE_GPTW3_GTCIV3,DTCE_GPTW3_GTCIU3,
    DTCE_GPTW4_GTCIA4,DTCE_GPTW4_GTCIB4,DTCE_GPTW4_GTCIC4,DTCE_GPTW4_GTCID4,
    DTCE_GPTW4_GTCIE4,DTCE_GPTW4_GTCIF4,DTCE_GPTW4_GTCIV4,DTCE_GPTW4_GTCIU4,
    DTCE_GPTW5_GTCIA5,DTCE_GPTW5_GTCIB5,DTCE_GPTW5_GTCIC5,DTCE_GPTW5_GTCID5,
    DTCE_GPTW5_GTCIE5,DTCE_GPTW5_GTCIF5,DTCE_GPTW5_GTCIV5,DTCE_GPTW5_GTCIU5,
    DTCE_GPTW6_GTCIA6,DTCE_GPTW6_GTCIB6,DTCE_GPTW6_GTCIC6,DTCE_GPTW6_GTCID6,
    DTCE_GPTW6_GTCIE6,DTCE_GPTW6_GTCIF6,DTCE_GPTW6_GTCIV6,DTCE_GPTW6_GTCIU6,
    DTCE_GPTW7_GTCIA7,DTCE_GPTW7_GTCIB7,DTCE_GPTW7_GTCIC7,DTCE_GPTW7_GTCID7,
    DTCE_GPTW7_GTCIE7,DTCE_GPTW7_GTCIF7,DTCE_GPTW7_GTCIV7,DTCE_GPTW7_GTCIU7,
    DTCE_TMR0_CMIA0,DTCE_TMR0_CMIB0,
    DTCE_TMR1_CMIA1,DTCE_TMR1_CMIB1,
    DTCE_TMR2_CMIA2,DTCE_TMR2_CMIB2,
    DTCE_TMR3_CMIA3,DTCE_TMR3_CMIB3,
    DTCE_CANFD_RFDREQ0,DTCE_CANFD_RFDREQ1,DTCE_CANFD0_CFDREQ0,
    DTCE_DMAC_DMAC0I,
    DTCE_DMAC_DMAC1I,DTCE_DMAC_DMAC2I,DTCE_DMAC_DMAC3I,
    DTCE_RSIP_WRRDY0,DTCE_RSIP_WRRDY2,DTCE_RSIP_RDRDY0,
    DTCE_RSIP_INTEGRATE_RDRDY,DTCE_RSIP_INTEGRATE_WRRDY,
    DTCE_RSCI9_AED,DTCE_RSCI0_RXI,DTCE_RSCI0_TXI,
    DTCE_SCI1_RXI1,DTCE_SCI1_TXI1,
    DTCE_SCI5_RXI5,DTCE_SCI5_TXI5,
    DTCE_SCI6_RXI6,DTCE_SCI6_TXI6,
    DTCE_RSCI8_RXI,DTCE_RSCI8_TXI,
    DTCE_RSCI9_RXI,DTCE_RSCI9_TXI,
    DTCE_SCI12_RXI12,DTCE_SCI12_TXI12,
    DTCE_RIIC0_RXI0,DTCE_RIIC0_TXI0,
    DTCE_LPT_LPTCMI1
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


#endif /* defined(BSP_MCU_RX260) */

/* End of File */

