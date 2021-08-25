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
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 31.03.2021 1.00     First Release
*******************************************************************************/
/*******************************************************************************
* File Name    : r_qspix_rx671.c
* Description  : Functions for using QSPIX on RX devices.
*******************************************************************************/
/* Access to peripherals and board defines. */
#include "platform.h"

#ifdef BSP_MCU_RX671

/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Defines for QSPIX support */
#include "r_qspix_rx_private.h"
#include "r_qspix_rx671_private.h"
#include "r_qspix_rx_if.h"
#include "r_qspix_rx_config.h"

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
qspix_hdl_t g_qspix_handles[QSPIX_NUM_CH];

#if QSPIX_CFG_ERI_INCLUDED
static void r_qspix_eri_grp_isr(void *pdata);
#endif

/*----------------------------------------------------------------------------*/
/*  internal functions                                                        */
/*----------------------------------------------------------------------------*/

/************************************************************************************
* Function Name: r_qspix_parameter_channel_check
* Description  : Checks channel parameter.
* Arguments    : channel
* Return Value : QSPIX_SUCCESS              Successful operation
*              ; QSPIX_ERR_INVALID_ARG      Parameter error
************************************************************************************/
qspix_err_t r_qspix_parameter_channel_check(uint8_t channel)
{
    qspix_err_t ret = QSPIX_SUCCESS;

    /* ---- Check channel argument. ---- */
        if (QSPIX_CH0 != channel)
        {
            return QSPIX_ERR_INVALID_ARG;
        }
    return ret;
}
/******************************************************************************
End of function r_qspix_parameter_channel_check
******************************************************************************/

/************************************************************************************
* Function Name: r_qspix_parameter_check
* Description  : Checks parameter.
* Arguments    : channel
*                p_cfg
* Return Value : QSPIX_SUCCESS              Processing completed without problem
*              : QSPIX_ERR_INVALID_ARG      Invalid argument
*              ; QSPIX_ERR_NULL_PTR         Argument pointers are NULL
************************************************************************************/
qspix_err_t r_qspix_parameter_check(uint8_t channel, qspix_cfg_t * p_cfg)
{
    qspix_err_t ret = QSPIX_SUCCESS;

    /* ---- Check channel argument. ---- */
    ret = r_qspix_parameter_channel_check(channel);

    if(QSPIX_SUCCESS != ret)
    {
        return ret;
    }

    /* ---- Check p_cfg argument. ---- */
    if(NULL == p_cfg)
    {
        return QSPIX_ERR_NULL_PTR;
    }
    
#if QSPIX_CFG_ERI_INCLUDED
    /* Call the user callback function passing pointer to the result structure. */
    if ((FIT_NO_FUNC == p_cfg->p_callback) || (NULL == p_cfg->p_callback))
    {
        return QSPIX_ERR_INVALID_ARG;
    }
#endif   

    /* ---- Check address_size of p_cfg argument. ---- */
    if (QSPIX_4_BYTES != p_cfg->address_size)
    {
        if (QSPIX_INSTRUCTION_4_BYTE_ADDRESS_ENABLE ==
                                            p_cfg->instruction_4_Byte_address)
        {
            return QSPIX_ERR_INVALID_ARG;
        }
    }

    /* ---- Check data_output_select of p_cfg argument. ---- */
    if (QSPIX_EXTEND_BY_1_QSPCLK == p_cfg->data_output_select)
    {
        if (QSPIX_3_QSPCLK == p_cfg->dummy_clocks)
        {
            return QSPIX_ERR_INVALID_ARG;
        }
    }

    return ret;
}
/******************************************************************************
End of function r_qspix_parameter_check
******************************************************************************/

/************************************************************************************
* Function Name: r_qspix_enable
* Description  : Enable QSPIX.
* Arguments    : channel
*                p_cfg
* Return Value : QSPIX_SUCCESS              Processing completed without problem
*              : QSPIX_ERR_OPENED           QSPIX was initialized already
*              : QSPIX_ERR_INVALID_ARG      Invalid argument
*              : QSPIX_ERR_NULL_PTR         Argument pointers are NULL
*              : QSPIX_ERR_BUSY             The QSPIX resources are locked by another process
************************************************************************************/
qspix_err_t r_qspix_enable(uint8_t channel, qspix_cfg_t * p_cfg)
{
    qspix_err_t ret = QSPIX_SUCCESS;
    
#if QSPIX_CFG_ERI_INCLUDED
    bsp_int_ctrl_t int_ctrl_data;

    /* Set interrupt priority */
    int_ctrl_data.ipl = (uint32_t)QSPIX_CFG_ERI_IR_PRIORITY;
#endif

#if QSPIX_CFG_PARAM_CHECKING_ENABLE
    if(QSPIX_OPEN == g_qspix_handles[channel].open)
    {
        return QSPIX_ERR_OPENED;
    }
#endif
    /* Check hw lock require */
    if (false == R_BSP_HardwareLock((mcu_lock_t)(BSP_LOCK_QSPIX)))
    {
        /* Lock has already been acquired by another task. Need to try again later. */
      return QSPIX_ERR_BUSY;
    }

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
bsp_int_ctrl_t int_ctrl;
#endif
    /* Enable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    /* Release from module stop state. */
    MSTP(QSPIX) = 0;

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif

    /* Disable writing to MSTP registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if QSPIX_CFG_ERI_INCLUDED
    /* Enable the group interrupt for the AL0 interrupt. (sets priority also). */
    R_BSP_InterruptControl(BSP_INT_SRC_AL0_QSPIX_ERI, BSP_INT_CMD_GROUP_INTERRUPT_ENABLE, &int_ctrl_data);

    /* Register the error callback function with the BSP group interrupt handler. */
    R_BSP_InterruptWrite(BSP_INT_SRC_AL0_QSPIX_ERI, (bsp_int_cb_t)r_qspix_eri_grp_isr);

    #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
    #endif

    /* Enable error interrupt source bit */
    ICU.GENAL0.BIT.EN30 = 1;

    #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
    #endif

    /* Dummy read */
    if (0 == ICU.GENAL0.BIT.EN30)
    {
        R_BSP_NOP();
    }
#endif

    /* Set for I/O Mode Select */
    QSPIX.SPMR2.BIT.IOMOD = p_cfg->protocol;

    /* Set for Slave Select Extension */
    QSPIX.SPMR0.BIT.SSE = p_cfg->slave_select_extension;

    /* Set for Prefetch Function Enable */
    QSPIX.SPMR0.BIT.PFE = p_cfg->prefetch_function;

    /* Set for Clock Mode Select */
    QSPIX.SPMR0.BIT.CKMD = p_cfg->clock_mode;

    /* Set for Data Output Duration Extension */
    QSPIX.SPMR0.BIT.DODE = p_cfg->data_output_select;

    /* Set for Special Read Instruction Select */
    QSPIX.SPMR0.BIT.SRIS = p_cfg->special_instruction_select;

    /* Set for Special Read Instruction Setting Register */
    if (QSPIX_INSTRUCTION_CODE_IN_SPRIR_REGISTER ==
                                            p_cfg->special_instruction_select)
    {
        /* Set for Special Read Instruction Setting Register */
        QSPIX.SPRIR = p_cfg->qspix_special_read_instruction;
    }
    else
    {
        /* Set for Read Instruction Select */
        QSPIX.SPMR0.BIT.RISEL = p_cfg->read_mode;
    }

    /* Set for Slave Select High Width Setting */
    QSPIX.SPSSCR.BIT.SSHW = p_cfg->slave_select_high_width;

    /* Set for Slave Select Hold Time Setting */
    QSPIX.SPSSCR.BIT.SSHLD = p_cfg->slave_select_hold_time;

    /* Set for Slave Select Setup Time Setting */
    QSPIX.SPSSCR.BIT.SSSU = p_cfg->slave_select_setup_time;

    /* Set for Clock Divisor Select */
    QSPIX.SPOCR.BIT.DIV = p_cfg->clock_divisor;

    /* Set for combination of Duty with Clock divisor  */
    if(((uint8_t)(p_cfg->clock_divisor & 0x01)) && (p_cfg->clock_divisor < QSPIX_ICLK_DIV_18))
    {
        /* Set for Duty Cycle Correction when the divisor is odd number */
        QSPIX.SPOCR.BIT.DUTY  = QSPIX_CORRECT_THE_DUTY_CYCLE;
    }
    else
    {
        /* Disable Duty Cycle Correction when the divisor is even number */
        QSPIX.SPOCR.BIT.DUTY  = QSPIX_MAKE_NO_CORRECTION;
    }

    /* Set for Memory mode */
    QSPIX.SPMR1.BIT.AMOD = QSPIX_MEMORY_MAPPED_MODE;

    /* Set for Address Size Setting */
    QSPIX.SPAMR.BIT.SIZE = p_cfg->address_size;

    /* Check combination for Address Size vs Instruction with 4-Byte Address enable*/
    if (QSPIX_4_BYTES == p_cfg->address_size)
    {
        /* Set for Instruction with 4-Byte Address Enable */
        QSPIX.SPAMR.BIT.I4AE = p_cfg->instruction_4_Byte_address;
    }

    /* Set for Number of Dummy Cycle */
    QSPIX.SPDCR.BIT.DCYC = p_cfg->dummy_clocks;


    /* Set for WP Pin Control */
    QSPIX.SPPCR.BIT.WP = p_cfg->WP_pin_control;

    /* Store to global variable */
   g_qspix_handles[channel].open = QSPIX_OPEN;
   g_qspix_handles[channel].p_cfg = *p_cfg;

    return ret;
}
/******************************************************************************
End of function r_qspix_enable
******************************************************************************/

/************************************************************************************
* Function Name: r_qspix_disable
* Description  : Enable QSPIX.
* Arguments    : channel
*                p_cfg
* Return Value : QSPIX_SUCCESS              Processing completed without problem
*              : QSPIX_ERR_NOT_OPEN         QSPIX module is not initialized yet
*              : QSPIX_ERR_INVALID_ARG      Invalid argument
************************************************************************************/
qspix_err_t r_qspix_disable(uint8_t channel)
{
    qspix_err_t ret = QSPIX_SUCCESS;
    qspix_cfg_t p_cfg;
#if QSPIX_CFG_PARAM_CHECKING_ENABLE
    if(QSPIX_CLOSE == g_qspix_handles[channel].open)
    {
        return QSPIX_ERR_NOT_OPEN;
    }

#endif
    /* Unlock hw */
    R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_QSPIX));

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
bsp_int_ctrl_t int_ctrl;
#endif
    /* Enable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    /* Set to module stop state. */
    MSTP(QSPIX) = 1;

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    /* Disable writing to MSTP registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* De-register the error callback function with the BSP group interrupt handler. */
    R_BSP_InterruptWrite(BSP_INT_SRC_AL0_QSPIX_ERI, FIT_NO_FUNC);

#if QSPIX_CFG_ERI_INCLUDED
    /* Disable error interrupt source bit */
    #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
    #endif

    /* Disable error interrupt source bit */
    ICU.GENAL0.BIT.EN30 = 0;

    #if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
    #endif

    /* Dummy read */
    if (0 == ICU.GENAL0.BIT.EN30)
    {
        R_BSP_NOP();
    }
#endif
    g_qspix_handles[channel].open = QSPIX_CLOSE;
    
    /* Reset value read_mode */
    p_cfg.read_mode = QSPIX_READ_MODE_STANDARD;
    
    /* Reset value slave_select_extension */
    p_cfg.slave_select_extension = QSPIX_DO_NOT_EXTEND_QSSL;
    
    /* Reset value prefetch_function */
    p_cfg.prefetch_function = QSPIX_PREFETCH_DISABLE;
    
    /* Reset value clock_mode */
    p_cfg.clock_mode = QSPIX_SPI_MODE_0;
    
    /* Reset value data_output_select */
    p_cfg.data_output_select = QSPIX_DO_NOT_EXTEND;
    
    /* Reset value special_instruction_select */
    p_cfg.special_instruction_select = QSPIX_DEFAULT_INSTRUCTION_CODE;
    
    /* Reset value slave_select_high_width */
    p_cfg.slave_select_high_width = QSPIX_1_CYCLE_QSPCLK;
    
    /* Reset value slave_select_hold_time */
    p_cfg.slave_select_hold_time = QSPIX_RELEASE_QSSL_0_5_QSPCLK;
    
    /* Reset value slave_select_setup_time */
    p_cfg.slave_select_setup_time = QSPIX_OUTPUT_QSSL_0_5_QSPCLK;
    
    /* Reset value clock_divisor */
    p_cfg.clock_divisor = QSPIX_ICLK_DIV_2;
    
    /* Reset value address_size */
    p_cfg.address_size = QSPIX_1_BYTE;
    
    /* Reset value instruction_4_Byte_address */
    p_cfg.instruction_4_Byte_address = QSPIX_INSTRUCTION_4_BYTE_ADDRESS_DISABLE;
    
    /* Reset value dummy_clocks */
    p_cfg.dummy_clocks = QSPIX_DEFAULT_DUMMY_CYCLES;
    
    /* Reset value protocol */
    p_cfg.protocol = QSPIX_EXTENDED_SPI_PROTOCOL;
    
    /* Reset value WP_pin_control */
    p_cfg.WP_pin_control = QSPIX_LOW_LEVEL;
    
    /* Reset value p_callback */
    p_cfg.p_callback = 0;
    
    /* Reset value p_cfg */
    g_qspix_handles[channel].p_cfg = p_cfg;

    return ret;
}
/******************************************************************************
End of function r_qspix_disable
******************************************************************************/

/************************************************************************************
* Function Name: r_qspix_control
* Description  : Control QSPIX.
* Arguments    : channel
*                p_cfg
* Return Value : QSPIX_SUCCESS              Processing completed without problem
*              : QSPIX_ERR_NOT_OPEN         QSPIX module is not initialized yet
*              : QSPIX_ERR_INVALID_ARG      Invalid argument
*              : QSPIX_ERR_NULL_PTR         Argument pointers are NULL
*              : QSPIX_ERR_BUSY             The QSPIX resources are locked by another process
************************************************************************************/
qspix_err_t r_qspix_control(uint8_t channel, qspix_cfg_t * p_cfg)
{
    qspix_err_t ret = QSPIX_SUCCESS;

#if QSPIX_CFG_PARAM_CHECKING_ENABLE
    if(QSPIX_CLOSE == g_qspix_handles[channel].open)
    {
        return QSPIX_ERR_NOT_OPEN;
    }
#endif
    /* Set for I/O Mode Select */
    QSPIX.SPMR2.BIT.IOMOD = p_cfg->protocol;

    /* Set for Slave Select Extension */
    QSPIX.SPMR0.BIT.SSE = p_cfg->slave_select_extension;

    /* Set for Prefetch Function Enable */
    QSPIX.SPMR0.BIT.PFE = p_cfg->prefetch_function;

    /* Set for Clock Mode Select */
    QSPIX.SPMR0.BIT.CKMD = p_cfg->clock_mode;

    /* Set for Data Output Duration Extension */
    QSPIX.SPMR0.BIT.DODE = p_cfg->data_output_select;

    /* Set for Special Read Instruction Select */
    QSPIX.SPMR0.BIT.SRIS = p_cfg->special_instruction_select;

    /* Set for Special Read Instruction Setting Register */
    if (QSPIX_INSTRUCTION_CODE_IN_SPRIR_REGISTER ==
                                            p_cfg->special_instruction_select)
    {
        /* Set for Special Read Instruction Setting Register */
        QSPIX.SPRIR = p_cfg->qspix_special_read_instruction;
    }
    else
    {
        /* Set for Read Instruction Select */
        QSPIX.SPMR0.BIT.RISEL = p_cfg->read_mode;
    }

    /* Set for Slave Select High Width Setting */
    QSPIX.SPSSCR.BIT.SSHW = p_cfg->slave_select_high_width;

    /* Set for Slave Select Hold Time Setting */
    QSPIX.SPSSCR.BIT.SSHLD = p_cfg->slave_select_hold_time;

    /* Set for Slave Select Setup Time Setting */
    QSPIX.SPSSCR.BIT.SSSU = p_cfg->slave_select_setup_time;

    /* Set for Clock Divisor Select */
    QSPIX.SPOCR.BIT.DIV = p_cfg->clock_divisor;

    /* Set for combination of Duty with Clock divisor  */
    if(((uint8_t)(p_cfg->clock_divisor & 0x01)) && (p_cfg->clock_divisor < QSPIX_ICLK_DIV_18))
    {
        /* Set for Duty Cycle Correction when the divisor is odd number */
        QSPIX.SPOCR.BIT.DUTY  = QSPIX_CORRECT_THE_DUTY_CYCLE;
    }
    else
    {
        /* Disable Duty Cycle Correction when the divisor is even number */
        QSPIX.SPOCR.BIT.DUTY  = QSPIX_MAKE_NO_CORRECTION;
    }

    /* Set for Memory mode */
    QSPIX.SPMR1.BIT.AMOD = QSPIX_MEMORY_MAPPED_MODE;

    /* Set for Address Size Setting */
    QSPIX.SPAMR.BIT.SIZE = p_cfg->address_size;

    /* Check combination for Address Size vs Instruction with 4-Byte Address
Enable*/
    if (QSPIX_4_BYTES == p_cfg->address_size)
    {
        /* Set for Instruction with 4-Byte Address Enable */
        QSPIX.SPAMR.BIT.I4AE = p_cfg->instruction_4_Byte_address;
    }

    /* Set for Number of Dummy Cycle */
    QSPIX.SPDCR.BIT.DCYC = p_cfg->dummy_clocks;

    /* Set for WP Pin Control */
    QSPIX.SPPCR.BIT.WP = p_cfg->WP_pin_control;

    g_qspix_handles[channel].p_cfg = *p_cfg;

    return ret;
}
/******************************************************************************
End of function r_qspix_control
******************************************************************************/

/************************************************************************************
* Function Name: r_qspix_xip
* Description  : Enter or exit XIP mode QSPIX.
* Arguments    : channel
*                mode
*                action
* Return Value : QSPIX_SUCCESS              Processing completed without problem
*              : QSPIX_ERR_HW               HW error
************************************************************************************/
qspix_err_t r_qspix_xip(uint8_t channel, uint8_t mode, bool action)
{
    qspix_err_t ret = QSPIX_SUCCESS;
    volatile uint8_t uc_temp = 0;
#if QSPIX_CFG_PARAM_CHECKING_ENABLE
    ret = r_qspix_parameter_channel_check(channel);
    if(QSPIX_SUCCESS != ret)
    {
        return ret;
    }

    if(QSPIX_CLOSE == g_qspix_handles[channel].open)
    {
        return QSPIX_ERR_NOT_OPEN;
    }
#endif
    if(action)
    {
        /* Set mode for XIP */
        QSPIX.SPDCR.BIT.MODE = mode;

        /* Enter to XIP */
        QSPIX.SPDCR.BIT.XIPE = QSPIX_XIP_ENABLE;

        /*Read data from memory map to trigger XIP mode.*/
        uc_temp = *(volatile uint8_t *)QSPI_DEVICE_START_ADDRESS;

        /* Read 1 from the XIPS bit in the SPDCR register */
        if(QSPIX_XIP_ENABLE != QSPIX.SPDCR.BIT.XIPS)
        {
            ret = QSPIX_ERR_HW;
        }
    }
    else
    {
        /* Set mode for XIP */
        QSPIX.SPDCR.BIT.MODE = mode;

        /* Exit to XIP */
        QSPIX.SPDCR.BIT.XIPE = QSPIX_XIP_DISABLE;

        /*Read data from memory map to trigger XIP mode.*/
        uc_temp = *(volatile uint8_t *)QSPI_DEVICE_START_ADDRESS;

        /* Read 0 from the XIPS bit in the SPDCR register */
        if(QSPIX_XIP_DISABLE != QSPIX.SPDCR.BIT.XIPS)
        {
            ret = QSPIX_ERR_HW;
        }
    }
    return ret;
}
/******************************************************************************
End of function r_qspix_xip
******************************************************************************/

/************************************************************************************
* Function Name: r_qspix_get_status
* Description  : Get status.
* Arguments    : cmd
*                return_status
* Return Value : QSPIX_SUCCESS              Processing completed without problem
*              : QSPIX_ERR_INVALID_COMMAND  Command parameters are invalid.
************************************************************************************/
qspix_err_t r_qspix_get_status(qspix_cmd_t cmd, uint8_t *return_status)
{
    qspix_err_t ret = QSPIX_SUCCESS;
#if QSPIX_CFG_PARAM_CHECKING_ENABLE
if (NULL == return_status)
    {
        return QSPIX_ERR_NULL_PTR;
    }
#endif
    /* ---- Check channel argument. ---- */
    switch(cmd)
    {
        case QSPIX_GET_PREFETCH_BUFFER_FILL_LEVEL:

            /* Return prefetch buffer fill level */
            *return_status = (uint8_t)QSPIX.SPPFSR.BIT.PBLVL;
            break;

        case QSPIX_GET_PREFETCH_BUFFER_FULL_FLAG:

        /* Return prefetch buffer full flag */
            *return_status = (uint8_t)QSPIX.SPPFSR.BIT.PBFUL;
            break;

        case QSPIX_GET_PREFETCH_FUNCTION_OPERATING_STATUS_FLAG:

        /* Return prefetch operating status flag */
            *return_status = (uint8_t)QSPIX.SPPFSR.BIT.PFOSF;
            break;

        case QSPIX_GET_BUS_BUSY_FLAG:

        /* Return bus busy flag */
            *return_status = (uint8_t)QSPIX.SPSR.BIT.BUSY;
            break;

        case QSPIX_GET_ROM_ACCESS_ERROR_FLAG:

        /* Return ROM access error flag */
            *return_status = (uint8_t)QSPIX.SPSR.BIT.ROMAE;
            break;

        case QSPIX_GET_XIP_STATUS_FLAG:

        /* Return XIP status flag */
            *return_status = (uint8_t)QSPIX.SPDCR.BIT.XIPS;
            break;
        default:

        /* Invalid command */
            ret = QSPIX_ERR_INVALID_COMMAND;
            break;

    }
    return ret;
}
/******************************************************************************
End of function r_qspix_get_status
******************************************************************************/

/************************************************************************************
* Function Name: r_qspix_read_indirect
* Description  : Read raw data.
* Arguments    : channel
*                p_des_addr
*                bytes
* Return Value : QSPIX_SUCCESS              Processing completed without problem
*              : QSPIX_ERR_NOT_OPEN         QSPIX module is not initialized yet
*              : QSPIX_ERR_INVALID_ARG      Invalid argument
*              : QSPIX_ERR_NULL_PTR         Argument pointers are NULL
************************************************************************************/
qspix_err_t r_qspix_read_indirect(uint8_t channel,
                                  uint8_t *p_des_addr,
                                  uint32_t bytes)
{
    qspix_err_t ret = QSPIX_SUCCESS;

#if QSPIX_CFG_PARAM_CHECKING_ENABLE
    ret = r_qspix_parameter_channel_check(channel);
    if(QSPIX_SUCCESS != ret)
    {
        return ret;
    }
    if(NULL == p_des_addr)
    {
       return QSPIX_ERR_NULL_PTR;
    }

    if(QSPIX_CLOSE == g_qspix_handles[channel].open)
    {
        return QSPIX_ERR_NOT_OPEN;
    }
#endif
    /* Read data from QSPI. */
    for (uint32_t i = 0; i < bytes; i++)
    {
        /* Store data to p_des_addr */
        p_des_addr[i] = (uint8_t) QSPIX.SPDR;
    }

    /* Close bus cycle process */
    QSPIX.SPMR1.BIT.AMOD = QSPIX_INDIRECT_ACCESS_MODE;

    /* Close bus cycle process */
    QSPIX.SPMR1.BIT.AMOD = QSPIX_MEMORY_MAPPED_MODE;

    /* Set protocol */
    QSPIX.SPMR2.BIT.IOMOD = g_qspix_handles[channel].p_cfg.protocol;
    return ret;
}
/******************************************************************************
End of function r_qspix_read_indirect
******************************************************************************/

/************************************************************************************
* Function Name: r_qspix_write_indirect
* Description  : Write raw data.
* Arguments    : channel
*                p_src_addr
*                bytes
*                read_after_write
* Return Value : QSPIX_SUCCESS              Processing completed without problem
*              : QSPIX_ERR_NOT_OPEN         QSPIX module is not initialized yet
*              : QSPIX_ERR_INVALID_ARG      Invalid argument
*              : QSPIX_ERR_NULL_PTR         Argument pointers are NULL
************************************************************************************/
qspix_err_t r_qspix_write_indirect(uint8_t channel,
                                   uint8_t *p_src_addr,
                                   uint32_t bytes,
                                   bool read_after_write)
{
    qspix_err_t ret = QSPIX_SUCCESS;

#if QSPIX_CFG_PARAM_CHECKING_ENABLE
    ret = r_qspix_parameter_channel_check(channel);
    if(QSPIX_SUCCESS != ret)
    {
        return ret;
    }
    if(NULL == p_src_addr)
    {
       return QSPIX_ERR_NULL_PTR;
    }
    if(QSPIX_CLOSE == g_qspix_handles[channel].open)
    {
        return QSPIX_ERR_NOT_OPEN;
    }
#endif

    /* Switch to indirect access mode */
    QSPIX.SPMR1.BIT.AMOD = QSPIX_INDIRECT_ACCESS_MODE;
    QSPIX.SPMR2.BIT.IOMOD = QSPIX_EXTENDED_SPI_PROTOCOL;

    /* Write data to QSPI. */
    for (uint32_t i = 0; i < bytes; i++)
    {
        /* write data from p_src_addr to spi bus */
        QSPIX.SPDR = p_src_addr[i];
    }

    if (!read_after_write)
    {

        /* Close bus cycle process */
        QSPIX.SPMR1.BIT.AMOD = QSPIX_INDIRECT_ACCESS_MODE;

        /* Close bus cycle process */
        QSPIX.SPMR1.BIT.AMOD = QSPIX_MEMORY_MAPPED_MODE;
        
        /* Set protocol */
        QSPIX.SPMR2.BIT.IOMOD = g_qspix_handles[channel].p_cfg.protocol;
    }
    return ret;
}
/******************************************************************************
End of function r_qspix_write_indirect
******************************************************************************/

/************************************************************************************
* Function Name: r_qspix_set_spi_protocol
* Description  : Set SPI protocol.
* Arguments    : channel
*                protocol
* Return Value : QSPIX_SUCCESS              Processing completed without problem
*              : QSPIX_ERR_NOT_OPEN         QSPIX module is not initialized yet
*              : QSPIX_ERR_INVALID_ARG      Invalid argument
************************************************************************************/
qspix_err_t r_qspix_set_spi_protocol(uint8_t channel, qspix_protocol_t protocol)
{
    qspix_err_t ret = QSPIX_SUCCESS;
    qspix_cfg_t p_cfg;

#if QSPIX_CFG_PARAM_CHECKING_ENABLE
    ret = r_qspix_parameter_channel_check(channel);
    if(QSPIX_SUCCESS != ret)
    {
        return ret;
    }

    if(QSPIX_CLOSE == g_qspix_handles[channel].open)
    {
        return QSPIX_ERR_NOT_OPEN;
    }
#endif

    p_cfg = g_qspix_handles[channel].p_cfg;
    p_cfg.protocol = protocol;

    g_qspix_handles[channel].p_cfg = p_cfg;

    /* Set protocol */
    QSPIX.SPMR2.BIT.IOMOD = protocol;
    return ret;
}
/******************************************************************************
End of function r_qspix_set_spi_protocol
******************************************************************************/

/************************************************************************************
* Function Name: r_qspix_bankset
* Description  : Checks parameter.
* Arguments    : channel
*                bank
* Return Value : QSPIX_SUCCESS              Successful operation
*              : QSPIX_ERR_INVALID_ARG      Parameter error
*              : QSPIX_ERR_NOT_OPEN         QSPIX module is not initialized yet
************************************************************************************/
qspix_err_t r_qspix_bankset(uint8_t channel, uint8_t bank)
{
    qspix_err_t ret = QSPIX_SUCCESS;
    
#if QSPIX_CFG_PARAM_CHECKING_ENABLE
    ret = r_qspix_parameter_channel_check(channel);
    if(QSPIX_SUCCESS != ret)
    {
        return ret;
    }

    if(QSPIX_CLOSE == g_qspix_handles[channel].open)
    {
        return QSPIX_ERR_NOT_OPEN;
    }

    /* Check requested bank. */
    if(bank > QSPIX_MAXIMUM_BANK)
    {
        return QSPIX_ERR_INVALID_ARG;
    }
#endif
    /* Select the requested bank. */
    QSPIX.SPUAR = bank<<QSPIX_BANK_MASK;

    return ret;
}
/******************************************************************************
End of function r_qspix_bankset
******************************************************************************/

#if QSPIX_CFG_ERI_INCLUDED
/******************************************************************************
* Function Name:    r_qspix_eri_grp_isr
* Description  :    BSP group interrupt handler for register the error callback function
* Arguments    :    pdata
* Return Value :    N/A
******************************************************************************/
static void r_qspix_eri_grp_isr(void *pdata)
{
    /* Clear error flag */
    QSPIX.SPSR.BIT.ROMAE = 0;

    /* Callback Function */
    g_qspix_handles[QSPIX_CH0].p_cfg.p_callback((void *)NULL);

}
/******************************************************************************
 End of function r_qspix_eri_grp_isr
 *****************************************************************************/
#endif
#endif /* BSP_MCU_RX671 */

/* End of File */

