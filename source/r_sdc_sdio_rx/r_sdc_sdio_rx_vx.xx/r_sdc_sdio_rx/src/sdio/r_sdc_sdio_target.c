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
* Copyright (C) 2014(2015-2017) Renesas Electronics Corporation. All rights reserved.    
**********************************************************************************************************************/
/**********************************************************************************************************************
* System Name  : SDHI Driver
* File Name    : r_sdc_sdio_target.c
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

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/

/**********************************************************************************************************************
* Outline      : Get Function Max Number Operation
* Function Name: r_sdc_sdio_get_func_max_num
* Description  : Gets function max number.
* Arguments    : void
* Return Value : SDC_SDIO_FUNC_MAXNUM                   ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
uint32_t r_sdc_sdio_get_func_max_num(void)
{
    return (uint32_t)SDC_SDIO_FUNC_MAXNUM;
} /* End of function r_sdc_sdio_get_func_max_num() */

/**********************************************************************************************************************
* Outline      : Get SDIO Register Size Operation
* Function Name: r_sdc_sdio_get_io_reg_size
* Description  : Gets SDIO register size.
* Arguments    : void
* Return Value : SDC_SDIO_REG_SIZE                      ;   Successful operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
int32_t r_sdc_sdio_get_io_reg_size(void)
{
    return (int32_t)SDC_SDIO_REG_SIZE;
} /* End of function r_sdc_sdio_get_io_reg_size() */

/**********************************************************************************************************************
* Outline      : Get SDIO Block Size Operation
* Function Name: r_sdc_sdio_get_block_size
* Description  : Gets SDIO block size.
* Arguments    : uint32_t       func                    ;   Function No, (0 to SDC_SDIO_FUNC_MAXNUM)
* Return Value : Block size of function no.             ;   Successful operation
*              : 0xffffffff                             ;   Failed operation
*----------------------------------------------------------------------------------------------------------------------
* Notes        : None
**********************************************************************************************************************/
int32_t r_sdc_sdio_get_block_size(uint32_t func)
{
    int32_t     block_size = 0;

    switch (func)
    {
        case SDC_SDIO_FNO_0:
            block_size = SDC_SDIO_TPLFE_FN0_BLK_SIZE;
        break;
        case SDC_SDIO_FNO_1:
            block_size = SDC_SDIO_TPLFE_FN1_BLK_SIZE;
        break;
        case SDC_SDIO_FNO_2:
            block_size = SDC_SDIO_TPLFE_FN2_BLK_SIZE;
        break;
        case SDC_SDIO_FNO_3:
            block_size = SDC_SDIO_TPLFE_FN3_BLK_SIZE;
        break;
        case SDC_SDIO_FNO_4:
            block_size = SDC_SDIO_TPLFE_FN4_BLK_SIZE;
        break;
        case SDC_SDIO_FNO_5:
            block_size = SDC_SDIO_TPLFE_FN5_BLK_SIZE;
        break;
        case SDC_SDIO_FNO_6:
            block_size = SDC_SDIO_TPLFE_FN6_BLK_SIZE;
        break;
        case SDC_SDIO_FNO_7:
            block_size = SDC_SDIO_TPLFE_FN7_BLK_SIZE;
        break;
        default:
            block_size = 0xffffffff;
        break;
    }

    return block_size;
} /* End of function r_sdc_sdio_get_block_size() */
#endif /* (SDC_SD_CFG_DRIVER_MODE & SDC_SD_MODE_IO) */

/* End of File */
