/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
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
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : cellular_rts_ctrl.c
 * Description  : Function to control the RTS pin.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "cellular_private_api.h"
#include "at_command.h"
#include "cellular_freertos.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/

/************************************************************************
 * Function Name  @fn            cellular_rts_ctrl
 ***********************************************************************/
void cellular_rts_ctrl(const uint8_t lowhigh)
{
    CELLULAR_LOG_DEBUG(("RTS output %d\n", lowhigh));
    CELLULAR_SET_PODR(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN) = lowhigh;
}
/**********************************************************************************************************************
 * End of function cellular_rts_ctrl
 *********************************************************************************************************************/

#if CELLULAR_CFG_CTS_SW_CTRL == 1
/************************************************************************
 * Function Name  @fn            cellular_rts_hw_flow_enable
 ***********************************************************************/
void cellular_rts_hw_flow_enable(void)
{
    CELLULAR_LOG_DEBUG(("RTS HW flow enable\n"));

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);
    CELLULAR_SET_PODR(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN) = 0;
    CELLULAR_SET_PDR(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN) = 0;
    CELLULAR_SET_PFS(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN) = CELLULAR_CFG_PFS_SET_VALUE;
    CELLULAR_SET_PMR(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN) = 1;
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}
/**********************************************************************************************************************
 * End of function cellular_rts_hw_flow_enable
 *********************************************************************************************************************/

/************************************************************************
 * Function Name  @fn            cellular_rts_hw_flow_disable
 ***********************************************************************/
void cellular_rts_hw_flow_disable(void)
{
    CELLULAR_LOG_DEBUG(("RTS HW flow disable\n"));

    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);
    CELLULAR_SET_PFS(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN) = 0x00U;
    CELLULAR_SET_PMR(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN) = 0;
    CELLULAR_SET_PDR(CELLULAR_CFG_RTS_PORT, CELLULAR_CFG_RTS_PIN) = 1;
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}
/**********************************************************************************************************************
 * End of function cellular_rts_hw_flow_disable
 *********************************************************************************************************************/
#endif  /* CELLULAR_CFG_CTS_SW_CTRL == 1 */
