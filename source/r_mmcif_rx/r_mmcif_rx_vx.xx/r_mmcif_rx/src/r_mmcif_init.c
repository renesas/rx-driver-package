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
* Copyright (C) 2014(2015-2019) Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/
/**********************************************************************************************************************
* System Name  : MMC Driver
* File Name    : r_mmcif_init.c
* Version      : 1.07
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
*              : 28.12.2016 1.01    Modified hardware lock and hardware unlock.
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


/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/
mmc_mmchndl_t *MMCHandle[(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t))];


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Open
 *****************************************************************************************************************/ /**
 * @brief This is the first function called when this MMCIF driver API is used.
 * @param[in] channel
 *             Channel number : The number of the MMCIF channel used (numbering starts at 0)
 * @param[in] *p_mmc_WorkArea
 *             Pointer to a working area on a 4-byte boundary (area size: 164 bytes)
 * @retval    MMC_SUCCESS              Successful operation
 * @retval    MMC_ERR                  General error
 * @retval    MMC_ERR_CPU_IF           Target microcontroller interface error
 * @retval    MMC_ERR_ADDRESS_BOUNDARY Argument buffer address error
 * @details   This function acquires the MMCIF channel resource specified with the argument channel 
 *            and initializes this MMCIF driver and the MMCIF channel. 
 *            Also, this function exclusively acquires that MMCIF channel resource.\n 
 *            The working area is also retained until MMCIF driver close processing completes, 
 *            and the application must not modify the working area contents.
 * @note      The pins must be set up before this function is called. 
 *            See section 4.4 in the application note for details.\n 
 *            If this function does not complete successfully, do not call any library functions other than 
 *            R_MMCIF_GetVersion(), R_MMCIF_Log() or R_MMCIF_Set_LogHdlAddress().\n 
 *            If this function does complete successfully, the card insertion interrupt may be enabled. 
 *            If the MMC card insertion interrupt is used, after this function has run, 
 *            use the R_MMCIF_Cd_Int() function to enable the card insertion interrupt.\n 
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.\n 
 *            The microcontroller pin states do not change from before to after the execution of this function.
 */
mmc_status_t R_MMCIF_Open(uint32_t channel, void *p_mmc_WorkArea)
{
    mmc_mmchndl_t      *p_hndl = 0;
    int16_t             i = 0;
    uint32_t           *p_ptr = 0;
#ifdef MMC_CFG_USE_FIT
    bool                chk = false;
#endif  /* MMC_CFG_USE_FIT */

    /* Check channel. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_INIT, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /*  Get the MMC hardware resource.  */
#ifdef MMC_CFG_USE_FIT
    chk = r_mmcif_dev_hardware_lock(channel);
    if (false == chk)
    {
        /* Lock has already been acquired by another task. Needs to try again later. */
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_INIT, __LINE__);
        return MMC_ERR;
    }
#endif  /* MMC_CFG_USE_FIT */

    /*  Initialize the work memory.  */
    if (0 == (uint32_t)p_mmc_WorkArea)
    {
        /* Release the MMC hardware resource. */
#ifdef MMC_CFG_USE_FIT
        r_mmcif_dev_hardware_unlock(channel);
#endif  /* MMC_CFG_USE_FIT */
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_INIT, __LINE__);
        MMCHandle[channel] = 0;    /* Release the MMC handle. */
        return MMC_ERR;
    }

    /*  Check the work memory boundary (4-byte unit)  */
    if ((uint32_t)p_mmc_WorkArea & 0x3u)
    {
        /* Release the MMC hardware resource. */
#ifdef MMC_CFG_USE_FIT
        r_mmcif_dev_hardware_unlock(channel);
#endif  /* MMC_CFG_USE_FIT */
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_INIT, __LINE__);
        MMCHandle[channel] = 0;    /* Release the MMC handle. */
        return MMC_ERR_ADDRESS_BOUNDARY;
    }

    /*  Initialize the peripheral module.  */
    if (r_mmcif_dev_init(channel) != MMC_SUCCESS)
    {
        /* Release the MMC hardware resource. */
#ifdef MMC_CFG_USE_FIT
        r_mmcif_dev_hardware_unlock(channel);
#endif  /* MMC_CFG_USE_FIT */
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_INIT, __LINE__);
        MMCHandle[channel] = 0;    /* Release the MMC handle. */
        return MMC_ERR_CPU_IF;
    }

    /* Disable all interrupts. */
    r_mmcif_dev_disable_int(channel);

    p_hndl = (mmc_mmchndl_t *)p_mmc_WorkArea;

    MMCHandle[channel] = p_hndl;

    /* ---- Clear the work memory to '0'. --- */
    p_ptr = (uint32_t *)p_hndl;
    /* WAIT_LOOP */
    for (i = (sizeof(mmc_mmchndl_t) / sizeof(uint32_t)); i > 0; i--)
    {
        *p_ptr++ = 0;
    }

    /* ---- Set MMC register base address. ---- */
    p_hndl->reg_base = (uint32_t)MMC_CFG_IP_BASE[channel];

    /* ---- Initialize the maximum block count. ---- */
    p_hndl->mmcmem_trans_sectors = MMC_MMCMEM_MAX_TRANS;

    /* ==== Command Sequence Force Stop. ==== */
    if (r_mmcif_cmd_seq_force_stop(channel) != MMC_SUCCESS)
    {
        /* Release the MMC hardware resource. */
#ifdef MMC_CFG_USE_FIT
        r_mmcif_dev_hardware_unlock(channel);
#endif  /* MMC_CFG_USE_FIT */
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_INIT, __LINE__);
        MMCHandle[channel] = 0;    /* Release the MMC handle. */
        return MMC_ERR;
    }

    /* ==== Reset the MMCIF. ==== */
    r_mmcif_reset_reg(channel);

    /* ==== Initialize the MMCIF. ==== */
    r_mmcif_init_reg(channel);

    /* Enable all interrupts. */
    r_mmcif_dev_enable_int(channel);

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Close
 *****************************************************************************************************************/ /**
 * @brief This function releases the resources being used by the MMCIF driver.
 * @param[in] channel
 *             Channel number : The number of the MMCIF channel used (numbering starts at 0)
 * @retval    MMC_SUCCESS Successful operation
 * @retval    MMC_ERR     General error
 * @details   This function terminates all MMCIF driver processing and releases the resources for 
 *            the MMCIF channel specified in the argument channel.\n 
 *            That MMCIF channel is set to the module stop state.\n 
 *            After this function is called, the insertion interrupt will be in the disabled state.\n 
 *            The working area specified with the R_MMCIF_Open() function is not used after this function 
 *            has been executed. This are may be used for other purposes.
 * @note      The pins must be set up after this function is called. See section 4.5 in application note for details. 
 *            Before running this function, driver open processing must be performed by the R_MMCIF_Open() function.\n 
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.
 */
mmc_status_t R_MMCIF_Close(uint32_t channel)
{
    /* Check channel. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_INIT, __LINE__);
        return MMC_ERR;
    }

    if (0 == MMCHandle[channel])
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_INIT, __LINE__);
        return MMC_ERR;  /* Not initialized */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* Disable all interrupts. */
    r_mmcif_dev_disable_int(channel);

    /* ==== Command Sequence Force Stop. ==== */
    if (r_mmcif_cmd_seq_force_stop(channel) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_INIT, __LINE__);
        return MMC_ERR;
    }

    /* ==== Reset the MMCIF. ==== */
    r_mmcif_reset_reg(channel);

    /* Finish the peripheral module. */
    r_mmcif_dev_finalize(channel);

    MMCHandle[channel] = 0;     /* Destroy the MMC handle. */

    /* Release the MMC hardware resource. */
#ifdef MMC_CFG_USE_FIT
    r_mmcif_dev_hardware_unlock(channel);
#endif  /* MMC_CFG_USE_FIT */

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Initialize MMC Handle
* Function Name: r_mmcif_init_hndl
* Description  : Initializes the MMC handle members.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : uint32_t           mode                ;   Driver mode
*              : uint32_t           voltage             ;   Working voltage
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_init_hndl(uint32_t channel, uint32_t mode, uint32_t voltage)
{
    mmc_mmchndl_t     *p_hndl = 0;
    int16_t            i = 0;

    p_hndl = MMC_GET_HNDL(channel);
    
    p_hndl->resp_status     = MMC_STATE_IDLE;
    p_hndl->error           = MMC_SUCCESS;
    p_hndl->stop            = 0;
    p_hndl->voltage         = voltage;
    p_hndl->density_type    = 0;
    p_hndl->speed_mode      = 0;
    p_hndl->int_mode        = (uint8_t)(mode & MMC_MODE_HWINT);
    p_hndl->trans_mode      = (uint8_t)(mode & (MMC_MODE_DMA | MMC_MODE_DTC));
    p_hndl->sup_eMMC        = (uint8_t)(mode & MMC_MODE_eMMC);

    if (mode & MMC_MODE_1BIT)
    {
        p_hndl->sup_bus     = MMC_PORT_1BIT;
    }
    else if (mode & MMC_MODE_4BIT)
    {
        p_hndl->sup_bus     = MMC_PORT_4BIT;
    }
    else
    {
        p_hndl->sup_bus     = MMC_PORT_8BIT;
    }

    /* Initialize the MMC handle. */
    p_hndl->ocr[0]          = 0;
    p_hndl->dsr[0]          = 0;
    p_hndl->rca[0]          = MMC_INIT_RCA_NUM;
    p_hndl->resp_r4[0]      = 0;
    p_hndl->resp_r5[0]      = 0;
    /* WAIT_LOOP */
    for (i = 0; i < 4; ++i)
    {
        p_hndl->cid[i]      = 0;
        p_hndl->csd[i]      = 0;
    }

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Initialize MMCIF register
* Function Name: r_mmcif_init_reg
* Description  : Initializes the MMCIF registers.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_init_reg(uint32_t channel)
{
    mmc_mmchndl_t  *p_hndl = 0;
    uint32_t        mmc_ceclkctrl_init = 0;

    p_hndl = MMC_GET_HNDL(channel);

    /* ==== Initialize the MMCIF. ==== */
    mmc_ceclkctrl_init = r_mmcif_dev_ceclkctrl_init();
    MMC_OUTREG(p_hndl, MMC_CECLKCTRL, mmc_ceclkctrl_init);
    MMC_OUTREG(p_hndl, MMC_CEBUFACC, MMC_CEBUFACC_ENDIAN);
    if (MMC_INREG(p_hndl, MMC_CEBUFACC) == MMC_CEBUFACC_ENDIAN)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Reset MMCIF register
* Function Name: r_mmcif_reset_reg
* Description  : Resets the MMCIF registers.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_reset_reg(uint32_t channel)
{
    mmc_mmchndl_t  *p_hndl = 0;

    p_hndl = MMC_GET_HNDL(channel);

    /* ==== Reset the MMCIF. ==== */
    MMC_OUTREG(p_hndl, MMC_CEVERSION, MMC_CEVERSION_ENABLE);        /* Execute software reset. */
    if (MMC_INREG(p_hndl, MMC_CEVERSION) == MMC_CEVERSION_ENABLE)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }
    MMC_OUTREG(p_hndl, MMC_CEVERSION, MMC_CEVERSION_INIT);          /* Software reset cancel (normal operation) */
    if (MMC_INREG(p_hndl, MMC_CEVERSION) == MMC_CEVERSION_INIT)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Command Sequence Force Stop Operation
* Function Name: r_mmcif_cmd_seq_force_stop
* Description  : To stop the command sequence, when the BREAK bit is 0, set it to 1 and then to 0 in succession.
*              : Then confirm that the CEHOSTSTS1.CMDSEQ flag is 0.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_cmd_seq_force_stop(uint32_t channel)
{
    mmc_mmchndl_t  *p_hndl = 0;

    p_hndl = MMC_GET_HNDL(channel);

    /* Stop command sequence force. */
    MMC_OUTREG(p_hndl, MMC_CECMDCTRL, MMC_CECMDCTRL_STOP);
    if (0 == MMC_INREG(p_hndl, MMC_CECMDCTRL))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }
    MMC_OUTREG(p_hndl, MMC_CECMDCTRL, MMC_CECMDCTRL_INIT);
    if (0 == MMC_INREG(p_hndl, MMC_CECMDCTRL))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    /* Check that the command sequence was finished. */
    if ((MMC_INREG(p_hndl, MMC_CEHOSTSTS1) & MMC_CEHOSTSTS1_MASK_CMDSEQ) != 0)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_INIT, __LINE__);
        return MMC_ERR;
    }

    return MMC_SUCCESS;
}


/* End of File */
