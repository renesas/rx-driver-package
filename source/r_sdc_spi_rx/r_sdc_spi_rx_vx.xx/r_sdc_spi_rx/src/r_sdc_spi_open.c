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
 * File Name    : r_sdc_spi_open.c
 * Version      : 1.0
 * Description  : Functions for module open.
  *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 28.12.2023 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_sdc_spi_private.h"
#include "r_memdrv_rx_config.h"

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
static sdc_spi_hndl_t * gs_sdc_spi_handle[SDC_SPI_CARD_NUM];

/**********************************************************************************************************************
* Outline      : Initialize SPI mode SD card driver
* Function Name: r_sdc_spi_open
* Description  : Initializes the SPI mode SD card driver work memory specified by channel.
* Arguments    : uint32_t           card_no              ;   SD Card No.
*              : uint32_t           dev_no               ;   MEMDRV Channel No.
*              : void               * p_sdc_spi_workarea ;   SDC SPI mode Driver work memory
* Return Value : SDC_SPI_SUCCESS                         ;   Successful operation
*              : SDC_SPI_ERR_INTERNAL                    ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_open(uint32_t card_no, uint32_t dev_no, void * p_sdc_spi_workarea)
{
    sdc_spi_hndl_t * p_hndl = 0;

    /* Handle parameter initialize */
    p_hndl = (sdc_spi_hndl_t *)p_sdc_spi_workarea;
    gs_sdc_spi_handle[card_no] = p_hndl;

    /* p_sdc_spi_workarea clear */
    r_sdc_spi_mem_clear((uint8_t *)p_hndl, sizeof(sdc_spi_hndl_t));

    /* Use pin setting. */
    if (0 == card_no)
    {
        /* Cast according to variable */
        p_hndl->cd_pin = (gpio_port_pin_t)((SDC_SPI_CFG_CH0_CD_PORT << SDC_SPI_BIT_SHIFT_8) | SDC_SPI_CFG_CH0_CD_BIT);
        p_hndl->cs_pin = (gpio_port_pin_t)((SDC_SPI_CFG_CH0_CS_PORT << SDC_SPI_BIT_SHIFT_8) | SDC_SPI_CFG_CH0_CS_BIT);
        p_hndl->wp_pin = (gpio_port_pin_t)((SDC_SPI_CFG_CH0_WP_PORT << SDC_SPI_BIT_SHIFT_8) | SDC_SPI_CFG_CH0_WP_BIT);
        p_hndl->use_cd_pin = SDC_SPI_CFG_CH0_CD_ENABLE;
        p_hndl->use_cs_pin = SDC_SPI_CFG_CH0_CS_ENABLE;
        p_hndl->use_wp_pin = SDC_SPI_CFG_CH0_WP_ENABLE;
        p_hndl->bitrate = MEMDRV_CFG_DEV0_BR;

    }
    else
    {
        /* Cast according to variable */
        p_hndl->cd_pin = (gpio_port_pin_t)((SDC_SPI_CFG_CH1_CD_PORT << SDC_SPI_BIT_SHIFT_8) | SDC_SPI_CFG_CH1_CD_BIT);
        p_hndl->cs_pin = (gpio_port_pin_t)((SDC_SPI_CFG_CH1_CS_PORT << SDC_SPI_BIT_SHIFT_8) | SDC_SPI_CFG_CH1_CS_BIT);
        p_hndl->wp_pin = (gpio_port_pin_t)((SDC_SPI_CFG_CH1_WP_PORT << SDC_SPI_BIT_SHIFT_8) | SDC_SPI_CFG_CH1_WP_BIT);
        p_hndl->use_cd_pin = SDC_SPI_CFG_CH1_CD_ENABLE;
        p_hndl->use_cs_pin = SDC_SPI_CFG_CH1_CS_ENABLE;
        p_hndl->use_wp_pin = SDC_SPI_CFG_CH1_WP_ENABLE;
        p_hndl->bitrate = MEMDRV_CFG_DEV1_BR;
    }

    /* I/O port initialize */
    if (SDC_SPI_ENABLE == p_hndl->use_cd_pin)
    {
        R_GPIO_PinDirectionSet(p_hndl->cd_pin, GPIO_DIRECTION_INPUT);
    }
    if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
    {
        R_GPIO_PinDirectionSet(p_hndl->cs_pin, GPIO_DIRECTION_INPUT);
    }
    if (SDC_SPI_ENABLE == p_hndl->use_wp_pin)
    {
        R_GPIO_PinDirectionSet(p_hndl->wp_pin, GPIO_DIRECTION_INPUT);
    }

    /* Use MEMDRV FIT number set */
    p_hndl->dev_no = dev_no;

    /* wp initialize */
    p_hndl->wp = SDC_SPI_W_PROTECT_HARD | SDC_SPI_W_PROTECT_SOFT;

    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_open() */

/**********************************************************************************************************************
* Outline      : Get handler address.
* Function Name: r_sdc_spi_handle_get
* Description  : Get handler address.
* Arguments    : uint32_t           card_no             ; SD Card No.
* Return Value : sdc_spi_hndl_t *                       ; handler address
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_hndl_t * r_sdc_spi_handle_get(uint32_t card_no)
{
    return gs_sdc_spi_handle[card_no];
} /* End of function r_sdc_spi_handle_get() */

/**********************************************************************************************************************
* Outline      : Handler address clear.
* Function Name: r_sdc_spi_handle_clear
* Description  : Handler address clear.
* Arguments    : uint32_t           card_no     ; SD Card No.
* Return Value : None                           ; handler address
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
void r_sdc_spi_handle_clear(uint32_t card_no)
{
    gs_sdc_spi_handle[card_no] = NULL;
} /* End of function r_sdc_spi_handle_clear() */

/* End of File */
