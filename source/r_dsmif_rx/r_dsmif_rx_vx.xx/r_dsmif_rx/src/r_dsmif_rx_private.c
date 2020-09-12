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
* File Name    : r_dsmif_private.c
* Version      : 1.0
* Abstract     : driver for DSMIF
* Tool-Chain   : Renesas CCRX
* OS           : not use
* Description  : DSMIF driver for RX72M
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
#include "platform.h"
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

static volatile struct st_dsmif0 R_BSP_EVENACCESS_SFR *sp_dsmif[2] =
{
    /* DSMIF Units */
    &DSMIF0, &DSMIF1
};

static st_dsmif_config_t s_dsmif_config[DSMIF_UNIT_SIZE];

static void dsmif_set_diserr(st_dsmif_config_t* config);
static void set_icu_interrupt_handler(void);

static void dsmif0_ocdi0_inthandler(void);
static void dsmif0_scdi0_inthandler(void);
static void dsmif0_sumei0_inthandler(void);
static void dsmif1_ocdi1_inthandler(void);
static void dsmif1_scdi1_inthandler(void);
static void dsmif1_sumei1_inthandler(void);

/*******************************************************************************
* Function Name: dsmif_enable_error_int
* Description  : Clear error status of DSMIF and enable BL2 interrupt
* Arguments    : unit_no
* Return Value : none
*******************************************************************************/
void dsmif_enable_error_int(uint32_t unit_no)
{
    volatile uint32_t dummy32;
    uint32_t dis_error = s_dsmif_config[unit_no].dis_error;

    /* Clear DSMSF error status */
    dummy32 = sp_dsmif[unit_no]->DSSR.LONG;
    sp_dsmif[unit_no]->DSSR.LONG = DSMIF_CLEAR_ERR_ALL;

    /* Enable error interrupt */
    if (DSMIF_UNIT_DSMIF0 == unit_no)
    {
        if ((DSMIF_CH0 | DSMIF_CH1 | DSMIF_CH2) != (dis_error & (DSMIF_CH0 | DSMIF_CH1 | DSMIF_CH2)))
        {
        	/* Cast to unsigned long is valid */
        	ICU.GENBL2.BIT.EN1 = 1u;

        	/* Cast to unsigned long is valid */
        	ICU.GENBL2.BIT.EN3 = 1u;
        }
        if (0 == (dis_error & DSMIF_SUM))
        {
        	/* Cast to unsigned long is valid */
        	ICU.GENBL2.BIT.EN2 = 1u;
        }
    }
    if (DSMIF_UNIT_DSMIF1 == unit_no)
    {
        if ((DSMIF_CH0 | DSMIF_CH1 | DSMIF_CH2) != (dis_error & (DSMIF_CH0 | DSMIF_CH1 | DSMIF_CH2)))
        {
        	/* Cast to unsigned long is valid */
        	ICU.GENBL2.BIT.EN4 = 1u;

        	/* Cast to unsigned long is valid */
        	ICU.GENBL2.BIT.EN6 = 1u;
        }
        if (0 == (dis_error & DSMIF_SUM))
        {
        	/* Cast to unsigned long is valid */
        	ICU.GENBL2.BIT.EN5 = 1u;
        }
    }

    /* Disable group BL2 interrupt*/
    IEN(ICU,GROUPBL2) = 0u;

    /* Set group BL2 interrupt priority level */
    IPR(ICU,GROUPBL2) = DSMIF_ERR_INTR_PRI_LVL;

    /* Enable group BL2 interrupt */
    IEN(ICU,GROUPBL2) = 1u;
}
/*******************************************************************************
End of function dsmif_enable_error_int
*******************************************************************************/

/*******************************************************************************
* Function Name: dsmif_disable_error_int
* Description  : Disable error factor of DSMIF
* Arguments    : unit_no
* Return Value : none
*******************************************************************************/
void dsmif_disable_error_int(uint32_t unit_no)
{
    /* Clear DSMSF error status */
    sp_dsmif[unit_no]->DSSR.LONG = DSMIF_CLEAR_ERR_ALL;

    /* Disable error interrupt */
    if (DSMIF_UNIT_DSMIF0 == unit_no)
    {
    	/* Cast to unsigned long is valid */
    	ICU.GENBL2.BIT.EN1 = 0U;

    	/* Cast to unsigned long is valid */
    	ICU.GENBL2.BIT.EN2 = 0U;

    	/* Cast to unsigned long is valid */
    	ICU.GENBL2.BIT.EN3 = 0U;
    }
    if (DSMIF_UNIT_DSMIF1 == unit_no)
    {
    	/* Cast to unsigned long is valid */
    	ICU.GENBL2.BIT.EN4 = 0U;

    	/* Cast to unsigned long is valid */
    	ICU.GENBL2.BIT.EN5 = 0U;

    	/* Cast to unsigned long is valid */
    	ICU.GENBL2.BIT.EN6 = 0U;
    }
}
/*******************************************************************************
End of function dsmif_disable_error_int
*******************************************************************************/

/*******************************************************************************
* Function Name: dsmif_init
* Description  : initialize DSMIF
* Arguments    : config
*                    Configuration information pointer of DSMIF
* Return Value : none
*******************************************************************************/
void dsmif_init(st_dsmif_config_t* config)
{
    uint32_t unit_no = config->unit_no;

    /* Copy config value to driver variable */
    s_dsmif_config[unit_no].unit_no    = config->unit_no;
    s_dsmif_config[unit_no].mode       = config->mode;
    s_dsmif_config[unit_no].ckdiv      = config->ckdiv;
    s_dsmif_config[unit_no].dis_error  = config->dis_error;
    s_dsmif_config[unit_no].p_callback = config->p_callback;

    /* Enable writing to registers related to operating modes, LPC, CGC and software reset */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Release DSMIF from the module-stop state */
    MSTP(DSMIF) = 0u;

    /* Enable protection */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* stop DSMIF filtration */
    sp_dsmif[unit_no]->DSCR.BIT.FEN = 0u;

    /* clock direction select */
    sp_dsmif[unit_no]->DSCR.BIT.CKDIR = config->mode;

    /* master clock division select */
    if (DSMIF_MODE_MASTER == config->mode)
    {
        sp_dsmif[unit_no]->DSCR.BIT.CKDIV = config->ckdiv;
    }
    else
    {
        sp_dsmif[unit_no]->DSCR.BIT.CKDIV = 0u;
    }

    /* Initialize filter setting */
    sp_dsmif[unit_no]->DSCR.BIT.CMSINC = DSMIF_SINC_3;
    sp_dsmif[unit_no]->DSCR.BIT.CMDEC = DSMIF_DIV_MCLK_8;
    sp_dsmif[unit_no]->DSCR.BIT.CMSH = DSMIF_BITSHIFT_8;
    sp_dsmif[unit_no]->DSCR.BIT.OCSINC = DSMIF_SINC_3;
    sp_dsmif[unit_no]->DSCR.BIT.OCDEC = DSMIF_DIV_MCLK_8;
    sp_dsmif[unit_no]->DSCR.BIT.OCSH = DSMIF_BITSHIFT_8;

    /* Setup ERROR */
    dsmif_set_diserr(config);

    /* Setup interrupt handler */
    set_icu_interrupt_handler();
}
/*******************************************************************************
End of function dsmif_init
*******************************************************************************/

/*******************************************************************************
* Function Name: dsmif_start
* Description  : start dsmif filtration
* Arguments    : unit_no
* Return Value : none
*******************************************************************************/
void dsmif_start(uint32_t unit_no)
{
    /* start DSMIF filtration */
    sp_dsmif[unit_no]->DSCR.BIT.FEN = 1u;
}
/*******************************************************************************
End of function dsmif_start
*******************************************************************************/

/*******************************************************************************
* Function Name: dsmif_stop
* Description  : stop dsmif filtration
* Arguments    : unit_no
* Return Value : none
*******************************************************************************/
void dsmif_stop(uint32_t unit_no)
{
    /* stop DSMIF filtration */
    sp_dsmif[unit_no]->DSCR.BIT.FEN = 0u;
}
/*******************************************************************************
End of function dsmif_stop
*******************************************************************************/

/*******************************************************************************
* Function Name: dsmif_set_filter
* Description  : set filter register
* Arguments    : unit_no
*                p_cfg
*                    pointer to filter setting struct
* Return Value : none
*******************************************************************************/
void dsmif_set_filter(uint32_t unit_no, st_dsmif_filter_cfg_t * p_cfg)
{
    /* set CM register */
    sp_dsmif[unit_no]->DSCR.BIT.CMSINC = p_cfg->sinc;
    sp_dsmif[unit_no]->DSCR.BIT.CMDEC = p_cfg->deci;
    sp_dsmif[unit_no]->DSCR.BIT.CMSH = p_cfg->shift;

    /* set OC register */
    sp_dsmif[unit_no]->DSCR.BIT.OCSINC = p_cfg->sinc;
    sp_dsmif[unit_no]->DSCR.BIT.OCDEC = p_cfg->deci;
    sp_dsmif[unit_no]->DSCR.BIT.OCSH = p_cfg->shift;
}
/*******************************************************************************
End of function dsmif_set_filter
*******************************************************************************/

/*******************************************************************************
* Function Name: dsmif_read_data
* Description  : read data from DATA register
* Arguments    : data_reg
* Return Value : read_data
*                    current value read from DATA
*******************************************************************************/
uint16_t dsmif_read_data(uint8_t data_reg)
{
    uint16_t read_data;

    /* read DATA register */
    switch (data_reg)
    {
    case DSMIF_READ_DSMIF0_CDR0:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF0]->CDR0;
        break;
    case DSMIF_READ_DSMIF0_CDR1:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF0]->CDR1;
        break;
    case DSMIF_READ_DSMIF0_CDR2:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF0]->CDR2;
        break;
    case DSMIF_READ_DSMIF1_CDR0:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF1]->CDR0;
        break;
    case DSMIF_READ_DSMIF1_CDR1:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF1]->CDR1;
        break;
    case DSMIF_READ_DSMIF1_CDR2:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF1]->CDR2;
        break;

    case DSMIF_READ_DSMIF0_CCDR0:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF0]->CCDR0;
        break;
    case DSMIF_READ_DSMIF0_CCDR1:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF0]->CCDR1;
        break;
    case DSMIF_READ_DSMIF0_CCDR2:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF0]->CCDR2;
        break;
    case DSMIF_READ_DSMIF1_CCDR0:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF1]->CCDR0;
        break;
    case DSMIF_READ_DSMIF1_CCDR1:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF1]->CCDR1;
        break;
    case DSMIF_READ_DSMIF1_CCDR2:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF1]->CCDR2;
        break;

    case DSMIF_READ_DSMIF0_TCDR0:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF0]->TCDR0;
        break;
    case DSMIF_READ_DSMIF0_TCDR1:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF0]->TCDR1;
        break;
    case DSMIF_READ_DSMIF0_TCDR2:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF0]->TCDR2;
        break;
    case DSMIF_READ_DSMIF1_TCDR0:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF1]->TCDR0;
        break;
    case DSMIF_READ_DSMIF1_TCDR1:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF1]->TCDR1;
        break;
    case DSMIF_READ_DSMIF1_TCDR2:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF1]->TCDR2;
        break;

    case DSMIF_READ_DSMIF0_OCDR0:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF0]->OCDR0;
        break;
    case DSMIF_READ_DSMIF0_OCDR1:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF0]->OCDR1;
        break;
    case DSMIF_READ_DSMIF0_OCDR2:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF0]->OCDR2;
        break;
    case DSMIF_READ_DSMIF1_OCDR0:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF1]->OCDR0;
        break;
    case DSMIF_READ_DSMIF1_OCDR1:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF1]->OCDR1;
        break;
    case DSMIF_READ_DSMIF1_OCDR2:
        read_data = sp_dsmif[DSMIF_UNIT_DSMIF1]->OCDR2;
        break;

    default:
        read_data = 0;
        break;
    }

    return read_data;
}
/*******************************************************************************
End of function dsmif_read_data
*******************************************************************************/

/*******************************************************************************
* Function Name: dsmif_read_error_status
* Description  : read error_status from DSSR register
* Arguments    : unit_no
* Return Value : error_status
*                    current value read from DSSR
*******************************************************************************/
uint32_t dsmif_read_error_status(uint32_t unit_no)
{
    uint32_t error_status;
    
    error_status = sp_dsmif[unit_no]->DSSR.LONG;

    return error_status;
}
/*******************************************************************************
End of function dsmif_read_error_status
*******************************************************************************/

/*******************************************************************************
* Function Name: dsmif_set_limit
* Description  : set limit to OCLTR, OCHTR, SCLTR and SCHTR register
* Arguments    : unit_no
*                filter_no
*                    current filter setting number
* Return Value : none
*******************************************************************************/
void dsmif_set_limit(uint32_t unit_no, uint8_t filter_no)
{
    /* set OCLTR register (lower limit) */
    sp_dsmif[unit_no]->OCLTR = DSMIF_OCLTR_VALUE;

    /* set OCHTR register (upper limit) according to filter No. */
    switch(filter_no)
    {
    case DSMIF_FILTER_SET_0:
        sp_dsmif[unit_no]->OCHTR = DSMIF_UPPER_LIMIT_9 - DSMIF_ERROR_OFFSET;
        break;
    case DSMIF_FILTER_SET_1:
        sp_dsmif[unit_no]->OCHTR = DSMIF_UPPER_LIMIT_12 - DSMIF_ERROR_OFFSET;
        break;
    case DSMIF_FILTER_SET_2:
        sp_dsmif[unit_no]->OCHTR = DSMIF_UPPER_LIMIT_15 - DSMIF_ERROR_OFFSET;
        break;
    case DSMIF_FILTER_SET_3:
        sp_dsmif[unit_no]->OCHTR = DSMIF_UPPER_LIMIT_16 - DSMIF_ERROR_OFFSET;
        break;
    case DSMIF_FILTER_SET_4:
        sp_dsmif[unit_no]->OCHTR = DSMIF_UPPER_LIMIT_16 - DSMIF_ERROR_OFFSET;
        break;
    case DSMIF_FILTER_SET_5:
        sp_dsmif[unit_no]->OCHTR = DSMIF_UPPER_LIMIT_16 - DSMIF_ERROR_OFFSET;
        break;
    case DSMIF_FILTER_SET_6:
        sp_dsmif[unit_no]->OCHTR = DSMIF_UPPER_LIMIT_8 - DSMIF_ERROR_OFFSET;
        break;
    case DSMIF_FILTER_SET_7:
        sp_dsmif[unit_no]->OCHTR = DSMIF_UPPER_LIMIT_10 - DSMIF_ERROR_OFFSET;
        break;
    case DSMIF_FILTER_SET_8:
        sp_dsmif[unit_no]->OCHTR = DSMIF_UPPER_LIMIT_12 - DSMIF_ERROR_OFFSET;
        break;
    case DSMIF_FILTER_SET_9:
        sp_dsmif[unit_no]->OCHTR = DSMIF_UPPER_LIMIT_14 - DSMIF_ERROR_OFFSET;
        break;
    case DSMIF_FILTER_SET_10:
        sp_dsmif[unit_no]->OCHTR = DSMIF_UPPER_LIMIT_16 - DSMIF_ERROR_OFFSET;
        break;
    case DSMIF_FILTER_SET_11:
        sp_dsmif[unit_no]->OCHTR = DSMIF_UPPER_LIMIT_5 - DSMIF_ERROR_OFFSET;
        break;
    case DSMIF_FILTER_SET_12:
        sp_dsmif[unit_no]->OCHTR = DSMIF_UPPER_LIMIT_6 - DSMIF_ERROR_OFFSET;
        break;
    case DSMIF_FILTER_SET_13:
        sp_dsmif[unit_no]->OCHTR = DSMIF_UPPER_LIMIT_7 - DSMIF_ERROR_OFFSET;
        break;
    case DSMIF_FILTER_SET_14:
        sp_dsmif[unit_no]->OCHTR = DSMIF_UPPER_LIMIT_8 - DSMIF_ERROR_OFFSET;
        break;
    default:

        /* In case of an abnormal parameter, be initialized as No.0. */
        sp_dsmif[unit_no]->OCHTR = DSMIF_UPPER_LIMIT_9 - DSMIF_ERROR_OFFSET;
        break;
    }

    /* Set limit count of short circuit error */
    sp_dsmif[unit_no]->SCLTR = DSMIF_SHORT_CNT_0;
    sp_dsmif[unit_no]->SCHTR = DSMIF_SHORT_CNT_1;
}
/*******************************************************************************
End of function dsmif_set_limit
*******************************************************************************/

/*******************************************************************************
* Function Name: dsmif_set_diserr
* Description  : setup error for DSMIF
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void dsmif_set_diserr(st_dsmif_config_t* config)
{
    uint32_t unit_no = config->unit_no;
    uint32_t dis_error = config->dis_error;

    if (DSMIF_UNIT_DSMIF0 == unit_no)
    {
        if (0 != (dis_error & DSMIF_CH0))
        {
        	/* Cast to unsigned long is valid */
            DSMIF.DSCHR.BIT.DIS0 = 1u;
        }
        if (0 != (dis_error & DSMIF_CH1))
        {
        	/* Cast to unsigned long is valid */
        	DSMIF.DSCHR.BIT.DIS1 = 1u;
        }
        if (0 != (dis_error & DSMIF_CH2))
        {
        	/* Cast to unsigned long is valid */
        	DSMIF.DSCHR.BIT.DIS2 = 1u;
        }
    }
    if (DSMIF_UNIT_DSMIF1 == unit_no)
    {
        if (0 != (dis_error & DSMIF_CH0))
        {
        	/* Cast to unsigned long is valid */
        	DSMIF.DSCHR.BIT.DIS3 = 1u;
        }
        if (0 != (dis_error & DSMIF_CH1))
        {
        	/* Cast to unsigned long is valid */
            DSMIF.DSCHR.BIT.DIS4 = 1u;
        }
        if (0 != (dis_error & DSMIF_CH2))
        {
        	/* Cast to unsigned long is valid */
            DSMIF.DSCHR.BIT.DIS5 = 1u;
        }
    }
}
/*******************************************************************************
End of function dsmif_set_diserr
*******************************************************************************/

/*******************************************************************************
* Function Name: set_icu_interrupt_handler
* Description  : setup interrupt handler
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void set_icu_interrupt_handler(void)
{
    /* Register group BL2 interrupt OCDI0 (DSMIF0) */
    R_BSP_InterruptWrite(BSP_INT_SRC_BL2_DSMIF0_OCDI0,(bsp_int_cb_t)dsmif0_ocdi0_inthandler);

    /* Register group BL2 interrupt SCDI0 (DSMIF0) */
    R_BSP_InterruptWrite(BSP_INT_SRC_BL2_DSMIF0_SCDI0,(bsp_int_cb_t)dsmif0_scdi0_inthandler);

    /* Register group BL2 interrupt SUMEI0 (DSMIF0) */
    R_BSP_InterruptWrite(BSP_INT_SRC_BL2_DSMIF0_SUMEI0,(bsp_int_cb_t)dsmif0_sumei0_inthandler);

    /* Register group BL2 interrupt OCDI1 (DSMIF1) */
    R_BSP_InterruptWrite(BSP_INT_SRC_BL2_DSMIF1_OCDI1,(bsp_int_cb_t)dsmif1_ocdi1_inthandler);

    /* Register group BL2 interrupt SCDI1 (DSMIF1) */
    R_BSP_InterruptWrite(BSP_INT_SRC_BL2_DSMIF1_SCDI1,(bsp_int_cb_t)dsmif1_scdi1_inthandler);

    /* Register group BL2 interrupt SUMEI1 (DSMIF1) */
    R_BSP_InterruptWrite(BSP_INT_SRC_BL2_DSMIF1_SUMEI1,(bsp_int_cb_t)dsmif1_sumei1_inthandler);
}
/*******************************************************************************
End of function set_icu_interrupt_handler
*******************************************************************************/

/*******************************************************************************
* Function Name: dsmif0_ocdi0_inthandler
* Description  : BSP_INT_SRC_BL2_DSMIF0_OCDI0 interrupt handler
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void dsmif0_ocdi0_inthandler(void)
{
    if (NULL != s_dsmif_config[DSMIF_UNIT_DSMIF0].p_callback)
    {
        s_dsmif_config[DSMIF_UNIT_DSMIF0].p_callback(BSP_INT_SRC_BL2_DSMIF0_OCDI0);
    }
}
/*******************************************************************************
 End of function dsmif0_ocdi0_inthandler
*******************************************************************************/

/*******************************************************************************
* Function Name: dsmif0_scdi0_inthandler
* Description  : BSP_INT_SRC_BL2_DSMIF0_SCDI0 interrupt handler
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void dsmif0_scdi0_inthandler(void)
{
    if (NULL != s_dsmif_config[DSMIF_UNIT_DSMIF0].p_callback)
    {
        s_dsmif_config[DSMIF_UNIT_DSMIF0].p_callback(BSP_INT_SRC_BL2_DSMIF0_SCDI0);
    }
}
/*******************************************************************************
 End of function dsmif0_scdi0_inthandler
*******************************************************************************/

/*******************************************************************************
* Function Name: dsmif0_sumei0_inthandler
* Description  : BSP_INT_SRC_BL2_DSMIF0_SUMEI0 interrupt handler
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void dsmif0_sumei0_inthandler(void)
{
    if (NULL != s_dsmif_config[DSMIF_UNIT_DSMIF0].p_callback)
    {
        s_dsmif_config[DSMIF_UNIT_DSMIF0].p_callback(BSP_INT_SRC_BL2_DSMIF0_SUMEI0);
    }
}
/*******************************************************************************
 End of function dsmif0_sumei0_inthandler
*******************************************************************************/

/*******************************************************************************
* Function Name: dsmif1_ocdi1_inthandler
* Description  : BSP_INT_SRC_BL2_DSMIF1_OCDI1 interrupt handler
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void dsmif1_ocdi1_inthandler(void)
{
    if (NULL != s_dsmif_config[DSMIF_UNIT_DSMIF1].p_callback)
    {
        s_dsmif_config[DSMIF_UNIT_DSMIF1].p_callback(BSP_INT_SRC_BL2_DSMIF1_OCDI1);
    }
}
/*******************************************************************************
 End of function dsmif1_ocdi1_inthandler
*******************************************************************************/

/*******************************************************************************
* Function Name: dsmif1_scdi1_inthandler
* Description  : BSP_INT_SRC_BL2_DSMIF1_SCDI1 interrupt handler
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void dsmif1_scdi1_inthandler(void)
{
    if (NULL != s_dsmif_config[DSMIF_UNIT_DSMIF1].p_callback)
    {
        s_dsmif_config[DSMIF_UNIT_DSMIF1].p_callback(BSP_INT_SRC_BL2_DSMIF1_SCDI1);
    }
}
/*******************************************************************************
 End of function dsmif1_scdi1_inthandler
*******************************************************************************/

/*******************************************************************************
* Function Name: dsmif1_sumei1_inthandler
* Description  : BSP_INT_SRC_BL2_DSMIF1_SUMEI1 interrupt handler
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void dsmif1_sumei1_inthandler(void)
{
    if (NULL != s_dsmif_config[DSMIF_UNIT_DSMIF1].p_callback)
    {
        s_dsmif_config[DSMIF_UNIT_DSMIF1].p_callback(BSP_INT_SRC_BL2_DSMIF1_SUMEI1);
    }
}
/*******************************************************************************
 End of function dsmif1_sumei1_inthandler
*******************************************************************************/

/* End of File */
