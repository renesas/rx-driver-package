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
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_s12ad_rx660.c
* Description  : Primary source code for 12-bit A/D Converter driver.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           29.12.2021 1.00    Initial Release.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include <stdio.h>
#include "platform.h"

#if defined (BSP_MCU_RX660)
#include "r_s12ad_rx_config.h"
#include "r_s12ad_rx_private.h"
#include "r_s12ad_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#define ADC_PRV_GRPBL1_S12CMPIA0_FLAG    (1<<20)
#define ADC_PRV_GRPBL1_S12CMPIB0_FLAG    (1<<21)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Unit2 registers are a superset of Unit0, 1 EXCEPT that they do not contain the S&H registers and the PGA registers */
typedef R_BSP_VOLATILE_EVENACCESS struct st_s12ad   aregs_t;

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/* In RAM */

adc_ctrl_t g_dcb = {ADC_MODE_MAX,
                    NULL,
                    false,
                    ADC_PRV_GRPBL1_S12CMPIA0_FLAG,
                    ADC_PRV_GRPBL1_S12CMPIB0_FLAG,
                   };

/* In ROM. A/D Data Register pointers */

volatile uint16_t R_BSP_EVENACCESS_SFR * const  gp_dreg_ptrs[ADC_REG_MAX+1] =
                    {   &S12AD.ADDR0,       /* channel 0 */
                        &S12AD.ADDR1,       /* channel 1 */
                        &S12AD.ADDR2,       /* channel 2 */
                        &S12AD.ADDR3,       /* channel 3 */
                        &S12AD.ADDR4,       /* channel 4 */
                        &S12AD.ADDR5,       /* channel 5 */
                        &S12AD.ADDR6,       /* channel 6 */
                        &S12AD.ADDR7,       /* channel 7 */
                        &S12AD.ADDR8,       /* channel 8 */
                        &S12AD.ADDR9,       /* channel 9 */
                        &S12AD.ADDR10,      /* channel 10 */
                        &S12AD.ADDR11,      /* channel 11 */
                        &S12AD.ADDR12,      /* channel 12 */
                        &S12AD.ADDR13,      /* channel 13 */
                        &S12AD.ADDR14,      /* channel 14 */
                        &S12AD.ADDR15,      /* channel 15 */
                        &S12AD.ADDR16,      /* channel 16 */
                        &S12AD.ADDR17,      /* channel 17 */
                        &S12AD.ADDR18,      /* channel 18 */
                        &S12AD.ADDR19,      /* channel 19 */
                        &S12AD.ADDR20,      /* channel 20 */
                        &S12AD.ADDR21,      /* channel 21 */
                        &S12AD.ADDR22,      /* channel 22 */
                        &S12AD.ADDR23,      /* channel 23 */
                        &S12AD.ADTSDR,      /* temperature sensor */
                        &S12AD.ADOCDR,      /* voltage sensor */
                        &S12AD.ADDBLDR,     /* double trigger register */
                        &S12AD.ADDBLDRA,    /* double trigger registerA */
                        &S12AD.ADDBLDRB,    /* double trigger registerB */
                        &S12AD.ADRD.WORD    /* self-diagnosis register */
                    };

/* In ROM. Sample State (SST) Register pointers */

/* 8-bit register pointers */
volatile uint8_t R_BSP_EVENACCESS_SFR * const  gp_sreg0_ptrs[ADC_SST_REG_MAX+1] =
                    {   &S12AD.ADSSTR0,      /* channel 0 */
                        &S12AD.ADSSTR1,      /* channel 1 */
                        &S12AD.ADSSTR2,      /* channel 2 */
                        &S12AD.ADSSTR3,      /* channel 3 */
                        &S12AD.ADSSTR4,      /* channel 4 */
                        &S12AD.ADSSTR5,      /* channel 5 */
                        &S12AD.ADSSTR6,      /* channel 6 */
                        &S12AD.ADSSTR7,      /* channel 7 */
                        &S12AD.ADSSTR8,      /* channel 8 */
                        &S12AD.ADSSTR9,      /* channel 9 */
                        &S12AD.ADSSTR10,     /* channel 10 */
                        &S12AD.ADSSTR11,     /* channel 11 */
                        &S12AD.ADSSTR12,     /* channel 12 */
                        &S12AD.ADSSTR13,     /* channel 13 */
                        &S12AD.ADSSTR14,     /* channel 14 */
                        &S12AD.ADSSTR15,     /* channel 15 */
                        &S12AD.ADSSTRL,      /* channel 16-23 */
                        &S12AD.ADSSTRT,      /* temperature sensor */
                        &S12AD.ADSSTRO       /* voltage sensor */
                    };

#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
static adc_err_t adc_check_open_cfg(uint8_t                unit,
                                    adc_mode_t const       mode,
                                    adc_cfg_t * const      p_cfg,
                                    void         (* const  p_callback)(void *p_args));

static adc_err_t adc_check_scan_config(uint8_t const unit,  adc_ch_cfg_t *p_config);
#endif

static adc_err_t adc_configure_scan(uint8_t const unit,  adc_ch_cfg_t *p_config);
static void adc_configure_sensors(adc_ch_cfg_t  *p_config);
static void adc_enable_s12adi(uint8_t unit);
static void adc_enable_s12gbadi(uint8_t unit);
static void adc_enable_s12gcadi(uint8_t unit);
static void adc_s12cmpia0_isr(void *cb_args);
static void adc_s12cmpib0_isr(void *cb_args);
static adc_err_t adc_en_comparator_level0(uint8_t unit, adc_cmpwin_t  *p_cmpwin);
static uint32_t adc_get_and_clr_cmpi_flags(uint8_t unit);
static uint32_t adc_get_and_clr_cmpi_flagsb(uint8_t unit);

R_BSP_PRAGMA_STATIC_INTERRUPT(adc_s12adi0_isr, VECT(S12ADC0,S12ADI0))
R_BSP_PRAGMA_STATIC_INTERRUPT(adc_s12gbadi0_isr, VECT(S12ADC0,S12GBADI0))
R_BSP_PRAGMA_STATIC_INTERRUPT(adc_s12gcadi0_isr, VECT(S12ADC0,S12GCADI0))

/******************************************************************************
* Function Name: adc_open
* Description  : This function applies power to the A/D peripheral, sets the
*                operational mode, trigger sources, interrupt priority, and
*                configurations common to all channels and sensors. If interrupt
*                priority is non-zero, the function takes a callback function
*                pointer for notifying the user at interrupt level whenever a
*                scan has completed.
*
* Arguments    : unit -
*                    Unit number (0, 1 or 2)
*                mode -
*                    Operational mode (see enumeration below)
*                p_cfg-
*                    Pointer to configuration structure (see below)
*                p_callback-
*                    Optional pointer to function called from interrupt when
*                    a scan completes
* Return Value : ADC_SUCCESS-
*                    Successful
*                ADC_ERR_AD_LOCKED-
*                    Open() call is in progress elsewhere
*                ADC_ERR_AD_NOT_CLOSED-
*                    Peripheral is still running in another mode; Perform
*                    R_ADC_Close() first
*                ADC_ERR_INVALID_ARG-
*                    mode or element of p_cfg structure has invalid value.
*                ADC_ERR_ILLEGAL_ARG-
*                    an argument is illegal based upon mode
*                ADC_ERR_MISSING_PTR-
*                    p_cfg pointer is FIT_NO_PTR/NULL
*******************************************************************************/
adc_err_t adc_open(uint8_t const          unit,
                    adc_mode_t const       mode,
                    adc_cfg_t * const      p_cfg,
                    void         (* const  p_callback)(void *p_args))
{
    aregs_t     *p_regs;
    volatile uint16_t   u16_dummy;  /* Dummy read for "1" change to "0".(read first) */
    volatile uint8_t    u8_dummy;   /* Dummy read for "1" change to "0".(read first) */

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif

#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
    adc_err_t   err;

    if ((NULL == p_cfg) || (FIT_NO_PTR == p_cfg))
    {
        return ADC_ERR_MISSING_PTR;
    }

    err = adc_check_open_cfg(unit, mode, p_cfg, p_callback);
    if (ADC_SUCCESS != err)
    {
        return err;
    }
#endif

    /* ENSURE OPEN() NOT ALREADY IN PROGRESS */
    if (true == g_dcb.opened)
    {
        return ADC_ERR_AD_NOT_CLOSED;
    }
    if (R_BSP_HardwareLock(BSP_LOCK_S12AD) == false)
    {
        return ADC_ERR_AD_LOCKED;
    }

    /* APPLY POWER TO PERIPHERAL */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    MSTP(S12AD) = 0;

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Get S12AD register address */
    p_regs = (aregs_t *)&S12AD;

    /* Initialize S12AD register */

    /* A/D Control Register initialize */
    p_regs->ADCSR.WORD = 0;

    /* A/D Channel Select Register A0 initialize */
    p_regs->ADANSA0.WORD = 0;

    /* A/D-Converted Value Addition/Average Function Select Register 0 initialize */
    p_regs->ADADS0.WORD = 0;

    /* A/D-Converted Value Addition/Average Count Select Register initialize */
    p_regs->ADADC.BYTE = 0;

    /* A/D Control Extended Register initialize */
    p_regs->ADCER.WORD = 0;

    /* A/D Conversion Start Trigger Select Register initialize */
    p_regs->ADSTRGR.WORD = 0;

    /* A/D Channel Select Register B0 initialize */
    p_regs->ADANSB0.WORD = 0;

    /* A/D Disconnection Detection Control Register initialize */
    p_regs->ADDISCR.BYTE = 0;

    /* A/D Event Link Control Register initialize */
    p_regs->ADELCCR.BYTE = 0;

    /* A/D Group Scan Priority Control Register initialize */
    p_regs->ADGSPCR.WORD = 0;

    /* A/D Comparison Function Control Register initialize */
    p_regs->ADCMPCR.WORD = 0;

    /* A/D Comparison Function Window A Channel Select Register 0 initialize */
    p_regs->ADCMPANSR0.WORD = 0;

    /* A/D Comparison Function Window A Comparison Condition Setting Register 0 initialize */
    p_regs->ADCMPLR0.WORD = 0;

    /* A/D Comparison Function Window A Lower Level Setting Register initialize */
    p_regs->ADCMPDR0 = 0;

    /* A/D Comparison Function Window A Upper Level Setting Register initialize */
    p_regs->ADCMPDR1 = 0;

    /* A/D Comparison Function Window A Channel Status Register 0 initialize */
    u16_dummy = p_regs->ADCMPSR0.WORD;
    p_regs->ADCMPSR0.WORD = 0;

    /* A/D Comparison Function Window B Channel Select Register initialize */
    p_regs->ADCMPBNSR.BYTE = 0;

    /* A/D Comparison Function Window B Lower Level Setting Register initialize */
    p_regs->ADWINLLB = 0;

    /* A/D Comparison Function Window B Upper Level Setting Register initialize */
    p_regs->ADWINULB = 0;

    /* A/D Comparison Function Window B Channel Status Register initialize */
    u8_dummy = p_regs->ADCMPBSR.BYTE;
    p_regs->ADCMPBSR.BYTE = 0;

    /* A/D Channel Select Register C0 initialize */
    p_regs->ADANSC0.WORD = 0;

    /* A/D Group C Trigger Select Register initialize */
    p_regs->ADGCTRGR.BYTE = 0;

    /* A/D Sampling State Register initialize */
    p_regs->ADSSTR0 = 0x1B;
    p_regs->ADSSTR1 = 0x1B;
    p_regs->ADSSTR2 = 0x1B;
    p_regs->ADSSTR3 = 0x1B;
    p_regs->ADSSTR4 = 0x1B;
    p_regs->ADSSTR5 = 0x1B;
    p_regs->ADSSTR6 = 0x1B;
    p_regs->ADSSTR7 = 0x1B;
    p_regs->ADSSTR8 = 0x1B;
    p_regs->ADSSTR9 = 0x1B;
    p_regs->ADSSTR10 = 0x1B;
    p_regs->ADSSTR11 = 0x1B;
    p_regs->ADSSTR12 = 0x1B;
    p_regs->ADSSTR13 = 0x1B;
    p_regs->ADSSTR14 = 0x1B;
    p_regs->ADSSTR15 = 0x1B;
    p_regs->ADSSTRL = 0x1B;
    p_regs->ADSSTRT = 0x1B;
    p_regs->ADSSTRO = 0x1B;
    if (0 != S12AD.ADSSTRO)
    {

        /* Dommy read and compare */
        R_BSP_NOP();
    }
    /* A/D Channel Conversion Order Setting Register initialize */
    p_regs->ADSCS0 = 0x00;
    p_regs->ADSCS1 = 0x01;
    p_regs->ADSCS2 = 0x02;
    p_regs->ADSCS3 = 0x03;
    p_regs->ADSCS4 = 0x04;
    p_regs->ADSCS5 = 0x05;
    p_regs->ADSCS6 = 0x06;
    p_regs->ADSCS7 = 0x07;
    p_regs->ADSCS8  = 0x08;
    p_regs->ADSCS9  = 0x09;
    p_regs->ADSCS10 = 0x0A;
    p_regs->ADSCS11 = 0x0B;
    p_regs->ADSCS12 = 0x0C;
    p_regs->ADSCS13 = 0x0D;
    p_regs->ADSCS14 = 0x0E;
    p_regs->ADSCS15 = 0x0F;
    p_regs->ADSCS16 = 0x10;
    p_regs->ADSCS17 = 0x11;
    p_regs->ADSCS18 = 0x12;
    p_regs->ADSCS19 = 0x13;
    p_regs->ADSCS20 = 0x14;
    p_regs->ADSCS21 = 0x15;
    p_regs->ADSCS22 = 0x16;
    p_regs->ADSCS23 = 0x17;


    /* A/D Channel Select Register A1 initialize */
    S12AD.ADANSA1.WORD = 0;

    /* A/D-Converted Value Addition/Average Function Select Register 1 initialize */
    S12AD.ADADS1.WORD = 0;

    /* A/D Conversion Extended Input Control Register initialize */
    S12AD.ADEXICR.WORD = 0;

    /* A/D Channel Select Register B1 initialize */
    S12AD.ADANSB1.WORD = 0;

    /* A/D Comparison Function Window A Extended Input Select Register initialize */
    S12AD.ADCMPANSER.BYTE = 0;

    /* A/D Comparison Function Window A Extended Input Comparison Condition
       Setting Register initialize */
    S12AD.ADCMPLER.BYTE = 0;

    /* A/D Comparison Function Window A Channel Select Register 1 initialize */
    S12AD.ADCMPANSR1.WORD = 0;

    /* A/D Comparison Function Window A Comparison Condition Setting Register 1 initialize */
    S12AD.ADCMPLR1.WORD = 0;

    /* A/D Comparison Function Window A Channel Status Register 1 initialize */
    u16_dummy = S12AD.ADCMPSR1.WORD;
    S12AD.ADCMPSR1.WORD = 0;

    /* A/D Comparison Function Window A Extended Input Channel Status Register initialize */
    u8_dummy = S12AD.ADCMPSER.BYTE;
    S12AD.ADCMPSER.BYTE = 0;

    /* A/D Channel Select Register C1 initialize */
    S12AD.ADANSC1.WORD = 0;

    /* A/D Group C Extended Input Control Register */
    S12AD.ADGCEXCR.BYTE = 0;

    /* A/D Sampling State Register initialize */
    S12AD.ADSSTRL = 0x1B;
    S12AD.ADSSTRT = 0x1B;
    S12AD.ADSSTRO = 0x1B;
    S12AD.ADSSTR8 = 0x1B;
    S12AD.ADSSTR9 = 0x1B;
    S12AD.ADSSTR10 = 0x1B;
    S12AD.ADSSTR11 = 0x1B;
    if (0 != S12AD.ADSSTR11)
    {

        /* Dummy read and compare */
        R_BSP_NOP();
    }

    /* A/D Channel Conversion Order Setting Register initialize */
    S12AD.ADSCS8  = 0x08;
    S12AD.ADSCS9  = 0x09;
    S12AD.ADSCS10 = 0x0A;
    S12AD.ADSCS11 = 0x0B;
    S12AD.ADSCS12 = 0x10;
    S12AD.ADSCS13 = 0x11;

    /* A/D Internal Reference Voltage Monitoring Circuit Enable Register initialize */
    S12AD.ADVMONCR.BYTE = 0;

    /* A/D Internal Reference Voltage Monitoring Circuit Output Enable Register initialize */
    S12AD.ADVMONO.BYTE = 0;

    /* SET TEMPERATURE SENSOR */
    if (ADC_TEMP_SENSOR_NOT_AD_CONVERTED != p_cfg->temp_sensor)
    {
        if (ADC_TEMP_SENSOR_AD_CONVERTED == p_cfg->temp_sensor)
        {
            S12AD.ADEXICR.BIT.TSSA = 1;     // select temperature output GroupA
            S12AD.ADEXICR.BIT.TSSB = 0;
            S12AD.ADGCEXCR.BIT.TSSC = 0;
        }
        else if (ADC_TEMP_SENSOR_AD_CONVERTED_GROUPB == p_cfg->temp_sensor)
        {
            S12AD.ADEXICR.BIT.TSSA = 0;
            S12AD.ADEXICR.BIT.TSSB = 1;     // select temperature output GroupB
            S12AD.ADGCEXCR.BIT.TSSC = 0;
        }
        else
        {
            S12AD.ADEXICR.BIT.TSSA = 0;
            S12AD.ADEXICR.BIT.TSSB = 0;
            S12AD.ADGCEXCR.BIT.TSSC = 1;    // select temperature output GroupC
        }
    }

    /* SET MODE RELATED REGISTER FIELDS */
    g_dcb.mode = mode;
    if ((ADC_MODE_SS_MULTI_CH_GROUPED == mode)
    || (ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A == mode))
    {
        p_regs->ADCSR.BIT.ADCS = ADC_ADCS_GROUP_SCAN;
    }
    else if ((ADC_MODE_SS_MULTI_CH_GROUPED_GROUPC == mode)
    || (ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A_GROUPC == mode))
    {
        p_regs->ADCSR.BIT.ADCS = ADC_ADCS_GROUP_SCAN;
        p_regs->ADGCTRGR.BIT.GRCE = 1;       // Use Group C
    }
    else
    {
        if ((ADC_MODE_CONT_ONE_CH == mode) || (ADC_MODE_CONT_MULTI_CH == mode))
        {
            p_regs->ADCSR.BIT.ADCS = ADC_ADCS_CONT_SCAN;
        }

        /* other modes have ADCS=0 */
    }

    if ((ADC_MODE_SS_ONE_CH_DBLTRIG == mode)
    || (ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A == mode)
    || (ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A_GROUPC == mode))
    {
        p_regs->ADCSR.BIT.DBLE = 1;         // enable double trigger
    }

    /* SET TRIGGER AND INTERRUPT PRIORITY REGISTER FIELDS */
    if (ADC_TRIG_SOFTWARE != p_cfg->trigger)
    {
        p_regs->ADSTRGR.BIT.TRSA = p_cfg->trigger;
    }

    if (ADC_TRIG_ASYNC_ADTRG == p_cfg->trigger)
    {
        p_regs->ADCSR.BIT.EXTRG = 1;        // set ext trigger for async trigger
    }

    if (ADC_ADCS_GROUP_SCAN == p_regs->ADCSR.BIT.ADCS)
    {
        p_regs->ADSTRGR.BIT.TRSB = p_cfg->trigger_groupb;
        p_regs->ADGCTRGR.BIT.TRSC = p_cfg->trigger_groupc;      // for Group C

        IPR(S12ADC0,S12GBADI0) = p_cfg->priority_groupb;

        IPR(S12ADC0,S12GCADI0) = p_cfg->priority_groupc;    // for Group C

    }

    IPR(S12ADC0,S12ADI0) = p_cfg->priority;

    /* SET REGISTER FIELDS FOR REMAINING PARAMETERS */
    p_regs->ADCER.BIT.ADRFMT = (ADC_ALIGN_LEFT == p_cfg->alignment) ? 1 : 0;
    p_regs->ADCER.BIT.ACE = (ADC_CLEAR_AFTER_READ_ON == p_cfg->clearing) ? 1 : 0;

    if (ADC_TEMP_SENSOR_NOT_AD_CONVERTED != p_cfg->temp_sensor)
    {
        S12AD.ADEXICR.BIT.TSSAD = (ADC_TEMP_SENSOR_ADD_ON == p_cfg->add_temp_sensor) ? 1 : 0;
    }
    p_regs->ADADC.BYTE = p_cfg->add_cnt;

    /* SAVE CALLBACK FUNCTION POINTER */
    g_dcb.p_callback = p_callback;

    /* MARK DRIVER AS OPENED */
    g_dcb.opened = true;
    R_BSP_HardwareUnlock(BSP_LOCK_S12AD);

    return ADC_SUCCESS;
} /* End of function adc_open() */


/******************************************************************************
* Function Name: adc_check_open_cfg
* Description  : If Parameter Checking is enabled, this function will simply
*                validate the parameters passed to the adc_open function.
** Arguments    :unit -
*                    Unit number (0, 1 or 2)
*                mode -
*                    Operational mode (see enumeration below)
*                p_cfg-
*                    Pointer to configuration structure (see below)
*                p_callback-
*                    Optional pointer to function called from interrupt when
*                    a scan completes
* Return Value : ADC_SUCCESS-
*                    Successful
*                ADC_ERR_INVALID_ARG-
*                    mode or element of p_cfg structure has invalid value.
*                ADC_ERR_ILLEGAL_ARG-
*                    an argument is illegal based upon mode
*******************************************************************************/
#if (ADC_CFG_PARAM_CHECKING_ENABLE == 1)
static adc_err_t adc_check_open_cfg(uint8_t                unit,
                                    adc_mode_t const       mode,
                                    adc_cfg_t * const      p_cfg,
                                    void         (* const  p_callback)(void *p_args))
{

    /* Check for valid argument values */
    if ((unit > 2) || (p_cfg->priority > BSP_MCU_IPL_MAX))
    {
        return ADC_ERR_INVALID_ARG;
    }

    /* If interrupt driven, must have callback function */
    if ((0 != p_cfg->priority)
    && ((NULL == p_callback) || (FIT_NO_FUNC == p_callback)))
    {
        return ADC_ERR_ILLEGAL_ARG;
    }

    /* In double trigger mode, SW and async triggers not allowed */
    if ((ADC_MODE_SS_ONE_CH_DBLTRIG == mode)
    || ((ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A == mode) || (ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A_GROUPC == mode)))
    {
        if ((ADC_TRIG_SOFTWARE == p_cfg->trigger) || (ADC_TRIG_ASYNC_ADTRG == p_cfg->trigger))
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
    }

    /* Group checking; only synchronous triggers allowed; must be unique */
    if ((ADC_MODE_SS_MULTI_CH_GROUPED == mode) || (ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A == mode)
    || (ADC_MODE_SS_MULTI_CH_GROUPED_GROUPC == mode) || (ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A_GROUPC == mode))
    {

        /* for Group A and B */
        if ((ADC_TRIG_ASYNC_ADTRG == p_cfg->trigger)
        || (ADC_TRIG_ASYNC_ADTRG == p_cfg->trigger_groupb)
        || (p_cfg->trigger == p_cfg->trigger_groupb)
        || (ADC_TRIG_SOFTWARE == p_cfg->trigger)
        || (ADC_TRIG_SOFTWARE == p_cfg->trigger_groupb))
        {
            return ADC_ERR_ILLEGAL_ARG;
        }

        if (p_cfg->priority_groupb > BSP_MCU_IPL_MAX)
        {
            return ADC_ERR_INVALID_ARG;
        }

        if ((0 != p_cfg->priority_groupb)   // Interrupt driven, must have callback function
        && ((NULL == p_callback) || (FIT_NO_FUNC == p_callback)))
        {
            return ADC_ERR_ILLEGAL_ARG;
        }

        /* for Group C */
        if ((ADC_MODE_SS_MULTI_CH_GROUPED_GROUPC == mode) || (ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A_GROUPC == mode))
        {
            if ((ADC_TRIG_ASYNC_ADTRG == p_cfg->trigger_groupc)
            || (p_cfg->trigger == p_cfg->trigger_groupc)
            || (p_cfg->trigger_groupb == p_cfg->trigger_groupc)
            || (ADC_TRIG_SOFTWARE == p_cfg->trigger_groupc))
            {
                return ADC_ERR_ILLEGAL_ARG;
            }

            if (p_cfg->priority_groupc > BSP_MCU_IPL_MAX)
            {
                return ADC_ERR_INVALID_ARG;
            }

            if ((0 != p_cfg->priority_groupc)   // Interrupt driven, must have callback function
            && ((NULL == p_callback) || (FIT_NO_FUNC == p_callback)))
            {
                return ADC_ERR_ILLEGAL_ARG;
            }
        }
    }

    /* If sensors specified, verify in legal configuration */
    if (ADC_TEMP_SENSOR_NOT_AD_CONVERTED != p_cfg->temp_sensor)
    {
        if ((ADC_MODE_SS_ONE_CH_DBLTRIG == mode)    // double trigger mode
        || (ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A == mode)    // double trigger mode
        || (ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A_GROUPC == mode)    // double trigger mode
        || (ADC_MODE_CONT_ONE_CH == mode)    // continuous scan mode
        || (ADC_MODE_CONT_MULTI_CH == mode))    // continuous scan mode
        {
            return ADC_ERR_ILLEGAL_ARG;
        }

        if ((ADC_MODE_SS_ONE_CH == mode) || (ADC_MODE_SS_MULTI_CH == mode))
        {
            if ((ADC_TEMP_SENSOR_AD_CONVERTED_GROUPB == p_cfg->temp_sensor)
            || (ADC_TEMP_SENSOR_AD_CONVERTED_GROUPC == p_cfg->temp_sensor))
            {
                return ADC_ERR_ILLEGAL_ARG;
            }
        }
    }
    else
    {
        if (ADC_TEMP_SENSOR_ADD_ON == p_cfg->add_temp_sensor)
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
    }

    return ADC_SUCCESS;
} /* End of function adc_check_open_cfg() */
#endif /* ADC_CFG_PARAM_CHECKING_ENABLE == 1 */


/******************************************************************************
* Function Name: adc_control
* Description  : This function provides commands for enabling channels and
*                sensors and for runtime operations. These include enabling/
*                disabling trigger sources and interrupts, initiating a
*                software trigger, and checking for scan completion.
*
* NOTE: Enabling a channel or a sensor, or setting the sample state count reg
*       cannot be done while the ADCSR.ADST bit is set (conversion in progress).
*       Because these commands should only be called once during initialization
*       before triggers are enabled, this should not be an issue. Registers
*       with this restriction include ADANSA0, ADANSA1, ADANSB0, ADANSB1, ADADS,
*       ADADC, ADADS0, ADADS1, ADSSTRx, ADSHCR, ADSHMSR, ADEXICR, ADCMPCR,
*       ADCMPANSR0, ADCMPANSR1, ADACMPANSER, ADCMPLR0, ADCMPLR1, ADCMPLER,
*       ADCMPDR0, ADCMPDR1, and some bits in ADCSR, ADCMPCR, ADGSPCR and TSCR.
*       No runtime operational sequence checking of any kind is performed.
*
* Arguments    : unit -
*                    Unit number (0, 1 or 2)
*                cmd-
*                    Command to run
*                p_args-
*                    Pointer to optional configuration structure
* Return Value : ADC_SUCCESS-
*                    Successful
*                ADC_ERR_MISSING_PTR-
*                    p_args pointer is FIT_NO_PTR/NULL when required as an argument
*                ADC_ERR_INVALID_ARG-
*                    cmd or element of p_args structure has invalid value.
*                ADC_ERR_ILLEGAL_ARG-
*                    argument is illegal based upon rest of configuration
*                ADC_ERR_SCAN_NOT_DONE-
*                    The requested scan has not completed
*                ADC_ERR_TRIG_ENABLED -
*                    Cannot configure comparator because a scan is running
*                ADC_ERR_CONDITION_NOT_MET -
*                    No chans/sensors passed comparator condition
*                ADC_ERR_UNKNOWN
*                    Did not receive expected hardware response
*******************************************************************************/
adc_err_t adc_control(uint8_t const       unit,
                        adc_cmd_t const     cmd,
                        void * const        p_args)
{
    adc_err_t       err=ADC_SUCCESS;
    adc_sst_t       *p_sample;
    adc_dda_t       *p_charge;
    adc_cmpwin_t    *p_cmpwin;
    adc_comp_stat_t *p_cmpstat;

    /* Get S12AD register address */
    aregs_t         *p_regs = (aregs_t *)&S12AD;
    volatile uint32_t        flags;

    /* DO UNIVERSAL PARAMETER CHECKING */
#if (ADC_CFG_PARAM_CHECKING_ENABLE == 1)
    if (unit > 2)
    {
        return ADC_ERR_INVALID_ARG;
    }

    if ((NULL == p_args) || (FIT_NO_PTR == p_args))
    {
        if ((ADC_CMD_SET_DDA_STATE_CNT == cmd)
        || (ADC_CMD_SET_SAMPLE_STATE_CNT == cmd)
        || (ADC_CMD_ENABLE_CHANS == cmd)
        || (ADC_CMD_EN_COMPARATOR_LEVEL == cmd)
        || (ADC_CMD_EN_COMPARATOR_WINDOW == cmd)
        || (ADC_CMD_COMP_COMB_STATUS == cmd)
        || (ADC_CMD_CHECK_CONDITION_MET == cmd)
        || (ADC_CMD_CHECK_CONDITION_METB == cmd))
        {
            return ADC_ERR_MISSING_PTR;
        }
    }
#endif

    /* PROCESS INDIVIDUAL COMMANDS */
    switch (cmd)
    {
    case ADC_CMD_SET_DDA_STATE_CNT:

        /* Cast from void pointer to adc_dda_t */
        p_charge = (adc_dda_t *)p_args;
#if (ADC_CFG_PARAM_CHECKING_ENABLE == 1)
        if (ADC_DDA_OFF != p_charge->method)
        {
            if ((p_charge->num_states < ADC_DDA_STATE_CNT_MIN)
            || (p_charge->num_states > ADC_DDA_STATE_CNT_MAX)
            || (4 == p_charge->num_states)
            || (5 == p_charge->num_states)
            || (7 == p_charge->num_states)
            || (8 == p_charge->num_states)
            || (10 == p_charge->num_states)
            || (11 == p_charge->num_states)
            || (13 == p_charge->num_states)
            || (14 == p_charge->num_states)
            || (0 != p_regs->ADCER.BIT.DIAGM)          // self-diagnosis enabled
            || ((p_regs->ADEXICR.WORD & 0x0F00) != 0)  // sensors enabled
            || ((p_regs->ADGCEXCR.BYTE & 0x03) != 0))  // sensors enabled for Group C
            {
                return ADC_ERR_ILLEGAL_ARG;
            }
        }
#endif
        if (ADC_DDA_OFF == p_charge->method)
        {
            p_regs->ADDISCR.BYTE = 0;
        }
        else
        {

            /* NOTE: Using Disconnect Detection Assist adds num_states x (#chans) ADCLKS to scan time */
            p_regs->ADDISCR.BYTE = (uint8_t)((ADC_DDA_PRECHARGE == p_charge->method) ? 0x10 : 0);
            p_regs->ADDISCR.BYTE |= p_charge->num_states;
        }
        break;

    case ADC_CMD_SET_SAMPLE_STATE_CNT:

        /* Cast from void pointer to adc_sst_t */
        p_sample = (adc_sst_t *)p_args;
#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
        if ((p_sample->reg_id > ADC_SST_REG_MAX)
        || (p_sample->num_states < ADC_SST_CNT_MIN)
        || (p_sample->num_states > ADC_SST_CNT_MAX)
        || ((p_sample->num_states % 3) != 0))
        {
            return ADC_ERR_INVALID_ARG;
        }
#endif
        *(gp_sreg0_ptrs)[p_sample->reg_id] = p_sample->num_states;
        break;

    case ADC_CMD_ENABLE_CHANS:

        /* Cast from void pointer to adc_ch_cfg_t */
        err = adc_configure_scan(unit, (adc_ch_cfg_t *)p_args);
        break;

    case ADC_CMD_EN_COMPARATOR_LEVEL:

        /* Cast from void pointer to adc_cmpwin_t */
        p_cmpwin = (adc_cmpwin_t *)p_args;

        p_regs->ADCMPCR.BIT.WCMPE = 0;          // disable window function
        
        err = adc_en_comparator_level0(unit, p_cmpwin);
        break;

    case ADC_CMD_EN_COMPARATOR_WINDOW:

        /* Cast from void pointer to adc_cmpwin_t */
        p_cmpwin = (adc_cmpwin_t *)p_args;
#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
        if ((p_cmpwin->level_lo > p_cmpwin->level_hi) || (p_cmpwin->level_lob > p_cmpwin->level_hib))
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
#endif
        p_regs->ADCMPCR.BIT.WCMPE = 1;          // enable window function

        err = adc_en_comparator_level0(unit, p_cmpwin);
        break;

    case ADC_CMD_COMP_COMB_STATUS:

        /* Cast from void pointer to adc_comp_stat_t */
        p_cmpstat = (adc_comp_stat_t *)p_args;

        /* Cast from void pointer to adc_comp_stat_t */
        *p_cmpstat = (adc_comp_stat_t)(p_regs->ADWINMON.BIT.MONCOMB);
        break;

    case ADC_CMD_ENABLE_TRIG:
        p_regs->ADCSR.BIT.TRGE = 1;           // enable sync/async triggers
        break;

    case ADC_CMD_DISABLE_TRIG:
        p_regs->ADCSR.BIT.TRGE = 0;           // disable sync/async triggers
        break;

    case ADC_CMD_SCAN_NOW:
        if (0 == p_regs->ADCSR.BIT.ADST)
        {
            p_regs->ADCSR.BIT.ADST = 1;
        }
        else
        {
            err = ADC_ERR_SCAN_NOT_DONE;
        }
        break;

    case ADC_CMD_CHECK_SCAN_DONE:           // default/Group A or Group B
        if (1 == p_regs->ADCSR.BIT.ADST)
        {
            err = ADC_ERR_SCAN_NOT_DONE;
        }
        break;

    case ADC_CMD_CHECK_SCAN_DONE_GROUPA:
        if (1 == IR(S12ADC0,S12ADI0))
        {
            IR(S12ADC0,S12ADI0) = 0;
        }
        else
        {
            err = ADC_ERR_SCAN_NOT_DONE;
        }
        break;

    case ADC_CMD_CHECK_SCAN_DONE_GROUPB:
        if (1 == IR(S12ADC0,S12GBADI0))
        {
            IR(S12ADC0,S12GBADI0) = 0;
        }
        else
        {
            err = ADC_ERR_SCAN_NOT_DONE;
        }
        break;

    case ADC_CMD_CHECK_SCAN_DONE_GROUPC:
        if (1 == IR(S12ADC0,S12GCADI0))
        {
            IR(S12ADC0,S12GCADI0) = 0;
        }
        else
        {
            err = ADC_ERR_SCAN_NOT_DONE;
        }
        break;

    case ADC_CMD_CHECK_CONDITION_MET:
        flags = adc_get_and_clr_cmpi_flags(unit);
        if (0 == flags)
        {
            err = ADC_ERR_CONDITION_NOT_MET;
        }

        /* Cast from void pointer to uint32_t */
        *((uint32_t *)p_args) = flags;
        break;

    case ADC_CMD_CHECK_CONDITION_METB:
        flags = adc_get_and_clr_cmpi_flagsb(unit);
        if (0 == flags)
        {
            err = ADC_ERR_CONDITION_NOT_MET;
        }

        /* Cast from void pointer to uint32_t */
        *((uint32_t *)p_args) = flags;
        break;

    case ADC_CMD_ENABLE_INT:
        p_regs->ADCSR.BIT.ADIE = 1;           // enable S12ADI0 interrupt
        break;

    case ADC_CMD_DISABLE_INT:
        p_regs->ADCSR.BIT.ADIE = 0;           // disable S12ADI0 interrupt
        break;

    case ADC_CMD_ENABLE_INT_GROUPB:
        p_regs->ADCSR.BIT.GBADIE = 1;         // enable GBADI interrupt
        break;

    case ADC_CMD_DISABLE_INT_GROUPB:
        p_regs->ADCSR.BIT.GBADIE = 0;         // disable GBADI interrupt
        break;

    case ADC_CMD_ENABLE_INT_GROUPC:
        p_regs->ADGCTRGR.BIT.GCADIE = 1;      // enable GCADI interrupt
        break;

    case ADC_CMD_DISABLE_INT_GROUPC:
        p_regs->ADGCTRGR.BIT.GCADIE = 0;      // disable GCADI interrupt
        break;

    default:
        err = ADC_ERR_INVALID_ARG;
        break;
    }

    return err;
} /* End of function adc_control() */

/******************************************************************************
* Function Name: adc_en_comparator_level0
* Description  : This function sets up the comparator for level0 threshold
*                compare. The processing is identical for window except
*                for the setting of the window enable bit and the level1
*                value (ADCMPDR1).
*
* Arguments    : unit -
*                    Unit number (0, 1 or 2)
*                p_cmplvl -
*                    Pointer to comparator level compare structure
* Return Value : ADC_SUCCESS-
*                    Successful
*                ADC_ERR_INVALID_ARG-
*                    reg_id contains an invalid value.
*                ADC_ERR_ILLEGAL_ARG-
*                    an argument is illegal based upon rest of configuration
*                ADC_ERR_TRIG_ENABLED -
*                    Cannot configure comparator while scans are running
*******************************************************************************/
static adc_err_t adc_en_comparator_level0(uint8_t   unit, adc_cmpwin_t  *p_cmpwin)
{

    /* Get S12AD register address */
    aregs_t *p_regs = (aregs_t *)&S12AD;
    volatile bsp_int_ctrl_t group_priority;
    volatile uint32_t tmp_adcmpansr1;
    volatile uint32_t tmp_adcmpansl1;
    
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif

#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
    uint32_t    comp_winb_ch;

    if (p_cmpwin->int_priority > BSP_MCU_IPL_MAX)
    {
        return ADC_ERR_INVALID_ARG;
    }
    if (0 != (p_cmpwin->compare_mask & ADC_PRV_INVALID_CH_MASK0))
    {
        return ADC_ERR_INVALID_ARG;
    }
    if ((p_cmpwin->inside_window_mask | p_cmpwin->compare_mask) != p_cmpwin->compare_mask)
    {
        return ADC_ERR_INVALID_ARG;
    }
    if ((ADC_COMP_WINB_COND_BELOW != p_cmpwin->inside_window_maskb)
    && (ADC_COMP_WINB_COND_ABOVE != p_cmpwin->inside_window_maskb))
    {
        return ADC_ERR_INVALID_ARG;
    }
    if (1 == p_regs->ADCSR.BIT.DBLE)
    {
        return ADC_ERR_INVALID_ARG;
    }
    if (0 != p_regs->ADCER.BIT.DIAGM)
    {
        return ADC_ERR_INVALID_ARG;
    }

    if (ADC_COMP_WINB_CH23 >= p_cmpwin->compare_maskb)
    {
        comp_winb_ch = 1<<(p_cmpwin->compare_maskb);
    }
    else if (ADC_COMP_WINB_TEMP == p_cmpwin->compare_maskb)
    {
        comp_winb_ch = ADC_MASK_TEMP;
    }
    else if (ADC_COMP_WINB_VOLT == p_cmpwin->compare_maskb)
    {
        comp_winb_ch = ADC_MASK_VOLT;
    }
    else
    {
        return ADC_ERR_INVALID_ARG;     // Window B channel is out of range
    }
    if (0 != (comp_winb_ch & ADC_PRV_INVALID_CH_MASK0))
    {
        return ADC_ERR_INVALID_ARG;
    }

    if ((true == p_cmpwin->windowa_enable) && (true == p_cmpwin->windowb_enable))
    {
        if ((p_cmpwin->compare_mask & comp_winb_ch))
        {
            return ADC_ERR_INVALID_ARG;
        }
    }

    if ((p_cmpwin->compare_mask & ADC_MASK_SENSORS) && (true == p_cmpwin->windowb_enable))
    {
        return ADC_ERR_INVALID_ARG;
    }

    if (((ADC_COMP_WINB_TEMP == p_cmpwin->compare_maskb ) || (ADC_COMP_WINB_VOLT == p_cmpwin->compare_maskb))
    && (true == p_cmpwin->windowa_enable))
    {
        return ADC_ERR_INVALID_ARG;
    }
#endif /* ADC_CFG_PARAM_CHECKING_ENABLE == 1 */

    if (1 == p_regs->ADCSR.BIT.TRGE)
    {
        return ADC_ERR_TRIG_ENABLED;    // ADST must be 0 to configure comparator
    }

    /* CONFIGURE COMPARATOR */
    if (true == p_cmpwin->windowa_enable) // COMPARATOR A
    {
        p_regs->ADCMPCR.BIT.CMPAE = 0;
        
        p_regs->ADCMPDR0 = p_cmpwin->level_lo;
        
        if (1 == p_regs->ADCMPCR.BIT.WCMPE)
        {
            p_regs->ADCMPDR1 = p_cmpwin->level_hi;
        }

        /* Cast to match the register type */
        p_regs->ADCMPANSR0.WORD = (uint16_t)(p_cmpwin->compare_mask & (~ADC_MASK_SENSORS));

        /* Cast to match the register type */
        p_regs->ADCMPLR0.WORD = (uint16_t)(p_cmpwin->inside_window_mask & (~ADC_MASK_SENSORS));

        tmp_adcmpansr1 = (p_cmpwin->compare_mask & (~ADC_MASK_SENSORS));

        /* Cast to match the register type */
        S12AD.ADCMPANSR1.WORD = (uint16_t)(tmp_adcmpansr1 >> 16);
        S12AD.ADCMPANSER.BIT.CMPSTS = (p_cmpwin->compare_mask & ADC_MASK_TEMP) ? 1 : 0;
        S12AD.ADCMPANSER.BIT.CMPSOC = (p_cmpwin->compare_mask & ADC_MASK_VOLT) ? 1 : 0;

        tmp_adcmpansl1 = (p_cmpwin->compare_mask & (~ADC_MASK_SENSORS));

        /* Cast to match the register type */
        S12AD.ADCMPLR1.WORD = (uint16_t)(tmp_adcmpansl1 >> 16);
        S12AD.ADCMPLER.BIT.CMPLTS = (p_cmpwin->inside_window_mask & ADC_MASK_TEMP) ? 1 : 0;
        S12AD.ADCMPLER.BIT.CMPLOC = (p_cmpwin->inside_window_mask & ADC_MASK_VOLT) ? 1 : 0;

        p_regs->ADCMPCR.BIT.CMPAE = 1;
    }
    else
    {
        p_regs->ADCMPCR.BIT.CMPAE = 0;
    }

    if (true == p_cmpwin->windowb_enable) // COMPARATOR B
    {
        p_regs->ADCMPCR.BIT.CMPBE = 0;

        p_regs->ADWINLLB = p_cmpwin->level_lob;

        if (1 == p_regs->ADCMPCR.BIT.WCMPE)
        {
            p_regs->ADWINULB = p_cmpwin->level_hib;
        }
        
        p_regs->ADCMPBNSR.BIT.CMPCHB = p_cmpwin->compare_maskb & 0x3F;
        p_regs->ADCMPBNSR.BIT.CMPLB = p_cmpwin->inside_window_maskb;

        p_regs->ADCMPCR.BIT.CMPBE = 1;
    }
    else
    {
        p_regs->ADCMPCR.BIT.CMPBE = 0;
    }

    /* Window A/B Composite Conditions Setting */

    if ((true == p_cmpwin->windowa_enable) && (true == p_cmpwin->windowb_enable))
    {
        p_regs->ADCMPCR.BIT.CMPAB = p_cmpwin->comp_cond;
    }

    /* ENABLE INTERRUPT */
    /* Clear interrupt subvector flag and set callback */
    if (0 != p_cmpwin->int_priority)
    {

        /* Cast from uint8_t to uint32_t */
        group_priority.ipl = (uint32_t)(p_cmpwin->int_priority);

        if (true == p_cmpwin->windowa_enable) // COMPARATOR A
        {
            R_BSP_InterruptWrite(BSP_INT_SRC_BL1_S12AD0_S12CMPAI, adc_s12cmpia0_isr);
            p_regs->ADCMPCR.BIT.CMPAIE = 1;             // enable interrupt in ADC
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
            ICU.GENBL1.LONG |= g_dcb.cmpi_mask;   // enable group subvector
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
            R_BSP_InterruptControl(BSP_INT_SRC_BL1_S12AD0_S12CMPAI,

                                        /* Cast to match the argument type */
                                        BSP_INT_CMD_GROUP_INTERRUPT_ENABLE, (void *) &group_priority);
        }

        if (true == p_cmpwin->windowb_enable) // COMPARATOR B
        {
            R_BSP_InterruptWrite(BSP_INT_SRC_BL1_S12AD0_S12CMPBI, adc_s12cmpib0_isr);
            p_regs->ADCMPCR.BIT.CMPBIE = 1;             // enable interrupt in ADC
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
            ICU.GENBL1.LONG |= g_dcb.cmpi_maskb;   // enable group subvector
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
            R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
            R_BSP_InterruptControl(BSP_INT_SRC_BL1_S12AD0_S12CMPBI,

                                        /* Cast to match the argument type */
                                        BSP_INT_CMD_GROUP_INTERRUPT_ENABLE, (void *) &group_priority);
        }
    }

    /* Do NOT enable GENBL1, CMPIAE or CMPIBE here with the intention of polling the GRPBL1 bit
     * for condition met. Another driver may make the GROUPBL1 priority non-zero
     * and there will be no subvector interrupt/callback here to clear the ICU flag.
     * The result will be that the BSP will be in an "infinite loop" processing the
     * group interrupt because the flag never resets, and the app level will not run.
     */

    return ADC_SUCCESS;
} /* End of function adc_en_comparator_level0() */


/*****************************************************************************
* Function Name: adc_get_and_clr_cmpi_flags
* Description  : Returns flags for all channels/sensors that met comparator
*                condition and clears the flags for next interrupt.
* Arguments    : unit -
*                    Unit number (0, 1 or 2)
* Return Value : channels/sensors that met comparator condition
******************************************************************************/
static uint32_t adc_get_and_clr_cmpi_flags(uint8_t unit)
{
    volatile uint32_t    flags;
    uint32_t tmp_flags;
    aregs_t     *p_regs;

    /* NOTE: S12CMPAI interrupts are level-based, not edge, so there will be
     * continuous triggers while condition is met. Writing 0 to the flags has
     * no effect UNTIL the condition clears.
     *
     * Also, the data register for the channel/sensor will still be written to.
     * The S12CMPAI interrupt does not replace it but is in addition to it.
     */

    /* Get S12AD register address */
    p_regs = (aregs_t *)&S12AD;

    /* Cast to match variable type */
    flags = (uint32_t)p_regs->ADCMPSR0.WORD;
    p_regs->ADCMPSR0.WORD = 0;

    if (0 == p_regs->ADCMPSR0.WORD)
    {
        R_BSP_NOP();
    }

    /* Cast to match variable type */
    tmp_flags = (((uint32_t)p_regs->ADCMPSR1.WORD) << 16);
    flags |= tmp_flags;
    p_regs->ADCMPSR1.WORD = 0;

    if (0 == p_regs->ADCMPSR1.WORD)
    {
        R_BSP_NOP();
    }

    if (1 == p_regs->ADCMPSER.BIT.CMPFTS)
    {
        flags |= ADC_MASK_TEMP;
    }

    if (1 == p_regs->ADCMPSER.BIT.CMPFOC)
    {
        flags |= ADC_MASK_VOLT;
    }

    p_regs->ADCMPSER.BYTE = 0;

    if (0 == p_regs->ADCMPSER.BYTE)
    {
        R_BSP_NOP();
    }

    return flags;
} /* End of function adc_get_and_clr_cmpi_flags() */


/*****************************************************************************
* Function Name: adc_get_and_clr_cmpi_flagsb
* Description  : Returns flags for all channels/sensors that met comparator
*                condition and clears the flags for next interrupt.
* Arguments    : unit -
*                    Unit number (0, 1 or 2)
* Return Value : channels/sensors that met comparator condition
******************************************************************************/
static uint32_t adc_get_and_clr_cmpi_flagsb(uint8_t unit)
{
    volatile uint32_t    flags;
    aregs_t     *p_regs;

    /* NOTE: S12CMPBI interrupts are level-based, not edge, so there will be
     * continuous triggers while condition is met. Writing 0 to the flags has
     * no effect UNTIL the condition clears.
     *
     * Also, the data register for the channel/sensor will still be written to.
     * The S12CMPBI interrupt does not replace it but is in addition to it.
     */

    /* Get S12AD register address */
    p_regs = (aregs_t *)&S12AD;

    /* Cast to match variable type */
    flags = (uint32_t)p_regs->ADCMPBSR.BIT.CMPSTB;
    p_regs->ADCMPBSR.BIT.CMPSTB = 0;
    if (0 == p_regs->ADCMPBSR.BIT.CMPSTB)
    {
        R_BSP_NOP();
    }

    return flags;
} /* End of function adc_get_and_clr_cmpi_flagsb() */


/******************************************************************************
* Function Name: adc_configure_scan
* Description  : This function does extensive checking on channel mask
*                settings based upon operational mode. Mask registers are
*                initialized and interrupts enabled in peripheral. Interrupts
*                are also enabled in ICU if corresponding priority is not 0.
*
* Arguments    : p_config
*                    Pointer to channel config structure containing masks
* Return Value : ADC_SUCCESS-
*                    Successful
*                ADC_ERR_INVALID_ARG-
*                    reg_id contains an invalid value.
*                ADC_ERR_ILLEGAL_ARG-
*                    an argument is illegal based upon rest of configuration
*******************************************************************************/
static adc_err_t adc_configure_scan(uint8_t const   unit,
                                    adc_ch_cfg_t   *p_config)
{

    /* Get S12AD register address */
    aregs_t     *p_regs = (aregs_t *)&S12AD;
    uint32_t  i;
    volatile uint32_t  tmp_mask=0;
    volatile uint16_t  trig_a;
    volatile uint16_t  trig_b;
    volatile uint16_t  trig_c;
    volatile uint8_t   tmp_adscs[ADC_0_REG_ARRAY_MAX];
    volatile uint32_t  conv_order_cnt;
    volatile uint32_t  bit_cnt;
    volatile uint32_t  tmp_adansa;
    volatile uint32_t  tmp_adansa_invert;

#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
    adc_err_t   err;

    err = adc_check_scan_config(unit, p_config);
    if (ADC_SUCCESS != err)
    {
        return err;
    }

#endif /* parameter checking */

    /* SET MASKS FOR CHANNELS AND SENSORS */

    /* Cast to match the register type */
    p_regs->ADANSA0.WORD = (uint16_t) (p_config->chan_mask & 0x0FFF);

    /* Cast to match the register type */
    p_regs->ADANSB0.WORD = (uint16_t) (p_config->chan_mask_groupb & 0x0FFF);

    /* Cast to match the register type */
    p_regs->ADANSC0.WORD = (uint16_t) (p_config->chan_mask_groupc & 0x0FFF);

    /* Cast to match the register type */
    p_regs->ADADS0.WORD = (uint16_t) (p_config->add_mask & 0x0FFF);

    /* Cast to match the register type */
    S12AD.ADANSA1.WORD = (uint16_t) ((p_config->chan_mask >> 16) & 0x00ff);

    /* Cast to match the register type */
    S12AD.ADANSB1.WORD = (uint16_t) ((p_config->chan_mask_groupb >> 16) & 0x00ff);

    /* Cast to match the register type */
    S12AD.ADANSC1.WORD = (uint16_t) ((p_config->chan_mask_groupc >> 16) & 0x00ff);

    /* Cast to match the register type */
    S12AD.ADADS1.WORD = (uint16_t) ((p_config->add_mask >> 16) & 0x00ff);

    adc_configure_sensors(p_config);

    /* SET ORDER OF CHANNEL CONVERSION */
    tmp_adansa = 0;
    tmp_adansa |= (p_config->chan_mask & 0x00030FFF);
    tmp_adansa |= (p_config->chan_mask_groupb & 0x00030FFF);
    tmp_adansa |= (p_config->chan_mask_groupc & 0x00030FFF);
    tmp_adansa_invert = ((~tmp_adansa) & 0x00030FFF);

    bit_cnt = 0;
    conv_order_cnt = 0;

    /* WAIT_LOOP */
    for (i= 0x00000001; i<0x00040000; i<<=1)
    {
        if (0 != (tmp_adansa & i))
        {
            bit_cnt++;
        }
    }

    /* WAIT_LOOP */
    for (i=bit_cnt; i<ADC_0_REG_ARRAY_MAX; i++)
    {

        /* WAIT_LOOP */
        while (conv_order_cnt<32)
        {
            if (1 == (tmp_adansa_invert & 0x00000001))
            {
                tmp_adscs[i] = conv_order_cnt;
                tmp_adansa_invert >>= 1;
                conv_order_cnt++;
                break;
            }

            tmp_adansa_invert >>= 1;
            conv_order_cnt++;
        }
    }

    conv_order_cnt = 0;

    if (ADC_CONV_ORDER_MANUAL_SETTING == p_config->conv_order_status)
    {

        /* WAIT_LOOP */
        for (i=0; i<bit_cnt; i++)
        {
            tmp_adscs[i] = 0;
            conv_order_cnt = p_config->conversion_order[i];

            /* WAIT_LOOP */
            while (conv_order_cnt > 1)
            {
                tmp_adscs[i]++;
                conv_order_cnt >>= 1;
            }
        }
    }
    else
    {

        /* WAIT_LOOP */
        for (i=0; i<bit_cnt; i++)
        {

            /* WAIT_LOOP */
            while (conv_order_cnt<32)
            {
                if (1 == (tmp_adansa & 0x00000001))
                {
                    tmp_adscs[i] = conv_order_cnt;
                    tmp_adansa >>= 1;
                    conv_order_cnt++;
                    break;
                }

                tmp_adansa >>= 1;
                conv_order_cnt++;
            }
        }
    }

    /* Set ADSCSn Register */
    p_regs->ADSCS0 = tmp_adscs[0];
    p_regs->ADSCS1 = tmp_adscs[1];
    p_regs->ADSCS2 = tmp_adscs[2];
    p_regs->ADSCS3 = tmp_adscs[3];
    p_regs->ADSCS4 = tmp_adscs[4];
    p_regs->ADSCS5 = tmp_adscs[5];
    p_regs->ADSCS6 = tmp_adscs[6];
    p_regs->ADSCS7 = tmp_adscs[7];

    if (2 == unit)
    {
        p_regs->ADSCS8  = tmp_adscs[8];
        p_regs->ADSCS9  = tmp_adscs[9];
        p_regs->ADSCS10 = tmp_adscs[10];
        p_regs->ADSCS11 = tmp_adscs[11];
        p_regs->ADSCS12 = tmp_adscs[12];
        p_regs->ADSCS13 = tmp_adscs[13];
    }

    /* SET GROUP A PRIORITY ACTION (not interrupt priority!) */
    if (ADC_GRPA_PRIORITY_OFF != p_config->priority_groupa)
    {
        trig_a = p_regs->ADSTRGR.BIT.TRSA;              // save TRSA
        trig_b = p_regs->ADSTRGR.BIT.TRSB;              // save TRSB
        trig_c = p_regs->ADGCTRGR.BIT.TRSC;             // save TRSC
        p_regs->ADSTRGR.BIT.TRSA = ADC_TRIG_NONE;
        p_regs->ADSTRGR.BIT.TRSB = ADC_TRIG_NONE;
        p_regs->ADGCTRGR.BIT.TRSC = ADC_TRIG_NONE;
        if (ADC_TRIG_NONE != p_regs->ADSTRGR.BIT.TRSA)  // bus wait for ADSTRGR write
        {
            R_BSP_NOP();
        }
        if (ADC_TRIG_NONE != p_regs->ADSTRGR.BIT.TRSB)  // bus wait for ADSTRGR write
        {
            R_BSP_NOP();
        }
        if (ADC_TRIG_NONE != p_regs->ADGCTRGR.BIT.TRSC) // bus wait for ADGCTRGR write
        {
            R_BSP_NOP();
        }

        p_regs->ADGSPCR.WORD = p_config->priority_groupa;

        p_regs->ADSTRGR.BIT.TRSA = trig_a;           // restore TRSA

        if (0 == p_regs->ADGSPCR.BIT.GBRP)
        {
            p_regs->ADSTRGR.BIT.TRSB = trig_b;   // restore TRSB
            p_regs->ADGCTRGR.BIT.TRSC = trig_c;  // restore TRSC
        }
        else
        {
            if (0 == p_regs->ADGCTRGR.BIT.GRCE)
            {
                p_regs->ADGCTRGR.BIT.TRSC = trig_c;  // restore TRSC
            }
            else
            {
                p_regs->ADSTRGR.BIT.TRSB = trig_b;   // restore TRSB
            }
        }
    }
    else
    {
        p_regs->ADGSPCR.WORD = p_config->priority_groupa;
    }

    /* SET SELF DIAGNOSIS REGISTERS (VIRTUAL CHANNEL) */
    if (ADC_DIAG_OFF == p_config->diag_method)
    {
        p_regs->ADCER.BIT.DIAGM = 0;
    }
    else
    {

        /* NOTE: Using Self Diagnosis adds 11/13/15(resolution) + (ch0 SST) ADCLKS to scan time. */
        /* (ch0 can still be used with self diagnosis on) */
        if (ADC_DIAG_ROTATE_VOLTS == p_config->diag_method)
        {
            p_regs->ADCER.BIT.DIAGLD = 0;
            p_regs->ADCER.BIT.DIAGVAL = 1;
        }
        else
        {
            p_regs->ADCER.BIT.DIAGLD = 1;
            p_regs->ADCER.BIT.DIAGVAL = (uint8_t)(p_config->diag_method & 0x3);   // 2-bit value;
        }
        p_regs->ADCER.BIT.DIAGM = 1;
    }

    /* SET DOUBLE TRIGGER CHANNEL */
    if (1 == p_regs->ADCSR.BIT.DBLE)
    {
        i = 0;
        tmp_mask = p_config->chan_mask;         // tmp_mask is non-Group/Group A chans

        /* WAIT_LOOP */
        while (tmp_mask >>= 1)                  // determine bit/ch number
        {
            i++;
        }
        p_regs->ADCSR.BIT.DBLANS = i;
    }

    p_regs->ADELCCR.BIT.ELCC = p_config->signal_elc;

    /* ENABLE INTERRUPTS */
    adc_enable_s12adi(unit);
    if (ADC_ADCS_GROUP_SCAN == p_regs->ADCSR.BIT.ADCS)
    {
        adc_enable_s12gbadi(unit);
        if (1 == p_regs->ADGCTRGR.BIT.GRCE)
        {
            adc_enable_s12gcadi(unit);
        }
    }

    return ADC_SUCCESS;
} /* End of function adc_configure_scan() */


/******************************************************************************
* Function Name: adc_check_scan_config
* Description  : This function does extensive checking on channel mask
*                settings based upon operational mode.
*
* NOTE: A negative number is stored in two's complement form.
*       A quick way to change a binary number into two's complement is to
*       start at the right (LSB) and moving left, don't change any bits
*       until after the first "1" is reached.
*       Number          2's complement
*       0010 0110       1101 1010
*       Another way is to do a 1's complement on the number, then add 1 to that.
*       Number          1's complement  + 1
*       0010 0110       1101 1001       1101 1010
*
* Arguments    : p_config
*                    Pointer to channel config structure containing masks
* Return Value : ADC_SUCCESS-
*                    Successful
*                ADC_ERR_INVALID_ARG-
*                    reg_id contains an invalid value.
*                ADC_ERR_ILLEGAL_ARG-
*                    an argument is illegal based upon rest of configuration
*******************************************************************************/
#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
static adc_err_t adc_check_scan_config(uint8_t const   unit,
                                        adc_ch_cfg_t   *p_config)
{
    volatile uint32_t    i = 0;
    uint32_t    tmp_mask = 0;
    volatile uint32_t    a_mask;
    volatile uint32_t    b_mask;
    volatile uint32_t    c_mask;
    volatile uint32_t    tmp_adcsr;
    volatile uint32_t    tmp_adansa;
    volatile uint32_t    bit_cnt;

    /* Get S12AD register address */
    aregs_t     *p_regs = (aregs_t *)&S12AD;

    /* Verify at least one bonded channel is selected */
    if ((0 == p_config->chan_mask)
    || ((p_config->chan_mask & ADC_PRV_INVALID_CH_MASK0) != 0))
    {
        return ADC_ERR_INVALID_ARG;
    }

    /* Verify at least one unique bonded channel is selected for Group B
     * and GroupA priority setting is valid.
     */
    if (ADC_ADCS_GROUP_SCAN == p_regs->ADCSR.BIT.ADCS)
    {
        if ((0 == p_config->chan_mask_groupb)
        || ((p_config->chan_mask_groupb & ADC_PRV_INVALID_CH_MASK0) != 0))
        {
            return ADC_ERR_INVALID_ARG;
        }
        else if ((p_config->chan_mask & p_config->chan_mask_groupb) != 0)
        {
            return ADC_ERR_ILLEGAL_ARG;         // same chan in both groups
        }
        else
        {
            tmp_mask = p_config->chan_mask_groupb;
        }

        if (1 == p_regs->ADGCTRGR.BIT.GRCE) // for Group C
        {
            if ((0 == p_config->chan_mask_groupc)
            || ((p_config->chan_mask_groupc & ADC_PRV_INVALID_CH_MASK0) != 0))
            {
                return ADC_ERR_INVALID_ARG;
            }
            else if (((p_config->chan_mask & p_config->chan_mask_groupb)
            || (p_config->chan_mask & p_config->chan_mask_groupc)
            || (p_config->chan_mask_groupb & p_config->chan_mask_groupc)) != 0)
            {
                return ADC_ERR_ILLEGAL_ARG;         // same chan in three groups
            }
            else
            {
                tmp_mask |= p_config->chan_mask_groupc;
            }
        }
    }
    else
    {

        /* for addition mask checking */
        if ((0 != p_config->chan_mask_groupb) || (0 != p_config->chan_mask_groupc))
        {
            return ADC_ERR_INVALID_ARG;
        }

        /* PGS bit must be set when group scan mode */
        if (ADC_GRPA_PRIORITY_OFF != p_config->priority_groupa)
        {
            return ADC_ERR_INVALID_ARG;
        }
    }

    /* Confirm that the temperature sensor settings of R_ADC_Open function and R_ADC_Control function match. */
    if ((((p_regs->ADEXICR.WORD & 0x0100) != 0) && ((p_config->chan_mask & ADC_MASK_TEMP) == 0))
    || (((p_regs->ADEXICR.WORD & 0x0100) == 0) && ((p_config->chan_mask & ADC_MASK_TEMP) != 0)))
    {
        return ADC_ERR_ILLEGAL_ARG;
    }

    if ((((p_regs->ADEXICR.WORD & 0x0400) != 0) && ((p_config->chan_mask_groupb & ADC_MASK_TEMP) == 0))
    || (((p_regs->ADEXICR.WORD & 0x0400) == 0) && ((p_config->chan_mask_groupb & ADC_MASK_TEMP) != 0)))
    {
        return ADC_ERR_ILLEGAL_ARG;
    }

    if ((((p_regs->ADGCEXCR.BYTE & 0x01) != 0) && ((p_config->chan_mask_groupc & ADC_MASK_TEMP) == 0))
    || (((p_regs->ADGCEXCR.BYTE & 0x01) == 0) && ((p_config->chan_mask_groupc & ADC_MASK_TEMP) != 0)))
    {
        return ADC_ERR_ILLEGAL_ARG;
    }

    if ((((p_regs->ADEXICR.WORD & 0x0001) != 0) && ((p_config->add_mask & ADC_MASK_TEMP) == 0))
    || (((p_regs->ADEXICR.WORD & 0x0001) == 0) && ((p_config->add_mask & ADC_MASK_TEMP) != 0)))
    {
        return ADC_ERR_ILLEGAL_ARG;
    }

    /* If sensors specified, verify in legal configuration */
    if ((p_config->chan_mask & ADC_MASK_SENSORS)
    || (p_config->chan_mask_groupb & ADC_MASK_SENSORS)
    || (p_config->chan_mask_groupc & ADC_MASK_SENSORS))
    {
        if (
        (1 == S12AD.ADCSR.BIT.DBLE)         // double trigger mode
        || (0 != (S12AD.ADDISCR.BYTE & 0x0F))  // disconnect detection assist enabled
        || (ADC_ADCS_CONT_SCAN == S12AD.ADCSR.BIT.ADCS)   // continuous scan mode
        || (ADC_DIAG_OFF != p_config->diag_method))        // self-diagnosis
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
    }

    /* When A/D conversion is performed for the temperature sensor output or 
       internal reference voltage, do not select analog input channels. */
    if (p_config->chan_mask & ADC_MASK_SENSORS)
    {
        if (0 != (p_config->chan_mask & (~ADC_MASK_SENSORS)))
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
    }

    if (ADC_ADCS_GROUP_SCAN == p_regs->ADCSR.BIT.ADCS)
    {
        if (p_config->chan_mask_groupb & ADC_MASK_SENSORS)
        {
            if (0 != (p_config->chan_mask_groupb & (~ADC_MASK_SENSORS)))
            {
                return ADC_ERR_ILLEGAL_ARG;
            }
        }

        if (1 == p_regs->ADGCTRGR.BIT.GRCE)
        {
            if (p_config->chan_mask_groupc & ADC_MASK_SENSORS)
            {
                if (0 != (p_config->chan_mask_groupc & (~ADC_MASK_SENSORS)))
                {
                    return ADC_ERR_ILLEGAL_ARG;
                }
            }
        }
    }

    /* Addition mask should not include bits from inactive channels */
    if (ADC_ADD_OFF != p_regs->ADADC.BIT.ADC)
    {
        tmp_mask |= p_config->chan_mask;        // tmp_mask is Group A and B and C combined

        /* Bit-AND with 1s-complement */
        if ((p_config->add_mask & (~tmp_mask)) != 0)
        {
            return ADC_ERR_INVALID_ARG;
        }
    }
    else
    {

        /* WARNING! Other features messed up if add_mask is non-zero when addition is turned off!*/
        p_config->add_mask = 0;
    }

    /* Verify only 1 bit is set in default/Group A mask */

    if ((ADC_MODE_SS_ONE_CH == g_dcb.mode)
    || (ADC_MODE_CONT_ONE_CH == g_dcb.mode)
    || (1 == p_regs->ADCSR.BIT.DBLE))          // double trigger mode
    {
        tmp_mask = p_config->chan_mask;         // tmp_mask is non-Group/Group A chans

        /* Bit-AND with 2s-complement (see note in function header) */
        if ((tmp_mask & ((~tmp_mask) + 1)) != tmp_mask)
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
    }

    if ((ADC_DIAG_OFF != p_config->diag_method)
    &&   ((0 != p_regs->ADADC.BIT.ADC)           // addition
        || (1 == p_regs->ADCSR.BIT.DBLE)          // double trigger mode
        || (1 == p_regs->ADCMPCR.BIT.CMPAE)       // comparator A
        || (1 == p_regs->ADCMPCR.BIT.CMPBE)       // comparator B
        || (0 != (p_regs->ADDISCR.BYTE & 0x0F)))) // disconnect detection assist
    {
        return ADC_ERR_ILLEGAL_ARG;
    }

    /* Parameter check of the order of channel conversion */
    if (ADC_CONV_ORDER_MANUAL_SETTING == p_config->conv_order_status)
    {
        tmp_mask = 0;

        /* WAIT_LOOP */
        for (i=0; i<ADC_0_REG_ARRAY_MAX; i++)    /* 0-23 */
        {

            /* Macro definition must be used for parameter setting. */
            /* Temperature sensors can not be used. */
            /* Internal reference voltage can not be used. */
            if ((ADC_MASK_CH0 != p_config->conversion_order[i])
            && (ADC_MASK_CH1 != p_config->conversion_order[i])
            && (ADC_MASK_CH2 != p_config->conversion_order[i])
            && (ADC_MASK_CH3 != p_config->conversion_order[i])
            && (ADC_MASK_CH4 != p_config->conversion_order[i])
            && (ADC_MASK_CH5 != p_config->conversion_order[i])
            && (ADC_MASK_CH6 != p_config->conversion_order[i])
            && (ADC_MASK_CH7 != p_config->conversion_order[i])
            && (ADC_MASK_CH8 != p_config->conversion_order[i])
            && (ADC_MASK_CH9 != p_config->conversion_order[i])
            && (ADC_MASK_CH10 != p_config->conversion_order[i])
            && (ADC_MASK_CH11 != p_config->conversion_order[i])
            && (ADC_MASK_CH12 != p_config->conversion_order[i])
            && (ADC_MASK_CH13 != p_config->conversion_order[i])
            && (ADC_MASK_CH14 != p_config->conversion_order[i])
            && (ADC_MASK_CH15 != p_config->conversion_order[i])
            && (ADC_MASK_CH16 != p_config->conversion_order[i])
            && (ADC_MASK_CH17 != p_config->conversion_order[i])
            && (ADC_MASK_CH18 != p_config->conversion_order[i])
            && (ADC_MASK_CH19 != p_config->conversion_order[i])
            && (ADC_MASK_CH20 != p_config->conversion_order[i])
            && (ADC_MASK_CH21 != p_config->conversion_order[i])
            && (ADC_MASK_CH22 != p_config->conversion_order[i])
            && (ADC_MASK_CH23 != p_config->conversion_order[i])
            && (ADC_MASK_CONV_ORDER_OFF != p_config->conversion_order[i]))
            {
                return ADC_ERR_ILLEGAL_ARG;
            }

            tmp_mask |= p_config->conversion_order[i];
        }

        /* Invalid channels should not be used. */
        if (0 != (tmp_mask & ADC_PRV_INVALID_CH_MASK0))
        {
            return ADC_ERR_ILLEGAL_ARG;
        }

        /* Only the channels specified to 1 (enable) in the A/D channel select register such as
           the ADANSA0/1, ADANSB0/1, and ADANSC0/1 registers should be used */
        /* All channels specified to 1 (enable) in the A/D channel select register such as
           the ADANSA0/1, ADANSB0/1, and ADANSC0/1 registers should be used. */
        tmp_adansa = 0;

        tmp_adansa |= (p_config->chan_mask & 0x00030FFF);
        tmp_adansa |= (p_config->chan_mask_groupb & 0x00030FFF);
        tmp_adansa |= (p_config->chan_mask_groupc & 0x00030FFF);

        if (tmp_mask != tmp_adansa)
        {
            return ADC_ERR_ILLEGAL_ARG;
        }

        /* Should be set ADC_MASK_CONV_ORDER_OFF to not used ADSCSn register. */
        bit_cnt = 0;

        /* WAIT_LOOP */
        for (i= 0x00000001; i<0x00040000; i<<=1)
        {
            if (0 != (tmp_mask & i))
            {
                bit_cnt++;
            }
        }

        /* WAIT_LOOP */
        for (i=bit_cnt; i<ADC_0_REG_ARRAY_MAX; i++)
        {
            if (ADC_MASK_CONV_ORDER_OFF != p_config->conversion_order[i])
            {
                return ADC_ERR_ILLEGAL_ARG;
            }
        }
    }

    return ADC_SUCCESS;
} /* End of function adc_check_scan_config() */
#endif /* ADC_CFG_PARAM_CHECKING_ENABLE == 1 */


/******************************************************************************
* Function Name: adc_configure_sensors
* Description  : This function set the sensor bits taking into account group
*                inclusion and addition/average mode.
* Arguments    : p_config -
*                   The configuration argument passed to ADC_CMD_ENABLE_CHANS.
* Return Value : none
*******************************************************************************/
static void adc_configure_sensors(adc_ch_cfg_t   *p_config)
{

    /* TEMPERATURE SENSOR */
    if ((p_config->chan_mask & ADC_MASK_TEMP)
    || (p_config->chan_mask_groupb & ADC_MASK_TEMP)
    || (p_config->chan_mask_groupc & ADC_MASK_TEMP))
    {

        /* Waiting time for sensors stabilization. 
           Stabilization time * margin = 200us * 1.1 = 220us  */
        R_BSP_SoftwareDelay(220, BSP_DELAY_MICROSECS);
    }

    /* INTERNAL REFERENCE VOLTAGE SENSOR */
    if ((p_config->chan_mask & ADC_MASK_VOLT)
    || (p_config->chan_mask_groupb & ADC_MASK_VOLT)
    || (p_config->chan_mask_groupc & ADC_MASK_VOLT))
    {
        S12AD.ADEXICR.BIT.OCSA = 0;
        S12AD.ADEXICR.BIT.OCSB = 0;
        S12AD.ADGCEXCR.BIT.OCSC = 0;

        S12AD.ADVMONCR.BIT.VDE = 1;
        S12AD.ADVMONO.BIT.VDO = 1;

        if (p_config->chan_mask & ADC_MASK_VOLT)
        {
            S12AD.ADEXICR.BIT.OCSA = 1;            // select temperature output to GroupA
            S12AD.ADEXICR.BIT.OCSB = 0;
            S12AD.ADGCEXCR.BIT.OCSC = 0;
        }
        else if (p_config->chan_mask_groupb & ADC_MASK_VOLT)
        {
            S12AD.ADEXICR.BIT.OCSA = 0;
            S12AD.ADEXICR.BIT.OCSB = 1;            // select temperature output to GroupB
            S12AD.ADGCEXCR.BIT.OCSC = 0;
        }
        else
        {
            S12AD.ADEXICR.BIT.OCSA = 0;
            S12AD.ADEXICR.BIT.OCSB = 0;
            S12AD.ADGCEXCR.BIT.OCSC = 1;           // select temperature output to GroupC
        }

        S12AD.ADEXICR.BIT.OCSAD = (p_config->add_mask & ADC_MASK_VOLT) ? 1 : 0;

        /* Waiting time for sensors stabilization.
           Stabilization time * margin = 400ns * 1.1 = 440ns
           The minimum wait time of R_BSP_SoftwareDelay is 1 us. */
        R_BSP_SoftwareDelay(1, BSP_DELAY_MICROSECS);
    }

    return;
} /* End of function adc_configure_sensors() */


/******************************************************************************
* Function Name: adc_enable_s12adi
* Description  : This function clears the S12ADI interrupt flag and enables
*                interrupts in the peripheral (for IR flag usage). If priority
*                is not 0, interrupts are enabled in the ICU.
*                NOTE: This has nothing to do with enabling triggers.
* Arguments    : unit -
*                   0 or 1
* Return Value : none
*******************************************************************************/
static void adc_enable_s12adi(uint8_t unit)
{
    IR(S12ADC0,S12ADI0) = 0;            // clear flag
    S12AD.ADCSR.BIT.ADIE = 1;           // enable in peripheral
    if (0 != IPR(S12ADC0,S12ADI0))
    {
        R_BSP_InterruptRequestEnable(VECT(S12ADC0,S12ADI0));       // enable in ICU
    }

} /* End of function adc_enable_s12adi() */


/******************************************************************************
* Function Name: adc_enable_s12gbadi
* Description  : This function clears the S12GBADI interrupt flag and enables
*                interrupts in the peripheral (for IR flag usage). If priority
*                is not 0, interrupts are enabled in the ICU.
*                NOTE: This has nothing to do with enabling triggers.
* Arguments    : unit -
*                   0 or 1
* Return Value : none
*******************************************************************************/
static void adc_enable_s12gbadi(uint8_t unit)
{
    IR(S12ADC0,S12GBADI0) = 0;            // clear flag
    S12AD.ADCSR.BIT.GBADIE = 1;           // enable in peripheral
    if (0 != IPR(S12ADC0,S12GBADI0))
    {
        R_BSP_InterruptRequestEnable(VECT(S12ADC0,S12GBADI0));       // enable in ICU
    }

} /* End of function adc_enable_s12gbadi() */


/******************************************************************************
* Function Name: adc_enable_s12gcadi
* Description  : This function clears the S12GCADI interrupt flag and enables
*                interrupts in the peripheral (for IR flag usage). If priority
*                is not 0, interrupts are enabled in the ICU.
*                NOTE: This has nothing to do with enabling triggers.
* Arguments    : unit -
*                   0 or 1
* Return Value : none
*******************************************************************************/
static void adc_enable_s12gcadi(uint8_t unit)
{
    IR(S12ADC0,S12GCADI0) = 0;           // clear flag
    S12AD.ADGCTRGR.BIT.GCADIE = 1;           // enable in peripheral
    if (0 != IPR(S12ADC0,S12GCADI0))
    {
        R_BSP_InterruptRequestEnable(VECT(S12ADC0,S12GCADI0));       // enable in ICU
    }

} /* End of function adc_enable_s12gcadi() */


/******************************************************************************
* Function Name: adc_close
* Description  : This is implementation of R_ADC_Close().
*                This function ends any scan in progress, disables interrupts,
*                and removes power to the A/D peripheral.
* Arguments    : none
* Return Value : ADC_SUCCESS - Successful
*                ADC_ERR_INVALID_ARG - unit contains an invalid value.
*******************************************************************************/
adc_err_t adc_close(uint8_t const  unit)
{

    /* Get S12AD register address */
    aregs_t     *p_regs = (aregs_t *)&S12AD;
    volatile    uint8_t i;
    uint32_t    adc_wait_microsecs;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif

#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
    if (unit > 0)
    {
        return ADC_ERR_INVALID_ARG;
    }
#endif

    /* Stop triggers & conversions, and disable peripheral interrupts */
    R_BSP_InterruptRequestDisable(VECT(S12ADC0,S12ADI0));         // disable interrupts in ICU
    R_BSP_InterruptRequestDisable(VECT(S12ADC0,S12GBADI0));       // disable interrupts in ICU
    R_BSP_InterruptRequestDisable(VECT(S12ADC0,S12GCADI0));       // disable interrupts in ICU

    IPR(S12ADC0,S12ADI0) = 0;
    IPR(S12ADC0,S12GBADI0) = 0;
    IPR(S12ADC0,S12GCADI0) = 0;

    /* This program does not disable IPR register of Group interrupt.
     * Because, other peripheral is possible to use same vector Group interrupt. */
    R_BSP_InterruptRequestDisable(VECT(ICU,GROUPBL1));                      // disable group

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    ICU.GENBL1.LONG &= (~g_dcb.cmpi_mask);  // disable group subvector
    ICU.GENBL1.LONG &= (~g_dcb.cmpi_maskb);   // disable group subvector
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    p_regs->ADCMPCR.BIT.CMPAIE = 0;             // disable interrupt in ADC
    if (0 == p_regs->ADCMPCR.BIT.CMPAIE)        // dummy read for waiting until set the value of CMPIE
    {
        R_BSP_NOP();
    }

    p_regs->ADCMPCR.BIT.CMPBIE = 0;             // disable interrupt in ADC
    if (0 == p_regs->ADCMPCR.BIT.CMPBIE)        // dummy read for waiting until set the value of CMPIE
    {
        R_BSP_NOP();
    }

    if (1 == p_regs->ADGSPCR.BIT.PGS)
    {
        p_regs->ADGSPCR.BIT.PGS = 0;
    }

    if (ADC_ADCS_GROUP_SCAN == p_regs->ADCSR.BIT.ADCS)
    {
        p_regs->ADSTRGR.WORD = 0x3F3F;
        if (1 == p_regs->ADGCTRGR.BIT.GRCE)
        {
            p_regs->ADGCTRGR.BIT.TRSC = 0x3F;
            p_regs->ADGCTRGR.BIT.GCADIE = 0;
            p_regs->ADGCTRGR.BIT.GRCE = 0;
        }
        p_regs->ADCSR.BIT.GBADIE = 0;
    }
    else
    {
        p_regs->ADSTRGR.BIT.TRSA = 0x3F;
        p_regs->ADCSR.BIT.ADIE = 0;
    }

    p_regs->ADCSR.WORD = 0;
    if (0 != p_regs->ADCSR.WORD)
    {

        /* Dommy read and compare */
        R_BSP_NOP();
    }

    /* Wait for 2 ADCLK cycles (1 PCLKB + 1 ADCLK when ADCLK is faster than PCLKB */
    if (BSP_PCLKD_HZ > BSP_PCLKB_HZ)
    {
        /* 1PCLK + 1ADCLK Round up the decimal point */
        adc_wait_microsecs =  (uint32_t)((1000000 + (BSP_PCLKB_HZ - 1)) / BSP_PCLKB_HZ)
                            + (uint32_t)((1000000 + (BSP_PCLKD_HZ - 1)) / BSP_PCLKD_HZ);
    }
    else
    {
        /* 2ADCLK Round up the decimal point */
        adc_wait_microsecs =  (uint32_t)((2000000 + (BSP_PCLKD_HZ - 1)) / BSP_PCLKD_HZ);
    }
    R_BSP_SoftwareDelay (adc_wait_microsecs, BSP_DELAY_MICROSECS);

    /* Disabled the internal reference voltage monitoring function */
    S12AD.ADVMONO.BIT.VDO = 0;
    S12AD.ADVMONCR.BIT.VDE = 0;

    IR(S12ADC0,S12ADI0) = 0;          // clear interrupt flag
    IR(S12ADC0,S12GBADI0) = 0;        // clear interrupt flag
    IR(S12ADC0,S12GCADI0) = 0;        // clear interrupt flag

    if(0 != ICU.GENBL1.LONG)
    {
        R_BSP_InterruptRequestEnable(VECT(ICU,GROUPBL1));                      // enable group
    }

    /* Power down peripheral */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif

    MSTP(S12AD) = 1;

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Show driver as closed */
    g_dcb.opened = false;

    return ADC_SUCCESS;
} /* End of function adc_close() */

/******************************************************************************
* Function Name: adc_read_all
* Description  : This is implementation of R_ADC_ReadAll().
*                It reads conversion results from all potential sources,
*                enabled or not.
* Arguments    : p_data-
*                    Pointer to structure to load register values into.
* Return Value : ADC_SUCCESS-
*                    Successful
*******************************************************************************/
adc_err_t adc_read_all(adc_data_t * const  p_all_data)
{
    p_all_data->unit0.chan[ADC_REG_CH0]   = S12AD.ADDR0;
    p_all_data->unit0.chan[ADC_REG_CH1]   = S12AD.ADDR1;
    p_all_data->unit0.chan[ADC_REG_CH2]   = S12AD.ADDR2;
    p_all_data->unit0.chan[ADC_REG_CH3]   = S12AD.ADDR3;
    p_all_data->unit0.chan[ADC_REG_CH4]   = S12AD.ADDR4;
    p_all_data->unit0.chan[ADC_REG_CH5]   = S12AD.ADDR5;
    p_all_data->unit0.chan[ADC_REG_CH6]   = S12AD.ADDR6;
    p_all_data->unit0.chan[ADC_REG_CH7]   = S12AD.ADDR7;
    p_all_data->unit0.chan[ADC_REG_CH8]   = S12AD.ADDR8;
    p_all_data->unit0.chan[ADC_REG_CH9]   = S12AD.ADDR9;
    p_all_data->unit0.chan[ADC_REG_CH10]  = S12AD.ADDR10;
    p_all_data->unit0.chan[ADC_REG_CH11]  = S12AD.ADDR11;
    p_all_data->unit0.chan[ADC_REG_CH12]  = S12AD.ADDR12;
    p_all_data->unit0.chan[ADC_REG_CH13]  = S12AD.ADDR13;
    p_all_data->unit0.chan[ADC_REG_CH14]  = S12AD.ADDR14;
    p_all_data->unit0.chan[ADC_REG_CH15]  = S12AD.ADDR15;
    p_all_data->unit0.chan[ADC_REG_CH16]  = S12AD.ADDR16;
    p_all_data->unit0.chan[ADC_REG_CH17]  = S12AD.ADDR17;
    p_all_data->unit0.chan[ADC_REG_CH18]  = S12AD.ADDR18;
    p_all_data->unit0.chan[ADC_REG_CH19]  = S12AD.ADDR19;
    p_all_data->unit0.chan[ADC_REG_CH20]  = S12AD.ADDR20;
    p_all_data->unit0.chan[ADC_REG_CH21]  = S12AD.ADDR21;
    p_all_data->unit0.chan[ADC_REG_CH22]  = S12AD.ADDR22;
    p_all_data->unit0.chan[ADC_REG_CH23]  = S12AD.ADDR23;
    p_all_data->unit0.temp                = S12AD.ADTSDR;
    p_all_data->unit0.volt                = S12AD.ADOCDR;
    p_all_data->unit0.dbltrig             = S12AD.ADDBLDR;
    p_all_data->unit0.dbltrigA            = S12AD.ADDBLDRA;
    p_all_data->unit0.dbltrigB            = S12AD.ADDBLDRB;
    p_all_data->unit0.self_diag           = S12AD.ADRD.WORD;

    return ADC_SUCCESS;
} /* End of function adc_read_all() */


/*****************************************************************************
* Function Name: adc_s12cmpia0_isr
* Description  : Unit0 interrupt handler (bsp GRPBL1 callback) for comparator.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void adc_s12cmpia0_isr(void *cb_args)
{
    adc_cb_args_t   args;

    if ((NULL != g_dcb.p_callback) && (FIT_NO_FUNC != g_dcb.p_callback))
    {

        /* NOTE: S12CMPI interrupts are level-based, not edge, so there will be
         * continuous triggers while condition is met. When the condition is no
         * longer met, both the ICU.GRPBL1 flag and subvector flag are
         * automatically cleared.
         */
        args.event = ADC_EVT_CONDITION_MET;
        args.compare_flags = adc_get_and_clr_cmpi_flags(0);
        g_dcb.p_callback(&args);
    }
    else
    {
        adc_get_and_clr_cmpi_flags(0);
    }
} /* End of function adc_s12cmpia0_isr() */


/*****************************************************************************
* Function Name: adc_s12cmpib0_isr
* Description  : Unit0 interrupt handler (bsp GRPBL1 callback) for comparator.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void adc_s12cmpib0_isr(void *cb_args)
{
    adc_cb_args_t   args;

    if ((NULL != g_dcb.p_callback) && (FIT_NO_FUNC != g_dcb.p_callback))
    {

        /* NOTE: S12CMPI interrupts are level-based, not edge, so there will be
         * continuous triggers while condition is met. When the condition is no
         * longer met, both the ICU.GRPBL1 flag and subvector flag are
         * automatically cleared.
         */
        args.event = ADC_EVT_CONDITION_METB;
        args.compare_flagsb = adc_get_and_clr_cmpi_flagsb(0);
        g_dcb.p_callback(&args);
    }
    else
    {
        adc_get_and_clr_cmpi_flagsb(0);
    }
} /* End of function adc_s12cmpib0_isr() */

/*****************************************************************************
* Function Name: adc_s12adi0_isr
* Description  : Unit0 interrupt handler for normal/Group A/double trigger
*                scan complete.
* Arguments    : none
* Return Value : none
******************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void adc_s12adi0_isr(void)
{
    adc_cb_args_t   args;

    if ((NULL != g_dcb.p_callback) && (FIT_NO_FUNC != g_dcb.p_callback))
    {
        args.event = ADC_EVT_SCAN_COMPLETE;
        g_dcb.p_callback(&args);
    }
} /* End of function adc_s12adi0_isr() */

/*****************************************************************************
* Function Name: adc_s12gbadi0_isr
* Description  : Unit0 interrupt handler for Group B scan complete.
* Arguments    : none
* Return Value : none
******************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void adc_s12gbadi0_isr(void)
{
    adc_cb_args_t   args;

    if ((NULL != g_dcb.p_callback) && (FIT_NO_FUNC != g_dcb.p_callback))
    {
        args.event = ADC_EVT_SCAN_COMPLETE_GROUPB;
        g_dcb.p_callback(&args);
    }
} /* End of function adc_s12gbadi0_isr() */

/*****************************************************************************
* Function Name: adc_s12gcadi0_isr
* Description  : Unit0 interrupt handler for Group C scan complete.
* Arguments    : none
* Return Value : none
******************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void adc_s12gcadi0_isr(void)
{
    adc_cb_args_t   args;

    if ((NULL != g_dcb.p_callback) && (FIT_NO_FUNC != g_dcb.p_callback))
    {
        args.event = ADC_EVT_SCAN_COMPLETE_GROUPC;
        g_dcb.p_callback(&args);
    }
} /* End of function adc_s12gcadi0_isr() */

#endif /* #if defined BSP_MCU_RX660 */

