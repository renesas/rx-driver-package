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
* File Name    : r_ptp_light.c
* Description  : PTP light module device driver
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 10.07.2014 1.00    FIT spec version
*         : 20.08.2014 1.01    State transition and worst10 setting added
*         : 31.12.2014 1.02    Supported RX71M device and added BMC function
*                              Changed module name (eliminate "_api")
*         : 30.10.2015 1.00    Modified from the PTP driver full version
*         : 31.03.2016 1.10    Data structures changed
*         : 11.11.2016 1.11    Corrected the internal operation of getting version function
*		  : 31.07.2019 1.12    Added support for GNUC and ICCRX.
*                              Changed MINT interrupt handler operation.
*		  : 31.08.2019 1.13    Supported RX72M device.
*                              Added Bypass setting.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_ptp_light_rx_if.h"
#include "r_ptp_light_rx_private.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Register access structure to SYNFP0 or SYNFP1 part of the EPTPC */
static volatile struct st_eptpc0 R_BSP_EVENACCESS_SFR *synfp[2] =
{
	&EPTPC0,
	&EPTPC1,
};

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
static bool is_INFABT[NUM_PORT]; /* INFABT flag, 0: CH0, 1: CH1 */

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/**
 * Private global function prototyes
 */
static void _R_PTPL_Int_PRC(void);
static void _R_PTPL_Int_SYNFP0(void);
static void _R_PTPL_Int_SYNFP1(void);
static void _R_PTPL_Init_SYNFP(uint8_t ch, PTPLPort* port);
static void _R_PTPL_Init_PRC(PTPLPort port[]);

/**
 * Private global function prototyes
 */
/* Pointer to MINT interrupt handler */
static MINT_HNDLR ptpl_mint_isr[3] = {NULL, NULL, NULL};

/**
 * Renesas Ethernet API functions
 */
/***********************************************************************************************************************
* Function Name: R_PTPL_GetVersion
* Description  : Get PTP light driver version number.  
* Arguments    : None
* Return Value : PTP light module version number
* Note         : This function is expanded as inline function.
***********************************************************************************************************************/
uint32_t R_PTPL_GetVersion(void)
{
    uint32_t version;

    /* major version number (upper 16bit)*/
    version = ((uint16_t)RX_PTP_LIGHT_VERSION_MAJOR << 16u);
    
    /* mijor version number (lower 16bit) */
    version |= (uint16_t)RX_PTP_LIGHT_VERSION_MINOR;

    return version;
} /* End of function R_PTPL_GetVersion() */

/***********************************************************************************************************************
* Function Name: R_PTPL_Reset
* Description  : Reset EPTPC.  
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PTPL_Reset(void)
{
	/* Enable EPTPC */
	ptp_dev_enab();
	
	/* Reset EPTPC */
	EPTPC.PTRSTR.BIT.RESET = 1;

    /* Wait more than 64 PCLKA cycles */
	/* WAIT_LOOP */
	R_BSP_SoftwareDelay(1, BSP_DELAY_MICROSECS);
	
	/* Release reset EPTPC */
	EPTPC.PTRSTR.BIT.RESET = 0;

	/* Wait more than 256 PCLKA cycles */
	/* WAIT_LOOP */
	R_BSP_SoftwareDelay(3, BSP_DELAY_MICROSECS);
	
} /* End of function R_PTPL_Reset() */

/***********************************************************************************************************************
* Function Name: R_PTPL_SetTran
* Description  : Set inter ports transfer mode.
* Arguments    : mode - Inter ports transfer mode
*                dir - Relay enable directions (bit map form)
* Return Value : PTPL_OK        - Processing completed successfully
*                PTPL_ERR_PARAM - Parameter error
* Note         : This function is valid only the standard Ethernet frames. (not valid for PTP message frames).
*                If the argument (mode or dir) is NULL pointer, the value does not set.
***********************************************************************************************************************/
ptpl_return_t R_PTPL_SetTran(TranMode *mode, RelEnabDir *dir)
{
    ptpl_return_t ret = PTPL_OK;

    if (NULL != dir)
    { /* Set/clear relay enable directions */
        if (*dir > 0x3)
        {
            ret = PTPL_ERR_PARAM;
        }
        else
        {
            EPTPC.TRNMR.LONG = ((uint32_t)(*dir << 8u) | EPTPC.TRNMR.BIT.MOD);
        }
    }

    if (NULL != mode)
    { /* Set transfer mode */
        if (CT_THR == *mode)
        { /* Cut through mode */
	        EPTPC.TRNMR.BIT.MOD = 1;
        }
        else if (ST_FOR == *mode)
        { /* Store and forward mode */
	        EPTPC.TRNMR.BIT.MOD = 0;
        }
    }

    return ret;

} /* End of R_PTPL_SetTran() */

/***********************************************************************************************************************
* Function Name: R_PTPL_SetMCFilter
* Description  : Set multicast frames (MC) filter (FFLTR).
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                fil - Multicast frames (MC) filter setting
*                fmac - Reception frame MAC address (register FMAC0R(U/L) or FMAC1R(U/L))
* Return Value : PTPL_OK        - Processing completed successfully
*                PTPL_ERR_PARAM - Parameter error
* Note         : This function is valid only the standard Ethernet frames. (not valid for PTP message frames). 
*                This function clears the extended promiscuous mode implicitly. 
*                If 3rd argument (fmac) is NULL pointer, the value does not set.
***********************************************************************************************************************/
ptpl_return_t R_PTPL_SetMCFilter(uint8_t ch, MCRecFil fil, uint32_t *fmac)
{
    ptpl_return_t ret = PTPL_OK;

	if (NUM_PORT <= ch)
	{
		return PTPL_ERR_PARAM;
	}
	
	if (MC_REC_ALL == fil)
	{ /* Receive all MC frames (legacy compatible) */
	    synfp[ch]->FFLTR.LONG = (0x00000000 | (synfp[ch]->FFLTR.LONG & 0x00010000));
	}
	else if (MC_REC_NO == fil)
	{ /* Do not receive MC frame */
	    synfp[ch]->FFLTR.LONG = (0x00000004 | (synfp[ch]->FFLTR.LONG & 0x00010000));
	}
    else if (MC_REC_REG0 == fil)
	{ /* Receive only the MC frame registered FMAC0R(U/L) */
		if (NULL != fmac)
		{ /* Upadate FMAC0R */
		    synfp[ch]->FMAC0RU.LONG = *fmac;
			synfp[ch]->FMAC0RL.LONG = *(fmac + 1);
		}
	    synfp[ch]->FFLTR.LONG = (0x00000006 | (synfp[ch]->FFLTR.LONG & 0x00010000));
	}
	else /* MC_REC_REG1 == fil */
	{ /* Receive only the MC frame registered FMAC1R(U/L) */
		if (NULL != fmac)
		{ /* Upadate FMAC1R */
		    synfp[ch]->FMAC1RU.LONG = *fmac;
			synfp[ch]->FMAC1RL.LONG = *(fmac + 1);
		}
	    synfp[ch]->FFLTR.LONG = (0x00000007 | (synfp[ch]->FFLTR.LONG & 0x00010000));
	}
	
	return ret;
} /* End of R_PTPL_SetMCFilter() */

/***********************************************************************************************************************
* Function Name: R_PTPL_SetExtPromiscuous
* Description  : Set/clear extended promiscuous mode.
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                is_set - (true): set, (false): clear
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptpl_return_t R_PTPL_SetExtPromiscuous(uint8_t ch, bool is_set)
{
    ptpl_return_t ret = PTPL_OK;
    
    if (NUM_PORT <= ch)
	{
		return PTPL_ERR_PARAM;
	}

	if (true == is_set)
    { /* Set extended extended promiscuous mode */
		synfp[ch]->FFLTR.BIT.EXTPRM = 1;
    }
    else
    { /* Clear extended extended promiscuous mode */
		synfp[ch]->FFLTR.BIT.EXTPRM = 0;
    }

    return ret;
} /* End of R_PTPL_SetExtPromiscuous() */

/***********************************************************************************************************************
* Function Name: R_PTPL_Init
* Description  : Initialize EPTPC depends on the device configuration.
* Arguments    : tbl - PTP configuration table
* Return Value : PTPL_OK        - Processing completed successfully
*                PTPL_ERR_PARAM - Parameter error
*                PTPL_ERR       - Any error occured
***********************************************************************************************************************/
ptpl_return_t R_PTPL_Init(PTPLConfig *tbl)
{
	uint8_t ch;

#if (1 == BSP_MCU_RX72M)
	EPTPC.SYBYPSR.LONG = 0x00000000; /* bypass off */
#endif

	/* Set EPTPC interrupt mask */
    EPTPC.MIEIPR.LONG = 0x00000000;
    EPTPC.STIPR.LONG = 0x00000000;
    EPTPC.PRIPR.LONG = 0x00000000;
    EPTPC0.SYIPR.LONG = 0x00000000;
	EPTPC1.SYIPR.LONG = 0x00000000;

	/* ==== Initialize SYNFP0 and SYNFP1 ==== */
	/* WAIT_LOOP */
	for (ch = 0; ch < NUM_PORT; ch++)
    { /* Set SYNFP parameter by initial values */
		_R_PTPL_Init_SYNFP(ch, &(tbl->port[ch]));
	}

	/* Validate set values to SYNFP0 and SYNFP1 */
	EPTPC0.SYRVLDR.LONG = 0x00000007;
	EPTPC1.SYRVLDR.LONG = 0x00000007;

	/* ==== Initialize packet relation controller unit (PRC-TC) ==== */
	_R_PTPL_Init_PRC(tbl->port);

	/* Clear EPTPC interrupt status */
	EPTPC.MIESR = 0x003F000F;
	EPTPC.STSR = 0x0000001B;
	EPTPC.PRSR = 0x3000010F;
	EPTPC0.SYSR = 0x00035277;
	EPTPC1.SYSR = 0x00035277;

	/* Clear EPTPC interrupt mask */
	EPTPC.STIPR.LONG = 0x00000000;
	EPTPC.PRIPR.LONG = 0x00000000;

	EPTPC0.SYIPR.LONG = 0x00000000;
	EPTPC1.SYIPR.LONG = 0x00000000;

	EPTPC.MIEIPR.LONG = 0x0000000E;
	
	/* Set EPTPC interrupt */
    ptp_dev_start();

	return PTPL_OK;
} /* End of R_PTPL_Init() */

/***********************************************************************************************************************
* Function Name: R_PTPL_RegMINTHndr
* Description  : Set MINT interrupt and registers a user function to MINT interrupt handler of EPTPC.
* Arguments    : reg - MINT interrupt register.
*                    MINT_FUNC_PRC: Interrupt from PRC-TC
*                    MINT_FUNC_SYN0: Interrupt from SYNFP0
*                    MINT_FUNC_SYN1: Interrupt from SYNFP1
*                event - interrupt elements.
*                func - register function.
* Return Value : None
* Note         : Registered user function is updated if register the same MINT interrupt handler.
*                If 3rd argument(=func) is NULL, registered function is removed and the MINT interrupt is disabled.
***********************************************************************************************************************/
void R_PTPL_RegMINTHndr(MINT_Reg reg, uint32_t event, MINT_HNDLR func)
{
	volatile unsigned long R_BSP_EVENACCESS_SFR *ctrl[3] = {&(EPTPC.PRIPR.LONG), &(EPTPC0.SYIPR.LONG), &(EPTPC1.SYIPR.LONG)};

	/* Register the user function to the interrupt handler */
	ptpl_mint_isr[reg] = func;
	
	if (NULL == func)
	{ /* Clear interrupt event */
	    *ctrl[reg] &= ~event;
	}
	else
	{
		*ctrl[reg] |= event;
	}
	
} /* End of R_PTPL_RegMINTHndr() */

/***********************************************************************************************************************
* Function Name: R_PTPL_GetSyncConfig
* Description  : Get synchronous configuration (SYRFL1R, SYRFL2R, SYTRENR and SYCONFR).
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                fil1 - SYRFL1R
*                fil2 - SYRFL2R
*                tren - SYTRENR
*                conf - SYCONFR
* Return Value : PTPL_OK        - Processing completed successfully
*                PTPL_ERR_PARAM - Parameter error
* Note         : Need to update synchronous configuration every when clock is changed such as BMC algorithm.
*                If the argument (fil1, fil2, tren or conf) is NULL pointer, the value does not get.
***********************************************************************************************************************/
ptpl_return_t R_PTPL_GetSyncConfig(uint8_t ch, uint32_t *fil1, uint32_t *fil2, uint32_t *tren, uint32_t *conf)
{	
	ptpl_return_t ret = PTPL_OK;
	
	if (NUM_PORT <= ch)
	{
		return PTPL_ERR_PARAM;
	}
	
	if (NULL != fil1)
    { /* Get SYRFL1R */
        *fil1 = synfp[ch]->SYRFL1R.LONG;
    }
    if (NULL != fil2)
    { /* Get SYRFL2R */
        *fil2 = synfp[ch]->SYRFL2R.LONG;
    }
	if (NULL != tren)
    { /* Get SYTRENR */
        *tren = synfp[ch]->SYTRENR.LONG;
    }
    if (NULL != conf)
    { /* Get SYCONFR */
        *conf = synfp[ch]->SYCONFR.LONG;
    }
	
	return ret;
} /* End of R_PTPL_GetSyncConfig() */

/***********************************************************************************************************************
* Function Name: R_PTPL_SetSyncConfig
* Description  : Set synchronous configuration (SYRFL1R, SYRFL2R, SYTRENR and SYCONFR).
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                fil1 - SYRFL1R
*                fil2 - SYRFL2R
*                tren - SYTRENR
*                conf - SYCONFR
* Return Value : PTPL_OK        - Processing completed successfully
*                PTPL_ERR_PARAM - Parameter error
* Note         : Need to update synchronous configuration every when clock is changed such as BMC algorithm.
*                If the argument (fil1, fil2, tren or conf) is NULL pointer, the value does not set.
***********************************************************************************************************************/
ptpl_return_t R_PTPL_SetSyncConfig(uint8_t ch, uint32_t *fil1, uint32_t *fil2, uint32_t *tren, uint32_t *conf)
{	
	ptpl_return_t ret = PTPL_OK;
	
	if (NUM_PORT <= ch)
	{
		return PTPL_ERR_PARAM;
	}
	
	if (NULL != fil1)
    { /* Set SYRFL1R */
        synfp[ch]->SYRFL1R.LONG = *fil1;
    }
    if (NULL != fil2)
    { /* Set SYRFL2R */
        synfp[ch]->SYRFL2R.LONG = *fil2;
    }
	if (NULL != tren)
    { /* Set SYTRENR */
        synfp[ch]->SYTRENR.LONG = *tren;
    }
    if (NULL != conf)
    { /* Set SYCONFR */
        synfp[ch]->SYCONFR.LONG = *conf;
    }
    
    if ((NULL != fil1) || (NULL != fil2) || (NULL != tren)|| (NULL != conf))
    { /* Validate setting values */
        synfp[ch]->SYRVLDR.BIT.STUP = 1;
    }
	
	return ret;
} /* End of R_PTPL_SetSyncConfig() */

/***********************************************************************************************************************
* Function Name: R_PTPL_SetInterrupt
* Description  : Enable EPTPC INFABT interrupt.
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
* Return Value : PTPL_OK        - Processing completed successfully
*                PTPL_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptpl_return_t R_PTPL_SetInterrupt(uint8_t ch)
{
	if (NUM_PORT <= ch)
	{
		return PTPL_ERR_PARAM;
	}

	/* Clear INFABT detection flag */
	is_INFABT[ch] = false;

	if (0 == ch)
	{ /* CH0 */
    	/* Enable SYNFP0 interrupt */
		EPTPC.MIEIPR.LONG |= 0x00000002;

    	/* Enable SYNFP0.SYSR.INFABT interrupt */ 
    	EPTPC0.SYIPR.LONG |= 0x00004000;
	}
	else
	{ /* CH1 */
		/* Enable SYNFP1 interrupt */
		EPTPC.MIEIPR.LONG |= 0x00000004;

    	/* Enable SYNFP1.SYSR.INFABT interrupt */ 
    	EPTPC1.SYIPR.LONG |= 0x00004000;
	}

	return PTPL_OK;
} /* End of function R_PTPL_SetInterrupt() */

/***********************************************************************************************************************
* Function Name: R_PTPL_ChkInterrupt
* Description  : Check INFABT interrupt occurred or not?
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                is_det - INFABT interrupt detection flag
* Return Value : PTPL_OK        - Processing completed successfully
*                PTPL_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptpl_return_t R_PTPL_ChkInterrupt(uint8_t ch, bool *is_det)
{
	if (NUM_PORT <= ch)
	{
		return PTPL_ERR_PARAM;
	}

	*is_det = is_INFABT[ch];

	return PTPL_OK;
} /* End of function R_PTPL_ChkInterrupt() */

/***********************************************************************************************************************
* Function Name: R_PTPL_ClrInterrupt
* Description  : Clear INFABT interrupt occurrence flag.
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
* Return Value : PTPL_OK        - Processing completed successfully
*                PTPL_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptpl_return_t R_PTPL_ClrInterrupt(uint8_t ch)
{
	if (NUM_PORT <= ch)
	{
		return PTPL_ERR_PARAM;
	}

	is_INFABT[ch] = false;

	return PTPL_OK;
} /* End of function R_PTPL_ClrInterrupt() */

/***********************************************************************************************************************
* Function Name: Eptpc_isr
* Description  : Interrupt handler of EPTPC.
* Arguments    : None
* Return Value : None
* Note         : only support INFABT event
***********************************************************************************************************************/
void Eptpc_isr(void)
{
	uint32_t miesr;
	
	miesr = EPTPC.MIESR;

	if ((miesr & 0x003F0000) != 0x00000000)	/* Timer interrupt */
	{
		/* CYC[0-5] INT all mask */
	}
	
	if ((miesr & 0x00000001) != 0x00000000)	/* STCA */
	{
		/* STCA INT all mask */
	}

	if ((miesr & 0x00000002) != 0x00000000) /* SYN0 */
	{
		_R_PTPL_Int_SYNFP0();
	}

	if ((miesr & 0x00000004) != 0x00000000) /* SYN1 */
	{
		_R_PTPL_Int_SYNFP1();
	}

	if ((miesr & 0x00000008) != 0x00000000) /* PRC_TC */
	{
		_R_PTPL_Int_PRC();
	}

	EPTPC.MIESR = miesr;

} /* End of Eptpc_isr() */

/***********************************************************************************************************************
* Function Name: _R_PTPL_Int_PRC
* Description  : Interrupt handler of PRC-TC.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void _R_PTPL_Int_PRC(void)
{
	uint32_t prsr;

	prsr = EPTPC.PRSR;

	if (ptpl_mint_isr[MINT_FUNC_PRC] != NULL)
	{
		ptpl_mint_isr[MINT_FUNC_PRC](prsr);
	}

	EPTPC.PRSR = prsr;
} /* End of _R_PTPL_Int_PRC() */

/***********************************************************************************************************************
* Function Name: _R_PTPL_Int_SYNFP0
* Description  : Interrupt handler of SYNFP0
*              : Set INFABT interrupt occurrence flag of SYNFP0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void _R_PTPL_Int_SYNFP0(void)
{
	uint32_t sy0sr;

	sy0sr = EPTPC0.SYSR;

	if ((sy0sr & 0x00004000) == 0x00004000) 
	{
		is_INFABT[0] = true;
	}

	if (ptpl_mint_isr[MINT_FUNC_SYN0] != NULL)
	{
		ptpl_mint_isr[MINT_FUNC_SYN0](EPTPC0.SYSR);
	}

	EPTPC0.SYSR = sy0sr;
} /* End of _R_PTPL_Int_SYNFP0() */

/***********************************************************************************************************************
* Function Name: _R_PTPL_Int_SYNFP1
* Description  : Interrupt handler of SYNFP1
*              : Set INFABT interrupt occurrence flag of SYNFP1.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void _R_PTPL_Int_SYNFP1(void)
{
	uint32_t sy1sr;

	sy1sr = EPTPC1.SYSR;

	if ((sy1sr & 0x00004000) == 0x00004000) 
	{
		is_INFABT[1] = true;
	}

	if (ptpl_mint_isr[MINT_FUNC_SYN1] != NULL)
	{
		ptpl_mint_isr[MINT_FUNC_SYN1](EPTPC1.SYSR);
	}
	
	EPTPC1.SYSR = sy1sr;
} /* End of _R_PTPL_Int_SYNFP1() */

/***********************************************************************************************************************
* Function Name: _R_PTP_Init_SYNFP
* Description  : Set SYNFP parameters by initial values.
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                port - PTP port related information
* Return Value : None
***********************************************************************************************************************/
static void _R_PTPL_Init_SYNFP(uint8_t ch, PTPLPort* port)
{
	union {
		uint32_t All ;
		struct {
#if __LIT /* Little endian */
            uint8_t b0 ;
			uint8_t b1 ;
			uint8_t b2 ;
			uint8_t b3 ;
#else /* Big endian */
			uint8_t b3 ;
			uint8_t b2 ;
			uint8_t b1 ;
			uint8_t b0 ;
#endif
		} B ;
	} LDATA ;
	
	/* Set MAC address (SYMACRU and SYMACRL) */
	LDATA.B.b3 = 0;
	LDATA.B.b2 = port->macAddr[0];
	LDATA.B.b1 = port->macAddr[1];
	LDATA.B.b0 = port->macAddr[2];
    synfp[ch]->SYMACRU.LONG = LDATA.All;

	LDATA.B.b3 = 0 ;
	LDATA.B.b2 = port->macAddr[3];
	LDATA.B.b1 = port->macAddr[4];
	LDATA.B.b0 = port->macAddr[5];
    synfp[ch]->SYMACRL.LONG = LDATA.All;

	/* Set IP address (SYIPADDRR) */
	LDATA.B.b3 = port->ipAddr[0];
	LDATA.B.b2 = port->ipAddr[1];
	LDATA.B.b1 = port->ipAddr[2];
	LDATA.B.b0 = port->ipAddr[3];
	synfp[ch]->SYIPADDRR = LDATA.All;

	/* Initialize PTP reception filters */
	synfp[ch]->SYRFL1R.LONG = 0x00000000;
    synfp[ch]->SYRFL2R.LONG = 0x00000000;
    synfp[ch]->SYCONFR.LONG = 0x00000028;

} /* End of _R_PTPL_Init_SYNFP() */

/***********************************************************************************************************************
* Function Name: _R_PTPL_Init_PRC
* Description  : Set PRC-TC parameters by initial values.
* Arguments    : port - PTP port related information
* Return Value : None
***********************************************************************************************************************/
static void _R_PTPL_Init_PRC(PTPLPort port[])
{
	union {
		uint32_t All ;
		struct {
#if __LIT /* Little endian */
            uint8_t b0 ;
			uint8_t b1 ;
			uint8_t b2 ;
			uint8_t b3 ;
#else /* Big endian */
			uint8_t b3 ;
			uint8_t b2 ;
			uint8_t b1 ;
			uint8_t b0 ;
#endif
		} B ;
	} LDATA ;

	/* ==== Initialize packet relation controller unit (PRC-TC) ==== */
	/* Set MAC address */
	LDATA.B.b3 = 0;
	LDATA.B.b2 = port[0].macAddr[0];
	LDATA.B.b1 = port[0].macAddr[1];
	LDATA.B.b0 = port[0].macAddr[2];
	EPTPC.PRMACRU0.LONG = LDATA.All;

	LDATA.B.b3 = 0 ;
	LDATA.B.b2 = port[0].macAddr[3];
	LDATA.B.b1 = port[0].macAddr[4];
	LDATA.B.b0 = port[0].macAddr[5];
  	EPTPC.PRMACRL0.LONG = LDATA.All;

	LDATA.B.b3 = 0;
	LDATA.B.b2 = port[1].macAddr[0];
	LDATA.B.b1 = port[1].macAddr[1];
	LDATA.B.b0 = port[1].macAddr[2];
	EPTPC.PRMACRU1.LONG = LDATA.All;

	LDATA.B.b3 = 0 ;
	LDATA.B.b2 = port[1].macAddr[3];
	LDATA.B.b1 = port[1].macAddr[4];
	LDATA.B.b0 = port[1].macAddr[5];
  	EPTPC.PRMACRL1.LONG = LDATA.All;

    /* Set PTP packets are transmitted to both ports */
	EPTPC.TRNDISR.LONG = 0x00000000;

} /* End of _R_PTPL_Init_PRC() */
/* End of File */
