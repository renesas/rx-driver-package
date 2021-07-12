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
* Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_s12ad_rx24u.c
* Description  : Primary source code for 12-bit A/D Converter driver.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*           10.06.2020 1.00    Initial Release.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include <stdio.h>
#include "platform.h"

#if defined (BSP_MCU_RX24U)
#include "r_s12ad_rx_config.h"
#include "r_s12ad_rx_private.h"
#include "r_s12ad_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define ADC_PRV_GET_SST_ARR(x)     (((x)==0) ?  gp_sreg0_ptrs : \
                                    ((x)==1) ?  gp_sreg1_ptrs : \
                                                gp_sreg2_ptrs)

#define ADC_PRV_LOCK(x)             (((x)==0) ? BSP_LOCK_S12AD  : \
                                    ((x)==1) ?  BSP_LOCK_S12AD1 : \
                                                BSP_LOCK_S12AD2)

#define ADC_PRV_INVALID_CH_MASK(x)     (((x)==0) ?  ADC_PRV_INVALID_CH_MASK0 : \
                                    ((x)==1) ?  ADC_PRV_INVALID_CH_MASK1 : \
                                                ADC_PRV_INVALID_CH_MASK2)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Unit1 registers are a superset of Unit0, 1 EXCEPT that they do not contain the S&H registers */
typedef R_BSP_VOLATILE_EVENACCESS struct st_s12ad1  aregs_t;
typedef R_BSP_VOLATILE_EVENACCESS struct st_s12ad2  aregs2_t;

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/* In RAM */
adc_ctrl_t g_dcb[3] = {{ADC_MODE_MAX,
                        NULL,
                        false},

                        {ADC_MODE_MAX,
                        NULL,
                        false},

                        {ADC_MODE_MAX,
                        NULL,
                        false}};

/* In ROM. A/D Data Register pointers */

volatile uint16_t R_BSP_EVENACCESS_SFR * const  gp_dreg0_ptrs[ADC_REG_MAX+1] =
                    {

                        /* A/D data register */
                        &S12AD.ADDR0,       /* channel 0 */
                        &S12AD.ADDR1,       /* channel 1 */
                        &S12AD.ADDR2,       /* channel 2 */
                        &S12AD.ADDR3,       /* channel 3 */
                        &S12AD.ADDR16,      /* channel 16 */
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        &S12AD.ADDBLDR,     /* double trigger register */
                        &S12AD.ADDBLDRA,    /* double trigger registerA */
                        &S12AD.ADDBLDRB,    /* double trigger registerB */
                        &S12AD.ADRD    /* self-diagnosis register */
                    };

volatile uint16_t R_BSP_EVENACCESS_SFR * const  gp_dreg1_ptrs[ADC_REG_MAX+1] =
                    {

                        /* A/D data register */
                        &S12AD1.ADDR0,       /* channel 0 */
                        &S12AD1.ADDR1,       /* channel 1 */
                        &S12AD1.ADDR2,       /* channel 2 */
                        &S12AD1.ADDR3,       /* channel 3 */
                        &S12AD1.ADDR16,      /* channel 16 */
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        &S12AD1.ADDBLDR,     /* double trigger register */
                        &S12AD1.ADDBLDRA,    /* double trigger registerA */
                        &S12AD1.ADDBLDRB,    /* double trigger registerB */
                        &S12AD1.ADRD    /* self-diagnosis register */
                    };

volatile uint16_t R_BSP_EVENACCESS_SFR * const  gp_dreg2_ptrs[ADC_REG_MAX+1] =
                    {

                        /* A/D data register */
                        &S12AD2.ADDR0,       /* channel 0 */
                        &S12AD2.ADDR1,       /* channel 1 */
                        &S12AD2.ADDR2,       /* channel 2 */
                        &S12AD2.ADDR3,       /* channel 3 */
                        &S12AD2.ADDR4,       /* channel 4 */
                        &S12AD2.ADDR5,       /* channel 5 */
                        &S12AD2.ADDR6,       /* channel 6 */
                        &S12AD2.ADDR7,       /* channel 7 */
                        &S12AD2.ADDR8,       /* channel 8 */
                        &S12AD2.ADDR9,       /* channel 9 */
                        &S12AD2.ADDR10,      /* channel 10 */
                        &S12AD2.ADDR11,      /* channel 11 */
                        &S12AD2.ADOCDR,      /* voltage sensor */
                        &S12AD2.ADDBLDR,     /* double trigger register */
                        &S12AD2.ADDBLDRA,    /* double trigger registerA */
                        &S12AD2.ADDBLDRB,    /* double trigger registerB */
                        &S12AD2.ADRD    /* self-diagnosis register */
                    };

/* In ROM. Sample State (SST) Register pointers */

/* 8-bit register pointers */
volatile uint8_t R_BSP_EVENACCESS_SFR * const  gp_sreg0_ptrs[ADC_SST_REG_MAX+1] =
                    {

                        /* A/D Sampling State Register */
                        &S12AD.ADSSTR0,     /* channel 0 */
                        &S12AD.ADSSTR1,     /* channel 1 */
                        &S12AD.ADSSTR2,     /* channel 2 */
                        &S12AD.ADSSTR3,     /* channel 3 */
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        &S12AD.ADSSTRL,     /* channel 16 */
                        NULL
                    };

volatile uint8_t R_BSP_EVENACCESS_SFR * const  gp_sreg1_ptrs[ADC_SST_REG_MAX+1] =
                    {

                        /* A/D Sampling State Register */
                            &S12AD1.ADSSTR0,     /* channel 0 */
                            &S12AD1.ADSSTR1,     /* channel 1 */
                            &S12AD1.ADSSTR2,     /* channel 2 */
                            &S12AD1.ADSSTR3,     /* channel 3 */
                            NULL,
                            NULL,
                            NULL,
                            NULL,
                            NULL,
                            NULL,
                            NULL,
                            NULL,
                            &S12AD1.ADSSTRL,     /* channel 16 */
                            NULL
                    };

volatile uint8_t R_BSP_EVENACCESS_SFR * const  gp_sreg2_ptrs[ADC_SST_REG_MAX+1] =
                    {

                        /* A/D Sampling State Register */
                            &S12AD2.ADSSTR0,     /* channel 0 */
                            &S12AD2.ADSSTR1,     /* channel 1 */
                            &S12AD2.ADSSTR2,     /* channel 2 */
                            &S12AD2.ADSSTR3,     /* channel 3 */
                            &S12AD2.ADSSTR4,     /* channel 4 */
                            &S12AD2.ADSSTR5,     /* channel 5 */
                            &S12AD2.ADSSTR6,     /* channel 6 */
                            &S12AD2.ADSSTR7,     /* channel 7 */
                            &S12AD2.ADSSTR8,     /* channel 8 */
                            &S12AD2.ADSSTR9,     /* channel 9 */
                            &S12AD2.ADSSTR10,    /* channel 10 */
                            &S12AD2.ADSSTR11,    /* channel 11 */
                            NULL,
                            &S12AD2.ADSSTRO      /* voltage sensor */
                    };

#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
static adc_err_t adc_check_open_cfg(uint8_t            unit,
                                    adc_mode_t  const  mode,
                                    adc_cfg_t * const  p_cfg,
                                    void     (* const  p_callback)(void *p_args));

static adc_err_t adc_check_scan_config(uint8_t const unit,  adc_ch_cfg_t *p_config);
#endif

static adc_err_t adc_configure_scan(uint8_t const unit,  adc_ch_cfg_t *p_config);
static void adc_configure_sensors(adc_ch_cfg_t  *p_config);
static void adc_enable_s12adi(uint8_t unit);
static void adc_enable_s12gbadi(uint8_t unit);
static void adc_enable_s12gcadi(uint8_t unit);

R_BSP_PRAGMA_STATIC_INTERRUPT(adc_s12adi0_isr, VECT(S12AD,S12ADI))
R_BSP_PRAGMA_STATIC_INTERRUPT(adc_s12adi1_isr, VECT(S12AD1,S12ADI1))
R_BSP_PRAGMA_STATIC_INTERRUPT(adc_s12adi2_isr, VECT(S12AD2,S12ADI2))
R_BSP_PRAGMA_STATIC_INTERRUPT(adc_s12gbadi0_isr, VECT(S12AD,GBADI))
R_BSP_PRAGMA_STATIC_INTERRUPT(adc_s12gbadi1_isr, VECT(S12AD1,GBADI1))
R_BSP_PRAGMA_STATIC_INTERRUPT(adc_s12gbadi2_isr, VECT(S12AD2,GBADI2))
R_BSP_PRAGMA_STATIC_INTERRUPT(adc_s12gcadi0_isr, VECT(S12AD,GCADI))
R_BSP_PRAGMA_STATIC_INTERRUPT(adc_s12gcadi1_isr, VECT(S12AD1,GCADI1))
R_BSP_PRAGMA_STATIC_INTERRUPT(adc_s12gcadi2_isr, VECT(S12AD2,GCADI2))

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
*                    0
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
adc_err_t adc_open(uint8_t     const  unit,
                    adc_mode_t  const  mode,
                    adc_cfg_t * const  p_cfg,
                    void     (* const  p_callback)(void *p_args))
{
    aregs_t     *p_regs;
    aregs2_t     *p_regs2;
    volatile uint16_t   u16_dummy;  /* Dummy read for "1" change to "0".(read first) */
    volatile uint8_t    u8_dummy;   /* Dummy read for "1" change to "0".(read first) */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    volatile uint32_t   ipl_value;
#endif

#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
    adc_err_t   err;

    /* Pointer check */
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
    if (true == g_dcb[unit].opened)
    {
        return ADC_ERR_AD_NOT_CLOSED;
    }
    if (R_BSP_HardwareLock(ADC_PRV_LOCK(unit)) == false)
    {
        return ADC_ERR_AD_LOCKED;
    }

    /* APPLY POWER TO PERIPHERAL */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    /* Interrupt_disable */
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, (void *)&ipl_value);
#endif

    /* S12AD module clock is enabled */
    if (0 == unit)
    {
        MSTP(S12AD) = 0;
    }
    else if (1 == unit)
    {
        MSTP(S12AD1) = 0;
    }
    else
    {
        MSTP(S12AD2) = 0;
    }

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    /* Interrupt enable */
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, (void *)&ipl_value);
#endif

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    if (2 == unit)
    {
        /* A/D register address */
        p_regs2 = (aregs2_t *)&S12AD2;

        /* Initialize S12AD2 register */

        /* A/D Control Register initialize */
        p_regs2->ADCSR.WORD = 0;

        /* A/D Channel Select Register A0 initialize */
        p_regs2->ADANSA0.WORD = 0;

        /* A/D-Converted Value Addition/Average Function Select Register 0 initialize */
        p_regs2->ADADS0.WORD = 0;

        /* A/D-Converted Value Addition/Average Count Select Register initialize */
        p_regs2->ADADC.BYTE = 0;

        /* A/D Control Extended Register initialize */
        p_regs2->ADCER.WORD = 0;

        /* A/D Conversion Start Trigger Select Register initialize */
        p_regs2->ADSTRGR.WORD = 0;

        /* A/D Channel Select Register B0 initialize */
        p_regs2->ADANSB0.WORD = 0;

        /* A/D Disconnection Detection Control Register initialize */
        p_regs2->ADDISCR.BYTE = 0;

        /* A/D Group Scan Priority Control Register initialize */
        p_regs2->ADGSPCR.WORD = 0;

        /* A/D Channel Select Register C0 initialize */
        p_regs2->ADANSC0.WORD = 0;

        /* A/D Group C Trigger Select Register initialize */
        p_regs2->ADGCTRGR.BYTE = 0;

        /* A/D Sampling State Register initialize */
        p_regs2->ADSSTR0  = ADC_SST_CNT_DEFAULT;
        p_regs2->ADSSTR1  = ADC_SST_CNT_DEFAULT;
        p_regs2->ADSSTR2  = ADC_SST_CNT_DEFAULT;
        p_regs2->ADSSTR3  = ADC_SST_CNT_DEFAULT;
        p_regs2->ADSSTR4  = ADC_SST_CNT_DEFAULT;
        p_regs2->ADSSTR5  = ADC_SST_CNT_DEFAULT;
        p_regs2->ADSSTR6  = ADC_SST_CNT_DEFAULT;
        p_regs2->ADSSTR7  = ADC_SST_CNT_DEFAULT;
        p_regs2->ADSSTR8  = ADC_SST_CNT_DEFAULT;
        p_regs2->ADSSTR9  = ADC_SST_CNT_DEFAULT;
        p_regs2->ADSSTR10 = ADC_SST_CNT_DEFAULT;
        p_regs2->ADSSTR11 = ADC_SST_CNT_DEFAULT;
        p_regs2->ADSSTRO  = ADC_SST_CNT_DEFAULT;

        /* A/D Conversion Extended Input Control Register initialize */
        p_regs2->ADEXICR.WORD = 0;

        /* SET MODE RELATED REGISTER FIELDS */
        g_dcb[unit].mode = mode;
        if ((ADC_MODE_SS_MULTI_CH_GROUPED == mode)
        || (ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A == mode))
        {
            p_regs2->ADCSR.BIT.ADCS = ADC_ADCS_GROUP_SCAN;
        }
        else if ((ADC_MODE_SS_MULTI_CH_GROUPED_GROUPC == mode)
        || (ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A_GROUPC == mode))
        {
            p_regs2->ADCSR.BIT.ADCS = ADC_ADCS_GROUP_SCAN;
            p_regs2->ADGCTRGR.BIT.GRCE = 1;       // Use Group C
        }
        else
        {
            if ((ADC_MODE_CONT_ONE_CH == mode) || (ADC_MODE_CONT_MULTI_CH == mode))
            {
                p_regs2->ADCSR.BIT.ADCS = ADC_ADCS_CONT_SCAN;
            }

            /* other modes have ADCS=0 */
        }

        if ((ADC_MODE_SS_ONE_CH_DBLTRIG == mode)
        || (ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A == mode)
        || (ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A_GROUPC == mode))
        {
            p_regs2->ADCSR.BIT.DBLE = 1;         /* enable double trigger */
        }

        /* SET TRIGGER AND INTERRUPT PRIORITY REGISTER FIELDS */
        if (ADC_TRIG_SOFTWARE != p_cfg->trigger)
        {
            p_regs2->ADSTRGR.BIT.TRSA = p_cfg->trigger;
        }

        if (ADC_TRIG_ASYNC_ADTRG == p_cfg->trigger)
        {
            p_regs2->ADCSR.BIT.EXTRG = 1;        /* set ext trigger for async trigger */
        }

        if (ADC_ADCS_GROUP_SCAN == p_regs2->ADCSR.BIT.ADCS)
        {
            /* for Group B */
            p_regs2->ADSTRGR.BIT.TRSB = p_cfg->trigger_groupb;

            /* for Group C */
            p_regs2->ADGCTRGR.BIT.TRSC = p_cfg->trigger_groupc;

            /* for Group B */
            IPR(S12AD2,GBADI2) = p_cfg->priority_groupb;

            /* for Group C */
            IPR(S12AD2,GCADI2) = p_cfg->priority_groupc;
        }

        /* for Group A */
        IPR(S12AD2,S12ADI2) = p_cfg->priority;

        /* SET REGISTER FIELDS FOR REMAINING PARAMETERS */
        p_regs2->ADCER.BIT.ADRFMT = (ADC_ALIGN_LEFT == p_cfg->alignment) ? 1 : 0;
        p_regs2->ADCER.BIT.ACE = (ADC_CLEAR_AFTER_READ_ON == p_cfg->clearing) ? 1 : 0;

        p_regs2->ADADC.BYTE = p_cfg->add_cnt;
    }
    else
    {
        /* A/D register address */
        if (0 == unit)
        {
            p_regs = (aregs_t *)&S12AD;
        }
        else
        {
            p_regs = (aregs_t *)&S12AD1;
        }

        /* Initialize S12AD and S12AD1 register */

        /* A/D Control Register initialize */
        p_regs->ADCSR.WORD = 0;

        /* A/D Channel Select Register A0 initialize */
        p_regs->ADANSA0.WORD = 0;

        /* A/D Channel Select Register A1 initialize */
        p_regs->ADANSA1.WORD = 0;

        /* A/D-Converted Value Addition/Average Function Select Register 0 initialize */
        p_regs->ADADS0.WORD = 0;

        /* A/D-Converted Value Addition/Average Function Select Register 1 initialize */
        p_regs->ADADS1.WORD = 0;

        /* A/D-Converted Value Addition/Average Count Select Register initialize */
        p_regs->ADADC.BYTE = 0;

        /* A/D Control Extended Register initialize */
        p_regs->ADCER.WORD = 0;

        /* A/D Conversion Start Trigger Select Register initialize */
        p_regs->ADSTRGR.WORD = 0;

        /* A/D Channel Select Register B0 initialize */
        p_regs->ADANSB0.WORD = 0;

        /* A/D Channel Select Register B1 initialize */
        p_regs->ADANSB1.WORD = 0;

        /* A/D Disconnection Detection Control Register initialize */
        p_regs->ADDISCR.BYTE = 0;

        /* A/D Group Scan Priority Control Register initialize */
        p_regs->ADGSPCR.WORD = 0;

        /* A/D Channel Select Register C0 initialize */
        p_regs->ADANSC0.WORD = 0;

        /* A/D Channel Select Register C1 initialize */
        p_regs->ADANSC1.WORD = 0;

        /* A/D Group C Trigger Select Register initialize */
        p_regs->ADGCTRGR.BYTE = 0;

        /* A/D Sampling State Register initialize */
        p_regs->ADSSTR0 = ADC_SST_CNT_DEFAULT;
        p_regs->ADSSTR1 = ADC_SST_CNT_DEFAULT;
        p_regs->ADSSTR2 = ADC_SST_CNT_DEFAULT;
        p_regs->ADSSTR3 = ADC_SST_CNT_DEFAULT;
        p_regs->ADSSTRL = ADC_SST_CNT_DEFAULT;

        /* Only unit1 has Sample-and-Hold circuit */
        if (1 == unit)
        {
            /* A/D Sample-and-Hold Circuit Control Register initialize */
            p_regs->ADSHCR.WORD = ADC_SST_SH_CNT_DEFAULT;
        }

        /* A/D Programmable Gain Amplifier Control Register initialize */
        p_regs->ADPGACR.WORD = 0x9999;

        /* A/D Programmable Gain Amplifier Gain Setting Register 0 initialize */
        p_regs->ADPGAGS0.WORD = 0x0000;

        /* SET MODE RELATED REGISTER FIELDS */
        g_dcb[unit].mode = mode;
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
            p_regs->ADCSR.BIT.DBLE = 1;         /* enable double trigger */
        }

        /* SET TRIGGER AND INTERRUPT PRIORITY REGISTER FIELDS */
        if (ADC_TRIG_SOFTWARE != p_cfg->trigger)
        {
            p_regs->ADSTRGR.BIT.TRSA = p_cfg->trigger;
        }

        if (ADC_TRIG_ASYNC_ADTRG == p_cfg->trigger)
        {
            p_regs->ADCSR.BIT.EXTRG = 1;        /* set ext trigger for async trigger */
        }

        if (ADC_ADCS_GROUP_SCAN == p_regs->ADCSR.BIT.ADCS)
        {
            /* for Group B */
            p_regs->ADSTRGR.BIT.TRSB = p_cfg->trigger_groupb;

            /* for Group C */
            p_regs->ADGCTRGR.BIT.TRSC = p_cfg->trigger_groupc;

            if (0 == unit)
            {
                /* for Group B */
                IPR(S12AD,GBADI) = p_cfg->priority_groupb;

                /* for Group C */
                IPR(S12AD,GCADI) = p_cfg->priority_groupc;
            }
            else
            {
                /* for Group B */
                IPR(S12AD1,GBADI1) = p_cfg->priority_groupb;

                /* for Group C */
                IPR(S12AD1,GCADI1) = p_cfg->priority_groupc;
            }
        }

        /* for Group A */
        if (0 == unit)
        {
            IPR(S12AD,S12ADI) = p_cfg->priority;
        }
        else
        {
            IPR(S12AD1,S12ADI1) = p_cfg->priority;
        }

        /* SET REGISTER FIELDS FOR REMAINING PARAMETERS */
        p_regs->ADCER.BIT.ADRFMT = (ADC_ALIGN_LEFT == p_cfg->alignment) ? 1 : 0;
        p_regs->ADCER.BIT.ACE = (ADC_CLEAR_AFTER_READ_ON == p_cfg->clearing) ? 1 : 0;

        p_regs->ADADC.BYTE = p_cfg->add_cnt;
    }


    /* SAVE CALLBACK FUNCTION POINTER */
    g_dcb[unit].p_callback = p_callback;

    /* MARK DRIVER AS OPENED */
    g_dcb[unit].opened = true;
    R_BSP_HardwareUnlock(ADC_PRV_LOCK(unit));

    return ADC_SUCCESS;
} /* End of function adc_open() */


/******************************************************************************
* Function Name: adc_check_open_cfg
* Description  : If Parameter Checking is enabled, this function will simply
*                validate the parameters passed to the adc_open function.
** Arguments    : unit -
*                    0
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
    if ((0 != p_cfg->priority) && ((NULL == p_callback) || (FIT_NO_FUNC == p_callback)))
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
        if ((ADC_TRIG_ASYNC_ADTRG == p_cfg->trigger_groupb)
        || (p_cfg->trigger == p_cfg->trigger_groupb)
        || (ADC_TRIG_SOFTWARE == p_cfg->trigger)
        || (ADC_TRIG_SOFTWARE == p_cfg->trigger_groupb))
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
        }

        if (p_cfg->priority_groupb > BSP_MCU_IPL_MAX)
        {
            return ADC_ERR_INVALID_ARG;
        }

        if ((0 != p_cfg->priority_groupb)   /* Interrupt driven, must have callback function */
        && ((NULL == p_callback) || (FIT_NO_FUNC == p_callback)))
        {
            return ADC_ERR_ILLEGAL_ARG;
        }

        /* for Group C */
        if ((ADC_MODE_SS_MULTI_CH_GROUPED_GROUPC == mode) || (ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A_GROUPC == mode))
        {
            if (p_cfg->priority_groupc > BSP_MCU_IPL_MAX)
            {
                return ADC_ERR_INVALID_ARG;
            }

            if ((0 != p_cfg->priority_groupc)   /* Interrupt driven, must have callback function */
            && ((NULL == p_callback) || (FIT_NO_FUNC == p_callback)))
            {
                return ADC_ERR_ILLEGAL_ARG;
            }
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
* Arguments    : cmd-
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
    adc_err_t       err = ADC_SUCCESS;
    adc_sst_t       *p_sample;
    adc_dda_t       *p_charge;
    aregs_t         *p_regs;
    aregs2_t        *p_regs2;

    /* A/D register address */
    if (0 == unit)
    {
        p_regs = (aregs_t *)&S12AD;
    }
    else if (1 == unit)
    {
        p_regs = (aregs_t *)&S12AD1;
    }
    else
    {
        p_regs2 = (aregs2_t *)&S12AD2;
    }

    /* DO UNIVERSAL PARAMETER CHECKING */
#if (ADC_CFG_PARAM_CHECKING_ENABLE == 1)
    if (unit > 2)
    {
        return ADC_ERR_INVALID_ARG;
    }

    /* Pointer check */
    if ((NULL == p_args) || (FIT_NO_PTR == p_args))
    {
        if ((ADC_CMD_SET_DDA_STATE_CNT == cmd)
        || (ADC_CMD_SET_SAMPLE_STATE_CNT == cmd)
        || (ADC_CMD_ENABLE_CHANS == cmd))
        {
            return ADC_ERR_MISSING_PTR;
        }
    }
#endif

    /* PROCESS INDIVIDUAL COMMANDS */
    switch (cmd)
    {
        case ADC_CMD_SET_DDA_STATE_CNT:

            /* Disconnection Detection Assist (DDA) */
            p_charge = (adc_dda_t *)p_args;
            if (2 == unit)
            {
#if (ADC_CFG_PARAM_CHECKING_ENABLE == 1)
                if (ADC_DDA_OFF != p_charge->method)
                {
                    if ((p_charge->num_states < ADC_DDA_STATE_CNT_MIN)
                    || (p_charge->num_states > ADC_DDA_STATE_CNT_MAX)
                    || (0 != p_regs2->ADCER.BIT.DIAGM)           // self-diagnosis enabled
                    || ((p_regs2->ADEXICR.WORD & 0x0200) != 0))  // sensors enabled
                    {
                        return ADC_ERR_ILLEGAL_ARG;
                    }
                }
#endif
                if (ADC_DDA_OFF == p_charge->method)
                {
                    p_regs2->ADDISCR.BYTE = 0;
                }
                else
                {

                    /* NOTE: Using Disconnect Detection Assist adds num_states x (#chans) ADCLKS to scan time */
                    p_regs2->ADDISCR.BYTE = (uint8_t)((ADC_DDA_PRECHARGE == p_charge->method) ? 0x10 : 0);
                    p_regs2->ADDISCR.BYTE |= p_charge->num_states;
                }
            }
            else
            {
#if (ADC_CFG_PARAM_CHECKING_ENABLE == 1)
                if (ADC_DDA_OFF != p_charge->method)
                {
                    if ((p_charge->num_states < ADC_DDA_STATE_CNT_MIN)
                    || (p_charge->num_states > ADC_DDA_STATE_CNT_MAX)
                    || (0 != p_regs->ADCER.BIT.DIAGM))          // self-diagnosis enabled
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
            }
            break;

        case ADC_CMD_SET_SAMPLE_STATE_CNT:

            /* sampling state */
            p_sample = (adc_sst_t *)p_args;
#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
            if ((p_sample->reg_id > ADC_SST_REG_MAX)
            || (p_sample->num_states < ADC_SST_CNT_MIN))
            {
                return ADC_ERR_INVALID_ARG;
            }
#endif
            *(ADC_PRV_GET_SST_ARR(unit))[p_sample->reg_id] = p_sample->num_states;
            break;

        case ADC_CMD_ENABLE_CHANS:

            /* Specifies the A/D conversion channel. */
            err = adc_configure_scan(unit, (adc_ch_cfg_t *)p_args);
            break;

        case ADC_CMD_ENABLE_TRIG:
            if (2 == unit)
            {
                p_regs2->ADCSR.BIT.TRGE = 1;           // enable sync/async triggers
            }
            else
            {
                p_regs->ADCSR.BIT.TRGE = 1;            // enable sync/async triggers
            }
            break;

        case ADC_CMD_DISABLE_TRIG:
            if (2 == unit)
            {
                p_regs2->ADCSR.BIT.TRGE = 0;           // disable sync/async triggers
            }
            else
            {
                p_regs->ADCSR.BIT.TRGE = 0;            // disable sync/async triggers
            }
            break;

        case ADC_CMD_SCAN_NOW:
            if (2 == unit)
            {
                if (0 == p_regs2->ADCSR.BIT.ADST)
                {
                    p_regs2->ADCSR.BIT.ADST = 1;
                }
                else
                {
                    err = ADC_ERR_SCAN_NOT_DONE;
                }
            }
            else
            {
                if (0 == p_regs->ADCSR.BIT.ADST)
                {
                    p_regs->ADCSR.BIT.ADST = 1;
                }
                else
                {
                    err = ADC_ERR_SCAN_NOT_DONE;
                }
            }
            break;

        case ADC_CMD_CHECK_SCAN_DONE:           // default/Group A or Group B
            if (2 == unit)
            {
                if (1 == p_regs2->ADCSR.BIT.ADST)
                {
                    err = ADC_ERR_SCAN_NOT_DONE;
                }
            }
            else
            {
                if (1 == p_regs->ADCSR.BIT.ADST)
                {
                    err = ADC_ERR_SCAN_NOT_DONE;
                }
            }
            break;

        case ADC_CMD_CHECK_SCAN_DONE_GROUPA:
            if (0 == unit)
            {
                if (1 == IR(S12AD,S12ADI))
                {
                    IR(S12AD,S12ADI) = 0;
                }
                else
                {
                    err = ADC_ERR_SCAN_NOT_DONE;
                }
            }
            else if (1 == unit)
            {
                if (1 == IR(S12AD1,S12ADI1))
                {
                    IR(S12AD1,S12ADI1) = 0;
                }
                else
                {
                    err = ADC_ERR_SCAN_NOT_DONE;
                }
            }
            else
            {
                if (1 == IR(S12AD2,S12ADI2))
                {
                    IR(S12AD2,S12ADI2) = 0;
                }
                else
                {
                    err = ADC_ERR_SCAN_NOT_DONE;
                }
            }
            break;

        case ADC_CMD_CHECK_SCAN_DONE_GROUPB:
            if (0 == unit)
            {
                if (1 == IR(S12AD,GBADI))
                {
                    IR(S12AD,GBADI) = 0;
                }
                else
                {
                    err = ADC_ERR_SCAN_NOT_DONE;
                }
            }
            else if (1 == unit)
            {
                if (1 == IR(S12AD1,GBADI1))
                {
                    IR(S12AD1,GBADI1) = 0;
                }
                else
                {
                    err = ADC_ERR_SCAN_NOT_DONE;
                }
            }
            else
            {
                if (1 == IR(S12AD2,GBADI2))
                {
                    IR(S12AD2,GBADI2) = 0;
                }
                else
                {
                    err = ADC_ERR_SCAN_NOT_DONE;
                }
            }
            break;

        case ADC_CMD_CHECK_SCAN_DONE_GROUPC:
            if (0 == unit)
            {
                if (1 == IR(S12AD,GCADI))
                {
                    IR(S12AD,GCADI) = 0;
                }
                else
                {
                    err = ADC_ERR_SCAN_NOT_DONE;
                }
            }
            else if (1 == unit)
            {
                if (1 == IR(S12AD1,GCADI1))
                {
                    IR(S12AD1,GCADI1) = 0;
                }
                else
                {
                    err = ADC_ERR_SCAN_NOT_DONE;
                }
            }
            else
            {
                if (1 == IR(S12AD2,GCADI2))
                {
                    IR(S12AD2,GCADI2) = 0;
                }
                else
                {
                    err = ADC_ERR_SCAN_NOT_DONE;
                }
            }
            break;

        case ADC_CMD_ENABLE_INT:
            if (2 == unit)
            {
                p_regs2->ADCSR.BIT.ADIE = 1;           // enable S12ADI0 interrupt
            }
            else
            {
                p_regs->ADCSR.BIT.ADIE = 1;            // enable S12ADI0 interrupt
            }
            break;

        case ADC_CMD_DISABLE_INT:
            if (2 == unit)
            {
                p_regs2->ADCSR.BIT.ADIE = 0;           // disable S12ADI0 interrupt
            }
            else
            {
                p_regs->ADCSR.BIT.ADIE = 0;            // disable S12ADI0 interrupt
            }
            break;

        case ADC_CMD_ENABLE_INT_GROUPB:
            if (2 == unit)
            {
                p_regs2->ADCSR.BIT.GBADIE = 1;         // enable GBADI interrupt
            }
            else
            {
                p_regs->ADCSR.BIT.GBADIE = 1;          // enable GBADI interrupt
            }
            break;

        case ADC_CMD_DISABLE_INT_GROUPB:
            if (2 == unit)
            {
                p_regs2->ADCSR.BIT.GBADIE = 0;         // disable GBADI interrupt
            }
            else
            {
                p_regs->ADCSR.BIT.GBADIE = 0;          // disable GBADI interrupt
            }
            break;

        case ADC_CMD_ENABLE_INT_GROUPC:
            if (2 == unit)
            {
                p_regs2->ADGCTRGR.BIT.GCADIE = 1;      // enable GCADI interrupt
            }
            else
            {
                p_regs->ADGCTRGR.BIT.GCADIE = 1;       // enable GCADI interrupt
            }
            break;

        case ADC_CMD_DISABLE_INT_GROUPC:
            if (2 == unit)
            {
                p_regs2->ADGCTRGR.BIT.GCADIE = 0;      // disable GCADI interrupt
            }
            else
            {
                p_regs->ADGCTRGR.BIT.GCADIE = 0;       // disable GCADI interrupt
            }
            break;

        default:
            err = ADC_ERR_INVALID_ARG;
            break;
    }

    return err;
} /* End of function adc_control() */


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
    uint16_t    i=0;
    uint32_t    tmp_mask=0;

    aregs_t     *p_regs;
    aregs2_t     *p_regs2;
    uint16_t    trig_a;
    uint16_t    trig_b;
    uint16_t    trig_c;
    uint16_t    tmp_adpgacr;
    uint16_t    tmp_adpgags0;

    /* A/D register address */
    if (0 == unit)
    {
        p_regs = (aregs_t *)&S12AD;
    }
    else if (1 == unit)
    {
        p_regs = (aregs_t *)&S12AD1;
    }
    else
    {
        p_regs2 = (aregs2_t *)&S12AD2;
    }
#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
    adc_err_t   err;

    err = adc_check_scan_config(unit, p_config);
    if (ADC_SUCCESS != err)
    {
        return err;
    }

#endif /* parameter checking */

    if (2 == unit)
    {
        /* SET MASKS FOR CHANNELS */
        p_regs2->ADANSA0.WORD = (uint16_t) (p_config->chan_mask & 0x0FFF);

        /* SET MASKS FOR CHANNELS */
        p_regs2->ADANSB0.WORD = (uint16_t) (p_config->chan_mask_groupb & 0x0FFF);

        /* SET MASKS FOR CHANNELS */
        p_regs2->ADANSC0.WORD = (uint16_t) (p_config->chan_mask_groupc & 0x0FFF);

        /* SET MASKS FOR CHANNELS */
        p_regs2->ADADS0.WORD = (uint16_t) (p_config->add_mask & 0x0FFF);

        adc_configure_sensors(p_config);

        /* Operation stabilization wait time(5us) */
        R_BSP_SoftwareDelay((uint32_t)(5+1), BSP_DELAY_MICROSECS);

        /* SET GROUP A PRIORITY ACTION (not interrupt priority!) */

        if (ADC_GRPA_PRIORITY_OFF != p_config->priority_groupa)
        {
            trig_a = p_regs2->ADSTRGR.BIT.TRSA;              // save TRSA
            trig_b = p_regs2->ADSTRGR.BIT.TRSB;              // save TRSB
            trig_c = p_regs2->ADGCTRGR.BIT.TRSC;             // save TRSC
            p_regs2->ADSTRGR.BIT.TRSA = ADC_TRIG_NONE;
            p_regs2->ADSTRGR.BIT.TRSB = ADC_TRIG_NONE;
            p_regs2->ADGCTRGR.BIT.TRSC = ADC_TRIG_NONE;
            if (ADC_TRIG_NONE != p_regs2->ADSTRGR.BIT.TRSA)  // bus wait for ADSTRGR write
            {
                R_BSP_NOP();
            }
            if (ADC_TRIG_NONE != p_regs2->ADSTRGR.BIT.TRSB)  // bus wait for ADSTRGR write
            {
                R_BSP_NOP();
            }
            if (ADC_TRIG_NONE != p_regs2->ADGCTRGR.BIT.TRSC) // bus wait for ADGCTRGR write
            {
                R_BSP_NOP();
            }

            p_regs2->ADGSPCR.WORD = p_config->priority_groupa;

            p_regs2->ADSTRGR.BIT.TRSA = trig_a;              // restore TRSA

            if (0 == p_regs2->ADGSPCR.BIT.GBRP)
            {
                p_regs2->ADSTRGR.BIT.TRSB = trig_b;          // restore TRSB
                p_regs2->ADGCTRGR.BIT.TRSC = trig_c;         // restore TRSC
            }
            else
            {
                if (0 == p_regs2->ADGCTRGR.BIT.GRCE)
                {
                    p_regs2->ADGCTRGR.BIT.TRSC = trig_c;     // restore TRSC
                }
                else
                {
                    p_regs2->ADSTRGR.BIT.TRSB = trig_b;      // restore TRSB
                }
            }
        }
        else
        {
            p_regs2->ADGSPCR.WORD = p_config->priority_groupa;
        }

        /* SET SELF DIAGNOSIS REGISTERS (VIRTUAL CHANNEL) */
        if (ADC_DIAG_OFF == p_config->diag_method)
        {
            p_regs2->ADCER.BIT.DIAGM = 0;
        }
        else
        {

            /* NOTE: Using Self Diagnosis adds 11/13/15(resolution) + (ch0 SST) ADCLKS to scan time. */
            /* (ch0 can still be used with self diagnosis on) */
            if (ADC_DIAG_ROTATE_VOLTS == p_config->diag_method)
            {
                p_regs2->ADCER.BIT.DIAGLD = 0;
                p_regs2->ADCER.BIT.DIAGVAL = 1;
            }
            else
            {
                p_regs2->ADCER.BIT.DIAGLD = 1;

                /* 2-bit value */
                p_regs2->ADCER.BIT.DIAGVAL = (uint8_t)(p_config->diag_method & 0x3);
            }
            p_regs2->ADCER.BIT.DIAGM = 1;
        }

        /* SET DOUBLE TRIGGER CHANNEL */
        if (1 == p_regs2->ADCSR.BIT.DBLE)
        {
            tmp_mask = p_config->chan_mask;         /* tmp_mask is non-Group/Group A chans */

            /* WAIT_LOOP */
            while (tmp_mask >>= 1)                  /* determine bit/ch number @suppress("While statement braces") */
            {
                i++;
            }
            p_regs2->ADCSR.BIT.DBLANS = i;
        }

        /* ENABLE INTERRUPTS */
        adc_enable_s12adi(unit);
        if (ADC_ADCS_GROUP_SCAN == p_regs2->ADCSR.BIT.ADCS)
        {
            adc_enable_s12gbadi(unit);
            if (1 == p_regs2->ADGCTRGR.BIT.GRCE)
            {
                adc_enable_s12gcadi(unit);
            }
        }
    }
    else
    {
        /* SET MASKS FOR CHANNELS */
        p_regs->ADANSA0.WORD = (uint16_t) (p_config->chan_mask & 0x000F);
        p_regs->ADANSA1.WORD = (uint16_t) ((p_config->chan_mask >> 16) & 0x0001);

        /* SET MASKS FOR CHANNELS */
        p_regs->ADANSB0.WORD = (uint16_t) (p_config->chan_mask_groupb & 0x000F);
        p_regs->ADANSB1.WORD = (uint16_t) ((p_config->chan_mask_groupb >> 16) & 0x0001);

        /* SET MASKS FOR CHANNELS */
        p_regs->ADANSC0.WORD = (uint16_t) (p_config->chan_mask_groupc & 0x000F);
        p_regs->ADANSC1.WORD = (uint16_t) ((p_config->chan_mask_groupc >> 16) & 0x0001);

        /* SET MASKS FOR CHANNELS */
        p_regs->ADADS0.WORD = (uint16_t) (p_config->add_mask & 0x000F);
        p_regs->ADADS1.WORD = (uint16_t) ((p_config->add_mask >> 16) & 0x0001);

        adc_configure_sensors(p_config);

        if (1 == unit)
        {
            /* NOTE: S&H adds to scan time because normal state machine still runs. */
            S12AD1.ADSHCR.BIT.SHANS = p_config->sample_hold_mask;

            /* adds 12 + sample_hold_states ADCLKS to scan time */
            S12AD1.ADSHCR.BIT.SSTSH = p_config->sample_hold_states;
        }

        /* Programmable Gain Amplifier setting */
        tmp_adpgacr = 0x9999;
        tmp_adpgacr |= p_config->pga_amp[0];
        if (1 == unit)
        {
            tmp_adpgacr |= (p_config->pga_amp[1] << 4);
            tmp_adpgacr |= (p_config->pga_amp[2] << 8);
        }
        p_regs->ADPGACR.WORD = tmp_adpgacr;

        /* Operation stabilization wait time(5us) */
        R_BSP_SoftwareDelay((uint32_t)(5+1), BSP_DELAY_MICROSECS);

        tmp_adpgags0 = 0x0000;
        tmp_adpgags0 |= (p_config->pga_gain[0]);
        if (1 == unit)
        {
            tmp_adpgags0 |= (p_config->pga_gain[1] << 4);
            tmp_adpgags0 |= (p_config->pga_gain[2] << 8);
        }
        p_regs->ADPGAGS0.WORD = tmp_adpgags0;

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

            p_regs->ADSTRGR.BIT.TRSA = trig_a;              // restore TRSA

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

                /* 2-bit value */
                p_regs->ADCER.BIT.DIAGVAL = (uint8_t)(p_config->diag_method & 0x3);
            }
            p_regs->ADCER.BIT.DIAGM = 1;
        }

        /* SET DOUBLE TRIGGER CHANNEL */
        if (1 == p_regs->ADCSR.BIT.DBLE)
        {
            tmp_mask = p_config->chan_mask;         /* tmp_mask is non-Group/Group A chans */

            /* WAIT_LOOP */
            while (tmp_mask >>= 1)                  /* determine bit/ch number @suppress("While statement braces") */
            {
                i++;
            }
            p_regs->ADCSR.BIT.DBLANS = i;
        }

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
    uint8_t     i;
    uint32_t    tmp_mask=0;
    uint32_t    a_mask;
    uint32_t    b_mask;
    uint32_t    c_mask;

    aregs_t     *p_regs;

    /* A/D register address */
    if (0 == unit)
    {
        p_regs = (aregs_t *)&S12AD;
    }
    else if (1 == unit)
    {
        p_regs = (aregs_t *)&S12AD1;
    }
    else
    {
        p_regs = (aregs_t *)&S12AD2;
    }

    /* unit == 0 */
    INTERNAL_NOT_USED(unit);

    /* Verify at least one bonded channel is selected */
    if ((0 == p_config->chan_mask)
    || ((p_config->chan_mask & ADC_PRV_INVALID_CH_MASK(unit)) != 0))
    {
        return ADC_ERR_INVALID_ARG;
    }

    /* ADC_MASK_VOLT is groupA only. */
    if (ADC_MODE_SS_INT_REF_VOLT == g_dcb[unit].mode)
    {

        /* VOLT selection confirmation */
        if (0 == (p_config->chan_mask & ADC_MASK_VOLT))
        {
            return ADC_ERR_INVALID_ARG;
        }

        /* When A/D conversion of the internal reference voltage is to be performed, set all the bits in the ADANSA0
           and ADANSA1 and ADANSB0 and ADANSB1 and ADANSC0 and ADANSC1 registers and the ADCSR.DBLE bit should be set
            to all 0 in single scan mode. */
        if ((0 != (p_config->chan_mask & 0x00000FFF))          /* ADANSA0 != 0 and ADANSA1 != 0*/
        || (0 != (p_config->chan_mask_groupb & 0x00000FFF))    /* ADANSB0 != 0 and ADANSB1 != 0 */
        || (0 != (p_config->chan_mask_groupc & 0x00000FFF))    /* ADANSC0 != 0 and ADANSC1 != 0 */
        || (1 == p_regs->ADCSR.BIT.DBLE)        // double trigger mode
        || (2 != unit))                         // cannnot use unit0 and unit1
        {
            return ADC_ERR_INVALID_ARG;
        }

        /* Single scan mode only. */
        if (0 != p_regs->ADCSR.BIT.ADCS)
        {
            return ADC_ERR_ILLEGAL_ARG;

        }
    }

    /* Verify at least one unique bonded channel is selected for Group B
     * and GroupA priority setting is valid.
     */
    if (ADC_ADCS_GROUP_SCAN == p_regs->ADCSR.BIT.ADCS)
    {
        if ((0 == p_config->chan_mask_groupb)
        || ((p_config->chan_mask_groupb & ADC_PRV_INVALID_CH_MASK(unit)) != 0)
        || ((p_config->chan_mask_groupb & ADC_MASK_VOLT) != 0))
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

        if (1 == p_regs->ADGCTRGR.BIT.GRCE)     // for Group C
        {
            if ((0 == p_config->chan_mask_groupc)
            || ((p_config->chan_mask_groupc & ADC_PRV_INVALID_CH_MASK(unit)) != 0)
            || ((p_config->chan_mask_groupc & ADC_MASK_VOLT) != 0))
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

    if ((ADC_MODE_SS_ONE_CH == g_dcb[unit].mode)
    || (ADC_MODE_CONT_ONE_CH == g_dcb[unit].mode)
    || (1 == p_regs->ADCSR.BIT.DBLE))           // double trigger mode
    {
        tmp_mask = p_config->chan_mask;         // tmp_mask is non-Group/Group A chans

        /* Bit-AND with 2s-complement (see note in function header) */
        if ((tmp_mask & ((~tmp_mask) + 1)) != tmp_mask)
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
    }

    if (0 != p_config->sample_hold_mask)
    {
        if ((0 == unit) || (2 == unit))         // S&H only on unit 1
        {
            return ADC_ERR_ILLEGAL_ARG;
        }

        /* S&H chans can only be 1 and must have at least minimum state count specified */
        if ((p_config->sample_hold_mask > 0x7)
        || (p_config->sample_hold_states < ADC_SST_SH_CNT_MIN))
        {
            return ADC_ERR_INVALID_ARG;
        }

        /* S&H channels cannot not be split across groups */
        a_mask = p_config->sample_hold_mask & p_config->chan_mask;
        b_mask = p_config->sample_hold_mask & p_config->chan_mask_groupb;
        c_mask = p_config->sample_hold_mask & p_config->chan_mask_groupc;

        /*  */
        if (ADC_ADCS_GROUP_SCAN == p_regs->ADCSR.BIT.ADCS)
        {
            if ((a_mask != p_config->sample_hold_mask) && (b_mask != p_config->sample_hold_mask))
            {
                return ADC_ERR_ILLEGAL_ARG;
            }
            if (1 == p_regs->ADGCTRGR.BIT.GRCE)
            {
                if ((a_mask != p_config->sample_hold_mask)
                && (b_mask != p_config->sample_hold_mask)
                && (c_mask != p_config->sample_hold_mask))
                {
                    return ADC_ERR_ILLEGAL_ARG;
                }
            }
        }
        else if (a_mask != p_config->sample_hold_mask)
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
        else
        {
            ;
        }

        /* S&H channels cannot be a double trigger channel (can be in group B) */
        if ((1 == p_regs->ADCSR.BIT.DBLE) && (0 != a_mask))
        {
            return ADC_ERR_ILLEGAL_ARG;
        }

        /* S&H channels cannot be in GroupB if GroupA priority enabled */ 
        if (((0 != b_mask) || (0 != c_mask)) && (ADC_GRPA_PRIORITY_OFF != p_config->priority_groupa))
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
    }

    /* If sensors specified, verify in legal configuration */
    if (p_config->chan_mask & ADC_MASK_SENSORS)
    {
        if ((1 == p_regs->ADCSR.BIT.DBLE)         // double trigger mode
        || (0 == unit) || (1 == unit))            // only unit2 has sensors
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
    }
    

    if ((ADC_DIAG_OFF != p_config->diag_method)
    &&   ((0 != p_regs->ADADC.BIT.ADC)            /* addition */
    || (1 == p_regs->ADCSR.BIT.DBLE)              /* double trigger mode */
    ))
    {
        return ADC_ERR_ILLEGAL_ARG;
    }

    if (0 != (p_regs->ADDISCR.BYTE & 0x0F))
    {
        if (0 != p_config->sample_hold_mask)
        {
            return ADC_ERR_ILLEGAL_ARG;
        }

        for (i = 0 ; i < ADC_PGA_CHANNEL_MAX ; i++)
        {
            if (ADC_PGA_NOT_USE != p_config->pga_amp[i])
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

    /* INTERNAL REFERENCE VOLTAGE SENSOR */
    if (ADC_MODE_SS_INT_REF_VOLT == g_dcb[2].mode)
    {

        /* A/D conversion of internal reference voltage is performed. */
        S12AD2.ADEXICR.BIT.OCSA = 1;
    }
    else
    {

        /* A/D conversion of internal reference voltage is not performed. */
        S12AD2.ADEXICR.BIT.OCSA = 0;
    }

    /* Internal Reference Voltage A/D-Converted Value Addition/Average Mode Select */
    S12AD2.ADEXICR.BIT.OCSAD = (p_config->add_mask & ADC_MASK_VOLT) ? 1 : 0;

    return;
} /* End of function adc_configure_sensors() */


/******************************************************************************
* Function Name: adc_enable_s12adi
* Description  : This function clears the S12ADI interrupt flag and enables
*                interrupts in the peripheral (for IR flag usage). If priority
*                is not 0, interrupts are enabled in the ICU.
*                NOTE: This has nothing to do with enabling triggers.
* Arguments    : unit -
*                   0
* Return Value : none
*******************************************************************************/
static void adc_enable_s12adi(uint8_t unit)
{

    if (0 == unit)
    {
        IR(S12AD,S12ADI) = 0;               // clear flag
        S12AD.ADCSR.BIT.ADIE = 1;           // enable in peripheral
        if (0 != IPR(S12AD,S12ADI))
        {
            R_BSP_InterruptRequestEnable(VECT(S12AD,S12ADI));        // enable in ICU
        }
    }
    else if (1 == unit) /* Unit1 */
    {
        IR(S12AD1,S12ADI1) = 0;             // clear flag
        S12AD1.ADCSR.BIT.ADIE = 1;          // enable in peripheral
        if (0 != IPR(S12AD1,S12ADI1))
        {
            R_BSP_InterruptRequestEnable(VECT(S12AD1,S12ADI1));      // enable in ICU
        }
    }
    else /* Unit2 */
    {
        IR(S12AD2,S12ADI2) = 0;             // clear flag
        S12AD2.ADCSR.BIT.ADIE = 1;          // enable in peripheral
        if (0 != IPR(S12AD2,S12ADI2))
        {
            R_BSP_InterruptRequestEnable(VECT(S12AD2,S12ADI2));      // enable in ICU
        }
    }

} /* End of function adc_enable_s12adi() */


/******************************************************************************
* Function Name: adc_enable_s12gbadi
* Description  : This function clears the S12GBADI interrupt flag and enables
*                interrupts in the peripheral (for IR flag usage). If priority
*                is not 0, interrupts are enabled in the ICU.
*                NOTE: This has nothing to do with enabling triggers.
* Arguments    : unit -
*                   0
* Return Value : none
*******************************************************************************/
static void adc_enable_s12gbadi(uint8_t unit)
{

    if (0 == unit)
    {
        IR(S12AD,GBADI) = 0;                // clear flag
        S12AD.ADCSR.BIT.GBADIE = 1;         // enable in peripheral
        if (0 != IPR(S12AD,GBADI))
        {
            R_BSP_InterruptRequestEnable(VECT(S12AD,GBADI));         // enable in ICU
        }
    }
    else if (1 == unit) /* Unit1 */
    {
        IR(S12AD1,GBADI1) = 0;              // clear flag
        S12AD1.ADCSR.BIT.GBADIE = 1;        // enable in peripheral
        if (0 != IPR(S12AD1,GBADI1))
        {
            R_BSP_InterruptRequestEnable(VECT(S12AD1,GBADI1));       // enable in ICU
        }
    }
    else /* Unit2 */
    {
        IR(S12AD2,GBADI2) = 0;              // clear flag
        S12AD2.ADCSR.BIT.GBADIE = 1;        // enable in peripheral
        if (0 != IPR(S12AD2,GBADI2))
        {
            R_BSP_InterruptRequestEnable(VECT(S12AD2,GBADI2));       // enable in ICU
        }
    }

} /* End of function adc_enable_s12gbadi() */


/******************************************************************************
* Function Name: adc_enable_s12gcadi
* Description  : This function clears the S12GCADI interrupt flag and enables
*                interrupts in the peripheral (for IR flag usage). If priority
*                is not 0, interrupts are enabled in the ICU.
*                NOTE: This has nothing to do with enabling triggers.
* Arguments    : unit -
*                   0
* Return Value : none
*******************************************************************************/
static void adc_enable_s12gcadi(uint8_t unit)
{
    if (0 == unit)
    {
        IR(S12AD,GCADI) = 0;                // clear flag
        S12AD.ADGCTRGR.BIT.GCADIE = 1;      // enable in peripheral
        if (0 != IPR(S12AD,GCADI))
        {
            R_BSP_InterruptRequestEnable(VECT(S12AD,GCADI));         // enable in ICU
        }
    }
    else if (1 == unit) /* Unit1 */
    {
        IR(S12AD1,GCADI1) = 0;              // clear flag
        S12AD1.ADGCTRGR.BIT.GCADIE = 1;     // enable in peripheral
        if (0 != IPR(S12AD1,GCADI1))
        {
            R_BSP_InterruptRequestEnable(VECT(S12AD1,GCADI1));       // enable in ICU
        }
    }
    else /* Unit2 */
    {
        IR(S12AD2,GCADI2) = 0;              // clear flag
        S12AD2.ADGCTRGR.BIT.GCADIE = 1;     // enable in peripheral
        if (0 != IPR(S12AD2,GCADI2))
        {
            R_BSP_InterruptRequestEnable(VECT(S12AD2,GCADI2));       // enable in ICU
        }
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
    aregs_t      *p_regs;
    aregs2_t     *p_regs2;
    volatile    uint8_t i;

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif

    /* A/D register address */
    if (0 == unit)
    {
        p_regs = (aregs_t *)&S12AD;
    }
    else if (1 == unit)
    {
        p_regs = (aregs_t *)&S12AD1;
    }
    else
    {
        p_regs2 = (aregs2_t *)&S12AD2;
    }

#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
    if (unit > 2)
    {
        return ADC_ERR_INVALID_ARG;
    }
#endif

    /* Stop triggers & conversions, and disable peripheral interrupts */
    if (0 == unit)
    {
        R_BSP_InterruptRequestDisable(VECT(S12AD,S12ADI));      // disable interrupts in ICU
        R_BSP_InterruptRequestDisable(VECT(S12AD,GBADI));       // disable interrupts in ICU
        R_BSP_InterruptRequestDisable(VECT(S12AD,GCADI));       // disable interrupts in ICU

        IPR(S12AD,S12ADI) = 0;
        IPR(S12AD,GBADI) = 0;
        IPR(S12AD,GCADI) = 0;
    }
    else if (1 == unit) /* Unit1 */
    {
        R_BSP_InterruptRequestDisable(VECT(S12AD1,S12ADI1));    // disable interrupts in ICU
        R_BSP_InterruptRequestDisable(VECT(S12AD1,GBADI1));     // disable interrupts in ICU
        R_BSP_InterruptRequestDisable(VECT(S12AD1,GCADI1));     // disable interrupts in ICU

        IPR(S12AD1,S12ADI1) = 0;
        IPR(S12AD1,GBADI1) = 0;
        IPR(S12AD1,GCADI1) = 0;
    }
    else /* Unit2 */
    {
        R_BSP_InterruptRequestDisable(VECT(S12AD2,S12ADI2));    // disable interrupts in ICU
        R_BSP_InterruptRequestDisable(VECT(S12AD2,GBADI2));     // disable interrupts in ICU
        R_BSP_InterruptRequestDisable(VECT(S12AD2,GCADI2));     // disable interrupts in ICU

        IPR(S12AD2,S12ADI2) = 0;
        IPR(S12AD2,GBADI2) = 0;
        IPR(S12AD2,GCADI2) = 0;
    }

    if (2 == unit)
    {
        if (1 == p_regs2->ADGSPCR.BIT.PGS)
        {
            p_regs2->ADGSPCR.BIT.PGS = 0;
        }

        if (ADC_ADCS_GROUP_SCAN == p_regs2->ADCSR.BIT.ADCS)
        {
            p_regs2->ADSTRGR.WORD = 0x3F3F;
            if (1 == p_regs2->ADGCTRGR.BIT.GRCE)
            {
                p_regs2->ADGCTRGR.BIT.TRSC = 0x3F;
                p_regs2->ADGCTRGR.BIT.GCADIE = 0;
            }
        }
        else
        {
            p_regs2->ADSTRGR.BIT.TRSA = 0x3F;
        }

        p_regs2->ADCSR.WORD = 0;
        if (0 != p_regs2->ADCSR.WORD)
        {

            /* Dommy read and compare */
            R_BSP_NOP();
        }
    }
    else
    {
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
            }
        }
        else
        {
            p_regs->ADSTRGR.BIT.TRSA = 0x3F;
        }

        p_regs->ADCSR.WORD = 0;
        if (0 != p_regs->ADCSR.WORD)
        {

            /* Dommy read and compare */
            R_BSP_NOP();
        }
    }

    /* Wait for three clock cycles of ADCLK before entering the module stop mode */
    /* WAIT_LOOP */
    for (i = 0; i < 192; i++)
    {
        R_BSP_NOP();
    }

    if (0 == unit)
    {
        IR(S12AD,S12ADI) = 0;         // clear interrupt flag
        IR(S12AD,GBADI) = 0;          // clear interrupt flag
        IR(S12AD,GCADI) = 0;          // clear interrupt flag
    }
    else if (1 == unit) /* Unit1 */
    {
        IR(S12AD1,S12ADI1) = 0;       // clear interrupt flag
        IR(S12AD1,GBADI1) = 0;        // clear interrupt flag
        IR(S12AD1,GCADI1) = 0;        // clear interrupt flag
    }
    else /* Unit2 */
    {
        IR(S12AD2,S12ADI2) = 0;       // clear interrupt flag
        IR(S12AD2,GBADI2) = 0;        // clear interrupt flag
        IR(S12AD2,GCADI2) = 0;        // clear interrupt flag
    }

    /* Power down peripheral */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    /* S12A module clock is enabled */
    if (0 == unit)
    {
        MSTP(S12AD) = 1;
    }
    else if (1 == unit)
    {
        MSTP(S12AD1) = 1;
    }
    else
    {
        MSTP(S12AD2) = 1;
    }
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Show driver as closed */
    g_dcb[unit].opened = false;

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

    /* Read A/D conversion result of unit 0 */
    p_all_data->unit0.chan[ADC_REG_CH0]  = S12AD.ADDR0;
    p_all_data->unit0.chan[ADC_REG_CH1]  = S12AD.ADDR1;
    p_all_data->unit0.chan[ADC_REG_CH2]  = S12AD.ADDR2;
    p_all_data->unit0.chan[ADC_REG_CH3]  = S12AD.ADDR3;
    p_all_data->unit0.chan[ADC_REG_CH4_CH16] = S12AD.ADDR16;
    p_all_data->unit0.dbltrig            = S12AD.ADDBLDR;
    p_all_data->unit0.dbltrigA           = S12AD.ADDBLDRA;
    p_all_data->unit0.dbltrigB           = S12AD.ADDBLDRB;
    p_all_data->unit0.self_diag          = S12AD.ADRD;

    /* Read A/D conversion result of unit 1 */
    p_all_data->unit1.chan[ADC_REG_CH0]  = S12AD1.ADDR0;
    p_all_data->unit1.chan[ADC_REG_CH1]  = S12AD1.ADDR1;
    p_all_data->unit1.chan[ADC_REG_CH2]  = S12AD1.ADDR2;
    p_all_data->unit1.chan[ADC_REG_CH3]  = S12AD1.ADDR3;
    p_all_data->unit1.chan[ADC_REG_CH4_CH16] = S12AD1.ADDR16;
    p_all_data->unit1.dbltrig            = S12AD1.ADDBLDR;
    p_all_data->unit1.dbltrigA           = S12AD1.ADDBLDRA;
    p_all_data->unit1.dbltrigB           = S12AD1.ADDBLDRB;
    p_all_data->unit1.self_diag          = S12AD1.ADRD;

    /* Read A/D conversion result of unit 2 */
    p_all_data->unit2.chan[ADC_REG_CH0]  = S12AD2.ADDR0;
    p_all_data->unit2.chan[ADC_REG_CH1]  = S12AD2.ADDR1;
    p_all_data->unit2.chan[ADC_REG_CH2]  = S12AD2.ADDR2;
    p_all_data->unit2.chan[ADC_REG_CH3]  = S12AD2.ADDR3;
    p_all_data->unit2.chan[ADC_REG_CH4_CH16]  = S12AD2.ADDR4;
    p_all_data->unit2.chan[ADC_REG_CH5]  = S12AD2.ADDR5;
    p_all_data->unit2.chan[ADC_REG_CH6]  = S12AD2.ADDR6;
    p_all_data->unit2.chan[ADC_REG_CH7]  = S12AD2.ADDR7;
    p_all_data->unit2.chan[ADC_REG_CH8]  = S12AD2.ADDR8;
    p_all_data->unit2.chan[ADC_REG_CH9]  = S12AD2.ADDR9;
    p_all_data->unit2.chan[ADC_REG_CH10] = S12AD2.ADDR10;
    p_all_data->unit2.chan[ADC_REG_CH11] = S12AD2.ADDR11;
    p_all_data->unit2.volt               = S12AD2.ADOCDR;
    p_all_data->unit2.dbltrig            = S12AD2.ADDBLDR;
    p_all_data->unit2.dbltrigA           = S12AD2.ADDBLDRA;
    p_all_data->unit2.dbltrigB           = S12AD2.ADDBLDRB;
    p_all_data->unit2.self_diag          = S12AD2.ADRD;

    return ADC_SUCCESS;
} /* End of function adc_read_all() */

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

    if ((NULL != g_dcb[0].p_callback) && (FIT_NO_FUNC != g_dcb[0].p_callback))
    {
        args.event = ADC_EVT_SCAN_COMPLETE;
        args.unit = 0;
        g_dcb[0].p_callback(&args);
    }
} /* End of function adc_s12adi0_isr() */


/*****************************************************************************
* Function Name: adc_s12adi1_isr
* Description  : Unit1 interrupt handler for normal/Group A/double trigger
*                scan complete.
* Arguments    : none
* Return Value : none
******************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void adc_s12adi1_isr(void)
{
    adc_cb_args_t   args;

    if ((NULL != g_dcb[1].p_callback) && (FIT_NO_FUNC != g_dcb[1].p_callback))
    {
        args.event = ADC_EVT_SCAN_COMPLETE;
        args.unit = 1;
        g_dcb[1].p_callback(&args);
    }
} /* End of function adc_s12adi1_isr() */


/*****************************************************************************
* Function Name: adc_s12adi2_isr
* Description  : Unit2 interrupt handler for normal/Group A/double trigger
*                scan complete.
* Arguments    : none
* Return Value : none
******************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void adc_s12adi2_isr(void)
{
    adc_cb_args_t   args;

    if ((NULL != g_dcb[2].p_callback) && (FIT_NO_FUNC != g_dcb[2].p_callback))
    {
        args.event = ADC_EVT_SCAN_COMPLETE;
        args.unit = 2;
        g_dcb[2].p_callback(&args);
    }
} /* End of function adc_s12adi2_isr() */


/*****************************************************************************
* Function Name: adc_s12gbadi0_isr
* Description  : Unit0 interrupt handler for Group B scan complete.
* Arguments    : none
* Return Value : none
******************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void adc_s12gbadi0_isr(void)
{
    adc_cb_args_t   args;

    if ((NULL != g_dcb[0].p_callback) && (FIT_NO_FUNC != g_dcb[0].p_callback))
    {
        args.event = ADC_EVT_SCAN_COMPLETE_GROUPB;
        args.unit = 0;
        g_dcb[0].p_callback(&args);
    }
} /* End of function adc_s12gbadi0_isr() */


/*****************************************************************************
* Function Name: adc_s12gbadi1_isr
* Description  : Unit1 interrupt handler for Group B scan complete.
* Arguments    : none
* Return Value : none
******************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void adc_s12gbadi1_isr(void)
{
    adc_cb_args_t   args;

    if ((NULL != g_dcb[1].p_callback) && (FIT_NO_FUNC != g_dcb[1].p_callback))
    {
        args.event = ADC_EVT_SCAN_COMPLETE_GROUPB;
        args.unit = 1;
        g_dcb[1].p_callback(&args);
    }
} /* End of function adc_s12gbadi1_isr() */


/*****************************************************************************
* Function Name: adc_s12gbadi2_isr
* Description  : Unit2 interrupt handler for Group B scan complete.
* Arguments    : none
* Return Value : none
******************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void adc_s12gbadi2_isr(void)
{
    adc_cb_args_t   args;

    if ((NULL != g_dcb[2].p_callback) && (FIT_NO_FUNC != g_dcb[2].p_callback))
    {
        args.event = ADC_EVT_SCAN_COMPLETE_GROUPB;
        args.unit = 2;
        g_dcb[2].p_callback(&args);
    }
} /* End of function adc_s12gbadi2_isr() */


/*****************************************************************************
* Function Name: adc_s12gcadi0_isr
* Description  : Unit0 interrupt handler for Group C scan complete.
* Arguments    : none
* Return Value : none
******************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void adc_s12gcadi0_isr(void)
{
    adc_cb_args_t   args;

    if ((NULL != g_dcb[0].p_callback) && (FIT_NO_FUNC != g_dcb[0].p_callback))
    {
        args.event = ADC_EVT_SCAN_COMPLETE_GROUPC;
        args.unit = 0;
        g_dcb[0].p_callback(&args);
    }
} /* End of function adc_s12gcadi0_isr() */


/*****************************************************************************
* Function Name: adc_s12gcadi1_isr
* Description  : Unit1 interrupt handler for Group C scan complete.
* Arguments    : none
* Return Value : none
******************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void adc_s12gcadi1_isr(void)
{
    adc_cb_args_t   args;

    if ((NULL != g_dcb[1].p_callback) && (FIT_NO_FUNC != g_dcb[1].p_callback))
    {
        args.event = ADC_EVT_SCAN_COMPLETE_GROUPC;
        args.unit = 1;
        g_dcb[1].p_callback(&args);
    }
} /* End of function adc_s12gcadi1_isr() */


/*****************************************************************************
* Function Name: adc_s12gcadi2_isr
* Description  : Unit2 interrupt handler for Group C scan complete.
* Arguments    : none
* Return Value : none
******************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void adc_s12gcadi2_isr(void)
{
    adc_cb_args_t   args;

    if ((NULL != g_dcb[2].p_callback) && (FIT_NO_FUNC != g_dcb[2].p_callback))
    {
        args.event = ADC_EVT_SCAN_COMPLETE_GROUPC;
        args.unit = 2;
        g_dcb[2].p_callback(&args);
    }
} /* End of function adc_s12gcadi2_isr() */

#endif /* #if defined BSP_MCU_RX24U */

