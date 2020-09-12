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
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* System Name  : RX72M DSMIF sample program
* File Name    : r_dsmif_rx_private.h
* Version      : 1.0
* Abstract     : Definition for DSMIF
* Tool-Chain   : Renesas CCRX
* OS           : not use
* Description  : DSMIF setting for RX72M
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 25.03.2019 1.0      First Release
*******************************************************************************/

#ifndef R_DSMIF_RX_PRIVATE_H
#define R_DSMIF_RX_PRIVATE_H

/*******************************************************************************
Macro definitions
*******************************************************************************/

/* UNIT array for interrnal storage */
#define DSMIF_UNIT_SIZE                         (2U)

/* DSMIF MCLK Divide Setting(Master mode) */
#define DSMIF_CKDIV_2                           (0U)
#define DSMIF_CKDIV_4                           (1U)
#define DSMIF_CKDIV_8                           (2U)
#define DSMIF_CKDIV_16                          (3U)
#define DSMIF_CKDIV_32                          (4U)
#define DSMIF_CKDIV_64                          (5U)
#define DSMIF_CKDIV_128                         (6U)
#define DSMIF_CKDIV_256                         (7U)
#define DSMIF_CKDIV_512                         (8U)
#define DSMIF_CKDIV_1024                        (9U)
#define DSMIF_CKDIV_2048                        (10U)
#define DSMIF_CKDIV_4096                        (11U)
#define DSMIF_CKDIV_8192                        (12U)
#define DSMIF_CKDIV_16384                       (13U)
#define DSMIF_CKDIV_32768                       (14U)
#define DSMIF_CKDIV_65536                       (15U)

/* DSMIF Clear Error Setting -------------------------------------------------*/
#define DSMIF_CLEAR_ERR_ALL                     (0x00000177U)



/* DSMIF Sinc Filter Setting -------------------------------------------------*/
#define DSMIF_SINC_1                            (1U)
#define DSMIF_SINC_2                            (2U)
#define DSMIF_SINC_3                            (0U)

/* DSMIF Decimation Clock Setting --------------------------------------------*/
#define DSMIF_DIV_MCLK_8                        (2U)
#define DSMIF_DIV_MCLK_16                       (3U)
#define DSMIF_DIV_MCLK_32                       (4U)
#define DSMIF_DIV_MCLK_64                       (5U)
#define DSMIF_DIV_MCLK_128                      (6U)
#define DSMIF_DIV_MCLK_256                      (7U)

/* DSMIF Bit Shift Setting ---------------------------------------------------*/ 
#define DSMIF_BITSHIFT_0                        (0U)
#define DSMIF_BITSHIFT_1                        (1U)
#define DSMIF_BITSHIFT_2                        (2U)
#define DSMIF_BITSHIFT_3                        (3U)
#define DSMIF_BITSHIFT_4                        (4U)
#define DSMIF_BITSHIFT_5                        (5U)
#define DSMIF_BITSHIFT_6                        (6U)
#define DSMIF_BITSHIFT_7                        (7U)
#define DSMIF_BITSHIFT_8                        (8U)
#define DSMIF_BITSHIFT_9                        (9U)
#define DSMIF_BITSHIFT_10                       (10U)
#define DSMIF_BITSHIFT_11                       (11U)
#define DSMIF_BITSHIFT_12                       (12U)

/* DSMIF Upper Limit ---------------------------------------------------------*/
#define DSMIF_UPPER_LIMIT_5                     (0xF800U)
#define DSMIF_UPPER_LIMIT_6                     (0xFC00U)
#define DSMIF_UPPER_LIMIT_7                     (0xFE00U)
#define DSMIF_UPPER_LIMIT_8                     (0xFF00U)
#define DSMIF_UPPER_LIMIT_9                     (0xFF80U)
#define DSMIF_UPPER_LIMIT_10                    (0xFFC0U)
#define DSMIF_UPPER_LIMIT_12                    (0xFFF0U)
#define DSMIF_UPPER_LIMIT_14                    (0xFFFCU)
#define DSMIF_UPPER_LIMIT_15                    (0xFFFEU)
#define DSMIF_UPPER_LIMIT_16                    (0xFFFFU)

/* DSMIF Error Limit Values --------------------------------------------------*/
#define DSMIF_ERROR_OFFSET      (1u)

/* MPC Definition for DSMIF --------------------------------------------------*/
#define MPC_P33PFS_PSEL_MCLK0                   (0x29u)
#define MPC_P34PFS_PSEL_MDAT0                   (0x29u)
#define MPC_P83PFS_PSEL_MCLK1                   (0x29u)
#define MPC_P56PFS_PSEL_MDAT1                   (0x29u)
#define MPC_P74PFS_PSEL_MCLK2                   (0x29u)
#define MPC_P75PFS_PSEL_MDAT2                   (0x29u)
#define MPC_P71PFS_PSEL_MCLK3                   (0x29u)
#define MPC_P72PFS_PSEL_MDAT3                   (0x29u)
#define MPC_P92PFS_PSEL_MCLK4                   (0x29u)
#define MPC_P93PFS_PSEL_MDAT4                   (0x29u)
#define MPC_P90PFS_PSEL_MCLK5                   (0x29u)
#define MPC_P91PFS_PSEL_MDAT5                   (0x29u)

/*******************************************************************************
Typedef definitions
*******************************************************************************/

/* DSMIF filter definitions --------------------------------------------------*/
typedef struct
{
    uint8_t sinc;
    uint8_t deci;
    uint8_t shift;
} st_dsmif_filter_cfg_t;

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/

/*******************************************************************************
* Function Name: dsmif_enable_error_int
* Description  : Clear error status of DSMIF and enable BL2 interrupt
* Arguments    : unit_no
* Return Value : none
*******************************************************************************/
void dsmif_enable_error_int(uint32_t unit_no);

/*******************************************************************************
* Function Name: dsmif_disable_error_int
* Description  : Disable error factor of DSMIF
* Arguments    : unit_no
* Return Value : none
*******************************************************************************/
void dsmif_disable_error_int(uint32_t unit_no);

/*******************************************************************************
* Function Name: dsmif_init
* Description  : initialize DSMIF
* Arguments    : config
*                    Configuration information pointer of DSMIF
* Return Value : none
*******************************************************************************/
void dsmif_init(st_dsmif_config_t* config);

/*******************************************************************************
* Function Name: dsmif_start
* Description  : start dsmif filtration
* Arguments    : unit_no
* Return Value : none
*******************************************************************************/
void dsmif_start(uint32_t unit_no);

/*******************************************************************************
* Function Name: dsmif_stop
* Description  : stop dsmif filtration
* Arguments    : unit_no
* Return Value : none
*******************************************************************************/
void dsmif_stop(uint32_t unit_no);

/*******************************************************************************
* Function Name: dsmif_set_filter
* Description  : set filter register
* Arguments    : unit_no
*                p_cfg
*                    pointer to filter setting struct
* Return Value : none
*******************************************************************************/
void dsmif_set_filter(uint32_t unit_no, st_dsmif_filter_cfg_t * p_x1cfg);

/*******************************************************************************
* Function Name: dsmif_read_data
* Description  : read data from DATA register
* Arguments    : data_reg
* Return Value : read_data
*                    current value read from DATA
*******************************************************************************/
uint16_t dsmif_read_data(uint8_t data_reg);

/*******************************************************************************
* Function Name: dsmif_read_error_status
* Description  : read error_status from DSSR register
* Arguments    : unit_no
* Return Value : error_status
*                    current value read from DSSR
*******************************************************************************/
uint32_t dsmif_read_error_status(uint32_t unit_no);

/*******************************************************************************
* Function Name: dsmif_set_limit
* Description  : set limit to OCLTR, OCHTR, SCLTR and SCHTR register
* Arguments    : unit_no
*                filter_no
*                    current filter setting number
* Return Value : none
*******************************************************************************/
void dsmif_set_limit(uint32_t unit_no, uint8_t filter_no);

#endif /* R_DSMIF_RX_PRIVATE_H */

/* End of File */
