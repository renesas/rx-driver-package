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
* File Name    : r_dsmif_rx.c
* Version      : 1.0
* Abstract     : API of DSMIF commands
* Tool-Chain   : Renesas CCRX
* OS           : not use
* Description  : DSMIF commands API for RX72M
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 06.08.2019 1.0      First Release
*******************************************************************************/

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "platform.h"
#include "r_cmt_rx_if.h"
#include "r_dsmif_rx_if.h"
#include "r_dsmif_rx_config.h"
#include "r_dsmif_rx_private.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/

/*******************************************************************************
Typedef definitions
*******************************************************************************/

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/

/*******************************************************************************
Private variables and functions
*******************************************************************************/
static st_dsmif_filter_cfg_t s_filter_cfg[DSMIF_UNIT_SIZE];
static uint8_t s_filter_no[DSMIF_UNIT_SIZE];

static void dsmif_api_init(st_dsmif_config_t* config);
static void wait_for_setting(void);

static volatile bool s_wait_for_setting_flag = false;
static void wait_for_setting_callback(void* channel);

/*******************************************************************************
* Function Name: R_DSMIF_Create
* Description  : Initialize DSMIF
* Arguments    : config
*                    Configuration information pointer of DSMIF
* Return Value : none
*******************************************************************************/
void R_DSMIF_Create(st_dsmif_config_t* config)
{
    /* Initialize static variables */
    dsmif_api_init(config);

    /* Initialize DSMIF */
    dsmif_init(config);
}
/*******************************************************************************
End of function R_DSMIF_Create
*******************************************************************************/

/*******************************************************************************
* Function Name: R_DSMIF_Start
* Description  : Start DSMIF filtration
* Arguments    : unit_no
* Return Value : none
*******************************************************************************/
void R_DSMIF_Start(uint32_t unit_no)
{
    /* Set upper and lower limit */
    dsmif_set_limit(unit_no, s_filter_no[unit_no]);

    /* start DSMIF filtration */
    dsmif_start(unit_no);

    /* Wait until filter data becomes stable */
    wait_for_setting();

    /* Enable interrupt for error detection */
    dsmif_enable_error_int(unit_no);
}
/*******************************************************************************
End of function R_DSMIF_Start
*******************************************************************************/

/*******************************************************************************
* Function Name: R_DSMIF_Stop
* Description  : Stop DSMIF filtration
* Arguments    : unit_no
* Return Value : none
*******************************************************************************/
void R_DSMIF_Stop(uint32_t unit_no)
{
    /* stop DSMIF filtration */
    dsmif_stop(unit_no);

    /* Disables interrupt factor */
    dsmif_disable_error_int(unit_no);
}
/*******************************************************************************
End of function R_DSMIF_Stop
*******************************************************************************/

/*******************************************************************************
* Function Name: R_DSMIF_SetFilter
* Description  : configure filter setting
* Arguments    : unit_no
*                filter_setting
*                    filter setting
* Return Value : none
*******************************************************************************/
void R_DSMIF_SetFilter(uint32_t unit_no, uint8_t filter_setting)
{
    s_filter_no[unit_no] = filter_setting;

    /* filter setting */
    switch(filter_setting)
    {
    case DSMIF_FILTER_SET_0:
        s_filter_cfg[unit_no].sinc = DSMIF_SINC_3;
        s_filter_cfg[unit_no].deci = DSMIF_DIV_MCLK_8;
        s_filter_cfg[unit_no].shift = DSMIF_BITSHIFT_8;
        break;
    case DSMIF_FILTER_SET_1:
        s_filter_cfg[unit_no].sinc = DSMIF_SINC_3;
        s_filter_cfg[unit_no].deci = DSMIF_DIV_MCLK_16;
        s_filter_cfg[unit_no].shift = DSMIF_BITSHIFT_6;
        break;
    case DSMIF_FILTER_SET_2:
        s_filter_cfg[unit_no].sinc = DSMIF_SINC_3;
        s_filter_cfg[unit_no].deci = DSMIF_DIV_MCLK_32;
        s_filter_cfg[unit_no].shift = DSMIF_BITSHIFT_4;
        break;
    case DSMIF_FILTER_SET_3:
        s_filter_cfg[unit_no].sinc = DSMIF_SINC_3;
        s_filter_cfg[unit_no].deci = DSMIF_DIV_MCLK_64;
        s_filter_cfg[unit_no].shift = DSMIF_BITSHIFT_2;
        break;
    case DSMIF_FILTER_SET_4:
        s_filter_cfg[unit_no].sinc = DSMIF_SINC_3;
        s_filter_cfg[unit_no].deci = DSMIF_DIV_MCLK_128;
        s_filter_cfg[unit_no].shift = DSMIF_BITSHIFT_1;
        break;
    case DSMIF_FILTER_SET_5:
        s_filter_cfg[unit_no].sinc = DSMIF_SINC_3;
        s_filter_cfg[unit_no].deci = DSMIF_DIV_MCLK_256;
        s_filter_cfg[unit_no].shift = DSMIF_BITSHIFT_0;
        break;
    case DSMIF_FILTER_SET_6:
        s_filter_cfg[unit_no].sinc = DSMIF_SINC_2;
        s_filter_cfg[unit_no].deci = DSMIF_DIV_MCLK_16;
        s_filter_cfg[unit_no].shift = DSMIF_BITSHIFT_9;
        break;
    case DSMIF_FILTER_SET_7:
        s_filter_cfg[unit_no].sinc = DSMIF_SINC_2;
        s_filter_cfg[unit_no].deci = DSMIF_DIV_MCLK_32;
        s_filter_cfg[unit_no].shift = DSMIF_BITSHIFT_7;
        break;
    case DSMIF_FILTER_SET_8:
        s_filter_cfg[unit_no].sinc = DSMIF_SINC_2;
        s_filter_cfg[unit_no].deci = DSMIF_DIV_MCLK_64;
        s_filter_cfg[unit_no].shift = DSMIF_BITSHIFT_6;
        break;
    case DSMIF_FILTER_SET_9:
        s_filter_cfg[unit_no].sinc = DSMIF_SINC_2;
        s_filter_cfg[unit_no].deci = DSMIF_DIV_MCLK_128;
        s_filter_cfg[unit_no].shift = DSMIF_BITSHIFT_5;
        break;
    case DSMIF_FILTER_SET_10:
        s_filter_cfg[unit_no].sinc = DSMIF_SINC_2;
        s_filter_cfg[unit_no].deci = DSMIF_DIV_MCLK_256;
        s_filter_cfg[unit_no].shift = DSMIF_BITSHIFT_3;
        break;
    case DSMIF_FILTER_SET_11:
        s_filter_cfg[unit_no].sinc = DSMIF_SINC_1;
        s_filter_cfg[unit_no].deci = DSMIF_DIV_MCLK_32;
        s_filter_cfg[unit_no].shift = DSMIF_BITSHIFT_12;
        break;
    case DSMIF_FILTER_SET_12:
        s_filter_cfg[unit_no].sinc = DSMIF_SINC_1;
        s_filter_cfg[unit_no].deci = DSMIF_DIV_MCLK_64;
        s_filter_cfg[unit_no].shift = DSMIF_BITSHIFT_11;
        break;
    case DSMIF_FILTER_SET_13:
        s_filter_cfg[unit_no].sinc = DSMIF_SINC_1;
        s_filter_cfg[unit_no].deci = DSMIF_DIV_MCLK_128;
        s_filter_cfg[unit_no].shift = DSMIF_BITSHIFT_10;
        break;
    case DSMIF_FILTER_SET_14:
        s_filter_cfg[unit_no].sinc = DSMIF_SINC_1;
        s_filter_cfg[unit_no].deci = DSMIF_DIV_MCLK_256;
        s_filter_cfg[unit_no].shift = DSMIF_BITSHIFT_9;
        break;
    default:

        /* In case of an abnormal parameter, be initialized as No.0. */
        s_filter_cfg[unit_no].sinc = DSMIF_SINC_3;
        s_filter_cfg[unit_no].deci = DSMIF_DIV_MCLK_8;
        s_filter_cfg[unit_no].shift = DSMIF_BITSHIFT_8;
        break;
    }

    /* set filter */
    dsmif_set_filter(unit_no, &s_filter_cfg[unit_no]);
}
/*******************************************************************************
End of function R_DSMIF_SetFilter
*******************************************************************************/

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
uint16_t R_DSMIF_ReadData(uint8_t data_reg)
{
    uint16_t read_data;

    read_data = dsmif_read_data(data_reg);

    return read_data;
}
/*******************************************************************************
End of function R_DSMIF_ReadData
*******************************************************************************/

/*******************************************************************************
* Function Name: R_DSMIF_GetErrorStatus
* Description  : Read error status register
* Arguments    : unit_no
* Return Value : err_status
*******************************************************************************/
uint32_t R_DSMIF_GetErrorStatus(uint32_t unit_no)
{
    uint32_t err_status;

    err_status = dsmif_read_error_status(unit_no);

    return err_status;
}
/*******************************************************************************
End of function R_DSMIF_GetErrorStatus
*******************************************************************************/

/*******************************************************************************
* Function Name: dsmif_api_init
* Description  : Initialize static variables
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void dsmif_api_init(st_dsmif_config_t* config)
{
    uint32_t unit_no = config->unit_no;

	/* Cast to void* */
    memset((void *)&s_filter_cfg[unit_no], 0, sizeof(s_filter_cfg));
    s_filter_no[unit_no] = 0u;
}
/*******************************************************************************
 End of function dsmif_api_init
*******************************************************************************/

/*******************************************************************************
* Function Name: wait_for_setting
* Description  : wait for filter setting time
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void wait_for_setting(void)
{
    bool retval;
    uint32_t cmt_ch;

    retval = R_CMT_CreateOneShot(DSMIF_SETTING_WAIT_CNT, wait_for_setting_callback, &cmt_ch);

    if (false != retval)
    {
        while (1)
        {
            if (false != s_wait_for_setting_flag)
            {
                s_wait_for_setting_flag = false;
                break;
            }
        }
    }

    R_CMT_Stop(cmt_ch);
}
/*******************************************************************************
 End of function wait_for_setting
*******************************************************************************/

/*******************************************************************************
* Function Name: wait_for_setting_callback
* Description  : CMT timeout callback
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void wait_for_setting_callback(void* channel)
{
    s_wait_for_setting_flag = true;
}
/*******************************************************************************
 End of function wait_for_setting_callback
*******************************************************************************/

/* End of File */
