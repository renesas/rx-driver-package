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
* File Name    : r_s12ad_rx23t.c
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

#if defined (BSP_MCU_RX23T)
#include "r_s12ad_rx_config.h"
#include "r_s12ad_rx_private.h"
#include "r_s12ad_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef R_BSP_VOLATILE_EVENACCESS struct st_s12ad  aregs_t;

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/* In RAM */
extern adc_ctrl_t g_dcb;

/* In ROM. A/D Data Register pointers */
volatile uint16_t R_BSP_EVENACCESS_SFR * const  gp_dreg_ptrs[ADC_REG_MAX+1] =
                    {   &S12AD.ADDR0,     /* channel 0 */
                        &S12AD.ADDR1,     /* channel 1 */
                        &S12AD.ADDR2,     /* channel 2 */
                        &S12AD.ADDR3,     /* channel 3 */
                        &S12AD.ADDR4,     /* channel 4 */
                        &S12AD.ADDR5,     /* channel 5 */
                        &S12AD.ADDR6,     /* channel 6 */
                        &S12AD.ADDR7,     /* channel 7 */
                        &S12AD.ADDR16,    /* channel 16 */
                        &S12AD.ADDR17,    /* channel 17 */
                        &S12AD.ADOCDR,    /* voltage sensor */
                        &S12AD.ADDBLDR,   /* Data Duplication register */
                        &S12AD.ADDBLDRA,  /* Data Duplication register A */
                        &S12AD.ADDBLDRB,  /* Data Duplication register B */
                        &S12AD.ADRD       /* self-diagnosis register */
                    };


/* In ROM. Sample State (SST) Register pointers */
/* 8-bit register pointers */
volatile uint8_t R_BSP_EVENACCESS_SFR * const  gp_sreg_ptrs[ADC_SST_REG_MAX+1] =
                    {   &S12AD.ADSSTR0,     /* channel 0 */
                        &S12AD.ADSSTR1,     /* channel 1 */
                        &S12AD.ADSSTR2,     /* channel 2 */
                        &S12AD.ADSSTR3,
                        &S12AD.ADSSTR4,
                        &S12AD.ADSSTR5,
                        &S12AD.ADSSTR6,
                        &S12AD.ADSSTR7,     /* channel 7 */
                        &S12AD.ADSSTRL,     /* channels 16 to 17 */
                        &S12AD.ADSSTRO      /* internal voltage reference */
                    };



#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
static adc_err_t adc_check_open_cfg(adc_mode_t  const  mode,
                                    adc_cfg_t * const  p_cfg,
                                    void     (* const  p_callback)(void *p_args));

static adc_err_t adc_check_scan_config(adc_ch_cfg_t *p_config);
#endif

static adc_err_t adc_configure_scan(adc_ch_cfg_t *p_config);
static void adc_enable_s12gbadi(void);
static void adc_enable_s12adi(void);

R_BSP_PRAGMA_STATIC_INTERRUPT(adc_s12adi_isr, VECT(S12AD,S12ADI))
R_BSP_PRAGMA_STATIC_INTERRUPT(adc_s12gbadi_isr, VECT(S12AD,GBADI))



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
*                    Ignored for rx23t. Pass 0 for this argument.
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
adc_err_t adc_open(uint8_t     const   unit,
                    adc_mode_t  const   mode,
                    adc_cfg_t * const   p_cfg,
                    void     (* const   p_callback)(void *p_args))
{
    aregs_t             *p_regs;
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

    err = adc_check_open_cfg(mode, p_cfg, p_callback);
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
    p_regs->ADCSR.WORD = p_regs->ADCSR.WORD & 0x0400;
    p_regs->ADEXICR.WORD = 0; //Don't convert Internal Reference

    p_regs->ADANSA0.WORD = 0;
    p_regs->ADANSA1.WORD = 0;
    p_regs->ADANSB0.WORD = 0;
    p_regs->ADANSB1.WORD = 0;

    p_regs->ADADS0.WORD = 0;
    p_regs->ADADS1.WORD = 0;

    p_regs->ADADC.BYTE = 0;

    p_regs->ADCER.WORD = 0;
    p_regs->ADSTRGR.WORD = 0;

    p_regs->ADSSTR0 = 0x0d;
    p_regs->ADSSTR1 = 0x0d;
    p_regs->ADSSTR2 = 0x0d;
    p_regs->ADSSTR3 = 0x0d;
    p_regs->ADSSTR4 = 0x0d;
    p_regs->ADSSTR5 = 0x0d;
    p_regs->ADSSTR6 = 0x0d;
    p_regs->ADSSTR7 = 0x0d;
    p_regs->ADSSTRL = 0x0d;
    p_regs->ADSSTRO = 0x0d;

    /* A/D Sample-and-Hold Circuit Control Register initialize */
    p_regs->ADSHCR.WORD = ADC_SST_SH_CNT_DEFAULT;

    p_regs->ADDISCR.BYTE = 0;

    p_regs->ADGSPCR.WORD = 0;

    p_regs->ADHVREFCNT.BYTE = 0;


    /* SET MODE RELATED REGISTER FIELDS */
    g_dcb.mode = mode;

    if ((ADC_MODE_SS_MULTI_CH_GROUPED == mode)
    ||(ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A == mode))
    {
        p_regs->ADCSR.BIT.ADCS = ADC_ADCS_GROUP_SCAN;
    }
    else
    {
        if ((ADC_MODE_CONT_ONE_CH == mode) || (ADC_MODE_CONT_MULTI_CH == mode))
        {
            p_regs->ADCSR.BIT.ADCS = ADC_ADCS_CONT_SCAN;
        } // other modes have ADCS=0
    }

    if ((ADC_MODE_SS_ONE_CH_DBLTRIG == mode)
    || (ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A == mode))
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
        IPR(S12AD,GBADI) = p_cfg->priority_groupb;
    }

    IPR(S12AD,S12ADI) = p_cfg->priority;

    /* SET REGISTER FIELDS FOR REMAINING PARAMETERS */
    p_regs->ADCER.BIT.ADRFMT = (ADC_ALIGN_LEFT == p_cfg->alignment) ? 1 : 0;
    p_regs->ADCER.BIT.ACE = (ADC_CLEAR_AFTER_READ_ON == p_cfg->clearing) ? 1 : 0;
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
** Arguments    :mode -
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
static adc_err_t adc_check_open_cfg(adc_mode_t   const  mode,
                                    adc_cfg_t  * const  p_cfg,
                                    void      (* const  p_callback)(void *p_args))
{

    /* Check for valid argument values */
    if (p_cfg->priority > BSP_MCU_IPL_MAX)  // priority is not enum value(can't check by a Compiler)
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
    && ((ADC_TRIG_SOFTWARE == p_cfg->trigger) || (ADC_TRIG_ASYNC_ADTRG == p_cfg->trigger)))
    {
        return ADC_ERR_ILLEGAL_ARG;
    }

    /* Group checking; only synchronous triggers allowed; must be unique */
    if ((ADC_MODE_SS_MULTI_CH_GROUPED == mode) || (ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A == mode))
    {
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
*       with this restriction include ADANSA0, ADANSA1, ADANSB0, ADANSB1,
*       ADADC, ADADS0, ADADS1, ADSSTRx, ADEXICR, ADSHCR and some bits in
*       ADCSR, ADGSPCR.
*       No runtime operational sequence checking of any kind is performed.
*
* Arguments    : unit -
*                    Ignored for rx23t. Pass 0 for this argument.
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
*                ADC_ERR_UNKNOWN
*                    Did not receive expected hardware response
*******************************************************************************/
adc_err_t adc_control(uint8_t   const   unit,
                        adc_cmd_t const   cmd,
                        void *    const   p_args)
{
    adc_err_t       err=ADC_SUCCESS;
    adc_sst_t       *p_sample;
    adc_dda_t       *p_charge;

    /* Get S12AD register address */
    aregs_t         *p_regs = (aregs_t *)&S12AD;


    /* DO UNIVERSAL PARAMETER CHECKING */
#if (ADC_CFG_PARAM_CHECKING_ENABLE == 1)
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
    case ADC_CMD_USE_VREFH0:
        S12AD.ADHVREFCNT.BIT.HVSEL = 1;
        break;

    case ADC_CMD_USE_VREFL0:
        S12AD.ADHVREFCNT.BIT.LVSEL = 1;
        break;

    /* Set up Disconnect Detection Assist*/
    case ADC_CMD_SET_DDA_STATE_CNT:

        /* Cast from void pointer to adc_dda_t */
        p_charge = (adc_dda_t *)p_args;            //Get the command arguments
#if (ADC_CFG_PARAM_CHECKING_ENABLE == 1)
        if (ADC_DDA_OFF != p_charge->method)
        {

            /* Cannot have V-reference conversion enabled and Disconnect Detection enabled. */
            if ((p_charge->num_states < ADC_DDA_STATE_CNT_MIN)
            || (p_charge->num_states > ADC_DDA_STATE_CNT_MAX)
            || ((p_regs->ADEXICR.WORD & 0x0200) != 0)) // V-ref enabled
            {
                return ADC_ERR_ILLEGAL_ARG;
            }
        }
#endif
        if (ADC_DDA_OFF == p_charge->method)
        {
            p_regs->ADDISCR.BYTE = 0;              //Disable Disconnect Detection Assist
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
        p_sample = (adc_sst_t *)p_args;            //Get the command arguments
#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
        if (p_sample->num_states < ADC_SST_CNT_MIN)
        {
            return ADC_ERR_INVALID_ARG;
        }
#endif
        *(gp_sreg_ptrs)[p_sample->reg_id] = p_sample->num_states;
        break;

    case ADC_CMD_ENABLE_CHANS:

        /* Cast from void pointer to adc_ch_cfg_t */
        err = adc_configure_scan((adc_ch_cfg_t *)p_args);
        break;

    case ADC_CMD_ENABLE_VOLT_SENSOR:
#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
        if (ADC_MODE_SS_INT_REF_VOLT != g_dcb.mode)
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
#endif
        S12AD.ADEXICR.BIT.OCSA = 1;              // select internal reference voltage output on Single scan mode
        if (ADC_ADD_OFF != S12AD.ADADC.BIT.ADC)
        {
            S12AD.ADEXICR.BIT.OCSAD = 1;         // enable addition
        }
        adc_enable_s12adi();                    // setup interrupt handling
        break;

    /* Enable A/D conversion to be started by synchronous or asynchronous trigger */
    case ADC_CMD_ENABLE_TRIG:
        p_regs->ADCSR.BIT.TRGE = 1;           // enable sync/async triggers
        break;

    /* Disable A/D conversion to be started by synchronous or asynchronous trigger */
    case ADC_CMD_DISABLE_TRIG:
        p_regs->ADCSR.BIT.TRGE = 0;           // disable sync/async triggers
        break;

    /* Start the A/D conversion process */
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

    /* Check for completion of the current scan. */
    case ADC_CMD_CHECK_SCAN_DONE:
        if (1 == p_regs->ADCSR.BIT.ADST)
        {
            err = ADC_ERR_SCAN_NOT_DONE;
        }
        break;

    /* Check for completion of the current scan. */
    case ADC_CMD_CHECK_SCAN_DONE_GROUPA:
        if (1 == IR(S12AD,S12ADI))
        {
            IR(S12AD,S12ADI) = 0;
        }
        else
        {
            err = ADC_ERR_SCAN_NOT_DONE;
        }
        break;

    /* Check for completion of the current groupB scan. */
    case ADC_CMD_CHECK_SCAN_DONE_GROUPB:
        if (1 == IR(S12AD,GBADI))
        {
            IR(S12AD,GBADI) = 0;
        }
        else
        {
            err = ADC_ERR_SCAN_NOT_DONE;
        }
        break;

    case ADC_CMD_ENABLE_INT:
        p_regs->ADCSR.BIT.ADIE = 1;    // Enable Scan End Interrupt (S12ADI)
        break;

    case ADC_CMD_DISABLE_INT:
        p_regs->ADCSR.BIT.ADIE = 0;    // Disable Scan End Interrupt (S12ADI)
        break;

    case ADC_CMD_ENABLE_INT_GROUPB:
        p_regs->ADCSR.BIT.GBADIE = 1;  // Enable Group B Scan End Interrupt (GBADI)
        break;

    case ADC_CMD_DISABLE_INT_GROUPB:
        p_regs->ADCSR.BIT.GBADIE = 0;  // Disable Group B Scan End Interrupt (GBADI)
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
* Arguments    : p_config -
*                    Pointer to channel config structure containing masks
* Return Value : ADC_SUCCESS-
*                    Successful
*******************************************************************************/
static adc_err_t adc_configure_scan(adc_ch_cfg_t   *p_config)
{
    uint16_t  i=0;
    uint32_t  tmp_mask=0;

    /* Get S12AD register address */
    aregs_t   *p_regs = (aregs_t *)&S12AD;
    uint16_t  trig_a;
    uint16_t  trig_b;

#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
    adc_err_t   err;

    err = adc_check_scan_config(p_config);
    if (ADC_SUCCESS != err)
    {
        return err;
    }
#endif /* ADC_CFG_PARAM_CHECKING_ENABLE == 1 */

    /* SET MASKS FOR CHANNELS AND SENSORS */
    /* Select the channels from among Ch0 to Ch7 for conversion */
    p_regs->ADANSA0.WORD = (uint16_t) (p_config->chan_mask & 0x00FF);            //CH0 to CH7
    p_regs->ADANSB0.WORD = (uint16_t) (p_config->chan_mask_groupb & 0x00FF);     //CH0 to CH7

    /* Select the channels for Addition/Averaging (ch0 to ch7) */
    p_regs->ADADS0.WORD  = (uint16_t) (p_config->add_mask & 0x00FF);             //CH0 to CH7

    /* Select the channels from among Ch16 to Ch17 for conversion */
    S12AD.ADANSA1.WORD = (uint16_t) ((p_config->chan_mask >> 16) & 0x0003);      //CH16 to CH17

    /* Select the channels from among Ch16 to Ch17 for conversion */
    S12AD.ADANSB1.WORD = (uint16_t) ((p_config->chan_mask_groupb >> 16) & 0x0003);

    /* Select the channels for Addition/Averaging (Ch16 to Ch17)*/
    S12AD.ADADS1.WORD  = (uint16_t) ((p_config->add_mask >> 16) & 0x0003);

    /* NOTE: S&H adds to scan time because normal state machine still runs. */
    S12AD.ADSHCR.BIT.SHANS = p_config->sample_hold_mask;

   /* adds 12 + sample_hold_states ADCLKS to scan time */
    S12AD.ADSHCR.BIT.SSTSH = p_config->sample_hold_states;

    /* SET GROUP A PRIORITY ACTION (not interrupt priority!) */

    if (ADC_GRPA_PRIORITY_OFF != p_config->priority_groupa)
    {
        trig_a = p_regs->ADSTRGR.BIT.TRSA;              // save TRSA
        trig_b = p_regs->ADSTRGR.BIT.TRSB;              // save TRSB
        p_regs->ADSTRGR.BIT.TRSA = ADC_TRIG_NONE;
        p_regs->ADSTRGR.BIT.TRSB = ADC_TRIG_NONE;
        if (ADC_TRIG_NONE != p_regs->ADSTRGR.BIT.TRSA)  // bus wait for ADSTRGR write
        {
            R_BSP_NOP();
        }
        if (ADC_TRIG_NONE != p_regs->ADSTRGR.BIT.TRSB)  // bus wait for ADSTRGR write
        {
            R_BSP_NOP();
        }

        p_regs->ADGSPCR.WORD = p_config->priority_groupa;

        p_regs->ADSTRGR.BIT.TRSA = trig_a;             // restore TRSA
        if (1 != p_regs->ADGSPCR.BIT.GBRP)
        {
            p_regs->ADSTRGR.BIT.TRSB = trig_b;         // restore TRSB
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

        /* NOTE: Using Self Diagnosis adds 15(resolution) + (ch0 SST) ADCLKS to scan time.
           (ch0 can still be used with self diagnosis on) */
        if (ADC_DIAG_ROTATE_VOLTS == p_config->diag_method)
        {
            p_regs->ADCER.BIT.DIAGLD = 0;    //Rotate test voltages
            p_regs->ADCER.BIT.DIAGVAL = 1;   //Start with 0 V
        }
        else
        {

            /* Make sure DIAGVAL bits are not zero before setting the DIAGLD bit */
            p_regs->ADCER.BIT.DIAGVAL = (uint8_t)(p_config->diag_method & 0x3);   //Select diag. voltage
            p_regs->ADCER.BIT.DIAGLD = 1;    //Do not rotate test voltages
        }
        p_regs->ADCER.BIT.DIAGM = 1;         //Enable self diagnosis
    }

    /* SET DOUBLE TRIGGER CHANNEL */
    if (1 == p_regs->ADCSR.BIT.DBLE)         // If double-trigger mode is already selected.
    {
        tmp_mask = p_config->chan_mask;      // tmp_mask is non-Group/Group A chans

        /* WAIT_LOOP */
        while (tmp_mask >>= 1)               // determine bit/ch number
        {
            i++;
        }
        p_regs->ADCSR.BIT.DBLANS = i;
    }

    /* ENABLE INTERRUPTS */
    adc_enable_s12adi();
    if (ADC_ADCS_GROUP_SCAN == p_regs->ADCSR.BIT.ADCS)
    {
        adc_enable_s12gbadi();
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
static adc_err_t adc_check_scan_config(adc_ch_cfg_t   *p_config)
{
    uint32_t    tmp_mask=0;
    uint32_t    a_mask;
    uint32_t    b_mask;

    /* Get S12AD register address */
    aregs_t     *p_regs = (aregs_t *)&S12AD;

    /* Verify at least one bonded channel is selected */
    if ((0 == p_config->chan_mask)
    || (0 != (p_config->chan_mask & ADC_PRV_INVALID_CH_MASK))
    || ((ADC_MODE_SS_INT_REF_VOLT == g_dcb.mode) && (ADC_MASK_VOLT != p_config->chan_mask)))
    {
        return ADC_ERR_INVALID_ARG;
    }

    /* Verify at least one unique bonded channel is selected for Group B
     * and GroupA priority setting is valid.
     */
    if (ADC_ADCS_GROUP_SCAN == p_regs->ADCSR.BIT.ADCS)
    {
        if ((0 == p_config->chan_mask_groupb)
        || ((p_config->chan_mask_groupb & ADC_PRV_INVALID_CH_MASK) != 0)
        || ((ADC_MODE_SS_INT_REF_VOLT == g_dcb.mode) && (ADC_MASK_VOLT != p_config->chan_mask_groupb)))
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
    }
    else
    {

        /* for addition mask checking */
        if (0 != p_config->chan_mask_groupb)
        {
            return ADC_ERR_INVALID_ARG;
        }

        /* PGS bit must be set when group scan mode */
        if (ADC_GRPA_PRIORITY_OFF != p_config->priority_groupa)
        {
            return ADC_ERR_INVALID_ARG;
        }
    }

    /* If sensor specified, verify in legal configuation */
    if (ADC_MODE_SS_INT_REF_VOLT == g_dcb.mode)
    {
        if ((1 == S12AD.ADCSR.BIT.DBLE)       // Select double trigger mode
        || (0 != S12AD.ADDISCR.BYTE))        // disconnect detection assist enabled
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
    }

    /* Addition mask should not include bits from inactive channels */
    if (ADC_ADD_OFF != p_regs->ADADC.BIT.ADC)
    {
        tmp_mask |= p_config->chan_mask;        // tmp_mask is Group A and B combined

        /* Bit-AND with 1s-complement */
        if ((p_config->add_mask & (~tmp_mask)) != 0)
        {
            return ADC_ERR_INVALID_ARG;
        }
    }
    else
    {

        /* WARNING! Other features messed up if add_mask is non-zero when addition is turned off! */
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


    if (0 != p_config->sample_hold_mask)
    {

        /* S&H chans can only be 0,1,2 and must have at least minimum state count specified */
        if ((p_config->sample_hold_mask > 0x7)
        || (p_config->sample_hold_states < ADC_SST_SH_CNT_MIN))
        {
            return ADC_ERR_INVALID_ARG;
        }

        /* S&H channels cannot not be split across groups */
        a_mask = p_config->sample_hold_mask & p_config->chan_mask;
        b_mask = p_config->sample_hold_mask & p_config->chan_mask_groupb;

        /*  */
        if (ADC_ADCS_GROUP_SCAN == S12AD.ADCSR.BIT.ADCS)
        {
            if ((a_mask != p_config->sample_hold_mask) && (b_mask != p_config->sample_hold_mask))
            {
                return ADC_ERR_ILLEGAL_ARG;
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
        if ((1 == S12AD.ADCSR.BIT.DBLE) && (0 != a_mask))
        {
            return ADC_ERR_ILLEGAL_ARG;
        }

        /* S&H channels cannot be in GroupB if GroupA priority enabled */
        if ((0 != b_mask) && (ADC_GRPA_PRIORITY_OFF != p_config->priority_groupa))
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
    }

    if ((ADC_DIAG_OFF != p_config->diag_method)
    && ((0 != p_regs->ADADC.BIT.ADC)           // addition
    ||  (1 == p_regs->ADCSR.BIT.DBLE)))        // double trigger mode
    {
        return ADC_ERR_ILLEGAL_ARG;
    }

    if (0 != (p_regs->ADDISCR.BYTE & 0x0F))
    {
        if (0 != p_config->sample_hold_mask)
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
    }

    return ADC_SUCCESS;
} /* End of function adc_check_scan_config() */
#endif /* ADC_CFG_PARAM_CHECKING_ENABLE == 1 */


/******************************************************************************
* Function Name: adc_enable_s12gbadi
* Description  : This function clears the S12GBADI interrupt flag and enables
*                interrupts in the peripheral (for IR flag usage). If priority
*                is not 0, interrupts are enabled in the ICU.
*                NOTE: This has nothing to do with enabling triggers.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void adc_enable_s12gbadi(void)
{
    IR(S12AD,GBADI) = 0;            // clear flag
    S12AD.ADCSR.BIT.GBADIE = 1;     // enable in peripheral
    if (0 != IPR(S12AD,GBADI))
    {
        R_BSP_InterruptRequestEnable(VECT(S12AD,GBADI));       // enable in ICU
    }
} /* End of function adc_enable_s12gbadi() */

/*****************************************************************************
* Function Name: adc_enable_s12adi
* Description  : This function clears the S12ADI interrupt flag and enables
*                interrupts in the peripheral (for IR flag usage). If priority
*                is not 0, interrupts are enabled in the ICU.
*                NOTE: This has nothing to do with enabling triggers.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void adc_enable_s12adi(void)
{

    IR(S12AD,S12ADI) = 0;                  // clear flag
    S12AD.ADCSR.BIT.ADIE = 1;               // enable in peripheral
    if (0 != ICU.IPR[IPR_S12AD_S12ADI].BYTE)
    {
        R_BSP_InterruptRequestEnable(VECT(S12AD,S12ADI));             // enable in ICU
    }
} /* End of function adc_enable_s12adi() */


/******************************************************************************
* Function Name: adc_close
* Description  : This is the RX23T implementation of R_ADC_Close().
*                This function ends any scan in progress, disables interrupts,
*                and removes power to the A/D peripheral.
* Arguments    : unit - This argument is ignored for the RX23T.
* Return Value : ADC_SUCCESS - Successful
*                ADC_ERR_INVALID_ARG - unit contains an invalid value.
*******************************************************************************/
adc_err_t adc_close(uint8_t const  unit)
{
    volatile uint8_t i;

    /* Get S12AD register address */
    aregs_t     *p_regs = (aregs_t *)&S12AD;

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
    R_BSP_InterruptRequestDisable(VECT(S12AD,S12ADI));     // disable interrupts in ICU
    IR(S12AD,S12ADI) = 0;      // clear interrupt flag
    R_BSP_InterruptRequestDisable(VECT(S12AD,GBADI));       // disable interrupts in ICU
    IR(S12AD,GBADI) = 0;        // clear interrupt flag

    p_regs->ADGSPCR.BIT.PGS = 0;
    if (ADC_ADCS_GROUP_SCAN == p_regs->ADCSR.BIT.ADCS)
    {
        p_regs->ADSTRGR.WORD = 0x3F3F;
    }
    else
    {
        p_regs->ADSTRGR.BIT.TRSA = 0x3F;
    }

    p_regs->ADCSR.WORD = p_regs->ADCSR.WORD & 0x0400;
    if (0x0000 == (p_regs->ADCSR.WORD & 0xFBFF))   // dummy read for waiting until set the value of ADCSR
    {
        R_BSP_NOP();
    }

    /* Wait for 2 ADCLK cycles (MAX: 128 ICLK cycles) */
    /* WAIT_LOOP */
    for (i = 0; i < 128; i++)
    {
        R_BSP_NOP();
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
* Description  : This is the RX23T implementation of R_ADC_ReadAll().
*                It reads conversion results from all potential sources,
*                enabled or not.
* Arguments    : p_all_data-
*                    Pointer to structure to load register values into.
* Return Value : ADC_SUCCESS-
*                    Successful
*******************************************************************************/
adc_err_t adc_read_all(adc_data_t * const  p_all_data)
{
    p_all_data->chan[ADC_REG_CH0]  = S12AD.ADDR0;
    p_all_data->chan[ADC_REG_CH1]  = S12AD.ADDR1;
    p_all_data->chan[ADC_REG_CH2]  = S12AD.ADDR2;
    p_all_data->chan[ADC_REG_CH3]  = S12AD.ADDR3;
    p_all_data->chan[ADC_REG_CH4]  = S12AD.ADDR4;
    p_all_data->chan[ADC_REG_CH5]  = S12AD.ADDR5;
    p_all_data->chan[ADC_REG_CH6]  = S12AD.ADDR6;
    p_all_data->chan[ADC_REG_CH7]  = S12AD.ADDR7;

    p_all_data->chan[ADC_REG_CH16] = S12AD.ADDR16;
    p_all_data->chan[ADC_REG_CH17] = S12AD.ADDR17;
    p_all_data->volt               = S12AD.ADOCDR;

    p_all_data->dbltrig            = S12AD.ADDBLDR;
    p_all_data->dbltrigA           = S12AD.ADDBLDRA;
    p_all_data->dbltrigB           = S12AD.ADDBLDRB;
    p_all_data->self_diag          = S12AD.ADRD;

    return ADC_SUCCESS;
} /* End of function adc_read_all() */


/*****************************************************************************
* Function Name: adc_s12adi_isr
* Description  : Unit0 interrupt handler for normal/Group A/double trigger
*                scan complete.
* Arguments    : none
* Return Value : none
******************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void adc_s12adi_isr(void)
{
    adc_cb_evt_t    event = ADC_EVT_SCAN_COMPLETE;

    /* presence of callback function verified in Open() */
    if ((NULL != g_dcb.p_callback) && (FIT_NO_FUNC != g_dcb.p_callback))
    {
        g_dcb.p_callback(&event);
    }
} /* End of function adc_s12adi_isr() */


/*****************************************************************************
* Function Name: adc_s12gbadi_isr
* Description  : Unit0 interrupt handler for Group B scan complete.
* Arguments    : none
* Return Value : none
******************************************************************************/
R_BSP_ATTRIB_STATIC_INTERRUPT void adc_s12gbadi_isr(void)
{
    adc_cb_evt_t    event = ADC_EVT_SCAN_COMPLETE_GROUPB;

    /* presence of callback function verified in Open() */
    if ((NULL != g_dcb.p_callback) && (FIT_NO_FUNC != g_dcb.p_callback))
    {
        g_dcb.p_callback(&event);
    }
} /* End of function adc_s12gbadi_isr() */

#endif /* #if defined BSP_MCU_RX23T */

