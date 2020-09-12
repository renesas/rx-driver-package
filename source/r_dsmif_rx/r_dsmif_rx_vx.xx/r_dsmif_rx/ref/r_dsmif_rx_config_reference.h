/* Generated configuration header file - do not edit */
/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : r_dsmif_rx_config.h
* Description   : Configures the DSMIF module
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 06.08.2019 1.0      First Release
*******************************************************************************/

#ifndef R_DSMIF_CONFIG_HEADER
#define R_DSMIF_CONFIG_HEADER

/*******************************************************************************
Configuration Options
*******************************************************************************/

/* Error interrupt handler priority */
#define DSMIF_ERR_INTR_PRI_LVL  (0x0FU) /* Level 15 (highest) */

/* Wait count for filter setting */
#define DSMIF_SETTING_WAIT_CNT  (100000u)

/* Over current detection low threshold value */
#define DSMIF_OCLTR_VALUE       (1u)

/* Short-circuit detection high threshold value */
#define DSMIF_SHORT_CNT_1       (0x00001FFEu)

/* Short-circuit detection low threshold value */
#define DSMIF_SHORT_CNT_0       (0x00001FFEu)

#endif /* R_DSMIF_CONFIG_HEADER */

/* End of File */
