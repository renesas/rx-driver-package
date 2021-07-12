/****************************************************************************
 * CRI Middleware SDK
 * Copyright (c) 2019 CRI Middleware Co., Ltd.
 * File     : cri_aero_support_atom_rx_65n_ais.c
 ****************************************************************************/
#include "platform.h"

#ifdef BSP_MCU_RX65N

#include "cri_aero_config_ais.h"
#include "cri_aero_support_atom_internal.h"

#if CRI_AERO_CONFIG_AIS_USE_DAMP

#include "cri_atom_rxv2.h"
#include "cri_aero_stream_internal.h"
#include "r_gpio_rx_if.h"                                
#include "r_mpc_rx_if.h"                                 


#define USE_RESET_SYNC_MODE


#define TIMER_CLOCK				(120000000L)
#define OUTPUT_SAMPLING_RATE	(24000)
#define INPUT_SAMPLING_RATE		(24000)
#define NUM_CHANNELS			(1)
#define BITS_PER_SAMPLE			(16)

const CriSfrConfig_RXV2 SFR_CONFIG = {
	TIMER_CLOCK,

	CRISFR_PWM_MODE_RESET_SYNC,

	&MTU.TSTRA.BYTE,
	&MTU.TOERA.BYTE,
	&MTU.TOCR1A.BYTE,
	&MTU.TRWERA.BYTE,
	&MTU.TGCRA.BYTE,
	&MTU.TITCR1A.BYTE,

	3,
	4,

	&MTU3.TCR.BYTE,
	&MTU3.TMDR1.BYTE,
	&MTU3.TIORH.BYTE,
	&MTU3.TIORL.BYTE,
	&MTU3.TIER.BYTE,
	&MTU3.TSR.BYTE,
	&MTU3.TCNT,
	&MTU3.TGRA,
	&MTU3.TGRB,
	&MTU3.TGRC,
	&MTU3.TGRD,
	&MTU3.TBTM.BYTE,

	&MTU4.TCR.BYTE,
	&MTU4.TMDR1.BYTE,
	&MTU4.TIORH.BYTE,
	&MTU4.TIORL.BYTE,
	&MTU4.TIER.BYTE,
	&MTU4.TSR.BYTE,
	&MTU4.TCNT,
	&MTU3.TGRB,
	&MTU4.TGRB,
	&MTU3.TGRD,
	&MTU4.TGRD,
	&MTU4.TBTM.BYTE,
};

const CriAtomConfig_RXV2 LIB_CONFIG = {
	{
		CRIATOM_THREAD_MODEL_SINGLE,
		SERVER_FREQUENCY,
		NULL,
		NULL,
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


#pragma interrupt (INT_MTU3_TGIA3(vect=BSP_MAPPED_INT_CFG_A_VECT_MTU3_TGIA3))


void INT_MTU3_TGIA3(void);

static void init_port_tioc4a_tioc4b(void);
static void finish_port_tioc4a_tioc4b(void);


void CriAeroSupportAtom_HardwareInitialize(void)
{
	{
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
		bsp_int_ctrl_t int_ctrl;
#endif

        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif

        MSTP(MTU3) = 0;

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
		R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif

        R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

	}

	{
        CriAeroSupportAtom_EnableInterrupt_AIS();

        ICU.IPR[BSP_MAPPED_INT_CFG_A_VECT_MTU3_TGIA3].BIT.IPR |= 0x0F;
        R_BSP_InterruptRequestEnable(BSP_MAPPED_INT_CFG_A_VECT_MTU3_TGIA3);

	}
	{
#if defined(USE_RESET_SYNC_MODE)
		init_port_tioc4a_tioc4b();
#else
		init_port_tioc3a_tioc4a();
#endif
	}
	}

void CriAeroSupportAtom_HardwareFinalize(void)
{
	{
#if defined(USE_RESET_SYNC_MODE)
		finish_port_tioc4a_tioc4b();
#else
		finish_port_tioc3a_tioc4a();
#endif
	}

	{
        R_BSP_InterruptRequestDisable(BSP_MAPPED_INT_CFG_A_VECT_MTU3_TGIA3);
        ICU.IPR[BSP_MAPPED_INT_CFG_A_VECT_MTU3_TGIA3].BIT.IPR &= ~0x0F;
	}

	{
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
		bsp_int_ctrl_t int_ctrl;
#endif

        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif

		MSTP(MTU3) = 1;

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
		R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif

        R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

	}
}

#if defined(USE_RESET_SYNC_MODE)
static void init_port_tioc4a_tioc4b(void)
{

#if 1
    mpc_config_t damp_a_config;
    mpc_config_t damp_b_config;

    damp_a_config.analog_enable = false;
    damp_a_config.irq_enable = false;

    if (GPIO_PORT_1_PIN_6 == CRI_AERO_PIN_CONFIG_DAMP_A)
    {
        damp_a_config.pin_function = 0x2;
    }
    else if ((GPIO_PORT_E_PIN_0 == CRI_AERO_PIN_CONFIG_DAMP_A)
            || (GPIO_PORT_8_PIN_6 == CRI_AERO_PIN_CONFIG_DAMP_A))
    {
        damp_a_config.pin_function = 0x8;
    }
    else
    {
        damp_a_config.pin_function = 0x1;
    }

    damp_b_config.analog_enable = false;
    damp_b_config.irq_enable = false;

    if (GPIO_PORT_1_PIN_6 == CRI_AERO_PIN_CONFIG_DAMP_B)
    {
        damp_b_config.pin_function = 0x2;
    }
    else if ((GPIO_PORT_E_PIN_0 == CRI_AERO_PIN_CONFIG_DAMP_B)
            || (GPIO_PORT_8_PIN_6 == CRI_AERO_PIN_CONFIG_DAMP_B))
    {
        damp_b_config.pin_function = 0x8;
    }
    else
    {
        damp_b_config.pin_function = 0x1;
    }

    R_GPIO_PinControl(CRI_AERO_PIN_CONFIG_DAMP_A, GPIO_CMD_ASSIGN_TO_GPIO);
    R_GPIO_PinControl(CRI_AERO_PIN_CONFIG_DAMP_B, GPIO_CMD_ASSIGN_TO_GPIO);

    R_MPC_Write(CRI_AERO_PIN_CONFIG_DAMP_A, &damp_a_config);
    R_MPC_Write(CRI_AERO_PIN_CONFIG_DAMP_B, &damp_b_config);

    R_GPIO_PinControl(CRI_AERO_PIN_CONFIG_DAMP_A, GPIO_CMD_ASSIGN_TO_PERIPHERAL);
    R_GPIO_PinControl(CRI_AERO_PIN_CONFIG_DAMP_B, GPIO_CMD_ASSIGN_TO_PERIPHERAL);

#else
	PORTB.PDR.BYTE |= 0x40;
	PORTB.PMR.BYTE |= 0x40;
	PORT5.PDR.BYTE |= 0x20;
	PORT5.PMR.BYTE |= 0x20;

	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;

	MPC.PB6PFS.BYTE = 0x01;
	MPC.P55PFS.BYTE = 0x01;

	MPC.PWPR.BIT.PFSWE = 0;
	MPC.PWPR.BIT.B0WI = 1;
#endif

}

static void finish_port_tioc4a_tioc4b(void)
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

	MPC.PB6PFS.BYTE = 0x00;
	MPC.P55PFS.BYTE = 0x00;

	MPC.PWPR.BIT.PFSWE = 0;
	MPC.PWPR.BIT.B0WI = 1;

	PORTB.PDR.BYTE &= ~0x40;
	PORTB.PMR.BYTE &= ~0x40;
	PORT5.PDR.BYTE &= ~0x20;
	PORT5.PMR.BYTE &= ~0x20;
#endif
}

#else
#error
#endif

void INT_MTU3_TGIA3(void)
{
#ifndef __VS__
#endif

	criAtom_ExecuteIntr_RXV2();

#ifndef __VS__
#endif
}

void CriAeroSupportAtom_Finalize(void)
{
	criAtom_Finalize_RXV2();
}

CriSint32 CriAeroSupportAtom_CalculateWorkSize(void)
{
	return criAtom_CalculateWorkSize_RXV2(&LIB_CONFIG);
}

void CriAeroSupportAtom_Initialize(void *work, CriSint32 work_size)
{
	criAtom_Initialize_RXV2(&LIB_CONFIG, work, work_size);
	MTU.TOERA.BYTE |= 0x28;
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
