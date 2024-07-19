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
 * File Name    : r_sdc_spi_getcarddetection.c
 * Version      : 1.0
 * Description  : Functions for module get card detection.
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
* Outline      : SD card get detect status
* Function Name: r_sdc_spi_getcarddetection
* Description  : Read CD pin and get detection status of SD card.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SPI_SUCCESS                        ;   Card detection or do not use CD pin.
*              : SDC_SPI_ERR                            ;   CD pin not detected.
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_getcarddetection(uint32_t card_no)
{
    sdc_spi_hndl_t * p_hndl = 0;

    /* Get handle address */
    p_hndl = r_sdc_spi_handle_get(card_no);

    /* If CD pin not use then return success */
    if (SDC_SPI_DISABLE == p_hndl->use_cd_pin)
    {
        return SDC_SPI_SUCCESS;
    }
    else
    {
        /* CD pin Read */
        if (GPIO_LEVEL_LOW == R_GPIO_PinRead(p_hndl->cd_pin))
        {
            return SDC_SPI_SUCCESS;
        }
        else
        {
            return SDC_SPI_ERR;
        }
    }

} /* End of function r_sdc_spi_getcarddetection() */

/* End of File */
