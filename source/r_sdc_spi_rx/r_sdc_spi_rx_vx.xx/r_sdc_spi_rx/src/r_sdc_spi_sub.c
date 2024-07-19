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
 * File Name    : r_sdc_spi_sub.c
 * Version      : 1.0
 * Description  : Functions for module sub routine.
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
#if (1 == SDC_SPI_CFG_USE_SC_CRC)
#include "r_smc_entry.h"
#endif

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/
volatile exchg_short_t g_sdc_spi_crc16;
volatile uint8_t g_sdc_spi_resbuf[SDC_SPI_MAX_RES_SIZE];

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/

/* CRC7 calculation table */
static const uint8_t        gs_crc7_table[] = {
/*""VAL DEF END""*/
/*  0x*0  0x*1  0x*2  0x*3  0x*4  0x*5  0x*6  0x*7  0x*8  0x*9  0x*A  0x*B  0x*C  0x*D  0x*E  0x*F          */
    0x00, 0x12, 0x24, 0x36, 0x48, 0x5A, 0x6C, 0x7E, 0x90, 0x82, 0xB4, 0xA6, 0xD8, 0xCA, 0xFC, 0xEE, /* 0x0* */
    0x32, 0x20, 0x16, 0x04, 0x7A, 0x68, 0x5E, 0x4C, 0xA2, 0xB0, 0x86, 0x94, 0xEA, 0xF8, 0xCE, 0xDC, /* 0x1* */
    0x64, 0x76, 0x40, 0x52, 0x2C, 0x3E, 0x08, 0x1A, 0xF4, 0xE6, 0xD0, 0xC2, 0xBC, 0xAE, 0x98, 0x8A, /* 0x2* */
    0x56, 0x44, 0x72, 0x60, 0x1E, 0x0C, 0x3A, 0x28, 0xC6, 0xD4, 0xE2, 0xF0, 0x8E, 0x9C, 0xAA, 0xB8, /* 0x3* */
    0xC8, 0xDA, 0xEC, 0xFE, 0x80, 0x92, 0xA4, 0xB6, 0x58, 0x4A, 0x7C, 0x6E, 0x10, 0x02, 0x34, 0x26, /* 0x4* */
    0xFA, 0xE8, 0xDE, 0xCC, 0xB2, 0xA0, 0x96, 0x84, 0x6A, 0x78, 0x4E, 0x5C, 0x22, 0x30, 0x06, 0x14, /* 0x5* */
    0xAC, 0xBE, 0x88, 0x9A, 0xE4, 0xF6, 0xC0, 0xD2, 0x3C, 0x2E, 0x18, 0x0A, 0x74, 0x66, 0x50, 0x42, /* 0x6* */
    0x9E, 0x8C, 0xBA, 0xA8, 0xD6, 0xC4, 0xF2, 0xE0, 0x0E, 0x1C, 0x2A, 0x38, 0x46, 0x54, 0x62, 0x70, /* 0x7* */
    0x82, 0x90, 0xA6, 0xB4, 0xCA, 0xD8, 0xEE, 0xFC, 0x12, 0x00, 0x36, 0x24, 0x5A, 0x48, 0x7E, 0x6C, /* 0x8* */
    0xB0, 0xA2, 0x94, 0x86, 0xF8, 0xEA, 0xDC, 0xCE, 0x20, 0x32, 0x04, 0x16, 0x68, 0x7A, 0x4C, 0x5E, /* 0x9* */
    0xE6, 0xF4, 0xC2, 0xD0, 0xAE, 0xBC, 0x8A, 0x98, 0x76, 0x64, 0x52, 0x40, 0x3E, 0x2C, 0x1A, 0x08, /* 0xA* */
    0xD4, 0xC6, 0xF0, 0xE2, 0x9C, 0x8E, 0xB8, 0xAA, 0x44, 0x56, 0x60, 0x72, 0x0C, 0x1E, 0x28, 0x3A, /* 0xB* */
    0x4A, 0x58, 0x6E, 0x7C, 0x02, 0x10, 0x26, 0x34, 0xDA, 0xC8, 0xFE, 0xEC, 0x92, 0x80, 0xB6, 0xA4, /* 0xC* */
    0x78, 0x6A, 0x5C, 0x4E, 0x30, 0x22, 0x14, 0x06, 0xE8, 0xFA, 0xCC, 0xDE, 0xA0, 0xB2, 0x84, 0x96, /* 0xD* */
    0x2E, 0x3C, 0x0A, 0x18, 0x66, 0x74, 0x42, 0x50, 0xBE, 0xAC, 0x9A, 0x88, 0xF6, 0xE4, 0xD2, 0xC0, /* 0xE* */
    0x1C, 0x0E, 0x38, 0x2A, 0x54, 0x46, 0x70, 0x62, 0x8C, 0x9E, 0xA8, 0xBA, 0xC4, 0xD6, 0xE0, 0xF2  /* 0xF* */
};

#if (0 == SDC_SPI_CFG_USE_SC_CRC)
/* CRC-CCITT calculation table */
static const uint16_t      gs_crc_ccitt_table[] = {
/*""VAL DEF END""*/
/*   0x*0    0x*1    0x*2    0x*3    0x*4    0x*5    0x*6    0x*7    0x*8    0x*9    0x*A    0x*B    0x*C    0x*D    0x*E    0x*F           */
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7, 0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF, /* 0x0* */
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6, 0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE, /* 0x1* */
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485, 0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D, /* 0x2* */
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4, 0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC, /* 0x3* */
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823, 0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B, /* 0x4* */
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12, 0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A, /* 0x5* */
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41, 0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49, /* 0x6* */
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70, 0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78, /* 0x7* */
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F, 0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067, /* 0x8* */
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E, 0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256, /* 0x9* */
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D, 0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405, /* 0xA* */
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C, 0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634, /* 0xB* */
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB, 0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3, /* 0xC* */
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A, 0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92, /* 0xD* */
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9, 0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1, /* 0xE* */
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8, 0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0, /* 0xF* */
};
#endif

/**********************************************************************************************************************
* Outline      : send command
* Function Name: r_sdc_spi_send_cmd
* Description  : send command.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint8_t            cmd                 ;   Command.
*              : uint32_t           arg                 ;   Command argument.
*              : uint8_t            ressize             ;   Response byte size.
*              : uint8_t            stat                ;   current status.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_SUCCESS_IDLE                   ;   Successful operation idle.
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_send_cmd(sdc_spi_hndl_t * p_hndl, uint8_t cmd, uint32_t arg, uint8_t ressize, uint8_t stat)
{
    volatile uint8_t sdc_spi_cmdbuf[SDC_SPI_CMD_SIZE+SDC_SPI_NCR_MIN];
    volatile uint8_t i;
    volatile uint8_t * p_arg;
    volatile uint32_t work_data_buf;
    sdc_spi_status_t ret;

    /* ---- send command setting ---- */
    sdc_spi_cmdbuf[SDC_SPI_CMD_CMD_IDX] = cmd;
    p_arg = (uint8_t *)&arg;                    /* Cast to send with SPI  */
    arg = r_sdc_spi_exchglong(arg);             /* Exchange endian */
    sdc_spi_cmdbuf[SDC_SPI_CMD_ARG_IDX+0] = *(p_arg + 0);
    sdc_spi_cmdbuf[SDC_SPI_CMD_ARG_IDX+1] = *(p_arg + 1);
    sdc_spi_cmdbuf[SDC_SPI_CMD_ARG_IDX+2] = *(p_arg + 2);
    sdc_spi_cmdbuf[SDC_SPI_CMD_ARG_IDX+3] = *(p_arg + 3);

    /* CRC7 calculation */
    sdc_spi_cmdbuf[SDC_SPI_CMD_CRC_IDX] =
            r_sdc_spi_crc7((uint8_t *)sdc_spi_cmdbuf, (SDC_SPI_CMD_SIZE-1)) | 0x01;

    /* Dummy data set */
    sdc_spi_cmdbuf[SDC_SPI_CMD_NCR_IDX] = SDC_SPI_DUMMY_DATA;

    /* If send command is CMD12 then send dummy data */
    if (SDC_SPI_CMD12 != sdc_spi_cmdbuf[SDC_SPI_CMD_CMD_IDX])
    {
        r_sdc_spi_tx_dummy_data(p_hndl, SDC_SPI_NRC_MIN);
    }

    /* Send command */
    for (i = 0; i < (SDC_SPI_CMD_SIZE+SDC_SPI_NCR_MIN); i++)
    {
#ifdef __BIG
        work_data_buf = (uint32_t)(sdc_spi_cmdbuf[i] << SDC_SPI_BIT_SHIFT_24);
#else
        work_data_buf = (uint32_t)sdc_spi_cmdbuf[i];
#endif
        ret = r_sdc_spi_tx_data(p_hndl, 1, (uint8_t *)&work_data_buf);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return ret;
        }
    }

    /* Wait response receive */
    R_BSP_SoftwareDelay(1, BSP_DELAY_MICROSECS);

    /* Buffer clear */
    g_sdc_spi_resbuf[0] = 0xff;

    /* Response check */
    /* WAIT_LOOP */
    for (i = SDC_SPI_NCR_MAX; i != 0; i--)
    {
        /* Receive response */
        ret = r_sdc_spi_rx_data(p_hndl, (uint16_t)1, (uint8_t *)&work_data_buf);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return ret;
        }
#ifdef __BIG
        g_sdc_spi_resbuf[0] = (uint8_t)((work_data_buf & 0xff000000) >> SDC_SPI_BIT_SHIFT_24);
#else
        g_sdc_spi_resbuf[0] = (uint8_t)(work_data_buf & 0x000000ff);
#endif

        /* Check response */
        if ((0x00 == (g_sdc_spi_resbuf[0] & 0x80U)))
        {
            break;
        }
    }

    if (0 == i)
    {
        /* wait NRC size */
        r_sdc_spi_tx_dummy_data(p_hndl, SDC_SPI_NRC_MIN);
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
        return SDC_SPI_ERR;
    }

    /* If response size over 1 then receive additional response  */
    if (1 < ressize)
    {
        /* Response receive */
        for (i = 0; i < (ressize - 1); i++)
        {
            /* Response receive */
            ret = r_sdc_spi_rx_data(p_hndl, (uint16_t)1, (uint8_t *)&work_data_buf);
            if (SDC_SPI_SUCCESS > ret)
            {
                /* wait NRC size */
                r_sdc_spi_tx_dummy_data(p_hndl, SDC_SPI_NRC_MIN);
                r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
                return ret;
            }
#ifdef __BIG
            g_sdc_spi_resbuf[1 + i] = (uint8_t)((work_data_buf & 0xff000000) >> SDC_SPI_BIT_SHIFT_24);
#else
            g_sdc_spi_resbuf[1 + i] = (uint8_t)(work_data_buf & 0x000000ff);
#endif
        }
    }

    /* Response check */
    if (0x00 == g_sdc_spi_resbuf[SDC_SPI_RES_R1IDX])
    {
        /* Response is success */
        if (SDC_SPI_OTHER_STAT == (stat & SDC_SPI_OTHER_STAT))
        {
            return SDC_SPI_SUCCESS;
        }

    }
    else if (0x01 == g_sdc_spi_resbuf[SDC_SPI_RES_R1IDX])
    {
        /* Response is success and idle state */
        if (SDC_SPI_IDLE_STAT == (stat & SDC_SPI_IDLE_STAT))
        {
            return SDC_SPI_SUCCESS_IDLE;
        }
    }
    else if (0x08 == (g_sdc_spi_resbuf[SDC_SPI_RES_R1IDX] & 0x08))
    {
        /* Response is CRC error */
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
        return SDC_SPI_ERR_CRC;
    }
    else if (0x04 == (g_sdc_spi_resbuf[SDC_SPI_RES_R1IDX] & 0x04))
    {
        /* Response is illegal command error */
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
        return SDC_SPI_ERR_ILLEGAL_COMMAND;
    }
    else
    {
        /* Do Nothing */
    }

    /* Other error */
    r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
    return SDC_SPI_ERR;

} /* End of function r_sdc_spi_send_cmd() */

/**********************************************************************************************************************
* Outline      : Send dummy data
* Function Name: r_sdc_spi_tx_dummy_data
* Description  : Send dummy data
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint32_t           txcnt               ;   Send dummy data count.
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_tx_dummy_data(sdc_spi_hndl_t * p_hndl, uint16_t txcnt)
{
    sdc_spi_status_t ret;
    uint32_t data;
#ifdef __BIG
    data = (uint32_t)(SDC_SPI_DUMMY_DATA << SDC_SPI_BIT_SHIFT_24);
#else
    data = (uint32_t)SDC_SPI_DUMMY_DATA;
#endif

    /* Send dummy data */
    /* WAIT_LOOP */
    while(txcnt != 0)
    {
        ret = r_sdc_spi_tx_data(p_hndl, 1, (uint8_t *)&data);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return ret;
        }
        txcnt--;
    }

    return SDC_SPI_SUCCESS;

} /* End of function r_sdc_spi_tx_dummy_data() */

/**********************************************************************************************************************
* Outline      : Data send using MEMDRV FIT
* Function Name: r_sdc_spi_tx_data
* Description  : Data send using MEMDRV FIT
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint32_t           txcnt               ;   Send dummy data count.
*              : uint8_t            * p_data            ;   Send data pointer
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_tx_data(sdc_spi_hndl_t * p_hndl, uint16_t txcnt, uint8_t * p_data)
{
    memdrv_err_t memdrv_err;
    st_memdrv_info_t  sdc_spi_memdrv_info;

    /* **** Send data **** */
    if (0 == p_hndl->dev_no)
    {
#if (MEMDRV_TRNS_CPU == MEMDRV_CFG_DEV0_MODE_TRNS)
        /* Setting the parameters used in MEMDRV */
        sdc_spi_memdrv_info.cnt = txcnt;
        sdc_spi_memdrv_info.p_data = p_data;
        sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        sdc_spi_memdrv_info.read_after_write = true;

        memdrv_err = R_MEMDRV_Tx(p_hndl->dev_no, &sdc_spi_memdrv_info);
        if (memdrv_err != MEMDRV_SUCCESS)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return SDC_SPI_ERR_INTERNAL;
        }

#else
        /* Setting the parameters used in MEMDRV */
        sdc_spi_memdrv_info.cnt = 0;
        sdc_spi_memdrv_info.p_data = NULL;
        sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        sdc_spi_memdrv_info.read_after_write = true;

        memdrv_err = R_MEMDRV_EnableTxData(p_hndl->dev_no, &sdc_spi_memdrv_info);
        if (memdrv_err != MEMDRV_SUCCESS)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return SDC_SPI_ERR_INTERNAL;
        }

        /* Setting the parameters used in MEMDRV */
        sdc_spi_memdrv_info.cnt = txcnt;
        sdc_spi_memdrv_info.p_data = p_data;
        sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        sdc_spi_memdrv_info.read_after_write = true;

        memdrv_err = R_MEMDRV_TxData(p_hndl->dev_no, &sdc_spi_memdrv_info);
        if (memdrv_err != MEMDRV_SUCCESS)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return SDC_SPI_ERR_INTERNAL;
        }

        /* Setting the parameters used in MEMDRV */
        sdc_spi_memdrv_info.cnt = 0;
        sdc_spi_memdrv_info.p_data = NULL;
        sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        sdc_spi_memdrv_info.read_after_write = true;

        memdrv_err = R_MEMDRV_DisableTxData(p_hndl->dev_no, &sdc_spi_memdrv_info);
        if (memdrv_err != MEMDRV_SUCCESS)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return SDC_SPI_ERR_INTERNAL;
        }

#endif
    }
    else
    {
#if (MEMDRV_TRNS_CPU == MEMDRV_CFG_DEV1_MODE_TRNS)
        /* Setting the parameters used in MEMDRV */
        sdc_spi_memdrv_info.cnt = txcnt;
        sdc_spi_memdrv_info.p_data = p_data;
        sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        sdc_spi_memdrv_info.read_after_write = true;

        memdrv_err = R_MEMDRV_Tx(p_hndl->dev_no, &sdc_spi_memdrv_info);
        if (memdrv_err != MEMDRV_SUCCESS)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return SDC_SPI_ERR_INTERNAL;
        }

#else
        /* Setting the parameters used in MEMDRV */
        sdc_spi_memdrv_info.cnt = 0;
        sdc_spi_memdrv_info.p_data = NULL;
        sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        sdc_spi_memdrv_info.read_after_write = true;

        memdrv_err = R_MEMDRV_EnableTxData(p_hndl->dev_no, &sdc_spi_memdrv_info);
        if (memdrv_err != MEMDRV_SUCCESS)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return SDC_SPI_ERR_INTERNAL;
        }

        /* Setting the parameters used in MEMDRV */
        sdc_spi_memdrv_info.cnt = txcnt;
        sdc_spi_memdrv_info.p_data = p_data;
        sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        sdc_spi_memdrv_info.read_after_write = true;

        memdrv_err = R_MEMDRV_TxData(p_hndl->dev_no, &sdc_spi_memdrv_info);
        if (memdrv_err != MEMDRV_SUCCESS)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return SDC_SPI_ERR_INTERNAL;
        }

        /* Setting the parameters used in MEMDRV */
        sdc_spi_memdrv_info.cnt = 0;
        sdc_spi_memdrv_info.p_data = NULL;
        sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        sdc_spi_memdrv_info.read_after_write = true;

        memdrv_err = R_MEMDRV_DisableTxData(p_hndl->dev_no, &sdc_spi_memdrv_info);
        if (memdrv_err != MEMDRV_SUCCESS)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return SDC_SPI_ERR_INTERNAL;
        }
#endif
    }

    return SDC_SPI_SUCCESS;

} /* End of function r_sdc_spi_tx_data() */

/**********************************************************************************************************************
* Outline      : Data receive using MEMDRV FIT
* Function Name: r_sdc_spi_rx_data
* Description  : Data receive using MEMDRV FIT
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint32_t           txcnt               ;   Send dummy data count.
*              : uint8_t            * p_data            ;   Send data pointer
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR_INTERNAL                   ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_rx_data(sdc_spi_hndl_t * p_hndl, uint16_t rxcnt, uint8_t * p_data)
{
    memdrv_err_t memdrv_err;
    st_memdrv_info_t  sdc_spi_memdrv_info;

    /* **** Receive data **** */
    if (0 == p_hndl->dev_no)
    {
#if (MEMDRV_TRNS_CPU == MEMDRV_CFG_DEV0_MODE_TRNS)
        /* Setting the parameters used in MEMDRV */
        sdc_spi_memdrv_info.cnt = rxcnt;
        sdc_spi_memdrv_info.p_data = p_data;
        sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        sdc_spi_memdrv_info.read_after_write = true;

        memdrv_err = R_MEMDRV_Rx(p_hndl->dev_no, &sdc_spi_memdrv_info);
        if (memdrv_err != MEMDRV_SUCCESS)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return SDC_SPI_ERR_INTERNAL;
        }
#else
        /* Setting the parameters used in MEMDRV */
        sdc_spi_memdrv_info.cnt = 0;
        sdc_spi_memdrv_info.p_data = NULL;
        sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        sdc_spi_memdrv_info.read_after_write = true;

        memdrv_err = R_MEMDRV_EnableRxData(p_hndl->dev_no, &sdc_spi_memdrv_info);
        if (memdrv_err != MEMDRV_SUCCESS)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return SDC_SPI_ERR_INTERNAL;
        }

        /* Setting the parameters used in MEMDRV */
        sdc_spi_memdrv_info.cnt = rxcnt;
        sdc_spi_memdrv_info.p_data = p_data;
        sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        sdc_spi_memdrv_info.read_after_write = true;

        memdrv_err = R_MEMDRV_RxData(p_hndl->dev_no, &sdc_spi_memdrv_info);
        if (memdrv_err != MEMDRV_SUCCESS)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return SDC_SPI_ERR_INTERNAL;
        }

        /* Setting the parameters used in MEMDRV */
        sdc_spi_memdrv_info.cnt = 0;
        sdc_spi_memdrv_info.p_data = NULL;
        sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        sdc_spi_memdrv_info.read_after_write = true;

        memdrv_err = R_MEMDRV_DisableRxData(p_hndl->dev_no, &sdc_spi_memdrv_info);
        if (memdrv_err != MEMDRV_SUCCESS)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return SDC_SPI_ERR_INTERNAL;
        }

#endif
    }
    else
    {
#if (MEMDRV_TRNS_CPU == MEMDRV_CFG_DEV1_MODE_TRNS)
        /* Setting the parameters used in MEMDRV */
        sdc_spi_memdrv_info.cnt = rxcnt;
        sdc_spi_memdrv_info.p_data = p_data;
        sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        sdc_spi_memdrv_info.read_after_write = true;

        memdrv_err = R_MEMDRV_Rx(p_hndl->dev_no, &sdc_spi_memdrv_info);
        if (memdrv_err != MEMDRV_SUCCESS)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return SDC_SPI_ERR_INTERNAL;
        }
#else
        /* Setting the parameters used in MEMDRV */
        sdc_spi_memdrv_info.cnt = 0;
        sdc_spi_memdrv_info.p_data = NULL;
        sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        sdc_spi_memdrv_info.read_after_write = true;

        memdrv_err = R_MEMDRV_EnableRxData(p_hndl->dev_no, &sdc_spi_memdrv_info);
        if (memdrv_err != MEMDRV_SUCCESS)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return SDC_SPI_ERR_INTERNAL;
        }

        /* Setting the parameters used in MEMDRV */
        sdc_spi_memdrv_info.cnt = rxcnt;
        sdc_spi_memdrv_info.p_data = p_data;
        sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        sdc_spi_memdrv_info.read_after_write = true;

        memdrv_err = R_MEMDRV_RxData(p_hndl->dev_no, &sdc_spi_memdrv_info);
        if (memdrv_err != MEMDRV_SUCCESS)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return SDC_SPI_ERR_INTERNAL;
        }

        /* Setting the parameters used in MEMDRV */
        sdc_spi_memdrv_info.cnt = 0;
        sdc_spi_memdrv_info.p_data = NULL;
        sdc_spi_memdrv_info.io_mode = MEMDRV_MODE_SINGLE;
        sdc_spi_memdrv_info.read_after_write = true;

        memdrv_err = R_MEMDRV_DisableRxData(p_hndl->dev_no, &sdc_spi_memdrv_info);
        if (memdrv_err != MEMDRV_SUCCESS)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return SDC_SPI_ERR_INTERNAL;
        }

#endif
    }
    return SDC_SPI_SUCCESS;

} /* End of function r_sdc_spi_rx_data() */

/**********************************************************************************************************************
* Outline      : SD card access wait and ready
* Function Name: r_sdc_spi_wait_ready
* Description  : SD card access wait and ready.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_wait_ready(sdc_spi_hndl_t * p_hndl)
{
    uint16_t waitcnt;
    sdc_spi_status_t  ret;

    /* Wait for loop */
    /* WAIT_LOOP */
    for(waitcnt = SDC_SPI_READY_WAIT; waitcnt != 0; waitcnt--)
    {
        ret = r_sdc_spi_chk_rxdata(p_hndl, (uint32_t)SDC_SPI_READY_SIZE, SDC_SPI_READY_DATA);
        if (SDC_SPI_SUCCESS <= ret)
        {
            return SDC_SPI_SUCCESS;
        }

        R_BSP_SoftwareDelay(10, BSP_DELAY_MICROSECS);
    }
    r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
    return SDC_SPI_ERR;
} /* End of function r_sdc_spi_wait_ready() */

/**********************************************************************************************************************
* Outline      : Status check
* Function Name: r_sdc_spi_chk_sts
* Description  : Status check using CMD13.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Failed operation
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*              : SDC_SPI_ERR_CRC                        ;   Error CRC error
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_chk_sts(sdc_spi_hndl_t * p_hndl)
{
    uint8_t * p_status;
    sdc_spi_status_t  ret;

    /* Send CMD13 command */
    ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_CMD13, 0, SDC_SPI_R2_SIZE, SDC_SPI_OTHER_STAT);
    if (SDC_SPI_SUCCESS <= ret)
    {
        /* Check response status */
        p_status = (uint8_t *)&g_sdc_spi_resbuf[SDC_SPI_RES_R2IDX];
        if (SDC_SPI_RES2_CHKDATA == (*p_status))
        {
            return SDC_SPI_SUCCESS;
        }
        else
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return SDC_SPI_ERR;
        }
    }

    r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
    return ret;
} /* End of function r_sdc_spi_chk_sts() */

/**********************************************************************************************************************
* Outline      : Receive CRC16 and verify
* Function Name: r_sdc_spi_read_data_crc
* Description  : Receive CRC16 and verify.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint32_t           rxcnt               ;   Receive data count.
*              : uint8_t            * p_data            ;   Receive data pointer
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Failed operation
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*              : SDC_SPI_ERR_CRC                        ;   Error CRC error
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_read_data_crc(sdc_spi_hndl_t * p_hndl, uint16_t rxcnt, uint8_t * p_data)
{
    volatile exchg_short_t crcbuf;
    volatile uint16_t crcdata;
    volatile uint32_t work_data_buf;
    sdc_spi_status_t ret;

    /* Get CRC16 data */
    ret = r_sdc_spi_rx_data_crc16(p_hndl, rxcnt, p_data, (uint16_t *)&crcdata);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
        return ret;
    }

    /* Receive CRC16 first byte */
    ret = r_sdc_spi_rx_data(p_hndl, (uint16_t)1, (uint8_t *)&work_data_buf);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
        return ret;
    }
#ifdef __BIG
    crcbuf.uc[1] = (uint8_t)((work_data_buf & 0xff000000) >> SDC_SPI_BIT_SHIFT_24);
#else
    crcbuf.uc[1] = (uint8_t)(work_data_buf & 0x000000ff);
#endif

    /* Receive CRC16 second byte  */
    ret = r_sdc_spi_rx_data(p_hndl, (uint16_t)1, (uint8_t *)&work_data_buf);
    if (SDC_SPI_SUCCESS > ret)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
        return ret;
    }
#ifdef __BIG
    crcbuf.uc[0] = (uint8_t)((work_data_buf & 0xff000000) >> SDC_SPI_BIT_SHIFT_24);
#else
    crcbuf.uc[0] = (uint8_t)(work_data_buf & 0x000000ff);
#endif

    /* Check CRC */
    if (crcdata != crcbuf.us)
    {
        r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
        return SDC_SPI_ERR_CRC;
    }

    return SDC_SPI_SUCCESS;
} /* End of function r_sdc_spi_read_data_crc() */

/**********************************************************************************************************************
* Outline      : Send command CMD12.
* Function Name: r_sdc_spi_send_cmd12
* Description  : Send command CMD12.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Other error operation.
*              : SDC_SPI_ERR_ILLEGAL_COMMAND            ;   Illegal command error.
*              : SDC_SPI_ERR_CRC                        ;   CRC error.
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_send_cmd12(sdc_spi_hndl_t * p_hndl)
{
    uint16_t rtycnt;
    sdc_spi_status_t ret;

    /* Send CMD12 routine */
    /* WAIT_LOOP */
    for(rtycnt = SDC_SPI_RTY_CNT;rtycnt != 0;rtycnt--)
    {
        /* Send command */
        ret = r_sdc_spi_send_cmd(p_hndl, SDC_SPI_CMD12, 0, SDC_SPI_R1_SIZE, SDC_SPI_OTHER_STAT);
        if (SDC_SPI_SUCCESS == ret)
        {
            ret = r_sdc_spi_chk_sts(p_hndl);
            if (SDC_SPI_SUCCESS == ret)
            {
                r_sdc_spi_wait_ready(p_hndl);
                return SDC_SPI_SUCCESS;
            }
        }
        /* Status check */
        r_sdc_spi_chk_sts(p_hndl);
    }
    r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
    return ret;
} /* End of function r_sdc_spi_send_cmd12() */

/**********************************************************************************************************************
* Outline      : Check receive data
* Function Name: r_sdc_spi_chk_rxdata
* Description  : Check receive data.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint32_t           rtycnt              ;   Retry count.
*              : uint8_t            chkdata             ;   check data
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR                            ;   Failed operation
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_chk_rxdata(sdc_spi_hndl_t * p_hndl, uint32_t rtycnt, uint8_t chkdata)
{
    volatile uint8_t rxbuf;
    volatile uint32_t work_data_buf;
    sdc_spi_status_t ret;

    /* WAIT LOOP */
    do{
        /* Receive data */
        ret = r_sdc_spi_rx_data(p_hndl, 1, (uint8_t *)&work_data_buf);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return ret;
        }
#ifdef __BIG
        rxbuf = (uint8_t)((work_data_buf & 0xff000000) >> SDC_SPI_BIT_SHIFT_24);
#else
        rxbuf = (uint8_t)(work_data_buf & 0x000000ff);
#endif
        /* Receive data check */
        if (chkdata == rxbuf)
        {
            return SDC_SPI_SUCCESS;
        }
        rtycnt--;

    }while(0 != rtycnt);

    r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
    return SDC_SPI_ERR;

} /* End of function r_sdc_spi_chk_rxdata() */

/**********************************************************************************************************************
* Outline      : Receive data and crc16.
* Function Name: r_sdc_spi_rx_data_crc16
* Description  : Receive data and crc16.
* Arguments    : sdc_spi_hndl_t     * p_hndl            ;   SPI mode SDC driver handle
*              : uint32_t           rxcnt               ;   Receive data count.
*              : uint8_t            * p_data            ;   Receive data pointer
*              : uint8_t            * p_crc16           ;   CRC16 data pointer
* Return Value : SDC_SPI_SUCCESS                        ;   Successful operation
*              : SDC_SPI_ERR_INTERNAL                   ;   Error in module used inside driver
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_spi_status_t r_sdc_spi_rx_data_crc16(sdc_spi_hndl_t * p_hndl, uint16_t rxcnt, uint8_t * p_data, uint16_t * p_crc16)
{
    sdc_spi_status_t ret;
    uint16_t rxcnttmp = rxcnt;
    uint8_t * p_datatmp = p_data;
    volatile uint32_t work_data_buf;
    int32_t i;

    /* Data receive */
    for (i = rxcnt; i != 0 ; i--)
    {
        ret = r_sdc_spi_rx_data(p_hndl, 1, (uint8_t *)&work_data_buf);
        if (SDC_SPI_SUCCESS > ret)
        {
            r_sdc_spi_log(SDC_SPI_DEBUG_ERR_ID, SDC_SPI_SUB, __LINE__);
            return ret;
        }
#ifdef __BIG
        *(p_data + (rxcnt - i)) = (uint8_t)((work_data_buf & 0xff000000) >> SDC_SPI_BIT_SHIFT_24);
#else
        *(p_data + (rxcnt - i)) = (uint8_t)(work_data_buf & 0x000000ff);
#endif
    }

    /* Get CRC16 */
    *p_crc16 = r_sdc_spi_crc16(p_datatmp, rxcnttmp);

    return SDC_SPI_SUCCESS;

} /* End of function r_sdc_spi_rx_data_crc16() */

/**********************************************************************************************************************
* Outline      : Exchange 32bit data
* Function Name: r_sdc_spi_exchglong
* Description  : Exchange 32bit data
* Arguments    : uint32_t           chgdata             ;   Exchange data
* Return Value : uint32_t
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t r_sdc_spi_exchglong(uint32_t chgdata)
{
    exchg_long_t  tmp;
    exchg_long_t  ret;

    /* Exchange data */
    tmp.ul = chgdata;
#ifdef __BIG
    ret.uc[0] = tmp.uc[0];
    ret.uc[1] = tmp.uc[1];
    ret.uc[2] = tmp.uc[2];
    ret.uc[3] = tmp.uc[3];
#else
    ret.uc[0] = tmp.uc[3];
    ret.uc[1] = tmp.uc[2];
    ret.uc[2] = tmp.uc[1];
    ret.uc[3] = tmp.uc[0];
#endif
    return ret.ul;

} /* End of function r_sdc_spi_exchglong() */

/**********************************************************************************************************************
* Outline      : Exchange crc16 data
* Function Name: r_sdc_spi_exchg_crc16
* Description  : Exchange crc16 data
* Arguments    : None
* Return Value : uint16_t
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint16_t r_sdc_spi_exchg_crc16(void)
{

    uint8_t * p_ptr;
    uint16_t dat;

    p_ptr =(uint8_t *) &g_sdc_spi_crc16.us;
    dat = g_sdc_spi_crc16.us;
    /* Exchange data */
    p_ptr[0] = (uint8_t )( dat & (uint16_t )0x00FF);
    p_ptr[1] = (uint8_t )((uint16_t)( dat & (uint16_t )0xFF00) >> 8);

    return g_sdc_spi_crc16.us;

} /* End of function r_sdc_spi_exchg_crc16() */

/**********************************************************************************************************************
* Outline      : CRC7 data calculation.
* Function Name: r_sdc_spi_crc7
* Description  : CRC7 data calculation.
* Arguments    : uint8_t            * p_data            ;   Receive data pointer
*              : uint16_t           crccnt              ;   CRC7 count data.
* Return Value : uint8_t                                ;   CRC7 data
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint8_t r_sdc_spi_crc7(uint8_t * p_data, uint16_t crccnt)
{
    uint8_t crcdata = 0x00;

    /* WAIT_LOOP */
    do
    {
        crcdata = gs_crc7_table[crcdata ^ *p_data];
        p_data++;
        crccnt--;
    }while(0 != crccnt);

    return crcdata;

} /* End of function r_sdc_spi_crc7() */


/**********************************************************************************************************************
* Outline      : CRC16 data calculation.
* Function Name: r_sdc_spi_crc16
* Description  : CRC16 data calculation.
* Arguments    : uint8_t            * p_data            ;   Receive data pointer
*              : uint16_t           crccnt              ;   CRC16 count data.
* Return Value : uint16_t                               ;   CRC16 data
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint16_t r_sdc_spi_crc16(uint8_t * p_data, uint16_t crccnt)
{
    g_sdc_spi_crc16.us = 0x0000;
    volatile uint8_t uctmp;

#if (1 == SDC_SPI_CFG_USE_SC_CRC)
    R_Config_CRC_SetCCITT();
#endif

    /* WAIT_LOOP */
    do
    {
#if (1 == SDC_SPI_CFG_USE_SC_CRC)
        R_Config_CRC_Input_Data(*p_data);
#else

#ifdef __BIG
        uctmp = g_sdc_spi_crc16.uc[1];
        g_sdc_spi_crc16.us = gs_crc_ccitt_table[g_sdc_spi_crc16.uc[0] ^ *(p_data)];
        g_sdc_spi_crc16.uc[0] = g_sdc_spi_crc16.uc[0] ^ uctmp;
#else
        uctmp = g_sdc_spi_crc16.uc[0];
        g_sdc_spi_crc16.us = gs_crc_ccitt_table[g_sdc_spi_crc16.uc[1] ^ *(p_data)];
        g_sdc_spi_crc16.uc[1] = g_sdc_spi_crc16.uc[1] ^ uctmp;
#endif

#endif
        p_data++;
        crccnt--;

    }while(0 != crccnt);

#if (1 == SDC_SPI_CFG_USE_SC_CRC)
    R_Config_CRC_Get_Result ((uint16_t *)&(g_sdc_spi_crc16.us));
#endif
    return r_sdc_spi_exchg_crc16();

} /* End of function r_sdc_spi_crc16() */

/* End of File */
