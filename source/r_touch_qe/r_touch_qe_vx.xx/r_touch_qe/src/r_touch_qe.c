/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_qetouch_rx.c
* Description  : This module implements the QE TOUCH API
***********************************************************************************************************************/

/***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              :            0.99    Jim Page: Changed GAIN_100 to 1
*              : 04.10.2018 1.00    First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <string.h>
#include "platform.h"
#include "r_touch_qe_if.h"
#include "r_touch_qe_private.h"
#include "qe_common.h"
#include "r_ctsu_qe_if.h"
#include "r_monitor.h"


/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/
#define SET_SUCCESS             (0)
#define SET_ERROR               (1)

#define _0_SUCCESS              (0)
#define _1_ERROR                (1)

#define GAIN_100                (1)
#if (GAIN_100 == 1)
    #define TUNING_UPPER_LIMIT  (200)
    #define TUNING_LOWER_LIMIT  (150)
#else
    #define TUNING_UPPER_LIMIT  (150)
    #define TUNING_LOWER_LIMIT  (100)
#endif

#define OFFSET_CNT_PLUS         (2)
#define OFFSET_CNT_MINUS        (-2)
#define ADD_TIME                (4)

#define _0_BUTTON               (0)
#define _1_MATRIX               (1)
#define _2_SLIDER               (2)
#define _3_WHEEL                (3)

#define _00_GET_OK              0x00
#define _01_GET_NG              0x01
#define _02_SENS_OVER           0x02
#define _03_REF_OVER            0x03
#define _04_SENS_REF_OVER       0x04
#define _05_TSCAP_ERR           0x05
#define _06_TUNING_ERR          0x06

#define _0_NON_UPDATE           (0)
#define _1_UPDATE               (1)


/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
* Exported global variables
***********************************************************************************************************************/
self_ico_t      *g_self_ico_data_pt[QE_NUM_METHODS];       // ptr to ctsu scan buffer
self_ico_t      *g_self_raw_data_pt[QE_NUM_METHODS];       // ptr to copied ctsu scan buffer data
uint16_t        *g_self_ico_sensor_pt[QE_NUM_METHODS];     // ptr into ctsu scan buffer
uint16_t        *g_self_add_sensor_pt[QE_NUM_METHODS];     // ptr to a buffer for moving average calculation
uint16_t        *g_self_sensor_cnt_pt[QE_NUM_METHODS];     // ptr to another buffer for moving average calculation

mutual_ico_t    *g_mutual_ico_data_pt[QE_NUM_METHODS];
mutual_ico_t    *g_mutual_raw_data_pt[QE_NUM_METHODS];
uint16_t        *g_mutual_ico_pri_sensor_pt[QE_NUM_METHODS];
uint16_t        *g_mutual_ico_snd_sensor_pt[QE_NUM_METHODS];
uint16_t        *g_mutual_add_pri_sensor_pt[QE_NUM_METHODS];
uint16_t        *g_mutual_add_snd_sensor_pt[QE_NUM_METHODS];
uint16_t        *g_mutual_sensor_diff_pt[QE_NUM_METHODS];

int8_t          *g_current_sign_pt[QE_NUM_METHODS];

key_info_t          g_key_info[QE_NUM_METHODS];
touch_group_param_t g_touch_key_group[QE_NUM_METHODS];
calib_info_t        g_calib_info[QE_NUM_METHODS];
touch_tuning_t      g_touch_tuning_info[QE_NUM_METHODS];

touch_system_t      g_touch_system;
touch_func_flag_t   g_touch_function[QE_NUM_METHODS];
touch_result_t      g_touch_all_result[QE_NUM_METHODS];
uint16_t            g_offset_time[QE_NUM_METHODS];
uint8_t             g_current_offset_count[QE_NUM_METHODS];
touch_func_param_t  g_touch_paramter[QE_NUM_METHODS];
volatile uint8_t    g_data_tim;                     /* Calibration data storage times */

uint16_t        g_slider_data[QE_SLIDER_MAX_ELEMENTS];
uint16_t        g_wheel_data[QE_WHEEL_MAX_ELEMENTS];
uint8_t         g_method_stor=0;
uint8_t         g_open_num_methods;
touch_mlist_t   g_mlist;

touch_cfg_t     **gp_touch_configs; // ptr to array of touch configurations
sldr_ctrl_t     *gp_sliderInfo;
wheel_ctrl_t    *gp_wheelInfo;


/***********************************************************************************************************************
* Private global variables and functions
***********************************************************************************************************************/
static volatile uint8_t     g_ctsu_offset_mode;     /* Stabilization mode flag */
static uint8_t  g_correction_create;

static qe_trig_t g_trigger;
static bool     g_cycle_methods;
static uint8_t  g_set_method;

static uint8_t  R_Set_Cap_Touch_Offset_Timing(uint8_t method, uint16_t count);
static uint8_t  R_Get_Cap_Touch_Data_Check(uint8_t method);
static uint8_t  R_Get_Cap_Touch_Initial_Status(void);
static uint16_t R_Set_Cap_Touch_Result_Create(uint8_t method);
static uint8_t  R_Set_Cap_Touch_Initial_Tuning(uint8_t method);
static uint16_t R_Get_Cap_Touch_Sensor_Data(uint8_t method, uint8_t mode, uint8_t index_num);
static void     touch_data_moving_average(uint8_t method);
static void     correction_sensor_magnification_set(uint8_t method);
static uint8_t  initial_offset_tuning(uint8_t method, uint8_t number);
static uint16_t correction_sensor_cnt_create(uint8_t method, uint8_t ts_num, uint8_t number);
static uint16_t correction_pri_sensor_cnt_create(uint8_t method, uint8_t ts_num, uint8_t number);
static uint16_t correction_snd_sensor_cnt_create(uint8_t method, uint8_t ts_num, uint8_t number);
static qe_err_t update_data(void);


/***********************************************************************************************************************
* Function Name: R_TOUCH_Open
* Description  : This functions Open()s the CTSU, and initializes all data for the Touch layer.
* Arguments    : p_ctsu_cfgs -
*                    Pointer to array of method configurations for the CTSU driver
*                p_touch_cfgs -
*                    Pointer to array of method configurations for the Touch driver
*                num_methods -
*                    Number of methods in arrays
*                trigger -
*                    CTSU scan trigger type
* Return Value : QE_SUCCESS -
*                    API initialized successfully.
*                QE_ERR_NULL_PTR
*                    Missing argument pointer
*                QE_ERR_INVALID_ARG -
*                    Number of methods or trigger type is invalid.
*                QE_ERR_ALREADY_OPEN -
*                    CTSU already open.
*                QE_ERR_BUSY
*                    CTSU already open and busy running.
***********************************************************************************************************************/
qe_err_t R_TOUCH_Open(ctsu_cfg_t *p_ctsu_cfgs[], touch_cfg_t *p_touch_cfgs[], uint8_t num_methods, qe_trig_t trigger)
{
    qe_err_t    err=QE_SUCCESS;
    uint8_t     i;
    uint8_t     method;
#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((p_ctsu_cfgs == NULL) || (p_touch_cfgs == NULL))
    {
        return QE_ERR_NULL_PTR;
    }

    if ((num_methods > QE_NUM_METHODS) || (trigger >= QE_TRIG_END_ENUM))
    {
        return QE_ERR_INVALID_ARG;
    }

    /* Do not parameter check configuration contents generated by QE Tool. */
#endif

    g_open_num_methods = num_methods;
    g_trigger = trigger;


    /* NOTE: Based upon legacy R_Set_Cap_Touch_Create() */

    touch_parameter_address_set();
    ctsu_sensor_data_address_set();
#if (TOUCH_CFG_UPDATE_MONITOR == 1)
    monitor_init();
#endif

    err = R_CTSU_Open(p_ctsu_cfgs, num_methods, trigger);
    if (err == QE_SUCCESS)
    {
        g_correction_create = _0_SUCCESS;
        touch_parameter_set(p_ctsu_cfgs, p_touch_cfgs);
        g_ctsu_offset_mode = _1_START;      // start stabilization process
        R_Set_Cap_Touch_Offset_Timing(0, g_offset_time[0]);

        /* by default, set up to cycle through all methods, starting with method 0 */
        g_mlist.num_methods = num_methods;
        g_cycle_methods = true;
        for (i=0; i < num_methods; i++)
        {
            g_mlist.methods[i] = i;
        }

        if (num_methods == 1)
        {
            g_cycle_methods = false;
        }

        method = 0;
        g_method_stor = method;
        g_set_method = method;
        gp_touch_configs = p_touch_cfgs;
        gp_sliderInfo = p_touch_cfgs[method]->p_sliders;
        gp_wheelInfo = p_touch_cfgs[method]->p_wheels;

        g_ctsu_offset_mode = _1_START;
        for (i=0; i < num_methods; i++)
        {
            R_Set_Cap_Touch_Offset_Timing(i, g_offset_time[i]);
        }
    }
    else
    {
        g_correction_create = _1_ERROR;
    }

    return err;
}


/***********************************************************************************************************************
* Function Name: R_TOUCH_UpdateDataAndStartScan
* Description  : This function is used when software-trigger is specified at open.
*                It should be called each time a periodic timer expires.
*                The first several calls are used to establish a baseline for the sensors. Once that is complete,
*                normal processing of the data from the last scan occurs. If a different method should be run on
*                the next scan, that is set up as well, then the next scan is started.
* Arguments    : None
* Return Value : QE_SUCCESS -
*                    API initialized successfully.
*                QE_ERR_TRIGGER_TYPE -
*                    This function should not be called when using external triggers
*                QE_ERR_BUSY
*                    CTSU busy. Likely need more time between calling this function/starting scans.
***********************************************************************************************************************/
qe_err_t R_TOUCH_UpdateDataAndStartScan(void)
{
    qe_err_t    err=QE_SUCCESS;


    if (g_trigger == QE_TRIG_EXTERNAL)
    {
        return QE_ERR_TRIGGER_TYPE;
    }

    err = update_data();
    if (err != QE_ERR_BUSY)
    {
        R_CTSU_StartScan();
    }

    return err;
}


/***********************************************************************************************************************
* Function Name: R_TOUCH_UpdateData
* Description  : This function is used when external-trigger is specified at open.
*                It should be called each time a scan completes ("timer flag" automatically set).
*                The first several calls are used to establish a baseline for the sensors. Once that is complete,
*                normal processing of the data from the last scan occurs. If a different method should be run on
*                the next scan, that is set up as well.
* Arguments    : None
* Return Value : QE_SUCCESS -
*                    API initialized successfully.
*                QE_ERR_TRIGGER_TYPE -
*                    This function should not be called when using software triggers
*                QE_ERR_BUSY
*                    CTSU busy. Likely need more time between calling this function/starting scans.
***********************************************************************************************************************/
qe_err_t R_TOUCH_UpdateData(void)
{
    qe_err_t    err=QE_SUCCESS;


    if (g_trigger == QE_TRIG_SOFTWARE)
    {
        return QE_ERR_TRIGGER_TYPE;
    }

    err = update_data();

    return err;
}


/***********************************************************************************************************************
* Function Name: update_data
* Description  : The first several calls to this function establish a baseline for the sensors in the system. Once that
*                is complete, this function 1) Determines if any buttons, sliders, or wheels were touched and calculates
*                results, and 2) Determines if the method needs to be changed before the next scan starts, either because
*                methods are being cycled through or a TOUCH_CMD_SET_METHOD command changed the desired method.
* Arguments    : None
* Return Value : QE_SUCCESS -
*                    API initialized successfully.
*                QE_ERR_BUSY
*                    CTSU busy. Likely need more time between calling this function/starting scans.
***********************************************************************************************************************/
qe_err_t update_data(void)
{
    qe_err_t        err=QE_SUCCESS;
    ctsu_status_t   ctsu_status;
    ctsu_state_t    ctsu_state;
    bool            chg_method=false;
    uint8_t         stabilization;


    /* Based upon legacy R_Set_Cap_Touch_Measurement_Start() */

    R_CTSU_Control(CTSU_CMD_GET_STATE, g_method_stor, &ctsu_state);
    if (ctsu_state == CTSU_STATE_READY)
    {
        R_CTSU_Control(CTSU_CMD_GET_STATUS, g_method_stor, &ctsu_status);
        if ((_1_FINISH == ctsu_status.flag.ctsu_measure) && (_0_SUCCESS == ctsu_status.flag.icomp_error) && (_1_FINISH == g_touch_function[g_method_stor].flag.tuning))
        {
            touch_data_moving_average(g_method_stor);                   /* Make Touch count value                    */
        }

        if (_1_FINISH == g_touch_system.flag.initial)                   /* Touch initialization finish check         */
        {
            offset_tuning_stop_judgement(g_method_stor);
        }


        /* Based upon legacy main() loop */

        if (_00_GET_OK == R_Get_Cap_Touch_Data_Check(g_method_stor))    // if scan completed without hardware error
        {
            /* ...but first, save raw scan data */
            if (TOUCH_SELF_MODE == g_key_info[g_method_stor].mode)
            {
                memcpy(g_self_raw_data_pt[g_method_stor], g_self_ico_data_pt[g_method_stor], sizeof(self_ico_t) * g_key_info[g_method_stor].ena_num );
            }
            else
            {
                memcpy(g_mutual_raw_data_pt[g_method_stor], g_mutual_ico_data_pt[g_method_stor], sizeof(mutual_ico_t) * g_key_info[g_method_stor].ena_num );
            }

            stabilization = R_Get_Cap_Touch_Initial_Status();
            if (stabilization == _1_FINISH)     // if stabilization (aka offset mode aka calibration aka tuning here) phase done
            {
                R_Set_Cap_Touch_Result_Create(g_method_stor);           // create results
#if (TOUCH_CFG_UPDATE_MONITOR == 1)
                monitor_update_data();
#endif
            }
            else
            {
                R_Set_Cap_Touch_Initial_Tuning(g_method_stor);          // perform stabilization/baseline calculation using scan data
            }
        }

        /* Based upon legacy R_Set_Cap_Touch_Next_Method_Change() */

        /* if not done calculating baseline for all methods */
        if (stabilization != _1_FINISH)
        {
            /* set method to next to scan */
            g_method_stor++;
            if (g_method_stor >= g_open_num_methods)
            {
                g_method_stor = 0;
            }

            chg_method = true;
        }
        else // done calculating baselines for all methods
        {
            if (g_cycle_methods == true)
            {
                /* set method to next in local list */
                g_mlist.cur_index++;
                if (g_mlist.cur_index >= g_mlist.num_methods)
                {
                    g_mlist.cur_index = 0;
                }

                g_method_stor = g_mlist.methods[g_mlist.cur_index];
                chg_method = true;
            }

            else if (g_method_stor != g_set_method)
            {
                /* set method to specific method */
                g_method_stor = g_set_method;
                chg_method = true;
            }

            else
            {
                /* leave current method running */
            }
        }

        if (chg_method == true)
        {
            R_CTSU_Control(CTSU_CMD_SET_METHOD, g_method_stor, NULL);
            gp_sliderInfo = gp_touch_configs[g_method_stor]->p_sliders;
            gp_wheelInfo = gp_touch_configs[g_method_stor]->p_wheels;
        }

        /* Based upon legacy R_Set_Cap_Touch_Measurement_Start() */

        correction_sensor_magnification_set(g_method_stor);
    }
    else
    {
        err = QE_ERR_BUSY;
    }

    return err;
}


/***********************************************************************************************************************
* Function Name: correction_sensor_magnification_set
* Description  :
* Arguments    : uint8_t method    : Measurement method(0-8)
* Return Value : none
***********************************************************************************************************************/
void correction_sensor_magnification_set( uint8_t method )
{
    uint8_t    loop;
    uint8_t    pt;
    uint8_t    snum;
    uint8_t    spda;
    uint8_t    mea_freq;
    uint16_t   ts_measure_time,reg_value;

    /* loop through each key for the method */
    for (loop = 0, pt = 1; loop < g_key_info[method].ena_num; loop++)
    {
        R_CTSU_ReadReg(CTSU_REG_SO0, loop, &reg_value);
        snum = (uint8_t)((reg_value & 0xFC00) >> 10);
        R_CTSU_ReadReg(CTSU_REG_SO1, loop, &reg_value);
        spda = (uint8_t)((reg_value & 0x1F00) >> 8);

        mea_freq = CTSU_INPUT_FREQUENCY_DIV * 10 / ((spda + 1) * 2);

        switch (mea_freq)
        {
            case 40:                                                            /* 4.0MHz    32MHz / 8     24MHz / 6  */
                ts_measure_time =  625 * (snum + 1);
                break;
            case 33:                                                            /* 3.3MHz    27MHz / 8  */
                ts_measure_time =  750 * (snum + 1);
                break;
            case 20:                                                            /* 2.0MHz    32MHz / 16    24MHz / 12  */
                ts_measure_time = 1250 * (snum + 1);
                break;
            case 16:                                                            /* 1.6MHz    27MHz / 16 */
                ts_measure_time = 1500 * (snum + 1);
                break;
            case 10:                                                            /* 1.0MHz    32MHz / 32    24MHz / 24  */
                ts_measure_time = 2500 * (snum + 1);
                break;
            case 8:                                                             /* 0.8MHz    27MHz / 32 */
                ts_measure_time = 3000 * (snum + 1);
                break;
            case 5:                                                             /* 0.5MHz    32MHz / 64    24MHz / 48  */
                ts_measure_time = 5000 * (snum + 1);
                break;
            case 4:                                                             /* 0.4MHz    27MHz / 64 */
                ts_measure_time = 6000 * (snum + 1);
                break;
            default:
                break;
        }

        if (5000 <= ts_measure_time)
        {
            *(g_key_info[method].counter_magni + loop) = (int8_t)(ts_measure_time / 500);
        }
        else
        {
            *(g_key_info[method].counter_magni + loop) = (int8_t)((50000 / ts_measure_time) * -1);
        }
        pt = pt + 3;
    }

}    /* End of function correction_sensor_magnification_set() */


/***********************************************************************************************************************
* Function Name: R_Get_Cap_Touch_Initial_Status
* Description  : Capacitive touch initialization status get API
* Arguments    : None
* Return Value : uint8_t status    : _0_RUN
*                                  : _1_FINISH
***********************************************************************************************************************/
uint8_t R_Get_Cap_Touch_Initial_Status( void )
{
    uint8_t status;

    status = _1_FINISH;                                          /* Return value set finish flag                      */

    if (_0_RUN == g_touch_system.flag.initial)                   /* Touch initial process run check                   */
    {
        status = _0_RUN;                                         /* Return value set run flag                         */
    }

    return status;
}    /* End of function R_Get_Cap_Touch_Initial_Status() */


/***********************************************************************************************************************
* Function Name: R_Set_Cap_Touch_Initial_Tuning
* Description  : Capacitive touch sensor counter initial offset tuning API
* Arguments    : uint8_t method    : Measurement method number(0-8)
* Return Value : uint8_t status    : TUNE_CONTINUE : 0
*              :                   : TUNE_COMPLETE : 1
***********************************************************************************************************************/
uint8_t R_Set_Cap_Touch_Initial_Tuning( uint8_t method )
{
    uint8_t    offset_status;
    uint8_t    initial_status;
    uint8_t    id;

    if (_0_STOP != g_ctsu_offset_mode)
    {
        offset_status = initial_offset_tuning( method , g_key_info[method].ena_num);
        initial_status = touch_calibration_check( method, offset_status );
    }
    else    // only set to STOP in Workbench.
    {
        for (id = 0; id < g_open_num_methods; id++)
        {
            R_CTSU_Control(CTSU_CMD_CLR_UPDATE_FLG, method, NULL);
            g_touch_function[method].flag.tuning = 1;
        }
        offset_status  = 1;
        initial_status = touch_calibration_check( method, offset_status );
    }

    return initial_status;
}    /* End of function R_Set_Cap_Touch_Initial_Tuning() */


/***********************************************************************************************************************
* Function Name: initial_offset_tuning
* Description  :
* Arguments    : uint8_t method    : Maeasurement method : 0-8
*              : uint8_t number    : Key number
* Return Value : none
***********************************************************************************************************************/
uint8_t initial_offset_tuning( uint8_t method, uint8_t number )
{
             uint8_t  loop;
             uint8_t  pt;
    volatile uint8_t  st;
    volatile uint8_t  status;
             uint16_t  beas_val;
             uint16_t  reg_value;
    volatile uint16_t sensor_raw[64];             /* ICO sensor counter storage buffer[Maximum number of sensors = 64] */

    status = _1_FINISH;
    pt     = 0;
    st     = 1;

    /* loop through each element for the method */
    for (loop = 0; loop < number; loop++ )
    {
        if (0 == *(g_touch_tuning_info[method].result + loop))
        {
            if (TOUCH_SELF_MODE == g_key_info[method].mode)
            {
                if (SET_SUCCESS == g_correction_create)
                {
                    sensor_raw[loop] = correction_sensor_cnt_create( method, loop, (loop + pt));      /* Sensor count get      */
                }
                else
                {
                    sensor_raw[loop] = *(g_self_ico_sensor_pt[method]  + (loop + pt));
                }
                beas_val = _07_2UA;                                                         /* 7.2uA = 15630 set     */
            }
            else
            {
                if (SET_SUCCESS == g_correction_create)
                {
                    sensor_raw[loop] = correction_pri_sensor_cnt_create( method, loop, (loop + pt));  /* Sensor count get      */
                }
                else
                {
                    sensor_raw[loop] = *(g_mutual_ico_pri_sensor_pt[method]  + (loop + pt));
                }
                beas_val = _04_8UA;                                                         /* 4.8uA = 10240 set     */
            }

           R_CTSU_ReadReg(CTSU_REG_SO0, loop, &reg_value);
           *(g_touch_tuning_info[method].ctsuso  + loop) = reg_value & 0x03FF;

            if (beas_val < (sensor_raw[loop] - TUNING_UPPER_LIMIT))                         /* Current over check    */
            {
                if (0x03FF != *(g_touch_tuning_info[method].ctsuso  + loop))                /* CTSUSO limit check    */
                {
                   reg_value = (reg_value & 0xFC00) + (*(g_touch_tuning_info[method].ctsuso + loop) + 1);
                   R_CTSU_WriteReg(CTSU_REG_SO0, loop, reg_value);

                   *(g_touch_tuning_info[method].result + loop)  = 0;
                   *(g_current_sign_pt[method] + loop) = *(g_current_sign_pt[method] + loop) + 1;    /* Plus         */
                }
                else
                {
                   *(g_touch_tuning_info[method].result + loop) = 1;                       /* Tuning finish flag set */
                }
            }
            else if (beas_val > (sensor_raw[loop] + TUNING_LOWER_LIMIT))                    /* Current down check    */
            {
                if (0x0000 != *(g_touch_tuning_info[method].ctsuso  + loop))                /* CTSUSO limit check    */
                {
                   reg_value = (reg_value & 0xFC00) + (*(g_touch_tuning_info[method].ctsuso + loop) - 1);
                   R_CTSU_WriteReg(CTSU_REG_SO0, loop, reg_value);

                   *(g_touch_tuning_info[method].result + loop) = 0;
                   *(g_current_sign_pt[method] + loop) = *(g_current_sign_pt[method] + loop) - 1;
                }
                else
                {
                   *(g_touch_tuning_info[method].result + loop) = 1;
                }
            }
            else
            {
               *(g_touch_tuning_info[method].result + loop) = 1;
            }
        }

        if (10 == g_current_offset_count[method])
        {
            if ((OFFSET_CNT_PLUS >= (*(g_current_sign_pt[method] + loop))) && (OFFSET_CNT_MINUS <= (*(g_current_sign_pt[method] + loop))))
            {
                *(g_touch_tuning_info[method].result + loop) = 1;
            }
            else
            {
                g_current_offset_count[method] = 0;
               *(g_current_sign_pt[method] + loop) = 0;
            }
        }

        if (TOUCH_SELF_MODE == g_key_info[method].mode)
        {
            pt     = pt + 1;
        }
        else
        {
            pt     = pt + 3;
        }
        st = st + 3;
    }

    R_CTSU_Control(CTSU_CMD_CLR_UPDATE_FLG, method, NULL);

    if (1 != g_touch_function[method].flag.tuning)
    {
        g_current_offset_count[method] = g_current_offset_count[method] + 1;
    }

    for (loop = 0; loop < number; loop++)
    {
        if (1 != *(g_touch_tuning_info[method].result + loop))
        {
            status = _0_RUN;
            return status;
        }
    }

    g_touch_function[method].flag.tuning = 1;

    for (loop = 0; loop < g_open_num_methods; loop++)
    {
        if (1 != g_touch_function[loop].flag.tuning)
        {
            status = _0_RUN;
            return status;
        }
    }

    return status;
}    /* End of function initial_offset_tuning() */


/***********************************************************************************************************************
* Function Name: R_Set_Cap_Touch_Offset_Timing
* Description  : 
* Arguments    : uint8_t mode            0 : Internal tuning stop
*              :                         1 : Internal tuning start
*              : uint16_t count          0 : Set Error
*              :                   1-65535 : Internal tuning timing
* Return Value : uint8_t status            : _0_SUCCESS
*              :                           : _1_ERROR
***********************************************************************************************************************/
uint8_t R_Set_Cap_Touch_Offset_Timing( uint8_t method, uint16_t count )
{
    uint8_t status;

    status = _1_ERROR;                                           /* Return value set error flag                       */

    if (_1_START == g_ctsu_offset_mode)
    {
        g_offset_time[method]    = count;
        status                   = _0_SUCCESS;
    }

    return status;
}    /* End of function R_Set_Cap_Touch_Offset_Timing() */


/***********************************************************************************************************************
* Function Name: R_Get_Cap_Touch_Data_Check
* Description  : Capacitive touch measurement data get check API
* Arguments    : uint8_t method    : Measurement method number(0-8)
* Return Value : uint8_t status    : _00_GET_OK
*              :                   : _01_GET_NG (Not data updata)
*              :                   : _02_SENS_OVER
*              :                   : _03_REF_OVER
*              :                   : _04_SENS_REF_OVER
*              :                   : _05_TSCAP_ERR
***********************************************************************************************************************/
uint8_t R_Get_Cap_Touch_Data_Check( uint8_t method )
{
    uint8_t    status;
    ctsu_status_t   ctsu_status;


    R_CTSU_Control(CTSU_CMD_GET_STATUS, method, &ctsu_status);

    if (_1_UPDATE == ctsu_status.flag.data_update)             /* Data get OK (non-error)                   */
    {
        status = _00_GET_OK;
    }
    else if ((_1_ERROR == ctsu_status.flag.sens_over) && (_1_ERROR == ctsu_status.flag.ref_over))
    {
        status = _04_SENS_REF_OVER;
    }
    else if (_1_ERROR == ctsu_status.flag.sens_over)           /* Sensor counter over error                 */
    {
        status = _02_SENS_OVER;
    }
    else if (_1_ERROR == ctsu_status.flag.ref_over)            /* Reference counter over error              */
    {
        status = _03_REF_OVER;
    }
    else if (_1_ERROR == ctsu_status.flag.icomp_error)         /* TSCAP voltage error                       */
    {
        status = _05_TSCAP_ERR;
    }
    else                                                       /* Not data update                           */
    {
        status = _01_GET_NG;    // get data "no good"/in transition (scan in progress)
    }

    return status;
}    /* End of function R_Get_Cap_Touch_Data_Check() */


/***********************************************************************************************************************
* Function Name: R_Get_Cap_Touch_Sensor_Data
* Description  : Sensor data get
* Arguments    : uint8_t  method     : Measurement method number(0-8)
*              : uint8_t  mode       : Function mode
*              : uint8_t  index_num  : Sensor number
* Return Value : uint16_t data       : Sensor data
***********************************************************************************************************************/
uint16_t R_Get_Cap_Touch_Sensor_Data( uint8_t method, uint8_t mode, uint8_t index_num )
{
    uint16_t data;

    switch (mode)
    {
        case _0_BUTTON:
            data = *(g_self_sensor_cnt_pt[method] + (*(g_key_info[method].sensor_index + index_num)));
            break;
        case _2_SLIDER:
            data = *(g_self_sensor_cnt_pt[method] + index_num);
            break;
        case _3_WHEEL:
            data = *(g_self_sensor_cnt_pt[method] + index_num);
            break;
        case _1_MATRIX:
            data = *(g_mutual_sensor_diff_pt[method] + index_num);
            break;
        default:
            data = 0xFFFF;
            break;
    }
    return data;
}    /* End of function R_Get_Touch_Cap_Sensor_Data() */

/***********************************************************************************************************************
* Function Name: R_Set_Cap_Touch_Result_Create
* Description  : 
* Arguments    : uint8_t method    : Measurement method number(0-8)
* Return Value : uint8_t status    : _0_SUCCESS
*              :                   : _1_ERROR
***********************************************************************************************************************/
uint16_t R_Set_Cap_Touch_Result_Create( uint8_t method )
{
    uint8_t  loop;
#if (QE_MAX_SLIDERS != 0)
    uint8_t     slider_id;
#endif
#if (QE_MAX_WHEELS != 0)
    uint8_t     wheel_id;
#endif
#if ((QE_MAX_SLIDERS != 0) || (QE_MAX_WHEELS != 0))
    uint16_t    pos_value;
#endif
    uint16_t    sensor_val;
    uint16_t    status;


    status = _1_ERROR;

    if (TOUCH_SELF_MODE == g_key_info[method].mode)
    {
        if (0 != g_key_info[method].key_num)
        {
            for (loop = 0; loop < MCU_MAX_SENSORS; loop++)
            {
                if (KEY_ENABLE == touch_key_function_check( method, loop))
                {
                    sensor_val = R_Get_Cap_Touch_Sensor_Data(method, _0_BUTTON, loop);
                    touch_key_decode(method, sensor_val, loop);
                }
            }

            for (loop = 0; loop < g_key_info[method].key_max_group; loop++)
            {
                g_touch_all_result[method].button[loop]  = *(g_key_info[method].touch_result  + loop);
                status = _0_SUCCESS;
            }
        }

#if (QE_MAX_SLIDERS != 0)
        if (gp_touch_configs[method]->num_sliders != 0)
        {
            for (slider_id = 0; slider_id < gp_touch_configs[method]->num_sliders; slider_id++)
            {
                for (loop = 0; loop < gp_sliderInfo[slider_id].num_elements; loop++)
                {
                    sensor_val = R_Get_Cap_Touch_Sensor_Data(method, _2_SLIDER, gp_sliderInfo[slider_id].elem_index[loop]);
                    g_slider_data[loop]  = sensor_val;
                }
                pos_value = slider_decode( slider_id );
                g_touch_all_result[method].slider[slider_id] = pos_value;
                status = _0_SUCCESS;
            }
        }
#endif

#if (QE_MAX_WHEELS != 0)
        if (gp_touch_configs[method]->num_wheels != 0)
        {
            for (wheel_id = 0; wheel_id < gp_touch_configs[method]->num_wheels; wheel_id++)
            {
                for (loop = 0; loop < gp_wheelInfo[wheel_id].num_elements; loop++)
                {
                    sensor_val = R_Get_Cap_Touch_Sensor_Data(method, _3_WHEEL, gp_wheelInfo[wheel_id].elem_index[loop]);
                    g_wheel_data[loop] = sensor_val;
                }
                pos_value = wheel_decode( wheel_id );
                g_touch_all_result[method].wheel[wheel_id] = pos_value;
                status = _0_SUCCESS;
            }
        }
#endif

    }
    else
    {
        if (0 != g_key_info[method].key_num)
        {
            for (loop = 0; loop < g_key_info[method].ena_num; loop++)
            {
                sensor_val = R_Get_Cap_Touch_Sensor_Data(method, _1_MATRIX, loop);
                touch_key_decode(method, sensor_val, loop);
            }

            for (loop = 0; loop < g_key_info[method].key_max_group; loop++)
            {
                g_touch_all_result[method].matrix[loop]  = *(g_key_info[method].touch_result  + loop);
                status = _0_SUCCESS;
            }
        }
    }

    R_CTSU_Control(CTSU_CMD_CLR_UPDATE_FLG, method, NULL);

    return status;
}    /* End of function R_Set_Cap_Touch_Result_Create() */


/***********************************************************************************************************************
* Function Name: touch_data_moving_average
* Description  : touch data moving average
* Arguments    : uint8_t method    : Measurement method : 0-8
* Return Value : none
***********************************************************************************************************************/
void touch_data_moving_average( uint8_t method )
{
    uint8_t     pt;
    uint8_t     offset;
    uint32_t    correction_work;
    uint32_t    correction_work2;
    uint32_t    scount_work;
    uint32_t    scount_work2;

    offset  = 0;

    for (pt = 0; pt < g_key_info[method].ena_num; pt++)
    {
        if (0 == g_touch_function[method].flag.average)
        {
            if (TOUCH_SELF_MODE == g_key_info[method].mode)
            {
                if (SET_SUCCESS == g_correction_create)
                {
                     *(g_self_add_sensor_pt[method]  + pt) = correction_sensor_cnt_create( method, pt, (pt + offset));
                }
                else
                {
                     *(g_self_add_sensor_pt[method]  + pt) = *(g_self_ico_sensor_pt[method] + (pt + offset));
                }
            }
            else
            {
                if (SET_SUCCESS == g_correction_create)
                {
                     *(g_mutual_add_pri_sensor_pt[method]  + pt) = correction_pri_sensor_cnt_create( method, pt, (pt + offset));
                     *(g_mutual_add_snd_sensor_pt[method]  + pt) = correction_snd_sensor_cnt_create( method, pt, (pt + offset));
                }
                else
                {
                     *(g_mutual_add_pri_sensor_pt[method]  + pt) = *(g_mutual_ico_pri_sensor_pt[method]  + (pt + offset));
                     *(g_mutual_add_snd_sensor_pt[method]  + pt) = *(g_mutual_ico_snd_sensor_pt[method]  + (pt + offset));
                }
            }
        }
        else
        {
            if (TOUCH_SELF_MODE == g_key_info[method].mode)
            {
                scount_work      = *(g_self_add_sensor_pt[method]  + pt);                   /* Get Add data            */
                scount_work     -= scount_work / ADD_TIME;                                  /* Average / ADD_TIME      */
                if (SET_SUCCESS == g_correction_create)
                {
                    correction_work  = correction_sensor_cnt_create( method, pt,(pt + offset));
                    scount_work     += correction_work / ADD_TIME;                          /* Add Now data / ADD_TIME */
                }
                else
                {
                    scount_work += *(g_self_ico_sensor_pt[method] + (pt + offset)) / ADD_TIME;     /* Add Now data / ADD_TIME     */
                }
               *(g_self_add_sensor_pt[method]  + pt) = scount_work;                         /* Data store for next     */
            }
            else
            {
                scount_work      = *(g_mutual_add_pri_sensor_pt[method]  + pt);             /* Get Add data            */
                scount_work2     = *(g_mutual_add_snd_sensor_pt[method]  + pt);             /* Get Add data            */
                scount_work     -= scount_work   / ADD_TIME;                                /* Average / ADD_TIME      */
                scount_work2    -= scount_work2  / ADD_TIME;                                /* Average / ADD_TIME      */
                if (SET_SUCCESS == g_correction_create)
                {
                    correction_work  = correction_pri_sensor_cnt_create( method, pt, (pt + offset));
                    correction_work2 = correction_snd_sensor_cnt_create( method, pt, (pt + offset));
                    scount_work     += correction_work   / ADD_TIME;                        /* Add Now data / ADD_TIME */
                    scount_work2    += correction_work2  / ADD_TIME;                        /* Add Now data / ADD_TIME */
                }
                else
                {
                    scount_work   += *(g_mutual_ico_pri_sensor_pt[method] + (pt + offset)) / ADD_TIME;  /* Add Now data / ADD_TIME     */
                    scount_work2  += *(g_mutual_ico_snd_sensor_pt[method] + (pt + offset)) / ADD_TIME;  /* Add Now data / ADD_TIME     */
                }
               *(g_mutual_add_pri_sensor_pt[method]    + pt) = scount_work;                 /* Data store for next     */
               *(g_mutual_add_snd_sensor_pt[method]    + pt) = scount_work2;                /* Data store for next     */
            }
        }

        if (TOUCH_SELF_MODE == g_key_info[method].mode)
        {
             *(g_self_sensor_cnt_pt[method]    + pt) = *(g_self_add_sensor_pt[method]    + pt);
             offset = offset + 1;
        }
        else
        {
            if (*(g_mutual_add_snd_sensor_pt[method] + (pt)) > *(g_mutual_add_pri_sensor_pt[method] + (pt)))
            {
                *(g_mutual_sensor_diff_pt[method] + pt) = (uint16_t)(*(g_mutual_add_snd_sensor_pt[method] + (pt)) - *(g_mutual_add_pri_sensor_pt[method] + (pt)));
            }
            else
            {
                 *(g_mutual_sensor_diff_pt[method] + pt) = 0;
            }
            offset = offset + 3;
        }
    }

    if ( 0 == g_touch_function[method].flag.average)
    {
        g_touch_function[method].flag.average = 1;
    }
}    /* End of function touch_data_moving_average() */


/***********************************************************************************************************************
* Function Name: correction_sensor_cnt_create
* Description  :
* Arguments    : uint8_t method    : Maeasurement method : 0-8
*              : uint8_t number    : Key number
* Return Value : none
***********************************************************************************************************************/
uint16_t correction_sensor_cnt_create( uint8_t method, uint8_t ts_num, uint8_t number)
{
    uint8_t  loop;
    uint8_t  loop_coef;
    uint32_t cmp_val;
    uint32_t correct_box;
    uint32_t correct_box2;
    uint16_t correction_sensor_cnt;

    if ( 0 > (*(g_key_info[method].counter_magni + ts_num)))
    {
        cmp_val     = (*(g_self_ico_sensor_pt[method] + number)) * (*(g_key_info[method].counter_magni + ts_num) * -1);
        cmp_val     =  cmp_val / 10;
        correct_box = *(g_self_ico_sensor_pt[method] + number);
    }
    else
    {
        cmp_val     = ((*(g_self_ico_sensor_pt[method] + number)) * 10) / (*(g_key_info[method].counter_magni + ts_num));
        correct_box = *(g_self_ico_sensor_pt[method] + number);
    }

    if (g_linear_interpolation_table[59] > cmp_val)
    {
        correct_box2          = correct_box * g_linear_interpolation_coefficient[0];
        correct_box2          = correct_box2 >> 10;         /* Unscaling corrected value */

        if(0x0000FFFF < correct_box2)
        {
            correct_box2 = 0x0000FFFF;                      /* 16bit length overflow limiter */
        }
        correction_sensor_cnt = (uint16_t)correct_box2;
    }
    else
    {
        if (g_linear_interpolation_table[29] <= cmp_val)
        {
            if (g_linear_interpolation_table[14] <= cmp_val)
            {
                for (loop = 0, loop_coef = 60; loop < 15; loop++)
                {
                    if (g_linear_interpolation_table[loop] <= cmp_val)
                    {
                        correct_box2         = correct_box * g_linear_interpolation_coefficient[loop_coef];
                        correct_box2         = correct_box2 >> 10;

                        if(0x0000FFFF < correct_box2)
                        {
                            correct_box2 = 0x0000FFFF;
                        }
                        correction_sensor_cnt = (uint16_t)correct_box2;
                        break;
                    }
                    loop_coef = loop_coef - 1;
                }
            }
            else
            {
                for (loop = 15, loop_coef = 45; loop < 30; loop++)
                {
                    if (g_linear_interpolation_table[loop] <= cmp_val)
                    {
                        correct_box2         = correct_box * g_linear_interpolation_coefficient[loop_coef];
                        correct_box2         = correct_box2 >> 10;

                        if(0x0000FFFF < correct_box2)
                        {
                            correct_box2 = 0x0000FFFF;
                        }
                        correction_sensor_cnt = (uint16_t)correct_box2;
                        break;
                    }
                    loop_coef = loop_coef - 1;
                }
            }
        }
        else
        {
            if (g_linear_interpolation_table[44] <= cmp_val)
            {
                for (loop = 30, loop_coef = 30; loop < 45; loop++)
                {
                    if (g_linear_interpolation_table[loop] <= cmp_val)
                    {
                        correct_box2         = correct_box * g_linear_interpolation_coefficient[loop_coef];
                        correct_box2         = correct_box2 >> 10;

                        if(0x0000FFFF < correct_box2)
                        {
                            correct_box2 = 0x0000FFFF;
                        }
                        correction_sensor_cnt = (uint16_t)correct_box2;
                        break;
                    }
                    loop_coef = loop_coef - 1;
                }
            }
            else
            {
                for (loop = 45, loop_coef = 15; loop < 60; loop++)
                {
                    if (g_linear_interpolation_table[loop] <= cmp_val)
                    {
                        correct_box2         = correct_box * g_linear_interpolation_coefficient[loop_coef];
                        correct_box2         = correct_box2 >> 10;

                        if(0x0000FFFF < correct_box2)
                        {
                            correct_box2 = 0x0000FFFF;
                        }
                        correction_sensor_cnt = (uint16_t)correct_box2;
                        break;
                    }
                    loop_coef = loop_coef - 1;
                }
            }
        }
    }

    return correction_sensor_cnt;

}    /* End of function correction_sensor_cnt_create() */


/***********************************************************************************************************************
* Function Name: correction_pri_sensor_cnt_create
* Description  :
* Arguments    : uint8_t method    : Maeasurement method : 0-8
*              : uint8_t number    : Key number
* Return Value : none
***********************************************************************************************************************/
uint16_t correction_pri_sensor_cnt_create( uint8_t method, uint8_t ts_num, uint8_t number)
{
    uint8_t  loop;
    uint8_t  loop_coef;
    uint32_t cmp_val;
    uint32_t correct_box;
    uint32_t correct_box2;
    uint16_t correction_pri_sensor_cnt;

    if ( 0 > (*(g_key_info[method].counter_magni + ts_num)))
    {
        cmp_val     = (*(g_mutual_ico_pri_sensor_pt[method] + number)) * (*(g_key_info[method].counter_magni + ts_num) * -1);
        cmp_val     =  cmp_val / 10;
        correct_box = *(g_mutual_ico_pri_sensor_pt[method] + number);
    }
    else
    {
        cmp_val     = (*(g_mutual_ico_pri_sensor_pt[method] + number) * 10) / (*(g_key_info[method].counter_magni + ts_num));
        correct_box = *(g_mutual_ico_pri_sensor_pt[method] + number);
    }

    if (g_linear_interpolation_table[59] > cmp_val)
    {
        correct_box2              = correct_box * g_linear_interpolation_coefficient[0];
        correct_box2              = correct_box2 >> 10;     /* Unscaling corrected value */

        if(0x0000FFFF < correct_box2)
        {
            correct_box2 = 0x0000FFFF;                      /* 16bit length overflow limiter */
        }
        correction_pri_sensor_cnt = (uint16_t)correct_box2;
    }
    else
    {
        if (g_linear_interpolation_table[29] <= cmp_val)
        {
            if (g_linear_interpolation_table[14] <= cmp_val)
            {
                for (loop = 0, loop_coef = 60; loop < 15; loop++)
                {
                    if (g_linear_interpolation_table[loop] <= cmp_val)
                    {
                        correct_box2              = correct_box * g_linear_interpolation_coefficient[loop_coef];
                        correct_box2              = correct_box2 >> 10;

                        if(0x0000FFFF < correct_box2)
                        {
                            correct_box2 = 0x0000FFFF;
                        }
                        correction_pri_sensor_cnt = (uint16_t)correct_box2;
                        break;
                    }
                    loop_coef = loop_coef - 1;
                }
            }
            else
            {
                for (loop = 15, loop_coef = 45; loop < 30; loop++)
                {
                    if (g_linear_interpolation_table[loop] <= cmp_val)
                    {
                        correct_box2              = correct_box * g_linear_interpolation_coefficient[loop_coef];
                        correct_box2              = correct_box2 >> 10;

                        if(0x0000FFFF < correct_box2)
                        {
                            correct_box2 = 0x0000FFFF;
                        }
                        correction_pri_sensor_cnt = (uint16_t)correct_box2;
                        break;
                    }
                    loop_coef = loop_coef - 1;
                }
            }
        }
        else
        {
            if (g_linear_interpolation_table[44] <= cmp_val)
            {
                for (loop = 30, loop_coef = 30; loop < 45; loop++)
                {
                    if (g_linear_interpolation_table[loop] <= cmp_val)
                    {
                        correct_box2              = correct_box * g_linear_interpolation_coefficient[loop_coef];
                        correct_box2              = correct_box2 >> 10;

                        if(0x0000FFFF < correct_box2)
                        {
                            correct_box2 = 0x0000FFFF;
                        }
                        correction_pri_sensor_cnt = (uint16_t)correct_box2;
                        break;
                    }
                    loop_coef = loop_coef - 1;
                }
            }
            else
            {
                for (loop = 45, loop_coef = 15; loop < 60; loop++)
                {
                    if (g_linear_interpolation_table[loop] <= cmp_val)
                    {
                        correct_box2              = correct_box * g_linear_interpolation_coefficient[loop_coef];
                        correct_box2              = correct_box2 >> 10;

                        if(0x0000FFFF < correct_box2)
                        {
                            correct_box2 = 0x0000FFFF;
                        }
                        correction_pri_sensor_cnt = (uint16_t)correct_box2;
                        break;
                    }
                    loop_coef = loop_coef - 1;
                }
            }
        }
    }

    return correction_pri_sensor_cnt;

}    /* End of function correction_pri_sensor_cnt_create() */


/***********************************************************************************************************************
* Function Name: correction_snd_sensor_cnt_create
* Description  :
* Arguments    : uint8_t method    : Maeasurement method : 0-8
*              : uint8_t number    : Key number
* Return Value : none
***********************************************************************************************************************/
uint16_t correction_snd_sensor_cnt_create( uint8_t method, uint8_t ts_num, uint8_t number)
{
    uint8_t  loop;
    uint8_t  loop_coef;
    uint32_t cmp_val;
    uint32_t correct_box;
    uint32_t correct_box2;
    uint16_t correction_snd_sensor_cnt;

    if ( 0 > (*(g_key_info[method].counter_magni + ts_num)))
    {
        cmp_val     = (*(g_mutual_ico_snd_sensor_pt[method] + number)) * (*(g_key_info[method].counter_magni + ts_num) * -1);
        cmp_val     =  cmp_val / 10;
        correct_box = *(g_mutual_ico_snd_sensor_pt[method] + number);
    }
    else
    {
        cmp_val     = (*(g_mutual_ico_snd_sensor_pt[method] + number) * 10) / (*(g_key_info[method].counter_magni + ts_num));
        correct_box = *(g_mutual_ico_snd_sensor_pt[method] + number);
    }

    if (g_linear_interpolation_table[59] > cmp_val)
    {
        correct_box2              = correct_box * g_linear_interpolation_coefficient[0];
        correct_box2              = correct_box2 >> 10;     /* Unscaling corrected value */

        if(0x0000FFFF < correct_box2)
        {
            correct_box2 = 0x0000FFFF;                      /* 16bit length overflow limiter */
        }
        correction_snd_sensor_cnt = (uint16_t)correct_box2;
    }
    else
    {
        if (g_linear_interpolation_table[29] <= cmp_val)
        {
            if (g_linear_interpolation_table[14] <= cmp_val)
            {
                for (loop = 0, loop_coef = 60; loop < 15; loop++)
                {
                    if (g_linear_interpolation_table[loop] <= cmp_val)
                    {
                        correct_box2              = correct_box * g_linear_interpolation_coefficient[loop_coef];
                        correct_box2              = correct_box2 >> 10;

                        if(0x0000FFFF < correct_box2)
                        {
                            correct_box2 = 0x0000FFFF;
                        }
                        correction_snd_sensor_cnt = (uint16_t)correct_box2;
                        break;
                    }
                    loop_coef = loop_coef - 1;
                }
            }
            else
            {
                for (loop = 15, loop_coef = 45; loop < 30; loop++)
                {
                    if (g_linear_interpolation_table[loop] <= cmp_val)
                    {
                        correct_box2              = correct_box * g_linear_interpolation_coefficient[loop_coef];
                        correct_box2              = correct_box2 >> 10;

                        if(0x0000FFFF < correct_box2)
                        {
                            correct_box2 = 0x0000FFFF;
                        }
                        correction_snd_sensor_cnt = (uint16_t)correct_box2;
                        break;
                    }
                    loop_coef = loop_coef - 1;
                }
            }
        }
        else
        {
            if (g_linear_interpolation_table[44] <= cmp_val)
            {
                for (loop = 30, loop_coef = 30; loop < 45; loop++)
                {
                    if (g_linear_interpolation_table[loop] <= cmp_val)
                    {
                        correct_box2              = correct_box * g_linear_interpolation_coefficient[loop_coef];
                        correct_box2              = correct_box2 >> 10;

                        if(0x0000FFFF < correct_box2)
                        {
                            correct_box2 = 0x0000FFFF;
                        }
                        correction_snd_sensor_cnt = (uint16_t)correct_box2;
                        break;
                    }
                    loop_coef = loop_coef - 1;
                }
            }
            else
            {
                for (loop = 45, loop_coef = 15; loop < 60; loop++)
                {
                    if (g_linear_interpolation_table[loop] <= cmp_val)
                    {
                        correct_box2              = correct_box * g_linear_interpolation_coefficient[loop_coef];
                        correct_box2              = correct_box2 >> 10;

                        if(0x0000FFFF < correct_box2)
                        {
                            correct_box2 = 0x0000FFFF;
                        }
                        correction_snd_sensor_cnt = (uint16_t)correct_box2;
                        break;
                    }
                    loop_coef = loop_coef - 1;
                }
            }
        }
    }

    return correction_snd_sensor_cnt;

}    /* End of function correction_snd_sensor_cnt_create() */


/***********************************************************************************************************************
* Function Name: R_TOUCH_Control
* Description  : This function is used to perform special operations.
* Arguments    : cmd -
*                    TOUCH_CMD_SET_METHOD:
*                       Stops cycling through methods if was cycling.
*                       Sets "method" to only method to scan going forward.
*                    TOUCH_CMD_CYCLE_ALL_METHODS:
*                       Starts cycling through all methods beginning with the method after the currently running one.
*                    TOUCH_CMD_CYCLE_METHOD_LIST:
*                       Starts cycling through specified list of methods only.
*                p_arg -
*                    Unused (for future expansion)
* Return Value : QE_SUCCESS -
*                    Command performed successfully.
*                QE_ERR_INVALID_ARG -
*                    Command, method number, or number of methods is invalid.
***********************************************************************************************************************/
qe_err_t R_TOUCH_Control(touch_cmd_t cmd, void *p_arg)
{
    uint8_t         *p_set_method=(uint8_t *)p_arg;
    touch_mlist_t   *p_list=(touch_mlist_t *)p_arg;
    qe_err_t        err=QE_SUCCESS;
    uint8_t         i;

#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((cmd >= TOUCH_CMD_END_ENUM)
     || ((cmd == TOUCH_CMD_SET_METHOD) && (*p_set_method >= g_open_num_methods)))
    {
        return QE_ERR_INVALID_ARG;
    }

    if (cmd == TOUCH_CMD_CYCLE_METHOD_LIST)
    {
        if (p_list->num_methods > g_open_num_methods)
        {
            return QE_ERR_INVALID_ARG;
        }

        for (i=0; i < p_list->num_methods; i++)
        {
            if (p_list->methods[i] >= g_open_num_methods)
            {
                return QE_ERR_INVALID_ARG;
            }
        }
    }
#endif


    switch (cmd)
    {
    case TOUCH_CMD_SET_METHOD:
        g_set_method = *p_set_method;
        g_cycle_methods = false;
        /* for monitor use */
        g_mlist.num_methods = 1;
        g_mlist.methods[0] = *p_set_method;
        break;

    case TOUCH_CMD_CYCLE_ALL_METHODS:
        /* set local list to all methods in Open() */
        for (i=0; i < g_open_num_methods; i++)
        {
            g_mlist.methods[i] = i;
        }

        g_mlist.num_methods = g_open_num_methods;
        g_mlist.cur_index = g_method_stor;
        g_cycle_methods = true;
        break;

    case TOUCH_CMD_CYCLE_METHOD_LIST:
        /* set local list to specified list */
        memcpy(&g_mlist, p_list, sizeof(touch_mlist_t));
        g_mlist.cur_index = 0;
        g_cycle_methods = true;
        break;

    default:
        err = QE_ERR_INVALID_ARG;
        break;
    }

    return err;
}


/***********************************************************************************************************************
* Function Name: R_TOUCH_GetRawData
* Description  : This function gets the sensor values as scanned by the CTSU (correction not applied).
* Arguments    : method -
*                    Method to get data for.
*                p_buf -
*                    Pointer to buffer to load data into.
*                p_cnt -
*                    Pointer to variable to load number of of words loaded into buffer.
* Return Value : QE_SUCCESS -
*                    Command performed successfully.
*                QE_ERR_INVALID_ARG -
*                    "method" is invalid.
*                QE_ERR_NULL_PTR -
*                    "p_buf" or "p_cnt" is NULL.
***********************************************************************************************************************/
qe_err_t R_TOUCH_GetRawData(uint8_t method, uint16_t *p_buf, uint8_t *p_cnt)
{
    uint8_t         i;
    self_ico_t      *p_self_data;
    mutual_ico_t    *p_mutual_data;

#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if (method >= g_open_num_methods)
    {
        return QE_ERR_INVALID_ARG;
    }

    if ((p_buf == NULL) || (p_cnt == NULL))
    {
        return QE_ERR_NULL_PTR;
    }
#endif


    *p_cnt = g_key_info[method].ena_num;

    if (TOUCH_SELF_MODE == g_key_info[method].mode)
    {
        p_self_data = g_self_raw_data_pt[method];
        for (i=0; i < *p_cnt; i++, p_self_data++)
        {
            *p_buf++ = p_self_data->sen;
        }
    }
    else
    {
        p_mutual_data = g_mutual_raw_data_pt[method];
        for (i=0; i < *p_cnt; i++, p_mutual_data++)
        {
            *p_buf++ = p_mutual_data->pri_sen;
            *p_buf++ = p_mutual_data->snd_sen;
        }

        *p_cnt *= 2;
    }

    return QE_SUCCESS;
}


/***********************************************************************************************************************
* Function Name: R_TOUCH_GetData
* Description  : This function gets the sensor values after correction is applied.
* Arguments    : method -
*                    Method to get data for.
*                p_buf -
*                    Pointer to buffer to load data into.
*                p_cnt -
*                    Pointer to variable to load number of of words loaded into buffer.
* Return Value : QE_SUCCESS -
*                    Command performed successfully.
*                QE_ERR_INVALID_ARG -
*                    "method" is invalid.
*                QE_ERR_NULL_PTR -
*                    "p_buf" or "p_cnt" is NULL.
***********************************************************************************************************************/
qe_err_t R_TOUCH_GetData(uint8_t method, uint16_t *p_buf, uint8_t *p_cnt)
{
    uint8_t     i;
    uint16_t    *p_data;

#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if (method >= g_open_num_methods)
    {
        return QE_ERR_INVALID_ARG;
    }

    if ((p_buf == NULL) || (p_cnt == NULL))
    {
        return QE_ERR_NULL_PTR;
    }
#endif


    *p_cnt = g_key_info[method].ena_num;

    if (TOUCH_SELF_MODE == g_key_info[method].mode)
    {
        p_data = g_self_sensor_cnt_pt[method];
    }
    else
    {
        p_data = g_mutual_sensor_diff_pt[method];
    }

    for (i=0; i < *p_cnt; i++)
    {
        *p_buf++ = *p_data++;
    }

    return QE_SUCCESS;
}


/***********************************************************************************************************************
* Function Name: R_TOUCH_GetAllBtnStates
* Description  : This function loads a 64-bit mask indicating which buttons are pressed.
* Arguments    : method -
*                    Method to get data for.
*                p_mask -
*                    Pointer to variable to load button-touched mask into. The masks are defined in the method-specific
*                    file named qe_<configuration name>.h.
* Return Value : QE_SUCCESS -
*                    Command performed successfully.
*                QE_ERR_INVALID_ARG -
*                    "method" is invalid.
*                QE_ERR_NULL_PTR -
*                    "p_mask" is NULL.
***********************************************************************************************************************/
qe_err_t R_TOUCH_GetAllBtnStates(uint8_t method, uint64_t *p_mask)
{
    qe_err_t    err=QE_SUCCESS;
    uint64_t    mask;
    uint8_t     i;

#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if (method >= g_open_num_methods)
    {
        return QE_ERR_INVALID_ARG;
    }

    if (p_mask == NULL)
    {
        return QE_ERR_NULL_PTR;
    }
#endif

    if (R_Get_Cap_Touch_Initial_Status() != _1_FINISH)
    {
        err = QE_ERR_TUNING_IN_PROGRESS;
    }
    else
    {
        mask = 0;

        /* .touch_result is the mask in a 16-bit array of .key_max_group size */
        for (i=0; i < g_key_info[method].key_max_group; i++)
        {
            mask |= (uint64_t)(*(g_key_info[method].touch_result + i)) << (16 * i);
        }

        *p_mask = mask;
    }

    return err;
}


/***********************************************************************************************************************
* Function Name: R_TOUCH_GetSliderPosition
* Description  : This function gets the current location of where a slider is being touched (if at all).
* Arguments    : slider_id -
*                    ID of slider to inspect. The ID is defined in the method-specific file named
*                    qe_<configuration name>.h.
*                p_position -
*                    Pointer to variable to load the slider position into.
*                    Position ranges from 0 (low) to 100 (high). 65535 implies slider is not being touched.
* Return Value : QE_SUCCESS -
*                    Command performed successfully.
*                QE_ERR_INVALID_ARG -
*                    "slider_id" is invalid.
*                QE_ERR_NULL_PTR -
*                    "p_position" is NULL.
***********************************************************************************************************************/
qe_err_t R_TOUCH_GetSliderPosition(uint16_t slider_id, uint16_t *p_position)
{
    uint8_t     method,index;
    qe_err_t    err=QE_SUCCESS;

    method = slider_id >> 8;    // high byte of id is method
    index = slider_id & 0xFF;   // low byte of id is slider number/index for method

#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((method >= g_open_num_methods)
     || (index >= gp_touch_configs[method]->num_sliders))
    {
        return QE_ERR_INVALID_ARG;
    }

    if (p_position == NULL)
    {
        return QE_ERR_NULL_PTR;
    }
#endif


    if (R_Get_Cap_Touch_Initial_Status() != _1_FINISH)
    {
        err = QE_ERR_TUNING_IN_PROGRESS;
    }
    else
    {

        *p_position = gp_touch_configs[method]->p_sliders[index].value;
    }

    return err;
}


/***********************************************************************************************************************
* Function Name: R_TOUCH_GetWheelPosition
* Description  : This function gets the current location of where a wheel is being touched (if at all).
* Arguments    : wheel_id -
*                    ID of wheel to inspect. The ID is defined in the method-specific file named
*                    qe_<configuration name>.h.
*                p_position -
*                    Pointer to variable to load the wheel position into.
*                    Position ranges from 1 to 360 degrees. 65535 implies wheel is not being touched.
* Return Value : QE_SUCCESS -
*                    Command performed successfully.
*                QE_ERR_INVALID_ARG -
*                    "wheel_id" is invalid.
*                QE_ERR_NULL_PTR -
*                    "p_position" is NULL.
***********************************************************************************************************************/
qe_err_t R_TOUCH_GetWheelPosition(uint16_t wheel_id, uint16_t *p_position)
{
    uint8_t     method,index;
    qe_err_t    err=QE_SUCCESS;

    method = wheel_id >> 8;     // high byte of id is method
    index = wheel_id & 0xFF;    // low byte of id is slider number/index for method

#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((method >= g_open_num_methods)
     || (index >= gp_touch_configs[method]->num_wheels))
    {
        return QE_ERR_INVALID_ARG;
    }

    if (p_position == NULL)
    {
        return QE_ERR_NULL_PTR;
    }
#endif


    if (R_Get_Cap_Touch_Initial_Status() != _1_FINISH)
    {
        err = QE_ERR_TUNING_IN_PROGRESS;
    }
    else
    {
        *p_position = gp_touch_configs[method]->p_wheels[index].value;
    }

    return err;
}


/***********************************************************************************************************************
* Function Name: R_TOUCH_Close
* Description  : This function closes the CapTouch driver. This requires no processing at the Touch layer. The hardware
*                is shut down at the CTSU layer.
* Arguments    : none
* Return Value : QE_SUCCESS -
*                    CTSU peripheral shut down successfully.
*                QE_ERR_BUSY
*                    Cannot run function because CTSU is performing an operation (likely scan in progress).
***********************************************************************************************************************/
qe_err_t R_TOUCH_Close(void)
{
    qe_err_t    err;

    err = R_CTSU_Close();

    return err;
}


/***********************************************************************************************************************
* Function Name: R_TOUCH_GetVersion
* Description  : Returns the current version of this module. The version number is encoded where the top 2 bytes are the
*                major version number and the bottom 2 bytes are the minor version number. For example, Version 4.25
*                would be returned as 0x00040019.
* Arguments    : none
* Return Value : Version of this module.
***********************************************************************************************************************/
#pragma inline(R_TOUCH_GetVersion)
uint32_t R_TOUCH_GetVersion (void)
{
    /* These version macros are defined in r_flash_if.h. */
    return ((((uint32_t)QETOUCH_RX_VERSION_MAJOR) << 16) | (uint32_t)QETOUCH_RX_VERSION_MINOR);
}
