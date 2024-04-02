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
 * File Name    : r_sdc_spi_rx.c
 * Version      : 1.0
 * Description  : Functions for module interface.
  *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 28.12.2023 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_sdc_spi_rx_if.h"
#include "r_sdc_spi_private.h"

/**********************************************************************************************************************
* Outline      : Initialize SPI mode SD card driver
* Function Name: R_SDC_SPI_Open
* Description  : Initializes the SPI mode SD card driver work memory specified by channel.
* Arguments    : uint32_t           card_no              ;   SD Card No.
*              : uint32_t           dev_no               ;   MEMDRV Channel No.
*              : void               * p_sdc_spi_workarea ;   SPI mode SDC driver work memory
* Return Value : SDC_SPI_SUCCESS                         ;   Successful operation
*              : SDC_SPI_ERR                             ;   Parameter error
*              : SDC_SPI_ERR_ADDRESS_BOUNDARY            ;   There is a problem with the address
*              : SDC_SPI_ERR_INTERNAL                    ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t R_SDC_SPI_Open(uint32_t card_no, uint32_t dev_no, void * p_sdc_spi_workarea)
{
    sdc_spi_status_t  ret;

#if (SDC_SPI_CFG_PARAM_CHECKING_ENABLE)

    /*  Check the card no  */
    if (SDC_SPI_CARD_NUM <= card_no)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

    /* Check NULL address. */
    if (NULL == (uint32_t)p_sdc_spi_workarea)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR_ADDRESS_BOUNDARY;
    }

    /*  Check the work memory boundary (4-byte unit)  */
    if (0x00000000 != (((uint32_t)p_sdc_spi_workarea) & 0x00000003))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR_ADDRESS_BOUNDARY;
    }

    /* Check driver is opened */
    if (NULL != r_sdc_spi_handle_get(card_no))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

#endif /* SDC_SPI_CFG_PARAM_CHECKING_ENABLE */

    ret = r_sdc_spi_open(card_no, dev_no, p_sdc_spi_workarea);

    return ret;

} /* End of function R_SDC_SPI_Open() */

/**********************************************************************************************************************
* Outline      : Close SPI mode SD card driver
* Function Name: R_SDC_SPI_Close
* Description  : Close the SD card Driver and work memory initialize by channel.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Parameter error
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t R_SDC_SPI_Close(uint32_t card_no)
{
    sdc_spi_status_t  ret;

#if (SDC_SPI_CFG_PARAM_CHECKING_ENABLE)

    /*  Check the card_no and handle */
    if (SDC_SPI_ERR == r_sdc_spi_check_cardno_and_hndl(card_no))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

#endif /* SDC_SPI_CFG_PARAM_CHECKING_ENABLE */

    ret = r_sdc_spi_close(card_no);

    return ret;
} /* End of function R_SDC_SPI_Close() */

/**********************************************************************************************************************
* Outline      : SD card get detect status
* Function Name: R_SDC_SPI_GetCardDetection
* Description  : SD card get detect status.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SPI_SUCCESS                        ;   Port level of CD pin is low or CD pin is disable.
*              : SDC_SPI_ERR                            ;   Parameter error or port level of CD pin is high.
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t R_SDC_SPI_GetCardDetection(uint32_t card_no)
{
    sdc_spi_status_t  ret;

#if (SDC_SPI_CFG_PARAM_CHECKING_ENABLE)

    /*  Check the card_no and handle */
    if (SDC_SPI_ERR == r_sdc_spi_check_cardno_and_hndl(card_no))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

#endif /* SDC_SPI_CFG_PARAM_CHECKING_ENABLE */

    ret = r_sdc_spi_getcarddetection(card_no);

    return ret;
} /* End of function R_SDC_SPI_GetCardDetection() */


/**********************************************************************************************************************
* Outline      : Initialize SD Card
* Function Name: R_SDC_SPI_Initialize
* Description  : Initializes the SD card by channel.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_spi_cfg_t      *p_sdc_spi_config   ;   SPI mode SDC driver operation configuration.
*              : uint32_t           init_type           ;   SPI mode SDC driver init type.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ADDRESS_BOUNDARY           ;   There is a problem with the address
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------.
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t R_SDC_SPI_Initialize(uint32_t card_no, sdc_spi_cfg_t * p_sdc_spi_config, uint32_t init_type)
{
    sdc_spi_status_t  ret;
    sdc_spi_hndl_t * p_hndl = 0;

    /* Get handle address */
    p_hndl = r_sdc_spi_handle_get(card_no);

#if (SDC_SPI_CFG_PARAM_CHECKING_ENABLE)

    /*  Check the card_no and handle */
    if (SDC_SPI_ERR == r_sdc_spi_check_cardno_and_hndl(card_no))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

    /* Check NULL address. */
    if (NULL == (uint32_t)p_sdc_spi_config)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR_ADDRESS_BOUNDARY;
    }

    /* init_type check. */
    if (0 != init_type)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }
#endif /* SDC_SPI_CFG_PARAM_CHECKING_ENABLE */

    /* Check the initialize */
    if (SDC_SPI_FALSE != p_hndl->init_flag)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

    ret = r_sdc_spi_initialize(card_no, p_sdc_spi_config, init_type);

    return ret;
} /* End of function R_SDC_SPI_Initialize() */

/**********************************************************************************************************************
* Outline      : Access end SD card
* Function Name: R_SDC_SPI_End
* Description  : Access end SD card.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Parameter error
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t R_SDC_SPI_End(uint32_t card_no)
{
    sdc_spi_status_t  ret;

#if (SDC_SPI_CFG_PARAM_CHECKING_ENABLE)

    /*  Check the card_no and handle */
    if (SDC_SPI_ERR == r_sdc_spi_check_cardno_and_hndl(card_no))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

#endif /* SDC_SPI_CFG_PARAM_CHECKING_ENABLE */

    ret = r_sdc_spi_end(card_no);

    return ret;
} /* End of function R_SDC_SPI_End() */


/**********************************************************************************************************************
* Outline      : Read SD card.
* Function Name: R_SDC_SPI_Read
* Description  : Read SD card data.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : void               * p_sdc_spi_access  ;   Read access information
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t R_SDC_SPI_Read(uint32_t card_no, sdc_spi_access_t * p_sdc_spi_access)
{
    sdc_spi_status_t  ret;
    sdc_spi_hndl_t * p_hndl = 0;

    /* Get handle address */
    p_hndl = r_sdc_spi_handle_get(card_no);

#if (SDC_SPI_CFG_PARAM_CHECKING_ENABLE)

    /* Check the card_no and handle */
    if (SDC_SPI_ERR == r_sdc_spi_check_cardno_and_hndl(card_no))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

    /* Check NULL address */
    if (NULL == (uint32_t)p_sdc_spi_access)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

    /* Check access block */
    if ((0 == p_sdc_spi_access->cnt) || ( p_sdc_spi_access->cnt > (p_hndl->csdinfo.maxblkno + 1) ) ||
            (p_sdc_spi_access->lbn > ((p_hndl->csdinfo.maxblkno + 1) - p_sdc_spi_access -> cnt)) )
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }
#endif /* SDC_SPI_CFG_PARAM_CHECKING_ENABLE */

    /* Check the initialize */
    if (SDC_SPI_TRUE != p_hndl->init_flag)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

    ret = r_sdc_spi_read(card_no, p_sdc_spi_access);
    return ret;
} /* End of function R_SDC_SPI_Read() */


/**********************************************************************************************************************
* Outline      : Write SD card.
* Function Name: R_SDC_SPI_Write
* Description  : Write data SD card.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : void               * p_sdc_spi_access  ;   Write access information
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t R_SDC_SPI_Write(uint32_t card_no, sdc_spi_access_t * p_sdc_spi_access)
{
    sdc_spi_status_t  ret;
    sdc_spi_hndl_t * p_hndl = 0;

    /* Get handle address */
    p_hndl = r_sdc_spi_handle_get(card_no);

#if (SDC_SPI_CFG_PARAM_CHECKING_ENABLE)

    /*  Check the card_no and handle */
    if (SDC_SPI_ERR == r_sdc_spi_check_cardno_and_hndl(card_no))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

    /* Check NULL address */
    if (NULL == (uint32_t)p_sdc_spi_access)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

    /* Check write mode */
    if ((SDC_SPI_WRITE_WITH_PREERASE != p_sdc_spi_access->write_mode) &&
            ( SDC_SPI_WRITE_OVERWRITE != p_sdc_spi_access->write_mode))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

    /* Check access block */
    if ((0 == p_sdc_spi_access->cnt) || ( p_sdc_spi_access->cnt > (p_hndl->csdinfo.maxblkno + 1) ) ||
            (p_sdc_spi_access->lbn > ((p_hndl->csdinfo.maxblkno + 1) - p_sdc_spi_access -> cnt)) )
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }
#endif /* SDC_SPI_CFG_PARAM_CHECKING_ENABLE */

    /* Check the initialize */
    if (SDC_SPI_TRUE != p_hndl->init_flag)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

    ret = r_sdc_spi_write(card_no, p_sdc_spi_access);

    return ret;
} /* End of function R_SDC_SPI_Write() */

/**********************************************************************************************************************
* Outline      : Get card status.
* Function Name: R_SDC_SPI_GetCardStatus
* Description  : Get card status from selected SD Card No.
* Arguments    : uint32_t           card_no                    ;   SD Card No.
*              : sdc_spi_card_status_t * p_sdc_spi_card_status ;   Card status information struct pointer.
* Return Value : SDC_SPI_SUCCESS                               ;   Successful operation
*              : SDC_SPI_ERR                                   ;   Parameter error
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t R_SDC_SPI_GetCardStatus(uint32_t card_no, sdc_spi_card_status_t * p_sdc_spi_card_status)
{
    sdc_spi_status_t  ret;
    sdc_spi_hndl_t * p_hndl = 0;

    /* Get handle address */
    p_hndl = r_sdc_spi_handle_get(card_no);

#if (SDC_SPI_CFG_PARAM_CHECKING_ENABLE)

    /*  Check the card_no and handle */
    if (SDC_SPI_ERR == r_sdc_spi_check_cardno_and_hndl(card_no))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

    /* Check NULL address */
    if (NULL == (uint32_t)p_sdc_spi_card_status)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }
#endif /* SDC_SPI_CFG_PARAM_CHECKING_ENABLE */

    /* Check the initialize */
    if (SDC_SPI_TRUE != p_hndl->init_flag)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

    ret = r_sdc_spi_getcardstatus(card_no, p_sdc_spi_card_status);

    return ret;
} /* End of function R_SDC_SPI_GetCardStatus() */

/**********************************************************************************************************************
* Outline      : Get card information.
* Function Name: R_SDC_SPI_GetCardInfo
* Description  : Get card information from selected SD Card No.
* Arguments    : uint32_t            card_no             ;   SD Card No.
*              : sdc_spi_card_info_t *p_sdc_spi_card_reg ;   Card register information struct pointer.
* Return Value : SDC_SPI_SUCCESS                         ;   Successful operation
*              : SDC_SPI_ERR                             ;   Parameter error
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t R_SDC_SPI_GetCardInfo(uint32_t card_no, sdc_spi_card_reg_t * p_sdc_spi_card_reg)
{
    sdc_spi_status_t  ret;
    sdc_spi_hndl_t * p_hndl = 0;

    /* Get handle address */
    p_hndl = r_sdc_spi_handle_get(card_no);

#if (SDC_SPI_CFG_PARAM_CHECKING_ENABLE)

    /*  Check the card_no and handle */
    if (SDC_SPI_ERR == r_sdc_spi_check_cardno_and_hndl(card_no))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

    /* Check NULL address */
    if (NULL == (uint32_t)p_sdc_spi_card_reg)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }
#endif /* SDC_SPI_CFG_PARAM_CHECKING_ENABLE */

    /* Check the initialize */
    if (SDC_SPI_TRUE != p_hndl->init_flag)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

    ret = r_sdc_spi_getcardinfo(card_no, p_sdc_spi_card_reg);

    return ret;
} /* End of function R_SDC_SPI_GetCardInfo() */

/**********************************************************************************************************************
* Outline      : Set log handle address
* Function Name: R_SDC_SPI_SetLogHdlAddress
* Description  : Set longq FIT handle address.
* Arguments    : uint32_t           user_long_que       ;   Longq handle address.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Not use log function
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t R_SDC_SPI_SetLogHdlAddress(uint32_t user_long_que)
{
#if (1 == SDC_SPI_CFG_ERROR_LOG_ACQUISITION)
    sdc_spi_status_t  ret;

    ret = r_sdc_spi_setloghdladdress(user_long_que);
    return ret;
#else
    return SDC_SPI_ERR;
#endif
} /* End of function R_SDC_SPI_SetLogHdlAddress() */

/**********************************************************************************************************************
* Outline      : Get log operation
* Function Name: R_SDC_SPI_Log
* Description  : Get log operation.
* Arguments    : uint32_t           flg                 ;   flag(const value)
*              : uint32_t           fid                 ;   file id(const value)
*              : uint32_t           line                ;   line number(const value)
* Return Value : -1                                     ;   Not use log function
*              : 0                                      ;   Log get complete
*              : 1                                      ;   Log get error
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
int32_t R_SDC_SPI_Log(uint32_t flg, uint32_t fid, uint32_t line)
{
#if (1 == SDC_SPI_CFG_ERROR_LOG_ACQUISITION)
    int32_t  ret;

    ret = r_sdc_spi_log(flg, fid, line);
    return ret;
#else
    return (-1);
#endif
} /* End of function R_SDC_SPI_Log() */


/**********************************************************************************************************************
* Outline      : Get Version
* Function Name: R_SDC_SPI_Getversion
* Description  : Returns the version of this module. The version number is
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t R_SDC_SPI_Getversion(void)
{
    /* Return module version */
    return (uint32_t)((SDC_SPI_VERSION_MAJOR << 16) | SDC_SPI_VERSION_MINOR);

} /* End of function R_SDC_SPI_Getversion() */


/**********************************************************************************************************************
* Outline      : Check card_no and handle.
* Function Name: r_sdc_spi_check_cardno_and_hndl
* Description  : Check card_no and handle.
* Arguments    : uint32_t           card_no             ;   SD Card No.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Parameter error
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
R_BSP_PRAGMA_INLINE (r_sdc_spi_check_cardno_and_hndl)
sdc_spi_status_t r_sdc_spi_check_cardno_and_hndl(uint32_t card_no)
{

    /*  Check the card no  */
    if (SDC_SPI_CARD_NUM <= card_no)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

    /* Check driver is opened */
    if (NULL == r_sdc_spi_handle_get(card_no))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_RX, __LINE__);
        return SDC_SPI_ERR;
    }

    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_check_cardno_and_hndl() */

/* End of File */
