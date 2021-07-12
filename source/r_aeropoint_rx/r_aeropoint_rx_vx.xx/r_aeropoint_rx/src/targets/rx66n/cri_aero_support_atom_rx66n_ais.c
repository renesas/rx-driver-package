/****************************************************************************
 * CRI Middleware SDK
 * Copyright (c) 2019 CRI Middleware Co., Ltd.
 * File     : cri_aero_support_atom_rx66n_ais.c
 ****************************************************************************/
#include "platform.h"

#ifdef BSP_MCU_RX66N

#include "cri_aero_config_ais.h"
#include "cri_aero_support_atom_internal.h"

#if CRI_AERO_CONFIG_AIS_USE_DAMP

#include "platform.h"                                    
#include "cri_atom_rxv3.h"
#include "cri_aero_stream_internal.h" 
#include "r_gpio_rx_if.h"                                
#include "r_mpc_rx_if.h"                                 


#define GTP_TIMER_CLOCK (120000000L) 
#define OUTPUT_SAMPLING_RATE (24000) 
#define INPUT_SAMPLING_RATE (24000) 
#define NUM_CHANNELS (1) 
#define BITS_PER_SAMPLE (16) 

#define GPTW_CHANNEL_NUMBER (1)
#define _0F_GPT_PRIORITY_LEVEL0 (0x00F)
#define _0F_GPT_PRIORITY_LEVEL15 (0x0FU)
#define GTCIV1_INT_NUMBER (65)


const CriSfrConfig_RXV3 SFR_CONFIG = {
	GTP_TIMER_CLOCK,
	CRISFR_PWM_MODE_ONE,
	GPTW_CHANNEL_NUMBER,
	&GPTW1.GTADSMR.LONG,
	&GPTW1.GTBER.LONG,
	&GPTW1.GTCCRA,
	&GPTW1.GTCCRB,
	&GPTW1.GTCCRC,
	&GPTW1.GTCCRD,
	&GPTW1.GTCCRE,
	&GPTW1.GTCCRF,
	&GPTW1.GTCLR.LONG,
	&GPTW1.GTCNT,
	&GPTW1.GTCR.LONG,
	&GPTW1.GTCSR.LONG,
	&GPTW1.GTDNSR.LONG,
	&GPTW1.GTEITC.LONG,
	&GPTW1.GTINTAD.LONG,
	&GPTW1.GTIOR.LONG,
	&GPTW1.GTITC.LONG,
	&GPTW1.GTPR,
	&GPTW1.GTPSR.LONG,
	&GPTW1.GTSSR.LONG,
	&GPTW1.GTSTP.LONG,
	&GPTW1.GTSTR.LONG,
	&GPTW1.GTUDDTYC.LONG,
	&GPTW1.GTUPSR.LONG,
	&GPTW1.GTWP.LONG,
};

const CriAtomConfig_RXV3 LIB_CONFIG = {
	{
		CRIATOM_THREAD_MODEL_SINGLE,
		SERVER_FREQUENCY,
		CRI_NULL,
		CRI_NULL,
	},
	OUTPUT_SAMPLING_RATE,
	CRIATOM_DEFAULT_OUTPUT_CHANNELS,
	{
		&SFR_CONFIG,
	}
};

const CriAtomHcaPlayerConfig PLAYER_CONFIG = {
	NUM_CHANNELS,
	INPUT_SAMPLING_RATE,
	CRI_FALSE,
	CRIATOM_SOUND_RENDERER_DEFAULT,
	0,
};



void INT_GPT_GTCIV(void);

static void init_port_gtioc1a_gtioc2a(void);
static void finish_port_gtioc1a_gtioc2a(void);


void CriAeroSupportAtom_HardwareInitialize(void)
{
#if ( CRI_AERO_DAMP_GPTW_CH != 1 || CRI_AERO_DAMP_GPTW_INT_TABLE != 255 )
#error Not supported.
#endif
	{

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
		bsp_int_ctrl_t int_ctrl;
#endif

		R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
		R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif

		MSTP(GPTW) = 0;

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
		R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif

		R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);


	}

	{
		CriAeroSupportAtom_EnableInterrupt_AIS();

		ICU.SLIAR255.BYTE = GTCIV1_INT_NUMBER;
		IPR(PERIA, INTA255) = _0F_GPT_PRIORITY_LEVEL15;
		IR(PERIA, INTA255) = 0U;
		IEN(PERIA, INTA255) = 1U;
	}

	{
		init_port_gtioc1a_gtioc2a();
	}
}

void CriAeroSupportAtom_HardwareFinalize(void)
{
	{
		finish_port_gtioc1a_gtioc2a();
	}

	{
		IPR(PERIA, INTA255) = _0F_GPT_PRIORITY_LEVEL0;
		IEN(PERIA, INTA255) = 0U;
	}

	{

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
		bsp_int_ctrl_t int_ctrl;
#endif

		R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
		R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif

		MSTP(GPTW) = 1;

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
		R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif

		R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);


	}
}

static void init_port_gtioc1a_gtioc2a(void)
{

#if 1
	mpc_config_t config;

	config.analog_enable = false;
	config.irq_enable = false;
	config.pin_function = 0x1EU;

	R_GPIO_PinControl(CRI_AERO_PIN_CONFIG_DAMP_A, GPIO_CMD_ASSIGN_TO_GPIO);
	R_GPIO_PinControl(CRI_AERO_PIN_CONFIG_DAMP_B, GPIO_CMD_ASSIGN_TO_GPIO);

	R_MPC_Write(CRI_AERO_PIN_CONFIG_DAMP_A, &config);
	R_MPC_Write(CRI_AERO_PIN_CONFIG_DAMP_B, &config);

	R_GPIO_PinControl(CRI_AERO_PIN_CONFIG_DAMP_A, GPIO_CMD_ASSIGN_TO_PERIPHERAL);
	R_GPIO_PinControl(CRI_AERO_PIN_CONFIG_DAMP_B, GPIO_CMD_ASSIGN_TO_PERIPHERAL);

#else
	PORTD.PDR.BIT.B0 = 1;
	PORTD.PMR.BIT.B0 = 1;
	PORTD.PDR.BIT.B1 = 1;
	PORTD.PMR.BIT.B1 = 1;

	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;

	MPC.PD1PFS.BYTE = 0x1EU;
	MPC.PD0PFS.BYTE = 0x1EU;

	MPC.PWPR.BIT.PFSWE = 0;
	MPC.PWPR.BIT.B0WI = 1;
#endif
}

static void finish_port_gtioc1a_gtioc2a(void)
{
#if 1
	mpc_config_t config;

	config.analog_enable = false;
	config.irq_enable = false;
	config.pin_function = 0x0;

	R_GPIO_PinControl(CRI_AERO_PIN_CONFIG_DAMP_A, GPIO_CMD_ASSIGN_TO_GPIO);
	R_GPIO_PinControl(CRI_AERO_PIN_CONFIG_DAMP_B, GPIO_CMD_ASSIGN_TO_GPIO);

	R_MPC_Write(CRI_AERO_PIN_CONFIG_DAMP_A, &config);
	R_MPC_Write(CRI_AERO_PIN_CONFIG_DAMP_B, &config);

#else
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;

	MPC.PD1PFS.BYTE = 0x0U;
	MPC.PD0PFS.BYTE = 0x0U;

	MPC.PWPR.BIT.PFSWE = 0;
	MPC.PWPR.BIT.B0WI = 1;

	PORTD.PDR.BIT.B0 = 0;
	PORTD.PMR.BIT.B0 = 0;
	PORTD.PDR.BIT.B1 = 0;
	PORTD.PMR.BIT.B1 = 0;
#endif
}

#pragma interrupt INT_GPT_GTCIV(vect = VECT(PERIA, INTA255))
void INT_GPT_GTCIV(void)
{
	criAtom_ExecuteIntr_RXV3();
}

void CriAeroSupportAtom_Finalize(void)
{
	criAtom_Finalize_RXV3();
}

CriUint32 CriAeroSupportAtom_CalculateWorkSize(void)
{
	return criAtom_CalculateWorkSize_RXV3(&LIB_CONFIG);
}

void CriAeroSupportAtom_Initialize(void* pWork, CriSint32 WorkSize)
{
	criAtom_Initialize_RXV3(&LIB_CONFIG, pWork, WorkSize);
}

CriAeroSupportAtomPlayerConfig CriAeroSupportAtom_GetPlayerConfig(void)
{
	return (CriAeroSupportAtomPlayerConfig)&PLAYER_CONFIG;
}

#else

void CriAeroSupportAtom_HardwareInitialize(void)
{
}

void CriAeroSupportAtom_HardwareFinalize(void)
{
}

void CriAeroSupportAtom_Initialize(void* work, CriSint32 work_size)
{
}

void CriAeroSupportAtom_Finalize(void)
{
}

CriUint32 CriAeroSupportAtom_CalculateWorkSize(void)
{
	return 0;
}

void CriAeroSupportAtom_SetData(CriAeroSupportAtomHandle Handle, void *pData, CriSint32 Size)
{

}

CriAeroSupportAtomPlayerConfig CriAeroSupportAtom_GetPlayerConfig(void)
{
	return (CriAeroSupportAtomPlayerConfig)0;
}



#endif

#endif
