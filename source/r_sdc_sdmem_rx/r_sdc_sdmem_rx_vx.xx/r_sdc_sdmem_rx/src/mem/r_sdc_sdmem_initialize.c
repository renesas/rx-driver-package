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
* System Name  : SDHI Driver
* File Name    : r_sdc_sdmem_initialize.c
* Version      : 2.02
* Device       : RX
* Abstract     : API & Sub module
* Tool-Chain   : For RX e2_studio
* OS           : not use
* H/W Platform : RSK board for RX
* Description  : Interface file for SDC SD API for RX
* Limitation   : None
**********************************************************************************************************************/
/**********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 05.09.2014 1.00    First Release
*              : 17.07.2015 1.10    Standardized by the combo.
*              : 31.07.2017 2.00    SDHI FIT module separated into hardware low level layer and middleware layer.
*              :                    Changed prefix from SDHI to SDC_SD.
*              : 29.06.2018 2.02    Corresponded to SD Specifications Part1 Physical Layer Simplified Specification.
*              : 10.02.2020 3.00    Supported GCC and IAR compiler.
*              :                    Added comment "WAIT_LOOP".
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "r_sdc_sd_rx_if.h"
#include ".\src\r_sdc_sd_rx_private.h"

#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM)

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/
static uint32_t         g_sdc_sdmem_stat_buff[128/sizeof(uint32_t)];
static sdc_sd_status_t  r_sdc_sdmem_init_rca(uint32_t card_no);
static sdc_sd_status_t  r_sdc_sdmem_calc_erase_sector(uint32_t card_no);

/**********************************************************************************************************************
* Outline      : Initialize SD Card
* Function Name: r_sdc_sdmem_initialize
* Description  : Initializes the SD memory card.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           init_type           ;   SDHI Initialize type
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_initialize(uint32_t card_no, uint32_t init_type)
{
    sdc_sd_hndl_t   * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (SDC_SD_MODE_MEM == init_type)
    {
        /* ==== Initialize card and distinguish card type. ==== */
        if (r_sdc_sdmem_init(card_no) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
            return p_hndl->error;
        }

        /* ==== Check the card registers. ==== */
        if (r_sdc_sdmem_mount_chk_reg(card_no) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
            return p_hndl->error;
        }

        if (r_sdc_sdmem_mount(card_no) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
            return p_hndl->error;
        }
        /* Support the locked card. */
        if (SDC_SD_ERR_CARD_LOCK == p_hndl->error)
        {
            p_hndl->mount = (SDC_SD_CARD_LOCKED | SDC_SD_MOUNT_LOCKED_CARD);
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
            return SDC_SD_SUCCESS_LOCKED_CARD;
        }

        /* If it is SD memory card, get the SCR register. */
        if (r_sdc_sdmem_mount_get_scr(card_no) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
            return p_hndl->error;
        }
    }
    else
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }


    return p_hndl->error;
} /* End of function r_sdc_sdmem_initialize() */

/**********************************************************************************************************************
* Outline      : Mount SD Card
* Function Name: r_sdc_sdmem_mount_chk_reg
* Description  : 
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_mount_chk_reg(uint32_t card_no)
{
    sdc_sd_hndl_t   * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Check the CSD register. ---- */
    if (r_sdc_sd_check_csd(card_no) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        return p_hndl->error;
    }
    /* ---- Do not check the other registers (to be create). ---- */
    /* Get user area size. */
    if (r_sdc_sd_get_size(card_no, SDC_SD_USER_AREA) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        return p_hndl->error;
    }
    /* Check the write protect state. */
    p_hndl->write_protect = (p_hndl->write_protect | (uint8_t)r_sdc_sd_iswp(card_no));

    return p_hndl->error;
} /* End of function r_sdc_sdmem_mount_chk_reg() */

/**********************************************************************************************************************
* Outline      : Mount SD Card
* Function Name: r_sdc_sdmem_mount_get_scr
* Description  : 
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_mount_get_scr(uint32_t card_no)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    uint8_t           spec = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (r_sdc_sdmem_card_get_scr(card_no) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        return p_hndl->error;
    }
    spec = (uint8_t)((p_hndl->scr[0] & SDC_SD_SCR_SD_SPEC_MASK) >> SDC_SD_BIT_SHIFT_24);
    if (spec)
    {
        p_hndl->sd_spec = spec;
        r_sdc_sdmem_calc_erase_sector(card_no);
    }
    else
    {
        p_hndl->sd_spec = SDC_SD_SPEC_10;
    }

    return p_hndl->error;
} /* End of function r_sdc_sdmem_mount_get_scr() */

/**********************************************************************************************************************
* Outline      : Mount SD Card
* Function Name: r_sdc_sdmem_init
* Description  : 
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_init(uint32_t card_no)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    int32_t         i = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* Set the default value in order to initialize SD memory. */
    p_hndl->rca[0] = 0;

    /* ==== Set to idle state. (Issue CMD0.) ==== */
    /* WAIT_LOOP */
    for (i = 0 ; i < SDC_SD_MAX_AGAIN_COUNT; i++)
    {
        ret = r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD0, SDC_SD_RESP_NON, 0);
        if ((SDC_SD_SUCCESS == ret) && (i > 0))
        {
            break;
        }
        r_sdc_sd_wait(card_no, 2);  /* Wait for 1ms or more. */
    }

    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        return SDC_SD_ERR;  /* Error for CMD0 */
    }

    ret = r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD8, SDC_SD_RESP_R7, p_hndl->if_cond[0]);

    if (SDC_SD_SUCCESS == ret)
    {
        /* Check the R7 response. */
        if (SDC_SD_IF_COND_2_7_TO_3_6 != (p_hndl->if_cond[0] & SDC_SD_IF_COND_VOLT_SUP_MASK))
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
            p_hndl->error = SDC_SD_ERR_IFCOND_VOLT;
            return SDC_SD_ERR;
        }
        if (SDC_SD_R7_CHK_PATTERN != (p_hndl->if_cond[0] & SDC_SD_R7_CHK_PATTERN_MASK))
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
            p_hndl->error = SDC_SD_ERR_IFCOND_ECHO;
            return SDC_SD_ERR;
        }
        /* Set the SD_spec due to SDHC or SDXC. */
        p_hndl->sd_spec = SDC_SD_SPEC_20;
    }
    else
    {
        /* ==== Clear the illegal command error for CMD8. ==== */
        /* WAIT_LOOP */
        for (i = 0 ; i < SDC_SD_MAX_AGAIN_COUNT; i++)
        {
            ret = r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD0, SDC_SD_RESP_NON, 0);
            if (SDC_SD_SUCCESS == ret)
            {
                break;
            }
        }
        /* Set the SD_spec due to SDSC. */
        p_hndl->sd_spec = SDC_SD_SPEC_10;
    }

    /* Get OCR. (Issue ACMD41.) */
    ret = r_sdc_sd_send_ocr(card_no, (int32_t)SDC_SD_MEDIA_SDMEM);

    if (SDC_SD_SUCCESS != ret)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_CARD_TYPE);
        return SDC_SD_ERR;
    }

    p_hndl->media_type = (p_hndl->media_type | SDC_SD_MEDIA_SDMEM);

    /* ---- Get CID. (Issue CMD2.) ---- */
    if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD2, SDC_SD_RESP_R2_CID, 0) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        return SDC_SD_ERR;
    }

    return r_sdc_sdmem_init_rca(card_no);

} /* End of function r_sdc_sdmem_init() */

/**********************************************************************************************************************
* Outline      : Initialize Rca
* Function Name: r_sdc_sdmem_init_rca
* Description  : Initializes the SD card, and then gets RCA/CSD and sets SCR.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
static sdc_sd_status_t r_sdc_sdmem_init_rca(uint32_t card_no)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    int32_t           i = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Get RCA. (Issue CMD3.) ---- */
    if (p_hndl->media_type & SDC_SD_MEDIA_COMBO)  /* SDIO or SD Memory */
    {
        /* WAIT_LOOP */
        for (i = 0; i < SDC_SD_MAX_AGAIN_COUNT; i++)
        {
            if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD3, SDC_SD_RESP_R6, 0) != SDC_SD_SUCCESS)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
                return SDC_SD_ERR;
            }
            if (0x00 != p_hndl->rca[0])
            {
                break;
            }
        }
        /* Illegal RCA */
        if (SDC_SD_MAX_AGAIN_COUNT == i)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR_CARD_CC);
            return SDC_SD_ERR;
        }
    }
    else
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return p_hndl->error;
    }

    p_hndl->error = SDC_SD_SUCCESS;

    /* ==== Stand-by state  ==== */
    if (SDC_SD_MEDIA_SDIO == p_hndl->media_type)
    {
        return SDC_SD_SUCCESS;
    }

    /* ---- Get CSD. (Issue CMD9.) ---- */
    if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD9, SDC_SD_RESP_R2_CSD, p_hndl->rca[0]) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        return SDC_SD_ERR;
    }

    r_sdc_sd_wait(card_no, 3);  /* Wait for 100 clock or more. */

    p_hndl->dsr[0] = 0x0000;

    if (p_hndl->media_type & SDC_SD_MEDIA_MEM)
    {
        /* DSR implemented? */
        if (p_hndl->csd[1] & SDC_SD_CSD_DSR_IMP) /* Implemented */
        {
            /* Set DSR. (Issue CMD4.) */
            p_hndl->dsr[0] = SDC_SD_DSR_DEFAULT;
            if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD4, SDC_SD_RESP_NON, p_hndl->dsr[0]) != SDC_SD_SUCCESS)
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
                return SDC_SD_ERR;
            }
        }
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sdmem_init_rca() */

/**********************************************************************************************************************
* Outline      : Mount SD Memory Card
* Function Name: r_sdc_sdmem_mount
* Description  : Mounts the SD Memory and sets it from stand-by to transfer state.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_mount(uint32_t card_no)
{
    sdc_sd_hndl_t  * p_hndl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Change the clock (data-transfer ratio). ---- */
    if (SDC_SD_CLK_25MHz <= p_hndl->csd_tran_speed)
    {
        p_hndl->mem_tran_speed = r_sdc_sd_get_div_default_speed();
    }
    else
    {
        /* NOP */
    }
    if (R_SDHI_SetClock(p_hndl->channel, p_hndl->mem_tran_speed, SDHI_CLOCK_ENABLE) != SDHI_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return p_hndl->error;
    }

    /* ==== Check the status precede read operation. ==== */
    if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD13, SDC_SD_RESP_R1, p_hndl->rca[0]) != SDC_SD_SUCCESS)
    {
        if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD13, SDC_SD_RESP_R1, p_hndl->rca[0]) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
            return p_hndl->error;
        }
    }

    /* ==== Data-transfer mode (Transfer State) ==== */
    if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD7, SDC_SD_RESP_R1b, p_hndl->rca[0]) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        return p_hndl->error;
    }

    if (p_hndl->resp_status & SDC_SD_RES_CARD_IS_LOCKED)
    {
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_CARD_LOCK);
        return SDC_SD_SUCCESS;
    }

    /* ---- Set the block length. (Issue CMD16.) ---- */
    if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD16, SDC_SD_RESP_R1, SDC_SD_TRANS_BLOCK_SIZE) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        return p_hndl->error;
    }

    /* If 4bits transfer is supported (SD Memory card mandatory), change to the 4bits bus width. */
    if (p_hndl->media_type & SDC_SD_MEDIA_SDMEM)
    {
        if (r_sdc_sdmem_set_port(card_no, p_hndl->sup_if_mode) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
            return SDC_SD_ERR;
        }

        if (SDC_SD_MODE_1BIT == p_hndl->sup_if_mode)
        {
            R_SDHI_SetBus(p_hndl->channel, SDHI_PORT_1BIT);
        }
        else
        {
            R_SDHI_SetBus(p_hndl->channel, SDHI_PORT_4BIT);
        }

        /* Clear the pull-up DAT3. */
        if (r_sdc_sd_send_acmd(card_no, SDC_SD_ACMD42, 0) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
            return p_hndl->error;
        }
        /* Check the R1 response. */
        if (r_sdc_sd_get_resp(card_no, SDC_SD_RESP_R1) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
            return p_hndl->error;
        }
        /* Get the SD Status. */
        if (r_sdc_sdmem_card_get_sdstatus(card_no) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
            return p_hndl->error;
        }
        /* Get the protect area size. */
        if (r_sdc_sd_get_size(card_no, SDC_SD_PROT_AREA) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
            return p_hndl->error;
        }
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sdmem_mount() */

/**********************************************************************************************************************
* Outline      : Get SD Status
* Function Name: r_sdc_sdmem_card_get_sdstatus
* Description  : Gets the SD Status using ACMD13.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_card_get_sdstatus(uint32_t card_no)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    sdc_sd_status_t   ret = SDC_SD_SUCCESS;
    int32_t         i = 0;
    uint8_t         * p_rw_buff = 0;

    p_rw_buff = (uint8_t *)&g_sdc_sdmem_stat_buff[0];

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Get SD Status using ACMD13. ---- */
    if (r_sdc_sdmem_read_byte(card_no, SDC_SD_ACMD13, 0, p_rw_buff, SDC_SD_STATUS_BYTE) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        return SDC_SD_ERR;
    }

    /* ---- Save SD STATUS. ---- */
    /* Expand the SD STATUS relevant fields. */
    /* WAIT_LOOP */
    for (i = 0 ; i < 4 ; i++)
    {
#if (SDC_SD_BIG_ENDIAN)
        p_hndl->sdstatus[i] = g_sdc_sdmem_stat_buff[i];
#else
        p_hndl->sdstatus[i] = R_BSP_REVL(g_sdc_sdmem_stat_buff[i]);
#endif
    }

    /* ---- Distinguish the SD ROM card. ---- */
    if (0x00 == (p_hndl->sdstatus[0] & SDC_SD_STATUS_SD_CARD_TYPE_UPPER))
    {
        ret = SDC_SD_SUCCESS;
        if (0x01 == (p_hndl->sdstatus[0] & SDC_SD_STATUS_SD_CARD_TYPE_LOWER))
        {
            p_hndl->write_protect = (p_hndl->write_protect | SDC_SD_WP_ROM);
        }
    }
    else
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        ret = SDC_SD_ERR;
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_CARD_ERROR);
    }

    p_hndl->speed_class = (uint8_t)((p_hndl->sdstatus[2] & SDC_SD_STATUS_SPEED_CLASS) >> SDC_SD_BIT_SHIFT_24);
    p_hndl->perform_move = (uint8_t)((p_hndl->sdstatus[2] & SDC_SD_STATUS_PERFORMANCE_MOVE) >> SDC_SD_BIT_SHIFT_16);

    return ret;
} /* End of function r_sdc_sdmem_card_get_sdstatus() */

/**********************************************************************************************************************
* Outline      : Get SCR Register
* Function Name: r_sdc_sdmem_card_get_scr
* Description  : Gets the SCR register using ACMD51.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_card_get_scr(uint32_t card_no)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    int32_t           i = 0;
    uint8_t         * p_rw_buff = 0;

    p_rw_buff = (uint8_t *)&g_sdc_sdmem_stat_buff[0];

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Get SCR register using ACMD51. ---- */
    if (r_sdc_sdmem_read_byte(card_no, SDC_SD_ACMD51, 0, p_rw_buff, SDC_SD_SCR_REGISTER_BYTE) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        return SDC_SD_ERR;
    }

    /* ---- Save SCR register. ---- */
    /* WAIT_LOOP */
    for (i = 0 ; i < 2 ; i++)
    {
#if (SDC_SD_BIG_ENDIAN)
        p_hndl->scr[i] = g_sdc_sdmem_stat_buff[i];
#else
        p_hndl->scr[i] = R_BSP_REVL(g_sdc_sdmem_stat_buff[i]);
#endif
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sdmem_card_get_scr() */

/**********************************************************************************************************************
* Outline      : Read Byte Data from SD Card
* Function Name: r_sdc_sdmem_read_byte
* Description  : Reads byte data from SD card.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           acmd                ;   Command code
*              : uint32_t           arg                 ;   Command argument [31:0]
*              : uint8_t            * p_readbuff        ;   Read data buffer
*              : uint32_t           byte                ;   Number of read bytes
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_read_byte(uint32_t card_no, uint32_t cmd, uint32_t arg, uint8_t * p_readbuff,
                                       uint32_t byte)
{
    sdc_sd_hndl_t  * p_hndl = 0;
    uint32_t reg_base = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Clear SDBLKCNTEN in order to disable SDHI_SDBLKCNT. ---- */
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTOP, SDHI_SDSTOP_INIT);

    /* ---- Set transfer bytes. ---- */
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDSIZE, byte);

    /* ---- Issue a command. ---- */
    if (cmd & SDC_SD_ACMD)  /* ACMD13, ACMD22 and ACMD51 */
    {
        if (r_sdc_sd_send_acmd(card_no, cmd, arg) != SDC_SD_SUCCESS)
        {
            if ((SDC_SD_ERR_END_BIT == p_hndl->error) || (SDC_SD_ERR_CRC == p_hndl->error))
            {
                /* ---- Check the R1 response. ---- */
                if (r_sdc_sd_get_resp(card_no, SDC_SD_RESP_R1) != SDC_SD_SUCCESS)
                {
                    SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
                    r_sdc_sdmem_read_write_error(card_no);
                    return SDC_SD_ERR;
                }
            }
            else
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
                r_sdc_sdmem_read_write_error(card_no);
                return SDC_SD_ERR;
            }
        }
    }
    else    /* CMD30 */
    {
        /* Enable All end, BRE and errors interrupts. */
        if (r_sdc_sd_send_cmd_arg(card_no, cmd, SDC_SD_RESP_R1, arg) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
            r_sdc_sdmem_read_write_error(card_no);
            return SDC_SD_ERR;
        }
    }

    /* Enable All end, BRE and errors interrupts. */
    r_sdc_sd_disable_icu_int(card_no);
    r_sdc_sd_set_int_mask(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BRE);
    r_sdc_sd_enable_icu_int(card_no);

    /* ---- Wait for the BRE interrupt. ---- */
    if (r_sdc_sd_int_mem_wait(card_no, SDC_SD_TIMEOUT_MULTIPLE) != SDC_SD_SUCCESS)
    {
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_HOST_TOE);
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        r_sdc_sdmem_read_write_error(card_no);
        return SDC_SD_ERR;
    }   

    /* ---- Check the errors. ---- */
    if (p_hndl->sdsts2 & SDHI_SDIMSK2_ERR)
    {
        r_sdc_sd_check_sdsts2_err(card_no);
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        r_sdc_sdmem_read_write_error(card_no);
        return SDC_SD_ERR;
    }

    r_sdc_sd_clear_sdsts(card_no, 0, SDHI_SDIMSK2_RE); /* Clear the BRE bit. */

    /* Transfer data. */
    /* Enable All end, BRE and errors interrupts. */
    R_SDHI_GetBuffRegAddress(p_hndl->channel, &reg_base);
    if (r_sdc_sd_read_data(p_readbuff, reg_base, (int32_t)byte) != SDC_SD_SUCCESS)
    {
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_CPU_IF);
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        r_sdc_sdmem_read_write_error(card_no);
        return SDC_SD_ERR;
    }

    /* Wait for All end interrupt. */
    if (r_sdc_sd_int_mem_wait(card_no, SDC_SD_TIMEOUT_RESP) != SDC_SD_SUCCESS)
    {
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_HOST_TOE);
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        r_sdc_sdmem_read_write_error(card_no);
        return SDC_SD_ERR;
    }

    /* ---- Check the errors. ---- */
    if (p_hndl->sdsts2 & SDHI_SDIMSK2_ERR)
    {
        r_sdc_sd_check_sdsts2_err(card_no);
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        r_sdc_sdmem_read_write_error(card_no);
        return SDC_SD_ERR;
    }

    /* Disable all interrupts. */
    r_sdc_sd_disable_icu_int(card_no);
    r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BRE);
    r_sdc_sd_enable_icu_int(card_no);
    /* Clear All end bits. */
    r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_ERR);

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sdmem_read_byte() */

/**********************************************************************************************************************
* Outline      : Write Byte Data to SD Card
* Function Name: r_sdc_sdmem_write_byte
* Description  : Writes byte data to SD card
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           cmd                 ;   Command code
*              : uint32_t           arg                 ;   Command argument [31:0]
*              : uint8_t            * p_writebuff       ;   Write data buffer
*              : uint32_t           byte                ;   Number of write bytes
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : SDC_SD_ERR                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdmem_write_byte(uint32_t card_no, uint32_t cmd, uint32_t arg, uint8_t * p_writebuff,
                                        uint32_t byte)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    int32_t           time_out = 0;
    uint32_t reg_base = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ---- Clear SDBLKCNTEN in order to disable SDHI_SDBLKCNT. ---- */
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDSTOP, SDHI_SDSTOP_INIT);

    /* ---- Set transfer bytes. ---- */
    R_SDHI_OutReg(p_hndl->channel, SDHI_SDSIZE, byte);

    /* ---- Issue a command. ---- */
    if (r_sdc_sd_send_cmd_arg(card_no, cmd, SDC_SD_RESP_R1, arg) != SDC_SD_SUCCESS)
    {
        if (SDC_SD_ERR_CARD_LOCK == p_hndl->error)
        {
            p_hndl->error = SDC_SD_SUCCESS;
        }
        else
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
            r_sdc_sdmem_read_write_error(card_no);
            return SDC_SD_ERR;
        }
    }

    /* Enable All end, BWE and errors. */
    r_sdc_sd_disable_icu_int(card_no);
    r_sdc_sd_set_int_mask(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BWE);
    r_sdc_sd_enable_icu_int(card_no);

    /* ---- Wait for the BWE interrupt. ---- */
    if (r_sdc_sd_int_mem_wait(card_no, SDC_SD_TIMEOUT_MULTIPLE) != SDC_SD_SUCCESS)
    {
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_HOST_TOE);
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        r_sdc_sdmem_read_write_error(card_no);
        return SDC_SD_ERR;
    }

    /* ---- Check the errors. ---- */
    if (p_hndl->sdsts2 & SDHI_SDIMSK2_ERR)
    {
        r_sdc_sd_check_sdsts2_err(card_no);
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        r_sdc_sdmem_read_write_error(card_no);
        return SDC_SD_ERR;
    }

    r_sdc_sd_clear_sdsts(card_no, 0, SDHI_SDIMSK2_WE); /* Clear the BWE bit. */

    /* Transfer data. */
    R_SDHI_GetBuffRegAddress(p_hndl->channel, &reg_base);
    if (r_sdc_sd_write_data(p_writebuff, reg_base, (int32_t)byte) != SDC_SD_SUCCESS)
    {
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_CPU_IF);
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        r_sdc_sdmem_read_write_error(card_no);
        return SDC_SD_ERR;
    }

    /* Wait for All end interrupt. */
    if ((SDC_SD_CMD42 == cmd) && (1 == byte))
    {
        /* Forced erase timeout */
        time_out = SDC_SD_TIMEOUT_ERASE_CMD;
    }
    else
    {
        time_out = SDC_SD_TIMEOUT_RESP;
    }

    if (r_sdc_sd_int_mem_wait(card_no, time_out) != SDC_SD_SUCCESS)
    {
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_HOST_TOE);
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
        r_sdc_sdmem_read_write_error(card_no);
        return SDC_SD_ERR;
    }

    if (p_hndl->sdsts2 & SDHI_SDIMSK2_ERR)
    {
        r_sdc_sd_check_sdsts2_err(card_no);
        if (SDC_SD_TIMEOUT_ERASE_CMD == time_out)
        {
            /* Forced erase  */
            if (SDC_SD_ERR_CARD_TOE == p_hndl->error )
            {
                /* Forced erase timeout  */
                r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_TRNS_RESP, SDHI_SDIMSK2_ERR);
                if (r_sdc_sd_wait_rbusy(card_no, SDC_SD_TIMEOUT_FORCE_CMD) != SDC_SD_SUCCESS)
                {
                    SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
                    r_sdc_sdmem_read_write_error(card_no);
                    return SDC_SD_ERR;
                }
            }
            else
            {
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
                r_sdc_sdmem_read_write_error(card_no);
                return SDC_SD_ERR;
            }
        }
        else
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDMEM_INITIALIZE, __LINE__);
            r_sdc_sdmem_read_write_error(card_no);
            return SDC_SD_ERR;
        }
    }

    /* Disable all interrupts. */
    r_sdc_sd_disable_icu_int(card_no);
    r_sdc_sd_clear_int_mask(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_BWE);
    r_sdc_sd_enable_icu_int(card_no);
    /* Clear All end bits. */
    r_sdc_sd_clear_sdsts(card_no, SDHI_SDIMSK1_DATA_TRNS, SDHI_SDIMSK2_ERR);

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sdmem_write_byte() */

/**********************************************************************************************************************
* Outline      : Calculate Erase Sector
* Function Name: r_sdc_sdmem_calc_erase_sector
* Description  : Calculates erase sector
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
static sdc_sd_status_t r_sdc_sdmem_calc_erase_sector(uint32_t card_no)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    uint32_t          au = 0;
    uint32_t          erase_size = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);
    
    if (SDC_SD_SCR_SD_SPEC_2_0 == (p_hndl->scr[0] & SDC_SD_SCR_SD_SPEC_MASK))
    {
        /* AU is not defined. Set to fixed value. */
        p_hndl->erase_sect = SDC_SD_ERASE_SECTOR;
        /* Get AU size. */
        au = ((p_hndl->sdstatus[2] >> SDC_SD_BIT_SHIFT_12) & SDC_SD_STATUS_AU_SIZE);
        if (au > 0)
        {
            /* Get AU_SIZE(sectors). */
            p_hndl->erase_sect = ((8 * 1024) / SDC_SD_SECTOR_SIZE) << au;
            /* Get ERASE_SIZE. */ 
            erase_size = ((p_hndl->sdstatus[2] << SDC_SD_BIT_SHIFT_8) & SDC_SD_STATUS_ERASE_SIZE) |
                          (p_hndl->sdstatus[3] >> SDC_SD_BIT_SHIFT_24);
            if (0 != erase_size)
            {
                p_hndl->erase_sect = (p_hndl->erase_sect * erase_size);
            }
        }
    }
    else
    {
        /* If card is not Ver.2.00, use ERASE_BLK_LEN in CSD. */
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sdmem_calc_erase_sector() */

#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_MEM) */

/* End of File */
