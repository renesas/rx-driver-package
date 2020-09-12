/***********************************************************************************************************************
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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name     : r_datfrx_rx_config_reference.h
* Description   : Configuration options for the r_datfrx_rx module.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 28.09.2018 2.00     First Release
***********************************************************************************************************************/
#ifndef R_DATFRX_CONFIG_H
#define R_DATFRX_CONFIG_H

/************************************************************************************************
CODE FLASH AND DATA FLASH : SET FRDYI INTERRUPT PRIORITY
*************************************************************************************************/
/* Set interrupt priority if use BGO of the Code Flash or the Data Flash.
   The setable value is from 1 to 15. */
#define FLASH_DM_CFG_FRDYI_INT_PRIORITY  (1)

/************************************************************************************************
DATA FLASH : SET THE BLOCK ADDRESS NUMBER
*************************************************************************************************/
/* Please set the block address number to use by the data flash.
   FLASH_TYPE1 = The setable value is from 2 to 8.
   FLASH_TYPE2,3,4 = The setable vlue is from 2 to 1024.
   The block address to use is from 0x00100000 to 0x00101FFF. (up to 8 blocks) */
#define FLASH_DM_CFG_DF_BLOCK_NUM   (4)

/************************************************************************************************
DATA FLASH : SET THE DATA NUMBER OF THE DATA NUMBER
*************************************************************************************************/
/* Please set the data number to use by the data flash.
   The setable value is from 1 to 255. */
#define FLASH_DM_CFG_DF_DATA_NUM    (10)

/************************************************************************************************
DATA FLASH : SET THE DATA LENGTH FOR THE DATA NUMBER
*************************************************************************************************/
/* Please set the data size of the data number to use by the data flash.
   FLASH_TYPE1 = The setable value is from 0 to 255.
   FLASH_TYPE2,3,4 = The setable vlue is from 0 to 65535.
   The data size of the data number not to use is ignored.
   This number must be a multiple of the minimum programming size for memory area you are writing to.
   1 bytes (e.g. RX110, RX111, RX113, RX230, RX231) */
#define FLASH_DM_CFG_DF_SIZE_NO0    (1)
#define FLASH_DM_CFG_DF_SIZE_NO1    (1)
#define FLASH_DM_CFG_DF_SIZE_NO2    (1)
#define FLASH_DM_CFG_DF_SIZE_NO3    (1)
#define FLASH_DM_CFG_DF_SIZE_NO4    (1)
#define FLASH_DM_CFG_DF_SIZE_NO5    (1)
#define FLASH_DM_CFG_DF_SIZE_NO6    (1)
#define FLASH_DM_CFG_DF_SIZE_NO7    (1)
#define FLASH_DM_CFG_DF_SIZE_NO8    (1)
#define FLASH_DM_CFG_DF_SIZE_NO9    (1)
#define FLASH_DM_CFG_DF_SIZE_NO10   (1)
#define FLASH_DM_CFG_DF_SIZE_NO11   (1)
#define FLASH_DM_CFG_DF_SIZE_NO12   (1)
#define FLASH_DM_CFG_DF_SIZE_NO13   (1)
#define FLASH_DM_CFG_DF_SIZE_NO14   (1)
#define FLASH_DM_CFG_DF_SIZE_NO15   (1)
#define FLASH_DM_CFG_DF_SIZE_NO16   (1)
#define FLASH_DM_CFG_DF_SIZE_NO17   (1)
#define FLASH_DM_CFG_DF_SIZE_NO18   (1)
#define FLASH_DM_CFG_DF_SIZE_NO19   (1)
#define FLASH_DM_CFG_DF_SIZE_NO20   (1)
#define FLASH_DM_CFG_DF_SIZE_NO21   (1)
#define FLASH_DM_CFG_DF_SIZE_NO22   (1)
#define FLASH_DM_CFG_DF_SIZE_NO23   (1)
#define FLASH_DM_CFG_DF_SIZE_NO24   (1)
#define FLASH_DM_CFG_DF_SIZE_NO25   (1)
#define FLASH_DM_CFG_DF_SIZE_NO26   (1)
#define FLASH_DM_CFG_DF_SIZE_NO27   (1)
#define FLASH_DM_CFG_DF_SIZE_NO28   (1)
#define FLASH_DM_CFG_DF_SIZE_NO29   (1)
#define FLASH_DM_CFG_DF_SIZE_NO30   (1)
#define FLASH_DM_CFG_DF_SIZE_NO31   (1)
#define FLASH_DM_CFG_DF_SIZE_NO32   (1)
#define FLASH_DM_CFG_DF_SIZE_NO33   (1)
#define FLASH_DM_CFG_DF_SIZE_NO34   (1)
#define FLASH_DM_CFG_DF_SIZE_NO35   (1)
#define FLASH_DM_CFG_DF_SIZE_NO36   (1)
#define FLASH_DM_CFG_DF_SIZE_NO37   (1)
#define FLASH_DM_CFG_DF_SIZE_NO38   (1)
#define FLASH_DM_CFG_DF_SIZE_NO39   (1)

/************************************************************************************************
DATA FLASH : SET CRC HARDWARE
*************************************************************************************************/
/* Set CRC calculation.
   HARDWARE 1.
   SOFTWARE 0. */
#define FLASH_DM_CFG_CRC_HARDWARE   (0)

#endif /* R_DATFRX_CONFIG_H */

/* End of File */
