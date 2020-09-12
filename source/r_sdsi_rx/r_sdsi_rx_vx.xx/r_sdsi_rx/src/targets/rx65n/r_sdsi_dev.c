/**********************************************************************************************************************
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
* Copyright (C) 2016(2019) Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/
/**********************************************************************************************************************
* System Name  : SDSI Driver
* File Name    : r_sdsi_dev.c
* Version      : 2.02
* Device       : RX65N
* Abstract     : API & Sub module
* Tool-Chain   : -
* OS           : not use
* H/W Platform : RSK board for RX65N
* Description  : Interface file for SDSI API for RX
* Limitation   : None
**********************************************************************************************************************/
/**********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 30.09.2016 1.00    First Release
*              : 31.07.2017 2.00    Supported RX65N-2MB.
*              :                    Fixed to correspond to Renesas coding rule.
*              : 04.04.2019 2.02    Added support for GNUC and ICCRX.
*                                   Fixed coding style.
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "platform.h"
#if defined(BSP_MCU_RX65N)

#include "r_sdsi_rx_if.h"
#include ".\src\r_sdsi_rx_private.h"

#if (SDSI_CFG_CH0_INT_LEVEL < 1) || (SDSI_CFG_CH0_INT_LEVEL > 15)
    #error "ERROR !!! You must set (1) - (15) in SDSI_CFG_CH0_INT_LEVEL."
#endif /* (SDSI_CFG_CH0_INT_LEVEL < 1) || (SDSI_CFG_CH0_INT_LEVEL > 15) */

#if !defined(SDSI_CFG_CH0_INCLUDED)
    #error "ERROR !!! You must define SDSI_CFG_CH0_INCLUDED."
#endif /* !defined(SDSI_CFG_CH0_INCLUDED) */

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
#define SDSI_IPL_LEVEL               (15)

/* Software delay */
#define SDSI_OVERHEAD_LOOP_COUNT     (4)
        /* Overhead of 20 cycles or 4 loops to call/return from r_sdsi_dev_delaywait() function. */
#define SDSI_CPU_CYCLES_PER_LOOP     (5)
        /* Known number (5) of CPU cycles required to execute the r_sdsi_dev_delaywait() loop. */

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/

#ifdef SDSI_CFG_USE_FIT
/**********************************************************************************************************************
* Outline      : Hardware Lock
* Function Name: r_sdsi_dev_hardware_lock
* Description  : Hardware Lock.
* Arguments    : uint32_t           channel     ; Channel No.
* Return Value : true                           ; Lock was acquired.
*              : false                          ; Lock was not acquired.
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
bool r_sdsi_dev_hardware_lock(uint32_t channel)
{
    return R_BSP_HardwareLock((mcu_lock_t)(BSP_LOCK_SDSI + channel));
} /* End of function r_sdsi_dev_hardware_lock() */

/**********************************************************************************************************************
* Outline      : Hardware Unlock
* Function Name: r_sdsi_dev_hardware_unlock
* Description  : Hardware Unlock.
* Arguments    : uint32_t           channel     ; Channel No.
* Return Value : true                           ; Lock was released.
*              : false                          ; Lock was not released.
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
bool r_sdsi_dev_hardware_unlock(uint32_t channel)
{
    return R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_SDSI + channel));
} /* End of function r_sdsi_dev_hardware_unlock() */
#endif /* SDSI_CFG_USE_FIT */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_module_enable
* Description  : Enables the peripheral module.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_module_enable(uint32_t channel)
{
#ifdef SDSI_CFG_USE_FIT
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
    MSTP_SDSI = 0;              /* SDSI ON */
    if (0 == MSTP_SDSI)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
#else
    SYSTEM.PRCR.WORD = 0xA502;
    if (0xA502 == SYSTEM.PRCR.WORD)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    MSTP_SDSI = 0;              /* SDSI ON */
    if (0 == MSTP_SDSI)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    SYSTEM.PRCR.WORD = 0xA500;
    if (0xA500 == SYSTEM.PRCR.WORD)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }
#endif /* SDSI_CFG_USE_FIT */

    if (SDSI_CH0 == channel)    /* SDSI_CH0 */
    {
#if defined(SDSI_CFG_CH0_INCLUDED)
#ifdef SDSI_CFG_USE_FIT
        R_BSP_InterruptWrite(BSP_INT_SRC_BL2_SDSI_SDIOI, R_SDSI_IntHandler0);
#endif /* SDSI_CFG_USE_FIT */
#endif  /* SDSI_CFG_CH0_INCLUDED */
    }
    else
    {
        /* Do nothing. */
    }

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_module_enable() */


/**********************************************************************************************************************
* Function Name: r_sdsi_dev_module_disable
* Description  : Disables the peripheral module.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_module_disable(uint32_t channel)
{
#ifdef SDSI_CFG_USE_FIT
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
    MSTP_SDSI = 1;              /* SDSI OFF */
    if (1 == MSTP_SDSI)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
#else
    SYSTEM.PRCR.WORD = 0xA502;
    if (0xA502 == SYSTEM.PRCR.WORD)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    MSTP_SDSI = 1;              /* SDSI OFF */
    if (1 == MSTP_SDSI)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    SYSTEM.PRCR.WORD = 0xA500;
    if (0xA500 == SYSTEM.PRCR.WORD)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }
#endif /* SDSI_CFG_USE_FIT */

    if (SDSI_CH0 == channel)
    {
#if defined(SDSI_CFG_CH0_INCLUDED)
#ifdef SDSI_CFG_USE_FIT
        R_BSP_InterruptWrite(BSP_INT_SRC_BL2_SDSI_SDIOI, FIT_NO_FUNC);
#endif /* SDSI_CFG_USE_FIT */
#endif  /* SDSI_CFG_CH0_INCLUDED */
    }
    else    /* SDSI_CH1 */
    {
        /* Do nothing. */
    }

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_module_disable() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_disable_system_int
* Description  : Set to disable all interrupts.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_disable_system_int(uint32_t channel)
{
#if defined(SDSI_CFG_DISABLE_SYSTEM_INTERRUPT)
    #ifdef SDSI_CFG_USE_FIT
        R_BSP_InterruptsDisable();
    #else
        #if defined(__RENESAS__)
        /* Use the compiler intrinsic function to clear the I flag. This function is available with for the Renesas RX 
           compiler. This may need to be changed for other compilers. */
        R_BSP_CLRPSW_I();
        #endif /* __RENESAS__ */
    #endif /* SDSI_CFG_USE_FIT */
#endif /* SDSI_CFG_DISABLE_SYSTEM_INTERRUPT */
    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_disable_system_int() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_enable_system_int
* Description  : Get to disable all interrupts.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_enable_system_int(uint32_t channel)
{
#if defined(SDSI_CFG_DISABLE_SYSTEM_INTERRUPT)
    #ifdef SDSI_CFG_USE_FIT
        R_BSP_InterruptsEnable();
    #else
        #if defined(__RENESAS__)
        /* Use the compiler intrinsic function to set the I flag. This function is available with for the Renesas RX 
           compiler. This may need to be changed for other compilers. */
        R_BSP_SETPSW_I();
        #endif /* __RENESAS__ */
    #endif /* SDSI_CFG_USE_FIT */
#endif /* SDSI_CFG_DISABLE_SYSTEM_INTERRUPT */
    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_enable_system_int() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_open
* Description  : Sets SDSI register for R_SDSI_Open() function.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_open(uint32_t channel)
{
    sdsi_hndl_t * p_hndl = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    /* Reset SDSI. */
    /* Disable CCCR.IOR0(Function0). */
    SDSI_OUTREG_LONG(p_hndl, SDSI_SDSICR3,  SDSI_SDSICR3_SRST_ENABLE);
    if (0 == SDSI_INREG_LONG(p_hndl, SDSI_SDSICR3))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }
    SDSI_OUTREG_LONG(p_hndl, SDSI_SDSICR3,  SDSI_SDSICR3_SRST_DISABLE);
    if (0 == SDSI_INREG_LONG(p_hndl, SDSI_SDSICR3))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    /* Disable SDSI interrupts and cleer SDSI status register. */
/*  SDSI_OUTREG_BYTE(p_hndl, SDSI_INTENCR1, SDSI_INTENCR1_DISABLE); */ /* Clear by SDSI reset. */
    SDSI_OUTREG_LONG(p_hndl, SDSI_INTENCR2, SDSI_INTENCR2_DISABLE);
/*  SDSI_OUTREG_BYTE(p_hndl, SDSI_INTSR1,   SDSI_INTSR1_INIT); */ /* Clear by SDSI reset. */
    SDSI_OUTREG_LONG(p_hndl, SDSI_INTSR2,   SDSI_INTSR2_INIT);

    /* Enable access to function1 by the CPU. */
    SDSI_OUTREG_LONG(p_hndl, SDSI_FN1ACCR,  SDSI_FN1ACC_ENABLE); 

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_open() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_close
* Description  : Sets SDSI register for R_SDSI_Close() function.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_close(uint32_t channel)
{
    sdsi_hndl_t * p_hndl = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    /* Reset SDSI. */
    SDSI_OUTREG_LONG(p_hndl, SDSI_SDSICR3,  SDSI_SDSICR3_SRST_ENABLE);
    if (0 == SDSI_INREG_LONG(p_hndl, SDSI_SDSICR3))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }
    SDSI_OUTREG_LONG(p_hndl, SDSI_SDSICR3,  SDSI_SDSICR3_SRST_DISABLE);
    if (0 == SDSI_INREG_LONG(p_hndl, SDSI_SDSICR3))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    /* Disable SDSI interrupts and cleer SDSI status register. */
/*  SDSI_OUTREG_BYTE(p_hndl, SDSI_INTENCR1, SDSI_INTENCR1_DISABLE); */ /* Clear by SDSI reset. */
    SDSI_OUTREG_LONG(p_hndl, SDSI_INTENCR2, SDSI_INTENCR2_DISABLE);
/*  SDSI_OUTREG_BYTE(p_hndl, SDSI_INTSR1,   SDSI_INTSR1_INIT); */ /* Clear by SDSI reset. */
    SDSI_OUTREG_LONG(p_hndl, SDSI_INTSR2,   SDSI_INTSR2_INIT);
    if (SDSI_INTSR2_INIT == SDSI_INREG_LONG(p_hndl, SDSI_INTSR2))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_close() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_initialize
* Description  : Sets SDSI register for R_SDSI_Initialize() function.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_initialize(uint32_t channel)
{
    sdsi_hndl_t * p_hndl = 0;
    uint32_t      reg = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    /* Reset SDSI. */
    SDSI_OUTREG_LONG(p_hndl, SDSI_SDSICR3,  SDSI_SDSICR3_SRST_ENABLE);
    if (0 == SDSI_INREG_LONG(p_hndl, SDSI_SDSICR3))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }
    SDSI_OUTREG_LONG(p_hndl, SDSI_SDSICR3,  SDSI_SDSICR3_SRST_DISABLE);
    if (0 == SDSI_INREG_LONG(p_hndl, SDSI_SDSICR3))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    /* Disable SDSI interrupts and cleer SDSI status register. */
/*  SDSI_OUTREG_BYTE(p_hndl, SDSI_INTENCR1, SDSI_INTENCR1_DISABLE); */ /* Clear by SDSI reset. */
    SDSI_OUTREG_LONG(p_hndl, SDSI_INTENCR2, SDSI_INTENCR2_DISABLE);
/*  SDSI_OUTREG_BYTE(p_hndl, SDSI_INTSR1,   SDSI_INTSR1_INIT); */ /* Clear by SDSI reset. */
    SDSI_OUTREG_LONG(p_hndl, SDSI_INTSR2,   SDSI_INTSR2_INIT);

    /* Enable access to function1 by the CPU. */
    SDSI_OUTREG_LONG(p_hndl, SDSI_FN1ACCR,  SDSI_FN1ACC_ENABLE); 

    /* Disable direct transfer and not lock internal main bus. */
/*  SDSI_OUTREG_BYTE(p_hndl, SDSI_DMACR1, SDSI_DMACR1_DISABLE); */ /* Clear by SDSI reset. */

    /* Set FBRn register. */
    reg = (uint32_t)((uint32_t)(SDSI_CFG_FBR_ADR_101H << SDSI_BIT_SHIFT_8) |
                                SDSI_CFG_FBR_ADR_100H);
    SDSI_OUTREG_LONG(p_hndl, SDSI_FBR1, reg);
    SDSI_OUTREG_LONG(p_hndl, SDSI_FBR2, (uint32_t)SDSI_CFG_FBR_ADR_103H);
    reg = 0;
    reg = (uint32_t)((uint32_t)(SDSI_CFG_FBR_ADR_105H << SDSI_BIT_SHIFT_8) |
                                SDSI_CFG_FBR_ADR_104H);
    SDSI_OUTREG_LONG(p_hndl, SDSI_FBR3, reg);
    reg = 0;
    reg = (uint32_t)((uint32_t)(SDSI_CFG_FBR_ADR_107H << SDSI_BIT_SHIFT_8) |
                                SDSI_CFG_FBR_ADR_106H);
    SDSI_OUTREG_LONG(p_hndl, SDSI_FBR4, reg);
    SDSI_OUTREG_LONG(p_hndl, SDSI_FBR5, (uint32_t)SDSI_CFG_FBR_ADR_108H);

    /* Enable to access from the SD host controller and
       Set swap the data of register 1 to 5 in function 1 in byte units.
       Not get DMA bus of the SDSI and set swap the data for direct transfer in byte units. */
    SDSI_OUTREG_LONG(p_hndl, SDSI_SDSICR2,  SDSI_SDSICR2_INIT);
    SDSI_OUTREG_LONG(p_hndl, SDSI_DMACR2,   SDSI_DMACR2_INIT);

    /* Enable SDSI interrupts. */
    SDSI_OUTREG_BYTE(p_hndl, SDSI_INTENCR1, SDSI_INTENCR1_ENABLE);
    SDSI_OUTREG_LONG(p_hndl, SDSI_INTENCR2, SDSI_INTENCR2_ENABLE);

    /* Enable CCCR.IOE1/IOR1(Function1). */
    SDSI_OUTREG_BYTE(p_hndl, SDSI_SDSICR1,  SDSI_SDSICR1_IOE1IOR1);

    /* Enable CCCR.IOR0(Function0). */
    reg = (SDSI_CFG_FBR_SPS_BIT << SDSI_BIT_SHIFT_18);
    reg |= (SDSI_CFG_CCCR_SMPC_BIT << SDSI_BIT_SHIFT_19);
    reg |= SDSI_SDSICR3_IOR0;
    SDSI_OUTREG_LONG(p_hndl, SDSI_SDSICR3, reg);
    if (0 == SDSI_INREG_LONG(p_hndl, SDSI_SDSICR3))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_initialize() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_end
* Description  : Sets SDSI register for R_SDSI_End() function.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_end(uint32_t channel)
{
    sdsi_hndl_t * p_hndl = 0;
    uint32_t      reg = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    /* Disable CCCR.IOR0(Function0). */
    reg = (SDSI_INREG_LONG(p_hndl, SDSI_SDSICR3) & (uint32_t)~SDSI_SDSICR3_IOR0);
    SDSI_OUTREG_LONG(p_hndl, SDSI_SDSICR3,  reg);

    /* Disable CCCR.IOE1/IOR1(Function1). */
    reg = (SDSI_INREG_BYTE(p_hndl, SDSI_SDSICR1) & (~SDSI_SDSICR1_IOE1IOR1));
    SDSI_OUTREG_BYTE(p_hndl, SDSI_SDSICR1,  reg);

    /* Disable SDSI interrupts and cleer SDSI status register. */
    SDSI_OUTREG_BYTE(p_hndl, SDSI_INTENCR1, SDSI_INTENCR1_DISABLE);
    SDSI_OUTREG_LONG(p_hndl, SDSI_INTENCR2, SDSI_INTENCR2_DISABLE);
    SDSI_OUTREG_BYTE(p_hndl, SDSI_INTSR1,   SDSI_INTSR1_INIT);
    SDSI_OUTREG_LONG(p_hndl, SDSI_INTSR2,   SDSI_INTSR2_INIT);

    /* Reset swap the data of register 1 to 5 in function 1 in byte units.
       Not get DMA bus of the SDSI and set swap the data for direct transfer in byte units. */
    SDSI_OUTREG_LONG(p_hndl, SDSI_SDSICR2,  SDSI_SDSICR2_SWAP_DISABLE);
    SDSI_OUTREG_LONG(p_hndl, SDSI_DMACR2,   SDSI_DMACR2_SWAP_DISABLE);

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_end() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_c_flag_polling
* Description  : Check the C Flag.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   C Flag is 1 (Ready).
*              : SDSI_ERR                                ;   C Flag is 0 (Busy).
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_c_flag_polling(uint32_t channel)
{
    sdsi_hndl_t * p_hndl = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    /* Check the C Flag. */
    if (SDSI_SDSICR3_CEN == (SDSI_INREG_LONG(p_hndl, SDSI_SDSICR3) & SDSI_SDSICR3_CEN))
    {
        return SDSI_SUCCESS;
    }
    return SDSI_ERR;
} /* End of function r_sdsi_dev_c_flag_polling() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_write_cis_reg
* Description  : Sets SDSI CIS register for R_SDSI_WriteCisReg() function.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
*              : sdsi_reg_t * p_sdsi_reg                 ;
*              :    uint32_t    reg_no                   ;   Ignore value
*              :    uint32_t    offset                   ;   Offset from the targeted register.
*              :    uint32_t    * p_buff                 ;   Register value
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_write_cis_reg(uint32_t channel, sdsi_cis_reg_t * p_sdsi_reg)
{
    sdsi_hndl_t * p_hndl = 0;
    uint32_t      offset = 0;
    uint32_t      reg = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    offset = SDSI_CISDATAR0 + p_sdsi_reg->offset;

#if (SDSI_BIG_ENDIAN)
    reg = *p_sdsi_reg->p_buff;
#else
    reg = R_BSP_REVL(*p_sdsi_reg->p_buff);
#endif /* (SDSI_BIG_ENDIAN) */

    /* Write CIS register. */
    SDSI_OUTREG_LONG(p_hndl, offset, reg);
    if (0 == SDSI_INREG_LONG(p_hndl, offset))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }
    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_write_cis_reg() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_read_cis_reg
* Description  : Gets SDSI CIS register for R_SDSI_ReadCisReg() function.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
*              : sdsi_reg_t * p_sdsi_reg                 ;
*              :    uint32_t    reg_no                   ;   Ignore value
*              :    uint32_t    offset                   ;   Offset from the targeted register.
*              :    uint32_t    * p_buff                 ;   Register value
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_read_cis_reg(uint32_t channel, sdsi_cis_reg_t * p_sdsi_reg)
{
    sdsi_hndl_t * p_hndl = 0;
    uint32_t      offset = 0;
    uint32_t      reg = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    offset = SDSI_CISDATAR0 + p_sdsi_reg->offset;

    /* Read CIS register. */
    reg = SDSI_INREG_LONG(p_hndl, offset);

#if (SDSI_BIG_ENDIAN)
    *p_sdsi_reg->p_buff = reg;
#else
    *p_sdsi_reg->p_buff = R_BSP_REVL(reg);
#endif /* (SDSI_BIG_ENDIAN) */


    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_read_cis_reg() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_write_func_reg
* Description  : Sets SDSI function 1 register N for R_SDSI_WriteFuncReg() function.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
*              : sdsi_reg_t * p_sdsi_reg                 ;
*              :    uint32_t    reg_no                   ;   Ignore value
*              :    uint32_t    offset                   ;   Offset from the targeted register.
*              :    uint32_t    * p_buff                 ;   Register value
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_write_func_reg(uint32_t channel, sdsi_func_reg_t * p_sdsi_reg)
{
    sdsi_hndl_t * p_hndl = 0;
    uint32_t      offset = 0;
    uint32_t      reg = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    offset = (SDSI_FN1DATAR10 + ((p_sdsi_reg->reg_no - 1) * 0x100)) + p_sdsi_reg->offset;

    /* Enable function 1 register 5 access. */
    if (SDSI_FUNC1_REG5 == p_sdsi_reg->reg_no)
    {
//        reg = (SDSI_INREG_LONG(p_hndl, SDSI_SDSICR2) | SDSI_SDSICR2_REG5EN_ENABLE);
        reg = (SDSI_INREG_BYTE(p_hndl, SDSI_SDSICR2) | SDSI_SDSICR2_REG5EN_ENABLE);
//        SDSI_OUTREG_LONG(p_hndl, SDSI_SDSICR2,  reg);
        SDSI_OUTREG_BYTE(p_hndl, SDSI_SDSICR2,  reg);
//        if (0 == SDSI_INREG_LONG(p_hndl, SDSI_SDSICR2))
        if (0 == SDSI_INREG_BYTE(p_hndl, SDSI_SDSICR2))
        {
            R_BSP_NOP();    /* Wait for the write completion. */
        }
    }

    /* Write function 1 register N. */
    reg = *p_sdsi_reg->p_buff;
//    SDSI_OUTREG_LONG(p_hndl, offset, reg);
    SDSI_OUTREG_BYTE(p_hndl, offset, reg);
//    if (0 == SDSI_INREG_LONG(p_hndl, offset))
    if (0 == SDSI_INREG_BYTE(p_hndl, offset))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    /* Disable function 1 register 5 access. */
    if (SDSI_FUNC1_REG5 == p_sdsi_reg->reg_no)
    {
//        reg = (SDSI_INREG_LONG(p_hndl, SDSI_SDSICR2) & (uint32_t)~SDSI_SDSICR2_REG5EN_ENABLE);
        reg = (SDSI_INREG_BYTE(p_hndl, SDSI_SDSICR2) & (uint8_t)~SDSI_SDSICR2_REG5EN_ENABLE);
//        SDSI_OUTREG_LONG(p_hndl, SDSI_SDSICR2,  reg);
        SDSI_OUTREG_BYTE(p_hndl, SDSI_SDSICR2,  reg);
//        if (0 == SDSI_INREG_LONG(p_hndl, SDSI_SDSICR2))
        if (0 == SDSI_INREG_BYTE(p_hndl, SDSI_SDSICR2))
        {
            R_BSP_NOP();    /* Wait for the write completion. */
        }
    }

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_write_func_reg() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_read_func_reg
* Description  : Gets SDSI function 1 register N for R_SDSI_ReadFuncReg() function.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
*              : sdsi_reg_t * p_sdsi_reg                 ;
*              :    uint32_t    reg_no                   ;   Ignore value
*              :    uint32_t    offset                   ;   Offset from the targeted register.
*              :    uint32_t    * p_buff                 ;   Register value
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_read_func_reg(uint32_t channel, sdsi_func_reg_t * p_sdsi_reg)
{
    sdsi_hndl_t * p_hndl = 0;
    uint32_t      offset = 0;
    uint32_t      reg = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    offset = (SDSI_FN1DATAR10 + ((p_sdsi_reg->reg_no - 1) * 0x100)) + p_sdsi_reg->offset;

    /* Enable function 1 register 5 access. */
    if (SDSI_FUNC1_REG5 == p_sdsi_reg->reg_no)
    {
//        reg = (SDSI_INREG_LONG(p_hndl, SDSI_SDSICR2) | SDSI_SDSICR2_REG5EN_ENABLE);
        reg = (SDSI_INREG_BYTE(p_hndl, SDSI_SDSICR2) | SDSI_SDSICR2_REG5EN_ENABLE);
//        SDSI_OUTREG_LONG(p_hndl, SDSI_SDSICR2,  reg);
        SDSI_OUTREG_BYTE(p_hndl, SDSI_SDSICR2,  reg);
//        if (0 == SDSI_INREG_LONG(p_hndl, SDSI_SDSICR2))
        if (0 == SDSI_INREG_BYTE(p_hndl, SDSI_SDSICR2))
        {
            R_BSP_NOP();    /* Wait for the write completion. */
        }
    }

    /* Read function 1 register N. */
//      reg = SDSI_INREG_LONG(p_hndl, offset);
      reg = SDSI_INREG_BYTE(p_hndl, offset);
    *p_sdsi_reg->p_buff = reg;

    /* Disable function 1 register 5 access. */
    if (SDSI_FUNC1_REG5 == p_sdsi_reg->reg_no)
    {
//        reg = (SDSI_INREG_LONG(p_hndl, SDSI_SDSICR2) & (uint32_t)~SDSI_SDSICR2_REG5EN_ENABLE);
        reg = (SDSI_INREG_BYTE(p_hndl, SDSI_SDSICR2) & (uint8_t)~SDSI_SDSICR2_REG5EN_ENABLE);
//        SDSI_OUTREG_LONG(p_hndl, SDSI_SDSICR2,  reg);
        SDSI_OUTREG_BYTE(p_hndl, SDSI_SDSICR2,  reg);
//        if (0 == SDSI_INREG_LONG(p_hndl, SDSI_SDSICR2))
        if (0 == SDSI_INREG_BYTE(p_hndl, SDSI_SDSICR2))
        {
            R_BSP_NOP();    /* Wait for the write completion. */
        }
    }

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_read_func_reg() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_write_int_vector_reg
* Description  : Sets SDIO interrupt vector(FN1INTVECR).
* Arguments    : uint32_t       channel                 ;   SDSI Channel No.
*              : uint8_t        vector                  ;   SDIO interrupt vector
* Return Value : SDSI_SUCCESS                           ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_write_int_vector_reg(uint32_t channel, uint8_t vector)
{
    sdsi_hndl_t * p_hndl = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    /* Set SDIO interrupt vector. */
    SDSI_OUTREG_BYTE(p_hndl, SDSI_FN1INTVECR, vector);
    if (0 == SDSI_INREG_BYTE(p_hndl, SDSI_FN1INTVECR))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_write_int_vector_reg() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_read_int_vector_reg
* Description  : Gets SDIO interrupt vector(FN1INTVECR).
* Arguments    : uint32_t       channel                 ;   SDSI Channel No.
*              : uint8_t        * p_vector              ;   SDIO interrupt vector buffer
* Return Value : SDSI_SUCCESS                           ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_read_int_vector_reg(uint32_t channel, uint8_t * p_vector)
{
    sdsi_hndl_t * p_hndl = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    /* Get SDIO interrupt vector. */
    *p_vector = SDSI_INREG_BYTE(p_hndl, SDSI_FN1INTVECR);

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_read_int_vector_reg() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_read_int_clear_reg
* Description  : Gets SDIO interrupt vector clear register(FN1INTCLRR).
* Arguments    : uint32_t       channel                 ;   SDSI Channel No.
*              : uint8_t        * p_vector              ;   SDIO interrupt vector clear buffer
* Return Value : SDSI_SUCCESS                           ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_read_int_clear_reg(uint32_t channel, uint8_t * p_vector)
{
    sdsi_hndl_t * p_hndl = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    /* Get SDIO interrupt vector clear register. */
    *p_vector = SDSI_INREG_BYTE(p_hndl, SDSI_FN1INTCLRR);

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_read_int_clear_reg() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_chk_direct_trans_adr
* Description  : Checks the direct transfer address.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
*              : uint32_t           adr                  ;   Direct transfer address
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*              : SDSI_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_chk_direct_trans_adr(uint32_t channel, uint32_t adr)
{
    if (BSP_RAM_SIZE_BYTES < adr)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_DEV, __LINE__);
        return SDSI_ERR;
    }
    
    /* Check the address boundary (4-byte unit) */
    if (adr & SDSI_ADDR_BOUNDARY)
    {
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_DEV, __LINE__);
        return SDSI_ERR_ADDRESS_BOUNDARY;
    }
    
    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_chk_direct_trans_adr() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_enable_direct_trans
* Description  : Enables direct transfer.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
*              : sdsi_direct_trans_t    * p_sdsi_direct_trans   ;
*              :    uint32_t    adr                      ;   Direct transfer address
*              :    uint32_t    mode                     ;   Mode
*              :       SDSI_MODE_DIRECT_ADDR_FIXED       ;   Direct transfer address is fixed.
*              :       SDSI_MODE_DIRECT_ADDR_INC         ;   Direct transfer address is incremented after each transfer.
*              :       SDSI_MODE_DIRECT_BUS_LOCK         ;   Internal main bus is locked.
*              :       SDSI_MODE_DIRECT_BUS_UNLOCK       ;   Internal main bus is not locked.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*              : SDSI_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_enable_direct_trans(uint32_t channel, sdsi_direct_trans_t * p_sdsi_direct_trans)
{
    sdsi_hndl_t * p_hndl = 0;
    uint32_t      reg = 0;
#ifdef SDSI_CFG_USE_FIT
    bool          chk = false;

    /* Get the EDMAC0 hardware resource. */
    chk = R_BSP_HardwareLock((mcu_lock_t)BSP_LOCK_EDMAC0);
    if (false == chk)
    {
        /* Lock has already been acquired by another task. Needs to try again later. */
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_DEV, __LINE__);
        return SDSI_ERR;
    }
#endif /* SDSI_CFG_USE_FIT */

    /* Check ETHER0 module stop bit. */
    if (0 == MSTP_ETHERC0)
    {
#ifdef SDSI_CFG_USE_FIT
        R_BSP_HardwareUnlock((mcu_lock_t)BSP_LOCK_EDMAC0);
#endif /* SDSI_CFG_USE_FIT */
        SDSI_LOG_FUNC(SDSI_DEBUG_ERR_ID, (uint32_t)SDSI_DEV, __LINE__);
        return SDSI_ERR;
    }

    p_hndl = SDSI_GET_HNDL(channel);

    /* Set direct transfer address. */
    SDSI_OUTREG_LONG(p_hndl, SDSI_DMATRADDR,  p_sdsi_direct_trans->adr);

    /* Get DMA bus of the SDSI. */
    reg = (SDSI_INREG_LONG(p_hndl, SDSI_DMACR2) | SDSI_DMACR2_DMASDSEL);
    SDSI_OUTREG_LONG(p_hndl, SDSI_DMACR2, reg);

    if (SDSI_MODE_DIRECT_BUS_LOCK == (p_sdsi_direct_trans->mode & SDSI_MODE_DIRECT_BUS_LOCK))
    {
        /* Lock internal main bus and enable direct transfer. */
        SDSI_OUTREG_BYTE(p_hndl, SDSI_DMACR1, SDSI_DMACR1_ENABLE_BUS_LOCK);
    }
    else
    {
        /* Not lock internal main bus and enable direct transfer. */
        SDSI_OUTREG_BYTE(p_hndl, SDSI_DMACR1, SDSI_DMACR1_ENABLE_BUS_UNLOCK);
    }
    
    if (0 == SDSI_INREG_BYTE(p_hndl, SDSI_DMACR1))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_enable_direct_trans() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_disable_direct_trans
* Description  : Disables dierct transfer.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_disable_direct_trans(uint32_t channel)
{
    sdsi_hndl_t * p_hndl = 0;
    uint32_t      reg = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    /* Disable direct transfer and not lock internal main bus. */
    SDSI_OUTREG_BYTE(p_hndl, SDSI_DMACR1, SDSI_DMACR1_DISABLE);

    /* Release DMA bus of the SDSI. */
    reg = (SDSI_INREG_LONG(p_hndl, SDSI_DMACR2) & (uint32_t)~SDSI_DMACR2_DMASDSEL);
    SDSI_OUTREG_LONG(p_hndl, SDSI_DMACR2, reg);
    if (0 == SDSI_INREG_LONG(p_hndl, SDSI_DMACR2))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

#ifdef SDSI_CFG_USE_FIT
    R_BSP_HardwareUnlock((mcu_lock_t)BSP_LOCK_EDMAC0);
#endif /* SDSI_CFG_USE_FIT */

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_disable_direct_trans() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_set_direct_trans_adr
* Description  : Sets dierct transfer.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
*              : uint32_t           adr                  ;   Direct transfer address
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_set_direct_trans_adr(uint32_t channel, uint32_t adr)
{
    sdsi_hndl_t * p_hndl = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    /* Set direct transfer address. */
    SDSI_OUTREG_LONG(p_hndl, SDSI_DMATRADDR, adr);

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_set_direct_trans_adr() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_get_direct_trans_adr
* Description  : Gets dierct transfer.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
*              : uint32_t           * p_adr              ;   Direct transfer address buffer
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        :  None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_get_direct_trans_adr(uint32_t channel, uint32_t * p_adr)
{
    sdsi_hndl_t * p_hndl = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    /* Get direct transfer address. */
    *p_adr = SDSI_INREG_LONG(p_hndl, SDSI_DMATRADDR);

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_get_direct_trans_adr() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_enable_int
* Description  : Enable the Interrupt Request Enable (IENn).
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_enable_int(uint32_t channel)
{
#ifdef SDSI_CFG_USE_FIT
    uint32_t ipl = 0;
#endif /* SDSI_CFG_USE_FIT */

    if (SDSI_CH0 == channel)
    {
#if defined(SDSI_CFG_CH0_INCLUDED)
        /* ---- Enable interrupt request (IENn) of IER. ---- */
        ICU.GENBL2.BIT.EN0 = 1;
        if (1 == ICU.GENBL2.BIT.EN0)
        {
            R_BSP_NOP();    /* Wait for the write completion. */
        }
#ifdef SDSI_CFG_USE_FIT
        ipl = SDSI_CFG_CH0_INT_LEVEL;
        R_BSP_InterruptControl(BSP_INT_SRC_BL2_SDSI_SDIOI, BSP_INT_CMD_GROUP_INTERRUPT_ENABLE, (void *)&ipl);
#else
        IPR(ICU,GROUPBL2)  = SDSI_CFG_CH0_INT_LEVEL;
        R_BSP_InterruptRequestEnable(VECT(ICU,GROUPBL2));
        if (1 == IEN(ICU,GROUPBL2))
        {
            R_BSP_NOP();    /* Wait for the write completion. */
        }
#endif /* SDSI_CFG_USE_FIT */
#endif /* SDSI_CFG_CH0_INCLUDED */
    }
    else
    {
        /* Do nothing. */
    }

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_enable_int() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_disable_int
* Description  : Disable the Interrupt Request Enable (IENn).
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_disable_int(uint32_t channel)
{
#ifdef SDSI_CFG_USE_FIT
    uint32_t ipl = 0;
#endif /* SDSI_CFG_USE_FIT */

    if (SDSI_CH0 == channel)
    {
#if defined(SDSI_CFG_CH0_INCLUDED)
        /* ---- Disable interrupt request (IENn) of IER. ---- */
        ICU.GENBL2.BIT.EN0 = 0;
        if (0 == ICU.GENBL2.BIT.EN0)
        {
            R_BSP_NOP();    /* Wait for the write completion. */
        }
#ifdef SDSI_CFG_USE_FIT
        ipl = 0;
        R_BSP_InterruptControl(BSP_INT_SRC_BL2_SDSI_SDIOI, BSP_INT_CMD_GROUP_INTERRUPT_DISABLE, (void *)&ipl);
#else
        R_BSP_InterruptRequestDisable(VECT(ICU,GROUPBL2));
        IPR(ICU,GROUPBL2)  = 0;
        if (0 == IPR(ICU,GROUPBL2))
        {
            R_BSP_NOP();    /* Wait for the write completion. */
        }
#endif /* SDSI_CFG_USE_FIT */
#endif /* SDSI_CFG_CH0_INCLUDED */
    }
    else
    {
        /* Do nothing. */
    }

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_disable_int() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_get_int
* Description  : Gets the INTSR1 and INTSR2 interrupt sources, and then bits examine enabled elements hereafter
*              : clears INTSR1 and INTSR2 bits and saves those bits to intsr1 or intsr2.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*              : SDSI_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_get_int(uint32_t channel)
{
    sdsi_hndl_t * p_hndl = 0;
    uint8_t       intsr1 = 0;
    uint32_t      intsr2 = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    /* Get the INTSR1 and INTSR2 registers. */
    intsr1 = (uint32_t)(SDSI_INREG_BYTE(p_hndl, SDSI_INTSR1) & SDSI_INTENCR1_ENABLE);
    intsr2 = (uint32_t)(SDSI_INREG_LONG(p_hndl, SDSI_INTSR2) & SDSI_INTENCR2_ENABLE);

    /* Clear the INTSR1 and INTSR2 registers. */
    SDSI_OUTREG_BYTE(p_hndl, SDSI_INTSR1, intsr1);
    SDSI_OUTREG_LONG(p_hndl, SDSI_INTSR2, (uint32_t)~intsr2);
    if (0 == SDSI_INREG_LONG(p_hndl, SDSI_INTSR2))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    /* Store the interrupt sources to the INTSR1 and INTSR2. */
    p_hndl->intsr1 = (p_hndl->intsr1 | intsr1);
    p_hndl->intsr2 = (p_hndl->intsr2 | intsr2);
    if (intsr1 || intsr2)
    {
        return SDSI_SUCCESS;   /* Any interrupt occurred */
    }

    return SDSI_ERR;  /* No interrupt occurred */
} /* End of function r_sdsi_dev_get_int() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_clear_int
* Description  : Clears the p_hndl->intsr1 and p_hndl->intsr2.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_clear_int(uint32_t channel)
{
    sdsi_hndl_t * p_hndl = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    /* Clear the interrupt sources to the INTSR1 and INTSR2. */
    p_hndl->intsr1 = 0;
    p_hndl->intsr2 = 0;

    return SDSI_SUCCESS;   /* Any interrupt occurred */
} /* End of function r_sdsi_dev_clear_int() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_get_cmd_info
* Description  : Gets cmd information.
* Arguments    : uint32_t           channel     ;   SDSI Channel No.
*              : sdsi_cmd_t       * p_sdsi_cmd  ;
*              :    uint32_t    adr             ;   SDIO Register addresses in the argument (Lower 17bits).
*              :    uint16_t    blkcnt          ;   The block count in response to CMD53.
*              :                                ;   If get CMD52_W, set 0 to blkncnt.
*              :    uint16_t    bytcnt          ;   The byte count in response to CMD53.
*              :                                ;   If get CMD52_W, set 0 to bytecnt.
*              :    uint8_t     sdcmdcr         ;   SDCMDCR register value
*              :    uint8_t     cmd             ;   Command
*              :                                ;       SDSI_CMD52_W
*              :                                ;       SDSI_CMD53_W
*              :                                ;       SDSI_CMD53_R
*              :    uint8_t     rsv[2]          ;   Reserved
* Return Value : SDSI_SUCCESS                   ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_get_cmd_info(uint32_t channel, sdsi_cmd_t * p_sdsi_cmd)
{
    sdsi_hndl_t * p_hndl = 0;
    uint32_t adr = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    p_sdsi_cmd->sdcmdcr = SDSI_INREG_BYTE(p_hndl, SDSI_SDCMDCR);

    adr = ((uint32_t)SDSI_INREG_BYTE(p_hndl, SDSI_SDCADD2R) << SDSI_BIT_SHIFT_16);
    adr |= ((uint32_t)SDSI_INREG_BYTE(p_hndl, SDSI_SDCADD1R) << SDSI_BIT_SHIFT_8);
    adr |= (uint32_t)SDSI_INREG_BYTE(p_hndl, SDSI_SDCADD0R);
    p_sdsi_cmd->adr = adr;
    
    if (SDSI_SDCMDCR_SDCMDINDEX == (p_sdsi_cmd->sdcmdcr & SDSI_SDCMDCR_SDCMDINDEX))
    {
        p_sdsi_cmd->blkcnt = SDSI_INREG_WORD(p_hndl, SDSI_BLKCNT);
        p_sdsi_cmd->bytcnt = SDSI_INREG_WORD(p_hndl, SDSI_BYTCNT);
    }
    else
    {
        p_sdsi_cmd->blkcnt = 0;
        p_sdsi_cmd->bytcnt = 0;
    }

    switch (p_hndl->intsr1)
    {
        case SDSI_INTENCR1_CMD52WIREN:
            p_sdsi_cmd->cmd = SDSI_CMD52_W;
        break;
        case SDSI_INTENCR1_CMD53WIREN:
            p_sdsi_cmd->cmd = SDSI_CMD53_W;
        break;
        case SDSI_INTENCR1_CMD53RIREN:
            p_sdsi_cmd->cmd = SDSI_CMD53_R;
        break;
        default:
            /* Do nothing. */
        break;
    }

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_get_cmd_info() */

/**********************************************************************************************************************
* Function Name: r_sdsi_dev_update_direct_trans_adr
* Description  : Updates direct address.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : SDSI_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdsi_status_t r_sdsi_dev_update_direct_trans_adr(uint32_t channel)
{
    sdsi_hndl_t * p_hndl = 0;
    uint32_t      adr = 0;

    p_hndl = SDSI_GET_HNDL(channel);

    adr = SDSI_INREG_LONG(p_hndl, SDSI_DMATRADDR);
    SDSI_OUTREG_LONG(p_hndl, SDSI_DMATRADDR, adr);
    if (0 == SDSI_INREG_LONG(p_hndl, SDSI_DMATRADDR))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    return SDSI_SUCCESS;
} /* End of function r_sdsi_dev_update_direct_trans_adr() */

/**********************************************************************************************************************
* Function Name: R_SDSI_IntHandler0
* Description  : Checks the relevant elements (without masked) and call a callback function.
* Arguments    : uint32_t           channel              ;   SDSI Channel No.
* Return Value : None
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
void R_SDSI_IntHandler0(void * vect)
{
    sdsi_hndl_t   * p_hndl  = 0;
    uint32_t        channel = 0;
    sdsi_cmd_t      cmd;

    channel = 0;
    p_hndl = SDSI_GET_HNDL(channel);
#if (SDSI_CFG_PARAM_CHECKING_ENABLE == 1)
    if (0 == p_hndl)
    {
        return;
    }
#endif /* SDSI_CFG_PARAM_CHECKING_ENABLE == 1 */

    if (r_sdsi_dev_get_int(channel) == SDSI_SUCCESS)
    {
        r_sdsi_dev_get_cmd_info(channel, &cmd);
        if (p_hndl->intsr1)
        {
            if (p_hndl->int_callback)
            {
                (*p_hndl->int_callback)(&cmd);
            }
        }
        if (p_hndl->intsr2 & SDSI_INTENCR2_CDFEN)
        {
            if (p_hndl->int_cd_callback)
            {
                (*p_hndl->int_cd_callback)(SDSI_CD_FALL);
            }
        }
        if (p_hndl->intsr2 & SDSI_INTENCR2_CDREN)
        {
            if (p_hndl->int_cd_callback)
            {
                (*p_hndl->int_cd_callback)(SDSI_CD_RISE);
            }
        }
        if (p_hndl->intsr2 & SDSI_INTENCR2_DTEEN)
        {
            /* Update direct address. */
            if (p_hndl->direct_trans_mode & SDSI_MODE_DIRECT_ADDR_INC)
            {
                r_sdsi_dev_update_direct_trans_adr(channel);
            }
            if (p_hndl->int_dt_callback)
            {
                (*p_hndl->int_dt_callback)(&cmd);
            }
        }
        r_sdsi_dev_clear_int(channel);
    }
    return;
} /* End of function R_SDSI_IntHandler0() */

#endif /* defined(BSP_MCU_RX65N) */

/* End of File */
