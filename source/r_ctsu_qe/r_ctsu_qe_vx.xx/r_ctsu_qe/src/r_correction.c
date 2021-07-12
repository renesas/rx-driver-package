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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_correction.c
* Description  : This file contains functions necessary to "correct" silicon variations in current measurement.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              :            0.82    Shinji Takeda: Line 667- Change 1000000 to 1024*1024.
*              : 04.10.2018 1.00    First Release
*              : 29.04.2019 1.10    Added error return codes to correction_CTSU_sensor_ico()
*                                     and correction_CTSU_measurement_start().
*                                   Added error checking in correction_CTSU_register_txd_set() switch default.
*                                   Added #pragma sections and ported to GCC/IAR.
*                                   Modified CTSUInterrupt() to
*                                     -load g_pvt_scan_info[] with status
*                                     -also not call callback when touch is performing offset tuning
*                                     -call custom callback if configured
*                                     -reset data_update flag on err in case using ext trig and Touch did not process
*                                       last scan yet
*              : 09.01.2020 1.11    Replaced correction_CTSU_register_txd_set() switch statement with if-else statement
*                                     to fix RX231 13.5 PLL multiplier compile-time error.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"
#include "r_ctsu_qe_if.h"
#include "r_ctsu_qe_private.h"
#include "qe_common.h"


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define CTSU_PRV_0_NORMAL               (0)
#define CTSU_PRV_1_CORRECTION           (1)

#define CTSU_PRV_0_1ST                  (0)
#define CTSU_PRV_1_2ND                  (1)

#define CTSU_PRV_CORRECTION_AVERAGE     (32)        /* Correction measurement average times    */

#define CTSU_PRV_SENS_OK                (0x00)
#define CTSU_PRV_SENS_OVER              (0x01)
#define CTSU_PRV_REF_OVER               (0x02)
#define CTSU_PRV_SENS_REF_OVER          (0x03)

#define CTSU_PRV_WAFER_PARAMETER        (0.96523525)


/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
* Exported global variables
***********************************************************************************************************************/
/* for ctsu driver use */
uint16_t    g_correction_dtc_txd[3];           /* CTSU measurement parameter         */
uint8_t     g_correction_mode;                 /* Sensor correction mode flag        */

/* for Touch layer use */

#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_SAFETY_RAM
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(B, _QE_TOUCH_SAFETY_RAM, 2)
#endif
uint16_t    g_linear_interpolation_table[61];
uint16_t    g_linear_interpolation_coefficient[61];
#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE_END
#endif


/***********************************************************************************************************************
External global variables and functions
***********************************************************************************************************************/


/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
static uint16_t                g_correction_1st_coefficient;      /* 1st correction coefficient         */
static volatile ctsu_status_t  g_correction_status;               /* CTSU correction status flag        */
static uint32_t                g_correction_2nd_val;              /* 2nd standard correction value      */
static uint8_t                 g_correction_time;                 /* CTSU correction measurement flag   */
static uint16_t                g_correction_16point[16];          /* 16 point correction sensor data    */
static uint16_t                g_correction_16table[16];          /* 16 point correction table          */
static uint16_t                g_correction_16coefficient[16];    /* 16 point correction coefficient    */
static uint32_t                g_correction_1st_val;              /* 1st standard correction value      */
static uint16_t                g_correction_2nd_std_val;
static uint8_t                 g_correct_value_measure;
static uint16_t                g_correction_2nd_coefficient;      /* 2nd correction coefficient         */
static volatile ctsu_status_t  g_correction_status;               /* CTSU correction status flag        */



#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_SAFETY_CONSTANT
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(C, _QE_TOUCH_SAFETY_CONSTANT, 2)
#endif
static const uint16_t g_ctsu_correction_std[] =
{
    QE_19_2UA, QE_18_0UA, QE_16_8UA, QE_15_6UA, QE_14_4UA, QE_13_2UA, QE_12_0UA, QE_10_8UA,
    QE_09_6UA, QE_08_4UA, QE_07_2UA, QE_06_0UA, QE_04_8UA, QE_03_6UA, QE_02_4UA, QE_01_2UA
};
#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE_END
#endif


#if (CTSU_HAS_TRIMMER_REG)
static uint8_t g_ctsutrimr_def;
#endif

static void    configure_correction_method(void);
static qe_err_t correction_CTSU_register_txd_set(void);
static void    R_Set_CTSU_Correction_Mode(uint8_t mode);
static qe_err_t correction_CTSU_measurement_start(void);
static void    correction_CTSU_1st_coefficient_create(void);
static void    correction_CTSU_2nd_standard_val_create(void);
static void    correction_CTSU_2nd_coefficient_create(void);
static void    correction_CTSU_16point_coefficient_create(void);
static void    sensor_linear_interpolation(void);
static uint8_t CTSUGetTscapVoltageError(void);
static void    R_Set_CTSU_Power_Supply_Only(uint8_t mode);
static uint8_t R_Get_CTSU_Counter_Overflow_flag(void);


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: correction_CTSU_sensor_ico
* Description  : Sensor ICO gain correction initial setting.
* Arguments    : None
* Return Value : QE_SUCCESS -
*                   Correction completed successfully
*                QE_ERR_ABNORMAL_TSCAP -
*                   TSCAP error detected during correction
*                QE_ERR_SENSOR_OVERFLOW -
*                   Sensor overflow error detected during correction
*                QE_ERR_SENSOR_SATURATION -
*                   Initial sensor value beyond linear portion of correction curve
*                QE_ERR_UNSUPPORTED_CLK_CFG -
*                   No correction parameters available for selected clock frequency
***********************************************************************************************************************/
qe_err_t correction_CTSU_sensor_ico( void )
{
    qe_err_t    err=QE_SUCCESS;

    g_correction_mode  = CTSU_PRV_1_CORRECTION;                            /* Sensor ICO correction measurement mode setting  */
    g_correction_time  = CTSU_PRV_0_1ST;                                   /* 1st measurement flag setting                    */

#if (CTSU_HAS_TRIMMER_REG)
    g_ctsutrimr_def    = CTSU.CTSUTRMR;                            /* Current trimming value storage                  */
#endif


    while (0 != CTSU.CTSUCR0.BIT.CTSUSTRT)
    {
        ;  /* Measurement finish wait */
    }

    configure_correction_method();

    while (CTSU_PRV_1_CORRECTION == g_correction_mode)
    {
        err = correction_CTSU_register_txd_set();              /* CTSU correction parameter setting by DTC.       */
        if (QE_SUCCESS != err)
        {
            break;
        }

        R_Set_CTSU_Correction_Mode(CTSU_PRV_1_CORRECTION);     /* Sensor ICO correction measurement setting       */
        g_ctsu_ctrl.state = CTSU_STATE_READY;                  /* CTSU measurement ready mode setting             */
        err = correction_CTSU_measurement_start();             /* Sensor ICO gain correction measurement start    */
        if (QE_SUCCESS != err)
        {
            break;
        }

        if (CTSU_PRV_0_1ST == g_correction_time)
        {
            correction_CTSU_1st_coefficient_create();          /* 1st coefficient create                          */
            correction_CTSU_2nd_standard_val_create();         /* 2nd standard value create                       */
            g_correction_time = CTSU_PRV_1_2ND;                /* 2nd measurement flag setting                    */
#if (CTSU_HAS_TRIMMER_REG)
            CTSU.CTSUTRMR = 0xFF;                              /* 0xFF set in the current trimming register       */
#endif
        }
        else
        {
            correction_CTSU_2nd_coefficient_create();          /* 2nd coefficient create                          */
            correction_CTSU_16point_coefficient_create();      /* 16point coefficient create                      */
#if (CTSU_HAS_TRIMMER_REG)
            CTSU.CTSUTRMR = g_ctsutrimr_def;                   /* Return the current trimming register to the initial value */
#endif
            R_Set_CTSU_Correction_Mode(CTSU_PRV_0_NORMAL);     /* Sensor ICO normal measurement setting            */
            g_correction_mode = CTSU_PRV_0_NORMAL;             /* Sensor ICO normal measurement mode setting      */
        }
    }

    return err;
}    /* End of function correction_CTSU_sensor_ico() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: configure_correction_method
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void configure_correction_method(void)
{

    g_ctsu_ctrl.mode = CTSU_MODE_SELF_MULTI_SCAN;

    CTSU.CTSUCR1.BYTE = 0x43;
    while (1 != CTSU.CTSUCR1.BIT.CTSUPON)                       /* Wait for charging to stabilize */
    {
        ;
    }

    CTSU.CTSUSDPRS.BYTE = 0x23;
    CTSU.CTSUSST.BYTE = 0x10;   // always
    CTSU.CTSUDCLKC.BYTE = 0x30; // always

    /* Enable lowest configured channel */
    if (0 != gp_cur_cfg->ctsuchac0)
    {
        CTSU.CTSUCHAC0.BYTE = gp_cur_cfg->ctsuchac0 & (-gp_cur_cfg->ctsuchac0);
    }
    else if (0 != gp_cur_cfg->ctsuchac1)
    {
        CTSU.CTSUCHAC1.BYTE = gp_cur_cfg->ctsuchac1 & (-gp_cur_cfg->ctsuchac1);
    }
#if (CTSU_HAS_LARGE_REG_SET)
    else if (0 != gp_cur_cfg->ctsuchac2)
    {
        CTSU.CTSUCHAC2.BYTE = gp_cur_cfg->ctsuchac2 & (-gp_cur_cfg->ctsuchac2);
    }
    else if (0 != gp_cur_cfg->ctsuchac3)
    {
        CTSU.CTSUCHAC3.BYTE = gp_cur_cfg->ctsuchac3 & (-gp_cur_cfg->ctsuchac3);
    }
    else
    {
        CTSU.CTSUCHAC4.BYTE = gp_cur_cfg->ctsuchac4 & (-gp_cur_cfg->ctsuchac4);
    }
#endif

#if (CTSU_HAS_TXVSEL)
    CTSU.CTSUCR0.BIT.CTSUTXVSEL = 0;    // in SELF mode; cannot equal 1
#endif

    /* Initialize CTSU hardware control block */
    CTSU.CTSUCR0.BIT.CTSUINIT = 1;

    return;
} /* End of function configure_correction_method() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_Set_CTSU_Correction_Mode
* Description  :
* Arguments    : uint8_t mode    : 0 Normal mode    1 Test mode
* Return Value : none
***********************************************************************************************************************/
static void R_Set_CTSU_Correction_Mode( uint8_t mode )
{
    if (0 == SYSTEM.MSTPCRD.BIT.MSTPD10)
    {
        if (0 == CTSU.CTSUCR0.BIT.CTSUSTRT)
        {
            /***** CTSU Channel Transmit/Receive Control Register 0 (CTSUCHTRC0) **************************************/
            /* bit15   TSCAP Voltage Error Flag                                                                       */
            /* bit14-8 Reserved                                                                                       */
            /* bit7    CTSU Test Mode Select                                                                          */
            /* bit6-2  Reserved                                                                                       */
            /* bit1-0  CTSU Reference Current Test Mode (VDC Stop)                                                    */
            /**********************************************************************************************************/
            if (0 == mode)
            {
                CTSU.CTSUERRS.BIT.CTSUTSOC = 0;                       /* Bit7(CTSUTSOC)   setting                     */
                CTSU.CTSUERRS.BIT.CTSUSPMD = 0;                       /* Bit1,0(CTSUSPMD) setting                     */
            }
            else
            {
                CTSU.CTSUERRS.BIT.CTSUTSOC = 1;                       /* Bit7(CTSUTSOC)   setting                     */
                CTSU.CTSUERRS.BIT.CTSUSPMD = 2;                       /* Bit1,0(CTSUSPMD) setting                     */
            }
        }
    }
}    /* End of function R_Set_CTSU_Correction_Mode() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: correction_CTSU_register_txd_set
* Description  : CTSU correction parameters transmit setting by DTC.
* Arguments    : None
* Return Value : QE_SUCCESS-
*                   Correction parameter set.
*                QE_ERR_UNSUPPORTED_CLK_CFG-
*                   No correction parameter available for clock speed.
***********************************************************************************************************************/
static qe_err_t correction_CTSU_register_txd_set( void )
{
    qe_err_t    err = QE_SUCCESS;


    if (0 == g_correct_value_measure)
    {
        g_correction_dtc_txd[0] = 0x0700;
        g_correction_dtc_txd[1] = 0x0000;

        if (CTSU_INPUT_FREQUENCY_DIV == 32)
        {
            g_correction_dtc_txd[2] = 0x3F0F;       /* 32 / 64 = 0.5MHz              */
        }
        else if (CTSU_INPUT_FREQUENCY_DIV == 27)
        {
            g_correction_dtc_txd[2] = 0x3A0F;       /* 27 / 54 = 0.5MHz              */
        }
        else if (CTSU_INPUT_FREQUENCY_DIV == 24)
        {
            g_correction_dtc_txd[2] = 0x370F;       /* 24 / 48 = 0.5MHz              */
        }
        else if (CTSU_INPUT_FREQUENCY_DIV == 16)
        {
            g_correction_dtc_txd[2] = 0x2F0F;       /* 16 / 32 = 0.5MHz              */
        }
        else if (CTSU_INPUT_FREQUENCY_DIV == 8)
        {
            g_correction_dtc_txd[2] = 0x270F;       /*  8 / 16 = 0.5MHz              */
        }
        else
        {
            err = QE_ERR_UNSUPPORTED_CLK_CFG;
        }

#ifdef BSP_MCU_RX23_ALL
        if(CTSU_PRV_1_2ND == g_correction_time)
        {
            g_correction_dtc_txd[2] = g_correction_dtc_txd[2] | 0x6000;              /* ICO gain  66% -> 40%          */
        }
#endif
    }

    return err;
}    /* End of function correction_CTSU_register_txd_set() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: correction_CTSU_measurement_start
* Description  : Sensor ICO gain correction measurement start.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static qe_err_t correction_CTSU_measurement_start( void )
{
    uint8_t     measure_count;
    uint32_t    sensor_ico_add;
    qe_err_t    err = QE_SUCCESS;

    measure_count  = 0;
    sensor_ico_add = 0;

    while (CTSU_PRV_CORRECTION_AVERAGE != measure_count)                /* Average number of times loop */
    {
        while (CTSU_STATE_READY == g_ctsu_ctrl.state)
        {
            g_ctsu_ctrl.state = CTSU_STATE_RUN;                         /* CTSU measurement run mode setting */

            /* WARNING! Do NOT call R_CTSU_StartScan(). That is for application layer.
             * It may be using external triggers or the DTC for non-correction registers values.
             */
            CTSU.CTSUCR0.BIT.CTSUSTRT = 1;      // Start scan
        }

        if (CTSU_STATE_FINISH == g_ctsu_ctrl.state)
        {
            if (1 == g_correction_status.flag.data_update)
            {
                sensor_ico_add = sensor_ico_add + gp_cur_cfg->p_scan_buf[0];
                measure_count  = measure_count + 1;                     /* Measurement count up */

                g_correction_status.flag.data_update = 0;               /* Update flag clear */
                g_ctsu_ctrl.state = CTSU_STATE_READY;                   /* CTSU measurement run mode setting */
            }
            else if (1 == g_correction_status.flag.icomp_error)
            {
                return QE_ERR_ABNORMAL_TSCAP;
            }
            else if (1 == g_correction_status.flag.sens_over)
            {
                return QE_ERR_SENSOR_OVERFLOW;
            }
        }
    }

    if (CTSU_PRV_0_1ST == g_correction_time)                            /* 1st measurement judgment */
    {
        g_correction_1st_val = sensor_ico_add / CTSU_PRV_CORRECTION_AVERAGE;   /* get average */
        sensor_ico_add       = 0;                                       /* Sensor ico addition data clear */
        if (g_correction_1st_val >= CTSU_SENSOR_SATURATION)
        {
            err = QE_ERR_SENSOR_SATURATION;
        }
    }
    else
    {
        g_correction_2nd_val = sensor_ico_add / CTSU_PRV_CORRECTION_AVERAGE;   /* get average */
    }

    return err;
}    /* End of function correction_CTSU_measurement_start() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: CTSUInterrupt
* Description  : CTSU measurement end interrupt function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void CTSUInterrupt( void )
{
    uint8_t         err_status;
    ctsu_status_t   status;
    ctsu_isr_evt_t  event;


    status = gp_cur_cfg->ctsu_status;
    g_pvt_scan_info[g_ctsu_ctrl.cur_method].ctsuerrs = CTSU.CTSUERRS.WORD;
    g_pvt_scan_info[g_ctsu_ctrl.cur_method].ctsust = CTSU.CTSUST.BYTE;

    if (0 != CTSUGetTscapVoltageError())
    {
        if (CTSU_PRV_1_CORRECTION == g_correction_mode)
        {
            g_correction_status.flag.icomp_error = 1;   /* TSCAP voltage error */
        }
        else
        {
            status.flag.icomp_error = 1;                /* TSCAP voltage error */
        }
        R_Set_CTSU_Power_Supply_Only(0x00);             /* TSCUPON off -> TSCUICOMP clear */
#if (!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5))
        nop();
        nop();
#else
        R_BSP_NOP();
        R_BSP_NOP();
#endif
        R_Set_CTSU_Power_Supply_Only(0x01);             /* TSCUPON on */
    }
    else
    {
        if (CTSU_PRV_1_CORRECTION == g_correction_mode)
        {
            g_correction_status.flag.icomp_error = 0;   /* TSCAP voltage error clear */
        }
        else
        {
            status.flag.icomp_error = 0;                /* TSCAP voltage error clear */
        }
    }

    err_status = R_Get_CTSU_Counter_Overflow_flag();
    CTSU.CTSUST.BYTE &= 0x9F;                           /* clear possible overflow flags */

    if (CTSU_PRV_1_CORRECTION == g_correction_mode)
    {
        switch (err_status)
        {
            case CTSU_PRV_SENS_OK:
                g_correction_status.flag.sens_over = 0;
                g_correction_status.flag.ref_over = 0;
                if (1 != g_correction_status.flag.icomp_error)  /* TSCAP voltage error */
                {
                    g_correction_status.flag.data_update = 1;   /* Measurement data update status set */
                }
                g_ctsu_ctrl.state = CTSU_STATE_FINISH;
                g_correction_status.flag.ctsu_measure = 1;
                break;

            case CTSU_PRV_SENS_OVER:
                g_correction_status.flag.ref_over = 0;
                g_correction_status.flag.sens_over = 1;         /* Sensor Counter overflow status set */
                g_ctsu_ctrl.state = CTSU_STATE_FINISH;
                break;

            default:
                g_ctsu_ctrl.state = CTSU_STATE_STOP;
                break;
        }
    }
    else
    {
        switch (err_status)
        {
            case CTSU_PRV_SENS_OK:
                status.flag.sens_over = 0;
                status.flag.ref_over = 0;
                if (1 != status.flag.icomp_error)         /* TSCAP voltage error */
                {
                    status.flag.data_update = 1;          /* Measurement data update status set */
                    event = CTSU_EVT_SCAN_COMPLETE;
                }
                else
                {
                    event = CTSU_EVT_VOLT_ERR;
                }
                g_ctsu_ctrl.state = CTSU_STATE_READY;
                status.flag.ctsu_measure = 1;
                break;

            case CTSU_PRV_SENS_OVER:
                status.flag.ref_over = 0;
                status.flag.sens_over = 1;                /* Sensor Counter overflow status set     */
                status.flag.data_update = 0;    // clear in case using ext trig and Touch did not process last scan
                event = CTSU_EVT_SENSOR_OVERFLOW;
                g_ctsu_ctrl.state = CTSU_STATE_READY;
                break;

            case CTSU_PRV_REF_OVER:
                status.flag.sens_over = 0;
                status.flag.ref_over = 1;                 /* Reference Counter overflow status set  */
                status.flag.data_update = 0;    // clear in case using ext trig and Touch did not process last scan
                event = CTSU_EVT_SENSOR_OVERFLOW;
                g_ctsu_ctrl.state = CTSU_STATE_READY;
                break;

            case CTSU_PRV_SENS_REF_OVER:
                status.flag.sens_over = 1;                /* Sensor Counter overflow status set     */
                status.flag.ref_over = 1;                 /* Reference Counter overflow status set  */
                status.flag.data_update = 0;    // clear in case using ext trig and Touch did not process last scan
                event = CTSU_EVT_OVERFLOW;
                g_ctsu_ctrl.state = CTSU_STATE_READY;
                break;

            default:
                g_ctsu_ctrl.state = CTSU_STATE_STOP;
                break;
        }
    }

    gp_cur_cfg->ctsu_status = status;

    /* if not doing correction or offset tuning, do callback as appropriate */
    if ((CTSU_PRV_1_CORRECTION != g_correction_mode) && (1 != gp_cur_cfg->ctsu_status.flag.offset_tuning))
    {
        /* if power user created special callback, call that
         * if using external triggers, call default callback to notify user that scan is done
         * no callback is made by default for sw triggers because that is done by the timer interrupt
         */
        if (NULL != g_ctsu_ctrl.p_callback)
        {
            g_ctsu_ctrl.p_callback(event, &g_ctsu_ctrl.cur_method);
        }
        else if (QE_TRIG_EXTERNAL == g_ctsu_ctrl.trigger)
        {
            qetouch_timer_callback(NULL);
        }
    }


}    /* End of function CTSUInterrupt() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: CTSUGetTscapVoltageError
* Description  : This API reads the value in [CTSU Error Status Register (CTSUERRS)].
* Arguments    : None
* Return Value : data
***********************************************************************************************************************/
static uint8_t CTSUGetTscapVoltageError( void )
{
    /* CTSU Error Status Register (CTSUERRS)
    b15        CTSU TSCAP voltage error
    b14-0    Reserved    */
    return(CTSU.CTSUERRS.BIT.CTSUICOMP);                    //    CTSU Error Status Register (CTSUICOMP)

} /* End of function CTSUGetTscapVoltageError() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_Set_CTSU_Power_Supply_Only
* Description  : This API switches  power of AFE (Analog Front End).
* Arguments    : mode
* Return Value : None
***********************************************************************************************************************/
static void R_Set_CTSU_Power_Supply_Only( uint8_t mode )
{
    volatile uint8_t data;

    if (0 == SYSTEM.MSTPCRD.BIT.MSTPD10)
    {
        if (0 == CTSU.CTSUCR0.BIT.CTSUSTRT)
        {
            /***** CTSU Control Register 1 (CTSUCR1) ******************************************************************/
            /* bit7-6  CTSU Measurement Mode Select                                                                   */
            /* bit5-4  CTSU Operating Clock Select                                                                    */
            /* bit3    CTSU Power Supply Capacity Adjustment                                                          */
            /* bit2    CTSU Power Supply Operating Mode Setting                                                       */
            /* bit1    CTSU LPF Capacitance Charging Control                                                          */
            /* bit0    CTSU Power Supply Enable                                                                       */
            /**********************************************************************************************************/
            if( 1 == mode )
            {
                data              = CTSU.CTSUCR1.BYTE;       /* CTSU Control Register 1 (CTSUCR0) storage             */
                data              = data | 0x01;             /* set Power Supply on(bit0)                             */
                CTSU.CTSUCR1.BYTE = data;                    /* set  CTSU Control Register 1 (CTSUCR1)                */
            }
            else
            {
                data              = CTSU.CTSUCR1.BYTE;       /* CTSU Control Register 1 (CTSUCR0) storage             */
                data              = data & 0xFE;             /* set Power Supply off(bit0)                            */
                CTSU.CTSUCR1.BYTE = data;                    /* set  CTSU Control Register 1 (CTSUCR1)                */
            }
        }
    }
} /* End of function R_Set_CTSU_Power_Supply_Only() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_Get_CTSU_Counter_Overflow_flag
* Description  :
* Arguments    : None
* Return Value : uint8_t flag    bit1 CTSU Reference Counter Overflow Flag    0 : No overflow
*              :                                                              1 : An overflow
*              :                 bit0 CTSU Sensor Counter Overflow Flag       0 : No overflow
*              :                                                              1 : An overflow
***********************************************************************************************************************/
static uint8_t R_Get_CTSU_Counter_Overflow_flag( void )
{
    uint8_t flag;

    /***** CTSU Channel Transmit/Receive Control Register 1 (CTSUCHTRC1) **********************************************/
    /* bit7    CTSU Mutual Capacitance Status Flag                                                                    */
    /* bit6    CTSU Reference Counter Overflow Flag                                                                   */
    /* bit5    CTSU Sensor Counter Overflow Flag                                                                      */
    /* bit4    CTSU Data Transfer Status Flag                                                                         */
    /* bit3    Reserved                                                                                               */
    /* bit2-0  CTSU Measurement Status Counter                                                                        */
    /******************************************************************************************************************/
    flag = CTSU.CTSUST.BYTE >> 5;                      /* CTSUST Register value 5 right sift reading data storage     */
    flag = flag & 0x03;                                /* bit1,bit0 without mask                                      */
    return flag;
}    /* End of function R_Get_CTSU_Counter_Overflow_flag() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: correction_CTSU_1st_coefficient_create
* Description  :
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void correction_CTSU_1st_coefficient_create( void)
{
    /*                                      (g_correction_1st_val * 1024)       g_correction_1st_val +20              */
    /*    g_correction_1st_coefficient  =  -------------------------------  =  --------------------------             */
    /*                                                     40960                            40                        */

    g_correction_1st_coefficient = (g_correction_1st_val + 20 ) / 40;
}    /* End of function correction_CTSU_1st_coefficient_create() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: correction_CTSU_2nd_standard_val_create
* Description  :
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void correction_CTSU_2nd_standard_val_create( void )
{
#ifdef BSP_MCU_RX23_ALL

    /* convert float to word */
    g_correction_2nd_std_val = (uint16_t)(24824 * CTSU_PRV_WAFER_PARAMETER);        /* ICOG = 66% : 40960, ICOG = 40% : 24824  */
#else
    uint32_t _1st_parm;

    /*                                  g_ctsutrimr_def + 273                    (g_ctsutrimr_def + 273) * 2560 *128  */
    /*    g_correction_2nd_std_val  =  ----------------------- * 40960 *128  =  ------------------------------------- */
    /*                                           528                                               33                 */

    _1st_parm = (g_ctsutrimr_def + 273) * 9930 + 64;
    g_correction_2nd_std_val = (uint16_t)(_1st_parm >> 7);
#endif
}    /* End of function correction_CTSU_2nd_standard_val_create() */


#if (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
#if (!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5))
#pragma section _QE_TOUCH_DRIVER
#else
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
#endif
/***********************************************************************************************************************
* Function Name: correction_CTSU_2nd_coefficient_create
* Description  :
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void correction_CTSU_2nd_coefficient_create( void)
{
    /*                                        g_correction_2nd_val                                                    */
    /*    g_correction_2nd_coefficient  =  -------------------------- * 1024                                          */
    /*                                      g_correction_2nd_std_val                                                  */

    g_correction_2nd_coefficient = ((g_correction_2nd_val << 10) + (g_correction_2nd_std_val/2)) / g_correction_2nd_std_val;
}    /* End of function correction_CTSU_2nd_coefficient_create() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: correction_CTSU_16point_coefficient_create
* Description  :
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void correction_CTSU_16point_coefficient_create(void)
{
    uint16_t diff_coefficient;
    uint8_t  up_down;
    uint8_t  loop;
    uint8_t  loop_p;
    uint16_t fill_diff;
    uint32_t work_table;
    uint32_t work_32;
    uint16_t work_16;

    if (g_correction_2nd_coefficient > g_correction_1st_coefficient)
    {
        diff_coefficient = g_correction_2nd_coefficient - g_correction_1st_coefficient;
        up_down = 0;
    }
    else
    {
        diff_coefficient = g_correction_1st_coefficient - g_correction_2nd_coefficient;
        up_down = 1;
    }

    /*         diff_coefficient * 40960                                                                               */
    /*     ----------------------------------                                                                         */
    /*      40960 - g_correction_2nd_std_val                                                                          */
    work_32    = diff_coefficient * 40960;
    work_16    = 40960 - g_correction_2nd_std_val;
    fill_diff  = work_32 / work_16;

    /*                                                                 fill_diff * N                                  */
    /*     g_correction_16point  =  g_correction_1st_coefficient +(-) ---------------                                 */
    /*                                                                      16                                        */
    if (0 == up_down)
    {
        for (loop = 1; 16 > loop; loop++)
        {
            g_correction_16point[loop] = g_correction_1st_coefficient + ((fill_diff * loop) >> 4);
        }
    }
    else
    {
        for (loop = 1; 16 > loop; loop++)
        {
            g_correction_16point[loop] = g_correction_1st_coefficient - ((fill_diff * loop) >> 4);
        }
    }
    g_correction_16point[0] = g_correction_1st_coefficient;

    for (loop = 0, loop_p = 15; loop < 16; loop++)
    {
        work_table                       = g_ctsu_correction_std[loop] * g_correction_16point[loop];
        work_table                       = (work_table + 512)>> 10;

        if(0x0000FFFF < work_table)
        {
            work_table = 0x0000FFFF;
        }

        /* convert uint32_t to uint16_t */
        g_correction_16table[loop]       = (uint16_t)work_table;
        g_correction_16coefficient[loop] = (1024 * 1024) / g_correction_16point[loop_p];
        loop_p = loop_p - 1;
    }

    sensor_linear_interpolation();

}    /* End of function correction_CTSU_16point_coefficient_create() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: sensor_linear_interpolation
* Description  :
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void sensor_linear_interpolation(void)
{
    uint8_t     loop;
    uint8_t     bnum;
    uint16_t    table_diff;
    uint16_t    coefficient_diff;

    bnum = 0;

    if (g_correction_16coefficient[1] > g_correction_16coefficient[0])
    {
        for (loop = 0; loop < 15; loop++)
        {
            table_diff       = ((g_correction_16table[loop] - g_correction_16table[loop+1]) + 2) / 4;
            g_linear_interpolation_table[bnum]         = g_correction_16table[loop];
            g_linear_interpolation_table[bnum+1]       = g_correction_16table[loop] - table_diff;
            g_linear_interpolation_table[bnum+2]       = g_correction_16table[loop] - (table_diff * 2);
            g_linear_interpolation_table[bnum+3]       = g_correction_16table[loop] - (table_diff * 3);

            coefficient_diff = ((g_correction_16coefficient[loop+1] - g_correction_16coefficient[loop]) +2) / 4;
            g_linear_interpolation_coefficient[bnum]   = g_correction_16coefficient[loop];
            g_linear_interpolation_coefficient[bnum+1] = g_correction_16coefficient[loop] + coefficient_diff;
            g_linear_interpolation_coefficient[bnum+2] = g_correction_16coefficient[loop] + (coefficient_diff * 2);
            g_linear_interpolation_coefficient[bnum+3] = g_correction_16coefficient[loop] + (coefficient_diff * 3);
            bnum = bnum +4;
        }
    }
    else
    {
        for (loop = 0; loop < 15; loop++)
        {
            table_diff       = ((g_correction_16table[loop] - g_correction_16table[loop+1]) + 2) / 4;
            g_linear_interpolation_table[bnum]         = g_correction_16table[loop];
            g_linear_interpolation_table[bnum+1]       = g_correction_16table[loop] - table_diff;
            g_linear_interpolation_table[bnum+2]       = g_correction_16table[loop] - (table_diff * 2);
            g_linear_interpolation_table[bnum+3]       = g_correction_16table[loop] - (table_diff * 3);

            coefficient_diff = ((g_correction_16coefficient[loop] - g_correction_16coefficient[loop+1]) +2) / 4;
            g_linear_interpolation_coefficient[bnum]   = g_correction_16coefficient[loop];
            g_linear_interpolation_coefficient[bnum+1] = g_correction_16coefficient[loop] - coefficient_diff;
            g_linear_interpolation_coefficient[bnum+2] = g_correction_16coefficient[loop] - (coefficient_diff * 2);
            g_linear_interpolation_coefficient[bnum+3] = g_correction_16coefficient[loop] - (coefficient_diff * 3);
            bnum = bnum +4;
        }
    }

    g_linear_interpolation_table[bnum]       = g_correction_16table[loop];
    g_linear_interpolation_coefficient[bnum] = g_correction_16coefficient[loop];

} /* End of function sensor_linear_interpolation() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE_END
#endif
