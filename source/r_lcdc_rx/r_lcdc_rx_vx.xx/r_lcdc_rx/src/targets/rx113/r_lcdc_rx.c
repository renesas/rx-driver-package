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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 01.12.2014 1.00     First Release
*              : 20.05.2019 1.01     Supported the following compilers:
*                                    - GCC for Renesas RX
*                                    - IAR C/C++ Compiler for Renesas RX
*                                    Added comments "WAIT_LOOP".
*                                    Deleted the inline expansion of the 
*                                    R_LCDC_GetVersion function.
*                                    Deleted the "#include <machine.h>".
*******************************************************************************/
/*******************************************************************************
* File Name    : r_lcdc_rx.c
* Device(s)    : RX113
* Tool-Chain   : Renesas RX v3.01.00
*                GCC for Renesas RX 4.8.4.201801
*                IAR C/C++ Compiler for Renesas RX version 4.10.1
* OS           : None
* H/W Platform : RSKRX113
* Description  : Functions for using LCDC on RX devices. 
*******************************************************************************/
/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stddef.h>
/* Fixed width integers */
#include <stdint.h>
/* Boolean defines */
#include <stdbool.h>
/* Access to peripherals and board defines. */
#include "platform.h"

/* Defines for LCDC support */
#include "r_lcdc_rx_if.h"
#include "r_lcdc_rx_config.h"
#include "r_lcdc_rx_private.h"

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/
uint8_t g_lcdc_open_flg;
uint8_t g_lcdc_vol_method_flg;

/*****************************************************************************
* Function Name: R_LCDC_Open
* Description  : This function initializes the LCDC to get it ready for 
*                displaying data on the LCD.
* Arguments    : void
* Return Value : LCDC_SUCCESS               ; Successful operation
*              : LCDC_ERR_ERR_NOT_CLOSED    ; Already locked
*              : LCDC_ERR_UNSUPPORTED       ; Combination of display mode not supported
*              : LCDC_ERR_NOT_OPERATE       ; Specified clock source not operated
******************************************************************************/
lcdc_err_t R_LCDC_Open(void)
{
    /* The valuable for storing a return value. */
    lcdc_err_t   ret;
    /* The valuable for storing a return value for lock function. */
    bool chk;

    /* ---- CHECK ARGUMENTS ---- */
#if (1U == LCDC_CFG_PARAM_CHECKING_ENABLE)

    /* ---- CHECK OSCILLATING LCD CLOCK SOURCE ---- */
#if ( LCDC_CLK_LOCO == BSP_CFG_LCD_CLOCK_SOURCE)
     /* LOCO is chosen as LCDC clock source */
    if ( LCDC_NOT_OPERATE == SYSTEM.LOCOCR.BIT.LCSTP)
    {
        return LCDC_ERR_NOT_OPERATE;
    }
#elif ( LCDC_CLK_HOCO == BSP_CFG_LCD_CLOCK_SOURCE) 
    /* HOCO is chosen as LCDC clock source */
    if (LCDC_NOT_OPERATE == SYSTEM.HOCOCR.BIT.HCSTP)
    {
        return LCDC_ERR_NOT_OPERATE;
    }
#elif ( LCDC_CLK_MAIN == BSP_CFG_LCD_CLOCK_SOURCE) 
    /* Main clock is chosen as LCDC clock source */
    if (LCDC_NOT_OPERATE == SYSTEM.MOSCCR.BIT.MOSTP)
    {
        return LCDC_ERR_NOT_OPERATE;
    }
#elif ( LCDC_CLK_SUB == BSP_CFG_LCD_CLOCK_SOURCE) 
    /* Sub-clock is chosen as LCDC clock source */
    if ( LCDC_NOT_OPERATE == SYSTEM.SOSCCR.BIT.SOSTP)
    {
        return LCDC_ERR_NOT_OPERATE;
    }
#elif ( LCDC_CLK_IWDT == BSP_CFG_LCD_CLOCK_SOURCE) 
    /* IWDT-dedicated on-chip oscillator is chosen as LCDC clock source */
    if ( LCDC_NOT_OPERATE == SYSTEM.ILOCOCR.BIT.ILCSTP)
    {
        return LCDC_ERR_NOT_OPERATE;
    }
#endif

    /* Check combination of display mode */
#if (LCDC_WAVE_A == LCDC_CFG_DRV_WAVEFORM)
    if(LCDC_SLICE_8 == LCDC_CFG_TIME_SLICES && (LCDC_BIAS_4 == LCDC_CFG_BIAS) 
    && ((LCDC_VOL_EXTARNAL == LCDC_CFG_DRV_GENERATOR) || (LCDC_VOL_INTERNAL == LCDC_CFG_DRV_GENERATOR)))
    {
        /* Do nothing */
    }
    else if((LCDC_SLICE_4 == LCDC_CFG_TIME_SLICES) && (LCDC_BIAS_3 == LCDC_CFG_BIAS))
    {
        /* Do nothing */
    }
        
    else if((LCDC_SLICE_3 == LCDC_CFG_TIME_SLICES) && ((LCDC_BIAS_3 == LCDC_CFG_BIAS) ||
    ((LCDC_BIAS_2 == LCDC_CFG_BIAS) && (LCDC_VOL_EXTARNAL == LCDC_CFG_DRV_GENERATOR))))
    {
        /* Do nothing */
    }
    
    else if ((LCDC_SLICE_2 == LCDC_CFG_TIME_SLICES) && (LCDC_BIAS_2 == LCDC_CFG_BIAS) 
    && (LCDC_VOL_EXTARNAL == LCDC_CFG_DRV_GENERATOR))
    {
        /* Do nothing */
    }
    
    else if ((LCDC_STATIC == LCDC_CFG_TIME_SLICES) && (LCDC_VOL_EXTARNAL == LCDC_CFG_DRV_GENERATOR))
    {
        /* Do nothing */
    }
    else
    {
        return LCDC_ERR_UNSUPPORTED;
    }
    
#else /* Waveform B*/
    if((LCDC_SLICE_8 == LCDC_CFG_TIME_SLICES) && (LCDC_BIAS_4 == LCDC_CFG_BIAS) &&
    ((LCDC_VOL_EXTARNAL == LCDC_CFG_DRV_GENERATOR)||(LCDC_VOL_INTERNAL == LCDC_CFG_DRV_GENERATOR)))
    {
        /* Do nothing */
    }
        
    else if((LCDC_SLICE_4 == LCDC_CFG_TIME_SLICES) && (LCDC_BIAS_3 == LCDC_CFG_BIAS))
    {
        /* Do nothing */
    }
    
    else
    {
        return LCDC_ERR_UNSUPPORTED;
    }
#endif
#endif
    /*  ---- LOCKS THE LCDC ---- */
    chk = R_BSP_HardwareLock(BSP_LOCK_LCDC);

    if (false == chk)
    {
        /* Lock has already been acquired by another task. Needs to try again later. */
        return LCDC_ERR_NOT_CLOSED;
    }

    /* ---- INITIALIZE LCDC INTERNAL STATUS INFORMATION ---- */
    g_lcdc_open_flg = LCDC_NOT_OPEN;
    g_lcdc_vol_method_flg = LCDC_VOL_METHOD_INTERNAL;

    /* ---- INITIALIZE LCDC ---- */
    /* Calls the API function. */
    ret = lcdc_open();

    return ret;
}

/*---------------------------------------------------------------------------*/
static lcdc_err_t lcdc_open(void)
{
    /* Sets LCDC_ERR_OTHER as initialization of ret. */
    lcdc_err_t ret = LCDC_SUCCESS;  
    
    /* Cancels the module-stop state for the LCDC. */
    power_on();
    

    /* Specifies the voltage boosting pin initial value switching control bit. */
#if (LCDC_VOL_INTERNAL == LCDC_CFG_DRV_GENERATOR)
    LCDC.LCDM1.BIT.LCDVLM = 1;
#endif

    /* Specifies waveform. */
    LCDC.LCDM0.BIT.LWAVE = LCDC_CFG_DRV_WAVEFORM;
    
    /* Specifies number of time slices. */
#if (LCDC_STATIC == LCDC_CFG_TIME_SLICES)
    LCDC.LCDM0.BIT.LDTY = 0;
#elif (LCDC_SLICE_2 == LCDC_CFG_TIME_SLICES)
    LCDC.LCDM0.BIT.LDTY = 1;
#elif (LCDC_SLICE_3 == LCDC_CFG_TIME_SLICES)
    LCDC.LCDM0.BIT.LDTY = 2;
#elif (LCDC_SLICE_4 == LCDC_CFG_TIME_SLICES)
    LCDC.LCDM0.BIT.LDTY = 3;
#elif (LCDC_SLICE_8 == LCDC_CFG_TIME_SLICES)
    LCDC.LCDM0.BIT.LDTY = 5;
#endif
    
    /* Specifies the bias method. */
#if (LCDC_BIAS_2 == LCDC_CFG_BIAS)
    LCDC.LCDM0.BIT.LBAS = 0;
#elif (LCDC_BIAS_3 == LCDC_CFG_BIAS)
    LCDC.LCDM0.BIT.LBAS = 1;
#elif (LCDC_BIAS_4 == LCDC_CFG_BIAS)
    LCDC.LCDM0.BIT.LBAS = 2;
#endif

    /* Configures the drive voltage generator. */
    LCDC.LCDM0.BIT.MDSET = LCDC_CFG_DRV_GENERATOR;
    
#if(LCDC_VOL_INTERNAL == LCDC_CFG_DRV_GENERATOR)
   g_lcdc_vol_method_flg = LCDC_VOL_METHOD_INTERNAL;
#elif (LCDC_VOL_CAPACITOR == LCDC_CFG_DRV_GENERATOR)
   g_lcdc_vol_method_flg = LCDC_VOL_METHOD_CAPA;
#endif
    
    /* Specifies the display data area. */
    if (LCDC_SLICE_8 !=  LCDC_CFG_TIME_SLICES)
    {
        LCDC.LCDM1.BIT.BLON = LCDC_NOT_BLINKING;
        LCDC.LCDM1.BIT.LCDSEL = LCDC_DISP_A;
    }
    
    /* Protect off of CGC. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_CGC);
    
    /* LCD source clock output is stopped. */
    SYSTEM.LCDSCLKCR2.BIT.LCDSCLKSTP = LCDC_CLK_STOP;
    
    /* Division of LCDC clock source. */
    LCDC.LCDC0.BIT.LCDC0 = LCDC_CFG_CLOCK_DIV;
    
    /* Select clock source for LCDC. */
    SYSTEM.LCDSCLKCR.BIT.LCDSCLKSEL = BSP_CFG_LCD_CLOCK_SOURCE;    
    /* WAIT_LOOP */
    while (BSP_CFG_LCD_CLOCK_SOURCE != SYSTEM.LCDSCLKCR.BIT.LCDSCLKSEL)
    {
        /* Wait for setting LCD clock source */
    }
    
    /* LCDC source clock output is start. */
    SYSTEM.LCDSCLKCR2.BIT.LCDSCLKSTP = LCDC_CLK_NOT_STOP;
    
    /* Protect on for CGC. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_CGC);

    /* Specifies the voltage level. */
#if (LCDC_VOL_INTERNAL == LCDC_CFG_DRV_GENERATOR)
    LCDC.VLCD.BYTE = LCDC_CFG_REF_VCC;

    if(LCDC_CFG_REF_VCC == LCDC.VLCD.BYTE)
    {
        R_BSP_NOP();
    }

    lcdc_delay_us(LCDC_SETUP_WAIT);
#endif

    /* Enable the voltage boost circuit or capacitor split circuit. */
#if (LCDC_VOL_INTERNAL == LCDC_CFG_DRV_GENERATOR) || (LCDC_VOL_CAPACITOR == LCDC_CFG_DRV_GENERATOR)
    ret = R_LCDC_OperateCircuit(LCDC_CIRCUIT_ENABLE);
#endif
    
    /* Set open judging flag. */
    g_lcdc_open_flg = LCDC_OPEN;

    return ret;
}


/*****************************************************************************
* Function Name: R_LCDC_Write
* Description  : This function specifies display data.
* Arguments    : uint8_t seg                ; Specifies the segment to be written
*              : uint8_t data               ; Data to be written
* Return Value : LCDC_SUCCESS               ; Processing completed successfully
*              : LCDC_ERR_INVALID_ARG       ; Invalid parameter
******************************************************************************/
lcdc_err_t R_LCDC_Write(uint8_t seg, uint8_t data)
{
    /* The valuable for storing a return value. */
    lcdc_err_t   ret;

    /* ---- CHECK ARGUMENTS ---- */
#if (1U == LCDC_CFG_PARAM_CHECKING_ENABLE)

    if (39 < seg)
    {
        return LCDC_ERR_INVALID_ARG;
    }
    
#endif

    /* Calls the API function. */
    ret = lcdc_write(seg, data);

    return ret;
}

/*---------------------------------------------------------------------------*/
static lcdc_err_t lcdc_write(uint8_t seg, uint8_t data)
{
    /* Sets LCDC_SUCCESS as initialization of ret. */
    lcdc_err_t   ret= LCDC_SUCCESS;
    
    volatile uint8_t * const  pcSEG_ADR = SEG_ADR(seg);
    
    /* The display data is stored in the display data register. */
    *pcSEG_ADR = data;

    return ret;
}


/*****************************************************************************
* Function Name: R_LCDC_Modify
* Description  : This function is used when rewriting the display data.
* Arguments    : uint8_t seg                ; Display data
*                uint8_t data_mask          ; Specifies where to rewrite
*                uint8_t data               ; Data to rewrite
* Return Value : LCDC_SUCCESS               ; Processing completed successfully
*              : LCDC_ERR_INVALID_ARG       ; Invalid parameter
******************************************************************************/
lcdc_err_t R_LCDC_Modify(uint8_t seg, uint8_t data_mask, uint8_t data)
{
    /* The variable for storing a return value. */
    lcdc_err_t   ret;

    /* ---- CHECK ARGUMENTS ---- */
#if (1U == LCDC_CFG_PARAM_CHECKING_ENABLE)

   if (39 < seg)
   {
       return LCDC_ERR_INVALID_ARG;
   }
   
#endif

    ret = lcdc_modify(seg, data_mask, data);


    return ret;
}

/*---------------------------------------------------------------------------*/
static lcdc_err_t lcdc_modify(uint8_t seg, uint8_t data_mask, uint8_t data)
{
    /* Sets LCDC_SUCCESS as initialization of ret. */
    lcdc_err_t ret = LCDC_SUCCESS;
    volatile uint8_t * const  pcSEG_ADR = SEG_ADR(seg);

    /* Masks the data being displayed. */
    *pcSEG_ADR &= data_mask;
    
    /* Specifies the data to rewrite. */
    *pcSEG_ADR |= data;

    return ret;
}


/*****************************************************************************
* Function Name: R_LCDC_DispOn
* Description  : This function specifies display data and displays the data on the LCD.
* Arguments    : void
* Return Value : LCDC_SUCCESS               ; Successful operation
*              : LCDC_ERR_NOT_OPEN          ; R_LCDC_Open function not executed
******************************************************************************/
lcdc_err_t R_LCDC_DispOn(void)
{
    /* The variable for storing a return value. */
    lcdc_err_t ret;

    /* ---- CHECK ARGUMENTS ---- */
#if (1U == LCDC_CFG_PARAM_CHECKING_ENABLE)

    if (LCDC_OPEN != g_lcdc_open_flg)
    {
        return LCDC_ERR_NOT_OPEN;
    }

#endif

    /* Calls the API function. */
    ret = lcdc_dispon();

    return ret;
}

/*---------------------------------------------------------------------------*/
static lcdc_err_t lcdc_dispon(void)
{
    /* Sets LCDC_SUCCESS as initialization of ret. */
    lcdc_err_t ret = LCDC_SUCCESS;

    /* Enables display on the LCD. */
    LCDC.LCDM1.BIT.LCDON = LCDC_DISP_ON;
    LCDC.LCDM1.BIT.SCOC = LCDC_DISP_ENABLE;

    return ret;
}


/*****************************************************************************
* Function Name: R_LCDC_DispOff
* Description  : This function disables display on the LCD, 
*                and stops the internal voltage boost circuit or capacitor split circuit.
* Arguments    : uint8_t stop_select        ; Select the operation, 
* according to the specifications of the user system, whether to disable 
* display on the LCD only or also stop the voltage boost/capacitor split circuit.
* Return Value : LCDC_SUCCESS               ; Processing completed successfully
*              : LCDC_ERR_INVALID_ARG       ; Invalid parameter
******************************************************************************/
lcdc_err_t R_LCDC_DispOff(uint8_t stop_select)
{
    /* The variable for storing a return value. */
    lcdc_err_t   ret;

    /* ---- CHECK ARGUMENTS ---- */
#if (1 == LCDC_CFG_PARAM_CHECKING_ENABLE)

    if((LCDC_ONLY_DISPLAY_OFF != stop_select) && (LCDC_CIRCUIT_STOP != stop_select))
    {
        return LCDC_ERR_INVALID_ARG;
    }
    
#endif

    /* Calls the API function. */
    ret = lcdc_dispoff(stop_select);

    return ret;
}

/*---------------------------------------------------------------------------*/
static lcdc_err_t lcdc_dispoff(uint8_t stop_select)
{
    /* Sets LCDC_SUCCESS as initialization of ret. */
    lcdc_err_t   ret = LCDC_SUCCESS; 

    /* Display is disable. */
    LCDC.LCDM1.BIT.LCDON = LCDC_DISP_OFF;
    LCDC.LCDM1.BIT.SCOC = LCDC_DISP_ENABLE;

    /* If LCDC_CIRCUIT_STOP is selected, the circuit is stopped. */
    if (LCDC_CIRCUIT_STOP == stop_select)
    {
#if((LCDC_VOL_INTERNAL == LCDC_CFG_DRV_GENERATOR) || (LCDC_VOL_CAPACITOR == LCDC_CFG_DRV_GENERATOR))
        ret = R_LCDC_OperateCircuit(LCDC_CIRCUIT_DISABLE);
#endif
    }

    return ret;
}

#if((LCDC_VOL_INTERNAL == LCDC_CFG_DRV_GENERATOR) || (LCDC_VOL_CAPACITOR == LCDC_CFG_DRV_GENERATOR))
/*****************************************************************************
* Function Name: R_LCDC_OperateCircuit
* Description  : This function enables or disables the internal voltage boost/capacitor split circuit.
* Arguments    : uint8_t select_operation   ; Select the operation whether to enable or disable 
*                                             the internal voltage boost/capacitor split circuit.
* Return Value : LCDC_SUCCESS               ; Processing completed successfully
*              : LCDC_ERR_INVALID_ARG       ; Invalid parameter
******************************************************************************/
lcdc_err_t R_LCDC_OperateCircuit(uint8_t select_operation)
{
    /* The variable for storing a return value. */
    lcdc_err_t   ret;

    /* ---- CHECK ARGUMENTS ---- */
#if (1U == LCDC_CFG_PARAM_CHECKING_ENABLE)

    if((LCDC_SEL_CIRCUIT_ENABLE != select_operation) && (LCDC_SEL_CIRCUIT_DISABLE != select_operation))
    {
        return LCDC_ERR_INVALID_ARG;
    }
    
#endif

    if (LCDC_SEL_CIRCUIT_ENABLE == select_operation)
    {
        /* Calls the API function. */
        ret = lcdc_operatecircuit_enable();
    }
    
    else if (LCDC_SEL_CIRCUIT_DISABLE == select_operation)
    {
        /* Calls the API function. */
        ret = lcdc_operatecircuit_disable();
    }
    
    else
    {
        ret = LCDC_ERR_OTHER;
    }

    return ret;
}

/*---------------------------------------------------------------------------*/
static lcdc_err_t lcdc_operatecircuit_enable(void)
{

    /* Sets LCDC_SUCCESS as initialization of ret. */
    lcdc_err_t ret = LCDC_SUCCESS;
    uint8_t i;
    
    /* Operating the circuit again */
    if (LCDC_OPEN == g_lcdc_open_flg)
    {
        /* The drive voltage method is switch back to the method previously used. */
        if(LCDC_VOL_METHOD_INTERNAL == g_lcdc_vol_method_flg) 
        {
            LCDC.LCDM0.BIT.MDSET  = LCDC_VOL_INTERNAL;
        }
        else if (LCDC_VOL_METHOD_CAPA == g_lcdc_vol_method_flg)
        {
            LCDC.LCDM0.BIT.MDSET = LCDC_VOL_CAPACITOR;
        }
        else
        {
            ret = LCDC_ERR_OTHER;
        }
    }
    
    /* The circuit is enabled. */
    LCDC.LCDM1.BIT.VLCON = LCDC_CIRCUIT_ENABLE;
    if(LCDC_CIRCUIT_ENABLE == LCDC.LCDM1.BIT.VLCON)
    {
        R_BSP_NOP();
    }

    if ( LCDC_VOL_METHOD_INTERNAL == g_lcdc_vol_method_flg)
    {
        /* Waiting for the voltage boosting using the internal (500ms). */
        /* WAIT_LOOP */
        for (i=0; i<LCDC_BOOST_COUNTER; i++)
        {
            lcdc_delay_us(LCDC_BOOST_WAIT);
        }
   }
   else if(LCDC_VOL_METHOD_CAPA == g_lcdc_vol_method_flg)
   {
        /* Waiting for the voltage boosting using the capacitor (100ms) */
        lcdc_delay_us(LCDC_BOOST_WAIT);
   }
   else
   {
        /* Do nothing */
   }

    return ret;
}
/*---------------------------------------------------------------------------*/
static lcdc_err_t lcdc_operatecircuit_disable(void)
{
    /* Sets LCDC_SUCCESS as initialization of ret. */
    lcdc_err_t ret = LCDC_SUCCESS;
    

    if (LCDC_DISP_ENABLE == LCDC.LCDM1.BIT.SCOC)
    {
        /* Display off */
        LCDC.LCDM1.BIT.SCOC = LCDC_DISP_DISABLE;
    }

    /* The circuit is disable. */
    LCDC.LCDM1.BIT.VLCON = LCDC_CIRCUIT_DISABLE;

    if(LCDC_CIRCUIT_DISABLE == LCDC.LCDM1.BIT.VLCON)
    {
        R_BSP_NOP();
    }

    /* Wait for 2 LCD clock */
    lcdc_delay_us((2*(LCDC_NS_TO_US/BSP_LCDSRCCLK_HZ))+1);

    /* Switch to the external resistance division. */
    LCDC.LCDM0.BIT.MDSET = LCDC_VOL_EXTARNAL;

    /* Display on */
    LCDC.LCDM1.BIT.SCOC = LCDC_DISP_ENABLE;

    return ret;
}
#endif


/*****************************************************************************
* Function Name: R_LCDC_Control
* Description  : This function changes the drive voltage method and the display data area
* Arguments    : uint8_t select_operation   ; Select what is changed
*                uint8_t select_drv_gen     ; When changing the drive voltage method, selects the way to change
*                uint8_t select_display_area; When changing the display data area, selects the way to change
* Return Value : LCDC_SUCCESS               ; Processing completed successfully
*              : LCDC_ERR_RTC_NOT_OPERATE   ; Invalid parameter
*              : LCDC_ERR_INVALID_ARG       ; RTC not operated with blinking display selected
******************************************************************************/
lcdc_err_t R_LCDC_Control(uint8_t config_pattern, uint8_t select_drv_gen, uint8_t select_display_area)
{
    /* The variable for storing a return value. */
    lcdc_err_t   ret;

    /* ---- CHECK ARGUMENTS ---- */
#if (1U == LCDC_CFG_PARAM_CHECKING_ENABLE)

    if (LCDC_GEN_ERR_CON == config_pattern)
    {
        return LCDC_ERR_INVALID_ARG;
    }
    
#endif


    /* Calls the API function. */
    ret = lcdc_conrtol(config_pattern, select_drv_gen, select_display_area);

    return ret;
}

/*---------------------------------------------------------------------------*/
static lcdc_err_t lcdc_conrtol(uint8_t config_pattern, uint8_t select_drv_gen, uint8_t select_display_area)
{

    /* Sets LCDC_SUCCESS as initialization of ret. */
    lcdc_err_t ret = LCDC_SUCCESS;
    
    /* Changing the drive voltage method. */
    if((config_pattern & LCDC_FIX_DRV_GENERETOR) == LCDC_FIX_DRV_GENERETOR)
    {

        /* Display off */
        ret = R_LCDC_DispOff(LCDC_ONLY_DISPLAY_OFF);
        
        /* Switch to capacitor split method from internal voltage boosting method. */
        if(LCDC_DRV_INTERNAL_TO_CAPA == select_drv_gen)
        { 
            LCDC.LCDM0.BIT.MDSET = LCDC_VOL_CAPACITOR;
            g_lcdc_vol_method_flg = LCDC_VOL_METHOD_CAPA;
        }
 
        /* Switch to internal voltage boosting method from capacitor spilit method. */
        else if(LCDC_DRV_CAPA_TO_INTERNAL == select_drv_gen) 
        {
            LCDC.LCDM0.BIT.MDSET = LCDC_VOL_INTERNAL;
            g_lcdc_vol_method_flg = LCDC_VOL_METHOD_INTERNAL;
        }
        else
        {
            ret = LCDC_ERR_OTHER;
        }

        /* Display on */
        ret = R_LCDC_DispOn();
    }
    
    /* Changing the display data area. */
    if((config_pattern & LCDC_FIX_DISPLAY_AREA) == LCDC_FIX_DISPLAY_AREA)
    {
        /* Switch to A-pattern from B-pattern or blinking. */
        if (LCDC_PATTERN_A == select_display_area)
        {
            LCDC.LCDM1.BIT.BLON = LCDC_NOT_BLINKING;
            LCDC.LCDM1.BIT.LCDSEL = LCDC_DISP_A;
        }
        
        /* Switch to B-pattern from A-pattern */
        else if(LCDC_PATTERN_B == select_display_area)
        {
            LCDC.LCDM1.BIT.BLON = LCDC_NOT_BLINKING;
            LCDC.LCDM1.BIT.LCDSEL = LCDC_DISP_B;
        }

        /* Switch to blinking. */
        else if (LCDC_PATTERN_BLINKING == select_display_area)
        {
            /* If Clock source of RTC is disabled. */
            if(0 == RTC.RCR3.BIT.RTCEN)
            {
                /* Return error. */
                ret = LCDC_ERR_RTC_NOT_OPERATE;
            }
            
            /* If IWDT clock source is selected as LCDC clock source */
            if (LCDC_CLK_IWDT == BSP_CFG_LCD_CLOCK_SOURCE)
            {
                /* Return error. */
                ret = LCDC_ERR_UNSUPPORTED;
            }
            
            LCDC.LCDM1.BIT.BLON = LCDC_BLINKING;
            LCDC.LCDM1.BIT.LCDSEL = LCDC_DISP_A;
        }
        
        else
        {
            ret = LCDC_ERR_OTHER;
        }
    }

    return ret;
}

#if(LCDC_VOL_INTERNAL == LCDC_CFG_DRV_GENERATOR)
/*****************************************************************************
* Function Name: R_LCDC_AdjustContrast
* Description  : This function adjusts the contrast.
* Arguments    : uint8_t select_read_adjust ; Selects whether to read or change the voltage level.
*                uint8_t updown_level       ; New voltage level.
* Return Value : LCDC_VOL_LEVEL             ; Current voltage level returned 
*                                             when processing completed successfully.
*                                             If the voltage level is changed, 
*                                             then the changed level is returned.
*              : LCDC_ERR_OUTSIDE_VOLTAGE   ; Invalid voltage (outside the range)
******************************************************************************/
int8_t R_LCDC_AdjustContrast(uint8_t select_read_adjust, int8_t updown_level)
{
    /* The valuable for storing a return value. */
    int8_t   ret;

    if(LCDC_VOL_READ == select_read_adjust)
    {
        /* Return the current voltage level */
        ret = (int8_t)LCDC.VLCD.BYTE;
    }
    
    else if(LCDC_VOL_ADJUST == select_read_adjust)
    {
       /* Calls the API function. */
       ret = lcdc_adjustcontrast(updown_level);
    }
    
    else
    {
        ret = LCDC_ERR_OTHER;
    }


    return ret;
}

/*---------------------------------------------------------------------------*/
int8_t lcdc_adjustcontrast(int8_t updown_level)
{
    /* The valuable for storing a return value. */
    int8_t ret; 
    /* The valuable for storing the current voltage. */
    uint8_t now_vol;
    /* The valuable for storing the voltage level after change */
    int8_t  new_vol;
    
    /* Verify the specified level is within the select range. */
    now_vol = LCDC.VLCD.BIT.VLCD;
    new_vol = (int8_t)(now_vol + updown_level);
    
    if ((LCDC_BIAS_2 == LCDC_CFG_BIAS) || (LCDC_BIAS_3 == LCDC_CFG_BIAS))
    {
        if((new_vol < LCDC_VOL_MIN) || (LCDC_VOL_MAX < new_vol))
        {
            R_BSP_NOP();
            return LCDC_ERR_OUTSIDE_VOLTAGE;
        }
    }
    else if (LCDC_BIAS_4 == LCDC_CFG_BIAS)
    {
        if((new_vol < LCDC_VOL_MIN) || (LCDC_VOL_MAX_4BIAS < new_vol))
        {
            return LCDC_ERR_OUTSIDE_VOLTAGE;
        }
    }
    else
    {
        ret = LCDC_ERR_OTHER;
    }

    /* Stop the internal voltage boost/capacitor split circuit. */
    R_LCDC_OperateCircuit(LCDC_CIRCUIT_DISABLE);
    
    /* Change the voltage level according to he specified value. */
    LCDC.VLCD.BYTE = (uint8_t)new_vol;

    if((uint8_t)new_vol == LCDC.VLCD.BYTE)
    {
        R_BSP_NOP();
    }

    /* Wait for the reference voltage setup wait time. */
    lcdc_delay_us(LCDC_SETUP_WAIT);
    
    /* Enable the internal voltage boost/capacitor spilit circuit. */
    R_LCDC_OperateCircuit(LCDC_CIRCUIT_ENABLE);

    ret = new_vol;
    
    return ret;
}
#endif

/*****************************************************************************
* Function Name: R_LCDC_Close
* Description  : The function stop the LCDC.
* Arguments    : void
* Return Value : LCDC_SUCCESS      ; Processing completed successfully
******************************************************************************/
lcdc_err_t R_LCDC_Close(void)
{    
    /* The valuable for storing a return value for lock function. */
    bool chk;

    /* ---- Calls the API function. ---- */
    lcdc_close();

    /* ---- Hardware Unlock ---- */
    chk = R_BSP_HardwareUnlock(BSP_LOCK_LCDC);

    if (false == chk)
    {
        return  LCDC_ERR_NOT_CLOSED;
    }
    
    /* ---- Enters the LCDC module-stop state. ---- */
    power_off();

    return LCDC_SUCCESS;
}

/*---------------------------------------------------------------------------*/
static void lcdc_close(void)
{
    uint8_t cnt;

    /* WAIT_LOOP */
    for (cnt=0; cnt < 40; cnt++)
    {
        R_LCDC_Write(cnt,0x00);
    }

    /* ---- Initial register value ---- */
    /* Initialize the voltage boosting pin initial value switching control bit. */
    LCDC.LCDM1.BIT.LCDVLM = LCDC_INIT_LCDVLM;

    /* Initialize waveform bit.(Waveform A) */
    LCDC.LCDM0.BIT.LWAVE = LCDC_INIT_WAVE;
    
    /* Initialize time slices bit. (Static) */
    LCDC.LCDM0.BIT.LDTY = LCDC_INIT_SLICE;

    /* Initialize the bias method select bit. (1/2 bias method) */
    LCDC.LCDM0.BIT.LBAS = LCDC_INIT_BIAS;

    /* Initialize the LCDC driver voltage generator select bit. (External resistance division method) */
    LCDC.LCDM0.BIT.MDSET = LCDC_INIT_MDSET;

    /* Initialize the LCDC display. (Output ground level to segment/common pin) */
    LCDC.LCDM1.BIT.SCOC = LCDC_INIT_SCOC;

    /* Initialize LCD boost level control register. */
#if (LCDC_VOL_INTERNAL == LCDC_CFG_DRV_GENERATOR)
    LCDC.VLCD.BYTE = LCDC_INIT_VOL;

    if(LCDC_INIT_VOL == LCDC.VLCD.BYTE)
    {
        R_BSP_NOP();
    }

/* Wait for the reference voltage setup wait time. */
    lcdc_delay_us(LCDC_SETUP_WAIT);
#endif

    /* ---- Initialize the valuable ---- */
    g_lcdc_vol_method_flg = LCDC_VOL_METHOD_INTERNAL;
    g_lcdc_open_flg = LCDC_NOT_OPEN;

    /* Reset div for lcdc */
    LCDC.LCDC0.BIT.LCDC0 = LCDC_INIT_DIV;

    /* Reset lcdc clock source */
    SYSTEM.LCDSCLKCR.BIT.LCDSCLKSEL = LCDC_INIT_LCDCLK; 

    /* Stop output of lcdc clock source */
    SYSTEM.LCDSCLKCR2.BIT.LCDSCLKSTP = LCDC_INIT_CLKSTP;

}


/*****************************************************************************
* Function Name: R_LCDC_GetVersion
* Description  : Returns the version of this module. The version number is 
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
******************************************************************************/
uint32_t  R_LCDC_GetVersion(void)
{
    uint32_t const version = ((LCDC_VERSION_MAJOR << 16) | LCDC_VERSION_MINOR);

    return version;
}


/************************************************************************************
* Function Name: power_on
* Description : Turns on power to a LCDC.
* Arguments : none
* Return Value : none
************************************************************************************/
static void power_on(void)
{
    /* Enable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Bring module out of stop state. */
    MSTP(LCDC) = 0;
    
    /* Disable writing to MSTP registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
}

/************************************************************************************
* Function Name: power_off
* Description : Turns off power to a LCDC channel.
* Arguments : none
* Return Value : none
************************************************************************************/
static void power_off(void)
{
    /* Enable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Put module in stop state. */
    MSTP(LCDC) = 1;
   
    /* Disable writing to MSTP registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
}

/*******************************************************************************
* Outline      : WAIT processing which can be performed in a fixed cycle
* Header       : none
* Function Name: R_DELAY
* Description  : The function waits until the arbitrary cycles shown in R1 pass.
* Arguments    : R1 : Waiting loop counter
* Return Value : none
*******************************************************************************/
R_BSP_PRAGMA_STATIC_INLINE_ASM(lcdc_delay)
void lcdc_delay  (uint32_t loop_cnt)
{
    R_BSP_ASM_INTERNAL_USED(loop_cnt)
    R_BSP_ASM_BEGIN
    R_BSP_ASM(    BRA.B   R_BSP_ASM_LAB_NEXT(0)    )
    R_BSP_ASM(    NOP                              )
    R_BSP_ASM_LAB(0:                               )
    R_BSP_ASM(    NOP                              )
    R_BSP_ASM(    SUB     #01H,R1                  )
    R_BSP_ASM(    BNE.B   R_BSP_ASM_LAB_PREV(0)    )
    R_BSP_ASM_END
}

/*******************************************************************************
* Outline      : WAIT processing which can be performed in a fixed cycle
               : (for several us)
* Header       : none
* Function Name: R_DELAY_Us
* Description  : The function performed in the fixed cycle is called and
               : It waits for the WAIT time for several us.
* Arguments    : R1 : Waiting time(us)
* Return Value : LCDC_SUCCESS         : Success
               : LCDC_INVALID_ARG : Invalid argument
*******************************************************************************/
lcdc_err_t lcdc_delay_us (uint32_t delay_us)
{


    uint32_t loop_cnt; /* Argument of R_DELAY function */

    /* Calculation of a loop count */

    loop_cnt = (uint32_t)(delay_us * (uint32_t)(((((BSP_ICLK_HZ / 1000.0) / (5.0 * 1000)) * 1024 )) + 0.9));
    loop_cnt >>= 10UL;

    /* R_DELAY function is performed when loop_cnt is 1 or more. */

    if(loop_cnt != 0)
    {

        lcdc_delay((uint32_t)loop_cnt);

    }
#if LCDC_CFG_PARAM_CHECKING_ENABLE  == 1

    else
    {
        return LCDC_ERR_INVALID_ARG;
    }

#endif

    return LCDC_SUCCESS;

}


/* end of file */
