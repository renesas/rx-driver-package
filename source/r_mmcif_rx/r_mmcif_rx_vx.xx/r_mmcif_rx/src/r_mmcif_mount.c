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
* File Name    : r_mmcif_mount.c
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
*              : 28.12.2016 1.01    The processing to restore the EXT_CSD register changed it to the specifications
*              :                    to carry out with the MMC SPEC V4.xx or more.
*              : 20.05.2019 1.05    Added support for GNUC and ICCRX.
*                                   Fixed coding style.
*              : 30.07.2019 1.06    Add WAIT LOOP.
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
static uint8_t          gs_extcsd[sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)][MMC_EXTCSD_BYTE];

static mmc_status_t r_mmcif_card_init_rca(uint32_t channel);
static mmc_status_t r_mmcif_card_change_buswidth(uint32_t channel, int8_t sup_bus);
static mmc_status_t r_mmcif_check_pattern(uint32_t channel, uint8_t *pat, uint8_t width);
static mmc_status_t r_mmcif_set_speed(uint32_t channel);
static mmc_status_t r_mmcif_restore_ext_csd(uint32_t channel);

/**********************************************************************************************************************
 * Function Name: R_MMCIF_Mount
 *****************************************************************************************************************/ /**
 * @brief This function initializes the MMC and transitions the state from the mountable state to the driver 
 *        idle state.
 * @param[in] channel
 *             Channel number : The number of the MMCIF channel used (numbering starts at 0)
 * @param[in] *p_mmc_Config
 *             Structure that holds the operating settings (See section 3.4 in application note for structure details.)
 * @return    MMC_SUCCESS:             Successful operation.\n
 *            MMC_SUCCESS_LOCKED_CARD: Successful operation, and, furthermore, MMC is in the locked state.\n
 *            Other than the above:    Error termination(See section 2.10 in application note for details).
 * @details   This function performs MMC mount processing. Execute this function after detecting an MMC card.\n 
 *            When the return value is MMC_SUCCESS, the MMC will have transitioned to the transfer state (tran) and 
 *            MMC read and write access will be possible. When the return value is MMC_SUCCESS_LOCKED_CARD, the MMC 
 *            will have transitioned to the transfer state (tran) but MMC read and write access will not be possible. 
 *            The locked state must be cleared by other means.
 * @note      This MMCIF driver discriminates between High-speed mode and Backward-compatible mode when mounting.\n 
 *            The pins must be set up before executing this function. See section 4.4 in application note for details. 
 *            Also, initialization using the R_MMCIF_Open() function is required before executing this function.\n 
 *            If this function returns an error, after setting the hardware to the unmounted state by calling the 
 *            R_MMCIF_Unmount() function, perform the mount processing again.\n 
 *            After mounting has completed normally, unmounting must be performed before performing another mount 
 *            operation.\n 
 *            When voltage in p_mmc_Config is set to an arbitrary value in the range 2.7 to 3.6 V, the output voltage 
 *            will be taken to be in the 2.7 to 3.6 V range.\n 
 *            If the R_MMCIF_Cd_Int() function is used, set MMC_MODE_HWINT as the p_mmc_Config mode status.
 */
mmc_status_t R_MMCIF_Mount(uint32_t channel, mmc_cfg_t *p_mmc_Config)
{
    mmc_mmchndl_t      *p_hndl = 0;
    uint32_t            ul_tmp = 0;
    uint32_t            i = 0;
    uint32_t            *p_extcsd = 0;

    /* Check channel. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = MMC_GET_HNDL(channel);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        return MMC_ERR;  /* Not initialized */
    }

    if (0 == p_mmc_Config)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

#if MMC_FORMAT_ON
    /* ==== Check that the work buffer is allocated. ==== */
    if (0 == p_hndl->p_rw_buff)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR);
        return MMC_ERR;     /* Not allocated yet */
    }
#endif  /* MMC_FORMAT_ON */

    /* ==== Initialize parameter. ==== */
    r_mmcif_init_hndl(channel, p_mmc_Config->mode, p_mmc_Config->voltage);

    /* ---- Initilaize the EXT_CSD data bufffer. ---- */
    p_extcsd = (uint32_t *)&gs_extcsd[channel][0];
    i = (MMC_EXTCSD_BYTE/sizeof(uint32_t));
    do
    {
        *p_extcsd++ = 0;
        i--;
    }
    while (0 != i);    /* WAIT_LOOP */

    p_hndl->error = MMC_SUCCESS;

    /* ==== Check the card insertion. ==== */
    if (R_MMCIF_Get_CardDetection(channel) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_NO_CARD);
        return p_hndl->error;
    }

    /* ==== Pre-boot state operation ==== */
    if (MMC_MODE_eMMC == p_hndl->sup_eMMC)
    {
        if (r_mmcif_preboot(channel) != MMC_SUCCESS)
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
            r_mmcif_set_err(channel, MMC_ERR);
            return p_hndl->error;
        }
    }

    /* ---- Sets the single port. ---- */
    r_mmcif_set_port(channel, MMC_PORT_1BIT);

    /* ---- Supply the clock for card-identification mode. ---- */
    if (r_mmcif_set_clock(channel, MMC_CLK_400kHz, MMC_CLOCK_ENABLE) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        /* ---- Halt the clock. ---- */
        r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);
        return p_hndl->error;     /* not inserted */
    }

    /* ---- Wait for 74 clock or more. ----*/
    r_mmcif_dev_wait(channel, 3);

    r_mmcif_dev_disable_int(channel);

    /* ---- Clear CEINTEN register. ---- */
    r_mmcif_clear_int_mask(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);

    /* ---- Disable the DMA/DTC. ---- */
    ul_tmp = (uint32_t)(MMC_INREG(p_hndl, MMC_CEBUFACC) & ~MMC_CEBUFACC_MASK_DMARWEN);
    MMC_OUTREG(p_hndl, MMC_CEBUFACC, ul_tmp);
    if (MMC_INREG(p_hndl, MMC_CEBUFACC) == 0)
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }
    r_mmcif_dev_enable_int(channel);

    /* ==== Initialize card and distinguish card type. ==== */
    if (r_mmcif_card_init(channel) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        /* ---- Halt the clock. ---- */
        r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);
        return p_hndl->error;
    }

    /* ==== Check the card registers. ==== */
    /* ---- Check the CSD register. ---- */
    if (r_mmcif_check_csd(channel) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        /* ---- Halt the clock. ---- */
        r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);
        return p_hndl->error;
    }

    /* ---- Supply the clock (Data-transfer ratio). ---- */
    r_mmcif_set_clock(channel, p_hndl->csd_tran_speed, MMC_CLOCK_ENABLE);

    /* ==== Data-transfer mode ==== */
    if (r_mmcif_send_cmd_arg(channel, MMC_CMD7, MMC_RESP_R1, p_hndl->rca[0]) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        /* ---- Halt the clock. ---- */
        r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);
        return p_hndl->error;
    }

    /* ---- Check the locked card. ---- */
    if (p_hndl->resp_status & MMC_RES_CARD_IS_LOCKED)
    {
        p_hndl->mount = (MMC_CARD_LOCKED | MMC_MOUNT_LOCKED_CARD);
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_CARD_LOCK);
        /* ---- Halt the clock. ---- */
        r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);
        return MMC_SUCCESS_LOCKED_CARD;
    }
    
    /* ---- Set block length (Issue CMD16.). ---- */
    if (r_mmcif_send_cmd_arg(channel, MMC_CMD16, MMC_RESP_R1, MMC_TRANS_BLOCK_SIZE) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        /* ---- Halt the clock. ---- */
        r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);
        return p_hndl->error;
    }

    /* ---- Check spec version. ---- */
    if (p_hndl->csd_spec >= MMC_SPEC_40)    /* Ver4.0 or more */
    {
        /* Set card speed. */
        if (r_mmcif_set_speed(channel) != MMC_SUCCESS)
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
            r_mmcif_restore_ext_csd(channel);
            /* ---- Halt the clock. ---- */
            r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);
            return p_hndl->error;
        }

        /* ---- Supply the clock (Data-transfer ratio). ---- */
        r_mmcif_set_clock(channel, p_hndl->csd_tran_speed, MMC_CLOCK_ENABLE);

        /* Set card bus width (8bits, 4bits or 1bit). */
        if (r_mmcif_card_change_buswidth(channel,(int8_t)p_hndl->sup_bus) != MMC_SUCCESS)
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
            r_mmcif_restore_ext_csd(channel);
           /* ---- Halt the clock. ---- */
            r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);
            return p_hndl->error;
        }
        /* Set card density. */
        /* Check the access mode from OCR[30:29]. 
           In the case of sector mode, set p_hndl->density_type to 1. */
        if ((p_hndl->ocr[0] & MMC_OCR_ACCESS_MODE_MASK) == MMC_OCR_SECTOR_MODE_MASK)
        {
            p_hndl->density_type = 1;
        }
        else{
            p_hndl->density_type = 0;
        }
    }

    /* Get area size. */
    if (r_mmcif_get_size(channel) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        r_mmcif_restore_ext_csd(channel);
        /* ---- Halt the clock. ---- */
        r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);
        return p_hndl->error;
    }

    /* ---- Set mount flag. ---- */
    p_hndl->mount = MMC_MOUNT_UNLOCKED_CARD;

    /* ---- Halt the clock. ---- */
    r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);

    return p_hndl->error;
}


/**********************************************************************************************************************
* Outline      : Pre-boot operation for eMMC
* Function Name: r_mmcif_preboot
* Description  : Performs pre-boot operation.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : MMC_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_preboot(uint32_t channel)
{
    /* ==== To be created ==== */
    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Initialize Card
* Function Name: r_mmcif_card_init
* Description  : Initializes the MMC, and then sets from idle state to stand-by.
*              : Gets CID and CSD from the MMC.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : MMC_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_card_init(uint32_t channel)
{
    mmc_status_t    ret = MMC_SUCCESS;
    int32_t             loop = 0;

    /* ==== Set to idle state. (Issue CMD0.) ==== */
    /* WAIT_LOOP */
    for (loop = 0; loop < MMC_MAX_AGAIN_COUNT; loop++)
    {
        ret = r_mmcif_send_cmd_arg(channel, MMC_CMD0, MMC_RESP_NON, 0x00000000);
        if (MMC_SUCCESS == ret)
        {
            break;
        }
        
        r_mmcif_dev_wait(channel, 1);
    }

    if (MMC_SUCCESS != ret)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        return MMC_ERR;
    }

    /* ---- Get OCR (Issue CMD1.) ---- */
    ret = r_mmcif_send_ocr(channel);
    if (MMC_SUCCESS != ret)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        return MMC_ERR;
    }

    /* ---- Get CID (Issue CMD2.) ---- */
    if (r_mmcif_send_cmd_arg(channel, MMC_CMD2, MMC_RESP_R2_CID, 0) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        return MMC_ERR;
    }

    return r_mmcif_card_init_rca(channel);
}


/**********************************************************************************************************************
* Outline      : Initialize Rca
* Function Name: r_mmcif_card_init_rca
* Description  : Initializes the MMC, and then gets RCA/CSD.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : MMC_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
static mmc_status_t r_mmcif_card_init_rca(uint32_t channel)
{
    mmc_mmchndl_t  *p_hndl = 0;

    p_hndl = MMC_GET_HNDL(channel);

    /* ---- Get RCA. (Issue CMD3.) ---- */
    if (r_mmcif_send_cmd_arg(channel, MMC_CMD3, MMC_RESP_R1, p_hndl->rca[0]) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        return MMC_ERR;
    }

    /* ==== Stand-by state  ==== */

    /* ---- Get CSD, (Issue CMD9.) ---- */
    if (r_mmcif_send_cmd_arg(channel, MMC_CMD9, MMC_RESP_R2_CSD, p_hndl->rca[0]) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        return MMC_ERR;
    }

    p_hndl->dsr[0] = 0x00000000ul;
    /* DSR implemented? */
    if (p_hndl->csd[1] & 0x00001000ul) /* Implemented */
    {
        /* Set DSR. (Issue CMD4.) */
        p_hndl->dsr[0] = 0x00000404ul;
        if (r_mmcif_send_cmd_arg(channel, MMC_CMD4, MMC_RESP_NON, p_hndl->dsr[0]) != MMC_SUCCESS)
        {
           R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
            return MMC_ERR;
        }
    }

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Unmount
 *****************************************************************************************************************/ /**
 * @brief This function clears the MMC mounted state and transitions from the transfer state to the state from which 
 *        the driver can enter the idle state.
 * @param[in] channel
 *             Channel number : The number of the MMCIF channel used (numbering starts at 0)
 * @retval    MMC_SUCCESS Successful operation
 * @retval    MMC_ERR     General error
 * @details   This function performs MMC unmount processing. If this function is called in the transfer state, it 
 *            initializes the MMC extended CSD register.\n 
 *            For MMC card, it switches to the MMC card removable state. Note that even if this function has been 
 *            called and the MMC card mounted state has been cleared, the MMC card insertion interrupt and the MMC 
 *            card insertion verification interrupt callback function remain enabled.
 * @note      If the MMC card is removed after this function has been called, the pins must be set up. See section 
 *            4.5 in application note for details. Also, initialization using the R_MMCIF_Open() function is
 *            required before executing this function.\n 
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.
 */
mmc_status_t R_MMCIF_Unmount(uint32_t channel)
{
    mmc_mmchndl_t    *p_hndl = 0;
    uint32_t          i = 0;
    uint32_t         *p_extcsd = 0;
    
    /* Check channel. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = MMC_GET_HNDL(channel);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        return MMC_ERR;  /* not initialized */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    /* ---- Restore EXT_CSD register. ---- */
    if (p_hndl->csd_spec >= MMC_SPEC_40)    /* Ver4.0 or more */
    {
        if (MMC_MOUNT_UNLOCKED_CARD == p_hndl->mount)
        {
            /* ---- Supply the clock (Data-transfer ratio). ---- */
            r_mmcif_set_clock(channel, p_hndl->csd_tran_speed, MMC_CLOCK_ENABLE);

            if (r_mmcif_restore_ext_csd(channel) != MMC_SUCCESS)
            {
                R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
                r_mmcif_read_write_error(channel);
/*              return MMC_ERR; */ /* Get not to stop unmount processing. */
            }
        }
    }

    /* ---- Clear the mount flag. ---- */
    p_hndl->mount = MMC_UNMOUNT_CARD;

    /* ---- Halt the clock. ---- */
    r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);

    /* ---- Initilaize the MMC handle. ---- */
    r_mmcif_init_hndl(channel, 0, p_hndl->voltage);

    /* ---- Initilaize the EXT_CSD data bufffer. ---- */
    p_extcsd = (uint32_t *)&gs_extcsd[channel][0];
    i = (MMC_EXTCSD_BYTE/sizeof(uint32_t));
    do
    {
        *p_extcsd++ = 0;
        i--;
    }
    while (0 != i);    /* WAIT_LOOP */

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Execute SWITCH Function Operation
* Function Name: r_mmcif_card_switch_func
* Description  : Issues SWITCH command.
*              : Sets appropriate bus width.
*              : Sets bus timing.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : uint32_t           arg                 ;   Command argument [31:0]
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : MMC_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : SWITCH command is supported the MMC spec version 4.0 or more.
*              : Response type is R1b.
**********************************************************************************************************************/
mmc_status_t r_mmcif_card_switch_func(uint32_t channel, uint32_t arg)
{
    mmc_mmchndl_t   *p_hndl = 0;

    p_hndl = MMC_GET_HNDL(channel);

    if (r_mmcif_send_cmd_arg(channel, MMC_CMD6, MMC_RESP_R1b, arg) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        r_mmcif_read_write_error(channel);
        return MMC_ERR;
    }
    
    /* Check SWITCH ERROR. */
    if (p_hndl->resp_status & MMC_RES_SWITCH_ERROR)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        return MMC_ERR;
    }
    
    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Read Byte Data from Card
* Function Name: r_mmcif_read_byte
* Description  : read byte data from card
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : uint32_t           cmd                 ;   Command code
*              : uint32_t           arg                 ;   Command argument [31:0]
*              : uint8_t            *p_readbuff         ;   Read data buffer
*              : uint32_t           byte                ;   Number of read bytes
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : MMC_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_read_byte(uint32_t channel, uint32_t cmd, uint32_t arg, uint8_t *p_readbuff,
                                       uint32_t byte)
{
    mmc_mmchndl_t  *p_hndl = 0;

    p_hndl = MMC_GET_HNDL(channel);

    /* ---- Set transfer bytes. ---- */
    MMC_OUTREG(p_hndl, MMC_CEBLOCKSET, byte);
    if (MMC_INREG(p_hndl, MMC_CEBLOCKSET) == byte)
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }

    /* ---- Issue a command. ---- */
    if (r_mmcif_send_cmd_arg(channel, cmd, MMC_RESP_R1, arg) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        r_mmcif_read_write_error(channel);
        return MMC_ERR;
    }

    /* Enable the read complete bit, the read enable bit and all errors. */
    r_mmcif_set_int_mask(channel, MMC_CEINT1_TRNSR_ENAB, MMC_CEINT2_ALL_ERR);

    /* ---- Wait for the read enable interrupt. ---- */
    if (r_mmcif_dev_int_wait(channel, MMC_TIMEOUT_MULTIPLE) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_HOST_TOE);
        r_mmcif_read_write_error(channel);
        return MMC_ERR;
    }   

    /* ---- Check the errors. ---- */
    if (p_hndl->ceint2 & MMC_CEINT2_ALL_ERR)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        r_mmcif_check_info2_err(channel);     /* Set the error code. */
        r_mmcif_read_write_error(channel);
        return MMC_ERR;
    }

    /* ---- Clear the read enable bit. */
    r_mmcif_clear_info(channel, MMC_CEINT1_BUFR_ENAB, 0x0000);
    
    /* Transfer data. */
    if (r_mmcif_dev_read_data(p_readbuff, (uint32_t)(p_hndl->reg_base + MMC_CEDATA), (int32_t)byte) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_CPU_IF);
        r_mmcif_read_write_error(channel);
        return MMC_ERR;
    }

    /* Wait for the data transfer interrupt. */
    if (r_mmcif_dev_int_wait(channel, MMC_TIMEOUT_RESP) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_HOST_TOE);
        r_mmcif_read_write_error(channel);
        return MMC_ERR;
    }

    /* ---- Check the errors. ---- */
    if (p_hndl->ceint2 & MMC_CEINT2_ALL_ERR)
    {
        /* If CMD14, Check CRC error. */
        if ((MMC_CMD14 == cmd) && (MMC_CEINT2_READ_ERR == (p_hndl->ceint2 & MMC_CEINT2_ALL_ERR)))
        {
            p_hndl->ceint2 = 0;
        }
        else
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
            r_mmcif_check_info2_err(channel);     /* Set the error code. */
            r_mmcif_read_write_error(channel);
            return MMC_ERR;
        }
    }

    /* Disable all interrupts  */
    r_mmcif_clear_int_mask(channel, MMC_CEINT1_TRNSR_ENAB, MMC_CEINT2_ALL_ERR);

    /* Clear All end bits. */
    r_mmcif_clear_info(channel, MMC_CEINT1_READ_COMP, 0x0000);

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Write Byte Data to Card
* Function Name: r_mmcif_write_byte
* Description  : Writes byte data to card
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : uint32_t           cmd                 ;   Command code
*              : uint32_t           arg                 ;   Command argument [31:0]
*              : uint8_t            *p_writebuff        ;   Write data buffer
*              : uint32_t           byte                ;   Number of write bytes
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : MMC_ERR                                ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_write_byte(uint32_t channel, uint32_t cmd, uint32_t arg, uint8_t *p_writebuff,
                                        uint32_t byte)
{
    mmc_mmchndl_t      *p_hndl = 0;
    int32_t             time_out = 0;

    p_hndl = MMC_GET_HNDL(channel);

    /* ---- Set transfer bytes. ---- */
    MMC_OUTREG(p_hndl, MMC_CEBLOCKSET, byte);
    if (MMC_INREG(p_hndl, MMC_CEBLOCKSET) == byte)
    {
        R_BSP_NOP();  /* Wait for the write completion. */
    }

    /* ---- Issue a command. ---- */
    if (r_mmcif_send_cmd_arg(channel, cmd, MMC_RESP_R1, arg) != MMC_SUCCESS)
    {
        if (MMC_ERR_CARD_LOCK == p_hndl->error)
        {
            p_hndl->error = MMC_SUCCESS;
        }
        else
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
            r_mmcif_read_write_error(channel);
            return MMC_ERR;
        }
    }

    /* Enable the write complete, the write enable and all errors. */
    r_mmcif_set_int_mask(channel, MMC_CEINT1_TRNSW_ENAB, MMC_CEINT2_ALL_ERR);

    /* ---- Wait for the write enable interrupt. ---- */
    if (r_mmcif_dev_int_wait(channel, MMC_TIMEOUT_MULTIPLE) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_HOST_TOE);
        r_mmcif_read_write_error(channel);
        return MMC_ERR;
    }

    /* ---- Check the errors. ---- */
    if (p_hndl->ceint2 & MMC_CEINT2_ALL_ERR)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        r_mmcif_check_info2_err(channel);     /* Set the error code. */
        r_mmcif_read_write_error(channel);
        return MMC_ERR;
    }

    /* ---- Clear write enable bit. ---- */
    r_mmcif_clear_info(channel, MMC_CEINT1_BUFW_ENAB, 0x0000);

    /* Transfer data. */
    if (r_mmcif_dev_write_data(p_writebuff, (uint32_t)(p_hndl->reg_base + MMC_CEDATA), (int32_t)byte) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_CPU_IF);
        r_mmcif_read_write_error(channel);
        return MMC_ERR;
    }

    /* Wait for All end interrupt. */
    if ((MMC_CMD42 == cmd) && (1 == byte))
    {
        /* Force erase timeout  */
        time_out = MMC_TIMEOUT_ERASE_CMD;
    }
    else
    {
        time_out = MMC_TIMEOUT_RESP;
    }

    if (r_mmcif_dev_int_wait(channel, time_out) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_HOST_TOE);
        r_mmcif_read_write_error(channel);
        return MMC_ERR;
    }

    if (p_hndl->ceint2 & MMC_CEINT2_ALL_ERR)
    {
        r_mmcif_check_info2_err(channel);     /* Set the error code. */
        if (MMC_TIMEOUT_ERASE_CMD == time_out)
        {
            /* Forced erase. */
            if (MMC_ERR_RESPB_TOE == p_hndl->error)
            {
                /* Forced erase timeout. */
                r_mmcif_clear_info(channel, MMC_CEINT1_WRITE_COMP, 0x0000);
                if (r_mmcif_wait_rbusy(channel, MMC_TIMEOUT_FORCE_ERASE_RESP) != MMC_SUCCESS)
                {
                    R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
                    r_mmcif_read_write_error(channel);
                    return MMC_ERR;
                }
            }
            else
            {
                R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
                r_mmcif_read_write_error(channel);
                return MMC_ERR;
            }
        }
        else
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
            r_mmcif_read_write_error(channel);
            return MMC_ERR;
        }
    }

    /* Disable all interrupts. */
    r_mmcif_clear_int_mask(channel, MMC_CEINT1_TRNSW_ENAB, MMC_CEINT2_ALL_ERR);

    /* Clear All end bits. */
    r_mmcif_clear_info(channel, MMC_CEINT1_WRITE_COMP, 0x0000);  /* clear All end bit */

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Query and Change Bus Width
* Function Name: r_mmcif_card_change_buswidth
* Description  : Queries card support bus width (8bits, 4bits or 1bit).
*              : Maximum bus width is constrained to host bus width data pattern sent to card using CMD19.
*              : Reversed pattern received from card using CMD14.
*              : Thereafter, changes appropriate bus width.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : uint8_t            sup_bus             ;   Host support bus width (0:1bit 1:4bits 2:8bits)
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : MMC_ERR_CHANGE_BUS                     ;   Bus width change error
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
static mmc_status_t r_mmcif_card_change_buswidth(uint32_t channel, int8_t sup_bus)
{
    int8_t      width = 0;          /* Card bus width */
    int8_t      size[] = {1,4,8};   /* Pattern size [0]:1bit [1]:4bits [2]:8bits */
    uint8_t     *pat = 0;           /* Test pattern */
    uint8_t     test_ref[3][8] = {  /* Test data pattern reference */
        {0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, /* 1bit  test pattern */
        {0x5a,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, /* 4bits test pattern */
        {0x55,0xaa,0x00,0x00,0x00,0x00,0x00,0x00}  /* 8bits test pattern */
    };

    /* WAIT_LOOP */
    for (width = sup_bus; width >= 0; width--)
    {
        pat = &test_ref[width][0];
        r_mmcif_set_port(channel, width);

        /* ---- Send data pattern. (Issue CMD19.) ---- */
        if (r_mmcif_write_byte(channel, MMC_CMD19, 0, pat, (uint32_t)size[width]) != MMC_SUCCESS)
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
            r_mmcif_set_port(channel, MMC_PORT_1BIT);
            return MMC_ERR_CHANGE_BUS;
        }

        /* ---- Receive data pattern. (Issue CMD14.) ---- */
        if (r_mmcif_read_byte(channel, MMC_CMD14, 0, pat, (uint32_t)size[width]) != MMC_SUCCESS)
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
            r_mmcif_set_port(channel, MMC_PORT_1BIT);
            return MMC_ERR_CHANGE_BUS;
        }

        if (r_mmcif_check_pattern(channel, pat, width) == MMC_SUCCESS)
        {
            break;
        }
        if (0 == width)      /* Non support */
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
            r_mmcif_set_err(channel, MMC_ERR_CHANGE_BUS);
            r_mmcif_set_port(channel, MMC_PORT_1BIT);
            return MMC_ERR_CHANGE_BUS;
        }
    }

    /* ---- Set MMC bus width. (Issue CMD6.) ---- */
    /* Arg : 0x03b70000(1 bit data bus) or
             0x03b70100(4 bit data bus) or
             0x03b70200(8 bit data bus)
       bit31-26             : All 0b
       bit25-24 Access bits : 11b means write byte.
                              Refer to 7.6.1 Command sets and extended settings.
       bit23-16 Index field : BUS_WIDTH [183] -> B7h. Index of EXT_CSD register.
       bit15-8  Value       : BUS_WIDTH [183] <- 00h or 01h or 02h. Set value to EXT_CSD register.
                                                   00h : 1 bit data bus
                                                   01h : 4 bit data bus
                                                   02h : 8 bit data bus
                              Refer to 7.6.5 Bus width selection.
                              The card changes its bus width.
       bit7-3               : All 0b
       bit2-0   Cmd Set (Command Set) : The field is ignored in order to write Ext_CSD.
                                        Refer to 7.6.1 Command sets and extended settings. 
    */
    if (r_mmcif_card_switch_func(channel, (uint32_t)(MMC_SWITCH_BUS_WIDTH | (width<<8u))) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        r_mmcif_set_port(channel, MMC_PORT_1BIT);
        return MMC_ERR_CHANGE_BUS;
    }
    
    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Check test pattern
* Function Name: r_mmcif_check_pattern
* Description  : Checks test pattern.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : uint8_t            *pat                ;   Received test pattern
*              : uint8_t            width               ;   Check bus width
*              :    (0:1bit 1:4bits 2:8bits)
* Return Value : MMC_SUCCESS                            ;   Test pattern agreement
:              : MMC_ERR                                ;   Not test pattern agreement
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
static mmc_status_t r_mmcif_check_pattern(uint32_t channel, uint8_t *pat, uint8_t width)
{
    uint8_t     i = 0;
    uint8_t     size[] = {1,4,8};   /* Pattern size [0]:1bit [1]:4bits [2]:8bits */
    uint8_t     *check = 0;         /* Check pattern */
    uint8_t     check_ref[3][8] = { /* Check pattern reference */
        {0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, /* 1bit  check pattern */
        {0xa5,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, /* 4bits check pattern */
        {0xaa,0x55,0x00,0x00,0x00,0x00,0x00,0x00}  /* 8bits check pattern */
    };
    
    check = &check_ref[width][0];

    /* WAIT_LOOP */
    for (i = size[width]; i > 0; i--, pat++, check++)
    {
        if (*pat != *check)
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
            return MMC_ERR;
        }
    }

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Set speed
* Function Name: r_mmcif_set_speed
* Description  : Queries supported speed
*              : Set appropriate transfer speed.
*              : Set bus timing.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
static mmc_status_t r_mmcif_set_speed(uint32_t channel)
{
    mmc_mmchndl_t  *p_hndl = 0;
    mmc_status_t    er_cd = MMC_SUCCESS;

    p_hndl = MMC_GET_HNDL(channel);
    
    /* ---- Get Extended CSD. (Issue CMD8.) ---- */
    if (r_mmcif_read_byte(channel, MMC_CMD8, 0, &gs_extcsd[channel][0], MMC_EXTCSD_BYTE) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        er_cd = r_mmcif_read_write_error(channel);
        if (er_cd != MMC_SUCCESS)
        {
            return er_cd;
        }
        else
        {
            return MMC_ERR;
        }
    }
    
    /* ---- Check supported speed. (CARD_TYPE field) ---- */
    if (0x3 == (gs_extcsd[channel][196] & 0x3))          /* HS-MMC 52MHz */
    {
        p_hndl->speed_mode |= (uint8_t)(MMC_SUP_SPEED_52MHz | MMC_CUR_SPEED_52MHz);
        p_hndl->csd_tran_speed = MMC_CLK_52MHz;
    }
    else if (0x1 == (gs_extcsd[channel][196] & 0x3))     /* HS-MMC 26MHz */
    {
        p_hndl->speed_mode |= (uint8_t)(MMC_SUP_SPEED_26MHz | MMC_CUR_SPEED_26MHz);
        p_hndl->csd_tran_speed = MMC_CLK_26MHz;
    }
    else                                                /* not HS-MMC */
    {
        /* If CARD_TYPE is not 0x01 or 0x03, hndl->csd_tran_speed keep MMC_CLK_20Mz. */
    }
    
    /* ---- Get supported command sets. (MMC application) ---- */
    p_hndl->sup_app = (uint8_t)gs_extcsd[channel][504];
    
    /* ---- Get card capacity parameter. ---- */
    p_hndl->sec_count = (((uint32_t)gs_extcsd[channel][215] << 24u)
        | ((uint32_t)gs_extcsd[channel][214] << 16u)
            | ((uint32_t)gs_extcsd[channel][213] << 8u)
                | ((uint32_t)gs_extcsd[channel][212]));

    /* ---- Get reliable write sector count. ---- */
    p_hndl->rel_seccnt = gs_extcsd[channel][222];   /* Support MMC Ver.4.3 or more. */

    /* ---- Get eMMC parameter. ---- */
    if (p_hndl->sup_eMMC == MMC_MODE_eMMC)
    {
        /* Get boot partition sector size. */
        p_hndl->boot_size = (uint32_t)((uint32_t)(gs_extcsd[channel][226]) << 8u);

        /* Get boot acknowledge. */
        p_hndl->boot_ack = (uint8_t)((gs_extcsd[channel][179] & 0x40) >> 7u);

        /* Get boot partition enable. */
        p_hndl->boot_enab = (uint8_t)((gs_extcsd[channel][179] & 0x38) >> 3u);

        /* Get boot partition access. */
        p_hndl->boot_acc = (uint8_t)(gs_extcsd[channel][179] & 0x07);

        /* Get reset or retain boot bus width. */
        p_hndl->reset_boot_width = (uint8_t)((gs_extcsd[channel][177] & 0x38) >> 3u);

        /* Get current boot bus width. */
        p_hndl->boot_width = (uint8_t)(gs_extcsd[channel][177] & 0x07);
    }
    
    /* ---- Set bus timing. (Issue CMD6.) ---- */
    /* Arg : 0x03b90100
       bit31-26             : All 0b
       bit25-24 Access bits : 11b means write byte.
                              Refer to 7.6.1 Command sets and extended settings.
       bit23-16 Index field : HS_TIMING [185] -> B9h. Index of EXT_CSD register.
       bit15-8  Value       : HS_TIMING [185] <- 01h. Set value to EXT_CSD register.
                              Refer to 7.6.2 High-speed mode selection.
                              The card changes its timing to high speed timing.
       bit7-3               : All 0b
       bit2-0   Cmd Set (Command Set) : The field is ignored in order to write Ext_CSD.
                                        Refer to 7.6.1 Command sets and extended settings. 
    */
    if (MMC_CLK_20MHz < p_hndl->csd_tran_speed)
    {
        if (r_mmcif_card_switch_func(channel, MMC_SWITCH_HS_TIMING) != MMC_SUCCESS)
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
            return MMC_ERR;
        }
    }

    return MMC_SUCCESS;
}


/**********************************************************************************************************************
* Outline      : Restore The Value of EXT_CSD for MMC.
* Function Name: r_mmcif_restore_ext_csd
* Description  : Restore the value of EXT_CSD to the default.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
* Return Value : MMC_SUCCESS                            ;   Successful operation
:              : MMC_ERR                                ;   Not test pattern agreement
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
static mmc_status_t r_mmcif_restore_ext_csd(uint32_t channel)
{
    mmc_mmchndl_t          *p_hndl = 0;
    volatile mmc_status_t   error = MMC_SUCCESS;

    p_hndl = MMC_GET_HNDL(channel);

    error = p_hndl->error;

    /* ---- Set bus timing. (Issue CMD6.) ---- */
    /* ---- Default (backwards compatibility interface timing) ---- */
    if (r_mmcif_card_switch_func(channel, MMC_SWITCH_DS_TIMING) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        return MMC_ERR;
    }

    /* ---- Set MMC bus width. (Issue CMD6.) ---- */
    /* ---- Default (1 bit bus) ---- */
    if (r_mmcif_card_switch_func(channel, (uint32_t)MMC_SWITCH_BUS_WIDTH) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        return MMC_ERR;
    }
    
    p_hndl->error = error;
    
    return MMC_SUCCESS;
}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Get_ExtCsd
 *****************************************************************************************************************/ /**
 * @brief This function acquires the MMC extended CSD information.
 * @param[in] channel
 *             Channel number : The number of the MMCIF channel used (numbering starts at 0)
 * @param[out] *p_ext_csd_buffer
 *             Extended CSD receive buffer pointer (512 bytes)
 * @retval    MMC_SUCCESS Successful operation
 * @retval    MMC_ERR     General error
 * @details   This function stores the MMC extended CSD information in the argument p_ext_csd_buffer.
 * @note      Both initialization processing by the R_MMCIF_Open() function and mount processing by the R_MMCIF_Mount()
 *            function are required prior to executing this function.\n
 *            Note that the error code cannot be acquired with the R_MMCIF_Get_ErrCode() function.
 */
mmc_status_t R_MMCIF_Get_ExtCsd(uint32_t channel, uint8_t *p_ext_csd_buffer)
{
    mmc_mmchndl_t      *p_hndl = 0;

    /* Check channel. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = MMC_GET_HNDL(channel);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR);
        return MMC_ERR;  /* Not initialized */
    }

    if (0 == p_ext_csd_buffer)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */
    
    /* ---- Check MMC spec ver. ----*/
    if (p_hndl->csd_spec < MMC_SPEC_40)         /* Not supported EXT_CSD */
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
        return MMC_ERR;
    }

    if (MMC_MOUNT_UNLOCKED_CARD == p_hndl->mount)
    {
        /* ---- Supply the clock (Data-transfer ratio). ---- */
        r_mmcif_set_clock(channel, p_hndl->csd_tran_speed, MMC_CLOCK_ENABLE);

        /* ---- Get Extended CSD. (Issue CMD8.) ---- */
        if (r_mmcif_read_byte(channel, MMC_CMD8, 0, p_ext_csd_buffer, MMC_EXTCSD_BYTE) != MMC_SUCCESS)
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_MOUNT, __LINE__);
            return MMC_ERR;
        }

        r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);
    }
    
    return MMC_SUCCESS;
}


/* End of File */
