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
 * File Name    : r_sdc_spi_close.c
 * Version      : 1.0
 * Description  : Functions for module close.
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
* Outline      : Close SPI mode SD card driver
* Function Name: r_sdc_spi_close
* Description  : Close the SD Driver work memory specified by channel.
* Arguments    : uint32_t           card_no              ;   SD Card No.
* Return Value : SDC_SPI_SUCCESS                         ;   Successful operation
*              : SDC_SPI_ERR_INTERNAL                    ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_close(uint32_t card_no)
{
    sdc_spi_hndl_t * p_hndl = 0;

    /* Get handle address */
    p_hndl = r_sdc_spi_handle_get(card_no);

    /* I/O port initialize */
    if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
    {
        R_GPIO_PinDirectionSet(p_hndl->cs_pin, GPIO_DIRECTION_INPUT);
    }
    if (SDC_SPI_ENABLE == p_hndl->use_cd_pin)
    {
        R_GPIO_PinDirectionSet(p_hndl->cd_pin, GPIO_DIRECTION_INPUT);
    }
    if (SDC_SPI_ENABLE == p_hndl->use_wp_pin)
    {
        R_GPIO_PinDirectionSet(p_hndl->wp_pin, GPIO_DIRECTION_INPUT);
    }

    /* Handle parameter initialize */
    r_sdc_spi_mem_clear((uint8_t *)p_hndl, sizeof(sdc_spi_hndl_t));
    r_sdc_spi_handle_clear(card_no);

    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_close() */

/* End of File */
