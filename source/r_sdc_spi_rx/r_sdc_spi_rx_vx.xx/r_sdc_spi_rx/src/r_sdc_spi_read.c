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
 * File Name    : r_sdc_spi_read.c
 * Version      : 1.0
 * Description  : Functions for module read.
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
* Outline      : Read SD card.
* Function Name: r_sdc_spi_read
* Description  : Read SD card.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : sdc_spi_access_t   * p_sdc_spi_access  ;   SD card access information structure pointer.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_read(uint32_t card_no,sdc_spi_access_t * p_sdc_spi_access)
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
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
        return SDC_SPI_ERR;
    }

    /* Card detect check */
    if (SDC_SPI_ERR == r_sdc_spi_getcarddetection(card_no))
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
        return SDC_SPI_ERR;
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
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
        return SDC_SPI_ERR_INTERNAL;
    }

    /* CS pin output is low */
    if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
    {
        R_GPIO_PinWrite (p_hndl->cs_pin, GPIO_LEVEL_LOW );
    }

    /* Process single block read or multi block read depending on write count */
    if (p_sdc_spi_access->cnt <= SDC_SPI_CFG_SBLK_NUM)
    {
        /* Single block read */
        ret = r_sdc_spi_read_sb(p_hndl, p_sdc_spi_access);
        if (SDC_SPI_SUCCESS > ret)
        {
            /* Disable MEMDRV corresponding to card_no */
            R_MEMDRV_Disable(p_hndl->dev_no, &sdc_spi_memdrv_info);

            /* CS pin output is high */
            if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
            {
                R_GPIO_PinWrite(p_hndl->cs_pin, GPIO_LEVEL_HIGH);
            }

            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
            return ret;
        }
    }
    else if ((p_sdc_spi_access->lbn + p_sdc_spi_access->cnt) == (p_hndl->csdinfo.maxblkno+1))
    {
        /* ---- Multi block read after that single black read ---- */

        /* Multi block read */
        p_sdc_spi_access->cnt = p_sdc_spi_access->cnt - 1;
        ret = r_sdc_spi_read_mb(p_hndl, p_sdc_spi_access);
        if (SDC_SPI_SUCCESS > ret)
        {
            /* Disable MEMDRV corresponding to card_no */
            R_MEMDRV_Disable(p_hndl->dev_no, &sdc_spi_memdrv_info);

            /* CS pin output is high */
            if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
            {
                R_GPIO_PinWrite(p_hndl->cs_pin, GPIO_LEVEL_HIGH);
            }

            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
            return ret;
        }
        p_sdc_spi_access -> p_buff = p_sdc_spi_access -> p_buff + ((p_sdc_spi_access->cnt) * SDC_SPI_BLK_SIZE);

        /* Single block read */
        ret = r_sdc_spi_read_sb(p_hndl, p_sdc_spi_access);
        if (SDC_SPI_SUCCESS > ret)
        {
            /* Disable MEMDRV corresponding to card_no */
            R_MEMDRV_Disable(p_hndl->dev_no, &sdc_spi_memdrv_info);

            /* CS pin output is high */
            if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
            {
                R_GPIO_PinWrite(p_hndl->cs_pin, GPIO_LEVEL_HIGH);
            }

            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
            return ret;
        }
    }
    else
    {
        /* Multi block read */
        ret = r_sdc_spi_read_mb(p_hndl, p_sdc_spi_access);
        if (SDC_SPI_SUCCESS > ret)
        {
            /* Disable MEMDRV corresponding to card_no */
            R_MEMDRV_Disable(p_hndl->dev_no, &sdc_spi_memdrv_info);

            /* CS pin output is high */
            if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
            {
                R_GPIO_PinWrite(p_hndl->cs_pin, GPIO_LEVEL_HIGH);
            }

            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
            return ret;
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

        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
        return SDC_SPI_ERR_INTERNAL;
    }

    /* CS pin output is high */
    if (SDC_SPI_ENABLE == p_hndl->use_cs_pin)
    {
        R_GPIO_PinWrite(p_hndl->cs_pin, GPIO_LEVEL_HIGH);
    }

    return SDC_SPI_SUCCESS;

} /* End of function r_sdc_spi_read() */

/**********************************************************************************************************************
* Outline      : Read single block.
* Function Name: r_sdc_spi_read_sb
* Description  : Read single block routine.
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
sdc_spi_status_t r_sdc_spi_read_sb(sdc_spi_hndl_t * p_hndl, sdc_spi_access_t * p_sdc_spi_access)
{
    sdc_spi_status_t  ret;
    volatile int32_t blkcnt;
    volatile int32_t blkno;
    volatile uint8_t * p_buff;

    /* Access data setting */
    blkno = p_sdc_spi_access->lbn;
    p_buff = p_sdc_spi_access->p_buff;

    /* Read single block routine */
    /* WAIT_LOOP */
    for (blkcnt = p_sdc_spi_access->cnt; 0 != blkcnt; blkcnt--)
    {
        /* Single block read start */
        ret = r_sdc_spi_sbread_start(p_hndl, (uint16_t)p_hndl->addrunit, blkno);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
            return ret;
        }

        /* block read */
        ret = r_sdc_spi_read_blkdata(p_hndl, (uint8_t *)p_buff);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
            return ret;
        }

        /* Single block read end */
        ret = r_sdc_spi_sbread_end(p_hndl);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
            return ret;
        }

        /* Next read address */
        blkno++;
        p_buff += SDC_SPI_BLK_SIZE;
    }
    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_read_sb() */

/**********************************************************************************************************************
* Outline      : Read single block start process.
* Function Name: r_sdc_spi_sbread_start
* Description  : Read single block start process using CMD17.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint16_t           addrunit            ;   unit number
*              : uint32_t           blkno               ;   Block number
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Failed operation
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_sbread_start(sdc_spi_hndl_t * p_hndl, uint16_t addrunit, uint32_t blkno)
{
    uint32_t dataaddr;
    sdc_spi_status_t ret;

    /* Get data address */
    dataaddr = blkno * addrunit;

    /* Send CMD17 command */
    ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_CMD17, dataaddr, SDC_SPI_R1_SIZE, SDC_SPI_OTHER_STAT);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_chk_sts(p_hndl);
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
        return ret;
    }

    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_sbread_start() */

/**********************************************************************************************************************
* Outline      : Read single block end process.
* Function Name: r_sdc_spi_sbread_end
* Description  : Read single block end process.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_sbread_end(sdc_spi_hndl_t * p_hndl)
{
    sdc_spi_status_t ret;

    /* Check status */
    ret = r_sdc_spi_chk_sts(p_hndl);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
        return ret;
    }

    /* Wait ready */
    r_sdc_spi_wait_ready(p_hndl);
    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_sbread_end() */


/**********************************************************************************************************************
* Outline      : Read multi block.
* Function Name: r_sdc_spi_read_mb
* Description  : Read multi block routine.
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
sdc_spi_status_t r_sdc_spi_read_mb(sdc_spi_hndl_t * p_hndl, sdc_spi_access_t * p_sdc_spi_access)
{
    sdc_spi_status_t  ret;
    sdc_spi_status_t  ret2 = SDC_SPI_SUCCESS;
    volatile int32_t blkcnt;
    volatile int32_t blkno;
    volatile uint8_t * p_buff;

    blkno = p_sdc_spi_access->lbn;
    p_buff = p_sdc_spi_access->p_buff;

    /* ---- Read multi block routine ---- */

    /* Multi block read start */
    ret = r_sdc_spi_mbread_start(p_hndl, (uint16_t)p_hndl->addrunit, blkno);
    if (SDC_SPI_SUCCESS == ret)
    {
        /* r_sdc_spi_read_blkdata */
        /* WAIT_LOOP */
        for (blkcnt = p_sdc_spi_access->cnt; 0 != blkcnt; blkcnt--)
        {
            ret2 = r_sdc_spi_read_blkdata(p_hndl, (uint8_t *)p_buff);
            if (SDC_SPI_SUCCESS > ret2)
            {
                break;
            }
            /* Next read address */
            p_buff += SDC_SPI_BLK_SIZE;
        }

        /* Multi block read end */
        ret = r_sdc_spi_mbread_end(p_hndl);

        /* Error check */
        if ((SDC_SPI_SUCCESS > ret) || (SDC_SPI_SUCCESS > ret2))
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);

            if (SDC_SPI_SUCCESS > ret2)
            {
                return ret2;
            }
            else
            {
                return ret;
            }
        }
        return SDC_SPI_SUCCESS;
    }
    else if (SDC_SPI_ERR_ILLEGAL_COMMAND == ret)
    {
        /* Single block read */
        ret = r_sdc_spi_read_sb(p_hndl, p_sdc_spi_access);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
            return ret;
        }
        return SDC_SPI_SUCCESS;
    }
    else
    {
        /* Do Nothing */
    }

    r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
    return SDC_SPI_ERR;

} /* End of function r_sdc_spi_read_mb() */

/**********************************************************************************************************************
* Outline      : Read multi block start process.
* Function Name: r_sdc_spi_mbread_start
* Description  : Read multi block start process.
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
sdc_spi_status_t r_sdc_spi_mbread_start(sdc_spi_hndl_t * p_hndl, uint16_t addrunit, uint32_t blkno)
{
    uint32_t dataaddr;
    sdc_spi_status_t ret;

    /* Get data address */
    dataaddr = blkno*addrunit;

    /* Send CMD18 command */
    ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_CMD18, dataaddr, SDC_SPI_R1_SIZE, SDC_SPI_OTHER_STAT);
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
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
        return ret;
    }
} /* End of function r_sdc_spi_mbread_start() */


/**********************************************************************************************************************
* Outline      : Read multi block end process.
* Function Name: r_sdc_spi_mbread_end
* Description  : Read multi block end process.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_mbread_end(sdc_spi_hndl_t * p_hndl)
{
    sdc_spi_status_t ret;

    /* Read block token */
    ret = r_sdc_spi_read_startblocktoken(p_hndl);
    if (SDC_SPI_SUCCESS > ret)
    {
        /* Send CMD12 command */
        r_sdc_spi_send_cmd12(p_hndl);
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
        return ret;
    }

    /* Send CMD12 command */
    ret = r_sdc_spi_send_cmd12(p_hndl);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
        return ret;
    }

    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_mbread_end() */


/**********************************************************************************************************************
* Outline      : Read block data.
* Function Name: r_sdc_spi_read_blkdata
* Description  : Read block data.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint8_t            * p_data            ;   Read data pointer
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_read_blkdata(sdc_spi_hndl_t * p_hndl, uint8_t * p_data)
{
    sdc_spi_status_t ret;

    /* Read block token */
    ret = r_sdc_spi_read_startblocktoken(p_hndl);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
        return ret;
    }

    /* Read block data and check CRC */
    ret = r_sdc_spi_read_data_crc(p_hndl, SDC_SPI_BLK_SIZE, p_data);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
        return ret;
    }
    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_read_blkdata() */


/**********************************************************************************************************************
* Outline      : Read start block token.
* Function Name: r_sdc_spi_read_startblocktoken
* Description  : Read start block token.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Failed operation
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_read_startblocktoken(sdc_spi_hndl_t * p_hndl)
{
    volatile uint8_t rxbuf;
    volatile uint32_t rxwait;
    volatile uint32_t work_data_buf;
    sdc_spi_status_t ret;

    /* Send dummy data */
    r_sdc_spi_tx_dummy_data(p_hndl, SDC_SPI_NAC_MIN);

    /* WAIT_LOOP */
    for (rxwait = p_hndl->csdinfo.nac; rxwait >= 8; rxwait -= 8)
    {
        /* Read response */
        ret = r_sdc_spi_rx_data(p_hndl, SDC_SPI_DATARES_SIZE, (uint8_t *)&work_data_buf);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
            return ret;
        }
#ifdef __BIG
        rxbuf = (uint8_t)((work_data_buf & 0xff000000) >> SDC_SPI_BIT_SHIFT_24);
#else
        rxbuf = (uint8_t)(work_data_buf & 0x000000ff);
#endif

        /* Start block token check */
        if (SDC_SPI_DTOKEN_STR_MR == rxbuf)
        {
            return SDC_SPI_SUCCESS;
        }

        /* Error check */
        if (0xff != rxbuf)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
            return SDC_SPI_ERR;
        }
    }

    r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_READ, __LINE__);
    return SDC_SPI_ERR;
} /* End of function r_sdc_spi_read_startblocktoken() */

/* End of File */
