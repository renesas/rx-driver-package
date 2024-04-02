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
 * File Name    : r_sdc_spi_get_info.c
 * Version      : 1.0
 * Description  : Functions for module get card information.
  *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 28.12.2023 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_sdc_spi_private.h"

/**********************************************************************************************************************
* Outline      : Get card information
* Function Name: r_sdc_spi_getcardinfo
* Description  : Get card information from handle.
* Arguments    : uint32_t           card_no              ;   SD Card No.
*              : sdc_spi_card_reg_t * p_sdc_spi_card_reg ;   Card information struct pointer
* Return Value : SDC_SPI_SUCCESS                         ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_getcardinfo(uint32_t card_no, sdc_spi_card_reg_t * p_sdc_spi_card_reg)
{
    sdc_spi_hndl_t * p_hndl = 0;
    uint8_t i;

    /* Get handle address */
    p_hndl = r_sdc_spi_handle_get(card_no);

    /* Get OCR register */
    p_sdc_spi_card_reg->ocr[0] = p_hndl->ocrbuf[0] | (p_hndl->ocrbuf[1] << SDC_SPI_BIT_SHIFT_8) |
            (p_hndl->ocrbuf[2] << SDC_SPI_BIT_SHIFT_16) | (p_hndl->ocrbuf[3] << SDC_SPI_BIT_SHIFT_24);

    /* Get SCR register */
    /* WAIT_LOOP */
    for (i = 0; i < SDC_SPI_SCR_SIZE; i += 4)
    {
        p_sdc_spi_card_reg->scr[i] = p_hndl->scrbuf[i] | (p_hndl->scrbuf[i+1] << SDC_SPI_BIT_SHIFT_8) |
                (p_hndl->scrbuf[i+2] << SDC_SPI_BIT_SHIFT_16) | (p_hndl->scrbuf[i+3] << SDC_SPI_BIT_SHIFT_24);
    }

    /* Get CID register and CSD register, and status buffer */
    /* WAIT_LOOP */
    for (i = 0; i < 16; i += 4)
    {
        p_sdc_spi_card_reg->cid[i] = p_hndl->cidbuf[i] | (p_hndl->cidbuf[i+1] << SDC_SPI_BIT_SHIFT_8) |
                (p_hndl->cidbuf[i+2] << SDC_SPI_BIT_SHIFT_16) | (p_hndl->cidbuf[i+3] << SDC_SPI_BIT_SHIFT_24);

        p_sdc_spi_card_reg->csd[i] = p_hndl->csdbuf[i] | (p_hndl->csdbuf[i+1] << SDC_SPI_BIT_SHIFT_8) |
                (p_hndl->csdbuf[i+2] << SDC_SPI_BIT_SHIFT_16) | (p_hndl->csdbuf[i+3] << SDC_SPI_BIT_SHIFT_24);

        p_sdc_spi_card_reg->sdstatus[i] = p_hndl->sdstatbuf[i] | (p_hndl->sdstatbuf[i+1] << SDC_SPI_BIT_SHIFT_8) |
                (p_hndl->sdstatbuf[i+2] << SDC_SPI_BIT_SHIFT_16) | (p_hndl->sdstatbuf[i+3] << SDC_SPI_BIT_SHIFT_24);
    }

    return SDC_SPI_SUCCESS;
} /* End of function sdc_spi_status_r_sdc_spi_getcardinfo() */

/* End of File */

