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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_widgets.c
* Description  : This file contains support functions for key, slider, and wheel initializations and calculations.
***********************************************************************************************************************/

/***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              :            0.85    Shinji Takeda: Add "return status" to else condition, line 447 in touch_calibration()
*              : 04.10.2018 1.00    First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "r_touch_qe_private.h"
#include "r_touch_qe_if.h"
#include "qe_common.h"


/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/
#define OFF    0
#define ON     1

#define KEY_GROUP0      (16)
#define KEY_GROUP1      (32)
#define KEY_GROUP2      (48)
#define KEY_GROUP3      (64)


/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
* Global variables and functions
***********************************************************************************************************************/
static void     touch_sensor_index_set(uint8_t method, ctsu_cfg_t *p_ctsu_cfgs[]);
static uint8_t  touch_drift_correction(uint8_t method, uint16_t value, uint8_t group, uint8_t offset);
static void     touch_judgement_parameter_create(uint8_t method, uint16_t value);
static void     touch_judgement(uint8_t method, uint16_t key_val_sub, uint8_t group, uint8_t offset);


/***********************************************************************************************************************
* Function Name: touch_parameter_set
* Description  : Touch parameter setting
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void touch_parameter_set(ctsu_cfg_t *p_ctsu_cfgs[], touch_cfg_t *p_touch_cfgs[])
{
    uint8_t     method;
    uint8_t     pt;
    uint8_t     loop;
#if ((QE_MAX_SLIDERS != 0) || (QE_MAX_WHEELS != 0))
    uint8_t     id;
#endif


    /* Touch system flag initial */
    g_touch_system.value = 0x00;

    for (method = 0; method < g_open_num_methods; method++)
    {
        /**************************************************************************************************************/
        /*****     Touch function parameter setting     ***************************************************************/
        /**************************************************************************************************************/
        g_touch_paramter[method].calib_key_num     = 0;

        /* Compulsion touch cancellation frequency setting    */
        g_touch_paramter[method].msa_freq          = p_touch_cfgs[method]->max_on_time;

        /* Continuous agreement touch comparison frequency setting    */
        g_touch_paramter[method].touch_freq        = p_touch_cfgs[method]->touch_on;

        /* Continuous agreement non-touch comparison frequency setting    */
        g_touch_paramter[method].not_touch_freq    = p_touch_cfgs[method]->touch_off;

        /* Drift correction frequency setting    */
        g_touch_paramter[method].drift_freq        = p_touch_cfgs[method]->drift_freq;

        /**************************************************************************************************************/
        /*****     Touch function flag setting          ***************************************************************/
        /**************************************************************************************************************/
        g_touch_function[method].value             = 0x00;         /* Touch function flag initial                     */

        if (g_touch_paramter[method].msa_freq)
        {
            g_touch_function[method].flag.msa      = ON;           /* Compulsion touch cancellation function enable   */
        }

        if (g_touch_paramter[method].touch_freq)
        {
            g_touch_function[method].flag.acd0     = ON;           /* Continuous touch comparison function enable     */
        }

        if (g_touch_paramter[method].not_touch_freq)
        {
            g_touch_function[method].flag.acd1     = ON;           /* Continuous non-touch comparison function enable */
        }

        if (1 == p_touch_cfgs[method]->drift_enable)
        {
            g_touch_function[method].flag.drift    = ON;           /* Drift correction function enable                */
        }
        else
        {
            g_touch_function[method].flag.drift    = OFF;
        }

#if (QE_MAX_BUTTONS != 0)
        if (TOUCH_SELF_MODE == g_key_info[method].mode)
        {
            if (0 == g_key_info[method].key_num)
            {
                g_touch_function[method].flag.calib    = OFF;      /* Calibration function disable                    */
            }
            else
            {
                g_touch_function[method].flag.calib    = ON;       /* Calibration function enable                     */
            }
        }
        else
        {
            g_touch_function[method].flag.calib        = ON;       /* Calibration function enable                     */
        }
        g_calib_info[method].calib_key                 = 0;
        g_calib_info[method].calib_cnt                 = 0;
#else
        g_touch_function[method].flag.calib            = OFF;      /* Calibration function disable                    */
#endif    // (QE_MAX_BUTTONS != 0)

        /**************************************************************************************************************/
        /*****     Threshold and Hysteresis setting     ***************************************************************/
        /**************************************************************************************************************/
#if (QE_MAX_BUTTONS != 0)
        for (pt = 0; pt < g_key_info[method].key_num; pt++)
        {
             *(g_key_info[method].user_thr + pt)   = p_touch_cfgs[method]->p_buttons[pt].thr_offset;
             *(g_key_info[method].hys      + pt)   = p_touch_cfgs[method]->p_buttons[pt].hys_offset;
        }

        /**************************************************************************************************************/
        /*****     Touch parameter initial setting     ****************************************************************/
        /**************************************************************************************************************/
        for (pt = 0; pt < g_key_info[method].key_max_group; pt++)
        {
             *(g_key_info[method].in_touch         + pt) = 0x0000;
             *(g_key_info[method].out_touch        + pt) = 0x0000;
             *(g_key_info[method].touch_result     + pt) = 0x0000;
             *(g_key_info[method].drift_permission + pt) = 0xFFFF;
             *(g_key_info[method].key_used_info    + pt) = p_touch_cfgs[method]->button_masks[pt];
             *(g_touch_tuning_info[method].result  + pt) = 0;
        }


#endif    // (QE_MAX_BUTTONS != 0)

        for (pt = 0; pt < g_key_info[method].ena_num; pt++)
        {
             *(g_touch_tuning_info[method].result  + pt) = 0;
        }

        g_touch_all_result[method].button[0] = 0x0000;
        g_touch_all_result[method].button[1] = 0x0000;
        g_touch_all_result[method].button[2] = 0x0000;
        g_touch_all_result[method].matrix[0] = 0x0000;
        g_touch_all_result[method].matrix[1] = 0x0000;
        g_touch_all_result[method].matrix[2] = 0x0000;
        g_touch_all_result[method].matrix[3] = 0x0000;

#if (QE_MAX_SLIDERS != 0)
        for (id = 0; id < QE_MAX_SLIDERS; id++)
        {
            g_touch_all_result[method].slider[id] = 0xFFFF;
        }
#endif

#if (QE_MAX_WHEELS != 0)
        for (id = 0; id < QE_MAX_WHEELS; id++)
        {
            g_touch_all_result[method].wheel[id] = 0xFFFF;
        }
#endif

        /**************************************************************************************************************/
        /****    Initialize index table for conversion ts-number to SENSOR0_NUM index    ******************************/
        /**************************************************************************************************************/
        touch_sensor_index_set(method, p_ctsu_cfgs);

   }


    for (loop = 0; loop < g_open_num_methods; loop++)
    {
        g_offset_time[loop]          = 10;
        g_current_offset_count[loop] = 0;
    }
}    /* End of function touch_parameter_set() */


/***********************************************************************************************************************
* Function Name: touch_sensor_index_set
* Description  : 
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void touch_sensor_index_set(uint8_t method, ctsu_cfg_t *p_ctsu_cfgs[])
{
    uint16_t    chac_stor;
    uint8_t     loop;
    uint8_t     pt;
    uint8_t     cmp_key[3];
    uint8_t     key_num;
    uint8_t     used_key;

    //chac_stor  = g_ctsu_parameter[method].ctsu_chac01;
    chac_stor = p_ctsu_cfgs[method]->ctsuchac0 | (p_ctsu_cfgs[method]->ctsuchac1 << 8);

    if (TOUCH_SELF_MODE == g_key_info[method].mode )
    {
        used_key = 0;
        pt = 0;

        if (32 < MCU_MAX_SENSORS)
        {
#if (MCU_MAX_SENSORS > 32)
            cmp_key[0] = 16;
            cmp_key[1] = 16;
            cmp_key[2] = MCU_MAX_SENSORS - 32;
#endif
        }
        else if (16 < MCU_MAX_SENSORS)
        {
#if (MCU_MAX_SENSORS > 16)
            cmp_key[0] = 16;
            cmp_key[1] = MCU_MAX_SENSORS - 16;
            cmp_key[2] = 0;
#endif
        }
        else
        {
            cmp_key[0] = MCU_MAX_SENSORS;
            cmp_key[1] = 0;
            cmp_key[2] = 0;
        }

        for (loop = 0; loop < 3; loop++)
        {
            for (key_num = 0; key_num < cmp_key[loop]; key_num++)
            {
                if (0 != (chac_stor & (1 << key_num)))
                {
                     *(g_key_info[method].sensor_index + pt) = used_key++;
                }
                else
                {
                     *(g_key_info[method].sensor_index + pt) =  0xFF;
                }
                pt = pt + 1;
            }
            if(0 == loop)
            {
                //chac_stor  = g_ctsu_parameter[method].ctsu_chac23;
                chac_stor  = p_ctsu_cfgs[method]->ctsuchac2 | (p_ctsu_cfgs[method]->ctsuchac3 << 8);
            }
            else if (1 == loop)
            {
                //chac_stor  = g_ctsu_parameter[method].ctsu_chac4;
                chac_stor  = p_ctsu_cfgs[method]->ctsuchac4;
            }
        }
    }
    else
    {
        for (key_num = 0, used_key = 0; key_num < g_key_info[method].ena_num; key_num++)
        {
            if (16 > key_num)
            {
                if (0 != (*g_key_info[method].key_used_info & (0x0001 << key_num)))
                {
                     *(g_key_info[method].sensor_index + key_num) = used_key++;
                }
                else
                {
                     *(g_key_info[method].sensor_index + key_num) =  0xFF;
                }
            }
            else if (32 > key_num)
            {
                if (0 != (*(g_key_info[method].key_used_info + 1) & (0x0001 << (key_num - 16))))
                {
                     *(g_key_info[method].sensor_index + key_num) = used_key++;
                }
                else
                {
                     *(g_key_info[method].sensor_index + key_num) =  0xFF;
                }
            }
            else if (48 > key_num)
            {
                if (0 != (*(g_key_info[method].key_used_info + 2) & (0x0001 << (key_num - 32))))
                {
                     *(g_key_info[method].sensor_index + key_num) = used_key++;
                }
                else
                {
                     *(g_key_info[method].sensor_index + key_num) =  0xFF;
                }
            }
            else
            {
                if (0 != (*(g_key_info[method].key_used_info + 3) & (0x0001 << (key_num - 48))))
                {
                     *(g_key_info[method].sensor_index + key_num) = used_key++;
                }
                else
                {
                     *(g_key_info[method].sensor_index + key_num) =  0xFF;
                }
            }
        }
    }
}    /* End of function touch_sensor_index_set() */


/***********************************************************************************************************************
* Function Name: touch_calibration_check
* Description  : Key data calibration
* Arguments    : uint8_t method    : Measurement method(0-8)
* Return Value : 
***********************************************************************************************************************/
uint8_t touch_calibration_check( uint8_t method, uint8_t offset_sta )
{
#if (QE_MAX_BUTTONS != 0)
    uint8_t    loop;
    uint8_t    finish;
    uint8_t    calib_status;
#endif
    uint8_t    ret_val;

    ret_val = _0_RUN;

#if (QE_MAX_BUTTONS != 0)
    if (_1_FINISH == offset_sta)
    {
        if (1 == g_touch_function[method].flag.average)
        {
            calib_status = touch_calibration( method );

            if (_1_FINISH == calib_status)
            {
                for (loop = 0, finish = 0; loop < g_open_num_methods; loop++)
                {
                    if (0 == g_touch_function[loop].flag.calib)
                    {
                        finish = finish + 1;
                    }
                }
                if (g_open_num_methods == finish)
                {
                    g_touch_system.flag.initial = 1;
                    ret_val = _1_FINISH;
                }
            }
        }
    }
#else
        if (_1_FINISH == offset_sta)
        {
            g_touch_system.flag.initial = 1;
            ret_val = _1_FINISH;
        }
#endif    // (QE_MAX_BUTTONS != 0)

    return ret_val;
}


#if (QE_MAX_BUTTONS != 0)
/***********************************************************************************************************************
* Function Name: touch_calibration
* Description  : Key data calibration
* Arguments    : uint8_t method    : Measurement method(0-8)
* Return Value : uint8_t status    : CALIB_RUN    : 0
*              :                   : CALIB_FINISH : 1
***********************************************************************************************************************/
uint8_t touch_calibration( uint8_t method )
{
    uint8_t loop;
    uint8_t status;

    status = _0_RUN;

    while (1 == g_touch_function[method].flag.calib)
    {
        if (TOUCH_SELF_MODE == g_key_info[method].mode)
        {
            for (loop = 0; loop < MCU_MAX_SENSORS; loop++)
            {
                if (KEY_ENABLE == touch_key_function_check( method, loop ))
                {
                    touch_initial_reference_set( method, loop );
                }
            }
        }
        else
        {
            for (loop = 0; loop < g_key_info[method].ena_num; loop++)
            {
                if (KEY_ENABLE == touch_key_function_check( method, loop ))
                {
                    touch_initial_reference_set( method, loop );
                }
            }
        }

        if (0 == g_touch_function[method].flag.calib)
        {
            g_calib_info[method].calib_cnt = 0;
            status       = _1_FINISH;
        }
        else
        {
            g_calib_info[method].calib_key = 0;
            g_calib_info[method].calib_cnt = (uint8_t)(g_calib_info[method].calib_cnt + 1);
            return status;
        }
    }

    return status;
}    /* End of function touch_calibration() */


/***********************************************************************************************************************
* Function Name: touch_initial_reference_set
* Description  : Reference value setting
* Arguments    : uint8_t method    : Measurement method(0-8)
*              : uint8_t number    : Key number
* Return Value : none
***********************************************************************************************************************/
void touch_initial_reference_set( uint8_t method, uint8_t number )
{
    uint8_t     loop;
    uint8_t     loop_max;
    uint8_t     key_pt;
    uint8_t     cnt_pt;
    uint8_t     offset;
    uint8_t     max_num;
    uint32_t    work_buff[6];

    offset = 0;

    if (0 == g_calib_info[method].calib_key )
    {
        key_pt = 0;
    }
    else
    {
        key_pt = 4 * g_calib_info[method].calib_key;
    }

    if (0 == g_calib_info[method].calib_cnt )
    {
        cnt_pt = 0;
    }
    else
    {
        cnt_pt = g_calib_info[method].calib_cnt;
    }

    if (QE_CALIB_TIME != g_calib_info[method].calib_cnt)
    {
        if (TOUCH_SELF_MODE == g_key_info[method].mode)
        {
             *(g_calib_info[method].calib_data + key_pt + cnt_pt) = *(g_self_sensor_cnt_pt[method] + (*(g_key_info[method].sensor_index + number)));
        }
        else
        {
             *(g_calib_info[method].calib_data + key_pt + cnt_pt) = *(g_mutual_sensor_diff_pt[method] + number);
        }
        g_calib_info[method].calib_key = (uint8_t)(g_calib_info[method].calib_key + 1);
        return;
    }

    for (loop = 0; loop < g_key_info[method].key_num; loop++)
    {
        work_buff[0] = (uint32_t)(*(g_calib_info[method].calib_data + offset    ) + *(g_calib_info[method].calib_data + offset + 1));    /* buff0 + buff1 */
        work_buff[1] = (uint32_t)(*(g_calib_info[method].calib_data + offset    ) + *(g_calib_info[method].calib_data + offset + 2));    /* buff0 + buff2 */
        work_buff[2] = (uint32_t)(*(g_calib_info[method].calib_data + offset    ) + *(g_calib_info[method].calib_data + offset + 3));    /* buff0 + buff3 */
        work_buff[3] = (uint32_t)(*(g_calib_info[method].calib_data + offset + 1) + *(g_calib_info[method].calib_data + offset + 2));    /* buff1 + buff2 */
        work_buff[4] = (uint32_t)(*(g_calib_info[method].calib_data + offset + 1) + *(g_calib_info[method].calib_data + offset + 3));    /* buff1 + buff3 */
        work_buff[5] = (uint32_t)(*(g_calib_info[method].calib_data + offset + 2) + *(g_calib_info[method].calib_data + offset + 3));    /* buff2 + buff3 */

        for (loop_max = 0, max_num = 0; loop_max < 5; loop_max++)
        {
            if (work_buff[max_num] < work_buff[loop_max + 1])
            {
                max_num = (uint8_t)(loop_max + 1);
            }
        }
         *(g_key_info[method].ref + loop) = (uint16_t)(work_buff[max_num] / 2);
        offset = offset + 4;
    }
    g_touch_function[method].flag.calib = 0;
}    /* End of function touch_initial_reference_set() */

#endif    // (QE_MAX_BUTTONS != 0)


/***********************************************************************************************************************
* Function Name: touch_key_decode
* Description  : Touch Key decoding
* Arguments    : uint8_t  method    : Measurement method(0-8)
*              : uint16_t value     : Sensor data
*              : uint8_t  number    : Key number
* Return Value : None
***********************************************************************************************************************/
void touch_key_decode( uint8_t method, uint16_t value, uint8_t number )
{
    uint8_t    key_id;
    uint8_t    offset;
    uint16_t   sBit;

    touch_judgement_parameter_create( method, value );          /* Make ON/OFFdecision-value */

    if (KEY_GROUP0 > number)
    {
        key_id = 0;
        offset = number;
    }
    else if (KEY_GROUP1 > number)
    {
        key_id = 1;
        offset = (uint8_t)(number - KEY_GROUP0);
    }
    else if (KEY_GROUP2 > number)
    {
        key_id = 2;
        offset = (uint8_t)(number - KEY_GROUP1);
    }
    else if (KEY_GROUP3 > number)
    {
        key_id = 3;
        offset = (uint8_t)(number - KEY_GROUP2);
    }
    else
    {
        return;
    }

    sBit = (uint16_t)(0x0001 << offset);
    if (0x0000 == (*(g_key_info[method].key_used_info + key_id) & sBit))
    {
        return;
    }

    touch_judgement( method, value, key_id, offset );             /* ON/OFF judgment               */
    touch_drift_correction( method, value, key_id, offset);       /* Drift correction              */

    if ((g_key_info[method].key_num - 1) == g_data_tim)
    {
        g_data_tim = 0;
    }
    else
    {
        g_data_tim = (uint8_t)(g_data_tim + 1);
    }
}    /* End of function touch_key_decode() */


/***********************************************************************************************************************
* Function Name: touch_judgement_parameter_create
* Description  : Touch/non-touch judgement parameter creatting function. Create parameters are g_dcount and g_cthr.
* Arguments    : value    Measurement value
* Return Value : None
***********************************************************************************************************************/
void touch_judgement_parameter_create( uint8_t method, uint16_t value )
{
    uint16_t over_cnt;

    if (TOUCH_SELF_MODE == g_key_info[method].mode)
    {
        if ( *(g_key_info[method].ref  + g_data_tim) < value )
        {
            /* Dcount = g_nref - g_ncount */
             *(g_key_info[method].delta  + g_data_tim) = (uint16_t)(value  - *(g_key_info[method].ref  + g_data_tim));
        }
        else
        {
             *(g_key_info[method].delta  + g_data_tim) = 0;
        }

        over_cnt = (65535 - (*(g_key_info[method].user_thr + g_data_tim)));

        if (over_cnt > *(g_key_info[method].ref  + g_data_tim))
        {
             *(g_key_info[method].thr  + g_data_tim) = (uint16_t)(*(g_key_info[method].ref  + g_data_tim) + *(g_key_info[method].user_thr + g_data_tim));
        }
        else
        {
             *(g_key_info[method].thr  + g_data_tim) = 65535;
        }
    }
    else
    {
        if (*(g_key_info[method].ref  + g_data_tim) > value)
        {
            *(g_key_info[method].delta  + g_data_tim) = (uint16_t)(*(g_key_info[method].ref  + g_data_tim) - value);
        }
        else
        {
             *(g_key_info[method].delta  + g_data_tim) = 0;
        }

        if (*(g_key_info[method].ref  + g_data_tim) > *(g_key_info[method].thr  + g_data_tim))
        {
             *(g_key_info[method].thr  + g_data_tim) = (uint16_t)(*(g_key_info[method].ref  + g_data_tim) - *(g_key_info[method].user_thr + g_data_tim));
        }
        else
        {
             *(g_key_info[method].thr  + g_data_tim) = 0;
        }
    }
}    /* End of function touch_judgement_parameter_create() */


/***********************************************************************************************************************
* Function Name: touch_judgement
* Description  : 
* Arguments    : value    Measurement value
*              : group    Key touch/non-touch data storage buffer ID
*              : offset   Key number offset value
* Return Value : None
***********************************************************************************************************************/
void touch_judgement( uint8_t method, uint16_t value, uint8_t group, uint8_t offset )
{
    uint16_t    sBit;
    uint32_t    ntouch_val;
    uint8_t     freq;

    if (TOUCH_SELF_MODE == g_key_info[method].mode)
    {
        if (*(g_key_info[method].thr + g_data_tim) > *(g_key_info[method].hys + g_data_tim))
        {
            ntouch_val = (uint16_t)((*(g_key_info[method].thr + g_data_tim)) - (*(g_key_info[method].hys + g_data_tim)));
        }
        else
        {
            ntouch_val = *(g_key_info[method].thr + g_data_tim);
        }
    
        sBit = (uint16_t)( 0x0001 << offset );
        if (value > *(g_key_info[method].thr  + g_data_tim))               /* Measurement value > Threshold = Touch   */
        {
             *(g_key_info[method].in_touch + group) |= sBit;               /* Inside touch                            */
        }
        else if (value < ntouch_val )                                    /* Measurement value < Threshold = Non-Touch */
        {
             *(g_key_info[method].in_touch + group) &= (~sBit);            /* Inside non-touch                        */
        }
        else
        {
            /* Do Nothing */
        }
    }
    else                                                                   /* Mutual-capacitance measurement          */
    {
        ntouch_val = (uint32_t)(*(g_key_info[method].thr + g_data_tim)) + (uint32_t)(*(g_key_info[method].hys + g_data_tim));

        sBit = (uint16_t)( 0x0001 << offset );
        if (value < *(g_key_info[method].thr + g_data_tim))                /* Measurement value < Threshold = Touch   */
        {
             *(g_key_info[method].in_touch + group) |= sBit;               /* Inside touch                            */
        }
        else if ((uint32_t)(value) > ntouch_val)                         /* Measurement value > Threshold = Non-Touch */
        {
             *(g_key_info[method].in_touch + group) &= (~sBit);            /* Inside non-touch                        */
        }
        else
        {
            /* Do Nothing */
        }
    }

    if (0 != (*(g_key_info[method].in_touch + group) & sBit))              /* Inside touch flag check                 */
    {
         *(g_key_info[method].non_touch_cnt  + g_data_tim) = 0;            /* Inside non-touch counter clear          */

        if ( OFF == g_touch_function[method].flag.acd0 )                   /* Total touch function enable flag check  */
        {
            freq = 1;
        }
        else
        {
            freq = g_touch_paramter[method].touch_freq;
        }

        if ((*(g_key_info[method].touch_cnt  + g_data_tim)) == (uint16_t)freq)        /* Outside touch                */
        {
             *(g_key_info[method].out_touch  + group) = (uint16_t)(*(g_key_info[method].out_touch + group) | sBit);    /* Outside touch */
            if (0 != g_touch_paramter[method].msa_freq)
            {
                *(g_key_info[method].touch_cnt  + g_data_tim) = *(g_key_info[method].touch_cnt  + g_data_tim) + 1;     /* Total touch count up */
            }
        }
        else
        {
             *(g_key_info[method].touch_cnt  + g_data_tim) = *(g_key_info[method].touch_cnt  + g_data_tim) + 1;        /* Total touch count up */
        }
    }
    else                                                               /* Outside non-touch                           */
    {
         *(g_key_info[method].touch_cnt  + g_data_tim) = 0;            /* Inside touch counter clear                  */

        if ( OFF == g_touch_function[method].flag.acd1 )               /* Total non-touch function enable flag check  */
        {
            freq = 1;
        }
        else
        {
            freq = g_touch_paramter[method].not_touch_freq;
        }

        if ((*(g_key_info[method].non_touch_cnt  + g_data_tim)) == (uint16_t)freq)    /* Outside non-touch            */
        {
             *(g_key_info[method].out_touch  + group) = (uint16_t)(*(g_key_info[method].out_touch  + group) & (~sBit));    /* Outside non-touch */
        }
        else
        {
             *(g_key_info[method].non_touch_cnt  + g_data_tim) = *(g_key_info[method].non_touch_cnt  + g_data_tim) + 1;    /* Total non-touch count up */
        }
    }

    /* ===== The reset judgment processing at the time of continuation on ===== */
    if (1 == g_touch_function[method].flag.msa)
    {
        if (0 != g_touch_paramter[method].msa_freq)
        {
            /* If reaching (c_msa(DF_MSA_DATA) + AcdON threshold),
                it makes OnOff judgment result off and it revises a drift. */
            if ((*(g_key_info[method].touch_cnt  + g_data_tim)) == g_touch_paramter[method].msa_freq + (uint16_t)freq)
            {
                /* Real touch and Virtual touch OFF */
                 *(g_key_info[method].out_touch  + group)       &= (~sBit);
                 *(g_key_info[method].in_touch   + group)       &= (~sBit);
                 *(g_key_info[method].touch_cnt  + g_data_tim)   = 0;

                /* parameter reset */
                 *(g_key_info[method].drift_cnt     + g_data_tim)   = 0;
                 *(g_key_info[method].drift_add_ref + g_data_tim)   = 0x00000000ul;
                 *(g_key_info[method].ref           + g_data_tim)   = value;
            }
        }
    }

    /* ===== ONOFF final result ===== */
    if (0 != (*(g_key_info[method].out_touch  + group) & sBit))
    {
         *(g_key_info[method].touch_result + group) |= sBit;
    }
    else
    {
         *(g_key_info[method].touch_result + group) &= (~sBit);
    }
}    /* End of function touch_judgement() */


/***********************************************************************************************************************
* Function Name: touch_drift_correction
* Description  : Drift correction function ( Reference value update )
* Arguments    : value    Measurement value
*              : group    Key touch/non-touch data storage buffer ID
*              : offset   Key number offset value
* Return Value : DRIFT_OK(0)
*              : DRIFT_OFF(2)
***********************************************************************************************************************/
uint8_t touch_drift_correction( uint8_t method, uint16_t value, uint8_t group, uint8_t offset )
{
    uint16_t    sBit;

    /* If the drift processing is a prohibition */
    if (OFF == g_touch_function[method].flag.drift)
    {
        /* There is no processing */
        return DRIFT_OFF;
    }

    sBit = (uint16_t)( 0x0001 << offset );
    /* In case of doing drift correction being and moreover On/Off judgment result 1=OFF */
    if ((0x0000ul == (*(g_key_info[method].in_touch   + group) & sBit)) && (0x0000ul != (*(g_key_info[method].drift_permission + group) & sBit)))
    {
        /* It is an addition for the drift correction average calculation */
         *(g_key_info[method].drift_add_ref  + g_data_tim) = *(g_key_info[method].drift_add_ref + g_data_tim) + value;
        /* Drift correction counter's being incremented */
         *(g_key_info[method].drift_cnt  + g_data_tim) = (uint16_t)(*(g_key_info[method].drift_cnt  + g_data_tim) + 1);

        if (*(g_key_info[method].drift_cnt  + g_data_tim) <= g_touch_paramter[method].drift_freq)
        {
            /* If reaching the correction number of times */
            if (*(g_key_info[method].drift_cnt  + g_data_tim) == g_touch_paramter[method].drift_freq)
            {
                if (g_touch_paramter[method].drift_freq == 0)
                {
                    g_touch_paramter[method].drift_freq = 1;
                }
                 *(g_key_info[method].ref  + g_data_tim)  = (uint16_t)(*(g_key_info[method].drift_add_ref  + g_data_tim) / g_touch_paramter[method].drift_freq);

                /* To REF of the average */
                 *(g_key_info[method].drift_add_ref  + g_data_tim) = 0x0000000ul;
                /* Work clear */
                 *(g_key_info[method].drift_cnt      + g_data_tim) = 0;
            }
        }
        else
        {
            *(g_key_info[method].drift_add_ref  + g_data_tim) = 0x0000000ul;
            *(g_key_info[method].drift_cnt      + g_data_tim) = 0;
        }
    }
    else
    {
         *(g_key_info[method].drift_add_ref  + g_data_tim) = 0x00000000ul;
         *(g_key_info[method].drift_cnt      + g_data_tim) = 0;
    }
    return DRIFT_OK;
}


/***********************************************************************************************************************
* Function Name: touch_key_function_check
* Description  : 
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
uint8_t touch_key_function_check( uint8_t method, uint8_t loop )
{
    uint8_t     key_id;
    uint8_t     offset;
    uint16_t    sBit;

    if (KEY_GROUP0 > loop)
    {
        key_id = 0;
        offset = loop;
    }
    else if (KEY_GROUP1 > loop)
    {
        key_id = 1;
        offset = (uint8_t)(loop - KEY_GROUP0);
    }
    else if (KEY_GROUP2 > loop)
    {
        key_id = 2;
        offset = (uint8_t)(loop - KEY_GROUP1);
    }
    else if (KEY_GROUP3 > loop)
    {
        key_id = 3;
        offset = (uint8_t)(loop - KEY_GROUP2);
    }
    else
    {
        return KEY_DISENABLE;
    }

    sBit = (uint16_t)(0x0001 << offset);
    if (0x0000 == (*(g_key_info[method].key_used_info + key_id) & sBit))
    {
        return KEY_DISENABLE;
    }

    return KEY_ENABLE;
}


/***********************************************************************************************************************
* Function Name: offset_tuning_stop_judgement
* Description  : 
* Arguments    : 
* Return Value : 
***********************************************************************************************************************/
uint8_t offset_tuning_stop_judgement( uint8_t method )
{
    uint8_t status;
    uint8_t group;

    status = _1_START;

#if (QE_MAX_BUTTONS != 0)
    if (TOUCH_SELF_MODE == g_key_info[method].mode)
    {
        if (0 != g_key_info[method].key_num)
        {
            for (group = 0; group < g_key_info[method].key_max_group; group++)
            {
                if (0x0000 !=  *(g_key_info[method].touch_result + group))
                {
                    status = _0_STOP;
                }
            }
        }
    }
    else
    {
        for (group = 0; group < g_key_info[method].key_max_group; group++)
        {
            if (0x0000 !=  *(g_key_info[method].touch_result + group))
            {
                status = _0_STOP;
            }
        }
    }
#endif    // (QE_MAX_BUTTONS != 0)

#if (QE_MAX_SLIDERS != 0)
    for (group = 0; group < gp_touch_configs[method]->num_sliders; group++)              /* Slider group loop                                 */
    {
        if (0xFFFF != gp_sliderInfo[group].value)                       /* Slider touch check                                */
        {
            status = _0_STOP;                                    /* Internal offset tuning flag stop setting          */
        }
    }
#endif

#if (QE_MAX_WHEELS != 0)
    for (group = 0; group < gp_touch_configs[method]->num_wheels; group++)               /* Wheel group loop                                  */
    {
        if (0xFFFF != gp_wheelInfo[group].value)
        {
            status = _0_STOP;                                    /* Internal offset tuning flag stop setting          */
        }
    }
#endif

    return status;
}


#if (QE_MAX_SLIDERS != 0)
/***********************************************************************************************************************
* Function Name: slider_decode
* Description  : Slider decode function
* Arguments    : s_id           Slider ID( 0-7 )
* Return Value : slider_rpos    Slider touch position( 65535 = non-touch )
***********************************************************************************************************************/
uint16_t slider_decode( uint8_t s_id )
{
    uint8_t     loop;
    uint8_t     max_num;
    uint8_t     maxch1[3];
    uint16_t    d1,d2,d3;
    uint16_t    slider_vpos;        /* Slider virtual position        */
    uint16_t    slider_rpos;
    uint8_t     resol_num;
    uint16_t    resol_plus;
    uint16_t    resol_minus;
    uint16_t    dsum;           // GI: replaces g_slider_dsum[s_id]

    /* Unstable Correction */
    for (loop = 0, max_num = 0; loop < (gp_sliderInfo[s_id].num_elements-1); loop++)
    {
        if (g_slider_data[max_num] < g_slider_data[loop+1])
        {
            max_num = loop + 1;
        }
    }

    /*The maximum ch number to change is put in the array. */
    maxch1[0] = max_num;

    /* Array making for slider operation-------------*/
    /*     |    Maximum change CH_No -----> Array"0"    */
    /*     |    Maximum change CH_No + 1 -> Array"2"    */
    /*     |    Maximum change CH_No - 1 -> Array"1"    */
    if (0u < g_slider_data[maxch1[0]])
    {
        if (0u == maxch1[0])
        {
            maxch1[1] = (uint8_t)(maxch1[0] + 2);
            maxch1[2] = (uint8_t)(maxch1[0] + 1);
        }
        else if (maxch1[0] == (gp_sliderInfo[s_id].num_elements - 1))
        {
            maxch1[1] = (uint8_t)(maxch1[0] - 1);
            maxch1[2] = (uint8_t)(maxch1[0] - 2);
        }
        else
        {
            maxch1[1] = (uint8_t)(maxch1[0] - 1);
            maxch1[2] = (uint8_t)(maxch1[0] + 1);
        }

        /* Constant decision for operation of angle of slider */
        /*    Three constant decisions for the operation      */
        /*    Operation of angle of slider                    */
        d1 = g_slider_data[maxch1[0]] - g_slider_data[maxch1[1]];
        d2 = g_slider_data[maxch1[0]] - g_slider_data[maxch1[2]];
        dsum = (uint16_t)(d1 + d2);

        if(dsum > gp_sliderInfo[s_id].thr_offset)
        {
            dsum = dsum - gp_sliderInfo[s_id].thr_offset;
        }
        else
        {
            dsum = 0;
        }

        if (dsum > gp_sliderInfo[s_id].threshold)
        {
            if (ZERO_ENA != slider_decode_abnormal_judgement( s_id, d1, d2, maxch1[0] ))
            {
                 d3          = 100 + ((d2 * 100) / d1);
                 slider_vpos = ((10000 / d3) + (100 * maxch1[0]));

                 resol_num   = (uint8_t)(gp_sliderInfo[s_id].num_elements - 1);
                 resol_plus  = (uint16_t)(100 * resol_num);
                 resol_minus = (uint16_t)(50 / gp_sliderInfo[s_id].resolution);

                 /* swa = 0 -> Max ------ swa output 1 to Max */
                 slider_rpos = (uint16_t)(slider_vpos / gp_sliderInfo[s_id].resolution);
                 if (0 == slider_rpos)
                 {
                     slider_rpos = 1;
                 }
                 else if (slider_rpos >= (resol_plus / gp_sliderInfo[s_id].resolution))
                 {
                     slider_rpos = (slider_rpos - (resol_plus / gp_sliderInfo[s_id].resolution)) * 2;
                     slider_rpos = slider_rpos + (resol_plus / gp_sliderInfo[s_id].resolution);
                     if (slider_rpos > ((100 / gp_sliderInfo[s_id].resolution) * gp_sliderInfo[s_id].num_elements))
                     {
                         slider_rpos = ((100 / gp_sliderInfo[s_id].resolution) * gp_sliderInfo[s_id].num_elements);
                     }
                 }
                 else if (slider_rpos <= (100 / gp_sliderInfo[s_id].resolution))
                 {
                     if( slider_rpos < resol_minus )
                     {
                        slider_rpos = 1;
                     }
                     else
                     {
                         slider_rpos = slider_rpos - resol_minus;
                         if( slider_rpos == 0 )
                         {
                             slider_rpos = 1;
                         }
                         else
                         {
                             slider_rpos = slider_rpos * 2;
                         }
                     }
                 }
                 else
                 {
                     /* Do Nothing */
                 }
                 /* GI: Scale results to be 0-100 */
                 slider_rpos /= gp_sliderInfo[s_id].num_elements;
                 gp_sliderInfo[s_id].value = slider_rpos;
             }
          }
        else
        {
            gp_sliderInfo[s_id].value = 0xFFFFu;
            slider_rpos = 0xFFFFu;
        }
    }
    else
    {
        /* Touch Off Value */
        gp_sliderInfo[s_id].value = 0xFFFFu;
        slider_rpos = 0xFFFFu;
    }
    return slider_rpos;
}    /* End of function slider_decode() */


/***********************************************************************************************************************
* Function Name: slider_decode_abnormal_judgement
* Description  : 0 operation removal
* Arguments    : id            slider ID ( 0-7 )
*              : delta1        delta value 1
*              : delta2        delta value 2
*              : maxch         Max measurement value sensor number
* Return Value : status        ZERO_ENA (0)
*              :               ZERO_DIS (1)
***********************************************************************************************************************/
uint8_t slider_decode_abnormal_judgement(uint8_t id, uint16_t delta1, uint16_t delta2, uint8_t maxch)
{
    uint8_t status;

    status = ZERO_DIS;

    if( 4 == gp_sliderInfo[id].num_elements )
    {
        if (0x0000 == delta1)
        {
            if (0 == maxch)
            {
                gp_sliderInfo[id].value = 1;
            }
            else
            {
                gp_sliderInfo[id].value = (100 / gp_sliderInfo[id].resolution) * maxch;
            }
            status = ZERO_ENA;
        }
        else if (0x0000 == delta2)
        {
            gp_sliderInfo[id].value = (100 / gp_sliderInfo[id].resolution) * (maxch + 1);
            status = ZERO_ENA;
        }
    }
    return status;
}    /* End of function slider_decode_abnormal_judgement() */

#endif    // SLIDER_USE


#if (QE_MAX_WHEELS != 0)
/***********************************************************************************************************************
* Function Name: wheel_decode
* Description  : Wheel Decode function
* Arguments    : w_id          Wheel ID ( 0-7 )
* Return Value : wheel_rpos    Wheel touch position( 65535 = non-touch )
***********************************************************************************************************************/
uint16_t wheel_decode( uint8_t w_id )
{
    uint8_t     loop;
    uint8_t     max_num;
    uint8_t     maxch[3];
    uint16_t    d1,d2,d3;
    uint16_t    wheel_vpos;        /* Wheel virtual position    */
    uint16_t    wheel_rpos;
    uint16_t    dsum;           // GI: replaces g_wheel_dsum[w_id]

    /* Unstable Correction */
    for (loop = 0, max_num = 0; loop < (gp_wheelInfo[w_id].num_elements-1); loop++)
    {
        if (g_wheel_data[max_num] < g_wheel_data[loop+1])
        {
            max_num = loop + 1;
        }
    }

    /*The maximum ch number to change is put in the array. */
    maxch[0] = max_num;

    /* Array making for slider operation          */
    /*    Maximum change CH_No -----> Array"0"    */
    /*    Maximum change CH_No + 1 -> Array"2"    */
    /*    Maximum change CH_No - 1 -> Array"1"    */
    if (0u < g_wheel_data[maxch[0]])
    {
        if (0u == maxch[0])
        {
            maxch[1] = (uint8_t)(gp_wheelInfo[w_id].num_elements - 1);
            maxch[2] = (uint8_t)(maxch[0] + 1);
        }
        else if ( maxch[0] == ( gp_wheelInfo[w_id].num_elements - 1 ))
        {
            maxch[1] = (uint8_t)(maxch[0] - 1);
            maxch[2] = 0;
        }
        else
        {
            maxch[1] = (uint8_t)(maxch[0] - 1u);
            maxch[2] = (uint8_t)(maxch[0] + 1u);
        }

        /* Constant decision for operation of angle of wheel    */
        /*    Three constant decisions for the operation        */
        /*    Operation of angle of wheel                       */
        d1 = g_wheel_data[maxch[0]] - g_wheel_data[maxch[1]];
        d2 = g_wheel_data[maxch[0]] - g_wheel_data[maxch[2]];
        dsum = (uint16_t)(d1 + d2);

        if(dsum > gp_wheelInfo[w_id].thr_offset)
        {
            dsum = dsum - gp_wheelInfo[w_id].thr_offset;
        }
        else
        {
            dsum = 0;
        }

        if (dsum > gp_wheelInfo[w_id].threshold)
        {
            if (ZERO_ENA != wheel_decode_abnormal_judgement( w_id, d1, d2, maxch[0] ))
            {
                d3 = 100 + ((d2 * 100) / d1);

                if( 4 == gp_wheelInfo[w_id].num_elements )
                {
                    wheel_vpos = ((9000 / d3) + ( 90 * maxch[0] ));
                }
                else
                {
                    wheel_vpos = ((4500 / d3) + ( 45 * maxch[0] ));
                }

                /* Angle division output */
                /* diff_angle_ch = 0 -> 359 ------ diff_angle_ch output 1 to 360 */
                if (0 == wheel_vpos)
                {
                    wheel_vpos = 360;
                }
                else if (361 == wheel_vpos)
                {
                    wheel_vpos = 1;
                }
                else if (361 < wheel_vpos)
                {
                    wheel_vpos = (wheel_vpos-360) >> 1;
                }
                else
                {
                    /* Do Nothing */
                }

                /* swa = 0 -> Max ------ swa output 1 to Max */
                wheel_rpos = (uint16_t)(wheel_vpos / gp_wheelInfo[w_id].resolution);
                if (0 == wheel_rpos)
                {
                    wheel_rpos = 1;
                }
                gp_wheelInfo[w_id].value = wheel_rpos;
            }
        }
        else
        {
            gp_wheelInfo[w_id].value = 0xFFFFu;
            wheel_rpos = 0xFFFFu;
        }
    }
    else
    {
        /* Touch Off Value */
        gp_wheelInfo[w_id].value = 0xFFFFu;
        wheel_rpos = 0xFFFFu;
    }
    return wheel_rpos;
}    /* End of function wheel_decode() */


/***********************************************************************************************************************
* Function Name: wheel_decode_abnormal_judgement
* Description  : 0 operation removal
* Arguments    : id            wheel ID ( 0-7 )
*              : delta1        delta value 1
*              : delta2        delta value 2
*              : maxch         Max measurement value sensor number
* Return Value : status        ZERO_ENA (0)
*              :               ZERO_DIS (1)
***********************************************************************************************************************/
uint8_t wheel_decode_abnormal_judgement(uint8_t id, uint16_t delta1, uint16_t delta2, uint8_t maxch)
{
    uint8_t status;

    status = ZERO_DIS;

    if( 4 == gp_wheelInfo[id].num_elements )
    {
        if (0x0000 == delta1)
        {
            if (0 == maxch)
            {
                gp_wheelInfo[id].value = (360 / gp_wheelInfo[id].resolution);
            }
            else
            {
                gp_wheelInfo[id].value = (90 / gp_wheelInfo[id].resolution) * maxch;
            }
            status = ZERO_ENA;
        }
        else if (0x0000 == delta2)
        {
            gp_wheelInfo[id].value = (90 / gp_wheelInfo[id].resolution) * (maxch + 1);
            status = ZERO_ENA;
        }
    }
    else
    {
        if (0x0000 == delta1)
        {
            if (0 == maxch)
            {
                gp_wheelInfo[id].value = (360 / gp_wheelInfo[id].resolution);
            }
            else
            {
                gp_wheelInfo[id].value = (45 / gp_wheelInfo[id].resolution) * maxch;
            }
            status = ZERO_ENA;
        }
        else if (0x0000 == delta2)
        {
            gp_wheelInfo[id].value = (45 / gp_wheelInfo[id].resolution) * (maxch + 1);
            status = ZERO_ENA;
        }
    }

    return status;
}    /* End of function wheel_decode_abnormal_judgement() */

#endif    // (QE_MAX_WHEELS != 0)
