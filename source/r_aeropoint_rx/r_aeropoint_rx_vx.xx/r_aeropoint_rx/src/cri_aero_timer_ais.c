/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2020 CRI Middleware Co., Ltd.
 *
 * Library  : Aeropoint Lite Runtime
 * Module   : Library System Source
 * File     : cri_aero_timer.c
 *
 ****************************************************************************/

#include "cri_aero_timer_internal.h"
#include "r_sys_time_rx_if.h"
#include "r_cmt_rx_if.h"

static uint32_t _tchannel = 0;

void CRIAPI criAeroTimer_Initialize_AIS()
{
    if (R_SYS_TIME_Open() != SYS_TIME_SUCCESS) {
        for (;;)
        {
           
        }
    }

	R_CMT_CreatePeriodic(1000, criAeroTimer_SystemCallback, &_tchannel);

	criAeroTimer_SaveFrequency( 1000 );

	return;
}

void CRIAPI criAeroTimer_Finalize_AIS()
{
	R_CMT_Stop( _tchannel );

    R_SYS_TIME_Close();

	return;
}


