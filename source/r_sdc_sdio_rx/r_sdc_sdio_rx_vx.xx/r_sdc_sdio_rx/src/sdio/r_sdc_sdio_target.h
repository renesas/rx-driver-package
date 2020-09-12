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
* Copyright (C) 2016(2017) Renesas Electronics Corporation. All rights reserved.      
**********************************************************************************************************************/

/**********************************************************************************************************************
* File Name    : r_sdc_sdio_target.h
* Device       : RX
* Tool-Chain   : RX Family C Compiler
* H/W Platform : -
* Description  : Interface file for SDC SD API for RX
**********************************************************************************************************************/
/**********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 06.01.2016 1.20    First Release.
*              : 30.09.2016 1.21    Added #define SDIO_IO_CHK_xxxx.
*              :                    Changed the value of SDC_SDIO_TPLFE_FN2_BLK_SIZE.
*              : 31.07.2017 2.00    SDHI FIT module separated into hardware low level layer and middleware layer.
*              :                    Changed prefix from SDHI to SDC_SD.
**********************************************************************************************************************/
#ifndef SDHI_SDIO_TARGET_H
#define SDHI_SDIO_TARGET_H

/**********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
**********************************************************************************************************************/

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/
#define SDC_SDIO_FUNC_MAXNUM            (0x02)   /* For TypeZX */
#define SDC_SDIO_REG_SIZE               (0x14l)  /* CCCR, FBR  : 000h - 013h     */
#define SDIO_IO_CHK_NUM                 (0x10)
#define SDIO_IO_CHK_CODE                (0xd724)
#define SDIO_IO_CHK_ADR0                (0x00001038)
#define SDIO_IO_CHK_ADR1                (0x00001070)
#define SDIO_IO_CHK_ADR2                (0x000010e0)
#define SDIO_IO_CHK_ADR3                (0x000011c0)

/* Definition of the TPLFE block size
   According to SDIO specifications, the maximum block size is 2048 (0x800).
   But the maximum block size is 512 (0x200) in the case of SDIO driver.
   Because, this is limit of SDSIZE register. */
#define SDC_SDIO_TPLFE_FN0_BLK_SIZE     (0x0020)    /* 32 Bytes : The maximum block size and byte count that 
                                                       function 0 can support. */
#define SDC_SDIO_TPLFE_FN1_BLK_SIZE     (0x0040)    /* 64 Bytes : The maximum block size and byte count that 
                                                       function 1 can support. */
#define SDC_SDIO_TPLFE_FN2_BLK_SIZE     (0x0200)    /* 512 Bytes: The maximum block size and byte count that 
                                                       function 2 can support. */
#define SDC_SDIO_TPLFE_FN3_BLK_SIZE     (0xFFFF)    /* Not support : The maximum block size and byte count that 
                                                       function 3 cannot support. */
#define SDC_SDIO_TPLFE_FN4_BLK_SIZE     (0xFFFF)    /* Not support : The maximum block size and byte count that 
                                                       function 4 cannot support. */
#define SDC_SDIO_TPLFE_FN5_BLK_SIZE     (0xFFFF)    /* Not support : The maximum block size and byte count that 
                                                       function 5 cannot support. */
#define SDC_SDIO_TPLFE_FN6_BLK_SIZE     (0xFFFF)    /* Not support : The maximum block size and byte count that 
                                                       function 6 cannot support. */
#define SDC_SDIO_TPLFE_FN7_BLK_SIZE     (0xFFFF)    /* Not support : The maximum block size and byte count that 
                                                       function 7 cannot support. */

/**********************************************************************************************************************
Exported global functions (to be accessed by other files)
**********************************************************************************************************************/

#endif /* SDHI_SDIO_TARGET_H */

