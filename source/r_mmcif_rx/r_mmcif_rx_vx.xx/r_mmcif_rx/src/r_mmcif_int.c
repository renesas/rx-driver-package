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
* File Name    : r_mmcif_int.c
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
*              : 22.11.2019 1.07    Modified comment of API function to Doxygen style.
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes <System Includes> , "Project Includes"
**********************************************************************************************************************/
/* Public interface header file for this package. */
#include "r_mmcif_rx_if.h"
/* Private header file for this package. */
#include "./src/r_mmcif_rx_private.h"


/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/


/**********************************************************************************************************************
* Outline      : Set MMC_CEINTEN Interrupt Mask
* Function Name: r_mmcif_set_int_mask
* Description  : Sets p_hndl->ceinten1 and p_hndl->ceinten2 according to the specified mask1 and mask2.
*              : Ebables interrupt according to the specified mask1 and mask2.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : uint32_t           mask1               ;   MMC_CEINTEN[31:16] bits value
*              : uint32_t           mask2               ;   MMC_CEINTEN[15: 0] bits value
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_set_int_mask(uint32_t channel, uint32_t mask1, uint32_t mask2)
{
    mmc_mmchndl_t       *p_hndl = 0;

    p_hndl = MMC_GET_HNDL(channel);

    r_mmcif_dev_disable_int(channel);

    /* ---- Set the ceinten1 and ceinten2 according to the specified mask1 and mask2. ---- */
    p_hndl->ceinten1 = (p_hndl->ceinten1 | mask1);
    p_hndl->ceinten2 = (p_hndl->ceinten2 | mask2);

    /* ---- Set the MMC_CEINTEN registers. ---- */
    MMC_OUTREG(p_hndl, MMC_CEINTEN, ((p_hndl->ceinten1 << 16) | p_hndl->ceinten2));
    if (MMC_INREG(p_hndl, MMC_CEINTEN) == ((p_hndl->ceinten1 << 16) | p_hndl->ceinten2))
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }

    r_mmcif_dev_enable_int(channel);

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Clear MMC_CEINTEN Interrupt Mask
* Function Name: r_mmcif_clear_int_mask
* Description  : Clears the p_hndl->ceinten1 and p_hndl->ceinten2 according to the specified mask1 and mask2.
*              : Disables interrupt according to the specified mask1 and mask2.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : uint32_t           mask1               ;   MMC_CEINTEN[31:16] bits value
*              : uint32_t           mask2               ;   MMC_CEINTEN[15: 0] bits value
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_clear_int_mask(uint32_t channel, uint32_t mask1, uint32_t mask2)
{
    mmc_mmchndl_t       *p_hndl = 0;

    p_hndl = MMC_GET_HNDL(channel);

    r_mmcif_dev_disable_int(channel);

    /* ---- Clear the ceinten1 and ceinten2 according to the specified mask1 and mask2. ---- */
    p_hndl->ceinten1 = (p_hndl->ceinten1 & ~mask1);
    p_hndl->ceinten2 = (p_hndl->ceinten2 & ~mask2);

    /* ---- Clear the MMC_CEINTEN registers. ---- */
    MMC_OUTREG(p_hndl, MMC_CEINTEN, ((p_hndl->ceinten1 << 16) | p_hndl->ceinten2));
    if (MMC_INREG(p_hndl, MMC_CEINTEN) == ((p_hndl->ceinten1 << 16) | p_hndl->ceinten2))
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }

    r_mmcif_dev_enable_int(channel);

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Clear p_hndl->ceint1 and p_hndl->ceint2 bits
* Function Name: r_mmcif_clear_info
* Description  : Clears the p_hndl->ceint1 and p_hndl->ceint2 according to the specified clear_ceint1 and clear_ceint2.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : uint32_t           clear_ceint1        ;   ceint1 clear bits value
*              : uint32_t           clear_ceint2        ;   ceint2 clear bits value
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_clear_info(uint32_t channel, uint32_t clear_ceint1, uint32_t clear_ceint2)
{
    mmc_mmchndl_t       *p_hndl = 0;

    p_hndl = MMC_GET_HNDL(channel);

    r_mmcif_dev_disable_int(channel);

    /* ---- Clears the p_hndl->ceint1 and p_hndl->ceint2 according to 
            the specified clear_ceint1 and clear_ceint2. ---- */
    p_hndl->ceint1 = (p_hndl->ceint1 & ~clear_ceint1);
    p_hndl->ceint2 = (p_hndl->ceint2 & ~clear_ceint2);

    r_mmcif_dev_enable_int(channel);

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Clear MMC_CEINT Register Bits
* Function Name: r_mmcif_clear_info_reg
* Description  : Clears the p_hndl->ceint1 and p_hndl->ceint2the CEINT register according to the specified
*              : clear_ceint1 and clear_ceint2.
*              : Clears the CEINT register according to the specified clear_ceint1 and clear_ceint2.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : uint32_t           clear_ceint1        ;   ceint1 clear bits value
*              : uint32_t           clear_ceint2        ;   ceint2 clear bits value
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_clear_info_reg(uint32_t channel, uint32_t clear_ceint1, uint32_t clear_ceint2)
{
    mmc_mmchndl_t       *p_hndl = 0;
    volatile uint32_t       reg = 0;
    uint32_t reg_tmp;

    p_hndl = MMC_GET_HNDL(channel);

    r_mmcif_dev_disable_int(channel);

    /* ---- Clear the p_hndl->ceint1 and p_hndl->ceint2 according to 
            the specified clear_ceint1 and clear_ceint2. ---- */
    p_hndl->ceint1 = (p_hndl->ceint1 & ~clear_ceint1);
    p_hndl->ceint2 = (p_hndl->ceint2 & ~clear_ceint2);

    /* ---- Clear the MMC_CEINT register. ---- */
    reg = ~((clear_ceint1 << 16) | clear_ceint2);
    MMC_OUTREG(p_hndl, MMC_CEINT, reg);
    reg_tmp = reg;
    if (MMC_INREG(p_hndl, MMC_CEINT) == reg_tmp)
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }

    r_mmcif_dev_enable_int(channel);

    return MMC_SUCCESS;

}


/**********************************************************************************************************************
* Outline      : Get MMC_CEINT Interrupt Sources.
* Function Name: r_mmcif_get_int
* Description  : Gets the CEINT Interrupt sources.
*              : Clears the CEINT bits and stores those bits to p_hndl->ceinten11 and p_hndl->ceinten2.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : MMC_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_get_int(uint32_t channel)
{
    mmc_mmchndl_t       *p_hndl = 0;
    uint32_t                reg = 0;

    p_hndl = MMC_GET_HNDL(channel);

    /* Get the MMC_CEINT register. */
    reg = (MMC_INREG(p_hndl, MMC_CEINT) & ((p_hndl->ceinten1 << 16u) | p_hndl->ceinten2));

    /* Clear the MMC_CEINT register. */
    MMC_OUTREG(p_hndl, MMC_CEINT, (uint32_t)~reg);
    if (MMC_INREG(p_hndl, MMC_CEINT) == reg)
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }

    /* Store the interrupt sources to the p_hndl->ceint1 and p_hndl->ceint2. */
    p_hndl->ceint1 |= reg >> 16u;
    p_hndl->ceint2 |= reg & 0x0000ffff;
    if (reg)
    {
        return MMC_SUCCESS;      /* Any interrupt occurred */
    }

    return MMC_ERR;         /* No interrupt occurred */
}


/**********************************************************************************************************************
* Outline      : Check MMC_CEINT Interrupt Sources
* Function Name: r_mmcif_get_intstatus
* Description  : Checks MMC_CEINT interrupt sources.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : MMC_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_get_intstatus(uint32_t channel)
{
    mmc_mmchndl_t       *p_hndl = 0;

    /* Check channel. */
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_INT, __LINE__);
        return MMC_ERR;
    }

    p_hndl = MMC_GET_HNDL(channel);
    if (0 == p_hndl)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_INT, __LINE__);
        return MMC_ERR;  /* Not initialized */
    }

    if (p_hndl->int_mode)
    {
        /* ---- Hardware interrupt mode ---- */
        if (p_hndl->ceint1 || p_hndl->ceint2)
        {
            return MMC_SUCCESS;
        }
        else
        {
            return MMC_ERR;
        }
    }

    /* ---- Polling mode ---- */
    return r_mmcif_get_int(channel);
}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_IntCallback
 *****************************************************************************************************************/ /**
 * @brief This function registers an MMC protocol status interrupt callback function.
 * @param[in] channel
 *             Channel number : The number of the MMCIF channel used (numbering starts at 0)
 * @param[in] (*callback)(int32_t)
 *            Callback function to be registered.\n 
 *            If a null pointer is specified, no callback function is registered. If a callback function is to be used,
 *            register a callback function before the R_MMCIF_Mount() function is executed.\n 
 *            The value 0 is always stored in (int32_t).
 * @retval    MMC_SUCCESS Successful operation
 * @retval    MMC_ERR     General error
 * @details   This function registers an MMC protocol status interrupt callback function.\n 
 *            The callback function registered by this function is called as a subroutine from the interrupt handler 
 *            when an interrupt occurs due to a change in the MMC protocol status (ACCIO or ERRIO).
 * @note      Initialization by the R_MMCIF_Open() function is required before this function is executed.\n 
 *            The stack wait state clear operation and other processing is performed in registered callback function.\n
 *            The callback function registered by this function differs from the MMC card insertion interrupt 
 *            callback function.\n 
 *            The callback function registered by this function is not called when an MMC card insertion 
 *            interrupt occurs.\n 
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.
 */
mmc_status_t R_MMCIF_IntCallback(uint32_t channel, mmc_status_t (*callback)(int32_t))
{
    mmc_mmchndl_t       *p_hndl = 0;

    /* Check channel. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_INT, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = MMC_GET_HNDL(channel);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_INT, __LINE__);
        return MMC_ERR;  /* Not initialized */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl->int_callback = callback;

    return MMC_SUCCESS;
}


/* End of File */
