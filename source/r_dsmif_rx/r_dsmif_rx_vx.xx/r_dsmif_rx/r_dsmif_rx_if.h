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
* System Name  : RX72M DSMIF sample program
* File Name    : r_dsmif_rx_if.h
* Version      : 1.0
* Abstract     : API for DSMIF function
* Tool-Chain   : Renesas CCRX
* OS           : not use
* Description  : DSMIF setting API for RX72M
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 06.08.2019 1.0      First Release
*******************************************************************************/

#ifndef R_DSMIF_RX_IF_H
#define R_DSMIF_RX_IF_H

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* DSMIF Unit No */
#define DSMIF_UNIT_DSMIF0                       (0U)
#define DSMIF_UNIT_DSMIF1                       (1U)

/* DSMIF Mode Setting */
#define DSMIF_MODE_SLAVE                        (0U)
#define DSMIF_MODE_MASTER                       (1U)

/* DSMIF ErrorDisable No */
#define DSMIF_CH0                               (1U)
#define DSMIF_CH1                               (2U)
#define DSMIF_CH2                               (4U)
#define DSMIF_SUM                               (8U)

/* DSMIF Filter Setting Code -------------------------------------------------*/
#define DSMIF_FILTER_SET_0                      (0U)
#define DSMIF_FILTER_SET_1                      (1U)
#define DSMIF_FILTER_SET_2                      (2U)
#define DSMIF_FILTER_SET_3                      (3U)
#define DSMIF_FILTER_SET_4                      (4U)
#define DSMIF_FILTER_SET_5                      (5U)
#define DSMIF_FILTER_SET_6                      (6U)
#define DSMIF_FILTER_SET_7                      (7U)
#define DSMIF_FILTER_SET_8                      (8U)
#define DSMIF_FILTER_SET_9                      (9U)
#define DSMIF_FILTER_SET_10                     (10U)
#define DSMIF_FILTER_SET_11                     (11U)
#define DSMIF_FILTER_SET_12                     (12U)
#define DSMIF_FILTER_SET_13                     (13U)
#define DSMIF_FILTER_SET_14                     (14U)

/* DSMIF Target Register to Read ---------------------------------------------*/
#define DSMIF_READ_DSMIF0_CDR0                  (1U)
#define DSMIF_READ_DSMIF0_CDR1                  (2U)
#define DSMIF_READ_DSMIF0_CDR2                  (3U)
#define DSMIF_READ_DSMIF1_CDR0                  (4U)
#define DSMIF_READ_DSMIF1_CDR1                  (5U)
#define DSMIF_READ_DSMIF1_CDR2                  (6U)
#define DSMIF_READ_DSMIF0_CCDR0                 (7U)
#define DSMIF_READ_DSMIF0_CCDR1                 (8U)
#define DSMIF_READ_DSMIF0_CCDR2                 (9U)
#define DSMIF_READ_DSMIF1_CCDR0                 (10U)
#define DSMIF_READ_DSMIF1_CCDR1                 (11U)
#define DSMIF_READ_DSMIF1_CCDR2                 (12U)
#define DSMIF_READ_DSMIF0_TCDR0                 (13U)
#define DSMIF_READ_DSMIF0_TCDR1                 (14U)
#define DSMIF_READ_DSMIF0_TCDR2                 (15U)
#define DSMIF_READ_DSMIF1_TCDR0                 (16U)
#define DSMIF_READ_DSMIF1_TCDR1                 (17U)
#define DSMIF_READ_DSMIF1_TCDR2                 (18U)
#define DSMIF_READ_DSMIF0_OCDR0                 (19U)
#define DSMIF_READ_DSMIF0_OCDR1                 (20U)
#define DSMIF_READ_DSMIF0_OCDR2                 (21U)
#define DSMIF_READ_DSMIF1_OCDR0                 (22U)
#define DSMIF_READ_DSMIF1_OCDR1                 (23U)
#define DSMIF_READ_DSMIF1_OCDR2                 (24U)

/*******************************************************************************
Typedef definitions
*******************************************************************************/
/* DSMIF unit configurations--------------------------------------------------*/
typedef struct
{
    uint32_t unit_no;
    uint32_t mode;
    uint32_t ckdiv;
    uint32_t dis_error;
    void (*p_callback)(uint32_t intfactor);
} st_dsmif_config_t;

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/

/*******************************************************************************
* Function Name: R_DSMIF_Create
* Description  : Initialize DSMIF
* Arguments    : config
*                    Configuration information pointer of DSMIF
* Return Value : none
*******************************************************************************/
void R_DSMIF_Create(st_dsmif_config_t* config);

/*******************************************************************************
* Function Name: R_DSMIF_Start
* Description  : Start DSMIF filtration
* Arguments    : unit_no
* Return Value : none
*******************************************************************************/
void R_DSMIF_Start(uint32_t unit_no);

/*******************************************************************************
* Function Name: R_DSMIF_Stop
* Description  : Stop DSMIF filtration
* Arguments    : unit_no
* Return Value : none
*******************************************************************************/
void R_DSMIF_Stop(uint32_t unit_no);

/*******************************************************************************
* Function Name: R_DSMIF_SetFilter
* Description  : configure filter setting
* Arguments    : unit_no
*                filter_setting
*                    filter setting
* Return Value : none
*******************************************************************************/
void R_DSMIF_SetFilter(uint32_t unit_no, uint8_t filter_setting);

/*******************************************************************************
* Function Name: R_DSMIF_ReadData
* Description  : read data from specified register
* Arguments    : data_reg
*                   specify from the following register names
*                   DSMIF_READ_DSMIF0_CDR0
*                   DSMIF_READ_DSMIF0_CDR1
*                   DSMIF_READ_DSMIF0_CDR2
*                   DSMIF_READ_DSMIF1_CDR0
*                   DSMIF_READ_DSMIF1_CDR1
*                   DSMIF_READ_DSMIF1_CDR2
*                   DSMIF_READ_DSMIF0_CCDR0
*                   DSMIF_READ_DSMIF0_CCDR1
*                   DSMIF_READ_DSMIF0_CCDR2
*                   DSMIF_READ_DSMIF1_CCDR0
*                   DSMIF_READ_DSMIF1_CCDR1
*                   DSMIF_READ_DSMIF1_CCDR2
*                   DSMIF_READ_DSMIF0_TCDR0
*                   DSMIF_READ_DSMIF0_TCDR1
*                   DSMIF_READ_DSMIF0_TCDR2
*                   DSMIF_READ_DSMIF1_TCDR0
*                   DSMIF_READ_DSMIF1_TCDR1
*                   DSMIF_READ_DSMIF1_TCDR2
*                   DSMIF_READ_DSMIF0_OCDR0
*                   DSMIF_READ_DSMIF0_OCDR1
*                   DSMIF_READ_DSMIF0_OCDR2
*                   DSMIF_READ_DSMIF1_OCDR0
*                   DSMIF_READ_DSMIF1_OCDR1
*                   DSMIF_READ_DSMIF1_OCDR2
*
* Return Value : read_data
*                    current value read from specified register
*******************************************************************************/
uint16_t R_DSMIF_ReadData(uint8_t data_reg);

/*******************************************************************************
* Function Name: R_DSMIF_GetErrorStatus
* Description  : Read error status register
* Arguments    : unit_no
* Return Value : err_status
*******************************************************************************/
uint32_t R_DSMIF_GetErrorStatus(uint32_t unit_no);

#endif /* R_DSMIF_RX_IF_H */

/* End of File */
