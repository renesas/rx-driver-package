/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_sdc_spi_initialize.c
 * Version      : 1.0
 * Description  : Functions for module initialize.
  *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 28.12.2023 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_sdc_spi_private.h"
#include "r_memdrv_rx_if.h"
#include "r_memdrv_rx_config.h"

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
/* [0] is reserved. unit = 0.1 */
const uint8_t g_taac_value_tbl[16] = {0,10,12,13,15,20,25,30,35,40,45,50,55,60,70,80};

extern uint8_t g_sdc_spi_resbuf[SDC_SPI_MAX_RES_SIZE];

/**********************************************************************************************************************
* Outline      : Initialize SD Card
* Function Name: r_sdc_spi_initialize
* Description  : Initialize SD Card by channel.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_spi_cfg_t      * p_sdc_spi_config  ;   SPI mode SDC driver operation configuration.
*              : uint32_t           init_type           ;   SPI mode SDC driver init type.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------.
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_initialize(uint32_t card_no, sdc_spi_cfg_t * p_sdc_spi_config, uint32_t init_type)
{
    sdc_spi_hndl_t * p_hndl = 0;
#if ((MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_CFG_DEV1_MODE_DRVR) && (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_CFG_DEV1_MODE_DRVR_CH))
    sdc_spi_hndl_t * p_hndl2 = 0;
#endif
    st_memdrv_info_t  sdc_spi_memdrv_info;
    sdc_spi_status_t  ret;
    memdrv_err_t memdrv_err;

    /* Get handle address */
    p_hndl = r_sdc_spi_handle_get(card_no);

    /* I/O port initialize and CS pin output is high */
    r_sdc_spi_port_initialize(p_hndl);

    /* Handle parameter initialize */
    r_sdc_spi_handle_initialize(p_hndl);

    /* Setting the parameters used in MEMDRV */
    sdc_spi_memdrv_info.cnt = 0;
    sdc_spi_memdrv_info.p_data = NULL;
    sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
    sdc_spi_memdrv_info.read_after_write = true;

    /* Open MEMDRV corresponding to card_no */
    memdrv_err = R_MEMDRV_Open(p_hndl->dev_no, &sdc_spi_memdrv_info);
    if (MEMDRV_SUCCESS != memdrv_err)
    {
#if ((MEMDRV_CFG_DEV0_MODE_DRVR == MEMDRV_CFG_DEV1_MODE_DRVR) && (MEMDRV_CFG_DEV0_MODE_DRVR_CH == MEMDRV_CFG_DEV1_MODE_DRVR_CH))
        /* Get handle address */
        p_hndl = r_sdc_spi_handle_get(0);
        p_hndl2 = r_sdc_spi_handle_get(1);

        /* Check if already used in other channels */
        if (0 == card_no)
        {

            if ((NULL == p_hndl2) || ((NULL != p_hndl2) && (SDC_SPI_FALSE == p_hndl2->init_flag)))
            {
                r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
                return SDC_SPI_ERR_INTERNAL;
            }
        }
        else
        {
            if ((NULL == p_hndl)  || ((NULL != p_hndl) && (SDC_SPI_FALSE == p_hndl->init_flag)))
            {
                r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
                return SDC_SPI_ERR_INTERNAL;
            }
        }

        /* Get handle address */
        p_hndl = r_sdc_spi_handle_get(card_no);
#else
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return SDC_SPI_ERR_INTERNAL;
#endif
    }

    /* Enable MEMDRV corresponding to card_no */
    memdrv_err = R_MEMDRV_Enable(p_hndl->dev_no, &sdc_spi_memdrv_info);
    if (MEMDRV_SUCCESS != memdrv_err)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return SDC_SPI_ERR_INTERNAL;
    }

    /* SD card initialize sub routine. */
    ret = r_sdc_spi_initialize_sub(p_hndl, p_sdc_spi_config, init_type);
    if (SDC_SPI_SUCCESS > ret)
    {
        /* Disable MEMDRV corresponding to card_no */
        R_MEMDRV_Disable(p_hndl->dev_no, &sdc_spi_memdrv_info);

        /* Handle parameter initialize */
        r_sdc_spi_handle_initialize(p_hndl);

        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* disable MEMDRV corresponding to card_no */
    memdrv_err = R_MEMDRV_Disable(p_hndl->dev_no, &sdc_spi_memdrv_info);
    if (MEMDRV_SUCCESS != memdrv_err)
    {
        /* Handle parameter initialize */
        r_sdc_spi_handle_initialize(p_hndl);

        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return SDC_SPI_ERR_INTERNAL;
    }

    /* Initialize flag set */
    p_hndl->init_flag = SDC_SPI_TRUE;

    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_initialize() */

/**********************************************************************************************************************
* Outline      : Initialize SD Card sub routine
* Function Name: r_sdc_spi_initialize_sub
* Description  : Initialize SD Card sub routine.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : sdc_spi_cfg_t      * p_sdc_spi_config  ;   SPI mode SDC driver operation configuration.
*              : uint32_t           init_type           ;   SPI mode SDC driver init type.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------.
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_initialize_sub(sdc_spi_hndl_t * p_hndl, sdc_spi_cfg_t * p_sdc_spi_config, uint32_t init_type)
{
    sdc_spi_status_t  ret;

    /* Send dummy data */
    r_sdc_spi_tx_dummy_data(p_hndl, SDC_SPI_SPIMODE_SEL);

    /* CS pin output is low */
    if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
    {
        R_GPIO_PinWrite(p_hndl->cs_pin, GPIO_LEVEL_LOW);
    }

    /* SD card reset */
    ret = r_sdc_spi_reset_sd(p_hndl, p_sdc_spi_config, init_type);
    if (SDC_SPI_SUCCESS > ret)
    {
        /* CS pin output is high */
        if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
        {
            R_GPIO_PinWrite(p_hndl->cs_pin, GPIO_LEVEL_HIGH);
        }
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* Get write protection status */
    r_sdc_spi_get_wp(p_hndl);

    /* Send CMD59 command */
    ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_CMD59, SDC_SPI_CRC_OP_ON, SDC_SPI_R1_SIZE, SDC_SPI_OTHER_STAT);
    if (SDC_SPI_SUCCESS > ret)
    {
        /* CS pin output is high */
        if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
        {
            R_GPIO_PinWrite(p_hndl->cs_pin, GPIO_LEVEL_HIGH);
        }
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* Command wait ready */
    r_sdc_spi_wait_ready(p_hndl);

    /* CS pin output is high */
    if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
    {
        R_GPIO_PinWrite(p_hndl->cs_pin, GPIO_LEVEL_HIGH);
    }

    return ret;
} /* End of function r_sdc_spi_initialize_sub() */

/**********************************************************************************************************************
* Outline      : SD card reset
* Function Name: r_sdc_spi_reset_sd
* Description  : SD card reset routine.
* Arguments    : sdc_spi_hndl_t     * p_hndl             ;   SPI mode SDC driver handle
*              : sdc_spi_cfg_t      * p_sdc_spi_config   ;   SPI mode SDC driver operation configuration.
*              : uint32_t           init_type            ;   SPI mode SDC driver init type.
* Return Value : SDC_SPI_SUCCESS                         ;   Successful operation.
*              : SDC_SPI_ERR                             ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND             ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                         ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                    ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_reset_sd(sdc_spi_hndl_t * p_hndl, sdc_spi_cfg_t * p_sdc_spi_config, uint32_t init_type)
{
    uint32_t csize;
    uint8_t csdver;
    uint8_t ccs;
    sdc_spi_status_t ret;

    /* Send CMD0 command */
    ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_CMD0, 0, SDC_SPI_R1_SIZE, SDC_SPI_IDLE_STAT);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* CSD version check */
    ret = r_sdc_spi_chk_csdver(p_hndl, &csdver);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* Send CMD59 command */
    r_sdc_spi_send_cmd(p_hndl, SDC_SPI_CMD59, SDC_SPI_CRC_OP_ON, SDC_SPI_R1_SIZE, SDC_SPI_IDLE_STAT);

    /* Get OCR register */
    r_sdc_spi_get_ocr(p_hndl, SDC_SPI_IDLE_STAT);

    /* SD card initialize */
    ret = r_sdc_spi_sd_init(p_hndl, csdver);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* Get OCR register */
    ret = r_sdc_spi_get_ocr(p_hndl, SDC_SPI_OTHER_STAT);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* Check CCS register */
    ccs = r_sdc_spi_chk_ccs(p_hndl);

    /* Software delay */
    R_BSP_SoftwareDelay(1, BSP_DELAY_MILLISECS);

    /* Get CID register */
    ret = r_sdc_spi_get_cid(p_hndl);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* Get CSD register */
    ret = r_sdc_spi_get_csd(p_hndl);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* Get CSD information */
    if ((SDC_SPI_CSD_VER2 == csdver) && (SDC_SPI_SUCCESS == ccs))
    {
        /* CSD v2 information set */
        ret = r_sdc_spi_get_sd_csdinfov2(p_hndl, &csize);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
            return ret;
        }

        /* SD card status set */
        if (0xFFFF > csize)
        {
            p_hndl->media = SDC_SPI_CARD_SDHC;
        }
        else
        {
            p_hndl->media = SDC_SPI_CARD_SDXC;
        }
    }
    else
    {
        /* CSD v1 information set */
        ret = r_sdc_spi_get_sd_csdinfov1(p_hndl);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
            return ret;
        }

        /* SD card status set */
        p_hndl->media = SDC_SPI_CARD_SD;
    }

    /* SD card status check */
    ret = r_sdc_spi_chk_sts(p_hndl);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* Get SD card status */
    ret = r_sdc_spi_get_sdcard_type(p_hndl);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_reset_sd() */

/**********************************************************************************************************************
* Outline      : Get write protection status
* Function Name: r_sdc_spi_get_wp
* Description  : Get write protection status.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_get_wp(sdc_spi_hndl_t * p_hndl)
{

    /* Protect status reset */
    p_hndl->wp = SDC_SPI_NO_PROTECT;

    if (0x00 != p_hndl->csdinfo.wp)
    {
        p_hndl->wp = SDC_SPI_W_PROTECT_SOFT;
    }

    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_get_wp() */

/**********************************************************************************************************************
* Outline      : Get SD card type
* Function Name: r_sdc_spi_get_sdcard_type
* Description  : Get SD card type using ACMD13.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_get_sdcard_type(sdc_spi_hndl_t * p_hndl)
{
    volatile uint16_t sdcardtype;
    volatile uint8_t * p_regbuf;
    sdc_spi_status_t  ret;

    /* Get register buffer address. */
    p_regbuf = p_hndl->sdstatbuf;

    /* Send CMD55 command */
    ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_CMD55, 0, SDC_SPI_R1_SIZE, SDC_SPI_OTHER_STAT);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* Send ACMD13 command */
    ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_ACMD13, 0, SDC_SPI_R2_SIZE, SDC_SPI_OTHER_STAT);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* receive data check */
    ret = r_sdc_spi_chk_rxdata(p_hndl, p_hndl->csdinfo.nac, SDC_SPI_DTOKEN_STR_SR);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* receive CRC16 check */
    ret = r_sdc_spi_read_data_crc(p_hndl, SDC_SPI_SDSTAT_SIZE, (uint8_t *)p_regbuf);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* Get SD card type */
    sdcardtype = ((uint16_t)p_regbuf[2] << 8);
    sdcardtype |= ((uint16_t)p_regbuf[3] & 0x00FF);

    /* Set media according to sdcardtype */
    if (0x0000 == sdcardtype)
    {
        p_hndl->media |= SDC_SPI_CARD_WR;
    }
    else if (0x0001 == sdcardtype)
    {
        p_hndl->media |= SDC_SPI_CARD_ROM;
    }
    else if (0x0002 == sdcardtype)
    {
        p_hndl->media |= SDC_SPI_CARD_OTP;
    }
    else
    {
        /* Do Nothing */
    }

    return SDC_SPI_SUCCESS;

} /* End of function r_sdc_spi_get_sdcard_type() */

/**********************************************************************************************************************
* Outline      : Get SD card spec version
* Function Name: r_sdc_spi_get_sd_specver
* Description  : Get SD card spec version using ACMD51.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : int32_t                                ;   SD spec version number
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
int32_t r_sdc_spi_get_sd_specver(sdc_spi_hndl_t * p_hndl)
{
    volatile uint16_t sdspec;
    volatile uint8_t * p_regbuf;
    sdc_spi_status_t  ret;

    /* Get SCR buffer address. */
    p_regbuf = p_hndl->scrbuf;

    /* Send CMD55 command */
    ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_CMD55, 0, SDC_SPI_R1_SIZE, SDC_SPI_OTHER_STAT);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* Send ACMD51 command */
    ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_ACMD51, 0, SDC_SPI_R2_SIZE, SDC_SPI_OTHER_STAT);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* receive data check */
    ret = r_sdc_spi_chk_rxdata(p_hndl, p_hndl->csdinfo.nac, SDC_SPI_DTOKEN_STR_SR);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* receive CRC16 check */
    ret = r_sdc_spi_read_data_crc(p_hndl, SDC_SPI_SCR_SIZE, (uint8_t *)p_regbuf);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* SD spec check */
    sdspec = (*p_regbuf) & 0x0F;
    if (2 == sdspec)
    {
        if (0x80 == ((*(p_regbuf + 2)) & 0x80))
        {
            sdspec = 3;
        }
    }

    return (int32_t)sdspec;
} /* End of function r_sdc_spi_get_sd_specver() */

/**********************************************************************************************************************
* Outline      : Get CSD information from version 1.0 card.
* Function Name: r_sdc_spi_get_sd_csdinfov1
* Description  : Get CSD information from version 1.0 card.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_get_sd_csdinfov1(sdc_spi_hndl_t * p_hndl)
{
    volatile uint32_t rblklen;
    volatile uint32_t csize;
    volatile uint32_t csizemult;
    volatile csd_info_t * p_csdinfo;
    volatile uint8_t * p_csdbuf;

    /* Get CSD buffer and CSD info address. */
    p_csdbuf = (p_hndl->csdbuf);
    p_csdinfo = &(p_hndl->csdinfo);

    /* CSD check */
    if (SDC_SPI_CSD_STRUCTURE_V1 != ((*p_csdbuf) & 0xC0))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return SDC_SPI_ERR;
    }

    /* Get block length */
    rblklen = (uint32_t)(*(p_csdbuf + 5)) & 0x000FU;
    if ((SDC_SPI_READ_BLK_LEN_MIN >= rblklen) || (SDC_SPI_READ_BLK_LEN_MAX <= rblklen))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return SDC_SPI_ERR;
    }

    /* Set csize */
    csize = (((uint32_t)(*(p_csdbuf + 6)) << 10) | ((uint32_t)(*(p_csdbuf + 7)) << 2) |
            ((uint32_t)(*(p_csdbuf + 8)) >> 6)) & 0x0FFFU;
    csizemult = (((uint32_t)(*(p_csdbuf + 9)) << 1) | ((uint32_t)(*(p_csdbuf + 10)) >> 7)) & 0x0007U;

    /* Set csd information */
    p_csdinfo->memsize = (csize + 1) * (1U << (csizemult + 2)) * (1U << rblklen);   /* Refer to SD card spec. */
    p_csdinfo->maxblkno = (p_csdinfo->memsize / SDC_SPI_BLK_SIZE) - 1;      /* Refer to SD card spec. */
    p_csdinfo->taac = 1000; /* Fix value, Refer to SD card spec. */
    p_csdinfo->nsac = 0;    /* Fix value, Refer to SD card spec. */
    p_csdinfo->nac = ((p_csdinfo->taac * (p_hndl->bitrate / 10000) ) +
            ((uint32_t)p_csdinfo->nsac * 10000) );   /* Refer to MMC card spec */
    p_csdinfo->wp = ((*(p_csdbuf + 14)) >> 4) & 0x03;   /* Refer to SD card spec. */
    p_hndl->addrunit = SDC_SPI_BLK_SIZE;

    return SDC_SPI_SUCCESS;

} /* End of function r_sdc_spi_get_sd_csdinfov1() */

/**********************************************************************************************************************
* Outline      : Get CSD information from version 2.0 card.
* Function Name: r_sdc_spi_get_sd_csdinfov2
* Description  : Get CSD information from version 2.0 card.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint32_t           * p_csize           ;   csize pointer.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_get_sd_csdinfov2(sdc_spi_hndl_t * p_hndl, uint32_t * p_csize)
{
    volatile csd_info_t * p_csdinfo;
    volatile uint8_t * p_csdbuf;
    volatile uint8_t taac_unit_tbl_idx;
    volatile uint8_t taac_value;

    /* Get CSD buffer and CSD info address. */
    p_csdbuf = (p_hndl->csdbuf);
    p_csdinfo = &(p_hndl->csdinfo);

    /* CSD check */
    if (SDC_SPI_CSD_STRUCTURE_V2 != ((*(p_csdbuf)) & 0xC0))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return SDC_SPI_ERR;
    }

    /* csize check */
    *p_csize = (((uint32_t)(*(p_csdbuf + 7)) << 16) | ((uint32_t)(*(p_csdbuf + 8)) << 8) |
            (uint32_t)(*(p_csdbuf + 9))) & 0x3FFFFU;

    if (0 == (*p_csize))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return SDC_SPI_ERR;
    }

    /* Set csd information */
    p_csdinfo->memsize = SDC_SPI_MEMSIZE_DUMMY; /* Refer to SD card spec. */
    p_csdinfo->maxblkno = (((*p_csize) + 1) * 1024) - 1;  /* Refer to SD card spec. */
    taac_unit_tbl_idx = (*(p_csdbuf+1)) & 0x07; /* Refer to SD card spec. */
    taac_value = g_taac_value_tbl[((*(p_csdbuf+1)) & 0x78) >> 3];   /* Refer to SD card spec. */

    /* Refer to SD card spec. */
    p_csdinfo->taac = ((uint32_t)taac_value * ((uint32_t)g_taac_value_tbl[taac_unit_tbl_idx] / 10));
    if (0 == p_csdinfo->taac)
    {
        p_csdinfo->taac = 1;
    }

    p_csdinfo->nsac = (uint16_t)*(p_csdbuf + 2);    /* Refer to SD card spec. */
    p_csdinfo->nac = (((uint32_t)p_csdinfo->nsac * 10000) +
            (p_csdinfo->taac * (p_hndl->bitrate / 10000) )); /* Refer to MMC card spec. */

    p_csdinfo->wp = ((*(p_csdbuf + 14)) >> 4) & 0x03;   /* Refer to SD card spec. */
    p_hndl->addrunit = 1;   /* Fix value, Refer to SD card spec. */

    return SDC_SPI_SUCCESS;

} /* End of function r_sdc_spi_get_sd_csdinfov2() */

/**********************************************************************************************************************
* Outline      : Get CSD register.
* Function Name: r_sdc_spi_get_csd
* Description  : Get CSD register using CMD9.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Other error operation
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error
*              : SDC_SPI_ERR_CRC                        ;   CRC error
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_get_csd(sdc_spi_hndl_t * p_hndl)
{
    volatile exchg_short_t crcccittbuf;
    volatile uint16_t crcccittdata;
    volatile uint8_t crc7data;
    volatile uint8_t * p_csdbuf;
    volatile uint32_t work_data_buf;
    sdc_spi_status_t ret;

    /* Get CSD buffer address. */
    p_csdbuf = (p_hndl->csdbuf);

    /* Send CMD9 command */
    ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_CMD9, 0, SDC_SPI_R1_SIZE, SDC_SPI_OTHER_STAT);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* check rxdata */
    ret = r_sdc_spi_chk_rxdata(p_hndl, (uint32_t)SDC_SPI_NCX_MAX, SDC_SPI_DTOKEN_STR_SR);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* Get CRC16 data */
    ret = r_sdc_spi_rx_data_crc16(p_hndl, SDC_SPI_CSD_SIZE, (uint8_t *)p_csdbuf, (uint16_t *)&crcccittdata);
    if (SDC_SPI_SUCCESS > ret)
    {
        /* csdbuf clear */
        r_sdc_spi_mem_clear((uint8_t *)p_csdbuf, SDC_SPI_CSD_SIZE);
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* Receive CRC16 first byte */
    ret = r_sdc_spi_rx_data(p_hndl, (uint16_t)1, (uint8_t *)&work_data_buf);
    if (SDC_SPI_SUCCESS > ret)
    {
        /* csdbuf clear */
        r_sdc_spi_mem_clear((uint8_t *)p_csdbuf, SDC_SPI_CSD_SIZE);
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }
#ifdef __BIG
    crcccittbuf.uc[1] = (uint8_t)((work_data_buf & 0xff000000) >> SDC_SPI_BIT_SHIFT_24);
#else
    crcccittbuf.uc[1] = (uint8_t)(work_data_buf & 0x000000ff);
#endif

    /* Receive CRC16 second byte  */
    ret = r_sdc_spi_rx_data(p_hndl, (uint16_t)1, (uint8_t *)&work_data_buf);
    if (SDC_SPI_SUCCESS > ret)
    {
        /* csdbuf clear */
        r_sdc_spi_mem_clear((uint8_t *)p_csdbuf, SDC_SPI_CSD_SIZE);
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }
#ifdef __BIG
    crcccittbuf.uc[0] = (uint8_t)((work_data_buf & 0xff000000) >> SDC_SPI_BIT_SHIFT_24);
#else
    crcccittbuf.uc[0] = (uint8_t)(work_data_buf & 0x000000ff);
#endif

    /* CRC7 verifier　*/
    crc7data = r_sdc_spi_crc7((uint8_t *)p_csdbuf, (uint16_t)(SDC_SPI_CSD_SIZE-1)) | 0x01;
    if (crc7data != (*(p_csdbuf + SDC_SPI_CSD_CRC7IDX)))
    {
        /* csdbuf clear */
        r_sdc_spi_mem_clear((uint8_t *)p_csdbuf, SDC_SPI_CSD_SIZE);
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return SDC_SPI_ERR_CRC;
    }

    /* Check crc */
    if (crcccittdata != crcccittbuf.us)
    {
        /* csdbuf clear */
        r_sdc_spi_mem_clear((uint8_t *)p_csdbuf, SDC_SPI_CSD_SIZE);
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return SDC_SPI_ERR_CRC;
    }

    return SDC_SPI_SUCCESS;

} /* End of function r_sdc_spi_get_csd() */

/**********************************************************************************************************************
* Outline      : Get CID register.
* Function Name: r_sdc_spi_get_cid
* Description  : Get CID register using CMD10.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Other error operation
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error
*              : SDC_SPI_ERR_CRC                        ;   CRC error
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_get_cid(sdc_spi_hndl_t * p_hndl)
{
    volatile exchg_short_t crcccittbuf;
    volatile uint16_t crcccittdata;
    volatile uint8_t crc7data;
    volatile uint8_t * p_cidbuf;
    volatile uint32_t work_data_buf;
    sdc_spi_status_t ret;

    /* Get CID buffer address. */
    p_cidbuf = (p_hndl->cidbuf);

    /* Send CMD10 command */
    ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_CMD10, 0, SDC_SPI_R1_SIZE, SDC_SPI_OTHER_STAT);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* check rxdata */
    ret = r_sdc_spi_chk_rxdata(p_hndl, (uint32_t)SDC_SPI_NCX_MAX, SDC_SPI_DTOKEN_STR_SR);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* Get CRC16 data */
    ret = r_sdc_spi_rx_data_crc16(p_hndl, SDC_SPI_CID_SIZE, (uint8_t *)p_cidbuf, (uint16_t *)&crcccittdata);
    if (SDC_SPI_SUCCESS > ret)
    {
        /* cidbuf clear */
        r_sdc_spi_mem_clear((uint8_t *)p_cidbuf, SDC_SPI_CID_SIZE);

        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* Receive CRC16 first byte */
    ret = r_sdc_spi_rx_data(p_hndl, (uint16_t)1, (uint8_t *)&work_data_buf);
    if (SDC_SPI_SUCCESS > ret)
    {
        /* cidbuf clear */
        r_sdc_spi_mem_clear((uint8_t *)p_cidbuf, SDC_SPI_CID_SIZE);
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }
#ifdef __BIG
    crcccittbuf.uc[1] = (uint8_t)((work_data_buf & 0xff000000) >> SDC_SPI_BIT_SHIFT_24);
#else
    crcccittbuf.uc[1] = (uint8_t)(work_data_buf & 0x000000ff);
#endif

    /* Receive CRC16 second byte  */
    ret = r_sdc_spi_rx_data(p_hndl, (uint16_t)1, (uint8_t *)&work_data_buf);
    if (SDC_SPI_SUCCESS > ret)
    {
        /* cidbuf clear */
        r_sdc_spi_mem_clear((uint8_t *)p_cidbuf, SDC_SPI_CID_SIZE);
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }
#ifdef __BIG
    crcccittbuf.uc[0] = (uint8_t)((work_data_buf & 0xff000000) >> SDC_SPI_BIT_SHIFT_24);
#else
    crcccittbuf.uc[0] = (uint8_t)(work_data_buf & 0x000000ff);
#endif

    /* CRC7 verifier　*/
    crc7data = r_sdc_spi_crc7((uint8_t *)p_cidbuf, (uint16_t)(SDC_SPI_CID_SIZE-1)) | 0x01;
    if (crc7data != (*(p_cidbuf + SDC_SPI_CID_CRC7IDX)))
    {
        /* cidbuf clear */
        r_sdc_spi_mem_clear((uint8_t *)p_cidbuf, SDC_SPI_CID_SIZE);
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return SDC_SPI_ERR_CRC;
    }

    /* Check crc */
    if (crcccittdata != crcccittbuf.us)
    {
        /* cidbuf clear */
        r_sdc_spi_mem_clear((uint8_t *)p_cidbuf, SDC_SPI_CID_SIZE);
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return SDC_SPI_ERR_CRC;
    }

    return SDC_SPI_SUCCESS;

} /* End of function r_sdc_spi_get_cid() */

/**********************************************************************************************************************
* Outline      : Get OCR register.
* Function Name: r_sdc_spi_get_ocr
* Description  : Get OCR register using CMD58.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint8_t            stat                ;   Current status.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_SUCCESS_IDLE                   ;   Successful operation and idle status
*              : SDC_SPI_ERR                            ;   Other error operation
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error
*              : SDC_SPI_ERR_CRC                        ;   CRC error
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_get_ocr(sdc_spi_hndl_t * p_hndl, uint8_t stat)
{
    volatile uint8_t * p_ocrbuf;
    sdc_spi_status_t  ret;

    /* Get OCR buffer address. */
    p_ocrbuf = (p_hndl->ocrbuf);

    /* Send CMD58 command */
    ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_CMD58, 0, SDC_SPI_R3_SIZE, stat);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* Check idle status  */
    if ((SDC_SPI_SUCCESS_IDLE == ret)  ||
            (SDC_SPI_OCR_BUSY != (g_sdc_spi_resbuf[SDC_SPI_RES_R3IDX] & SDC_SPI_OCR_BUSY)) )
    {
        return SDC_SPI_SUCCESS_IDLE;
    }

    /* Get OCR buffer */
    *p_ocrbuf = g_sdc_spi_resbuf[SDC_SPI_RES_R3IDX];
    *(p_ocrbuf+1) = g_sdc_spi_resbuf[SDC_SPI_RES_R3IDX+1];
    *(p_ocrbuf+2) = g_sdc_spi_resbuf[SDC_SPI_RES_R3IDX+2];
    *(p_ocrbuf+3) = g_sdc_spi_resbuf[SDC_SPI_RES_R3IDX+3];

    return SDC_SPI_SUCCESS;

} /* End of function r_sdc_spi_get_ocr() */

/**********************************************************************************************************************
* Outline      : Check card capacity
* Function Name: r_sdc_spi_chk_ccs
* Description  : Check card capacity
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : SDC_SPI_SUCCESS                        ;   Capacity is over SDHC
*              : SDC_SPI_ERR                            ;   Capacity is SDSC
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_chk_ccs(sdc_spi_hndl_t * p_hndl)
{
    volatile uint8_t * p_ocrbuf;

    /* Get OCR buffer address. */
    p_ocrbuf = p_hndl->ocrbuf;

    /* Check card status */
    if (SDC_SPI_OCR_CCS == ((*(p_ocrbuf)) & SDC_SPI_OCR_CCS))
    {
        return SDC_SPI_SUCCESS;
    }
    else
    {
        return SDC_SPI_ERR;
    }
} /* End of function r_sdc_spi_chk_ccs() */

/**********************************************************************************************************************
* Outline      : Check CSD version
* Function Name: r_sdc_spi_chk_csdver
* Description  : Check CSD version using CMD8.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint8_t            * p_csdver          ;   CSD version pointer.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_SUCCESS_IDLE                   ;   Successful operation and idle status
*              : SDC_SPI_ERR                            ;   Failed operation
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_chk_csdver(sdc_spi_hndl_t * p_hndl, uint8_t * p_csdver)
{
    sdc_spi_status_t  ret;

    /* Send CMD8 command */
    ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_CMD8,  SDC_SPI_CMD8_VHS |  SDC_SPI_CMD8_CHKPTN, SDC_SPI_R7_SIZE, SDC_SPI_IDLE_STAT);
    if (SDC_SPI_ERR_ILLEGAL_COMMAND == ret)
    {
        *p_csdver = SDC_SPI_CSD_VER1;
        return SDC_SPI_SUCCESS;
    }

    if (SDC_SPI_SUCCESS_IDLE != ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return ret;
    }

    /* Check pattern */
    if (SDC_SPI_CMD8_CHKPTN != g_sdc_spi_resbuf[SDC_SPI_RES_R7IDX_CHKPTN])
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return SDC_SPI_ERR;
    }

    /* R7 response check */
    if (SDC_SPI_RES_R7_VHS != (g_sdc_spi_resbuf[SDC_SPI_RES_R7IDX_VHS] & 0x0F))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
        return SDC_SPI_ERR;
    }

    *p_csdver = SDC_SPI_CSD_VER2;
    return SDC_SPI_SUCCESS;

} /* End of function r_sdc_spi_chk_csdver() */


/**********************************************************************************************************************
* Outline      : SD card initialize
* Function Name: r_sdc_spi_sd_init
* Description  : SD card initialize using ACMD41
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint8_t            csdver              ;   CSD version pointer.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_sd_init(sdc_spi_hndl_t * p_hndl, uint8_t csdver)
{
    sdc_spi_status_t ret;
    uint32_t arg;
    uint16_t rtycnt;

    /* Check CSD version to determine command arguments. */
    if (SDC_SPI_CSD_VER2 == csdver)
    {
        arg = SDC_SPI_ACMD41_HCS_ON;
    }
    else
    {
        arg = SDC_SPI_ACMD41_HCS_OFF;
    }

    /* Command send */
    /* WAIT_LOOP */
    for (rtycnt = 512; 0 != rtycnt; rtycnt--)
    {
        /* Send CMD55 command */
        ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_CMD55, 0, SDC_SPI_R1_SIZE, SDC_SPI_ALL_STAT);
        if (SDC_SPI_SUCCESS <= ret)
        {
            /* Send ACMD41 command */
            ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_ACMD41, arg, SDC_SPI_R1_SIZE, SDC_SPI_ALL_STAT);
            if (SDC_SPI_SUCCESS == ret)
            {
                return SDC_SPI_SUCCESS;
            }
        }

        /* ACMD41 command interval time. Refer to 4.4 of SD card spec */
        R_BSP_SoftwareDelay(4, BSP_DELAY_MILLISECS);
    }

    r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
    return SDC_SPI_ERR;

} /* End of function r_sdc_spi_sd_init() */

/**********************************************************************************************************************
* Outline      : Access end SD card
* Function Name: r_sdc_spi_end
* Description  : Access end SD card.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_end(uint32_t card_no)
{
    sdc_spi_hndl_t * p_hndl = 0;
    sdc_spi_hndl_t * p_hndl2 = 0;
    st_memdrv_info_t  sdc_spi_memdrv_info;
    memdrv_err_t memdrv_err;

    /* Get handle address */
    p_hndl = r_sdc_spi_handle_get(0);
    p_hndl2 = r_sdc_spi_handle_get(1);

    /* If not used in other channels than MEMDRV close */
    if (0 == card_no)
    {
        if ((0 == p_hndl2) || ((NULL != p_hndl2) && (SDC_SPI_FALSE == p_hndl2->init_flag)))
        {
            /* Setting the parameters used in MEMDRV */
            sdc_spi_memdrv_info.cnt = 0;
            sdc_spi_memdrv_info.p_data = NULL;
            sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
            sdc_spi_memdrv_info.read_after_write = true;

            /* Close MEMDRV corresponding to card_no */
            memdrv_err = R_MEMDRV_Close(p_hndl->dev_no, &sdc_spi_memdrv_info);
            if (MEMDRV_SUCCESS != memdrv_err)
            {
                r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
                return SDC_SPI_ERR_INTERNAL;
            }
        }
    }
    else
    {
        if ((0 == p_hndl) || ((NULL != p_hndl) && (SDC_SPI_FALSE == p_hndl->init_flag)))
        {
            /* Setting the parameters used in MEMDRV */
            sdc_spi_memdrv_info.cnt = 0;
            sdc_spi_memdrv_info.p_data = NULL;
            sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
            sdc_spi_memdrv_info.read_after_write = true;

            /* Close MEMDRV corresponding to card_no */
            memdrv_err = R_MEMDRV_Close(p_hndl2->dev_no, &sdc_spi_memdrv_info);
            if (MEMDRV_SUCCESS != memdrv_err)
            {
                r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_INITIALIZE, __LINE__);
                return SDC_SPI_ERR_INTERNAL;
            }
        }
    }

    /* Get handle address */
    p_hndl = r_sdc_spi_handle_get(card_no);

    /* Handle initialize */
    r_sdc_spi_handle_initialize(p_hndl);
    return SDC_SPI_SUCCESS;

} /* End of function r_sdc_spi_end() */

/**********************************************************************************************************************
* Outline      : Initialize handle data.
* Function Name: r_sdc_spi_handle_initialize
* Description  : Initialize handle data.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : None
*----------------------------------------------------------------------------------------------------------------------.
* Notes        : None
**********************************************************************************************************************/
void r_sdc_spi_handle_initialize(sdc_spi_hndl_t * p_hndl)
{

    /* Handle parameter setting */
    p_hndl->media = SDC_SPI_CARD_UNDETECT;
    p_hndl->wp = SDC_SPI_W_PROTECT_HARD | SDC_SPI_W_PROTECT_SOFT;

    /* csdinfo initialize */
    r_sdc_spi_mem_clear((uint8_t *)&p_hndl->csdinfo, sizeof(p_hndl->csdinfo));

    /* CSD buff initialize */
    r_sdc_spi_mem_clear(p_hndl->csdbuf, SDC_SPI_CSD_SIZE);

    /* CID buff initialize */
    r_sdc_spi_mem_clear(p_hndl->cidbuf, SDC_SPI_CID_SIZE);

    /* OCR buff initialize */
    r_sdc_spi_mem_clear(p_hndl->ocrbuf, SDC_SPI_OCR_SIZE);

    /* SD status buff initialize */
    r_sdc_spi_mem_clear(p_hndl->sdstatbuf, SDC_SPI_SDSTAT_SIZE);

    /* SCR buff initialize */
    r_sdc_spi_mem_clear(p_hndl->scrbuf, SDC_SPI_SCR_SIZE);

    /* Initialize flag clear */
    p_hndl->init_flag = SDC_SPI_FALSE;

}/* End of function r_sdc_spi_handle_initialize() */

/**********************************************************************************************************************
* Outline      : Initialize port.
* Function Name: r_sdc_spi_port_initialize
* Description  : CD pin and WP pin is input, and CS pin output is high.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : None
*----------------------------------------------------------------------------------------------------------------------.
* Notes        : None
**********************************************************************************************************************/
void r_sdc_spi_port_initialize(sdc_spi_hndl_t * p_hndl)
{

    /* CS pin output is high */
    if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
    {
        R_GPIO_PinWrite(p_hndl->cs_pin, GPIO_LEVEL_HIGH);
        R_GPIO_PinDirectionSet(p_hndl->cs_pin, GPIO_DIRECTION_OUTPUT);
    }

    /* CD pin is input */
    if (SDC_SPI_ENABLE == p_hndl->use_cd_pin)
    {
        R_GPIO_PinDirectionSet(p_hndl->cd_pin, GPIO_DIRECTION_INPUT);
    }

    /* WP pin is input */
    if (SDC_SPI_ENABLE == p_hndl->use_wp_pin)
    {
        R_GPIO_PinDirectionSet(p_hndl->wp_pin, GPIO_DIRECTION_INPUT);
    }
}/* End of function r_sdc_spi_port_initialize() */


/**********************************************************************************************************************
* Outline      : memory clear function.
* Function Name: r_sdc_spi_mem_clear
* Description  : memory clear function.
* Arguments    : uint8_t        * p_ptr           ; Clear buffer pointer.
*              : uint32_t       cnt               ; Clear buffer counter.
* Return Value : None
*----------------------------------------------------------------------------------------------------------------------.
* Notes        : None
**********************************************************************************************************************/
void r_sdc_spi_mem_clear(uint8_t * p_ptr, uint32_t cnt)
{
    uint32_t i;

    /* WAIT_LOOP */
    for (i = cnt; i != 0; i--)
    {
        *p_ptr = 0;
        p_ptr++;
    }
    
}/* End of function r_sdc_spi_mem_clear() */

/* End of File */
