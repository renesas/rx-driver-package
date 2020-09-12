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
* File Name    : r_mmcif_write.c
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
static mmc_status_t r_mmcif_write_sect(uint32_t channel, mmc_access_t *p_mmc_Access);
static mmc_status_t r_mmcif_write_sect_sub(uint32_t channel, mmc_access_t *p_mmc_Access);


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Write_Memory
 *****************************************************************************************************************/ /**
 * @brief This function performs write processing.
 * @param[in] channel
 *             Channel number : The number of the MMCIF channel used (numbering starts at 0)
 * @param[in] *p_mmc_Access
 *             Access information structure (See section 3.8 in application note for structure details.)
 * @return    MMC_SUCCESS:          Successful operation.\n
 *            Other than the above: Error termination(See section 2.10 in application note for details).
 * @details   Writes the data from p_buff in the argument p_mmc_Access to an area with the number of blocks set by 
 *            cnt in the argument p_mmc_Access. That area starts at the block specified by lbn in the argument 
 *            p_mmc_Access.\n 
 *            If MMC card removal is detected at the start of this function's execution, processing is interrupted and 
 *            processing is terminated and an error is returned.\n 
 *            If a forced stop request due to an R_MMCIF_Control() function MMC_SET_STOP (forced stop request) command 
 *            is detected at the start of this function's execution, the forced stop is cleared and processing is 
 *            terminated and an error is returned.\n 
 *            The following commands are used to write out the block data.\n 
 *            \li First block: WRITE_SINGLE_BLOCK command (CMD24)
 *            \li Second and later blocks: WRITE_MULTIPLE_BLOCK command (CMD25)
 *            
 * @note      Both initialization processing by the R_MMCIF_Open() function and mount processing by 
 *            the R_MMCIF_Mount() function are required prior to executing this function.\n 
 *            We recommend repeating the write operation when this function terminates with a write error.\n 
 *            If the number of blocks to be transferred exceeds 65,535, break up the read into multiple function 
 *            calls. This issue requires care when this functionality is called from upper layer application programs 
 *            such as the FAT file system.\n 
 *            Note that the size of a block is 512 bytes.
 */
mmc_status_t R_MMCIF_Write_Memory(uint32_t channel, mmc_access_t *p_mmc_Access)
{
    mmc_mmchndl_t       *p_hndl = 0;

    /* Check channel. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = MMC_GET_HNDL(channel);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        return MMC_ERR;     /* Not initialized */
    }

    if (0 == p_mmc_Access)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR);
        return MMC_ERR;
    }

    if (0 == (uint32_t)p_mmc_Access->p_buff)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR);
        return MMC_ERR;
    }

    /* ---- Check the buffer boundary (4-byte unit). ---- */
    if (0 != ((uint32_t)p_mmc_Access->p_buff & 0x03u))
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_ADDRESS_BOUNDARY);
        return MMC_ERR_ADDRESS_BOUNDARY;
    }

    /* ---- Check access area. ---- */
    if ((p_mmc_Access->lbn >= p_hndl->card_sector_size) ||
        ((p_mmc_Access->lbn + p_mmc_Access->cnt) > p_hndl->card_sector_size))
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR);
        return MMC_ERR;       /* Out of area */
    }

    if ((p_mmc_Access->cnt > MMCMEM_TRANS_SECTORS) || (0 >= p_mmc_Access->cnt))
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR);
        return MMC_ERR;       /* Out of area */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

#if defined(MMC_CFG_FREE_LIB)
    /* ---- Check access area. ---- */
    if ((p_mmc_Access->lbn >= MMC_SIZE_CARD_1024MB) ||
        ((p_mmc_Access->lbn + p_mmc_Access->cnt) > MMC_SIZE_CARD_1024MB))
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR);
        return MMC_ERR; /* Out of area */
    }
#endif /* MMC_CFG_FREE_LIB */

    /* ---- Check the card is mounted. ---- */
    if (MMC_MOUNT_UNLOCKED_CARD != p_hndl->mount)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR);
        return MMC_ERR;       /* Not mounted yet */
    }

    /* ---- Check MMC spec ver. ----*/
    if (p_hndl->csd_spec < MMC_SPEC_30)         /* Not supported pre-defined mode */
    {
        if (MMC_OPEN_END != p_mmc_Access->rw_mode)
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_READ, __LINE__);
            r_mmcif_set_err(channel, MMC_ERR);
            return MMC_ERR;       /* Out of area */
        }
    }

    p_hndl->error = MMC_SUCCESS;

    /* ---- Not check the write protect. ---- */
    
    /* ---- Check the stop flag. ---- */
    if (p_hndl->stop)
    {
        p_hndl->stop = 0;
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_STOP);
        return p_hndl->error;
    }

    /* ---- Check the card insertion. ---- */
    if (R_MMCIF_Get_CardDetection(channel) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_NO_CARD);
        return p_hndl->error;
    }

    /* ---- Set mode to CPU/DMA/DTC transfer. ---- */
    if (p_hndl->trans_mode & MMC_MODE_DMA)
    {
        p_mmc_Access->mode = MMC_MODE_DMA;
    }
    else if (p_hndl->trans_mode & MMC_MODE_DTC)
    {
        p_mmc_Access->mode = MMC_MODE_DTC;
    }
    else
    {
        p_mmc_Access->mode = MMC_MODE_SW;
    }

    /* ---- Supply the clock for data-transfer mode. ---- */
    if (MMC_SUCCESS == r_mmcif_set_clock(channel, p_hndl->csd_tran_speed, MMC_CLOCK_ENABLE))
    {
        r_mmcif_clear_int_mask(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);
        r_mmcif_clear_info_reg(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);
        r_mmcif_clear_info(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);

        /* ==== Write data to the MMC. ==== */
        r_mmcif_write_sect(channel, p_mmc_Access);

        r_mmcif_clear_int_mask(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);
        r_mmcif_clear_info_reg(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);
        r_mmcif_clear_info(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);
    }
    else
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
    }

    /* ---- Halt the clock. ---- */
    r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);

    return p_hndl->error;
}


/**********************************************************************************************************************
 * Function Name: R_MMCIF_Write_Memory_Software_Trans
 *****************************************************************************************************************/ /**
 * @brief This function performs write processing (software transfers).
 * @param[in] channel
 *             Channel number : The number of the MMCIF channel used (numbering starts at 0)
 * @param[in] *p_mmc_Access
 *             Access information structure (See section 3.9 in application note for structure details.)
 * @return    MMC_SUCCESS:          Successful operation.\n
 *            Other than the above: Error termination(See section 2.10 in application note for details).
 * @details   Writes the data from p_buff in the argument p_mmc_Access to an area with the number of blocks set by 
 *            cnt in the argument p_mmc_Access. That area starts at the block specified by lbn in the argument
 *             p_mmc_Access.\n
 *            Software transfer is used, regardless of the operating mode data transfer setting at command processing
 *            time.\n 
 *            If MMC card removal is detected at the start of this function's execution, processing is interrupted and 
 *            processing is terminated and an error is returned.\n 
 *            If a forced stop request due to an R_MMCIF_Control() function MMC_SET_STOP (forced stop request) command 
 *            is detected at the start of this function's execution, the forced stop is cleared and processing is 
 *            terminated and an error is returned.\n 
 *            The following commands are used to write out the block data.\n 
 *            \li First block: WRITE_SINGLE_BLOCK command (CMD24)
 *            \li Second and later blocks: WRITE_MULTIPLE_BLOCK command (CMD25)
 *
 * @note      Both initialization processing by the R_MMCIF_Open() function and mount processing by the R_MMCIF_Mount()
 *            function are required prior to executing this function.\n 
 *            We recommend repeating the write operation when this function terminates with a write error.\n 
 *            If the number of blocks to be transferred exceeds 65,535, break up the read into multiple function calls.
 *            This issue requires care when this functionality is called from upper layer application programs such as 
 *            the FAT file system.\n 
 *            Note that the size of a block is 512 bytes.
 */
mmc_status_t R_MMCIF_Write_Memory_Software_Trans(uint32_t channel, mmc_access_t *p_mmc_Access)
{
    mmc_mmchndl_t       *p_hndl = 0;

    /* Check channel. */
#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if ((uint32_t)(sizeof(MMC_CFG_IP_BASE) / sizeof(uint32_t)) <= channel)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        return MMC_ERR;
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = MMC_GET_HNDL(channel);

#if (BSP_CFG_PARAM_CHECKING_ENABLE)
    if (0 == p_hndl)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        return MMC_ERR;     /* Not initialized */
    }

    if (0 == p_mmc_Access)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR);
        return MMC_ERR;
    }

    if (0 == (uint32_t)p_mmc_Access->p_buff)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR);
        return MMC_ERR;
    }

    /* ---- Check access area. ---- */
    if ((p_mmc_Access->lbn >= p_hndl->card_sector_size) ||
        ((p_mmc_Access->lbn + p_mmc_Access->cnt) > p_hndl->card_sector_size))
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR);
        return MMC_ERR;       /* Out of area */
    }

    if ((p_mmc_Access->cnt > MMCMEM_TRANS_SECTORS) || (0 >= p_mmc_Access->cnt))
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR);
        return MMC_ERR;       /* Out of area */
    }
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE */

#if defined(MMC_CFG_FREE_LIB)
    /* ---- Check access area. ---- */
    if ((p_mmc_Access->lbn >= MMC_SIZE_CARD_1024MB) ||
        ((p_mmc_Access->lbn + p_mmc_Access->cnt) > MMC_SIZE_CARD_1024MB))
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR);
        return MMC_ERR; /* Out of area */
    }
#endif /* MMC_CFG_FREE_LIB */

    /* ---- Check the card is mounted. ---- */
    if (MMC_MOUNT_UNLOCKED_CARD != p_hndl->mount)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR);
        return MMC_ERR;       /* Not mounted yet */
    }

    /* ---- Check MMC spec ver. ----*/
    if (p_hndl->csd_spec < MMC_SPEC_30)         /* Not supported pre-defined mode */
    {
        if (MMC_OPEN_END != p_mmc_Access->rw_mode)
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
            r_mmcif_set_err(channel, MMC_ERR);
            return MMC_ERR;       /* Out of area */
        }
    }

    p_hndl->error = MMC_SUCCESS;

    /* ---- Not check the write protect. ---- */

    /* ---- Check the stop flag. ---- */
    if (p_hndl->stop)
    {
        p_hndl->stop = 0;
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_STOP);
        return p_hndl->error;
    }

    /* ---- Check the card insertion. ---- */
    if (R_MMCIF_Get_CardDetection(channel) != MMC_SUCCESS)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_NO_CARD);
        return p_hndl->error;
    }

    p_mmc_Access->mode = MMC_MODE_SW;

    /* ---- Supply the clock for data-transfer mode. ---- */
    if (MMC_SUCCESS == r_mmcif_set_clock(channel, p_hndl->csd_tran_speed, MMC_CLOCK_ENABLE))
    {
        r_mmcif_clear_int_mask(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);
        r_mmcif_clear_info_reg(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);
        r_mmcif_clear_info(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);

        /* ==== Write data to the MMC. ==== */
        r_mmcif_write_sect(channel, p_mmc_Access);

        r_mmcif_clear_int_mask(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);
        r_mmcif_clear_info_reg(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);
        r_mmcif_clear_info(channel, MMC_CEINT1_ALL_INT, MMC_CEINT2_ALL_ERR);
    }
    else
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
    }

    /* ---- Halt the clock. ---- */
    r_mmcif_set_clock(channel, 0, MMC_CLOCK_DISABLE);

    return p_hndl->error;
}


/**********************************************************************************************************************
* Outline      : Write Sector Data to the MMC
* Function Name: r_mmcif_write_sect
* Description  : Writes data to the MMC.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : mmc_access_t      *p_mmc_Access        ;   MMC driver transfer configuration
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
mmc_status_t r_mmcif_write_sect(uint32_t channel, mmc_access_t *p_mmc_Access)
{
    mmc_mmchndl_t      *p_hndl = 0;
    uint32_t            block_set_back = 0;

    p_hndl = MMC_GET_HNDL(channel);

    /* ==== Check the status precede write operation. ==== */
    if (MMC_SUCCESS == r_mmcif_send_cmd_arg(channel, MMC_CMD13, MMC_RESP_R1, p_hndl->rca[0]))
    {
        if (MMC_STATE_TRAN != (p_hndl->resp_status & MMC_RES_STATE))    /* Not in transfer state */
        {
            p_hndl->error = MMC_ERR;
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
            r_mmcif_read_write_error(channel);
            return p_hndl->error;
        }
    }
    else    /* MMC error */
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_read_write_error(channel);
        return p_hndl->error;
    }

    /* In case of 1 blocks, execute the single write operation. */
    if (1 == p_mmc_Access->cnt)
    {
        /* Transfer size is fixed. (MMC_TRANS_BLOCK_SIZE bytes) */
        MMC_OUTREG(p_hndl, MMC_CEBLOCKSET, MMC_CEBLOCKSET_BLOCK_SIZE);
        if (MMC_INREG(p_hndl, MMC_CEBLOCKSET) == MMC_CEBLOCKSET_BLOCK_SIZE)
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }

        /* ==== Single block write ==== */
        r_mmcif_write_sect_sub(channel, p_mmc_Access);
    }
    else
    {
        /* Set the transfer block number. */
        block_set_back = (p_mmc_Access->cnt << 16u | MMC_CEBLOCKSET_BLOCK_SIZE);
        MMC_OUTREG(p_hndl, MMC_CEBLOCKSET, block_set_back);
        if (MMC_INREG(p_hndl, MMC_CEBLOCKSET) == block_set_back)
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }

        /* ==== Multi block write ==== */
        r_mmcif_write_sect_sub(channel, p_mmc_Access);
    }
    return p_hndl->error;
}


/**********************************************************************************************************************
* Outline      : Write a Sector Data to the MMC
* Function Name: r_mmcif_write_sect_sub
* Description  : Writes a sector data to the MMC.
* Arguments    : uint32_t           channel             ;   MMC Channel No.
*              : mmc_access_t      *p_mmc_Access        ;   MMC driver transfer configuration
* Return Value : MMC_SUCCESS                            ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
static mmc_status_t r_mmcif_write_sect_sub(uint32_t channel, mmc_access_t *p_mmc_Access)
{
    mmc_mmchndl_t      *p_hndl = 0;
    mmc_status_t        ret = MMC_SUCCESS;
    mmc_status_t        trans_ret = MMC_SUCCESS;
    uint32_t            info1_back = 0;
    uint32_t            cmd25_back = 0;
    uint32_t            ul_tmp = 0;

    p_hndl = MMC_GET_HNDL(channel);

    if ((MMC_MODE_DMA | MMC_MODE_DTC) & p_mmc_Access->mode)
    {
        R_MMCIF_Set_DmacDtc_Trans_Flg(channel, MMC_SET_TRANS_START);

        /* Enable DMA/DTC by "Buffer Write DMA Transfer Request Enable" bit of MMC_CEBUFACC. */
        ul_tmp = (MMC_INREG(p_hndl, MMC_CEBUFACC) | MMC_CEBUFACC_DMA_WRITE);
        MMC_OUTREG(p_hndl, MMC_CEBUFACC, ul_tmp);
        if (MMC_CEBUFACC_DMA_WRITE == MMC_INREG(p_hndl, MMC_CEBUFACC))
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }
    }

    /* ---- Issue CMD24 or CMD25. ---- */
    if (0x01 == p_mmc_Access->cnt)
    {
        /* Issue CMD24 (WRITE_SIGLE_BLOCK). */
        if (r_mmcif_send_cmd_arg(channel, MMC_CMD24, MMC_RESP_R1, MMC_SET_ACC_ADDR) != MMC_SUCCESS)
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
            r_mmcif_read_write_error(channel);
            return p_hndl->error;
        }
    }
    else
    {
        if (MMC_PRE_DEF == p_mmc_Access->rw_mode)   /* Pre-defined mode */
        {
            if (r_mmcif_send_cmd_arg(channel, MMC_CMD23, MMC_RESP_R1,
                (uint32_t)(0x00000000 | p_mmc_Access->cnt)) != MMC_SUCCESS)
            {
                R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
                r_mmcif_read_write_error(channel);
                return p_hndl->error;
            }
            cmd25_back = MMC_CMD25;
        }
        else        /* Open-ended mode */
        {
            cmd25_back = (MMC_CMD25 | MMC_CECMDSET_CMD12EN);        /* Set automatic CMD12. */
            MMC_OUTREG(p_hndl, MMC_CEARGCMD12, 0);
            if (MMC_CEARGCMD12_INIT == MMC_INREG(p_hndl, MMC_CEARGCMD12))
            {
                R_BSP_NOP();  /* Wait for the write completion. */
            }
        }

        /* Issue CMD25 (WRITE_MULTIPLE_BLOCK) */
        if (r_mmcif_send_cmd_arg(channel, cmd25_back, MMC_RESP_R1, MMC_SET_ACC_ADDR) != MMC_SUCCESS)
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
            r_mmcif_read_write_error(channel);
            return p_hndl->error;
        }
    }

    if (MMC_MODE_SW == p_mmc_Access->mode)
    {
        if ((MMC_PRE_DEF == p_mmc_Access->rw_mode) || (0x01 == p_mmc_Access->cnt))  /* Pre-defined mode */
        {
            /* Set the write complete, the write enable and all errors. */
            info1_back = MMC_CEINT1_TRNSW_ENAB;
        }
        else        /* Open-ended */
        {
            /* Set the resp busy complete for CMD12, teh write enable and all errors. */
            info1_back = (MMC_CEINT1_BUFW_ENAB | MMC_CEINT1_RESPB_COMP12);
        }

        /* Enable data transfer interrupt. */
        r_mmcif_set_int_mask(channel, info1_back, MMC_CEINT2_ALL_ERR);

        /* PIO */
        trans_ret = r_mmcif_software_trans(channel, p_mmc_Access->p_buff, p_mmc_Access->cnt,
                                              MMC_TRANS_WRITE);
    }
    else
    {
        if ((MMC_PRE_DEF == p_mmc_Access->rw_mode) || (0x01 == p_mmc_Access->cnt))  /* Pre-defined mode */
        {
            /* Enable the write complete and all errors. */
            info1_back = MMC_CEINT1_WRITE_COMP;
        }
        else        /* Open-ended */
        {
            /* Enable the resp busy complete for CMD12 and all errors. */
            info1_back = MMC_CEINT1_RESPB_COMP12;
        }

        /* Enable error interrupt. */
        r_mmcif_set_int_mask(channel, 0, MMC_CEINT2_ALL_ERR);

        /* DMA or DTC */
        trans_ret = r_mmcif_dev_dmacdtc_trans(channel, p_mmc_Access->cnt);

        /* Disable DMA/DTC by "Buffer Read DMA Transfer Request Enable" bit of MMC_CEBUFACC. */
        ul_tmp = (uint32_t)(MMC_INREG(p_hndl, MMC_CEBUFACC) & ~MMC_CEBUFACC_DMA_WRITE);
        MMC_OUTREG(p_hndl, MMC_CEBUFACC, ul_tmp);
        if (MMC_CEBUFACC_DMA_WRITE == MMC_INREG(p_hndl, MMC_CEBUFACC))
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }

        /* Enable data transfer interrupt. */
        r_mmcif_set_int_mask(channel, info1_back, 0);
    }

    /* ---- Wait All end interrupt. ---- */
    ret = r_mmcif_dev_int_wait(channel, MMC_TIMEOUT_RESP);

    /* ---- Check the result of transfer. ---- */
    if (MMC_SUCCESS != trans_ret)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_read_write_error(channel);
        return p_hndl->error;
    }

    /* ---- Check the result of wait All end interrupt. ---- */
    if (MMC_SUCCESS != ret)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_set_err(channel, MMC_ERR_HOST_TOE);
        r_mmcif_read_write_error(channel);
        return p_hndl->error;
    }

    /* ---- Check errors. ---- */
    if (p_hndl->ceint2 & MMC_CEINT2_ALL_ERR)
    {
        R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
        r_mmcif_check_info2_err(channel);
        r_mmcif_read_write_error(channel);
        return p_hndl->error;
    }

    /* Clear the write complete(or the resp busy complete for CMD12), 
       the write enable and all errors. */
    r_mmcif_clear_int_mask(channel, info1_back, MMC_CEINT2_ALL_ERR);

    if ((MMC_PRE_DEF == p_mmc_Access->rw_mode) || (0x01 == p_mmc_Access->cnt))
    {
        /* Clear the write complete bit. */
        r_mmcif_clear_info(channel, MMC_CEINT1_WRITE_COMP, 0x0000);
    }
    else        /* Open-ended */
    {
        if (r_mmcif_get_resp(channel, MMC_RESP_AUTO12) != MMC_SUCCESS)
        {
            /* Get and check CMD12 response. */
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
            r_mmcif_read_write_error(channel);
            return p_hndl->error;
        }

        /* Clear the resp busy complete for CMD12 bit. */
        r_mmcif_clear_info(channel, MMC_CEINT1_RESPB_COMP12, 0x0000);

        MMC_OUTREG(p_hndl, MMC_CEINT, (uint32_t)(~(MMC_CEINT1_RESP_COMP12 | MMC_CEINT1_WRITE_COMP)));
        if (0 == MMC_INREG(p_hndl, MMC_CEINT))
        {
            R_BSP_NOP();  /* Wait for the write completion. */
        }
    }

    /* ==== Check the status after write operation. ==== */
    if (r_mmcif_send_cmd_arg(channel, MMC_CMD13, MMC_RESP_R1, p_hndl->rca[0]) != MMC_SUCCESS)
    {
        /* Check OUT_OF_RANGE error. */
        /* Ignore errors during last block access. */
        if (p_hndl->resp_status & 0xffffe080ul)
        {
            if ((p_mmc_Access->lbn + p_mmc_Access->cnt) != p_hndl->card_sector_size)
            {
                /* Not last block */
                R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
                r_mmcif_read_write_error(channel);
                return p_hndl->error;
            }
            if (p_hndl->resp_status & 0x7fffe080ul)
            {
                /* Not OUT_OF_RANGE error */
                R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
                r_mmcif_read_write_error(channel);
                return p_hndl->error;
            }
            /* Clear OUT_OF_RANGE error. */
            p_hndl->resp_status = (p_hndl->resp_status & 0x1f00u);
            p_hndl->error = MMC_SUCCESS;
        }
        else    /* MMC error, ex)timeout error so on */
        {
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
            r_mmcif_read_write_error(channel);
            return p_hndl->error;
        }
    }

    if (MMC_STATE_TRAN != (p_hndl->resp_status & MMC_RES_STATE))
    {
        /* NOTE: In the case of the eMMC or the MMC card which response busy does not complete,
                 the host issues CMD13 until it becomes the tran state. */
        if (r_mmcif_wait_rbusy(channel, MMC_TIMEOUT_MULTIPLE) != MMC_SUCCESS)
        {
            p_hndl->error = MMC_ERR;
            R_MMCIF_Log_Func(MMC_DEBUG_ERR_ID, (uint32_t)MMC_WRITE, __LINE__);
            r_mmcif_read_write_error(channel);
        }
    }

    return p_hndl->error;
}


/* End of File */
