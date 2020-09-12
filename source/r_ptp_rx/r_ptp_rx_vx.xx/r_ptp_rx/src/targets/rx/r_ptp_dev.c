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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_ptp_dev.c
* Description  : EPTPC module device driver RX64M/71M/72M device setting
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 10.07.2014 1.00    FIT spec version
*         : 20.08.2014 1.01    State transition and worst10 setting added
*         : 31.12.2014 1.02    Supported RX71M device and added BMC function
*                              Changed module name (eliminate "_api")
*         : 31.03.2016 1.10    Data structures changed
*         : 11.11.2016 1.12    Corrected the internal operation of getting version functions
*                              Corrected wait operation to information retention of GETINFOR
*                              Corrected the array index deviation of ptp_tmr_isr.
*         : 31.03.2017 1.13    Corrected BC and P2P TC setting.
*                              Modified listening state operations of R_PTP_Init, R_PTP_Start and R_PTP_Stop functions.
*                              Added registering user function to MINT interrupt handler.
*                              Changed MINT interrupt handler operation.
*                              Changed TC&OC combined opeartion to TC only operation.
*                              Added my clockIdentity setting function.
*                              Added domainnumber field updating function.
*                              Added announce message fields updating function.
*                              Added and modified transmission interval setting function.
*                              Added offsetFromMaster updating wait function when device state is P2P slave.
*                              Added PTP port state setting function.
*         : 30.04.2017 1.14    Corrected underflow operation of R_PTP_GetSyncInfo function.
*                              Corrected logMessageInterval out of range judgment.
*		  : 31.07.2019 1.15    Added support for GNUC and ICCRX.
*                              Added "WAIT_LOOP" comments for every loop operation.
*		  : 31.08.2019 1.16    Supported RX72M device.
*                              Added Bypass setting.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_ptp_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
void ptp_dev_enab(void);
ptp_return_t ptp_dev_start(void);
ptp_return_t ptp_dev_stop(void);

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/**
 * Renesas API functions
 */
 
/***********************************************************************************************************************
* Function Name: ptp_dev_enab
* Description  : Enable the EPTPC of RX64M/71M/72M
* Return Value : None
***********************************************************************************************************************/
void ptp_dev_enab(void)
{
    if (1 == MSTP_EDMAC0)
	{ /* Case of EtherC and EDMAC CH0 stopped */
	    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR); /* WP OFF */
	    MSTP_EDMAC0 = 0; /* Enable CH0 */
		R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR); /* WP ON */
	}
	
	if (1 == MSTP_EDMAC1)
	{ /* Case of EtherC and EDMAC CH1 stopped */
	    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR); /* WP OFF */
	    MSTP_EDMAC1 = 0; /* Enable CH1 */
		R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR); /* WP ON */
	}

#if (1 == BSP_MCU_RX72M)
	R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR); /* WP OFF */
	MSTP_EPTPC = 0; /* Enable EPTPC */
	R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR); /* WP ON */
#endif
	
} /* End of function ptp_dev_enab() */

/***********************************************************************************************************************
* Function Name: ptp_dev_start
* Description  : Start RX64M/71M/72M Device for PTP driver
*                - Set EPTPC interrupt level
*                - Enable EPTPC interrupt
*                - Register EPTPC Interrupt handler
* Return Value : PTP_OK
*                PTP_ERR
***********************************************************************************************************************/
ptp_return_t ptp_dev_start(void)
{
    ptp_return_t ret = PTP_OK;
#if (PTP_CFG_MODE & PTP_MODE_HWINT)
	bsp_int_err_t bsp_ret;
    uint32_t ipl = PTP_CFG_INTERRUPT_LEVEL;

    /* Enable EPTPC intterupt */
    ICU.GENAL1.BIT.EN0 = 1;

    if (1 != ICU.IER[0x0E].BIT.IEN1)
    {
        bsp_ret = R_BSP_InterruptControl(BSP_INT_SRC_AL1_EPTPC_MINT, BSP_INT_CMD_GROUP_INTERRUPT_ENABLE, (void*)&ipl);
	    if (bsp_ret != BSP_INT_SUCCESS)
	    {
		    return PTP_ERR;
	    }
    }

    /* Register PTPEDMAC interrupt handler */
    bsp_ret = R_BSP_InterruptWrite (BSP_INT_SRC_AL1_EPTPC_MINT,  (bsp_int_cb_t)Eptpc_isr);
	if (bsp_ret != BSP_INT_SUCCESS)
	{
		return PTP_ERR;
	}
#endif

    return ret;

} /* End of function ptp_dev_start() */

/***********************************************************************************************************************
* Function Name: ptp_dev_stop
* Description  : Stop RX64M/71M/72M Device for PTP driver
*                - Disable EPTPC interrupt
*                - Release EPTPC Interrupt handler
* Return Value : PTP_OK
*                PTP_ERR
***********************************************************************************************************************/
ptp_return_t ptp_dev_stop(void)
{
    ptp_return_t ret = PTP_OK;
#if (PTP_CFG_MODE & PTP_MODE_HWINT)
	bsp_int_err_t bsp_ret;

    /* Disable EPTPC intterupt */
    ICU.GENAL1.BIT.EN0 = 0;

    bsp_ret = R_BSP_InterruptControl(BSP_INT_SRC_AL1_EPTPC_MINT, BSP_INT_CMD_GROUP_INTERRUPT_DISABLE, FIT_NO_PTR);
	if (bsp_ret != BSP_INT_SUCCESS)
	{
		return PTP_ERR;
	}

    /* Register PTPEDMAC interrupt handler */
    bsp_ret = R_BSP_InterruptWrite (BSP_INT_SRC_AL1_EPTPC_MINT, (bsp_int_cb_t)NULL);
	if (bsp_ret != BSP_INT_SUCCESS)
	{
		return PTP_ERR;
	}
#endif

    return ret;

} /* End of function ptp_dev_stop() */
/* End of File */
