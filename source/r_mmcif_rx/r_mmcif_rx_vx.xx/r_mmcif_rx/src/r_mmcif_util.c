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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/
/**********************************************************************************************************************
* System Name  : MMC Driver
* File Name    : r_mmcif_util.c
* Version      : 1.07.00
* Device       : RX64M (LQFP-176)
* Abstract     : API & Sub module
* Tool-Chain   : For RX64M Group
*              :  e2 studio (Version 7.4.0)
* OS           : not use
* H/W Platform : RSK board for RX64M
* Description  : Interface file for MMC API for RX
* Limitation   : None
**********************************************************************************************************************/
/**********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 03.09.2014 1.00    First Release
*              : 20.05.2019 1.05    Added support for GNUC and ICCRX.
*                                   Fixed coding style.
*              : 30.07.2019 1.06    Add WAIT_LOOP.
*              : 22.11.2019 1.07    Modified comment of API function to Doxygen style.
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes <System Includes> , "Project Includes"
**********************************************************************************************************************/
/* Public interface header file for this package. */
#include "r_mmcif_rx_if.h"
/* Private header file for this package. */
#include "./src/r_mmcif_rx_private.h"
/* ==== Uses FIT LongQ module headef file ==== */
#ifdef MMC_CFG_LONGQ_ENABLE
#include "r_longq_if.h"
#endif /* MMC_CFG_LONGQ_ENABLE */


/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/
static uint32_t next;                                       /* Volume ID Number                                     */
#ifdef MMC_CFG_LONGQ_ENABLE
static longq_hdl_t     p_mmc_long_que;                      /* LongQ handler                                        */
#endif /* MMC_CFG_LONGQ_ENABLE */


/**********************************************************************************************************************
* Outline      : Control MMC Clock
* Function Name: r_mmcif_set_clock
* Description  : Supplies or halts MMC clock.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : uint32_t           clock               ;   MMC clock frequency
*              : int32_t            enable              ;   MMC_CLOCK_ENABLE or MMC_CLOCK_DISABLE
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : MMC_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_set_clock(uint32_t channel, uint32_t clock, int32_t enable)
{
    mmc_mmchndl_t      *p_hndl = 0;
    uint32_t            div = 0;
    uint32_t            reg = 0;
    int32_t             i = 0;

    p_hndl = MMC_GET_HNDL(channel);

    reg = MMC_INREG(p_hndl, MMC_CECLKCTRL);
    reg &= MMC_CECLKCTRL_MASK_TIMEOUT;
    /* WAIT_LOOP */
    for (i = 0; i < MMC_CLKDIVEN_LOOP_COUNT; i++)
    {
        if ((MMC_INREG(p_hndl, MMC_CECLKCTRL) & MMC_CECLKCTRL_MASK_MMCBUSBSY) == 0)
        {
            break;
        }
    }
    if (MMC_CLKDIVEN_LOOP_COUNT == i)
    {
        if (MMC_CLOCK_DISABLE == enable)
        {
            /* ==== Command Sequence Force Stop. ==== */
            r_mmcif_cmd_seq_force_stop(channel);

            /* ==== Reset the MMCIF. ==== */
            /* NOTE: Reset all MMCIF registers of CEINT, CENINTEN, CEBUFACC(Disable DMAC and DTC) and so on. */
            r_mmcif_reset_reg(channel);

            /* ==== Initialize the MMCIF. ==== */
            r_mmcif_init_reg(channel);
        }
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_CBSY_ERROR);
        MMC_OUTREG(p_hndl, MMC_CECLKCTRL, reg);     /* Halt */
        if (0 == MMC_INREG(p_hndl, MMC_CECLKCTRL))
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }
        return MMC_ERR;
    }

    if (MMC_CLOCK_ENABLE == enable)
    {
        /* Convert clock frequency to clock divide ratio. */
        div = r_mmcif_dev_get_clockdiv(clock);
        if (div > MMC_DIV_1024)
        {
           R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
           r_mmcif_set_err(channel, MMC_ERR_CPU_IF);
           return MMC_ERR;
        }
        MMC_OUTREG(p_hndl, MMC_CECLKCTRL, reg);     /* Halt */
        if (MMC_CECLKCTRL_MASK_CLKEN != MMC_INREG(p_hndl, MMC_CECLKCTRL))
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }
        reg |= (uint32_t)(uint32_t)(div | MMC_CECLKCTRL_MASK_CLKEN);
        MMC_OUTREG(p_hndl, MMC_CECLKCTRL, reg);
        if (MMC_CECLKCTRL_MASK_CLKEN == MMC_INREG(p_hndl, MMC_CECLKCTRL))
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }
    }
    else
    {
        MMC_OUTREG(p_hndl, MMC_CECLKCTRL, reg);     /* Halt */
        if (0 == MMC_INREG(p_hndl, MMC_CECLKCTRL))
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }
    }
    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Control MMC Bus Width
* Function Name: r_mmcif_set_port
* Description  : Changes the MMC bus width
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : int32_t            port                ;   MMC_PORT_1BIT, MMC_PORT_4BIT, MMC_PORT_8BIT or MMC_PORT_CD
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_set_port(uint32_t channel, int32_t port)
{
    mmc_mmchndl_t   *p_hndl = 0;

    p_hndl = MMC_GET_HNDL(channel);

    p_hndl->if_mode = (uint8_t)port;

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Get Bit Information
* Function Name: r_mmcif_bit_search
* Description  : Checks the bits of argument (data) from LSB.
* Arguments    : uint32_t           data                 ;   Checked data
* Return Value : -1                                      ;   no bit has 1'b
*              : other                                   ;   bit number has 1'b
*----------------------------------------------------------------------------------------------------------------------
* Notes        : All 16bits value can be applied
**********************************************************************************************************************/
int32_t r_mmcif_bit_search(uint32_t data)
{
    int32_t i = 0;

    /* WAIT_LOOP */
    for (i = 15; 0 <= i; i--)
    {
        if (data & 1u)
        {
            return i;
        }
        data >>= 1;
    }
    return -1;
}


/**********************************************************************************************************************
* Outline      : Set Errors Information
* Function Name: r_mmcif_set_err
* Description  : Sets the error information (=error) to MMC Handle member (=p_hndl->error).
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : mmc_status_t   error                   ;   Error information
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_set_err(uint32_t channel, mmc_status_t error)
{
    mmc_mmchndl_t  *p_hndl = 0;

    p_hndl = MMC_GET_HNDL(channel);
    if (MMC_SUCCESS == p_hndl->error)
    {
        p_hndl->error = error;
    }
    return MMC_SUCCESS;
}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Get_ModeStatus
 *****************************************************************************************************************/ /**
 * @brief This function acquires the transfer mode status.
 * @param[in] channel
 *             Channel number : The number of the MMCIF channel used (numbering starts at 0)
 * @param[out] *p_mode
 *             Mode status information storage pointer (1 byte).
 * @retval    MMC_SUCCESS Successful operation
 * @retval    MMC_ERR     General error
 * @details   This function acquires the transfer mode status and stores it in the mode status information storage
 *            pointer.
 * @note      Both initialization processing by the R_MMCIF_Open() function and mount processing by the R_MMCIF_Mount()
 *            function are required prior to executing this function.\n 
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.
 */
mmc_status_t R_MMCIF_Get_ModeStatus(uint32_t channel, uint8_t * p_mode)
{
    mmc_mmchndl_t * p_hndl = 0;

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = MMC_GET_HNDL(channel);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;  /* Not initialized */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* ---- Check the card is mounted. ---- */
    if (MMC_MOUNT_UNLOCKED_CARD != p_hndl->mount)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;
    }

    /*  Check the parameter.  */
    if (0 == (uint32_t)p_mode)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;
    }
    else
    {
        if (p_hndl->trans_mode & MMC_MODE_DMA)
        {
            *p_mode = MMC_MODE_DMA;
        }
        else if (p_hndl->trans_mode & MMC_MODE_DTC)
        {
            *p_mode = MMC_MODE_DTC;
        }
        else
        {
            *p_mode = MMC_MODE_SW;
        }
    }

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Get_CardStatus
 *****************************************************************************************************************/ /**
 * @brief This function acquires the card status information.
 * @param[in] channel
 *             Channel number : The number of the MMCIF channel used (numbering starts at 0)
 * @param[out] *p_mmc_CardStatus
 *             Card status information structure pointer (See section 3.12 in application note for structure details.)
 * @retval    MMC_SUCCESS Successful operation
 * @retval    MMC_ERR     General error
 * @details   This function gets the MMC card status information and stores it in a card status information structure.
 * @note      Both initialization processing by the R_MMCIF_Open() function and mount processing by the R_MMCIF_Mount()
 *            function are required prior to executing this function.\n 
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.
 */
mmc_status_t R_MMCIF_Get_CardStatus(uint32_t channel, mmc_card_status_t *p_mmc_CardStatus)
{
    mmc_mmchndl_t *p_hndl = 0;

    /* Check channel. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = MMC_GET_HNDL(channel);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;     /* Not initialized */
    }

    if (0 == p_mmc_CardStatus)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;
    }
    else
    {
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */
        p_mmc_CardStatus->csd_structure    = p_hndl->csd_structure;
        p_mmc_CardStatus->speed_mode       = p_hndl->speed_mode;
        p_mmc_CardStatus->card_sector_size = p_hndl->card_sector_size;
        p_mmc_CardStatus->csd_spec         = p_hndl->csd_spec;
        p_mmc_CardStatus->if_mode          = p_hndl->if_mode;
        p_mmc_CardStatus->density_type     = p_hndl->density_type;
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Get_CardInfo
 *****************************************************************************************************************/ /**
 * @brief This function acquires the MMC register information.
 * @param[in] channel
 *             Channel number : The number of the MMCIF channel used (numbering starts at 0)
 * @param[out] *p_mmc_CardReg
 *             MMC register information structure pointer
 * @retval    MMC_SUCCESS Successful operation
 * @retval    MMC_ERR     General error
 * @details   This function acquires the MMC register information and stores it in the MMC register information 
 *            structure.
 * @note      Both initialization processing by the R_MMCIF_Open() function and mount processing by the R_MMCIF_Mount()
 *            function are required prior to executing this function.\n 
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.
 */
mmc_status_t R_MMCIF_Get_CardInfo(uint32_t channel, mmc_card_reg_t *p_mmc_CardReg)
{
    mmc_mmchndl_t      *p_hndl = 0;
    uint32_t            i = 0;

    /* Check channel. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = MMC_GET_HNDL(channel);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;     /* Not initialized */
    }

    if (0 == p_mmc_CardReg)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;
    }
    else
    {
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */
        p_mmc_CardReg->ocr[0] = p_hndl->ocr[0];
        p_mmc_CardReg->dsr[0] = p_hndl->dsr[0];
        p_mmc_CardReg->rca[0] = p_hndl->rca[0];
        /* WAIT_LOOP */
        for (i = 0; i < 4; ++i)
        {
            p_mmc_CardReg->cid[i] = p_hndl->cid[i];
            p_mmc_CardReg->csd[i] = p_hndl->csd[i];
        }
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Get Card Size
* Function Name: r_mmcif_get_size
* Description  : Gets the MMC size.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : uint32_t           area                ;   Memory area (bit0:user area, bit1:protect area)
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_get_size(uint32_t channel)
{
    mmc_mmchndl_t      *p_hndl = 0;
    uint32_t            c_mult = 0;
    uint32_t            c_size = 0;
    uint32_t            read_len =0;

    p_hndl = MMC_GET_HNDL(channel);

    if (1 == p_hndl->density_type)      /* High density card (over 2GB) */
    {
        p_hndl->card_sector_size = p_hndl->sec_count;
    }
    else                                /* Normal density card */
    {
        /* ---- READ BL LEN ---- */
        read_len = ((p_hndl->csd[1] & 0x000f0000ul) >> 16u);

        /* ---- C_SIZE_MULT ---- */
        c_mult = ((p_hndl->csd[2] & 0x00038000ul) >> 15u);

        /* ---- C_SIZE ---- */
        c_size = ((p_hndl->csd[1] & 0x000003fful) << 2u) | 
            ((p_hndl->csd[2] & 0xc0000000ul) >> 30u);

        /* ---- Calculate the number of all sectors. ---- */
        p_hndl->card_sector_size = ((uint32_t)(c_size + 1) * 
            ((uint32_t)(1 << (c_mult+2))) * 
                ((uint32_t)(1 << read_len))) / MMC_TRANS_BLOCK_SIZE;
    }
    return MMC_SUCCESS;
}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Get_ErrCode
 *****************************************************************************************************************/ /**
 * @brief This function acquires the driver error codes.
 * @param[in] channel
 *             Channel number : The number of the MMCIF channel used (numbering starts at 0)
 * @return    Error code: See section 2.10 in application note.
 * @details   This function returns the error codes that occur when the R_MMCIF_Mount(), R_MMCIF_Read_Memory(), 
 *            and R_MMCIF_Write_Memory() function are executed. Note that the error code is cleared when a library
 *            function is executed again.\n 
 *            Note : The R_MMCIF_Read_Memory_Software_Trans() and R_MMCIF_Write_Memory_Software_Trans() functions
 *            operate similarly.
 * @note      Initialization by the R_MMCIF_Open() function is required before this function is executed.\n 
 *            Use this function when an application program needs to acquire the MMCIF driver error code.
 */
mmc_status_t R_MMCIF_Get_ErrCode(uint32_t channel)
{
    mmc_mmchndl_t *p_hndl = 0;

    /* Check channel. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = MMC_GET_HNDL(channel);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;     /* Not initialized */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    return p_hndl->error;
}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Control
 *****************************************************************************************************************/ /**
 * @brief This function performs driver control processing.
 * @param[in] channel
 *             Channel number : The number of the MMCIF channel used (numbering starts at 0)
 * @param[in] *p_mmc_Cmd
 *             Control information structure
 * @return    MMC_SUCCESS:          Successful operation.\n
 *            Other than the above: Error termination(See section 2.10 in application note for details).
 * @details   This is an MMC control utility.\n 
 *            See section 3.10 in application note for the commands that can be controlled.
 * @note      Before running this function, driver open processing must be performed by the R_MMCIF_Open() function 
 *            and initialization by the R_MMCIF_Mount() function.\n 
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.
 */
mmc_status_t R_MMCIF_Control(uint32_t channel, mmc_cmd_t *p_mmc_Cmd)
{
    mmc_mmchndl_t  *p_hndl = 0;
    mmc_status_t   ret = MMC_SUCCESS;

    /* Check channel. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = MMC_GET_HNDL(channel);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;     /* Not initialized */
    }

    /*  Check the parameter.  */
    if (0 == (uint32_t)p_mmc_Cmd)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    switch (p_mmc_Cmd->cmd)
    {
        case MMC_SET_STOP:          /* Stop the operations compulsory. */
            p_hndl->stop = 1;
            ret = MMC_SUCCESS;
        break;

        default:
            ret = MMC_ERR;
        break;
    }
    return ret;
}


/**********************************************************************************************************************
* Outline      : Set Memory
* Function Name: r_mmcif_memset
* Description  : Sets MMC to the specified data of the specified size from start address.
* Arguments    : uint8_t            *p_ptr              ;   Start address of memory
*              : uint8_t            data                ;   Filling data
*              : uint32_t           cnt                 ;   Filling size
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_memset(uint8_t *p_ptr, uint8_t data, uint32_t cnt)
{
    /* WAIT_LOOP */
    while (cnt--)
    {
        *p_ptr++ = data;
    }
    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Copy Memory
* Function Name: r_mmcif_memcpy
* Description  : Copies data of the specified size from the specified source address to the specified destination address.
* Arguments    : uint8_t            *p_dst              ;   Destination address
*              : uint8_t            *p_src              ;   Source address
*              : uint32_t           cnt                 ;   Copy size
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_memcpy(uint8_t *p_dst, uint8_t *p_src, uint32_t cnt)
{
    /* WAIT_LOOP */
    while (cnt--)
    {
        *p_dst++ = *p_src++;
    }
    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Create Volume ID Number
* Function Name: r_mmcif_rand
* Description  : Get the Volume ID Number. The Volume ID number is created by pseudo random number.
* Arguments    : None
* Return Value : uint32_t                               ;   created Volume ID Number
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t r_mmcif_rand(void)
{
    next = next * 1103515245L + 12345;
    return (uint32_t)next;
}


/**********************************************************************************************************************
* Outline      : Set Volume ID Number
* Function Name: r_mmcif_srand
* Description  : Sets the initial value of Volume ID Number.
* Arguments    : uint32_t           seed                ;   Initial seting value
* Return Value : None
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
void r_mmcif_srand(uint32_t seed)
{
    if (0 == next)
    {
        next = seed;
    }
}


/**********************************************************************************************************************
* Outline      : Wait for Busy End
* Function Name: r_mmcif_wait_rbusy
* Description  : Waits for busy end.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : uint32_t           time                ;   Busy wait interval
* Return Value : MMC_SUCCESS                            ;   Busy finished
*              : MMC_ERR                                ;   Busy
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_wait_rbusy(uint32_t channel, uint32_t time)
{
    mmc_mmchndl_t      *p_hndl = 0;
    uint32_t            i = 0;

    p_hndl = MMC_GET_HNDL(channel);
    /* WAIT_LOOP */
    for (i = 0; i < time; ++i)
    {
        if (MMC_SUCCESS == r_mmcif_send_cmd_arg(channel, MMC_CMD13, MMC_RESP_R1, p_hndl->rca[0]))
        {
            if (MMC_STATE_TRAN == (p_hndl->resp_status & MMC_RES_STATE))        /* Transfer state */
            {
                return MMC_SUCCESS;
            }
        }
        else    /* MMC error */
        {
            /* NOTE: In the case of the error, the host reset MMCIF and CMD13 issue again. */
            r_mmcif_dev_disable_int(channel);
            r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);
            r_mmcif_cmd_seq_force_stop(channel);
            r_mmcif_reset_reg(channel);
            r_mmcif_init_reg(channel);
            r_mmcif_set_clock(channel, p_hndl->csd_tran_speed, MMC_CLOCK_ENABLE);
            r_mmcif_dev_enable_int(channel);
        }
        if (R_MMCIF_Get_CardDetection(channel) != MMC_SUCCESS)
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
            break;
        }
        r_mmcif_dev_wait(channel, 1);
    }
    r_mmcif_set_err(channel, MMC_ERR_HOST_TOE);
    return MMC_ERR;
}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Get_BuffRegAddress
 *****************************************************************************************************************/ /**
 * @brief This function acquires the address of the MMCIF data register (CEDATA).
 * @param[in] channel
 *             Channel number : The number of the MMCIF channel used (numbering starts at 0)
 * @param[out] *p_reg_buff
 *             Data register (CEDATA) address pointer
 * @retval    MMC_SUCCESS Successful operation
 * @retval    MMC_ERR     General error
 * @details   This function acquires the address of the data register (CEDATA) and stores it in a buffer.\n 
 *            This function is used, for example, when specifying the data register address when using 
 *            DMAC or DTC transfers.
 * @note      Initialization by the R_MMCIF_Open() function is required before this function is executed.\n 
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.
 */
mmc_status_t R_MMCIF_Get_BuffRegAddress(uint32_t channel, uint32_t *p_reg_buff)
{
    mmc_mmchndl_t  *p_hndl = 0;

    /* Check channel. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = MMC_GET_HNDL(channel);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;     /* Not initialized */
    }

    if (0 == p_reg_buff)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    *p_reg_buff = (uint32_t)(p_hndl->reg_base + MMC_CEDATA);

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Stop Operation
* Function Name: r_mmcif_stop
* Description  : Stops a command sequence compulsory and issues CMD12.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : MMC_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_stop(uint32_t channel)
{
    mmc_mmchndl_t   *p_hndl = 0;

    p_hndl = MMC_GET_HNDL(channel);

    /* Disable all interrupts. */
    r_mmcif_dev_disable_int(channel);

    /* ---- Halt the clock. ---- */
    r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);

    /* ==== Command Sequence Force Stop. ==== */
    r_mmcif_cmd_seq_force_stop(channel);

    /* ==== Reset the MMCIF. ==== */
    /* NOTE: Reset all MMCIF registers of CEINT, CENINTEN, CEBUFACC(Disable DMAC and DTC) and so on. */
    r_mmcif_reset_reg(channel);

    /* ==== Initialize the MMCIF. ==== */
    r_mmcif_init_reg(channel);

    /* ---- Supply the clock (Data-transfer ratio). ---- */
    r_mmcif_set_clock(channel, p_hndl->csd_tran_speed, MMC_CLOCK_ENABLE);

    /* Enable all interrupts. */
    r_mmcif_dev_enable_int(channel);

    /* ---- Force the card to stop transmission. (Issue CMD12.) ---- */
    if(r_mmcif_send_cmd_arg(channel, MMC_CMD12, MMC_RESP_R1, 0) != MMC_SUCCESS)
    {
        r_mmcif_dev_disable_int(channel);
        r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);
        r_mmcif_cmd_seq_force_stop(channel);
        r_mmcif_reset_reg(channel);
        r_mmcif_init_reg(channel);
        r_mmcif_set_clock(channel, p_hndl->csd_tran_speed, MMC_CLOCK_ENABLE);
        r_mmcif_dev_enable_int(channel);
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_UTIL, __LINE__);
        return MMC_ERR;
    }

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Read or Write Error Operation
* Function Name: r_mmcif_read_write_error
* Description  : Executes the read or write error operation.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_read_write_error(uint32_t channel)
{
    mmc_mmchndl_t           *p_hndl = 0;
    volatile mmc_status_t   error = MMC_SUCCESS;

    p_hndl = MMC_GET_HNDL(channel);

    error = p_hndl->error;

    R_MMCIF_Set_DmacDtc_Trans_Flg(channel, MMC_SET_TRANS_STOP);

    /* ---- Perform stop operation. ---- */
    /* NOTE: Stop the command sequence by force and issue CMD12. */
    r_mmcif_stop(channel);

    /* Disable the DMA/DTC. */
    /* Already disable the DMAC/DTC at r_mmcif_stop(). */

    /* ---- Check the state. (Issue CMD13.) ---- */
    /* NOTE: In the case of the state except "tran", send CMD12 and change the state "tran". */
    if (r_mmcif_wait_rbusy(channel, MMC_TIMEOUT_RESP) != MMC_SUCCESS)
    {
        /* ---- Force the card to stop transmission. (Issue CMD12.) ---- */
        if(r_mmcif_send_cmd_arg(channel, MMC_CMD12, MMC_RESP_R1, 0) != MMC_SUCCESS)
        {
            r_mmcif_dev_disable_int(channel);
            r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);
            r_mmcif_cmd_seq_force_stop(channel);
            r_mmcif_reset_reg(channel);
            r_mmcif_init_reg(channel);
            r_mmcif_set_clock(channel, p_hndl->csd_tran_speed, MMC_CLOCK_ENABLE);
            r_mmcif_dev_enable_int(channel);
        }
    }

    /* ---- Check the state. (Issue CMD13.) ---- */
    /* NOTE: It is necessary to issue CMD13 if the error bit of the card status is set.
             Because the error bit is cleared. */
    r_mmcif_send_cmd_arg(channel, MMC_CMD13, MMC_RESP_R1, p_hndl->rca[0]);

    /* ---- Disable all interrupts. ---- */
    r_mmcif_clear_int_mask(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);

    /* ---- Clear error bits. ---- */
    r_mmcif_clear_info(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);

    p_hndl->error = error;
    return p_hndl->error;
}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_GetVersion
 *****************************************************************************************************************/ /**
 * @brief This function acquires the version information for the driver.
 * @return    Upper 2 bytes:  Major version (decimal).\n
 *            Lower 2 bytes:  Minor version (decimal).
 * @details   This function returns the driver version information.
 * @note      Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.
 */
uint32_t  R_MMCIF_GetVersion(void)
{
    uint32_t const version = ((RX_MMC_API_VERSION_MAJOR << 16) | RX_MMC_API_VERSION_MINOR);

    return version;
}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Set_LogHdlAddress
 *****************************************************************************************************************/ /**
 * @brief This function sets the LONGQ FIT module handler address.
 * @param[in] user_long_que
 *             LONGQ FIT module handler address
 * @retval    MMC_SUCCESS Successful operation
 * @details   This function sets the LONGQ FIT module handler address in the MMCIF driver.
 * @note      This function performs the preparatory processing required to acquire an error log using the LONGQ 
 *            FIT module. This processing should be performed before the R_MMCIF_Open() function is called.\n 
 *            The LONGQ FIT module needs to be embedded in the application as a separate operation.\n 
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.\n
 *            If the MMC_CFG_LONGQ_ENABLE disable and this function is called, this function does nothing.
 */
mmc_status_t R_MMCIF_Set_LogHdlAddress(uint32_t user_long_que)
{
#ifdef MMC_CFG_LONGQ_ENABLE
    p_mmc_long_que = (longq_hdl_t)user_long_que;
#endif /* MMC_CFG_LONGQ_ENABLE */
    return MMC_SUCCESS;
}


#ifdef MMC_CFG_LONGQ_ENABLE
/**********************************************************************************************************************
* Outline      : Store Debugging Information
* Function Name: r_mmcif_log
* Description  : Stores the debugging information.
* Arguments    : uint32_t           flg                 ;   Breakpoint processing
*              : uint32_t           fid                 ;   MMC driver file No.
*              : uint32_t           line                ;   MMC driver line No.
* Return Value : 0                                      ;   Successful operation
*              : 1                                      ;   Error
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t r_mmcif_log(uint32_t flg, uint32_t fid, uint32_t line)
{
    longq_err_t err = LONGQ_SUCCESS;
    uint32_t    ul_tmp = 0;

    /* Long to Byte */
    ul_tmp = 0;                                 /* log   Reserved       */
    ul_tmp = (ul_tmp | (line << 8));            /* log   LINE           */
    ul_tmp = (ul_tmp | (fid  << 21));           /* log   FID FILE No.   */
    ul_tmp = (ul_tmp | (MMC_DRIVER_ID << 27));  /* log   FID DRIVER No. */
    ul_tmp = (ul_tmp | (flg  << 31));           /* log   Breakpoint processing  */

    err = R_LONGQ_Put(p_mmc_long_que, ul_tmp);
    if (err != LONGQ_SUCCESS)
    {
        return 1;
    }

    /*  Breakpoint processing */
    if (0x80000000 == (ul_tmp & 0x80000000))
    {
        return 1;
    }
    return 0;
}
#endif /* MMC_CFG_LONGQ_ENABLE */


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Log
 *****************************************************************************************************************/ /**
 * @brief This function acquires an error log.
 * @param[in] flg
 *             0x00000001 (Fixed value)
 * @param[in] fid
 *             0x0000003f (Fixed value)
 * @param[in] line
 *             0x00001fff (Fixed value)
 * @retval    0 Successful operation
 * @details   This function acquires an error log.\n 
 *            To terminate error log acquisition, call this function.
 * @note      The LONGQ FIT module needs to be embedded in the application as a separate operation.\n 
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.\n
 *            If the MMC_CFG_LONGQ_ENABLE disable and this function is called, this function does nothing.
 */
uint32_t R_MMCIF_Log(uint32_t flg, uint32_t fid, uint32_t line)
{
    R_MMCIF_Log_Func(flg, fid, line);
    return 0;
}


/* End of File */
