/**********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2014(2015-2016) Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/
/**********************************************************************************************************************
* System Name  : SDHI Driver
* File Name    : r_sdc_sdio_initialize.c
* Version      : 2.00
* Device       : RX
* Abstract     : API & Sub module
* Tool-Chain   : For RX e2_studio
* OS           : not use
* H/W Platform : RSK board for RX
* Description  : Interface file for SDC SD API for RX
* Limitation   : None
**********************************************************************************************************************/
/**********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 05.09.2015 1.00    First Release
*              : 17.07.2015 1.10    Standardized by the combo.
*              : 06.01.2016 1.20    Changed r_sdc_sd_set_speed().
*              : 30.09.2016 1.21    Added r_sdc_sdio_chk_support_type() to r_sdc_sdio_initialize().
*              :                    Changed the judgment of "Read After Write".
*              : 31.07.2017 2.00    SDHI FIT module separated into hardware low level layer and middleware layer.
*              :                    Changed prefix from SDHI to SDC_SD.
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "r_sdc_sd_rx_if.h"
#include ".\src\r_sdc_sd_rx_private.h"

#if (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO)

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
#define SDC_SDIO_DRIVER_CODE_NUM     (14)

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/
static sdc_sd_status_t r_sdc_sdio_init_rca(uint32_t card_no);
const uint8_t g_sdhi_sdio_driver_code[SDC_SDIO_DRIVER_CODE_NUM] = {0x52, 0x54, 0x4d, 0x30, 0x52, 0x58, 0x30, 0x30,
                                                                   0x30, 0x30, 0x44, 0x53, 0x44, 0x44};

/**********************************************************************************************************************
* Outline      : Initialize SDIO
* Function Name: r_sdc_sdio_initialize
* Description  : Initializes the SDIO media.
* Arguments    : uint32_t           card_no             ;   SD Card No.
*              : uint32_t           init_type           ;   SDHI Initialize type
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_initialize(uint32_t card_no, uint32_t init_type)
{
    sdc_sd_hndl_t   * p_hndl = 0;
    uint32_t          sdhi_sdiomd_ctrl = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    if (SDC_SD_MODE_IO == init_type)
    {
        /* ==== Initialize card and distinguish card type. ==== */
        if (r_sdc_sdio_init(card_no) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
            return p_hndl->error;
        }
#if 0
        if (r_sdc_sdio_chk_support_type(card_no) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
            return p_hndl->error;
        }
#endif
        if (r_sdc_sdio_mount(card_no) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
            return p_hndl->error;
        }

        /* If it is a SDIO or SD Combo, set the IO part speed. */
        if (r_sdc_sdio_set_speed(card_no) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
            return p_hndl->error;
        }
        /* Enable the SDIO interrupt. */
        R_SDHI_InReg(p_hndl->channel, SDHI_SDIOMD, &sdhi_sdiomd_ctrl);
        sdhi_sdiomd_ctrl = (uint32_t)(sdhi_sdiomd_ctrl | SDHI_SDIOMD_INTEN);
        R_SDHI_OutReg(p_hndl->channel, SDHI_SDIOMD, sdhi_sdiomd_ctrl);

        r_sdc_sdio_enable_int_ioirqm(card_no);
    }
    else
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return SDC_SD_ERR;
    }

    return p_hndl->error;
} /* End of function r_sdc_sdio_initialize() */

/**********************************************************************************************************************
* Outline      : Initialize Card
* Function Name: r_sdc_sdio_init
* Description  : Initializes the SDIO Card and then puts into Command State (CMD).
* Arguments    : uint32_t       card_no                 ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_init(uint32_t card_no)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    sdc_sd_status_t       ret = SDC_SD_SUCCESS;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ==== Initialize SDIO. ==== */
    /* ---- Get the I/O OCR using CMD5 with Arg = 0 and then store the OCR for I/O. ---- */
    ret = r_sdc_sd_send_ocr(card_no, SDC_SD_MEDIA_UNKNOWN);

    if (SDC_SD_SUCCESS == ret)
    {
        /* Check the Number (>0) of I/O functions and the OCR validity. */
        if ((0 != (p_hndl->io_ocr[0] & SDC_SDIO_CMD5_R4_NUMIOFNCS))
        &&  (r_sdc_sdio_get_func_max_num() == ((p_hndl->io_ocr[0] & SDC_SDIO_CMD5_R4_NUMIOFNCS) >> SDC_SD_BIT_SHIFT_28))
        &&  (0 != (p_hndl->io_ocr[0] & SDC_SDIO_CMD5_R4_IOOCR    )))
        {
            /* ---- Send OCR to start initialization using CMD5 with Arg and then store the OCR for I/O. ---- */
            if (SDC_SD_SUCCESS == (r_sdc_sd_send_ocr(card_no, SDC_SD_MEDIA_SDIO)))
            {
                /* Set a Initialized flag. */
                p_hndl->io_flag = (p_hndl->io_flag | SDC_SDIO_FUNC_INIT);

                /* Store the information (R4 Bits 31-24) of SDIO. */
                /* Bit 31      : Ready                            */
                /* Bit 30 - 28 : Numbers of I/O Functions         */
                /* Bit 27      : Memory Present                   */
                /* Bit 26      : UHS-II Card Status               */
                /* Bit 25      : Stuff bit                        */
                /* Bit 24      : S18A                             */
                p_hndl->io_info = (uint8_t)(p_hndl->io_ocr[0] >> SDC_SD_BIT_SHIFT_24);
                
                /* Media Type */
                p_hndl->media_type = (p_hndl->media_type | SDC_SD_MEDIA_SDIO);

                /* Put into Command State (CMD). */
                return r_sdc_sdio_init_rca(card_no);
            }
            else /* Response Timeout (, CBSY, No Response) */
            {
                /* The voltage range is already checked so that the card is not put into the inactive state. */
                SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
                r_sdc_sd_set_err(card_no, SDC_SD_ERR);
                return p_hndl->error;
            }
        }
        else /* The Number of I/O functions =0 or the invalid OCR */
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR);
            return SDC_SD_ERR;
        }
    }
    else /* No Response (,CBSY Error, Response Timeout) */
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
        return p_hndl->error;
    }

    return SDC_SD_SUCCESS;
    
} /* End of function r_sdc_sdio_init() */

/**********************************************************************************************************************
* Outline      : Initialize card RCA
* Function Name: r_sdc_sdio_init_rca
* Description  : Gets RCA and puts into Command State (CMD) via Standby State (STB).
* Arguments    : uint32_t       card_no                 ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
static sdc_sd_status_t r_sdc_sdio_init_rca(uint32_t card_no)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    int32_t             i = 0;
    uint32_t            tmp = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* Save RCA of Memory for SDIO re-initialization. */
    tmp = p_hndl->rca[0];

    /* ==== Put into Command State (CMD). ==== */
    /* ---- Request to publish a new RCA using CMD3 and then put into Standby State (STB). ---- */
    for (i = 0; i < SDC_SD_MAX_AGAIN_COUNT; i++)
    {
        if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD3, SDC_SD_RESP_R6, 0) != SDC_SD_SUCCESS)
        {
            p_hndl->rca[0] = tmp;
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
            return SDC_SD_ERR;
        }

        if (0x00 != p_hndl->rca[0])
        {
            p_hndl->rca[1] = p_hndl->rca[0];        /* Store the RCA to rca[1]. */
            p_hndl->rca[0] = tmp;
            p_hndl->io_flag = (p_hndl->io_flag | SDC_SDIO_POWER_INIT);
            break;
        }
    }

    /* Illegal RCA */
    if (SDC_SD_MAX_AGAIN_COUNT == i)
    {
        p_hndl->rca[0] = tmp;
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_CARD_CC);
        return SDC_SD_ERR;
    }

    /* ---- Put into Command State (CMD). ---- */
    if (r_sdc_sd_send_cmd_arg(card_no, SDC_SD_CMD7, SDC_SD_RESP_R1b, p_hndl->rca[1]) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
        return p_hndl->error;
    }

    return SDC_SD_SUCCESS;

} /* End of function r_sdc_sdio_init_rca() */

/**********************************************************************************************************************
* Outline      : Mount SDIO Card
* Function Name: r_sdc_sdio_mount
* Description  : Mounts the SDIO.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_mount(uint32_t card_no)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    int32_t             i = 0;
    sdc_sdio_daccess_t sdc_sdio_daccess;
    uint8_t             io_buff[4] = {0, 0, 0, 0};
    int32_t             cnt = 0;

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* ==== Get the Card Capability using CMD52 and then store the speed information for Transfer State (TRN). ==== */
    /* ---- Read the Card Capability using CMD52. ---- */
    /* Function 0, Address : 08h (Card Capability) */
    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_CARDCAPAB;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

    if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
    {
        /* Clear the error code in order to re-try. */
        p_hndl->error = SDC_SD_SUCCESS;

        if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
            return p_hndl->error;
        }
    }

    /* ---- Store the speed information for Transfer State (TRN). ---- */
    if (io_buff[0] & SDC_SDIO_CARDCAPAB_LSC)        /* 400kHz low speed card */
    {
        p_hndl->io_tran_speed = r_sdc_sd_get_div_init_speed();
        p_hndl->io_speed_mode = ((p_hndl->io_speed_mode | SDC_SD_CUR_SPEED_LOW) | SDC_SD_SUP_SPEED_LOW);
    }
    else /* 25MHz Default speed card or 50MHz High speed card */
    {
        p_hndl->io_tran_speed = r_sdc_sd_get_div_default_speed();
        p_hndl->io_speed_mode = (p_hndl->io_speed_mode & ~(((SDC_SD_CUR_SPEED      | SDC_SD_SUP_SPEED) |
                                                             SDC_SD_CUR_SPEED_LOW) | SDC_SD_SUP_SPEED_LOW));
    }

    /* ==== Change the clock for Transfer State (TRN). ==== */
    if (R_SDHI_SetClock(p_hndl->channel, p_hndl->io_tran_speed, SDHI_CLOCK_ENABLE) != SDHI_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
        /* The Clock is not started, and changed from 400KHz to the Transfer clock. */
        /* The upper layer software stop the clock. */
/*        R_SDHI_SetClock(p_hndl->channel, 0, SDHI_CLOCK_DISABLE);     */
        r_sdc_sd_set_err(card_no, SDC_SD_ERR);
        return p_hndl->error;
    }
    
    /* ==== Set the bus width of the card according to the information of the Card Capability. ==== */
    /* ---- Disable a pull-up register on DAT3 before issuing CMD53 and then set bus width. ---- */
    if ((io_buff[0] & SDC_SDIO_CARDCAPAB_LSC) && (0 == (io_buff[0] & SDC_SDIO_CARDCAPAB_4BLS)))
    {   /* Not support 4-bit Low Speed Card mode. */
        /* Set the CD Disable bit, also. */
        if (r_sdc_sdio_set_port(card_no, SDHI_PORT_1BIT) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
            return SDC_SD_ERR;
        }
        R_SDHI_SetBus(p_hndl->channel, SDHI_PORT_1BIT);
        p_hndl->sup_if_mode = SDC_SD_MODE_1BIT;           /* Set the bus width with both SD Memory and SDIO. */
    }
    else
    {
        /* Set the CD Disable bit, also. */
        if (r_sdc_sdio_set_port(card_no, p_hndl->sup_if_mode) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
            return SDC_SD_ERR;
        }

        if (SDC_SD_MODE_1BIT == p_hndl->sup_if_mode)
        {
            R_SDHI_SetBus(p_hndl->channel, SDHI_PORT_1BIT);
        }
        else
        {
            R_SDHI_SetBus(p_hndl->channel, SDHI_PORT_4BIT);
        }
    }

    /* ==== Disable IENM bit of Int Enable register. ==== */
    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_INTENABLE;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

    if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
        return p_hndl->error;
    }

    io_buff[0] &= (~SDC_SDIO_INTENABLE_IENM);
    io_buff[1] = io_buff[0];
    sdc_sdio_daccess.p_buff   = &io_buff[0];
    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_INTENABLE;
    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_W;
    sdc_sdio_daccess.raw_flag = SDC_SDIO_READ_AFTER_WRITE;

    if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
        return p_hndl->error;
    }

    if (io_buff[1] != io_buff[0])           /* Verify error */
    {
        /* Retry to read */
        io_buff[0] = 0;
        sdc_sdio_daccess.p_buff   = &io_buff[0];
        sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
        sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_INTENABLE;
        sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
        sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

        if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
            return p_hndl->error;
        }

        if (io_buff[1] != io_buff[0])           /* Verify error */
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
            r_sdc_sd_set_err(card_no, SDC_SD_ERR);
            return p_hndl->error;
        }
    }

    /* ==== Get the CCCR and FBR using CMD52 and then store the information. ==== */
    /* ---- Read the CCCR and FBR and then store it. ---- */
    for (i = 0; i < ((int32_t)r_sdc_sdio_get_func_max_num()+1); i++)
    {
        for (cnt = 0; cnt < (int32_t)r_sdc_sdio_get_io_reg_size(); cnt++)
        {
            if (SDC_SDIO_FNO_0 == i)
            {
                if ((int32_t)SDC_SDIO_ADRS_IOABORT == cnt)
                {
                    /* Do not access the following registers.
                       Write Only Register :
                            CCCR 06h       I/O Abort ASx and RES */
                }
                else
                {
                    sdc_sdio_daccess.p_buff   = &(p_hndl->io_reg[i][cnt]);
                    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
                    sdc_sdio_daccess.adr      = (uint32_t)(0x100 * i) + cnt;
                    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
                    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;
                    if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
                    {
                        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
                        return p_hndl->error;
                    }
                }
            }
            else if ((SDC_SDIO_FNO_1 <= i) && (SDC_SDIO_FNO_7 >= i))
            {
                if (((int32_t)SDC_SDIO_ADRS_CSAPTR0 == cnt) ||
                    ((int32_t)SDC_SDIO_ADRS_CSAPTR1 == cnt) ||
                    ((int32_t)SDC_SDIO_ADRS_CSAPTR2 == cnt) ||
                    ((int32_t)SDC_SDIO_ADRS_CSADAW  == cnt))
                {
                    /* Do not access the following registers.
                       The Driver is not supported :
                            FBR  x0Ch-x0Eh Pointer to Function1 Code Storage Area (CSA)
                            FBR  x0Fh      Data access window to Function 1 Code Storage Area (CSA) */
                }
                else
                {
                    sdc_sdio_daccess.p_buff   = &(p_hndl->io_reg[i][cnt]);
                    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
                    sdc_sdio_daccess.adr      = (uint32_t)(0x100 * i) + cnt;
                    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
                    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;
                    if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
                    {
                        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
                        return p_hndl->error;
                    }
                }
            }
            else
            {
                /* Do nothing. */
            }
        }
    }

    return SDC_SD_SUCCESS;
    
} /* End of function r_sdc_sdio_mount() */

/**********************************************************************************************************************
* Outline      : Set SDIO Speed
* Function Name: r_sdc_sdio_set_speed
* Description  : Queries the high speed for the High-Speed mode card.
* Arguments    : uint32_t       card_no                 ;   SD Card No.
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_set_speed(uint32_t card_no)
{
    sdc_sd_hndl_t       * p_hndl = 0;
    sdc_sdio_daccess_t sdc_sdio_daccess;
    uint8_t             io_buff[4] = {0, 0, 0, 0};

    p_hndl = SDC_SD_GET_HNDL(card_no);

    /* Check CCCR version 2.00. */
    if (SDC_SDIO_CCCRFBR_VER_20 <= (p_hndl->io_reg[SDC_SDIO_FNO_0][SDC_SDIO_ADRS_CCCRSDIO] & SDC_SDIO_CCCRSDIO_CCCR))
    {
        /* Check SHS of Bus Speed Select. */
        if (p_hndl->io_reg[SDC_SDIO_FNO_0][SDC_SDIO_ADRS_BUSSPEED] & SDC_SDIO_BUSSPEED_SHS)
        {
            /* Set BSS of Bus Speed Select to High speed and store it. */
            if (SDC_SD_SUCCESS == R_SDHI_GetSpeedType(p_hndl->channel))
            {
                /* Function 0, Address : 13h (Bus Speed Select), Data : 0x02 (High Speed) */
                io_buff[0] = SDC_SDIO_BUSSPEED_BSS;
                io_buff[1] = io_buff[0];
                sdc_sdio_daccess.p_buff   = &io_buff[0];
                sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
                sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_BUSSPEED;
                sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_W;
                sdc_sdio_daccess.raw_flag = SDC_SDIO_READ_AFTER_WRITE;

                if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
                {
                    SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
                    return p_hndl->error;
                }

                if (io_buff[1] == (io_buff[0] & io_buff[1]))             /* High speed mode */
                {
                    /* Store the speed to High speed mode */
                    p_hndl->io_reg[SDC_SDIO_FNO_0][SDC_SDIO_ADRS_BUSSPEED] = io_buff[0];
                    p_hndl->io_tran_speed = r_sdc_sd_get_div_high_speed();
                    p_hndl->io_speed_mode = ((p_hndl->io_speed_mode | SDC_SD_CUR_SPEED) | SDC_SD_SUP_SPEED);
                }
                else
                {
                    /* Retry to read Function 0, Address : 13h (Bus Speed Select). */
                    io_buff[0] = 0;
                    sdc_sdio_daccess.p_buff   = &io_buff[0];
                    sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
                    sdc_sdio_daccess.adr      = SDC_SDIO_ADRS_BUSSPEED;
                    sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
                    sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

                    if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
                    {
                        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
                        return p_hndl->error;
                    }

                    p_hndl->io_reg[SDC_SDIO_FNO_0][SDC_SDIO_ADRS_BUSSPEED] = io_buff[0];

                    if (io_buff[1] == (io_buff[0] & io_buff[1]))             /* High speed mode */
                    {
                        /* Store the speed to High speed mode */
                        p_hndl->io_tran_speed = r_sdc_sd_get_div_high_speed();
                        p_hndl->io_speed_mode = ((p_hndl->io_speed_mode | SDC_SD_CUR_SPEED) | SDC_SD_SUP_SPEED);
                    }
                }
            }
        }
    }

    return SDC_SD_SUCCESS;

} /* End of function r_sdc_sdio_set_speed() */

/**********************************************************************************************************************
* Outline      : Supported Type Checking Operation
* Function Name: r_sdc_sdio_chk_support_type
* Description  : Checks supported type.
* Arguments    : uint32_t       card_no                 ;   SDHI Channel No
* Return Value : SDC_SD_SUCCESS                         ;   Successful operation
*              : other                                  ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
sdc_sd_status_t r_sdc_sdio_chk_support_type(uint32_t card_no)
{
    sdc_sdio_daccess_t   sdc_sdio_daccess;
    uint32_t              i = 0;
    uint32_t              adr = 0;
    uint8_t               io_buf[4] = {0, 0, 0, 0};
    uint16_t              crc_code = 0;

    /* Clear CRC-CCITT calculation data */
    r_sdc_sdio_clear_crc16(card_no);

    /* Set SDIO driver code to CRC-CCITT calculation. */
    for (i = 0; i < SDC_SDIO_DRIVER_CODE_NUM; i++)
    {
        r_sdc_sdio_set_crc16(card_no, (uint8_t *)&g_sdhi_sdio_driver_code[i]);
    }

    /* Set the CIS address. */
    adr = SDIO_IO_CHK_ADR1;

    /* Get CIS and set CIS to CRC-CCITT calculation. */
    for (i = 0; i < SDIO_IO_CHK_NUM; i++)
    {
        /* Get CIS. */
        sdc_sdio_daccess.p_buff   = &io_buf[0];
        sdc_sdio_daccess.func     = SDC_SDIO_FNO_0;
        sdc_sdio_daccess.adr      = adr++;
        sdc_sdio_daccess.rw_flag  = SDC_SDIO_RWFLAG_R;
        sdc_sdio_daccess.raw_flag = SDC_SDIO_SIMPLE_READ;

        if (r_sdc_sdio_direct(card_no, &sdc_sdio_daccess) != SDC_SD_SUCCESS)
        {
            SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
            return SDC_SD_ERR;
        }

        /* Set CIS to CRC-CCITT calculation. */
        r_sdc_sdio_set_crc16(card_no, &io_buf[0]);
    }

    /* Judge the CRC calculation data. */
    crc_code = r_sdc_sdio_get_crc16(card_no);
    if (SDIO_IO_CHK_CODE != crc_code)
    {
        SDC_SD_LOG_FUNC(SDC_SD_DEBUG_ERR_ID, (uint32_t)SDC_SDIO_INITIALIZE, __LINE__);
        r_sdc_sd_set_err(card_no, SDC_SD_ERR_UNSUPPORTED_TYPE);
        return SDC_SD_ERR;
    }

    return SDC_SD_SUCCESS;
} /* End of function r_sdc_sdio_chk_support_type() */

#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */

/* End of File */
