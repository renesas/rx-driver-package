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
 * File Name    : r_sdc_spi_write.c
 * Version      : 1.0
 * Description  : Functions for module write.
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
* Outline      : Write SD card.
* Function Name: r_sdc_spi_write
* Description  : Write SD card.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_spi_access_t   * p_sdc_spi_access  ;   SD card access information structure pointer.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_WP                         ;   Write error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_write(uint32_t card_no, sdc_spi_access_t * p_sdc_spi_access)
{
    sdc_spi_hndl_t * p_hndl = 0;
    st_memdrv_info_t  sdc_spi_memdrv_info;
    sdc_spi_status_t  ret;
    memdrv_err_t memdrv_err;

    /* Get handle address */
    p_hndl = r_sdc_spi_handle_get(card_no);

    /* Card type check */
    if (SDC_SPI_CARD_UNDETECT ==  p_hndl->media)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return SDC_SPI_ERR;
    }

    /* Card detect check */
    if (SDC_SPI_ERR == r_sdc_spi_getcarddetection(card_no))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return SDC_SPI_ERR;
    }

    /* Protect status check */
    if ((SDC_SPI_NO_PROTECT != p_hndl->wp) && (SDC_SPI_ENABLE == p_hndl->use_wp_pin))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return SDC_SPI_ERR_WP;
    }

    /* Setting the parameters used in MEMDRV */
    sdc_spi_memdrv_info.cnt = 0;
    sdc_spi_memdrv_info.p_data = NULL;
    sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
    sdc_spi_memdrv_info.read_after_write = true;

    /* Enable MEMDRV corresponding to card_no */
    memdrv_err = R_MEMDRV_Enable(p_hndl->dev_no, &sdc_spi_memdrv_info);
    if (MEMDRV_SUCCESS != memdrv_err)
    {
        /* Disable MEMDRV corresponding to card_no */
        R_MEMDRV_Disable(p_hndl->dev_no, &sdc_spi_memdrv_info);
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return SDC_SPI_ERR_INTERNAL;
    }

    /* CS pin output is low */
    if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
    {
        R_GPIO_PinWrite(p_hndl->cs_pin, GPIO_LEVEL_LOW );
    }

    /* Process single block write or multi block write depending on write count */
    if (p_sdc_spi_access->cnt <= SDC_SPI_CFG_SBLK_NUM)
    {
        /* Single block write */
        ret = r_sdc_spi_write_sb(p_hndl, p_sdc_spi_access);
        if (SDC_SPI_SUCCESS > ret)
        {
            /* Disable MEMDRV corresponding to card_no */
            R_MEMDRV_Disable(p_hndl->dev_no, &sdc_spi_memdrv_info);

            /* CS pin output is high */
            if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
            {
                R_GPIO_PinWrite(p_hndl->cs_pin, GPIO_LEVEL_HIGH);
            }

            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
            return SDC_SPI_ERR;
        }
    }
    else
    {
        /* Multi block write */
        ret = r_sdc_spi_write_mb(p_hndl, p_sdc_spi_access);
        if (SDC_SPI_SUCCESS > ret)
        {
            /* Disable MEMDRV corresponding to card_no */
            R_MEMDRV_Disable(p_hndl->dev_no, &sdc_spi_memdrv_info);

            /* CS pin output is high */
            if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
            {
                R_GPIO_PinWrite(p_hndl->cs_pin, GPIO_LEVEL_HIGH);
            }

            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
            return SDC_SPI_ERR;
        }
    }

    /* Disable MEMDRV corresponding to card_no */
    memdrv_err = R_MEMDRV_Disable(p_hndl->dev_no, &sdc_spi_memdrv_info);
    if (MEMDRV_SUCCESS != memdrv_err)
    {
        /* CS pin output is high */
        if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
        {
            R_GPIO_PinWrite(p_hndl->cs_pin, GPIO_LEVEL_HIGH);
        }

        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return SDC_SPI_ERR_INTERNAL;
    }

    /* CS pin output is high */
    if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
    {
        R_GPIO_PinWrite(p_hndl->cs_pin, GPIO_LEVEL_HIGH);
    }

    return SDC_SPI_SUCCESS;

} /* End of function r_sdc_spi_write() */


/**********************************************************************************************************************
* Outline      : Write single block.
* Function Name: r_sdc_spi_write_sb
* Description  : Write single block routine.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : sdc_spi_access_t   * p_sdc_spi_access  ;   SD card access information structure pointer.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_write_sb(sdc_spi_hndl_t * p_hndl, sdc_spi_access_t * p_sdc_spi_access)
{
    sdc_spi_status_t  ret;
    volatile int32_t blkcnt;
    volatile int32_t blkno;
    volatile uint8_t * p_buff;

    /* Access data setting */
    blkno = p_sdc_spi_access->lbn;
    p_buff = p_sdc_spi_access->p_buff;

    /* Write single block routine */
    /* WAIT_LOOP */
    for (blkcnt = p_sdc_spi_access->cnt; 0 != blkcnt; blkcnt--)
    {
        ret = r_sdc_spi_sbwrite_start(p_hndl, p_hndl->addrunit, blkno);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
            return ret;
        }

        ret = r_sdc_spi_sbwrite_blkdata(p_hndl, (uint8_t *)p_buff);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
            return ret;
        }

        ret = r_sdc_spi_sbwrite_end(p_hndl);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
            return ret;
        }

        /* Next read address */
        blkno++;
        p_buff += SDC_SPI_BLK_SIZE;
    }
    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_write_sb() */

/**********************************************************************************************************************
* Outline      : Write single block start process.
* Function Name: r_sdc_spi_sbwrite_start
* Description  : Write single block start process.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint16_t           addrunit            ;   unit number
*              : uint32_t           blkno               ;   Block number
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_sbwrite_start(sdc_spi_hndl_t * p_hndl, uint16_t addrunit, uint32_t blkno)
{
    uint32_t dataaddr;
    sdc_spi_status_t ret;

    /* Get data address */
    dataaddr = blkno * addrunit;

    /* Send CMD24 command */
    ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_CMD24, dataaddr, SDC_SPI_R1_SIZE, SDC_SPI_OTHER_STAT);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_chk_sts(p_hndl);
        r_sdc_spi_wait_ready(p_hndl);
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return ret;
    }

    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_sbwrite_start() */


/**********************************************************************************************************************
* Outline      : Write single block data.
* Function Name: r_sdc_spi_sbwrite_blkdata
* Description  : Write single block data.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint8_t            * p_data            ;   Write data pointer
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_sbwrite_blkdata(sdc_spi_hndl_t * p_hndl, uint8_t * p_data)
{
    volatile uint8_t txbuf[SDC_SPI_NWR_MIN+SDC_SPI_DTOKEN_SIZE];
    sdc_spi_status_t ret;
    volatile uint32_t work_data_buf;
    uint8_t i;

    /* Setting token data */
    txbuf[0] = SDC_SPI_DUMMY_DATA;
    txbuf[1] = SDC_SPI_DTOKEN_STR_SW;

    /* Send token data */
    for (i = 0; i < (SDC_SPI_NWR_MIN + SDC_SPI_DTOKEN_SIZE); i++)
    {
#ifdef __BIG
        work_data_buf = (uint32_t)(txbuf[i] << SDC_SPI_BIT_SHIFT_24);
#else
        work_data_buf = (uint32_t)txbuf[i];
#endif
        ret = r_sdc_spi_tx_data(p_hndl, 1, (uint8_t *)&work_data_buf);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
            return ret;
        }
    }

    /* Send data and CRC16 */
    ret = r_sdc_spi_write_data_crc(p_hndl, p_data);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return ret;
    }

    /* Check response */
    ret = r_sdc_spi_chk_datares(p_hndl);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return ret;
    }

    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_sbwrite_blkdata() */

/**********************************************************************************************************************
* Outline      : Write single block end process.
* Function Name: r_sdc_spi_sbwrite_end
* Description  : Write single block end process.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_sbwrite_end(sdc_spi_hndl_t * p_hndl)
{
    sdc_spi_status_t ret;

    /* Check status */
    ret = r_sdc_spi_chk_sts(p_hndl);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return ret;
    }

    /* Wait ready */
    r_sdc_spi_wait_ready(p_hndl);

    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_sbwrite_end() */


/**********************************************************************************************************************
* Outline      : Write multi block.
* Function Name: r_sdc_spi_write_mb
* Description  : Write multi block routine.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : sdc_spi_access_t   * p_sdc_spi_access  ;   SD card access information structure pointer.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_write_mb(sdc_spi_hndl_t * p_hndl, sdc_spi_access_t * p_sdc_spi_access)
{
    sdc_spi_status_t  ret;
    volatile int32_t blkcnt;
    volatile int32_t blkno;
    volatile uint8_t * p_buff;

    /* Access data setting */
    blkno = p_sdc_spi_access->lbn;
    p_buff = p_sdc_spi_access->p_buff;

    /* Pre erase */
    if (SDC_SPI_WRITE_WITH_PREERASE == p_sdc_spi_access->write_mode)
    {
        ret = r_sdc_spi_preerase(p_hndl, p_sdc_spi_access->cnt);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
            return ret;
        }
    }

    /* Multi write routine */
    ret = r_sdc_spi_mbwrite_start(p_hndl, (uint16_t)p_hndl->addrunit, blkno);
    if (SDC_SPI_SUCCESS == ret)
    {
        /* WAIT_LOOP */
        for (blkcnt = p_sdc_spi_access->cnt; 0 != blkcnt; blkcnt--)
        {
            ret = r_sdc_spi_mbwrite_blkdata(p_hndl, (uint8_t *)p_buff);
            if (SDC_SPI_SUCCESS > ret)
            {
                /* Send CMD12 command */
                r_sdc_spi_send_cmd12(p_hndl);
                r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
                return ret;
            }

            /* Next read address */
            p_buff += SDC_SPI_BLK_SIZE;
        }

        ret = r_sdc_spi_mbwrite_end(p_hndl);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
            return ret;
        }

        return SDC_SPI_SUCCESS;

    }
    else if (SDC_SPI_ERR_ILLEGAL_COMMAND == ret)
    {
        /* Single block write */
        ret = r_sdc_spi_write_sb(p_hndl, p_sdc_spi_access);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
            return SDC_SPI_ERR;
        }
        return SDC_SPI_SUCCESS;
    }
    else
    {
        /* Do Nothing */
    }

    r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
    return SDC_SPI_ERR;

} /* End of function r_sdc_spi_write_mb() */


/**********************************************************************************************************************
* Outline      : Write multi block start process.
* Function Name: r_sdc_spi_mbwrite_start
* Description  : Write multi block start process.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint16_t           addrunit            ;   unit number
*              : uint32_t           blkno               ;   Block number
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_mbwrite_start(sdc_spi_hndl_t * p_hndl, uint16_t addrunit, uint32_t blkno)
{
    volatile uint32_t dataaddr;
    volatile uint32_t txbuf;
    sdc_spi_status_t ret;

    /* Get data address */
    dataaddr = blkno * addrunit;

    /* Send CMD25 command */
    ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_CMD25, dataaddr, SDC_SPI_R1_SIZE, SDC_SPI_OTHER_STAT);
    if (SDC_SPI_SUCCESS == ret)
    {
        return SDC_SPI_SUCCESS;
    }
    else if (SDC_SPI_ERR_ILLEGAL_COMMAND == ret)
    {
        return SDC_SPI_ERR_ILLEGAL_COMMAND;
    }
    else
    {
        /* Check status */
        r_sdc_spi_chk_sts(p_hndl);
#ifdef __BIG
        txbuf = (uint32_t)(SDC_SPI_DTOKEN_STP_MW << SDC_SPI_BIT_SHIFT_24);
#else
        txbuf = (uint32_t)SDC_SPI_DTOKEN_STP_MW;
#endif
        /* Send token */
        r_sdc_spi_tx_data(p_hndl, SDC_SPI_DTOKEN_SIZE, (uint8_t *)&txbuf);
        r_sdc_spi_wait_ready(p_hndl);

        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return ret;
    }
} /* End of function r_sdc_spi_mbwrite_start() */

/**********************************************************************************************************************
* Outline      : Write multi block data.
* Function Name: r_sdc_spi_mbwrite_blkdata
* Description  : Write multis block data.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint8_t            * p_data            ;   Write data pointer
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_mbwrite_blkdata(sdc_spi_hndl_t * p_hndl, uint8_t * p_data)
{
    volatile uint8_t txbuf[SDC_SPI_NWR_MIN+SDC_SPI_DTOKEN_SIZE];
    sdc_spi_status_t ret;
    volatile uint32_t work_data_buf;
    uint8_t i;

    /* Dummy data send */
    txbuf[0] = SDC_SPI_DUMMY_DATA;
    txbuf[1] = SDC_SPI_DTOKEN_STR_MW;

    /* Send token */
    for (i = 0; i < (SDC_SPI_NWR_MIN + SDC_SPI_DTOKEN_SIZE); i++)
    {
#ifdef __BIG
        work_data_buf = (uint32_t)(txbuf[i] << SDC_SPI_BIT_SHIFT_24);
#else
        work_data_buf = (uint32_t)txbuf[i];
#endif
        ret = r_sdc_spi_tx_data(p_hndl, 1, (uint8_t *)&work_data_buf);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
            return ret;
        }
    }

    /* Send data and crc */
    ret = r_sdc_spi_write_data_crc(p_hndl, p_data);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return ret;
    }

    /* Check data response */
    ret = r_sdc_spi_chk_datares(p_hndl);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return ret;
    }

    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_mbwrite_blkdata() */


/**********************************************************************************************************************
* Outline      : Write multi block end process.
* Function Name: r_sdc_spi_mbwrite_end
* Description  : Write multi block end process.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_mbwrite_end(sdc_spi_hndl_t * p_hndl)
{
    volatile uint8_t txbuf[SDC_SPI_DTOKEN_SIZE+SDC_SPI_NBR_MIN];
    sdc_spi_status_t ret;
    volatile uint32_t work_data_buf;
    uint8_t i;

    /* Send dummy data */
    ret = r_sdc_spi_tx_dummy_data(p_hndl, SDC_SPI_NWR_MIN);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return ret;
    }
    txbuf[0] = SDC_SPI_DUMMY_DATA;
    txbuf[1] = SDC_SPI_DTOKEN_STR_SW;

    /* Send command */
    for (i = 0; i < (SDC_SPI_NWR_MIN+SDC_SPI_DTOKEN_SIZE); i++)
    {
#ifdef __BIG
        work_data_buf = (uint32_t)(txbuf[i] << SDC_SPI_BIT_SHIFT_24);
#else
        work_data_buf = (uint32_t)txbuf[i];
#endif
        ret = r_sdc_spi_tx_data(p_hndl, 1, (uint8_t *)&work_data_buf);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
            return ret;
        }
    }

    /* Long wait */
    ret = r_sdc_spi_long_wait(p_hndl);
    if (SDC_SPI_SUCCESS == ret)
    {
        ret = r_sdc_spi_chk_sts(p_hndl);
        if (SDC_SPI_SUCCESS == ret)
        {
            r_sdc_spi_wait_ready(p_hndl);
            return SDC_SPI_SUCCESS;
        }
    }

    /* Check status */
    r_sdc_spi_chk_sts(p_hndl);

    r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
    return ret;
} /* End of function r_sdc_spi_mbwrite_end() */

/**********************************************************************************************************************
* Outline      : Write data and crc.
* Function Name: r_sdc_spi_write_data_crc
* Description  : Write data after that CRC.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint8_t            * p_data            ;   Write data pointer
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_write_data_crc(sdc_spi_hndl_t * p_hndl, uint8_t * p_data)
{
    volatile exchg_short_t crcbuf;
    volatile uint32_t work_data_buf;
    sdc_spi_status_t  ret;

    /* Send write data, and generate CRC16 data */
    ret = r_sdc_spi_tx_data_crc16(p_hndl, SDC_SPI_BLK_SIZE, p_data, (uint16_t *)&crcbuf.us);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return ret;
    }

    /* Send first CRC data */
#ifdef __BIG
    work_data_buf = (uint32_t)(crcbuf.uc[1] << SDC_SPI_BIT_SHIFT_24);
#else
    work_data_buf = (crcbuf.uc[1] & 0x000000ff);
#endif
    ret = r_sdc_spi_tx_data(p_hndl, (uint16_t)1, (uint8_t *)&work_data_buf);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return ret;
    }

    /* Send second CRC data */
#ifdef __BIG
    work_data_buf = (uint32_t)(crcbuf.uc[0] << SDC_SPI_BIT_SHIFT_24);
#else
    work_data_buf = (crcbuf.uc[0] & 0x000000ff);
#endif
    ret = r_sdc_spi_tx_data(p_hndl, (uint16_t)1, (uint8_t *)&work_data_buf);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return ret;
    }
    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_write_data_crc() */

/**********************************************************************************************************************
* Outline      : Send write data, and generate CRC16 data.
* Function Name: r_sdc_spi_tx_data_crc16
* Description  : Send write data, and generate CRC16 data.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint8_t            txcnt               ;   Send count
*              : uint8_t            * p_data            ;   Write data pointer
*              : uint8_t            * p_crc16           ;   CRC16 data pointer
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_tx_data_crc16(sdc_spi_hndl_t * p_hndl, uint16_t txcnt, uint8_t * p_data, uint16_t * p_crc16)
{
    sdc_spi_status_t ret;
    volatile uint32_t work_data_buf;
    int32_t i;

    /* Get crc16 */
    *p_crc16 = r_sdc_spi_crc16(p_data, txcnt);

    /* Send data */
    for (i = txcnt; i != 0 ; i--)
    {
#ifdef __BIG
        work_data_buf = (uint32_t)((*(p_data + (txcnt - i))) << SDC_SPI_BIT_SHIFT_24);
#else
        work_data_buf = *(p_data + (txcnt - i));
#endif
        ret = r_sdc_spi_tx_data(p_hndl, 1, (uint8_t *)&work_data_buf);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
            return ret;
        }
    }

    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_tx_data_crc16() */


/**********************************************************************************************************************
* Outline      : Long wait.
* Function Name: r_sdc_spi_long_wait
* Description  : Long wait.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_long_wait(sdc_spi_hndl_t * p_hndl)
{
    return r_sdc_spi_wait_ready(p_hndl);
} /* End of function r_sdc_spi_long_wait() */


/**********************************************************************************************************************
* Outline      : Check data response.
* Function Name: r_sdc_spi_chk_datares
* Description  : Check data response.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_chk_datares(sdc_spi_hndl_t * p_hndl)
{
    volatile uint8_t rxbuf;
    volatile uint32_t work_data_buf;
    sdc_spi_status_t ret;

    work_data_buf = 0x00000000;
    rxbuf = 0x00;

    /* Receive response data */
    ret = r_sdc_spi_rx_data(p_hndl, SDC_SPI_DATARES_SIZE, (uint8_t *)&work_data_buf);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return ret;
    }
#ifdef __BIG
        rxbuf = (uint8_t)((work_data_buf & 0xff000000) >> SDC_SPI_BIT_SHIFT_24);
#else
        rxbuf = (uint8_t)(work_data_buf & 0x000000ff);
#endif

    /* Check response */
    rxbuf &= SDC_SPI_DATARES_MASK;
    if ((SDC_SPI_DATARES_OK == rxbuf))
    {
        ret = r_sdc_spi_long_wait(p_hndl);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
            return SDC_SPI_ERR;
        }
        return SDC_SPI_SUCCESS;
    }
    else if (SDC_SPI_DATARES_CRCERR == rxbuf)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return SDC_SPI_ERR_CRC;
    }
    else if (SDC_SPI_DATARES_WERR == rxbuf)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return SDC_SPI_ERR;
    }
    else
    {
        /* Do Nothing */
    }

    return SDC_SPI_ERR;
} /* End of function r_sdc_spi_chk_datares() */

/**********************************************************************************************************************
* Outline      : Pre erase.
* Function Name: r_sdc_spi_preerase
* Description  : Pre erase using ACMD13.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint32_t           blkcnt              ;   block count
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_preerase(sdc_spi_hndl_t * p_hndl, uint32_t blkcnt)
{
    sdc_spi_status_t ret;

    /* Check SD card size and status */
    if ((0x0000 == (p_hndl->media & (SDC_SPI_CARD_SD | SDC_SPI_CARD_SDHC | SDC_SPI_CARD_SDXC))) ||
            (SDC_SPI_CARD_OTP == (p_hndl->media & SDC_SPI_CARD_OTP)))
    {
        return SDC_SPI_SUCCESS;
    }
    ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_CMD55, 0, SDC_SPI_R1_SIZE, SDC_SPI_OTHER_STAT);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return ret;
    }
    ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_ACMD23, (blkcnt & 0x007fffff), SDC_SPI_R1_SIZE, SDC_SPI_OTHER_STAT);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_WRITE, __LINE__);
        return ret;
    }

    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_preerase() */

/* End of File */
