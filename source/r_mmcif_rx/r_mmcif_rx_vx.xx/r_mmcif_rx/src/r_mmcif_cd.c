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
* File Name    : r_mmcif_cd.c
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
 * Function Name: R_MMCIF_Cd_Int
 *****************************************************************************************************************/ /**
 * @brief This function sets up the insertion interrupt (including registering the insertion interrupt callback 
 *        function).
 * @param[in] channel
 *             Channel number : The number of the MMCIF channel used (numbering starts at 0)
 * @param[in] enable
 *             Specifies enable/disable of the MMC card insertion interrupt.
 * @param[in] (*callback)(int32_t)
 *            Callback function to be registered.\n 
 *            If a null pointer is specified, no callback function is registered. If a callback function is to be used,
 *            execute this function to register the callback function before an MMC card is inserted.\n 
 *            The MMC_CD pin detection state is stored in the (int32_t).
 * @retval    MMC_SUCCESS Successful operation
 * @retval    MMC_ERR     General error
 * @details   This function sets up the MMC card insertion interrupt and registers a callback function.\n 
 *            The callback function registered by this function is called as a subroutine from the interrupt handler 
 *            when an MMC card insertion interrupt occurs.\n 
 *            Note that the MMC card insertion state can be verified with the R_MMCIF_Get_CardDetection() function 
 *            regardless of the enabled/disabled state of the MMC card insertion interrupt.
 * @note      To enable card detection, set #define MMC_CFG_CHx_CD_ACTIVE to 1.\n 
 *            Initialization by the R_MMCIF_Open() function is required before this function is executed.\n 
 *            After this function has been executed, the MMC card insertion interrupt will be caused by an MMC card
 *            insertion.\n 
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.
 */
mmc_status_t R_MMCIF_Cd_Int(uint32_t channel, int32_t enable, mmc_status_t (*callback)(int32_t))
{
    uint32_t           ul_tmp = 0;
    mmc_mmchndl_t     *p_hndl = 0;

    /* Check channel. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CD, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = MMC_GET_HNDL(channel);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CD, __LINE__);
        return MMC_ERR;  /* Not initialized */
    }
    if ((MMC_CD_INT_ENABLE != enable) && (MMC_CD_INT_DISABLE != enable))
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CD, __LINE__);
        return MMC_ERR;  /* Parameter error */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Check the interrupt setting. */
    if (0 != (p_hndl->int_det_mask & MMC_CEDETECT_MASK_DET))
    {
        r_mmcif_dev_disable_int(channel);
        ul_tmp = MMC_INREG(p_hndl, MMC_CEDETECT);
        ul_tmp &= (uint32_t)~MMC_CEDETECT_DET;
        MMC_OUTREG(p_hndl, MMC_CEDETECT, ul_tmp); /* Clear the insertion and removal bits. */
        if (ul_tmp == MMC_INREG(p_hndl, MMC_CEDETECT))
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }
        r_mmcif_dev_enable_int(channel);
    }

    if (MMC_SUCCESS == r_mmcif_dev_cd_layout(channel))
    {
        if (MMC_CD_INT_ENABLE == enable)
        {
            r_mmcif_set_det_mask(channel, MMC_CEDETECT_MASK_DET);
        }
        else    /* case MMC_CD_INT_DISABLE */
        {
            r_mmcif_clear_det_mask(channel, MMC_CEDETECT_MASK_DET);
        }
    }

    /* ---- Register the callback function. ---- */
    p_hndl->int_cd_callback = callback;

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Get_CardDetection
 *****************************************************************************************************************/ /**
 * @brief This function verifies the MMC Card insertion state.
 * @param[in] channel
 *             Channel number : The number of the MMCIF channel used (numbering starts at 0)
 * @retval    MMC_SUCCESS The MMC_CD pin was at the low level or card detection was invalid.
 * @retval    MMC_ERR     The MMC_CD pin was at the high level.
 * @details   This function verifies the MMC card insertion state.\n 
 *            - When MMC_CFG_CHx_CD_ACTIVE == 1 (card detection enabled)\n
 *            If the MMC_CD pin is low, this function returns MMC_SUCCESS.\n 
 *            If the MMC_CD pin is high, this function returns MMC_ERR.\n 
 *            - When MMC_CFG_CHx_CD_ACTIVE == 0 (card detection disabled)\n
 *            This function will always return MMC_SUCCESS.
 * @note      When using the card insertion detection function, pin setting is necessary after this function is 
 *            executed. See section 4.4 in application note for details. Before running this function, driver open
 *            processing must be performed by the R_MMCIF_Open() function.\n 
 *            When using with card removal detection, pin setting is required before this function is executed. 
 *            See section 4.5 in application note for details.\n 
 *            The MMC_CD pin, which is connected to the MMC card socket CD pin, is used as the MMC card insertion 
 *            detection pin.\n 
 *            In this MMCIF, there is no hardware function to remove the chattering generated when an MMC card is 
 *            inserted. Users should implement card detection processing that takes chattering into consideration.\n 
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.\n 
 *            See section 4.6 in application note for MMC_CD pin handling methods.\n 
 *            After an MMC card has been detected, the processing that provides the power supply voltage to the MMC 
 *            card must be performed.
 */
mmc_status_t R_MMCIF_Get_CardDetection(uint32_t channel)
{
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    mmc_mmchndl_t     *p_hndl = 0;

    /* Check channel. */
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CD, __LINE__);
        return MMC_ERR;
    }

    p_hndl = MMC_GET_HNDL(channel);
    if (0 == p_hndl)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CD, __LINE__);
        return MMC_ERR;  /* Not initialized */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    return r_mmcif_Get_CardDetection(channel);
}


/**********************************************************************************************************************
* Outline      : Check Card Insertion
* Function Name: r_mmcif_Get_CardDetection
* Description  : Checks card insertion.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   A card is inserted.
*              : MMC_ERR                                ;   A card is not inserted.
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_Get_CardDetection(uint32_t channel)
{
    mmc_mmchndl_t     *p_hndl = 0;
    uint32_t           reg = 1;

    p_hndl = MMC_GET_HNDL(channel);

    reg = MMC_INREG(p_hndl, MMC_CEDETECT);

    if (MMC_SUCCESS == r_mmcif_dev_cd_layout(channel))
    {
        reg &= MMC_CEDETECT_MASK_CDSIG;     /* Check CD level. */
    }
    else
    {
        reg = 0;    /* In eMMC, the CD pin check is unnecessary. */
    }
    if (0 == reg)
    {
        return MMC_SUCCESS;  /* Inserted */
    }

    R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CD, __LINE__);
    return MMC_ERR;     /* No card */
}


/**********************************************************************************************************************
* Outline      : Set CE_DETECT interrupt mask
* Function Name: r_mmcif_det_mask
* Description  : Sets int_det_mask depend on the mask bits value.
* Arguments    : uint32_t           channel             ;    MMC Channel No.
*              : uint32_t           mask                ;    Mask bits value
* Return Value : MMC_SUCCESS                            ;    Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_set_det_mask(uint32_t channel, uint32_t mask)
{
    mmc_mmchndl_t    *p_hndl = 0;
    uint32_t          ul_tmp = 0;

    p_hndl = MMC_GET_HNDL(channel);

    r_mmcif_dev_disable_int(channel);

    /* ---- Set int_det_mask. ---- */
    p_hndl->int_det_mask |= mask;

    /* ---- Set hardware mask. ---- */
    ul_tmp = MMC_INREG(p_hndl, MMC_CEDETECT);
    ul_tmp |= (p_hndl->int_det_mask | MMC_CEDETECT_DET);
                                            /* NOTE: If write CEDETECT register, must set CDFALL bit and CDRISE bit. */
    MMC_OUTREG(p_hndl, MMC_CEDETECT, ul_tmp);
    if (MMC_INREG(p_hndl, MMC_CEDETECT) == ul_tmp)
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }

    r_mmcif_dev_enable_int(channel);

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Clear CE_DETECT interrupt mask
* Function Name: r_mmcif_clear_det_mask
* Description  : Clears int_det_mask depend on the mask bits value.
* Arguments    : uint32_t           channel             ;    MMC Channel No.
*              : uint32_t           mask                ;    Mask bits value
* Return Value : MMC_SUCCESS                            ;    Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_clear_det_mask(uint32_t channel, uint32_t mask)
{
    mmc_mmchndl_t    *p_hndl = 0;
    uint32_t          ul_tmp = 0;

    p_hndl = MMC_GET_HNDL(channel);

    r_mmcif_dev_disable_int(channel);

    /* ---- Clear int_det_mask. ---- */
    p_hndl->int_det_mask &= ~mask;

    /* ---- Clear hardware mask. ---- */
    ul_tmp = MMC_INREG(p_hndl, MMC_CEDETECT);
    ul_tmp &= ~MMC_CEDETECT_MASK_DET;
    ul_tmp |= (p_hndl->int_det_mask | MMC_CEDETECT_DET);
                                            /* NOTE: If write CEDETECT register, must set CDFALL bit and CDRISE bit. */
    MMC_OUTREG(p_hndl, MMC_CEDETECT, ul_tmp);
    if (MMC_INREG(p_hndl, MMC_CEDETECT) == ul_tmp)
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }

    r_mmcif_dev_enable_int(channel);

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Get CE_DETECT interrupt elements
* Function Name: r_mmcif_get_det_int
* Description  : Gets CE_DETECT bits examine enabled elements hear after, 
*              : clears CE_DETECT bits save those bits to int_det.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Any interrupt occured
*              : MMC_ERR                                ;   No interrupt occured
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_get_det_int(uint32_t channel)
{
    mmc_mmchndl_t          *p_hndl = 0;
    uint32_t                reg = 0;
    uint32_t                flag = 0;

    p_hndl = MMC_GET_HNDL(channel);

    /* Get CE_DETECT bits. */
    reg = MMC_INREG(p_hndl, MMC_CEDETECT);
    flag =  (reg & (p_hndl->int_det_mask << 8u));
    
    /* Clear CE_DETECT bits. */
    reg &= ~MMC_CEDETECT_DET;
    MMC_OUTREG(p_hndl, MMC_CEDETECT, reg);
    if (MMC_INREG(p_hndl, MMC_CEDETECT) == reg)
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }

    /* Save enabled elements. */
    p_hndl->int_det |= flag;
    if (flag)
    {
        return MMC_SUCCESS;  /* Any interrupt occurred. */
    }

    R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_CD, __LINE__);
    return MMC_ERR;     /* No interrupt occurred. */
}


/* End of File */
