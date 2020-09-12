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
* File Name    : r_ptp.c
* Description  : PTP module device driver
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
*                              Added reset release waiting operation in the R_PTP_Reset function.
*		  : 31.08.2019 1.16    Supported RX72M device.
*                              Added Bypass setting.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_ptp_rx_if.h"
#include "r_ptp_rx_private.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define MIN16_S  (0xFFFF8000) /* -32768 */
#define MAX16_S  (0x7FFF) /* 32767 */

/* Scaled time interval max and minimum valuea (2^(16) nano second unit)  */
#define SCALED_TIME_MAX_U (0x7FFFFFFF)
#define SCALED_TIME_MAX_L (0xFFFFFFFF)
#define SCALED_TIME_MIN_U (0x80000000)
#define SCALED_TIME_MIN_L (0x00000000)

/* Configuration constant values */
#if (CLK_ID_USR_DEFINE == PTP_CFG_CLK_ID)
const uint32_t PORTID_CLK_ID_U[NUM_PORT] = {PORTID_CLK_ID0_U, PORTID_CLK_ID1_U};
const uint32_t PORTID_CLK_ID_L[NUM_PORT] = {PORTID_CLK_ID0_L, PORTID_CLK_ID1_L};
#endif
const uint32_t PORTID_PORT_NUM[NUM_PORT] = {PTP_CFG_PORTID_PORT_NUM0, PTP_CFG_PORTID_PORT_NUM1};

const uint8_t ANNOUNCE_INTERVAL[NUM_PORT] = {PTP_CFG_LOG_ANNOUNCE_INTERVAL0, PTP_CFG_LOG_ANNOUNCE_INTERVAL1};
const uint8_t SYNC_INTERVAL[NUM_PORT] = {PTP_CFG_LOG_SYNC_INTERVAL0, PTP_CFG_LOG_SYNC_INTERVAL1};
const uint8_t DELAY_REQ_INTERVAL[NUM_PORT] = {PTP_CFG_LOG_MIN_DELAY_REQ_INTERVAL0, PTP_CFG_LOG_MIN_DELAY_REQ_INTERVAL1};
const uint8_t PDELAY_REQ_INTERVAL[NUM_PORT] = {PTP_CFG_LOG_MIN_PDELAY_REQ_INTERVAL0, PTP_CFG_LOG_MIN_PDELAY_REQ_INTERVAL1};

const uint8_t GM_PRIORITY1[NUM_PORT] = {PTP_CFG_GM_PRIORITY10, PTP_CFG_GM_PRIORITY11};
const uint8_t GM_PRIORITY2[NUM_PORT] = {PTP_CFG_GM_PRIORITY20, PTP_CFG_GM_PRIORITY21};

const uint32_t GM_CLK_QUALITY[NUM_PORT] = {PTP_CFG_GM_CLK_QUALITY0, PTP_CFG_GM_CLK_QUALITY1};
const uint32_t GM_CLK_ID_U[NUM_PORT] = {PTP_CFG_GM_CLK_ID0_U, PTP_CFG_GM_CLK_ID1_U};
const uint32_t GM_CLK_ID_L[NUM_PORT] = {PTP_CFG_GM_CLK_ID0_L, PTP_CFG_GM_CLK_ID1_L};

const uint16_t CUR_UTC_OFFSET[NUM_PORT] = {PTP_CFG_CUR_UTC_OFFSET0, PTP_CFG_CUR_UTC_OFFSET1};
const uint8_t TIME_SOURCE[NUM_PORT] = {PTP_CFG_TIME_SOURCE0, PTP_CFG_TIME_SOURCE1};

const uint16_t STEPS_REMOVED[NUM_PORT] = {PTP_CFG_STEPS_REMOVED0, PTP_CFG_STEPS_REMOVED1};

const uint8_t PTP_EVENT_TOS[NUM_PORT] = {PTP_CFG_PTP_EVENT_TOS0, PTP_CFG_PTP_EVENT_TOS1};
const uint8_t PTP_GENERAL_TOS[NUM_PORT] = {PTP_CFG_PTP_GENERAL_TOS0, PTP_CFG_PTP_GENERAL_TOS1};
const uint8_t PTP_PRIMARY_TTL[NUM_PORT] = {PTP_CFG_PTP_PRIMARY_TTL0, PTP_CFG_PTP_PRIMARY_TTL1};
const uint8_t PTP_PDELAY_TTL[NUM_PORT] = {PTP_CFG_PTP_PDELAY_TTL0, PTP_CFG_PTP_PDELAY_TTL1};

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
static bool is_INFABT[NUM_PORT]; /* INFABT flag, 0: Port0, 1: Port1 */

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/**
 * Private global function prototyes
 */
static int32_t _R_PTP_Wait(volatile uint32_t * reg, uint32_t event);
static int32_t _R_PTP_Wait_Ext(volatile uint32_t * reg, uint32_t event);
static int32_t _R_PTP_InfoChk(void);
static void _R_PTP_Int_STCA(void);
static void _R_PTP_Int_PRC(void);
static void _R_PTP_Int_SYNFP0(void);
static void _R_PTP_Int_SYNFP1(void);
static void _R_PTP_Init_SYNFP(uint8_t ch, PTPPort* port);
static void _R_PTP_Init_PRC_STCA(STCA_GRAD mode, PTPPort port[]);

/**
 * Private global variables
 */

/* Pointer to MINT interrupt handler */
static MINT_HNDLR ptp_mint_isr[4] = {NULL, NULL, NULL, NULL};

/* Pointer to the timer interrupt handler */
static TMR_HNDLR ptp_tmr_isr[NUM_OF_TMR_CHANNEL];

/* timer intterupt chnnel */
static uint8_t ptp_tmr_ch;

/* Clock type and port number */
static PTPDevice s_device;

/* Delay correction protocol (E2E or P2P) */
static DelayMechanism s_delay[NUM_PORT]; /* 0: Port0, 1: Port1 */

/* Master or slave  */
static PTPState s_state[NUM_PORT]; /* 0: Port0, 1: Port1 */

/**
 * Renesas API functions
 */
/***********************************************************************************************************************
* Function Name: R_PTP_GetVersion
* Description  : Get PTP synchronization driver part version number.  
* Arguments    : None
* Return Value : PTP module version number
* Note         : This function is expanded as inline function.
***********************************************************************************************************************/
uint32_t R_PTP_GetVersion(void)
{
    uint32_t version;

    /* major version number (upper 16bit)*/
    version = ((uint16_t)RX_PTP_VERSION_MAJOR << 16u);
    
    /* mijor version number (lower 16bit) */
    version |= (uint16_t)RX_PTP_VERSION_MINOR;

    return version;
} /* End of function R_PTP_GetVersion() */

/***********************************************************************************************************************
* Function Name: R_PTP_Reset
* Description  : Reset EPTPC.  
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PTP_Reset(void)
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
	
} /* End of function R_PTP_Reset() */

/***********************************************************************************************************************
* Function Name: R_PTP_SetTran
* Description  : Set inter ports transfer mode.
* Arguments    : mode - Inter ports transfer mode
*                dir - Relay enable directions (bit map form)
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
* Note         : This function is valid only the standard Ethernet frames. (not valid for PTP message frames).
*                If the argument (mode or dir) is NULL pointer, the value does not set.
***********************************************************************************************************************/
ptp_return_t R_PTP_SetTran(TranMode *mode, RelEnabDir *dir)
{
    ptp_return_t ret = PTP_OK;

    if (NULL != dir)
    { /* Set/clear relay enable directions */
        if (*dir > 0x3)
        {
            ret = PTP_ERR_PARAM;
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

} /* End of R_PTP_SetTran() */

/***********************************************************************************************************************
* Function Name: R_PTP_SetMCFilter
* Description  : Set multicast frames (MC) filter (FFLTR).
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                fil - Multicast frames (MC) filter setting
*                fmac - Reception frame MAC address (register FMAC0R(U/L) or FMAC1R(U/L))
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
* Note         : This function is valid only the standard Ethernet frames. (not valid for PTP message frames). 
*                This function clears the extended promiscuous mode implicitly. 
*                If 3rd argument (fmac) is NULL pointer, the value does not set.
***********************************************************************************************************************/
ptp_return_t R_PTP_SetMCFilter(uint8_t ch, MCRecFil fil, uint32_t *fmac)
{
    ptp_return_t ret = PTP_OK;

	if (NUM_PORT <= ch)
	{
		return PTP_ERR_PARAM;
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
} /* End of R_PTP_SetMCFilter() */

/***********************************************************************************************************************
* Function Name: R_PTP_SetExtPromiscuous
* Description  : Set/clear extended promiscuous mode.
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                is_set - (true): set, (false): clear
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptp_return_t R_PTP_SetExtPromiscuous(uint8_t ch, bool is_set)
{
    ptp_return_t ret = PTP_OK;
    
    if (NUM_PORT <= ch)
	{
		return PTP_ERR_PARAM;
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
} /* End of R_PTP_SetExtPromiscuous() */

/***********************************************************************************************************************
* Function Name: R_PTP_Init
* Description  : Initialize EPTPC depends on the device configuration.
* Arguments    : tbl - PTP configuration table
* Return Value : PTP_OK         - Processing completed successfully
*                PTP_ERR_PARAM  - Parameter error
*                PTP_ERR        - Any error occured
***********************************************************************************************************************/
ptp_return_t R_PTP_Init(PTPConfig *tbl)
{
    uint8_t ch;

#if (1 == BSP_MCU_RX72M)
	EPTPC.SYBYPSR.LONG = 0x00000000; /* bypass off */
#endif

	/* Check configuration parameter */
	if ((PD_TRANSPARENT_CLOCK < tbl->device) || (NP_E2E < tbl->port[0].delay) || (NP_E2E < tbl->port[1].delay) 
	    || (ST_LIST < tbl->port[0].state) || (ST_LIST < tbl->port[1].state))
	{
		return PTP_ERR_PARAM;
	}

	/* Set device parameter */
    s_device = tbl->device;
	s_delay[0] = tbl->port[0].delay;
    s_delay[1] = tbl->port[1].delay;
	s_state[0] = tbl->port[0].state;
	s_state[1] = tbl->port[1].state;
	
    /* Select synchronization frame processing unit (SYNFP0 or SYNFP1) */
    if ((PD_ORDINARY_CLOCK_PORT1 == s_device) || ((PD_BOUNDARY_CLOCK == s_device) && (ST_SLAVE == s_state[1])))
	{ /* OC port1 or BC port1 slave (BC port0 master) */
		EPTPC.STCHSELR.BIT.SYSEL = 1; /* SYNFP1 */
	}
    else
	{ /* OC port0, BC master-master, BC port0 slave (BC port1 master) and TC */
		EPTPC.STCHSELR.BIT.SYSEL = 0; /* SYNFP0 */
	}

    /* Initialize the pointer to timer interrupt handler */
	/* WAIT_LOOP */
    for (ch = 0; ch < NUM_OF_TMR_CHANNEL; ch++)
    {
        ptp_tmr_isr[ch] = NULL;
    }

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
		_R_PTP_Init_SYNFP(ch, &(tbl->port[ch]));
	}

	/* Set PTP reception filters depend on the device type */
	switch(s_device)
	{
        case PD_ORDINARY_CLOCK_PORT0: /* OC port0 */
		
		    if (NP_P2P == s_delay[0])
		    { /* P2P */
			    if (ST_MASTER == s_state[0])
			    { /* master */
                    EPTPC0.SYRFL1R.LONG = 0x44400000;
                    EPTPC0.SYRFL2R.LONG = 0x00000011;
                    EPTPC0.SYCONFR.LONG = 0x00000028;
			    }
			    else if (ST_SLAVE == s_state[0])
			    { /* slave */
                    EPTPC0.SYRFL1R.LONG = 0x44400441;
                    EPTPC0.SYRFL2R.LONG = 0x00000011;
                    EPTPC0.SYCONFR.LONG = 0x00000028;
			    }
				else
				{ /* listening */
					EPTPC0.SYRFL1R.LONG = 0x44400001;
                    EPTPC0.SYRFL2R.LONG = 0x00000011;
                    EPTPC0.SYCONFR.LONG = 0x00000028;
			    }
		    }
		    else
		    { /* E2E */
			    if (ST_MASTER == s_state[0])
			    { /* master */
                    EPTPC0.SYRFL1R.LONG = 0x00004001;
                    EPTPC0.SYRFL2R.LONG = 0x00000011;
                    EPTPC0.SYCONFR.LONG = 0x00000028;
			    }
			    else if (ST_SLAVE == s_state[0])
			    { /* slave */
                    EPTPC0.SYRFL1R.LONG = 0x00040441;
                    EPTPC0.SYRFL2R.LONG = 0x00000011;
                    EPTPC0.SYCONFR.LONG = 0x00000028;
			    }
				else
				{ /* listening */
				    EPTPC0.SYRFL1R.LONG = 0x44400001;
                    EPTPC0.SYRFL2R.LONG = 0x00000011;
                    EPTPC0.SYCONFR.LONG = 0x00000028;
			    }
		    }
		
		break;
		
	    case PD_ORDINARY_CLOCK_PORT1: /* OC port1 */
		
		    if (NP_P2P == s_delay[1])
		    { /* P2P */
			    if (ST_MASTER == s_state[1])
			    { /* master */
                    EPTPC1.SYRFL1R.LONG = 0x44400000;
                    EPTPC1.SYRFL2R.LONG = 0x00000011;
                    EPTPC1.SYCONFR.LONG = 0x00000028;
			    }
			    else if (ST_SLAVE == s_state[1])
			    { /* slave */
                    EPTPC1.SYRFL1R.LONG = 0x44400441;
                    EPTPC1.SYRFL2R.LONG = 0x00000011;
                    EPTPC1.SYCONFR.LONG = 0x00000028;
			    }
				else
				{ /* listening */
				    EPTPC1.SYRFL1R.LONG = 0x44400001;
                    EPTPC1.SYRFL2R.LONG = 0x00000011;
                    EPTPC1.SYCONFR.LONG = 0x00000028;
			    }
		    }
		    else
		    { /* E2E */
			    if (ST_MASTER == s_state[1])
			    { /* master */
			        EPTPC1.SYRFL1R.LONG = 0x00004001;
				    EPTPC1.SYRFL2R.LONG = 0x00000011;
				    EPTPC1.SYCONFR.LONG = 0x00000028;
			    }
			    else if (ST_SLAVE == s_state[1])
			    { /* slave */
				    EPTPC1.SYRFL1R.LONG = 0x00040441;
				    EPTPC1.SYRFL2R.LONG = 0x00000011;
				    EPTPC1.SYCONFR.LONG = 0x00000028;
			    }
				else
				{ /* listening */
				    EPTPC1.SYRFL1R.LONG = 0x44400001;
                    EPTPC1.SYRFL2R.LONG = 0x00000011;
                    EPTPC1.SYCONFR.LONG = 0x00000028;
			    }
            }
		
		break;
		
	    case PD_BOUNDARY_CLOCK: /* BC */
		
		    /* Each CH setting */
			/* WAIT_LOOP */
			for (ch = 0; ch < NUM_PORT; ch++)
			{
		    	if (NP_P2P == s_delay[ch])
		    	{ /* P2P */
			    	if (ST_MASTER == s_state[ch])
			    	{ /* master */
			        	synfp[ch]->SYRFL1R.LONG = 0x44400000;
				    	synfp[ch]->SYRFL2R.LONG = 0x00000011;
				    	synfp[ch]->SYCONFR.LONG = 0x00000028;
			    	}
			    	else if (ST_SLAVE == s_state[ch])
			    	{ /* slave */
				    	synfp[ch]->SYRFL1R.LONG = 0x44400441;
				    	synfp[ch]->SYRFL2R.LONG = 0x00000011;
				    	synfp[ch]->SYCONFR.LONG = 0x00000028;
			    	}
					else
					{ /* listening */
				    	synfp[ch]->SYRFL1R.LONG = 0x44400001;
                    	synfp[ch]->SYRFL2R.LONG = 0x00000011;
                    	synfp[ch]->SYCONFR.LONG = 0x00000028;
			    	}
		    	}
		    	else
		    	{ /* E2E */
			    	if (ST_MASTER == s_state[ch])
			    	{ /* master */
			 	    	synfp[ch]->SYRFL1R.LONG = 0x00004001;
				    	synfp[ch]->SYRFL2R.LONG = 0x00000011;
				    	synfp[ch]->SYCONFR.LONG = 0x00000028;
			    	}
			    	else if (ST_SLAVE == s_state[ch])
			    	{ /* slave */
				    	synfp[ch]->SYRFL1R.LONG = 0x00040441;
				    	synfp[ch]->SYRFL2R.LONG = 0x00000011;
				    	synfp[ch]->SYCONFR.LONG = 0x00000028;
			    	}
					else
					{ /* listening */
				    	synfp[ch]->SYRFL1R.LONG = 0x44400001;
                    	synfp[ch]->SYRFL2R.LONG = 0x00000011;
                    	synfp[ch]->SYCONFR.LONG = 0x00000028;
			    	}
		    	}
			}
		
		break;
		
	    case PD_TRANSPARENT_CLOCK: /* TC */

			/* Each CH setting */
			/* WAIT_LOOP */
			for (ch = 0; ch < NUM_PORT; ch++)
			{
		    	if (NP_P2P == s_delay[0])
		    	{ /* P2P TC */
		        	synfp[ch]->SYRFL1R.LONG = 0x44400222;
			    	synfp[ch]->SYRFL2R.LONG = 0x20000033;
			    	synfp[ch]->SYCONFR.LONG = 0x00100028;
		    	}
		        else
				{ /* E2E TC */
			    	synfp[ch]->SYRFL1R.LONG = 0x22222222;
			    	synfp[ch]->SYRFL2R.LONG = 0x20000033;
			    	synfp[ch]->SYCONFR.LONG = 0x00000028;
		    	}
			}
		
		break;
		
	    default:
		
		break;
	}

	/* Validate set values to SYNFP0 and SYNFP1 */
	EPTPC0.SYRVLDR.LONG = 0x00000007;
	EPTPC1.SYRVLDR.LONG = 0x00000007;

	/* ==== Initialize packet relation controller unit (PRC-TC) and Statistical time correction algorithm unit(STCA) ==== */
#if (PTP_SYNC_MODE1 == PTP_CFG_SYNC_MODE)
    /* gradient collection not applied (not use STCA) */
	_R_PTP_Init_PRC_STCA(STCA_MODE1, tbl->port);
#elif (PTP_SYNC_MODE2_HW == PTP_CFG_SYNC_MODE)
    /* HW gradient collection (use STCA) */ 
	_R_PTP_Init_PRC_STCA(STCA_MODE2_HW, tbl->port);
#else /* (PTP_SYNC_MODE2_SW == PTP_CFG_SYNC_MODE) */
    /* SW gradient collection (use STCA) */
	_R_PTP_Init_PRC_STCA(STCA_MODE2_SW, tbl->port);
#endif

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

	EPTPC.MIEIPR.LONG = 0x003F000F;

	/* Set EPTPC interrupt */
    ptp_dev_start();

	return PTP_OK;
} /* End of R_PTP_Init() */

/***********************************************************************************************************************
* Function Name: R_PTP_SubConfig
* Description  : Set the optional configuration of EPTPC.
*                Set interval of getting worst10 vlues. (Recomend more than 32 times sync reception)
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                tbl - PTP optional configuration table
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptp_return_t R_PTP_SubConfig(uint8_t ch, PTPSubConfig *tbl)
{
	if (NUM_PORT <= ch)
	{
		return PTP_ERR_PARAM;
	}

	EPTPC.STMR.BIT.WINT = tbl->w10_times;

	return PTP_OK;
} /* End of R_PTP_SubConfig() */

/***********************************************************************************************************************
* Function Name: R_PTP_RegMINTHndr
* Description  : Set MINT interrupt and registers a user function to MINT interrupt handler of EPTPC.
* Arguments    : reg - MINT interrupt register.
*                    MINT_FUNC_STCA: Interrupt from STCA
*                    MINT_FUNC_PRC: Interrupt from PRC-TC
*                    MINT_FUNC_SYN0: Interrupt from SYNFP0
*                    MINT_FUNC_SYN1: Interrupt from SYNFP1
*                event - interrupt elements.
*                func - register function.
* Return Value : None
* Note         : Registered user function is updated if register the same MINT interrupt handler.
*                If 3rd argument(=func) is NULL, registered function is removed and the MINT interrupt is disabled.
***********************************************************************************************************************/
void R_PTP_RegMINTHndr(MINT_Reg reg, uint32_t event, MINT_HNDLR func)
{
	volatile unsigned long R_BSP_EVENACCESS_SFR *ctrl[4] = {&(EPTPC.STIPR.LONG), &(EPTPC.PRIPR.LONG), &(EPTPC0.SYIPR.LONG), &(EPTPC1.SYIPR.LONG)};

	/* Register the user function to the interrupt handler */
	ptp_mint_isr[reg] = func;
	
	if (NULL == func)
	{ /* Clear interrupt event */
	    *ctrl[reg] &= ~event;
	}
	else
	{
		*ctrl[reg] |= event;
	}
	
} /* End of R_PTP_RegMINTHndr() */

/***********************************************************************************************************************
* Function Name: R_PTP_RegTmrHndr
* Description  : Register a user function to timer interrupt handler of EPTPC.
* Arguments    : ch - timer interrupt channel
*                func - user function registered
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
* Note         : Registered user function is updated if register the same channel interrupt handler.
*                If 2nd argument(=func) is NULL, registered function is removed and that channel interrupt is disabled.
***********************************************************************************************************************/
ptp_return_t R_PTP_RegTmrHndr(IntCycCh ch, TMR_HNDLR func)
{
    int32_t idx; /* array index */

	if (INT_CYC5 < ch)
	{
		return PTP_ERR_PARAM;
	}

    /* Set timer interrupt channel of the registered handler */
    ptp_tmr_ch = ch;

	/* WAIT_LOOP */
    for (idx = 0; idx < NUM_OF_TMR_CHANNEL; idx++)
    { /* convert array index */
        if ((ch & 0x01) == 0x01)
        { /* Register the user function to the interrupt handler */
			ptp_tmr_isr[idx] = func;
            break;
        }
        ch >>= 1u;
    }
	
	if (NULL == func)
	{ /* Clear application interrupt channel */
	    EPTPC.IPTSELR.LONG &= ~((uint32_t)ptp_tmr_ch);
	}
	else
	{ /* Set application interrupt channel */
        EPTPC.IPTSELR.LONG |= (uint32_t)ptp_tmr_ch;
	}

	return PTP_OK;
} /* End of R_PTP_RegTmrHndr() */

/***********************************************************************************************************************
* Function Name: R_PTP_ELC_Ind
* Description  : Set/clear ELC interrupt indication. 
* Arguments    : ch - timer interrupt channel
*                edge - (0): rise, (1): fall
*                set - (0): clear, (1): set
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptp_return_t R_PTP_ELC_Ind(IntCycCh ch, uint8_t edge, uint8_t set)
{
	uint32_t elippr;
	uint32_t val = ch;

	if (INT_CYC5 < ch)
	{
		return PTP_ERR_PARAM;
	}

	elippr = EPTPC.ELIPPR.LONG; 
	
	if (1 == edge)
	{
		val <<= 8u;
	}
	
	if (1 == set)
	{
		elippr |= val;
	}
	else
	{
		elippr &= ~val;
	}
	
	EPTPC.ELIPPR.LONG = elippr;

	return PTP_OK;
} /* End of R_PTP_ELC_Ind() */

/***********************************************************************************************************************
* Function Name: R_PTP_ELC_SetClr
* Description  : Set/clear ELC interrupt auto clear mode. 
* Arguments    : ch - timer interrupt channel
*                edge - (0): rise, (1): fall
*                set - (0): clear, (1): set
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptp_return_t R_PTP_ELC_SetClr(IntCycCh ch, uint8_t edge, uint8_t set)
{
	uint32_t elipacr;
	uint32_t val = ch;

	if (INT_CYC5 < ch)
	{
		return PTP_ERR_PARAM;
	}

	elipacr = EPTPC.ELIPACR.LONG;
	
	if (1 == edge)
	{
		val <<= 8u;
	}
	
	if (1 == set)
	{
		elipacr |= val;
	}
	else
	{
		elipacr &= ~val;
	}
	
	EPTPC.ELIPACR.LONG = elipacr;

	return PTP_OK;
} /* End of R_PTP_ELC_SetClr() */

/***********************************************************************************************************************
* Function Name: R_PTP_Tmr_Set
* Description  : Enable timer interrupt handler (timer event).
* Arguments    : ch - timer interrupt channel
*                event - timer event time(ns)
*                cycle - pulse output cycle interval(ns)
*                pulse - pulse output pulse interval(ns)
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptp_return_t R_PTP_Tmr_Set(IntCycCh ch, UInt64 event, uint32_t cycle, uint32_t pulse)
{
	uint32_t tmp;

	if (INT_CYC5 < ch)
	{
		return PTP_ERR_PARAM;
	}

    /* Set the indication timer interrupt to CPU */
	tmp = EPTPC.MITSELR.LONG;
	EPTPC.MITSELR.LONG = (tmp | ch);

    /* Set event time, PWM cycle interval and PWM pulse interval to specified timer channel */
	switch(ch) 
	{
	    case INT_CYC0:
		
		    EPTPC.TMSTTRU0 = event.hi;
			EPTPC.TMSTTRL0 = event.lo;
			EPTPC.TMCYCR0.LONG = cycle;
			EPTPC.TMPLSR0.LONG = pulse;
		
		break;
		
	    case INT_CYC1:
		
		    EPTPC.TMSTTRU1 = event.hi;
		    EPTPC.TMSTTRL1 = event.lo;
		    EPTPC.TMCYCR1.LONG = cycle;
		    EPTPC.TMPLSR1.LONG = pulse;
			
		break;
		
	    case INT_CYC2:
		
		    EPTPC.TMSTTRU2 = event.hi;
		    EPTPC.TMSTTRL2 = event.lo;
		    EPTPC.TMCYCR2.LONG = cycle;
		    EPTPC.TMPLSR2.LONG = pulse;
			
		break;
		
	    case INT_CYC3:
		
		    EPTPC.TMSTTRU3 = event.hi;
		    EPTPC.TMSTTRL3 = event.lo;
		    EPTPC.TMCYCR3.LONG = cycle;
		    EPTPC.TMPLSR3.LONG = pulse;
		
		break;
		
	    case INT_CYC4:
		
		    EPTPC.TMSTTRU4 = event.hi;
		    EPTPC.TMSTTRL4 = event.lo;
		    EPTPC.TMCYCR4.LONG = cycle;
		    EPTPC.TMPLSR4.LONG = pulse;
		
		break;
	
		case INT_CYC5:
		
		    EPTPC.TMSTTRU5 = event.hi;
		    EPTPC.TMSTTRL5 = event.lo;
		    EPTPC.TMCYCR5.LONG = cycle;
		    EPTPC.TMPLSR5.LONG = pulse;
			
		break;
		
	    default:
		
		break;
	}

	/* Enable timer event */
	tmp = EPTPC.TMSTARTR.LONG;
	EPTPC.TMSTARTR.LONG = (tmp | ch);

	return PTP_OK;
} /* End of R_PTP_Tmr_Set() */

/***********************************************************************************************************************
* Function Name: R_PTP_GetLcClk
* Description  : Get current local clock counter value.
* Arguments    : clk - local clock
* Return Value : PTP_OK  - Processing completed successfully
*                PTP_ERR - Not implemented in this version
* Note         : Getting local counter value is added some retardation of the loading operation.
***********************************************************************************************************************/
ptp_return_t R_PTP_GetLcClk(Timestamp *clk)
{
	/* Request local clock counter value */
	EPTPC.GETINFOR.BIT.INFO = 1;
	
	/* Wait local clock counter loaded */
	_R_PTP_InfoChk();
	
	/* Save local clock counter */
	clk->nanosecondsField = EPTPC.LCCVRL;
	clk->secondsField.lo = EPTPC.LCCVRM;
	clk->secondsField.hi = EPTPC.LCCVRU.BIT.CNTU;

	return PTP_OK;
} /* End of R_PTP_GetLcClk() */

/***********************************************************************************************************************
* Function Name: R_PTP_SetLcClk
* Description  : Set local clock counter value.
* Arguments    : clk - local clock
* Return Value : PTP_OK  - Processing completed successfully
*                PTP_ERR - Not implemented in this version
* Note         : Master clock set the master time. 
*              : If slave clock was informed any timescale in advance, it is recommend to set the initial time based on 
*              : that timescale before starting synchronization.
***********************************************************************************************************************/
ptp_return_t R_PTP_SetLcClk(Timestamp *clk)
{
	/* Set local clock counter value */
	EPTPC.LCIVRU.BIT.VALU = clk->secondsField.hi;
    EPTPC.LCIVRM = clk->secondsField.lo;
    EPTPC.LCIVRL = clk->nanosecondsField;
    
    /* Load set value to local clock counter */
    EPTPC.LCIVLDR.BIT.LOAD = 1;
    
	return PTP_OK;
} /* End of R_PTP_SetLcClk() */

/***********************************************************************************************************************
* Function Name: R_PTP_ChkW10
* Description  : Wait worst10 values got and those values set as gradient limits.
* Arguments    : None
* Return Value : PTP_OK       - Processing completed successfully
*                PTP_ERR_TOUT - Timeout error
* Note         : This function is executed when gradient correction is applied and hardware worst10 setting
*                (=PTP_SYNC_MODE2_HW).
***********************************************************************************************************************/
ptp_return_t R_PTP_ChkW10(void)
{
    int32_t ret;

	/* Wait worst10 values got and those values set as gradient limits */
	ret = _R_PTP_Wait_Ext((volatile uint32_t *)&(EPTPC.STSR), 0x00000010);
    if (ret != 0)
    {
        return PTP_ERR_TOUT;
    }

	return PTP_OK;
} /* End of R_PTP_ChkW10() */

/***********************************************************************************************************************
* Function Name: R_PTP_GetW10
* Description  : Get current worst10 values.
* Arguments    : p_w10 - Plus (positive) gradient worst10 value
                 m_w10 - Minus (negative) gradient worst10 value
* Return Value : PTP_OK       - Processing completed successfully
*                PTP_ERR_TOUT - Timeout error
* Note         : If the argument (p_w10 or n_w10) is NULL pointer, the value does not get.
*                This function is executed when gradient correction is applied and software worst10 setting
*                (=PTP_SYNC_MODE2_SW).
***********************************************************************************************************************/
ptp_return_t R_PTP_GetW10(uint32_t *p_w10, uint32_t *m_w10)
{
    int32_t ret;

	/* Request current worst10 values got */
	EPTPC.GETW10R.BIT.GW10 = 1;
	
	/* Wait gradient worst10 values got */
	ret = _R_PTP_Wait_Ext((volatile uint32_t *)&(EPTPC.STSR), 0x00000010);
    if (ret != 0)
    {
        return PTP_ERR_TOUT;
    }

    /* Request gradient worst10 values loaded */
	EPTPC.GETINFOR.BIT.INFO = 1;
	
	/* Wait gradient worst10 value loaded */
	ret = _R_PTP_InfoChk();
    if (ret != 0)
    {
        return PTP_ERR_TOUT;
    }

    if (NULL != p_w10)
    { /* Save PW10VR high, middle and low field */
	    *p_w10 = EPTPC.PW10VRU;
        *(p_w10 + 1) = EPTPC.PW10VRM;
        *(p_w10 + 2) = EPTPC.PW10VRL;
    }
	
	if (NULL != m_w10)
    { /* Save MW10VR high, middle and low field */
	    *m_w10 = EPTPC.MW10RU;
        *(m_w10 + 1) = EPTPC.MW10RM;
        *(m_w10 + 2) = EPTPC.MW10RL;
    }

	return PTP_OK;
} /* End of R_PTP_GetW10() */

/***********************************************************************************************************************
* Function Name: R_PTP_SetGradLimit
* Description  : Set the gradient limit values.
* Arguments    : p_lim - plus (positive) gradient limit value
                 m_lim - minus (negative) gradient limit value
* Return Value : PTP_OK  - Processing completed successfully
*                PTP_ERR - Not implemented in this version
* Note         : If the argument (p_lim or n_lim) is NULL pointer, the value does not set.
*                This function is executed when gradient correction is applied and software worst10 setting
*                (=PTP_SYNC_MODE2_SW).
***********************************************************************************************************************/
ptp_return_t R_PTP_SetGradLimit(uint32_t *p_lim, uint32_t *m_lim)
{
    if (NULL != p_lim)
    { /* Set PLIMITR high, middle and low field */
	    EPTPC.PLIMITRU.BIT.LMTU = *p_lim;
        EPTPC.PLIMITRM = *(p_lim + 1);
        EPTPC.PLIMITRL = *(p_lim + 2);
    }
	
	if (NULL != m_lim)
    { /* Set MLIMITR high, middle and low field */
	    EPTPC.MLIMITRU.BIT.LMTU = *m_lim;
        EPTPC.MLIMITRM = *(m_lim + 1);
        EPTPC.MLIMITRL = *(m_lim + 2);
    }

	return PTP_OK;
} /* End of R_PTP_SetGradLimit() */

/***********************************************************************************************************************
* Function Name: R_PTP_GetMPortID
* Description  : Get Master port identity.
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                clk - Master clock identity field
*                port - Master port number field
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
* Note         : If the argument (clk or port) is NULL pointer, the value does not get.
***********************************************************************************************************************/
ptp_return_t R_PTP_GetMPortID(uint8_t ch, uint32_t *clk, uint16_t *port)
{	
	if (NUM_PORT <= ch)
	{
		return PTP_ERR_PARAM;
	}

    if (NULL != clk)
    { /* Get master clock id high and low field */
        *clk = synfp[ch]->MTCIDU;
        *(clk + 1) = synfp[ch]->MTCIDL;
    }
	if (NULL != port)
    { /* Get master port number field */
        *port = synfp[ch]->MTPID.BIT.PNUM;
    }
	
	return PTP_OK;
} /* End of R_PTP_GetMPortID() */

/***********************************************************************************************************************
* Function Name: R_PTP_SetMPortID
* Description  : Set Master port identity.
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                clk - Master clock identity
*                port - Master port number
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
* Note         : If the argument (clk or port) is NULL pointer, the value does not set.
***********************************************************************************************************************/
ptp_return_t R_PTP_SetMPortID(uint8_t ch, uint32_t *clk, uint16_t *port)
{	
	if (NUM_PORT <= ch)
	{
		return PTP_ERR_PARAM;
	}
	
    if (NULL != clk)
    { /* Set master clock id high and low field */
	    synfp[ch]->MTCIDU = *clk;
        synfp[ch]->MTCIDL = *(clk + 1);
    }
	if (NULL != port)
	{ /* Set master port number field */
	    synfp[ch]->MTPID.BIT.PNUM = *port;
    }
    if ((NULL != clk) || (NULL != port))
    { /* Validate set values to SYNFP0 */
		synfp[ch]->SYRVLDR.BIT.BMUP = 1;
    }

	return PTP_OK;
} /* End of R_PTP_SetMPortID() */

/***********************************************************************************************************************
* Function Name: R_PTP_GetSyncConfig
* Description  : Get synchronous configuration (SYRFL1R, SYRFL2R, SYTRENR and SYCONFR).
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                fil1 - SYRFL1R
*                fil2 - SYRFL2R
*                tren - SYTRENR
*                conf - SYCONFR
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
* Note         : Need to update synchronous configuration every when clock is changed such as BMC algorithm.
*                If the argument (fil1, fil2, tren or conf) is NULL pointer, the value does not get.
***********************************************************************************************************************/
ptp_return_t R_PTP_GetSyncConfig(uint8_t ch, uint32_t *fil1, uint32_t *fil2, uint32_t *tren, uint32_t *conf)
{	
	ptp_return_t ret = PTP_OK;
	
	if (NUM_PORT <= ch)
	{
		return PTP_ERR_PARAM;
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
} /* End of R_PTP_GetSyncConfig() */

/***********************************************************************************************************************
* Function Name: R_PTP_SetSyncConfig
* Description  : Set synchronous configuration (SYRFL1R, SYRFL2R, SYTRENR and SYCONFR).
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                fil1 - SYRFL1R
*                fil2 - SYRFL2R
*                tren - SYTRENR
*                conf - SYCONFR
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
* Note         : Need to update synchronous configuration every when clock is changed such as BMC algorithm.
*                If the argument (fil1, fil2, tren or conf) is NULL pointer, the value does not set.
***********************************************************************************************************************/
ptp_return_t R_PTP_SetSyncConfig(uint8_t ch, uint32_t *fil1, uint32_t *fil2, uint32_t *tren, uint32_t *conf)
{	
	ptp_return_t ret = PTP_OK;
	
	if (NUM_PORT <= ch)
	{
		return PTP_ERR_PARAM;
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
} /* End of R_PTP_SetSyncConfig() */

/***********************************************************************************************************************
* Function Name: R_PTP_GetSyncInfo
* Description  : Get current offsetFromMaster and meanPathDelay.
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                ofm - offsetFromMaster
*                mpd - meanPathDelay
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
*                If the argument (ofm or mpd) is NULL pointer, the value does not get.
***********************************************************************************************************************/
ptp_return_t R_PTP_GetSyncInfo(uint8_t ch, TimeInterval *ofm, TimeInterval *mpd)
{
	uint32_t reg_u; /* upper register absolute value */
	uint32_t reg_l; /* lower register absolute value */
	TimeInterval max_p = {SCALED_TIME_MAX_U, SCALED_TIME_MAX_L}; /* largest positive value */
	TimeInterval min_n = {SCALED_TIME_MIN_U, SCALED_TIME_MIN_L}; /* smallest minimum value */
	
	if (NUM_PORT <= ch)
	{
		return PTP_ERR_PARAM;
	}
	
    if (NULL != ofm)
    { /* Request offsetFromMaster */
	    reg_u = synfp[ch]->OFMRU;
		reg_l = synfp[ch]->OFMRL;
		if (0x80000000 == (reg_u & 0x80000000))
		{ /* negative */
			if (reg_u < MIN16_S)
		    { /* Under flow case, change smallest minimum value */
		    	ofm->scaledNanoseconds.hi = min_n.scaledNanoseconds.hi;
	        	ofm->scaledNanoseconds.lo = min_n.scaledNanoseconds.lo;
			}
			else
			{ /* Convert scaled nano second expression */
				ofm->scaledNanoseconds.hi = ((reg_u << 16u) | ((reg_l >> 16u) & 0xFFFF));
	        	ofm->scaledNanoseconds.lo = (reg_l << 16u);
			}
		}
		else
		{ /* positive */
		    if (reg_u > MAX16_S)
			{ /* Over flow case, change largest positive value */
			    ofm->scaledNanoseconds.hi = max_p.scaledNanoseconds.hi;
		        ofm->scaledNanoseconds.lo = max_p.scaledNanoseconds.lo;
			}
			else
			{ /* Convert scaled nano second expression */
				ofm->scaledNanoseconds.hi = ((reg_u << 16u) | ((reg_l >> 16u) & 0xFFFF));
	        	ofm->scaledNanoseconds.lo = (reg_l << 16u);
			}
		}
	}
	if (NULL != mpd)
    { /* Request meanPathDelay */
		reg_u = synfp[ch]->MPDRU;
		reg_l = synfp[ch]->MPDRL;
		if (0x80000000 == (reg_u & 0x80000000))
		{ /* negative */
			if (reg_u < MIN16_S)
		    { /* Under flow case, change smallest minimum value */
		    	mpd->scaledNanoseconds.hi = min_n.scaledNanoseconds.hi;
	        	mpd->scaledNanoseconds.lo = min_n.scaledNanoseconds.lo;
			}
			else
			{ /* Convert scaled nano second expression */
				mpd->scaledNanoseconds.hi = ((reg_u << 16u) | ((reg_l >> 16u) & 0xFFFF));
	        	mpd->scaledNanoseconds.lo = (reg_l << 16u);
			}
		}
		else
		{ /* positive */
		    if (reg_u > MAX16_S)
			{ /* Over flow case, change largest positive value */
			    mpd->scaledNanoseconds.hi = max_p.scaledNanoseconds.hi;
		        mpd->scaledNanoseconds.lo = max_p.scaledNanoseconds.lo;
			}
			else
			{ /* Convert scaled nano second expression */
				mpd->scaledNanoseconds.hi = ((reg_u << 16u) | ((reg_l >> 16u) & 0xFFFF));
	        	mpd->scaledNanoseconds.lo = (reg_l << 16u);
			}
		}
	}

	return PTP_OK;
} /* End of R_PTP_GetSyncInfo() */

/***********************************************************************************************************************
* Function Name: R_PTP_UpdClkID
* Description  : Update my clockIdentity.
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                id - clockIdentity
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
* Note         : Default value of clockIdentity is EUI-48 format. In case of selecting your own format, please execute this
*                function after PTP device configuration.
***********************************************************************************************************************/
ptp_return_t R_PTP_UpdClkID(uint8_t ch, int8_t *id)
{
	if (NUM_PORT <= ch)
	{
		return PTP_ERR_PARAM;
	}

	/* Update my clockIdentity field */
	synfp[ch]->SYCIDRU = ((*id << 24u) | ((*(id + 1) << 16u) & 0xFF0000) | ((*(id + 2) << 8u) & 0xFF00) | (*(id + 3) & 0xFF));
	synfp[ch]->SYCIDRL = ((*(id + 4) << 24u) | ((*(id + 5) << 16u) & 0xFF0000) | ((*(id + 6) << 8u) & 0xFF00) | (*(id + 7) & 0xFF));

	return PTP_OK;
} /* End of R_PTP_UpdClkID() */

/***********************************************************************************************************************
* Function Name: R_PTP_UpdDomainNum
* Description  : Update domainNumber field of common message header.
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                dnum - updating domainNumber value
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptp_return_t R_PTP_UpdDomainNum(uint8_t ch, uint8_t dnum)
{
	if (NUM_PORT <= ch)
	{
		return PTP_ERR_PARAM;
	}

	/* Update domainNumber fields */
	synfp[ch]->SYDOMR.BIT.DNUM = dnum;

	return PTP_OK;
} /* End of R_PTP_UpdDomainNum() */

/***********************************************************************************************************************
* Function Name: R_PTP_UpdAnceFlags
* Description  : Update Announce message's flag fields.
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                flags - updating flag values
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptp_return_t R_PTP_UpdAnceFlags(uint8_t ch, AnceFlag *flags)
{
	if (NUM_PORT <= ch)
	{
		return PTP_ERR_PARAM;
	}

	/* Update PTP profile Specific 2/1 fields */
	synfp[ch]->ANFR.BIT.FLAG14 = flags->BIT.profileSpec2;
	synfp[ch]->ANFR.BIT.FLAG13 = flags->BIT.profileSpec1;

	/* Update unicastFlag field */
	synfp[ch]->ANFR.BIT.FLAG10 = flags->BIT.unicastFlag;

	/* Update alternateMasterFlag field */
	synfp[ch]->ANFR.BIT.FLAG8 = flags->BIT.alternateMasterFlag;

	/* Update frequencyTraceable field */
	synfp[ch]->ANFR.BIT.FLAG5 = flags->BIT.frequencyTraceable;

	/* Update timeTraceable field */
	synfp[ch]->ANFR.BIT.FLAG4 = flags->BIT.timeTraceable;

	/* Update ptpTimescale field */
	synfp[ch]->ANFR.BIT.FLAG3 = flags->BIT.ptpTimescale;

	/* Update currentUtcOffsetValid field */
	synfp[ch]->ANFR.BIT.FLAG2 = flags->BIT.currentUtcOffsetValid;

	/* Update leap59 field */
	synfp[ch]->ANFR.BIT.FLAG1 = flags->BIT.leap59;

	/* Update leap61 field */
	synfp[ch]->ANFR.BIT.FLAG0 = flags->BIT.leap61;

	/* Validate setting values */
    synfp[ch]->SYRVLDR.BIT.ANUP = 1;

	return PTP_OK;
} /* End of R_PTP_UpdAnceFlags() */

/***********************************************************************************************************************
* Function Name: R_PTP_UpdAnceMsgs
* Description  : Update Announce message's message fields.
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                msgs - updating field values
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptp_return_t R_PTP_UpdAnceMsgs(uint8_t ch, AnceMsg *msgs)
{
    int8_t *id;

	if (NUM_PORT <= ch)
	{
		return PTP_ERR_PARAM;
	}

	/* Update grandmasterPriority2/1 fields */
	synfp[ch]->GMPR.BIT.GMPR1 = msgs->grandmasterPriority1;
	synfp[ch]->GMPR.BIT.GMPR2 = msgs->grandmasterPriority2;

	/* Update grandmasterClockQuality fields */
	synfp[ch]->GMCQR = ((msgs->grandmasterClockQuality.clockClass << 24u)) 
	    | ((msgs->grandmasterClockQuality.clockAccuracy << 16u) & 0xFF0000) 
		    | (msgs->grandmasterClockQuality.offsetScaledLogVariance & 0xFFFF);

	/* Update grandmasterIdentity fields */
    id = msgs->grandmasterIdentity;
	synfp[ch]->GMIDRU = ((*id << 24u) | ((*(id + 1) << 16u) & 0xFF0000) | ((*(id + 2) << 8u) & 0xFF00) | (*(id + 3) & 0xFF));
	synfp[ch]->GMIDRL = ((*(id + 4) << 24u) | ((*(id + 5) << 16u) & 0xFF0000) | ((*(id + 6) << 8u) & 0xFF00) | (*(id + 7) & 0xFF));

	/* Validate setting values */
    synfp[ch]->SYRVLDR.BIT.ANUP = 1;

	return PTP_OK;
} /* End of R_PTP_UpdAnceMsgs() */

/***********************************************************************************************************************
* Function Name: R_PTP_UpdSyncAnceInterval
* Description  : Update transmission interval and logMessageInterval of Sync and Announce messages.
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                sync - Sync transmission interval (2^(interval) sec)
*                ance - Announce transmission interval (2^(interval) sec)
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
*                If the argument (interval or tout) is NULL pointer, the value does not update.
***********************************************************************************************************************/
ptp_return_t R_PTP_UpdSyncAnceInterval(uint8_t ch, int8_t *sync, int8_t *ance)
{
	if (NUM_PORT <= ch)
	{
		return PTP_ERR_PARAM;
	}
	
	if (NULL != sync)
	{ 
		if (*sync < -7)
		{ /* Less than 2^(-7) case, set 7.8125 msec */
		    synfp[ch]->SYTLIR.BIT.SYNC = 0xF9;
		}
		else if (*sync >= 7)
		{ /* More than 2^(6) case, set 64 sec */
			synfp[ch]->SYTLIR.BIT.SYNC = 0x06;
		}
		else
		{
			synfp[ch]->SYTLIR.BIT.SYNC = *sync;
		}

		/* Validate Sync interval setting values */
	    synfp[ch]->SYRVLDR.BIT.STUP = 1;
	}

	if (NULL != ance)
	{ 
		if (*ance < -7)
		{ /* Less than 2^(-7) case, set 7.8125 msec */
		    synfp[ch]->SYTLIR.BIT.ANCE = 0xF9;
		}
		else if (*ance >= 7)
		{ /* More than 2^(6) case, set 64 sec */
			synfp[ch]->SYTLIR.BIT.ANCE = 0x06;
		}
		else
		{
			synfp[ch]->SYTLIR.BIT.ANCE = *ance;
		}

		/* Validate Announce interval setting values */
	    synfp[ch]->SYRVLDR.BIT.ANUP = 1;
	}

	return PTP_OK;
} /* End of R_PTP_UpdSyncAnceInterval() */

/***********************************************************************************************************************
* Function Name: R_PTP_UpdDelayMsgInterval
* Description  : Update transmission interval, logMessageInterval and timeout values of Delay messages.
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                interval - Case of master: Delay_Resp logMessageInterval (2^(interval) sec)
*                           Case of slave or listening: Delay_Req/Pdelay_Req transmission interval (2^(interval) sec)
*                tout - Delay_Resp/Pdelay_Resp receiving timeout (tout * 1024 nsec)
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
*                If the argument (interval or tout) is NULL pointer, the value does not update.
***********************************************************************************************************************/
ptp_return_t R_PTP_UpdDelayMsgInterval(uint8_t ch, int8_t *interval, uint32_t *tout)
{
	int8_t dresp;
	
	if (NUM_PORT <= ch)
	{
		return PTP_ERR_PARAM;
	}
	
    if (NULL != interval)
    {
		if (ST_MASTER == s_state[ch])
		{ /* master */
			if (*interval < -7)
			{ /* Less than 2^(-7) case, set 7.8125 msec */
		    	synfp[ch]->SYTLIR.BIT.DREQ = 0xF9;
			}
			else if (*interval >= 7)
			{ /* More than 2^(6) case, set 64 sec */
				synfp[ch]->SYTLIR.BIT.DREQ = 0x06;
			}
			else
			{
				synfp[ch]->SYTLIR.BIT.DREQ = *interval;
			}
		}
		else
		{ /* slave or listening */
			/* Update Delay_Req/Pdelay_Req transmission interval value */
	    	dresp = synfp[ch]->SYRLIR.BIT.DRESP;
			if (dresp < -7)
			{ /* Less than 2^(-7) case, set 7.8125 msec */
		    	synfp[ch]->SYTLIR.BIT.DREQ = 0xF9; /* = -7 */
			}
			else if (dresp >= 7)
			{ /* More than 2^(6) case, set 64 sec */
				synfp[ch]->SYTLIR.BIT.DREQ = 0x06;
			}
			else
			{ /* Set Delay_Resp/Pdelay_Resp field to Delay_Req/Pdelay_Req field */ 
				synfp[ch]->SYTLIR.BIT.DREQ = dresp;
			}
        	*interval = synfp[ch]->SYTLIR.BIT.DREQ;
		}
	}
	
	if (NULL != tout)
	{ /* Update Delay_Resp/Pdelay_Resp receiving timeout value */
	    synfp[ch]->RSTOUTR = *tout;
	}
	
	if ((NULL != interval) || (NULL != tout))
	{ /* Validate setting values */
	    synfp[ch]->SYRVLDR.BIT.STUP = 1;
	}

	return PTP_OK;
} /* End of R_PTP_UpdDelayMsgInterval() */

/***********************************************************************************************************************
* Function Name: R_PTP_Start
* Description  : Start synchronization. 
* Arguments    : None
* Return Value : PTP_OK       - Processing completed successfully
*                PTP_ERR_TOUT - Timeout error
*                PTP_ERR      - Any error occurred
* Note         : This function is valid when synchronization was not started.
***********************************************************************************************************************/
ptp_return_t R_PTP_Start(void)
{
	int32_t ret;

	switch(s_device) 
	{
	    case PD_ORDINARY_CLOCK_PORT0:

		    if (NP_P2P == s_delay[0])
		    { /* P2P */
			    if (ST_MASTER == s_state[0])
			    { /* master */
				    EPTPC0.SYTRENR.LONG = 0x00001011;
			    }
			    else if (ST_SLAVE == s_state[0])
			    { /* slave */
				    EPTPC0.SYTRENR.LONG = 0x00001000;
					/* Check offsetFromMaster has been updated */
					ret = _R_PTP_Wait((volatile uint32_t *)&(EPTPC0.SYSR), 0x00000001);
					if (ret != 0)
    				{
        				return PTP_ERR_TOUT; /* Timeout error */
    				}
			    }
				else
				{ /* listening */
				    return PTP_ERR;
			    }
		    }
		    else
		    { /* E2E */
			    if (ST_MASTER == s_state[0])
			    { /* master */
				    EPTPC0.SYTRENR.LONG = 0x00000011;
			    }
			    else if (ST_SLAVE == s_state[0])
			    { /* slave */
				    EPTPC0.SYTRENR.LONG = 0x00000100;
                    /* Check offsetFromMaster has been updated */
					ret = _R_PTP_Wait((volatile uint32_t *)&(EPTPC0.SYSR), 0x00000001);
					if (ret != 0)
    				{
        				return PTP_ERR_TOUT; /* Timeout error */
    				}
				}
				else
				{ /* listening */
				    return PTP_ERR;
			    }
		    }

		break;
		
	    case PD_ORDINARY_CLOCK_PORT1:

		    if (NP_P2P == s_delay[1])
		    { /* P2P */
			    if (ST_MASTER == s_state[1])
			    { /* master */
				    EPTPC1.SYTRENR.LONG = 0x00001011;
			    }
			    else if (ST_SLAVE == s_state[1])
			    { /* slave */
				    EPTPC1.SYTRENR.LONG = 0x00001000;
					/* Check offsetFromMaster has been updated */
					ret = _R_PTP_Wait((volatile uint32_t *)&(EPTPC1.SYSR), 0x00000001);
					if (ret != 0)
    				{
        				return PTP_ERR_TOUT; /* Timeout error */
    				}
			    }
				else
				{ /* listening */
				    return PTP_ERR;
			    }
		    }
		    else
		    { /* E2E */
			    if (ST_MASTER == s_state[1])
			    { /* master */
				    EPTPC1.SYTRENR.LONG = 0x00000011;
			    }
			    else if (ST_SLAVE == s_state[1])
			    { /* slave */
				    EPTPC1.SYTRENR.LONG = 0x00000100;
                    /* Check offsetFromMaster has been updated */
					ret = _R_PTP_Wait((volatile uint32_t *)&(EPTPC1.SYSR), 0x00000001);
					if (ret != 0)
    				{
        				return PTP_ERR_TOUT; /* Timeout error */
    				}
			    }
				else
				{ /* listening */
				    return PTP_ERR;
			    }
		    }

		break;
		
	    case PD_BOUNDARY_CLOCK:

		    /* set port0 */
		    if (NP_P2P == s_delay[0])
		    { /* P2P */ 
			    if (ST_MASTER == s_state[0])
			    { /* master */
				    EPTPC0.SYTRENR.LONG = 0x00001011;
			    }
			    else if (ST_SLAVE == s_state[0])
			    { /* slave */
				    EPTPC0.SYTRENR.LONG = 0x00001000;
  					/* Check offsetFromMaster has been updated */
					ret = _R_PTP_Wait((volatile uint32_t *)&(EPTPC0.SYSR), 0x00000001);
					if (ret != 0)
    				{
        				return PTP_ERR_TOUT; /* Timeout error */
    				}
			    }
				else
				{ /* listening */
				    return PTP_ERR;
			    }
		    }
		    else 
		    { /* E2E */
			    if (ST_MASTER == s_state[0])
			    { /* master */
				    EPTPC0.SYTRENR.LONG = 0x00000011;
			    }
			    else if (ST_SLAVE == s_state[0])
			    { /* slave */
				    EPTPC0.SYTRENR.LONG = 0x00000100;
                    /* Check offsetFromMaster has been updated */
					ret = _R_PTP_Wait((volatile uint32_t *)&(EPTPC0.SYSR), 0x00000001);
					if (ret != 0)
    				{
        				return PTP_ERR_TOUT; /* Timeout error */
    				}
			    }
				else
				{ /* listening */
				    return PTP_ERR;
			    }
		    }

		    /* set port1 */
		    if (NP_P2P == s_delay[1])
		    { /* P2P */
			    if (ST_MASTER == s_state[1])
			    { /* master */
				    EPTPC1.SYTRENR.LONG = 0x00001011;
			    }
			    else if (ST_SLAVE == s_state[1])
			    { /* slave */
				    EPTPC1.SYTRENR.LONG = 0x00001000;
					/* Check offsetFromMaster has been updated */
					ret = _R_PTP_Wait((volatile uint32_t *)&(EPTPC1.SYSR), 0x00000001);
					if (ret != 0)
    				{
        				return PTP_ERR_TOUT; /* Timeout error */
    				}
				}
				else
				{ /* listening */
				    return PTP_ERR;
			    }
		    }
		    else
		    { /* E2E */
			    if (ST_MASTER == s_state[1])
			    { /* master */
				    EPTPC1.SYTRENR.LONG = 0x00000011;
			    }
			    else if (ST_SLAVE == s_state[1])
			    { /* slave */
				    EPTPC1.SYTRENR.LONG = 0x00000100;
                    /* Check offsetFromMaster has been updated */
					ret = _R_PTP_Wait((volatile uint32_t *)&(EPTPC1.SYSR), 0x00000001);
					if (ret != 0)
    				{
        				return PTP_ERR_TOUT; /* Timeout error */
    				}
			    }
				else
				{ /* listening */
				    return PTP_ERR;
			    }
		    }
			
		break;
		
	    case PD_TRANSPARENT_CLOCK:
		
		    if (NP_P2P == s_delay[0])
		    { /* P2P */
			    EPTPC0.SYTRENR.LONG = 0x00001000;
			    EPTPC1.SYTRENR.LONG = 0x00001000;
		    }
		    else
		    { /* E2E */
			    EPTPC0.SYTRENR.LONG = 0x00000000;
			    EPTPC1.SYTRENR.LONG = 0x00000000;
		    }
			
		break;
		
	    default:
		
		break;
	}

	/* Validate set values to SYNFP0 and SYNFP1 */
	EPTPC0.SYRVLDR.BIT.STUP = 1;
	EPTPC1.SYRVLDR.BIT.STUP = 1;

	if ((ST_SLAVE == s_state[0]) || (ST_SLAVE == s_state[1]))
	{ /* slave */
		EPTPC.SYNSTARTR.BIT.STR = 1;
	}

	return PTP_OK;
} /* End of R_PTP_Start() */

/***********************************************************************************************************************
* Function Name: R_PTP_Stop
* Description  : Stop synchronization. 
* Arguments    : None
* Return Value : PTP_OK  - Processing completed successfully
*                PTP_ERR - Any error occurred
* Note         : This function is valid when synchronization was started.
***********************************************************************************************************************/
ptp_return_t R_PTP_Stop(void)
{	
    switch(s_device)
    {
	    case PD_ORDINARY_CLOCK_PORT0:
		
		    if (NP_P2P == s_delay[0])
		    { /* P2P */
			    if (ST_MASTER == s_state[0])
			    { /* master */
				    EPTPC0.SYTRENR.LONG = 0x00001000;
				    EPTPC0.SYRVLDR.BIT.STUP = 1;
				    _R_PTP_Wait((volatile uint32_t *)&(EPTPC0.SYSR), 0x00020000);
			    }
			    else if (ST_SLAVE == s_state[0])
			    { /* slave */
				    EPTPC.SYNSTARTR.BIT.STR = 0;
				    EPTPC0.SYRFL1R.LONG = 0x44400001;
				    EPTPC0.SYRVLDR.BIT.STUP = 1;
			    }
				else
				{ /* listening */
				    return PTP_ERR;
			    }
			    EPTPC0.SYRFL1R.LONG = 0x00000001;
		        EPTPC0.SYTRENR.LONG = 0x00000000;
			    EPTPC0.SYRVLDR.BIT.STUP = 1;
			    _R_PTP_Wait((volatile uint32_t *)&(EPTPC0.SYSR), 0x00030000);
		    }
		    else
		    { /* E2E */
			    if (ST_MASTER == s_state[0])
			    { /* master */
				    EPTPC0.SYRFL1R.LONG = 0x00000001;
				    EPTPC0.SYTRENR.LONG = 0x00000000;
				    EPTPC0.SYRVLDR.BIT.STUP = 1;
				    _R_PTP_Wait((volatile uint32_t *)&(EPTPC0.SYSR), 0x00030000);
			    }
			    else if (ST_SLAVE == s_state[0])
			    { /* slave */
			        EPTPC.SYNSTARTR.BIT.STR = 0;
				    EPTPC0.SYRFL1R.LONG = 0x00000001;
				    EPTPC0.SYTRENR.LONG = 0x00000000;
				    EPTPC0.SYRVLDR.BIT.STUP = 1;
				    _R_PTP_Wait((volatile uint32_t *)&(EPTPC0.SYSR), 0x00020000);
			    }
				else
				{ /* listening */
					return PTP_ERR;
                }
		    }
		
        break;
		
	    case PD_ORDINARY_CLOCK_PORT1:
		
		    if (NP_P2P == s_delay[1])
		    { /* P2P */
			    if (ST_MASTER == s_state[1])
			    { /* master */
				    EPTPC1.SYTRENR.LONG = 0x00001000;
				    EPTPC1.SYRVLDR.BIT.STUP = 1;
				    _R_PTP_Wait((volatile uint32_t *)&(EPTPC1.SYSR), 0x00020000);
			    }
			    else if (ST_SLAVE == s_state[1])
			    { /* slave */
				    EPTPC.SYNSTARTR.BIT.STR = 0;
				    EPTPC1.SYRFL1R.LONG = 0x44400001;
				    EPTPC1.SYRVLDR.BIT.STUP = 1;
			    }
				else
				{ /* listening */
				    return PTP_ERR;
			    }
			    EPTPC1.SYRFL1R.LONG = 0x00000001;
		        EPTPC1.SYTRENR.LONG = 0x00000000;
			    EPTPC1.SYRVLDR.BIT.STUP = 1;
			    _R_PTP_Wait((volatile uint32_t *)&(EPTPC1.SYSR), 0x00030000);
		    }
		    else
		    { /* E2E */
			    if (ST_MASTER == s_state[1])
			    { /* master */
				    EPTPC1.SYRFL1R.LONG = 0x00000001;
				    EPTPC1.SYTRENR.LONG = 0x00000000;
				    EPTPC1.SYRVLDR.BIT.STUP = 1;
				    _R_PTP_Wait((volatile uint32_t *)&(EPTPC1.SYSR), 0x00030000);
			    }
			    else if (ST_SLAVE == s_state[1])
			    { /* slave */
				    EPTPC.SYNSTARTR.BIT.STR = 0;
				    EPTPC1.SYRFL1R.LONG = 0x00000001;
				    EPTPC1.SYTRENR.LONG = 0x00000000;
				    EPTPC1.SYRVLDR.BIT.STUP = 1;
				    _R_PTP_Wait((volatile uint32_t *)&(EPTPC1.SYSR), 0x00020000);
			    }
				else
				{  /* listening */
				    return PTP_ERR;
                }
		    }
		
		    break;
		
        case PD_BOUNDARY_CLOCK:
		
	        /* set port0 */
            if (NP_P2P == s_delay[0])
	        { /* P2P */
		        if (ST_MASTER == s_state[0])
		        { /* master */
			        EPTPC0.SYTRENR.LONG = 0x00001000;
			        EPTPC0.SYRVLDR.BIT.STUP = 1;
			        _R_PTP_Wait((volatile uint32_t *)&(EPTPC0.SYSR), 0x00020000);
		        }
		        else if (ST_SLAVE == s_state[0])
		        { /* slave */
			         EPTPC.SYNSTARTR.BIT.STR = 0;
			         EPTPC0.SYRFL1R.LONG = 0x44400001;
			         EPTPC0.SYRVLDR.BIT.STUP = 1;
		        }
				else
				{ /* listening */
				    return PTP_ERR;
			    }
                EPTPC0.SYRFL1R.LONG = 0x00000001;
			    EPTPC0.SYTRENR.LONG = 0x00000000;
                EPTPC0.SYRVLDR.BIT.STUP = 1;
                _R_PTP_Wait((volatile uint32_t *)&(EPTPC0.SYSR), 0x00030000);
            }
            else 
            { /* E2E */
		        if (ST_MASTER == s_state[0])
                { /* master */
                    EPTPC0.SYRFL1R.LONG = 0x00000001;
                    EPTPC0.SYTRENR.LONG = 0x00000000;
                    EPTPC0.SYRVLDR.BIT.STUP = 1;
                    _R_PTP_Wait((volatile uint32_t *)&(EPTPC0.SYSR), 0x00030000);
                }
                else if (ST_SLAVE == s_state[0])
                { /* slave */
                    EPTPC.SYNSTARTR.BIT.STR = 0;
                    EPTPC0.SYRFL1R.LONG = 0x00000001;
                    EPTPC0.SYTRENR.LONG = 0x00000000;
                    EPTPC0.SYRVLDR.BIT.STUP = 1;
                    _R_PTP_Wait((volatile uint32_t *)&(EPTPC0.SYSR), 0x00020000);
                }
				else
                { /* listening */
                    return PTP_ERR;
                }
            }

		    /* set port1 */
            if (NP_P2P == s_delay[1])
            { /* P2P */
                if (ST_MASTER == s_state[1])
                { /* master */
                    EPTPC1.SYTRENR.LONG = 0x00001000;
                    EPTPC1.SYRVLDR.BIT.STUP = 1;
                    _R_PTP_Wait((volatile uint32_t *)&(EPTPC1.SYSR), 0x00020000);
                }
                else if (ST_SLAVE == s_state[1])
                { /* slave */
                    EPTPC.SYNSTARTR.BIT.STR = 0;
                    EPTPC1.SYRFL1R.LONG = 0x44400001;
                    EPTPC1.SYRVLDR.BIT.STUP = 1;
                }
				else
				{ /* listening */
				    return PTP_ERR;
			    }
                EPTPC1.SYRFL1R.LONG = 0x00000001;
                EPTPC1.SYTRENR.LONG = 0x00000000;
                EPTPC1.SYRVLDR.BIT.STUP = 1;
                _R_PTP_Wait((volatile uint32_t *)&(EPTPC1.SYSR), 0x00030000);
            }
		    else
		    { /* E2E */
			    if (ST_MASTER == s_state[1])
			    { /* master */
		            EPTPC1.SYRFL1R.LONG = 0x00000001;
                    EPTPC1.SYTRENR.LONG = 0x00000000;
                    EPTPC1.SYRVLDR.BIT.STUP = 1;
                    _R_PTP_Wait((volatile uint32_t *)&(EPTPC1.SYSR), 0x00030000);
			    }
                else if (ST_SLAVE == s_state[1])
                { /* slave */
                    EPTPC.SYNSTARTR.BIT.STR = 0;
                    EPTPC1.SYRFL1R.LONG = 0x00000001;
                    EPTPC1.SYTRENR.LONG = 0x00000000;
                    EPTPC1.SYRVLDR.BIT.STUP = 1;
                    _R_PTP_Wait((volatile uint32_t *)&(EPTPC1.SYSR), 0x00020000);
                }
				else
                { /* listening */
                    return PTP_ERR;
                }
            }
		
        break;
		
	    case PD_TRANSPARENT_CLOCK:
		    /* no operation */
		break;
	
		default:
		
		break;
	}

	return PTP_OK;
} /* End of R_PTP_Stop() */

/***********************************************************************************************************************
* Function Name: R_PTP_SetPortState
* Description  : Set PTP port state.
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                state - updating port state (ST_MASTER, ST_SLAVE or ST_LIST)
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptp_return_t R_PTP_SetPortState(uint8_t ch, PTPState state)
{	
	ptp_return_t ret = PTP_OK;
	
	if ((NUM_PORT <= ch) || (ST_LIST < state))
	{
		return PTP_ERR_PARAM;
	}
	
    s_state[ch] = state;
	
	return ret;
} /* End of R_PTP_SetPortState() */

/***********************************************************************************************************************
* Function Name: R_PTP_GetSyncCH
* Description  : Get current synchronous channel.
* Arguments    : None
* Return Value : Current synchronous channel.
***********************************************************************************************************************/
uint32_t R_PTP_GetSyncCH(void)
{
    return (EPTPC.STCHSELR.BIT.SYSEL); /* SYNFP0 or SYNFP1 */
}

/***********************************************************************************************************************
* Function Name: R_PTP_SetInterrupt
* Description  : Enable EPTPC INFABT interrupt  
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptp_return_t R_PTP_SetInterrupt(uint8_t ch)
{
	if (NUM_PORT <= ch)
	{
		return PTP_ERR_PARAM;
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

	return PTP_OK;
} /* End of function R_PTP_SetInterrupt() */

/***********************************************************************************************************************
* Function Name: R_PTP_ChkInterrupt
* Description  : Check INFABT interrupt occurred or not?
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                is_det - INFABT interrupt detection flag
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptp_return_t R_PTP_ChkInterrupt(uint8_t ch, bool *is_det)
{
	if (NUM_PORT <= ch)
	{
		return PTP_ERR_PARAM;
	}

	*is_det = is_INFABT[ch];

	return PTP_OK;
} /* End of function R_PTP_ChkInterrupt() */

/***********************************************************************************************************************
* Function Name: R_PTP_ClrInterrupt
* Description  : Clear INFABT interrupt occurrence flag.
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptp_return_t R_PTP_ClrInterrupt(uint8_t ch)
{
	if (NUM_PORT <= ch)
	{
		return PTP_ERR_PARAM;
	}

	is_INFABT[ch] = false;

	return PTP_OK;
} /* End of function R_PTP_ClrInterrupt() */

/***********************************************************************************************************************
* Function Name: R_PTP_DisableTmr
* Description  : Disable timer event interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PTP_DisableTmr(void)
{
	uint32_t mieipr;
	
	mieipr = EPTPC.MIEIPR.LONG;
	mieipr &= ~(ptp_tmr_ch << 16u);
	
	/* Disable timer event interrupt to CPU */
	EPTPC.MIEIPR.LONG = mieipr;

} /* End of function R_PTP_DisableTmr() */

/***********************************************************************************************************************
* Function Name: R_PTP_SetSyncDet
* Description  : Set detect condition of change of synchronous state.
* Arguments    : dev - Condition of deviation state detection
*                syn - Condition of synchronous state detection
*                is_enb - Alarm enable or not: (1) enable, (0) disable
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptp_return_t R_PTP_SetSyncDet(SyncDet *dev, SyncDet *syn, bool is_enab)
{
	if ((dev->times > 0xF) || (syn->times > 0xF))
	{
		return PTP_ERR_PARAM;
	}

	/* Deviation state */
	EPTPC.SYNTDARU = dev->th_val.hi;
	EPTPC.SYNTDARL = dev->th_val.lo;
	EPTPC.STMR.BIT.DVTH = dev->times;

	/* Synchronous state */
	EPTPC.SYNTDBRU = syn->th_val.hi;
	EPTPC.SYNTDBRL = syn->th_val.lo;
	EPTPC.STMR.BIT.SYTH = syn->times;

	/* Set or clear alarm indication */
	EPTPC.STMR.BIT.ALEN0 = (uint32_t)is_enab;

	return PTP_OK;
} /* End of R_PTP_SetSyncDet() */

/***********************************************************************************************************************
* Function Name: R_PTP_SetSynctout
* Description  : Set detect condition of sync message reception timeout.
* Arguments    : tout - Sync message reception timeout value (1024ns unit)
*                is_enb - Alarm enable or not: (1) enable, (0) disable
* Return Value : PTP_OK        - Processing completed successfully
*                PTP_ERR_PARAM - Parameter error
***********************************************************************************************************************/
ptp_return_t R_PTP_SetSynctout(uint32_t tout, bool is_enab)
{
	/* Set timeout value */
	EPTPC.SYNTOR = tout;

	EPTPC.STMR.BIT.ALEN1 = (uint32_t)is_enab;

	return PTP_OK;
} /* End of R_PTP_SetSynctout() */

/***********************************************************************************************************************
* Function Name: Eptpc_isr
* Description  : Interrupt handler of EPTPC (synchronization related events).
* Arguments    : None
* Return Value : None
* Note         : If user wants to add their own operation as timer event interrupt handler, 
*                please adds or modifis the operations in the sample function (=CPU_Timer_Ope).
*                If user wants to add their own operation as STCA status change interrupt handler, 
*                please adds their own interrupt handler and register it.
*                If user wants to add their own operation as SYNFP0/1 status change interrupt handler, please adds
*                or modifis the operations in the sample function (=_R_PTP_Int_SYNFP0/1).
*                If user wants to add their own operation as PRC-TC status change interrupt handler, 
*                please adds their own interrupt handler and register it.
***********************************************************************************************************************/
void Eptpc_isr(void)
{
	uint32_t miesr, cyc;
    int32_t idx; /* array index */
	
	miesr = EPTPC.MIESR;

	if (ptp_tmr_ch != 0)
	{ /* Any timer channel set */
	    cyc = ((miesr >> 16u) & 0x3f);
	    if (ptp_tmr_ch == cyc) 
	    {
		    if (ptp_tmr_isr != NULL) 
		    {
				/* WAIT_LOOP */
                for (idx = 0; idx < NUM_OF_TMR_CHANNEL; idx++)
                { /* convert array index */
                    if ((cyc & 0x01) == 0x01)
                    {
						ptp_tmr_isr[idx](cyc);
                        break;
                    }
                    cyc >>= 1u;
                }
		    }
		}
	}
	
	if ((miesr & 0x00000001) != 0x00000000)	/* STCA */
	{
		_R_PTP_Int_STCA();
	}

	if ((miesr & 0x00000002) != 0x00000000) /* SYNFP0 */
	{
		_R_PTP_Int_SYNFP0();
	}

	if ((miesr & 0x00000004) != 0x00000000) /* SYNFP1 */
	{
		_R_PTP_Int_SYNFP1();
	}

	if ((miesr & 0x00000008) != 0x00000000) /* PRC-TC */
	{
		_R_PTP_Int_PRC();
	}

	EPTPC.MIESR = miesr;

} /* End of Eptpc_isr() */

/***********************************************************************************************************************
* Function Name: _R_PTP_Wait
* Description  : Wait PTP event operation completed. (timeout 100msec)
* Arguments    : reg - register
*                event - evet bits
* Return Value : (0) - complete without error
*                (-1) - error
***********************************************************************************************************************/
static int32_t _R_PTP_Wait(volatile uint32_t * reg, uint32_t event)
{	
#if (1 == BSP_MCU_RX64M)
	volatile int32_t retry_cnt = 12000000; /* 12M count (= more than 100msec) */
#else /* ((1 == BSP_MCU_RX71M) || (1 == BSP_MCU_RX72M)) */
    volatile int32_t retry_cnt = 24000000; /* 24M count (= more than 100msec) */
#endif
    volatile uint32_t tmp;

	/* WAIT_LOOP */
	do
	{ /* wait generation stoped */
		tmp = *reg;
		if ((tmp & event) == event)
		{
			*reg = (tmp & event);
			return (0);
		}
		retry_cnt--;
	} while(retry_cnt > 0);

    return (-1); /* timeout error */
} /* End of _R_PTP_Wait() */

/***********************************************************************************************************************
* Function Name: _R_PTP_Wait_Ext
* Description  : Wait PTP event operation completed. (timeout 400sec)
* Arguments    : reg - register
*                event - evet bits
* Return Value : (0) - complete without error
*                (-1) - error
***********************************************************************************************************************/
static int32_t _R_PTP_Wait_Ext(volatile uint32_t * reg, uint32_t event)
{
    volatile int32_t i, j;
    int32_t sec_cnt = 4000; /* 400sec (=4000*100msec) */
#if (1 == BSP_MCU_RX64M)
	int32_t unit_cnt = 12000000; /* 12M count (= more than 100msec) */
#else /* ((1 == BSP_MCU_RX71M) || (1 == BSP_MCU_RX72M)) */
    int32_t unit_cnt = 24000000; /* 24M count (= more than 100msec) */
#endif
    volatile uint32_t tmp;

	/* WAIT_LOOP */
    for (i = sec_cnt; i > 0; i--)
    {
		for (j = unit_cnt; j > 0; j--)
		{ /* wait generation stoped */
			tmp = *reg;
			if ((tmp & event) == event)
			{
				*reg = (tmp & event);
				return (0);
			}
		}
    }

	return (-1); /* timeout error */
} /* End of _R_PTP_Wait_Ext() */

/***********************************************************************************************************************
* Function Name: _R_PTP_InfoChk
* Description  : Wait PTP event operation completed. (timeout 100msec)
* Arguments    : None
* Return Value : (0) - complete without error
*                (-1) - error
***********************************************************************************************************************/
static int32_t _R_PTP_InfoChk(void)
{
#if (1 == BSP_MCU_RX64M)
	volatile int32_t retry_cnt = 12000000; /* 12M count (= more than 100msec) */
#else /* ((1 == BSP_MCU_RX71M) || (1 == BSP_MCU_RX72M)) */
    volatile int32_t retry_cnt = 24000000; /* 24M count (= more than 100msec) */
#endif

	/* WAIT_LOOP */
    do
    {
        if (0x00000000 == (EPTPC.GETINFOR.LONG & 0x00000001))
        {
            return (0);
        }
        retry_cnt--;
    } while(retry_cnt > 0);

    return (-1); /* timeout error */
}  /* End of _R_PTP_InfoChk() */

/***********************************************************************************************************************
* Function Name: _R_PTP_Int_STCA
* Description  : Interrupt handler of STCA.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void _R_PTP_Int_STCA(void)
{
	uint32_t stsr;

	stsr = EPTPC.STSR;

	if (ptp_mint_isr[MINT_FUNC_STCA] != NULL)
	{
		ptp_mint_isr[MINT_FUNC_STCA](stsr);
	}

	EPTPC.STSR = stsr;
} /* End of _R_PTP_Int_STCA() */

/***********************************************************************************************************************
* Function Name: _R_PTP_Int_PRC
* Description  : Interrupt handler of PRC-TC.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void _R_PTP_Int_PRC(void)
{
	uint32_t prsr;

	prsr = EPTPC.PRSR;

	if (ptp_mint_isr[MINT_FUNC_PRC] != NULL)
	{
		ptp_mint_isr[MINT_FUNC_PRC](prsr);
	}

	EPTPC.PRSR = prsr;
} /* End of _R_PTP_Int_PRC() */

/***********************************************************************************************************************
* Function Name: _R_PTP_Int_SYNFP0
* Description  : Interrupt handler of SYNFP0
*              : Set INFABT interrupt occurrence flag of SYNFP0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void _R_PTP_Int_SYNFP0(void)
{
	uint32_t sy0sr;

	sy0sr = EPTPC0.SYSR;

	if ((sy0sr & 0x00004000) == 0x00004000) 
	{
		is_INFABT[0] = true;
	}

	if (ptp_mint_isr[MINT_FUNC_SYN0] != NULL)
	{
		ptp_mint_isr[MINT_FUNC_SYN0](EPTPC0.SYSR);
	}

	EPTPC0.SYSR = sy0sr;
} /* End of _R_PTP_Int_SYNFP0() */

/***********************************************************************************************************************
* Function Name: _R_PTP_Int_SYNFP1
* Description  : Interrupt handler of SYNFP1
*              : Set INFABT interrupt occurrence flag of SYNFP1.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void _R_PTP_Int_SYNFP1(void)
{
	uint32_t sy1sr;

	sy1sr = EPTPC1.SYSR;

	if ((sy1sr & 0x00004000) == 0x00004000) 
	{
		is_INFABT[1] = true;
	}

	if (ptp_mint_isr[MINT_FUNC_SYN1] != NULL)
	{
		ptp_mint_isr[MINT_FUNC_SYN1](EPTPC1.SYSR);
	}
	
	EPTPC1.SYSR = sy1sr;
} /* End of _R_PTP_Int_SYNFP1() */

/***********************************************************************************************************************
* Function Name: _R_PTP_Init_Param_SYNFP
* Description  : Set SYNFP parameters by initial values.
* Arguments    : ch - Sync unit channel (SYNFP0 or SYNFP1)
*                port - PTP port related information
* Return Value : None
***********************************************************************************************************************/
static void _R_PTP_Init_SYNFP(uint8_t ch, PTPPort* port)
{
	union 
	{
		uint32_t All ;
		struct 
		{
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

#if ((1 == BSP_MCU_RX71M) || (1 == BSP_MCU_RX72M))
	/* Set LLC-CTL filed (SYLLCCTLR) */
	synfp[ch]->SYLLCCTLR = (uint32_t)PTP_CFG_LLC_CTL;
#endif

	/* Set IP address (SYIPADDRR) */
	LDATA.B.b3 = port->ipAddr[0];
	LDATA.B.b2 = port->ipAddr[1];
	LDATA.B.b1 = port->ipAddr[2];
	LDATA.B.b0 = port->ipAddr[3];
	synfp[ch]->SYIPADDRR = LDATA.All;

	/* Set PTP version (SYSPVRR) */
	synfp[ch]->SYSPVRR.BIT.VER = PTP_CFG_PTP_VER_NUM;

	/* Set domain number (SYDOMR) */
	synfp[ch]->SYDOMR.BIT.DNUM = PTP_CFG_DOMAIN_NUM;

	/* Set message flag (ANFR, SYNFR, DYRQFR and DYRPFR) */
	synfp[ch]->ANFR.LONG = PTP_CFG_ANNOUNCE_FLAG_FIELD; /* Announce message */
	synfp[ch]->SYNFR.LONG = PTP_CFG_SYNC_FLAG_FIELD; /* Sync message */
	synfp[ch]->DYRQFR.LONG = PTP_CFG_DELAY_REQ_FLAG_FIELD; /* Delay_Req/Pdelay_Req message */
	synfp[ch]->DYRPFR.LONG = PTP_CFG_DELAY_RESP_FLAG_FIELD; /* Delay_Resp/Pdelay_Resp message */

	/* Set PortIdentity clock-ID (SYCIDRU and SYCIDRL)  */
#if (CLK_ID_EUI48_BASE == PTP_CFG_CLK_ID)
	/* Create clock-ID from MAC address */
	/* Vendor ID (3byte) - FF - FE - Product ID (3byte) */
	LDATA.B.b3 = port->macAddr[0];
	LDATA.B.b2 = port->macAddr[1];
	LDATA.B.b1 = port->macAddr[2];
	LDATA.B.b0 = 0xFF;
	synfp[ch]->SYCIDRU = LDATA.All;

	LDATA.B.b3 = 0xFE;
	LDATA.B.b2 = port->macAddr[3];
	LDATA.B.b1 = port->macAddr[4];
	LDATA.B.b0 = port->macAddr[5];
	synfp[ch]->SYCIDRL = LDATA.All;
#else /* (CLK_ID_USR_DEFINE == PTP_CFG_CLOCK_ID) */
    /* Set user specific value */
	synfp[ch]->SYCIDRU = PORTID_CLK_ID_U[ch];
	synfp[ch]->SYCIDRL = PORTID_CLK_ID_L[ch];
#endif

	/* Set PortIdentity port number (SYPNUMR) */
	synfp[ch]->SYPNUMR.BIT.PNUM = PORTID_PORT_NUM[ch];

	/* Initialize messages transmission (SYTRENR) */
	synfp[ch]->SYTRENR.LONG = 0x00000000; /* Disable Announce, Sync, Delay_Req and Pdelay_Req */

	/* Initialize synchronize master PortIdentity */
	synfp[ch]->MTCIDU = PTP_CFG_MTCID_U;
	synfp[ch]->MTCIDL = PTP_CFG_MTCID_L;
	synfp[ch]->MTPID.BIT.PNUM = PTP_CFG_MTPID;

	/* Initialize transmission interval (SYTLIR) */
	synfp[ch]->SYTLIR.BIT.ANCE = ANNOUNCE_INTERVAL[ch];
	synfp[ch]->SYTLIR.BIT.SYNC = SYNC_INTERVAL[ch];
	if (NP_P2P == s_delay[ch])
	{ /* P2P */
		synfp[ch]->SYTLIR.BIT.DREQ = PDELAY_REQ_INTERVAL[ch];
	}
	else
	{ /* E2E */
		synfp[ch]->SYTLIR.BIT.DREQ = DELAY_REQ_INTERVAL[ch];
	}

	/* Set grandmasterPriority1/2 (GMPR) */
	synfp[ch]->GMPR.BIT.GMPR1 = GM_PRIORITY1[ch];
	synfp[ch]->GMPR.BIT.GMPR2 = GM_PRIORITY2[ch];

	/* Set grandmasterClockQuality (GMCQR) */
	synfp[ch]->GMCQR = GM_CLK_QUALITY[ch];

	/* Set grandmasterIdentity (GMIDRU and GMIDRL) */
	synfp[ch]->GMIDRU = GM_CLK_ID_U[ch];
	synfp[ch]->GMIDRL = GM_CLK_ID_L[ch];

	/* Set currentUtcOffset and timeSource (GMCQR) */
	synfp[ch]->CUOTSR.BIT.CUTO = CUR_UTC_OFFSET[ch];
	synfp[ch]->CUOTSR.BIT.TSRC = TIME_SOURCE[ch];

	/* Set stepsRemoved (SRR) */
	synfp[ch]->SRR.BIT.SRMV = STEPS_REMOVED[ch];

	/* Set PTP message MAC address (PPMACRU, PPMACRL, PDMACRU and PDMACRL) */
	/* PTP-primary messages (but for peer delay messages): 01-1B-19-00-00-00 */
	synfp[ch]->PPMACRU.LONG = 0x00011B19;
    synfp[ch]->PPMACRL.LONG = 0x00000000;
	/* PTP-pdelay messages: 01-80-C2-00-00-0E */
	synfp[ch]->PDMACRU.LONG = 0x000180C2;
	synfp[ch]->PDMACRL.LONG = 0x0000000E;

	/* Set PTP Ethertype (PETYPER) */
	synfp[ch]->PETYPER.BIT.TYPE = 0x88F7;

	/* Set PTP primary/pdelay message ip address setting */
	/* PTP-primary messages (but for peer delay messages): E0-00-01-81 (224.0.1.129) */
	synfp[ch]->PPIPR = 0xE0000181;
	/* PTP-pdelay messages: E0-00-00-6B (224.0.0.107) */
	synfp[ch]->PDIPR = 0xE000006B;

	/* Set PTP message TOS and TTL fields (PETOSR, PGTOSR, PPTTLR and PDTTLR) */
	synfp[ch]->PETOSR.BIT.EVTO = PTP_EVENT_TOS[ch];
	synfp[ch]->PGTOSR.BIT.GETO = PTP_GENERAL_TOS[ch];
	synfp[ch]->PPTTLR.BIT.PRTL = PTP_PRIMARY_TTL[ch];
	synfp[ch]->PDTTLR.BIT.PDTL = PTP_PDELAY_TTL[ch];

	/* Clear extended promiscuous mode (FFLTR) */
	synfp[ch]->FFLTR.BIT.EXTPRM = 0;

	/* Set UDP port number (PEUDPR and PGUDPR) */
	synfp[ch]->PEUDPR.BIT.EVUPT = 0x013F; /* 319(=0x13F) for event messages */
	synfp[ch]->PGUDPR.BIT.GEUPT = 0x0140; /* 320(=0x140) for general messages */

	/* Set asymmetric delay, prohibit but for 0 (always symmetric) (DASYMRU and DASYMRL) */
	synfp[ch]->DASYMRU.BIT.ASYMU = 0x0000;
	synfp[ch]->DASYMRL = 0x00000000;

	/* Set timestamp latency of ingress and egress ports (TSLATR) */
	synfp[ch]->TSLATR.LONG = PTP_CFG_TIMESTAMP_LATENCY;

	/* Set frame format of PTP transmission message (SYFORMR) */
#if (PTP_MSG_FORM_ETH == PTP_CFG_MSG_FORM)
	synfp[ch]->SYFORMR.LONG = 0x00000000;
#elif (PTP_MSG_FORM_ETH_8023 == PTP_CFG_MSG_FORM)
	synfp[ch]->SYFORMR.LONG = 0x00000001;
#elif (PTP_MSG_FORM_UDP4 == PTP_CFG_MSG_FORM)
	synfp[ch]->SYFORMR.LONG = 0x00000002;
#else /* (PTP_MSG_FORM_UDP4_8023 == PTP_CFG_MSG_FORM) */
	synfp[ch]->SYFORMR.LONG = 0x00000003;
#endif

	/* Initialize reception timeout of PTP messages (RSTOUTR), 1024 nsec unit */
	synfp[ch]->RSTOUTR = 0x00200000; /* 2097152(=0x200000)*1024 nsec, approx 2 sec */

} /* End of _R_PTP_Init_SYNFP() */

/***********************************************************************************************************************
* Function Name: _R_PTP_Init_PRC_STCA
* Description  : Set PRC-TC and STCA parameters by initial values.
* Arguments    : mode - synchronize correction mode
*                port - PTP port related information
* Return Value : None
***********************************************************************************************************************/
static void _R_PTP_Init_PRC_STCA(STCA_GRAD mode, PTPPort port[])
{
	uint8_t offset;
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

	switch(mode) 
	{
	    case STCA_MODE1: /* Mode1 (not use STCA) */
		
		    EPTPC.STMR.LONG = 0x00000000;
		
		break;
		
	    case STCA_MODE2_HW: /* Mode2 (use STCA) and HW gradient setting */
		
		    EPTPC.STMR.LONG = 0x00002000;
		
		break;
		
	    default: /* Mode2 (use STCA) and SW gradient setting */
		    
			EPTPC.STMR.LONG = 0x0000A000;
		
		break;
	}

    EPTPC.SYNTDARU = 0x00000002;  /* H'0000_0002_0000_0000 nsec, approx 8 sec */
    EPTPC.SYNTDARL = 0x00000000; 
    EPTPC.SYNTDBRU = 0x00000001;  /* H'0000_0001_0000_0000 nsec, approx 4 sec */
    EPTPC.SYNTDBRL = 0x00000000;

	EPTPC.STCSELR.LONG = 0x00000006; /* PCLKA supply, 6times divided */

	EPTPC.STCFR.BIT.STCF = 0; /* STCA clock 20MHz  */

	if ((ST_SLAVE == s_state[0]) || (ST_SLAVE == s_state[1]))
	{ /* Slave */ 
	    EPTPC.SYNTOR = PTP_CFG_SYNC_TIMEOUT;
	}
	else
	{ /* Master */
		EPTPC.LCIVRU.BIT.VALU = PTP_CFG_LCCLK_SEC_HI; 
		EPTPC.LCIVRM = PTP_CFG_LCCLK_SEC_LO;
		EPTPC.LCIVRL = PTP_CFG_LCCLK_NANO;
        EPTPC.LCIVLDR.BIT.LOAD = 1; /* load to local clock */
	}

	EPTPC.IPTSELR.LONG = 0x00000000;
	EPTPC.MITSELR.LONG = 0x00000000;
	EPTPC.ELTSELR.LONG = 0x00000000;

	EPTPC.SYNSTARTR.BIT.STR = 0;
	
	for (offset = 0; offset < 5; offset += 0x10) 
	{
		*(&(EPTPC.TMSTTRU0) + offset) = 0x00000000;
		*(&(EPTPC.TMSTTRL0) + offset) = 0x00000000;
	    *(&(EPTPC.TMCYCR0.LONG) + offset) = 0x00000000;
        *(&(EPTPC.TMPLSR0.LONG) + offset) = 0x00000000;
	}
} /* End of _R_PTP_Init_PRC_STCA() */
/* End of File */
