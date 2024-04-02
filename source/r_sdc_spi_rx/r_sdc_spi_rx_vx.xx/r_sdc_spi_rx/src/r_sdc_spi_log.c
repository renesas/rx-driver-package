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
 * File Name    : r_sdc_spi_log.c
 * Version      : 1.0
 * Description  : Functions for module log.
  *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 28.12.2023 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_sdc_spi_private.h"

#if (1 == SDC_SPI_CFG_ERROR_LOG_ACQUISITION)
#include "r_longq_if.h"
#endif

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
#if (1 == SDC_SPI_CFG_ERROR_LOG_ACQUISITION)
static longq_hdl_t gs_sdc_spi_long_que;
#endif

/**********************************************************************************************************************
* Outline      : Set log handle address.
* Function Name: r_sdc_spi_setloghdladdress
* Description  : Set log handle address.
* Arguments    : uint32_t           user_long_que       ;   LONG FIT handle.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_setloghdladdress(uint32_t user_long_que)
{
#if (1 == SDC_SPI_CFG_ERROR_LOG_ACQUISITION)
    gs_sdc_spi_long_que = (longq_hdl_t)user_long_que;
#endif
    return SDC_SPI_SUCCESS;

} /* End of function r_sdc_spi_setloghdladdress() */

/**********************************************************************************************************************
* Outline      : Log get or log set.
* Function Name: r_sdc_spi_log
* Description  : Log get or log set.
* Arguments    : uint32_t           flg                 ;   Error flag.
*              : uint32_t           fid                 ;   File id.
*              : uint32_t           line                ;   Line number
* Return Value : 0                                      ;   Successful operation
*              : 1                                      ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
int32_t r_sdc_spi_log(uint32_t flg, uint32_t fid, uint32_t line)
{
#if (1 == SDC_SPI_CFG_ERROR_LOG_ACQUISITION)
    longq_err_t err = LONGQ_SUCCESS;
    uint32_t ul_tmp = 0;

    /* Put data setting */
    ul_tmp = (ul_tmp | (line << SDC_SPI_BIT_SHIFT_8));
    ul_tmp = (ul_tmp | (fid << SDC_SPI_BIT_SHIFT_21));
    ul_tmp = (ul_tmp | (SDC_SPI_DRIVER_ID << SDC_SPI_BIT_SHIFT_27));
    ul_tmp = (ul_tmp | (flg << SDC_SPI_BIT_SHIFT_31));

    /* Data put */
    err = R_LONGQ_Put(gs_sdc_spi_long_que, ul_tmp);

    /* Error check */
    if (err != LONGQ_SUCCESS)
    {
        return 1;
    }
    else if (0x80000000 == (ul_tmp & 0x80000000))
    {
        return 1;
    }
    else
    {
        /* Do Nothing */
    }
#endif
    return 0;

} /* End of function r_sdc_spi_log() */

/* End of File */
