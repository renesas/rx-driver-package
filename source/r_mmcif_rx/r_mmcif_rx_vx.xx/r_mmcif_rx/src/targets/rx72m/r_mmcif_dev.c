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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/
/**********************************************************************************************************************
* System Name  : MMC Driver
* File Name    : r_mmcif_dev.c
* Version      : 1.07
* Device       : RX72M
* Abstract     : API & Sub module
* Tool-Chain   : For RX72M Group
*              :  e2 studio (Version 7.4.0.00)
* OS           : not use
* H/W Platform : RSK board for RX72M
* Description  : Interface file for MMC API for RX
* Limitation   : None
**********************************************************************************************************************/
/**********************************************************************************************************************
* History      : DD.MM.YYYY Version Description  
*              : 30.07.2019 1.06    First Release
*              : 22.11.2019 1.07    Added support for atomic control.
*                                   Modified comment of API function to Doxygen style.
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "platform.h"
#if defined(BSP_MCU_RX72M)

#include "r_mmcif_rx_if.h"
#include "./src/r_mmcif_rx_private.h"
#include "./src/targets/rx72m/r_mmcif_rx_target.h"


/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
#define MMC_TIMER_MAX_CH        (8)
#define MMC_TIMER_CH_FLG        (0)
#define MMC_TIMER_CH_COUNT      (1)
#define MMC_TIMER_CH_MAX_COUNT  (2)


/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/
static volatile uint32_t gs_mmc_int_dmacdtcflg[(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t))];
static volatile uint32_t gs_mmc_timer_cnt_out[MMC_TIMER_MAX_CH][MMC_TIMER_CH_MAX_COUNT + 1];

static mmc_status_t r_mmcif_dev_start_timer(uint32_t channel, uint32_t msec);
static mmc_status_t r_mmcif_dev_check_timer(uint32_t channel);
static void         r_mmcif_dev_end_timer(uint32_t channel);


/* MMC_MBFAI */
R_BSP_PRAGMA_STATIC_INTERRUPT(r_mmcif_mbfai_isr, VECT(MMCIF, MBFAI))
R_BSP_ATTRIB_STATIC_INTERRUPT void r_mmcif_mbfai_isr(void)
{
    R_MMCIF_Set_DmacDtc_Trans_Flg(MMC_CH0, MMC_SET_TRANS_STOP);
}


/**********************************************************************************************************************
* Outline      : Initialize MMC
* Function Name: r_mmcif_dev_init
* Description  : Initializes hardware to access MMC.
* Arguments    : uint32_t           channel         ;   MMC Channel No.
* Return Value : MMC_SUCCESS                        ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_dev_init(uint32_t channel)
{
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */

#ifdef MMC_CFG_USE_FIT
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);  /* Enable writing to MSTP registers. */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
    MSTP_MMCIF = 0;               /* MMC ON */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
    if (0 == MSTP_MMCIF)
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);   /* Disable writing to MSTP registers. */
#else
    SYSTEM.PRCR.WORD = 0xA502;
    if (0 == SYSTEM.PRCR.WORD)
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
    MSTP_MMCIF = 0;               /* MMC ON */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
    if (0 == MSTP_MMCIF)
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }
    SYSTEM.PRCR.WORD = 0xA500;
    if (0 == SYSTEM.PRCR.WORD)
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }
    
#endif  /* MMC_CFG_USE_FIT */

    if (MMC_CH0 == channel)
    {
#if defined(MMC_CFG_CH0_INCLUDED)
#ifdef MMC_CFG_USE_FIT
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_MMCIF_CDETIO, R_MMCIF_Int_Handler0);
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_MMCIF_ERRIO,  R_MMCIF_Int_Handler0);
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_MMCIF_ACCIO,  R_MMCIF_Int_Handler0);
#endif /* MMC_CFG_USE_FIT */

#endif  /* MMC_CFG_CH0_INCLUDED */
    }
    else    /* MMC_CH1 */
    {
#if defined(MMC_CFG_CH1_INCLUDED)
#ifdef MMC_CFG_USE_FIT
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_MMCIF_CDETIO, R_MMCIF_Int_Handler1);
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_MMCIF_ERRIO,  R_MMCIF_Int_Handler1);
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_MMCIF_ACCIO,  R_MMCIF_Int_Handler1);
#endif /* MMC_CFG_USE_FIT */

#endif  /* MMC_CFG_CH1_INCLUDED */
    }

    return MMC_SUCCESS;
}

/**********************************************************************************************************************
* Outline      : Finalize MMC Host IP
* Function Name: r_mmcif_dev_finalize
* Description  : Finalizes to access MMC.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_dev_finalize(uint32_t channel)
{
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */

#ifdef MMC_CFG_USE_FIT
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);  /* Enable writing to MSTP registers. */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
    MSTP_MMCIF = 1;               /* MMC OFF */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
    if (1 == MSTP_MMCIF)
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);   /* Disable writing to MSTP registers. */
#else
    SYSTEM.PRCR.WORD = 0xA502;
    if (0 == SYSTEM.PRCR.WORD)
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
    MSTP_MMCIF = 1;               /* MMC OFF */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
    if (1 == MSTP_MMCIF)
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }
    SYSTEM.PRCR.WORD = 0xA500;
    if (0 == SYSTEM.PRCR.WORD)
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }
    
#endif  /* MMC_CFG_USE_FIT */

    if (MMC_CH0 == channel)
    {
#if defined(MMC_CFG_CH0_INCLUDED)
#ifdef MMC_CFG_USE_FIT
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_MMCIF_CDETIO, FIT_NO_FUNC);
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_MMCIF_ERRIO,  FIT_NO_FUNC);
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_MMCIF_ACCIO,  FIT_NO_FUNC);
#endif /* MMC_CFG_USE_FIT */

#endif  /* MMC_CFG_CH0_INCLUDED */
    }
    else    /* MMC_CH1 */
    {
#if defined(MMC_CFG_CH1_INCLUDED)
#ifdef MMC_CFG_USE_FIT
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_MMCIF_CDETIO, FIT_NO_FUNC);
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_MMCIF_ERRIO,  FIT_NO_FUNC);
        R_BSP_InterruptWrite(BSP_INT_SRC_BL1_MMCIF_ACCIO,  FIT_NO_FUNC);
#endif /* MMC_CFG_USE_FIT */

#endif  /* MMC_CFG_CH1_INCLUDED */
    }

    return MMC_SUCCESS;
}

/**********************************************************************************************************************
* Outline      : Read from MMC
* Function Name: r_mmcif_dev_read_data
* Description  : Reads the sector data from the data register. 
* Arguments    : uint8_t            *p_buff             ;   Read data buffer
*              : uint32_t           reg_addr            ;   Register address
*              : int32_t            num                 ;   Number of data
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_dev_read_data(uint8_t *p_buff, uint32_t reg_addr, int32_t num)
{
    int32_t  i = 0;
    uint32_t *p_reg = (uint32_t *)(reg_addr);
    uint32_t *p_ptr = (uint32_t *)p_buff;
    uint32_t tmp = 0;
    
    /* Revised, num++ -> num+=3 because the argument num may be set to 1, */
    /*          and num/4 value must be greater than 0.                   */ 
    num = num + 3;
    num = num / 4;

    if ((uint32_t)p_buff & 0x3)
    {
        /* WAIT_LOOP */
        for (i = num ; i > 0 ; i--)
        {
#if (MMC_BIG_ENDIAN)
            tmp = *p_reg;
            *p_buff++ = (uint8_t)(tmp >> 24);
            *p_buff++ = (uint8_t)(tmp >> 16);
            *p_buff++ = (uint8_t)(tmp >> 8);
            *p_buff++ = (uint8_t)tmp;
#else
            tmp = *p_reg;
            *p_buff++ = (uint8_t)tmp;
            *p_buff++ = (uint8_t)(tmp >> 8);
            *p_buff++ = (uint8_t)(tmp >> 16);
            *p_buff++ = (uint8_t)(tmp >> 24);
#endif
        }
    }
    else
    {
        /* WAIT_LOOP */
        for (i = num ; i > 0 ; i--)
        {
            *p_ptr++ = *p_reg;
        }
    }

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Write to MMC
* Function Name: r_mmcif_dev_write_data
* Description  : Writes the sector data in the data register. 
* Arguments    : uint8_t            *p_buff             ;   Write data buffer
*              : uint32_t           reg_addr            ;   Register address
*              : int32_t            num                 ;   Number of data
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_dev_write_data(uint8_t *p_buff, uint32_t reg_addr, int32_t num)
{
    int32_t  i = 0;
    uint32_t *p_reg = (uint32_t *)(reg_addr);
    uint32_t *p_ptr = (uint32_t *)p_buff;
    uint32_t tmp = 0;

    /* Revised, num++ -> num+=3 because the argument num may be set to 1, */
    /*          and num/4 value must be greater than 0.                   */ 
    num = num + 3;
    num = num / 4;
    if ((uint32_t)p_buff & 0x3)
    {
        /* WAIT_LOOP */
        for (i = num ; i > 0 ; i--)
        {
#if (MMC_BIG_ENDIAN)
            tmp =  (uint32_t)(*p_buff++ << 24);
            tmp |= (uint32_t)(*p_buff++ << 16);
            tmp |= (uint32_t)(*p_buff++ << 8);
            tmp |= (uint32_t)(*p_buff++);
            *p_reg = tmp;
#else
            tmp =  (uint32_t)(*p_buff++);
            tmp |= (uint32_t)(*p_buff++ << 8);
            tmp |= (uint32_t)(*p_buff++ << 16);
            tmp |= (uint32_t)(*p_buff++ << 24);
            *p_reg = tmp;
#endif
        }
    }
    else
    {
        /* WAIT_LOOP */
        for (i = num ; i > 0 ; i--)
        {
            *p_reg = *p_ptr++;
        }
    }

    return MMC_SUCCESS;

}


/**********************************************************************************************************************
* Outline      : Get Clock Dividing Ratio
* Function Name: r_mmcif_dev_get_clockdiv
* Description  : Gets Clock dividing ratio.
* Arguments    : int32_t            clock               ;   Clock frequency
* Return Value : uint32_t                               ;   Clock dividing ratio
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t r_mmcif_dev_get_clockdiv(int32_t clock)
{
    uint32_t div = 0;

    switch(clock)
    {
        case MMC_CLK_52MHz:
            div = MMC_CFG_DIV_HIGH_SPEED;
        break;
        case MMC_CLK_26MHz:
        case MMC_CLK_20MHz:
            /* 26MHz 20MHz It contains. */
            div = MMC_CFG_DIV_BACKWARD_COM_SPEED;
        break;
        case MMC_CLK_10MHz:
        case MMC_CLK_5MHz:
        case MMC_CLK_1MHz:
        case MMC_CLK_400kHz:
            /* 10MHz 5MHz 1MHz 400kHz It contains. */
            div = MMC_CFG_DIV_INIT_SPEED;
        break;
        default:
            div = MMC_CFG_DIV_INIT_SPEED;
        break;
    }

    return div;
}


/**********************************************************************************************************************
* Outline      : Wait MMC Card Interrupt
* Function Name: r_mmcif_dev_int_wait
* Description  : Starts timer and waits for an interrupt generation.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : int32_t            time                ;   Timeout (msec).
* Return Value : MMC_SUCCESS                            ;   Interrupt detection
*              : MMC_ERR                                ;   Timeout
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_dev_int_wait(uint32_t channel, uint32_t time)
{
    mmc_status_t ret = MMC_SUCCESS;

    if (r_mmcif_dev_start_timer(channel, time) == MMC_ERR)
    {
        return MMC_ERR;
    }

    /* WAIT_LOOP */
    while (1)
    {
        /* Get an interrupt generation. */
        if (MMC_SUCCESS == r_mmcif_get_intstatus(channel))
        {
            ret = MMC_SUCCESS;
            break;
        }

        /* Timeout? */
        if (MMC_ERR == r_mmcif_dev_check_timer(channel))
        {
            ret = MMC_ERR;
            break;
        }
    }

    r_mmcif_dev_end_timer(channel);

    return ret;
}


/**********************************************************************************************************************
* Outline      : Wait
* Function Name: r_mmcif_dev_wait
* Description  : Starts timer and waits.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : int32_t            time                ;   Timeout (msec).
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_dev_wait(uint32_t channel, uint32_t time)
{
    if (r_mmcif_dev_start_timer(channel, time) == MMC_ERR)
    {
        return MMC_ERR;
    }

    /* WAIT_LOOP */
    while (1)
    {
        /* Timeout? */
        if (MMC_ERR == r_mmcif_dev_check_timer(channel))
        {
            break;
        }
    }

    r_mmcif_dev_end_timer(channel);

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Disable ICU
* Function Name: r_mmcif_dev_disable_int
* Description  : Disables the Interrupt Request Enable (IENn).
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_dev_disable_int(uint32_t channel)
{
#ifdef MMC_CFG_USE_FIT
#if (MMC_CFG_DRIVER_MODE & MMC_MODE_HWINT)
    uint32_t ipl = 0;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
#endif  /* (MMC_CFG_DRIVER_MODE & MMC_MODE_HWINT) */
#endif /* MMC_CFG_USE_FIT */

    if (MMC_CH0 == channel)
    {
#if defined(MMC_CFG_CH0_INCLUDED)
        R_BSP_InterruptRequestDisable(VECT(MMCIF,MBFAI));
        if (0 == IEN(MMCIF,MBFAI))
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }
#if (MMC_CFG_DRIVER_MODE & MMC_MODE_HWINT)
        /* ---- Disable interrupt request (IENn) of IER. ---- */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
        ICU.GENBL1.BIT.EN8  = 0;       /* MMC ACCI */
        ICU.GENBL1.BIT.EN7  = 0;       /* MMC ERRI */
        ICU.GENBL1.BIT.EN6  = 0;       /* MMC CDTEI */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
        if (0 == ICU.GENBL1.BIT.EN6)
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }
#ifdef MMC_CFG_USE_FIT
        ipl = 0;
        R_BSP_InterruptControl(BSP_INT_SRC_BL1_MMCIF_CDETIO, BSP_INT_CMD_GROUP_INTERRUPT_DISABLE, (void *)&ipl);
#else
        R_BSP_InterruptRequestDisable(VECT(ICU,GROUPBL1));
        IPR(ICU,GROUPBL1)  = 0;
#endif /* MMC_CFG_USE_FIT */
#endif  /* (MMC_CFG_DRIVER_MODE & MMC_MODE_HWINT) */
        IPR(MMCIF,MBFAI)    = 0;
        if (0 == IPR(MMCIF,MBFAI))
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }
#endif  /* MMC_CFG_CH0_INCLUDED */
    }
    else    /* MMC_CH1 */
    {
#if defined(MMC_CFG_CH1_INCLUDED)
        R_BSP_InterruptRequestDisable(VECT(MMCIF,MBFAI));
        if (0 == IEN(MMCIF,MBFAI))
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }
    #if (MMC_CFG_DRIVER_MODE & MMC_MODE_HWINT)
        /* ---- Disable interrupt request (IENn) of IER. ---- */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
        ICU.GENBL1.BIT.EN8  = 0;       /* MMC ACCI */
        ICU.GENBL1.BIT.EN7  = 0;       /* MMC ERRI */
        ICU.GENBL1.BIT.EN6  = 0;       /* MMC CDTEI */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
        if (0 == ICU.GENBL1.BIT.EN6)
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }
#ifdef MMC_CFG_USE_FIT
        ipl = 0;
        R_BSP_InterruptControl(BSP_INT_SRC_BL1_MMCIF_CDETIO, BSP_INT_CMD_GROUP_INTERRUPT_DISABLE, (void *)&ipl);
#else
        R_BSP_InterruptRequestDisable(VECT(ICU,GROUPBL1));
        IPR(ICU,GROUPBL1)  = 0;
#endif /* MMC_CFG_USE_FIT */
#endif  /* (MMC_CFG_DRIVER_MODE & MMC_MODE_HWINT) */
        IPR(MMCIF,MBFAI)    = 0;
        if (0 == IPR(MMCIF,MBFAI))
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }
#endif  /* MMC_CFG_CH1_INCLUDED */
    }

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Enable ICU
* Function Name: r_mmcif_dev_enable_int
* Description  : Enables the Interrupt Request Enable (IENn).
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_dev_enable_int(uint32_t channel)
{
#if (MMC_CFG_DRIVER_MODE & MMC_MODE_HWINT)
#ifdef MMC_CFG_USE_FIT
    uint32_t ipl = 0;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
#endif /* MMC_CFG_USE_FIT */
#endif  /* (MMC_CFG_DRIVER_MODE & MMC_MODE_HWINT) */

    if (MMC_CH0 == channel)
    {
#if defined(MMC_CFG_CH0_INCLUDED)
        IPR(MMCIF,MBFAI)    = MMC_CFG_CH0_INT_LEVEL_DMADTC;
#if (MMC_CFG_DRIVER_MODE & MMC_MODE_HWINT)
        /* ---- Enable interrupt request (IENn) of IER. ---- */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
        ICU.GENBL1.BIT.EN6  = 1;       /* MMC CDTEI */
        ICU.GENBL1.BIT.EN7  = 1;       /* MMC ERRI */
        ICU.GENBL1.BIT.EN8  = 1;       /* MMC ACCI */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
        if (1 == ICU.GENBL1.BIT.EN8)
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }
#ifdef MMC_CFG_USE_FIT
        ipl = MMC_CFG_CH0_INT_LEVEL;
        R_BSP_InterruptControl(BSP_INT_SRC_BL1_MMCIF_CDETIO, BSP_INT_CMD_GROUP_INTERRUPT_ENABLE, (void *)&ipl);
#else
        IPR(ICU,GROUPBL1)  = MMC_CFG_CH0_INT_LEVEL;
        R_BSP_InterruptRequestEnable(VECT(ICU,GROUPBL1));
#endif  /* MMC_CFG_USE_FIT */
#endif  /* (MMC_CFG_DRIVER_MODE & MMC_MODE_HWINT) */
        R_BSP_InterruptRequestEnable(VECT(MMCIF,MBFAI));
        if (1 == IEN(MMCIF,MBFAI))
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }
#endif  /* MMC_CFG_CH0_INCLUDED */
    }
    else    /* MMC_CH1 */
    {
#if defined(MMC_CFG_CH1_INCLUDED)
        IPR(MMCIF,MBFAI)    = MMC_CFG_CH1_INT_LEVEL_DMADTC;
#if (MMC_CFG_DRIVER_MODE & MMC_MODE_HWINT)
        /* ---- Enable interrupt request (IENn) of IER. ---- */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
        ICU.GENBL1.BIT.EN6  = 1;       /* MMC CDTEI */
        ICU.GENBL1.BIT.EN7  = 1;       /* MMC ERRI */
        ICU.GENBL1.BIT.EN8  = 1;       /* MMC ACCI */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif  /* ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6) */
        if (1 == ICU.GENBL1.BIT.EN8)
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }
#ifdef MMC_CFG_USE_FIT
        ipl = MMC_CFG_CH1_INT_LEVEL;
        R_BSP_InterruptControl(BSP_INT_SRC_BL1_MMCIF_CDETIO, BSP_INT_CMD_GROUP_INTERRUPT_ENABLE, (void *)&ipl);
#else
        IPR(ICU,GROUPBL1)  = MMC_CFG_CH1_INT_LEVEL;
        R_BSP_InterruptRequestEnable(VECT(ICU,GROUPBL1));
#endif  /* MMC_CFG_USE_FIT */
#endif  /* (MMC_CFG_DRIVER_MODE & MMC_MODE_HWINT) */
        R_BSP_InterruptRequestEnable(VECT(MMCIF,MBFAI));
        if (1 == IEN(MMCIF,MBFAI))
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }
#endif  /* MMC_CFG_CH1_INCLUDED */
    }

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Transfer data by DMAC/DTC
* Function Name: r_mmcif_dev_dmacdtc_trans
* Description  : Waits for the DMAC/DTC transfer completion.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : int32_t            cnt                 ;   Number of data
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_dev_dmacdtc_trans(uint32_t channel, int32_t cnt)
{
    mmc_mmchndl_t  *p_hndl = 0;
    uint32_t           time = 0;

    p_hndl = MMC_GET_HNDL(channel);

    /* ---- Check DMAC/DTC transfer end.  --- */
    /* Timeout value depends on transfer size. */
    time = cnt * 100;

    if (time < 1000)            /* 1s */
    {
        time = 1000;
    }

    if (r_mmcif_dev_start_timer(channel, time) == MMC_ERR)
    {
        return MMC_ERR;
    } 

    /* WAIT_LOOP */
    while (1)
    {
        /* Check timeout. */
        if (MMC_ERR == r_mmcif_dev_check_timer(channel))
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_DEV_PORT, __LINE__);
            r_mmcif_set_err(channel, MMC_ERR_HOST_TOE);
            break;
        }
        
        /* Check error. */
        if (r_mmcif_get_intstatus(channel) == MMC_SUCCESS)
        {
            if (p_hndl->ceint2 & MMC_CEINT2_ALL_ERR)
            {
                R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_DEV_PORT, __LINE__);
                r_mmcif_check_info2_err(channel);
                break;
            }
        }
        
        /* Check DMAC/DTC transfer end */
        if (MMC_SET_TRANS_STOP == gs_mmc_int_dmacdtcflg[channel])
        {
            break;
        }

        /* ---- Check the stop flag. ---- */
        if (p_hndl->stop)
        {
            p_hndl->stop = 0;
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_DEV_PORT, __LINE__);
            p_hndl->error = MMC_ERR_STOP;   /* Set an error code forcibly. */
            break;
        }
    }

    r_mmcif_dev_end_timer(channel);

    return p_hndl->error;

}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Set_DmacDtc_Trans_Flg
 *****************************************************************************************************************/ /**
 * @brief This function sets the DMAC/DTC transfer complete flag.
 * @param[in] channel
 *             Channel number : The number of the MMCIF channel used (numbering starts at 0)
 * @param[in] flg
 *             DMAC/DTC transfer complete flag : MMC_SET_TRANS_STOP
 * @retval    MMC_SUCCESS Successful operation
 * @retval    MMC_ERR     General error (channel error)
 * @details   This function sets the DMAC/DTC transfer complete flag.\n 
 *            See Section 3.21 in application note for details.
 * @note      Both initialization processing by the R_MMCIF_Open() function and mount processing by the R_MMCIF_Mount()
 *            function are required prior to executing this function.\n 
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.
 */
mmc_status_t R_MMCIF_Set_DmacDtc_Trans_Flg(uint32_t channel, mmc_enum_trans_t flg)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    gs_mmc_int_dmacdtcflg[channel] = flg;

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Clock Control Register Init
* Function Name: r_mmcif_dev_ceclkctrl_init
* Description  : Clock Control Register Init.
* Arguments    : void
* Return Value : None
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t r_mmcif_dev_ceclkctrl_init(void)
{
    return (MMC_CFG_TIMEOUT_TRANS | MMC_CFG_TIMEOUT_RESBUSY | MMC_CFG_TIMEOUT_RES);
}


/**********************************************************************************************************************
* Outline      : CD Terminal layout check
* Function Name: r_mmcif_dev_cd_layout
* Description  : CD Terminal layout check.
* Arguments    : uint32_t           channel              ;   MMCIF Channel No.
* Return Value : MMCIF_SUCCESS                           ;   layout ACTIVE
*              : MMCIF_ERR                               ;   layout no ACTIVE
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_dev_cd_layout(uint32_t channel)
{
    mmc_status_t ret = MMC_ERR;

#if defined(MMC_CFG_CH0_INCLUDED)
    if (MMC_CH0 == channel)
    {
        if (MMC_CFG_CH0_CD_ACTIVE)
        {
            ret = MMC_SUCCESS;
        }
    }
#endif  /* #if defined(MMC_CFG_CH0_INCLUDED) */

#if defined(MMC_CFG_CH1_INCLUDED)
    if (MMC_CH1 == channel)
    {
        if (MMC_CFG_CH1_CD_ACTIVE)
        {
            ret = MMC_SUCCESS;
        }
    }
#endif  /* #if defined(MMC_CFG_CH1_INCLUDED) */

    return ret;
}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Int_Handler0
 *****************************************************************************************************************/ /**
 * @brief This function is the MMCIF interrupt handler.
 * @param[in] *vect
 *             Vector table
 * @details   This function is the interrupt handler for the MMCIF driver.\n 
 *            This function is already embedded in the system as the interrupt factor processing routine 
 *            for the MMCIF.\n 
 *            When an MMC card insertion interrupt setup callback function and a status verification interrupt callback
 *            function are registered, these callback functions will be called from this function.
 * @note      Both initialization processing by the R_MMCIF_Open() function and mount processing by the R_MMCIF_Mount()
 *            function are required prior to executing this function.\n 
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.
 */
void R_MMCIF_Int_Handler0(void * vect)
{
    mmc_mmchndl_t       *p_hndl = 0;
    uint32_t                int_isr = 0;
    int32_t                 cd = 0;
    uint32_t                channel = 0;

    channel = 0;
    p_hndl = MMC_GET_HNDL(channel);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        return;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Check group interrupt source register. */
    int_isr = ICU.GRPBL1.LONG;

    /* Check card detection interrupt. */
    if (int_isr & MMC_CH0_INT_CDETIO)
    {
        if (MMC_SUCCESS == r_mmcif_get_det_int(channel))
        {
            if (p_hndl->int_cd_callback)
            {
                if (p_hndl->int_det & MMC_CEDETECT_INS)
                {
                    cd = MMC_CD_INSERT; /* Insert */
                }
                else
                {
                    cd = MMC_CD_REMOVE; /* Remove */
                }
                
                (*p_hndl->int_cd_callback)(cd);
            }
        }
    }

    /* Normal operation or Error/timeout interrupt */
    if (int_isr & (MMC_CH0_INT_ACCIO | MMC_CH0_INT_ERRIO))
    {
        if (MMC_SUCCESS == r_mmcif_get_int(channel))
        {
            if (p_hndl->int_callback)
            {
                (*p_hndl->int_callback)(0);
            }
        }
    }
}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Int_Handler1
 *****************************************************************************************************************/ /**
 * @brief This function is the MMCIF interrupt handler.
 * @param[in] *vect
 *             Vector table
 * @details   This function is the interrupt handler for the MMCIF driver.\n 
 *            This function is already embedded in the system as the interrupt factor processing routine 
 *            for the MMCIF.\n 
 *            When an MMC card insertion interrupt setup callback function and a status verification interrupt callback
 *            function are registered, these callback functions will be called from this function.
 * @note      Both initialization processing by the R_MMCIF_Open() function and mount processing by the R_MMCIF_Mount()
 *            function are required prior to executing this function.\n 
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.
 */
void R_MMCIF_Int_Handler1(void * vect)
{
    mmc_mmchndl_t       *p_hndl = 0;
    uint32_t                int_isr = 0;
    int32_t                 cd = 0;
    uint32_t                channel = 0;

    channel = 0;
    p_hndl = MMC_GET_HNDL(channel);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        return;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Check group interrupt source register. */
    int_isr = ICU.GRPBL1.LONG;

    /* Check card detection interrupt. */
    if (int_isr & MMC_CH1_INT_CDETIO)
    {
        if (MMC_SUCCESS == r_mmcif_get_det_int(channel))
        {
            if (p_hndl->int_cd_callback)
            {
                if (p_hndl->int_det & MMC_CEDETECT_INS)
                {
                    cd = MMC_CD_INSERT; /* Insert */
                }
                else
                {
                    cd = MMC_CD_REMOVE; /* Remove */
                }
                
                (*p_hndl->int_cd_callback)(cd);
            }
        }
    }

    /* Normal operation or Error/timeout interrupt */
    if (int_isr & (MMC_CH1_INT_ACCIO | MMC_CH1_INT_ERRIO))
    {
        if (MMC_SUCCESS == r_mmcif_get_int(channel))
        {
            if (p_hndl->int_callback)
            {
                (*p_hndl->int_callback)(0);
            }
        }
    }
}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_1ms_Interval
 *****************************************************************************************************************/ /**
 * @brief This function increments the MMCIF driver's internal timer counter.
 * @details   The internal timer counter is incremented each time this function is called.
 * @note      The application must call this function once each millisecond. However, this is not required if the timer
 *            functionality has been replaced by the r_mmcif_dev_int_wait() and r_mmcif_dev_wait() functions in 
 *            r_mmcif_dev.c.\n 
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.
 */
void R_MMCIF_1ms_Interval(void)
{
    uint32_t channel = 0;

    /* WAIT_LOOP */
    for (channel = 0; channel < MMC_TIMER_MAX_CH; channel++)
    {
        if (gs_mmc_timer_cnt_out[channel][MMC_TIMER_CH_FLG] != 0)
        {
            gs_mmc_timer_cnt_out[channel][MMC_TIMER_CH_COUNT]++;
        }
    }

    return;
}


/**********************************************************************************************************************
* Outline      : MMC Start timer for timeout
* Function Name: r_mmcif_dev_start_timer
* Description  : Sets timer for checking timeout and start timer.
* Arguments    : uint32_t           channel             ;   Timer Channel No.
*              : uint32_t           msec                ;   timeout(msec order)
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : MMC_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
static mmc_status_t r_mmcif_dev_start_timer(uint32_t channel, uint32_t msec)
{

    if (MMC_TIMER_MAX_CH <= channel)
    {
        return MMC_ERR;
    }

    /* ---- Check argument ---- */
    if ((0 >= msec) || (msec > 0x0fffffff))
    {
        return MMC_ERR;
    }
    gs_mmc_timer_cnt_out[channel][MMC_TIMER_CH_FLG] = 1;
    gs_mmc_timer_cnt_out[channel][MMC_TIMER_CH_COUNT] = 0;
    gs_mmc_timer_cnt_out[channel][MMC_TIMER_CH_MAX_COUNT] = (uint32_t)(msec);

    return MMC_SUCCESS;

}


/**********************************************************************************************************************
* Outline      : MMC Check timeout
* Function Name: r_mmcif_dev_check_timer
* Description  : Checks timeout to set r_mmcif_dev_start_timer function.
*              : If timeout,return MMC_ERR.In the case of others, return MMC_SUCCESS.
* Arguments    : uint32_t           channel             ;   Timer Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : MMC_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
static mmc_status_t r_mmcif_dev_check_timer(uint32_t channel)
{
    mmc_status_t ret = MMC_SUCCESS;
    uint32_t gs_mmc_timer_cnt_out1 = gs_mmc_timer_cnt_out[channel][MMC_TIMER_CH_COUNT];
    uint32_t gs_mmc_timer_cnt_out2 = gs_mmc_timer_cnt_out[channel][MMC_TIMER_CH_MAX_COUNT];

    /* ---- Check compare ---- */
    if (gs_mmc_timer_cnt_out1 >= gs_mmc_timer_cnt_out2)
    {
        ret = MMC_ERR;
    }
    else
    {
        ret = MMC_SUCCESS;
    }

    return ret;
}


/**********************************************************************************************************************
* Outline      : MMC End timer for timeout
* Function Name: r_mmcif_dev_end_timer
* Description  : Stops timer to set r_mmcif_dev_start_timer function.
* Arguments    : uint32_t           channel             ;   Timer Channel No.
* Return Value : None
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
static void r_mmcif_dev_end_timer(uint32_t channel)
{
    gs_mmc_timer_cnt_out[channel][MMC_TIMER_CH_FLG] = 0;

    return;
}

#ifdef MMC_CFG_USE_FIT
/**********************************************************************************************************************
* Outline      : Hardware Lock
* Function Name: r_mmcif_dev_hardware_lock
* Description  : Hardware Lock.
* Arguments    : uint32_t           channel     ; Channel No.
* Return Value : true                           ; Lock was acquired.
*              : false                          ; Lock was not acquired.
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
bool r_mmcif_dev_hardware_lock(uint32_t channel)
{
    return R_BSP_HardwareLock((mcu_lock_t)(BSP_LOCK_MMC + channel));
} /* End of function r_mmcif_dev_hardware_lock() */


/**********************************************************************************************************************
* Outline      : Hardware UnLock
* Function Name: r_mmcif_dev_hardware_unlock
* Description  : Hardware Lock.
* Arguments    : uint32_t           channel     ; Channel No.
* Return Value : true                           ; Lock was released.
*              : false                          ; Lock was not released.
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
bool r_mmcif_dev_hardware_unlock(uint32_t channel)
{
    return R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_MMC + channel));
} /* End of function r_mmcif_dev_hardware_unlock() */
#endif /* MMC_CFG_USE_FIT */


#endif /* defined(BSP_MCU_RX72M) */

/* End of File */
