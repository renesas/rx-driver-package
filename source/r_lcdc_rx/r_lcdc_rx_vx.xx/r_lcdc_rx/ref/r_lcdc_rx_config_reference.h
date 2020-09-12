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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 01.12.2014 1.00     First Release
*              : 20.05.2019 1.01     Changed comment
*******************************************************************************/
/*******************************************************************************
* File Name     : r_lcdc_rx_config.h
* Description   : Configures the LCDC drivers
*******************************************************************************/
#ifndef LCDC_CONFIG_H
#define LCDC_CONFIG_H

/*******************************************************************************
Configuration Options
*******************************************************************************/
/* SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING */
// Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting
// Setting to 1 includes parameter checking; 0 compiles out parameter checking
#define LCDC_CFG_PARAM_CHECKING_ENABLE   (1)


/* LCD Drive Voltage Generator Select (MDSET bit).
   * The following Internal/Capacitor are prohibition when IWDT-dedicated is chosen as LCD clock source.  
   0 = The external resistance division method.
   1 = The internal voltage boosting method.
   2 = The capacitor split method.
*/
#define LCDC_CFG_DRV_GENERATOR   (1)


/* Whether voltage boosting wait time is performed within this module 
   or user's program.
   0 = Not to perform the wait processing in this module. 
       Perform it in the user program.
   1 = To perform the wait processing in this module.
*/
#define LCDC_CFG_VOLTAGE_WAIT    (1)


/* LCD display bias method select (LBAS bit).
   2 = 1/2 bias method.
   3 = 1/3 bias method.
   4 = 1/4 bias method.
*/
#define LCDC_CFG_BIAS (3)


/* Time slice of LCD display select (LDTY bit).
   1 = Static.
   2 = 2-time slice.
   3 = 3-time slice.
   4 = 4-time slice.
   8 = 8-time slice.
*/
#define LCDC_CFG_TIME_SLICES (4)


/* LCD display waveform select (LWAVE bit).
   0 = Waveform A.
   1 = Waveform B.
*/
#define LCDC_CFG_DRV_WAVEFORM (0)


/* LCD Clock Control Register 0 (LCDC0 bit).
   - LCD source clock is the main clock, HOCO clock or LOCO clock.
   17 = 1/256
   18 = 1/512
   19 = 1/1024
   20 = 1/2048
   21 = 1/4096
   22 = 1/8192
   23 = 1/16384
   24 = 1/32768
   25 = 1/65536
   26 = 1/131072
   27 = 1/262144
   43 = 1/524288
   
   - LCD source clock is the sub-clock.
   4 = 1/32
   5 = 1/64
   6 = 1/128
   7 = 1/256
   8 = 1/512
   9 = 1/1024
   
   - LCD source clock is the IWDT-dedicated on-chip oscillator.
   3 = 1/16
   4 = 1/32
   5 = 1/64
   6 = 1/128
   7 = 1/256
*/ 
#define LCDC_CFG_CLOCK_DIV (23)


/* Reference voltage (contrast Adjustment) select (VLCD bit).
   - VL1 voltage (reference voltage).
   4 = 1.00V
   5 = 1.05V
   6 = 1.10V
   7 = 1.15V
   8 = 1.20V
   9 = 1.25V
   10 = 1.30V
   11 = 1.35V
   12 = 1.40V
   13 = 1.45V
   14 = 1.50V
   15 = 1.55V
   16 = 1.60V
   17 = 1.65V
   18 = 1.70V
   19 = 1.75V
   
   - VL4 voltage (1/3 bias method).
   4 = 3.00V
   5 = 3.15V
   6 = 3.30V
   7 = 3.45V
   8 = 3.60V
   9 = 3.75V
   10 = 3.90V
   11 = 4.05V
   12 = 4.20V
   13 = 4.35V
   14 = 4.50V
   15 = 4.65V
   16 = 4.80V
   17 = 4.95V
   18 = 5.10V
   19 = 5.20V
   
   - VL4 voltage (1/4 bias method).
   4 = 4.00V
   5 = 4.20V
   6 = 4.40V
   7 = 4.60V
   8 = 4.80V
   9 = 5.00V
   10 = 5.20V
*/
#define LCDC_CFG_REF_VCC (12)


#endif /* LCDC_CONFIG_H */

