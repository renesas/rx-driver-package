/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
* THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
************************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_touch_wrapper_qe.c
* Description  : This module implements the FIT_v1.11 wrapper API.
***********************************************************************************************************************/

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <string.h>
#include "platform.h"
#include "rm_touch_qe_if.h"
#include "r_touch_wrapper_qe.h"

#if (1 == QE_WRAPPER_ENABLE)
/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/
/* Driver Version Number. */
#define QETOUCH_RX_VERSION_MAJOR (1)
#define QETOUCH_RX_VERSION_MINOR (11)

#define TOUCH_WRAPPER_SENSOR_SATURATION     (52000) /* top of linear portion of correction curve */
#define TOUCH_WRAPPER_TUNING_MAX            (0x03FF)
#define TOUCH_WRAPPER_TUNING_MIN            (0x0000)
#define TOUCH_WRAPPER_TUNING_OT_COUNT       (25)
#define TOUCH_WRAPPER_SENSOR_OVERFLOW_COUNT (0xFFFF)
#define TOUCH_WRAPPER_ABNORMAL_TSCAP_COUNT  (10)

/***********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
* Private global variables and functions
***********************************************************************************************************************/

static qe_err_t update_data(void);
static qe_err_t get_ctsu_err(uint8_t method, ctsu_status_t ctsu_status);

touch_mlist_t g_mlist;
uint8_t g_method_stor = 0;
uint8_t g_open_num_methods;
static touch_sensor_t g_bad_sensor; // offset tuning error details
static bool g_cycle_methods;
static uint8_t g_set_method;
static uint8_t g_last_method;

volatile bool g_qetouch_timer_flg;

/***********************************************************************************************************************
* Function Name: qetouch_timer_callback
* Description  : Callback function for periodic timer which indicates to start next scan.
* Arguments    : p_args
*                    (unused)
* Return Value : none
***********************************************************************************************************************/
void qetouch_timer_callback(void *p_args)
{
    g_qetouch_timer_flg = true;
} /* End of function qetouch_timer_callback() */

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
*                QE_ERR_ABNORMAL_TSCAP-
*                    TSCAP error detected during scan for correction or offset tuning
*                QE_ERR_SENSOR_OVERFLOW-
*                    Sensor overflow detected during scan for correction or offset tuning
*                QE_ERR_SENSOR_SATURATION -
*                    Initial sensor value beyond linear portion of correction curve.
*                QE_ERR_OT_MAX_OFFSET -
*                    Cannot tune SO0 offset any higher
*                QE_ERR_OT_MIN_OFFSET -
*                    Cannot tune SO0 offset any lower
*                QE_ERR_OT_MAX_ATTEMPTS -
*                    Maximum scans performed and all sensors still not in target window.
***********************************************************************************************************************/
qe_err_t R_TOUCH_Open(ctsu_cfg_t *p_ctsu_cfgs[], touch_cfg_t *p_touch_cfgs[], uint8_t num_methods, qe_trig_t trigger)
{
    volatile qe_err_t err = QE_SUCCESS;
    fsp_err_t fsp_err;
    uint8_t i;
    uint8_t j;
    uint64_t dummy;
    uint16_t slider_dummy;
    uint16_t wheel_dummy;
    ctsu_status_t ctsu_status;
    uint8_t tuning_ot_count;
    touch_instance_ctrl_t *p_touch_instance_ctrl;
    ctsu_instance_ctrl_t *p_ctsu_instance_ctrl;

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

    for (i = 0; i < num_methods; i++)
    {
        fsp_err = RM_TOUCH_Open((touch_ctrl_t *)p_touch_cfgs[i], (touch_cfg_t *)p_ctsu_cfgs[i]);
        if (FSP_ERR_ALREADY_OPEN == fsp_err)
        {
            err = QE_ERR_ALREADY_OPEN;
            return err;
        }

        p_touch_instance_ctrl = (touch_instance_ctrl_t *)p_touch_cfgs[i];
        p_ctsu_instance_ctrl = (ctsu_instance_ctrl_t *)p_touch_instance_ctrl->p_ctsu_instance->p_ctrl;

#if (BSP_FEATURE_CTSU_VERSION == 1)
        if (p_ctsu_instance_ctrl->p_correction_info->first_val >= TOUCH_WRAPPER_SENSOR_SATURATION)
        {
            err = QE_ERR_SENSOR_SATURATION;
        }
#endif

        /* Measure with a software trigger for initial offset tuning. */
        (p_ctsu_instance_ctrl->cap) = CTSU_CAP_SOFTWARE;
    }

    /* by default, set up to cycle through all methods, starting with method 0 */
    g_mlist.num_methods = num_methods;
    for (i = 0; i < num_methods; i++)
    {
        g_mlist.methods[i] = i;
    }

    /* set up global pointers for method 0 */
    g_method_stor = 0;
    g_cycle_methods = true;

    /* initial offset tuning */
    for (i = 0; i < num_methods; i++)
    {
        tuning_ot_count = 0;
        p_touch_instance_ctrl = (touch_instance_ctrl_t *)p_touch_cfgs[i];
        p_ctsu_instance_ctrl = (ctsu_instance_ctrl_t *)p_touch_instance_ctrl->p_ctsu_instance->p_ctrl;

        while (1)
        {
            fsp_err = RM_TOUCH_ScanStart((touch_ctrl_t *)p_touch_cfgs[i]);
            if (FSP_SUCCESS != fsp_err)
            {
                err = QE_ERR_BUSY;
                return err;
            }
            while (0 == g_qe_touch_flag)
            {
            }
            g_qe_touch_flag = 0;
            fsp_err = RM_TOUCH_DataGet((touch_ctrl_t *)p_touch_cfgs[i], &dummy, &slider_dummy, &wheel_dummy);

            /* Initial offset tuning error */
            for (j = 0; j < CTSU_CFG_NUM_SUMULTI; j++)
            {
#if (BSP_FEATURE_CTSU_VERSION == 2)
                if (TOUCH_WRAPPER_TUNING_MIN == (p_ctsu_instance_ctrl->p_ctsuwr[i * CTSU_CFG_NUM_SUMULTI + j].ctsuso & 0x3FF))
                {
                    err = QE_ERR_OT_MIN_OFFSET;
                }

                if (TOUCH_WRAPPER_TUNING_MAX == (p_ctsu_instance_ctrl->p_ctsuwr[i * CTSU_CFG_NUM_SUMULTI + j].ctsuso & 0x3FF))
                {
                    err = QE_ERR_OT_MAX_OFFSET;
                }
#endif
#if (BSP_FEATURE_CTSU_VERSION == 1)
                if (TOUCH_WRAPPER_TUNING_MIN == (p_ctsu_instance_ctrl->p_ctsuwr[i + j].ctsuso0 & 0x3FF))
                {
                    err = QE_ERR_OT_MIN_OFFSET;
                }

                if (TOUCH_WRAPPER_TUNING_MAX == (p_ctsu_instance_ctrl->p_ctsuwr[i + j].ctsuso0 & 0x3FF))
                {
                    err = QE_ERR_OT_MAX_OFFSET;
                }
#endif
            }

            if (TOUCH_WRAPPER_TUNING_OT_COUNT <= tuning_ot_count)
            {
                err = QE_ERR_OT_MAX_ATTEMPTS;
            }
            else
            {
                tuning_ot_count++;
            }

            /* tuning succes */
            if (FSP_SUCCESS == fsp_err)
            {
                break;
            }
            else
            {
            }
        }

        (p_ctsu_instance_ctrl->cap) = (ctsu_cap_t)trigger;

        if (QE_SUCCESS == err)
        {
            /* get error status*/
            ctsu_status.flag.sens_over = (p_ctsu_instance_ctrl->error_status & CTSU_EVENT_OVERFLOW);
            ctsu_status.flag.icomp_error = ((p_ctsu_instance_ctrl->error_status & CTSU_EVENT_ICOMP) >> 1) || ((p_ctsu_instance_ctrl->error_status & CTSU_EVENT_ICOMP1) >> 2);

            err = get_ctsu_err(g_method_stor, ctsu_status);
        }
    }

    return err;
} /* End of function R_TOUCH_UpdateData() */

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
*                QE_ERR_ABNORMAL_TSCAP-
*                    TSCAP error detected during scan
*                QE_ERR_SENSOR_OVERFLOW-
*                    Sensor overflow detected during scan
***********************************************************************************************************************/
qe_err_t R_TOUCH_UpdateDataAndStartScan(void)
{
    qe_err_t err = QE_SUCCESS;
    touch_instance_ctrl_t *p_touch_instance_ctrl;
    ctsu_instance_ctrl_t *p_ctsu_instance_ctrl;
    p_touch_instance_ctrl = (touch_instance_ctrl_t *)gp_touch_cfgs[g_method_stor];
    p_ctsu_instance_ctrl = (ctsu_instance_ctrl_t *)p_touch_instance_ctrl->p_ctsu_instance->p_ctrl;


    if (QE_TRIG_EXTERNAL == (p_ctsu_instance_ctrl->cap))
    {
        return QE_ERR_TRIGGER_TYPE;
    }

    err = update_data();

    if (QE_ERR_BUSY != err)
    {
        /* Scan start */
        RM_TOUCH_ScanStart((touch_ctrl_t *)gp_touch_cfgs[g_method_stor]);
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
    qe_err_t err = QE_SUCCESS;

    err = update_data();

    return err;
} /* End of function R_TOUCH_UpdateData() */

/***********************************************************************************************************************
* Function Name: update_data
* Description  : This function 1) Saves the raw scan data, applies correction and filter (4-deep moving average), then
*                2) Determines if any buttons, sliders, or wheels were touched and calculates results, and 3) Determines
*                if the method needs to be changed before the next scan starts, either because methods are being cycled
*                through or a TOUCH_CMD_SET_METHOD command changed the desired method.
* Arguments    : None
* Return Value : QE_SUCCESS -
*                    API initialized successfully.
*                QE_ERR_BUSY
*                    CTSU busy. Likely need more time between calling this function/starting scans.
*                QE_ERR_ABNORMAL_TSCAP-
*                    TSCAP error detected during scan
*                QE_ERR_SENSOR_OVERFLOW-
*                    Sensor overflow detected during scan
***********************************************************************************************************************/
static qe_err_t update_data(void)
{
    qe_err_t err = QE_SUCCESS;
    uint64_t dummy;
    uint16_t slider_dummy;
    uint16_t wheel_dummy;
    ctsu_status_t ctsu_status;
    touch_instance_ctrl_t *p_touch_instance_ctrl;
    ctsu_instance_ctrl_t *p_ctsu_instance_ctrl;
    fsp_err_t fsp_err;

    p_touch_instance_ctrl = (touch_instance_ctrl_t *)gp_touch_cfgs[g_method_stor];
    p_ctsu_instance_ctrl = (ctsu_instance_ctrl_t *)p_touch_instance_ctrl->p_ctsu_instance->p_ctrl;


    fsp_err = RM_TOUCH_DataGet((touch_ctrl_t *)gp_touch_cfgs[g_method_stor], &dummy, &slider_dummy, &wheel_dummy);
    if (FSP_SUCCESS != fsp_err)
    {
        if (FSP_ERR_CTSU_SCANNING == fsp_err)
        {
            err = QE_ERR_BUSY;
            return err;
        }
    }

    if (0 != p_ctsu_instance_ctrl->error_status)
    {
        /* get error status*/
        ctsu_status.flag.sens_over = (p_ctsu_instance_ctrl->error_status & CTSU_EVENT_OVERFLOW);
        ctsu_status.flag.icomp_error = ((p_ctsu_instance_ctrl->error_status & CTSU_EVENT_ICOMP) >> 1) || ((p_ctsu_instance_ctrl->error_status & CTSU_EVENT_ICOMP1) >> 2);

        err = get_ctsu_err(g_method_stor, ctsu_status);
    }
    /* control method */
    g_last_method = g_method_stor;

    if (true == g_cycle_methods)
    {
        /* set method to next in local list */
        g_mlist.cur_index++;
        if (g_mlist.cur_index >= g_mlist.num_methods)
        {
            g_mlist.cur_index = 0;
        }

        g_method_stor = g_mlist.methods[g_mlist.cur_index];
    }
    else if (g_method_stor != g_set_method)
    {
        /* set method to specific method */
        g_method_stor = g_set_method;
    }
    else
    {
        ; /* leave current method running */
    }

    return err;
} /* End of function R_TOUCH_UpdateData() */

/***********************************************************************************************************************
* Function Name: get_ctsu_err
* Description  : This functions checks the CTSU status flags for a hardware scan error. If one is detected, the
*                specific sensor at fault is identified if possible and is loaded into g_bad_sensor. This can be
*                retrieved later by the user with the Control() command TOUCH_CMD_GET_FAILED_SENSOR.
* Arguments    : method -
*                    Scan method index
*                ctsu_status
*                    CTSU status flags from scan
* Return Value : QE_SUCCESS -
*                    No hardware error detected
*                QE_ERR_ABNORMAL_TSCAP -
*                    TSCAP error detected.
*                QE_ERR_SENSOR_OVERFLOW -
*                    Sensor overflow detected.
***********************************************************************************************************************/
static qe_err_t get_ctsu_err(uint8_t method, ctsu_status_t ctsu_status)
{
    qe_err_t err = QE_SUCCESS;
    uint8_t i;
    touch_instance_ctrl_t *p_touch_instance_ctrl;
    ctsu_instance_ctrl_t *p_ctsu_instance_ctrl;

    p_touch_instance_ctrl = (touch_instance_ctrl_t *)gp_touch_cfgs[method];
    p_ctsu_instance_ctrl = (ctsu_instance_ctrl_t *)p_touch_instance_ctrl->p_ctsu_instance->p_ctrl;

    g_bad_sensor.method = method;


    if (ctsu_status.flag.icomp_error) // TSCAP error
    {

#if (BSP_FEATURE_CTSU_VERSION == 2)
        for (i = 0; i < p_ctsu_instance_ctrl->num_elements * CTSU_CFG_NUM_SUMULTI; i++)
        {
            if (CTSU_MODE_SELF_MULTI_SCAN == p_ctsu_instance_ctrl->p_ctsu_cfg->md)
            {
                /* One possible cause of a TSCAP error is an unconfigured pin. This gives a sensor count at or near 0. */
                if (p_ctsu_instance_ctrl->p_self_raw[i] < TOUCH_WRAPPER_ABNORMAL_TSCAP_COUNT)
                {
                    i = i / CTSU_CFG_NUM_SUMULTI;
                    break;
                }
            }
            else
            {
                /* One possible cause of a TSCAP error is an unconfigured pin. This gives a sensor count at or near 0. */
                if (p_ctsu_instance_ctrl->p_mutual_raw[i * 2] < TOUCH_WRAPPER_ABNORMAL_TSCAP_COUNT)
                {
                    i = i / CTSU_CFG_NUM_SUMULTI;
                    break;
                }

                /* One possible cause of a TSCAP error is an unconfigured pin. This gives a sensor count at or near 0. */
                if (p_ctsu_instance_ctrl->p_mutual_raw[i* 2 + 1] < TOUCH_WRAPPER_ABNORMAL_TSCAP_COUNT)
                {
                    i = i / CTSU_CFG_NUM_SUMULTI;
                    break;
                }
            }
        }
#endif
#if (BSP_FEATURE_CTSU_VERSION == 1)
        for (i = 0; i < p_ctsu_instance_ctrl->num_elements; i++)
        {
            if (CTSU_MODE_SELF_MULTI_SCAN == p_ctsu_instance_ctrl->p_ctsu_cfg->md)
            {


                /* One possible cause of a TSCAP error is an unconfigured pin. This gives a sensor count at or near 0. */
                if (p_ctsu_instance_ctrl->p_self_raw[i].sen < TOUCH_WRAPPER_ABNORMAL_TSCAP_COUNT)
                {
                    break;
                }

            }
            else
            {
                if (p_ctsu_instance_ctrl->p_mutual_raw[i].pri_sen < TOUCH_WRAPPER_ABNORMAL_TSCAP_COUNT)
                {
                    break;
                }

                if (p_ctsu_instance_ctrl->p_mutual_raw[i].snd_sen < TOUCH_WRAPPER_ABNORMAL_TSCAP_COUNT)
                {
                    break;
                }

            }
        }
#endif

        g_bad_sensor.element = i;
        err = QE_ERR_ABNORMAL_TSCAP;
    }
    else if (ctsu_status.flag.sens_over) // sensor overflow error
    {

#if (BSP_FEATURE_CTSU_VERSION == 2)
        for (i = 0; i < p_ctsu_instance_ctrl->num_elements * CTSU_CFG_NUM_SUMULTI; i++)
        {
            if (CTSU_MODE_SELF_MULTI_SCAN == p_ctsu_instance_ctrl->p_ctsu_cfg->md)
            {
                /* One possible cause of a TSCAP error is an unconfigured pin. This gives a sensor count at or near 0. */
                if (TOUCH_WRAPPER_SENSOR_OVERFLOW_COUNT == p_ctsu_instance_ctrl->p_self_raw[i])
                {
                    i = i / CTSU_CFG_NUM_SUMULTI;
                    break;
                }
            }
            else
            {
                /* One possible cause of a TSCAP error is an unconfigured pin. This gives a sensor count at or near 0. */
                if (TOUCH_WRAPPER_SENSOR_OVERFLOW_COUNT == p_ctsu_instance_ctrl->p_mutual_raw[i * 2])
                {
                    i = i / CTSU_CFG_NUM_SUMULTI;
                    break;
                }

                /* One possible cause of a TSCAP error is an unconfigured pin. This gives a sensor count at or near 0. */
                if (TOUCH_WRAPPER_SENSOR_OVERFLOW_COUNT == p_ctsu_instance_ctrl->p_mutual_raw[i * 2 + 1])
                {
                    i = i / CTSU_CFG_NUM_SUMULTI;
                    break;
                }
            }
        }
#endif
#if (BSP_FEATURE_CTSU_VERSION == 1)
        for (i = 0; i < p_ctsu_instance_ctrl->num_elements; i++)
        {
            if (CTSU_MODE_SELF_MULTI_SCAN == p_ctsu_instance_ctrl->p_ctsu_cfg->md)
            {
                /* One possible cause of a TSCAP error is an unconfigured pin. This gives a sensor count at or near 0. */
                if (TOUCH_WRAPPER_SENSOR_OVERFLOW_COUNT == p_ctsu_instance_ctrl->p_self_raw[i].sen)
                {
                    break;
                }

            }
            else
            {
                /* One possible cause of a TSCAP error is an unconfigured pin. This gives a sensor count at or near 0. */
                if (TOUCH_WRAPPER_SENSOR_OVERFLOW_COUNT == p_ctsu_instance_ctrl->p_mutual_raw[i].pri_sen)
                {
                    break;
                }

                if (TOUCH_WRAPPER_SENSOR_OVERFLOW_COUNT == p_ctsu_instance_ctrl->p_mutual_raw[i].snd_sen)
                {
                    break;
                }
            }
        }
#endif

        g_bad_sensor.element = i;
        err = QE_ERR_SENSOR_OVERFLOW;
    }
    else
    {
        ; // coding standard requirement
    }

    return err;
} /* End of function get_ctsu_err() */

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
*                    TOUCH_CMD_GET_FAILED_SENSOR:
*                       Loads correction or offset tuning error details after Open() failure.
*                    TOUCH_CMD_CLEAR_TOUCH_STATES:
*                       Sets all touch states and related counters to 0 for specified method.
*                p_arg -
*                    Pointer to command-specific argument.
*                    TOUCH_CMD_SET_METHOD               uint8_t *
*                    TOUCH_CMD_CYCLE_ALL_METHODS        (none)
*                    TOUCH_CMD_CYCLE_METHOD_LIST        touch_mlist_t *
*                    TOUCH_CMD_GET_FAILED_SENSOR        touch_sensor_t *
*                    TOUCH_CMD_GET_LAST_SCAN_METHOD     uint8_t *
*                    TOUCH_CMD_CLEAR_TOUCH_STATES       uint8_t *
*
* Return Value : QE_SUCCESS -
*                    Command performed successfully.
*                QE_ERR_NULL_PTR -
*                    Missing required argument.
*                QE_ERR_INVALID_ARG -
*                    Command, method number, or number of methods is invalid.
***********************************************************************************************************************/
qe_err_t R_TOUCH_Control(touch_cmd_t cmd, void *p_arg)
{
    uint8_t *p_set_method = (uint8_t *)p_arg;
    touch_mlist_t *p_list = (touch_mlist_t *)p_arg;
    touch_sensor_t *p_bad_sensor = (touch_sensor_t *)p_arg;
    uint8_t *p_last_method = (uint8_t *)p_arg;
    uint8_t *p_clr_states_method = (uint8_t *)p_arg;

    qe_err_t err = QE_SUCCESS;
    uint8_t i;

    touch_instance_ctrl_t *p_touch_instance_ctrl;

#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((TOUCH_CMD_CYCLE_ALL_METHODS != cmd) && (NULL == p_arg))
    {
        return QE_ERR_NULL_PTR;
    }

    if ((cmd >= TOUCH_CMD_END_ENUM) || (((TOUCH_CMD_SET_METHOD == cmd) || (TOUCH_CMD_CLEAR_TOUCH_STATES == cmd)) && ((*p_set_method) >= g_open_num_methods)))
    {
        return QE_ERR_INVALID_ARG;
    }

    if (TOUCH_CMD_CYCLE_METHOD_LIST == cmd)
    {
        if (p_list->num_methods > g_open_num_methods)
        {
            return QE_ERR_INVALID_ARG;
        }

        for (i = 0; i < p_list->num_methods; i++)
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
        case TOUCH_CMD_GET_FAILED_SENSOR:
        {
            *p_bad_sensor = g_bad_sensor; // structure value assignment
            break;
        }
        case TOUCH_CMD_CLEAR_TOUCH_STATES:
        {
            p_touch_instance_ctrl = (touch_instance_ctrl_t *)gp_touch_cfgs[*p_clr_states_method];

            /* clear buttons */
            for (i = 0; i < p_touch_instance_ctrl->p_touch_cfg->num_buttons; i++)
            {
                *(p_touch_instance_ctrl->binfo.p_on_count + i) = 0;
                *(p_touch_instance_ctrl->binfo.p_off_count + i) = 0;
            }

            /* clear sliders and wheels */
    #if (TOUCH_CFG_NUM_SLIDERS != 0)
            for (i = 0; i < p_touch_instance_ctrl->p_touch_cfg->num_sliders; i++)
            {
                *(p_touch_instance_ctrl->sinfo.p_position + i) = 0xFFFF;
            }
    #endif
    #if (TOUCH_CFG_NUM_WHEELS != 0)
            for (i = 0; i < p_touch_instance_ctrl->p_touch_cfg->num_wheels; i++)
            {
                *(p_touch_instance_ctrl->winfo.p_position + i) = 0xFFFF;
            }
    #endif
            break;
        }
        case TOUCH_CMD_SET_METHOD:
        {
            g_set_method = *p_set_method;
            g_cycle_methods = false;

            /* for monitor use */
            g_mlist.num_methods = 1;
            g_mlist.methods[0] = *p_set_method;
            break;
        }
        case TOUCH_CMD_CYCLE_ALL_METHODS:
        {
            /* set local list to all methods in Open() */
            for (i = 0; i < g_open_num_methods; i++)
            {
                g_mlist.methods[i] = i;
            }

            g_mlist.num_methods = g_open_num_methods;
            g_mlist.cur_index = g_method_stor;
            g_cycle_methods = true;
            break;
        }
        case TOUCH_CMD_CYCLE_METHOD_LIST:
        {
            /* set local list to specified list */
            memcpy(&g_mlist, p_list, sizeof(touch_mlist_t));
            g_mlist.cur_index = 0;
            g_cycle_methods = true;
            break;
        }
        case TOUCH_CMD_GET_LAST_SCAN_METHOD:
        {
            *p_last_method = g_last_method;
            break;
        }
        default:
        {
            err = QE_ERR_INVALID_ARG;
            break;
        }
    }

    return err;
} /* End of function R_TOUCH_Control() */

/***********************************************************************************************************************
* Function Name: R_TOUCH_GetRawData
* Description  : This function gets the sensor values as scanned by the CTSU (correction and filter not applied).
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
    uint16_t element_id;

    touch_instance_ctrl_t *p_touch_instance_ctrl = (touch_instance_ctrl_t *)gp_touch_cfgs[method];
    ctsu_instance_ctrl_t *p_ctsu_instance_ctrl = (ctsu_instance_ctrl_t *)p_touch_instance_ctrl->p_ctsu_instance->p_ctrl;

#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if (method >= g_open_num_methods)
    {
        return QE_ERR_INVALID_ARG;
    }

    if ((NULL == p_buf) || (NULL == p_cnt))
    {
        return QE_ERR_NULL_PTR;
    }
#endif

    if (CTSU_MODE_SELF_MULTI_SCAN == p_ctsu_instance_ctrl->p_ctsu_cfg->md)
    {
        *p_cnt = (uint8_t)(p_ctsu_instance_ctrl->num_elements * CTSU_CFG_NUM_SUMULTI);

        for (element_id = 0; element_id < (p_ctsu_instance_ctrl->num_elements * CTSU_CFG_NUM_SUMULTI); element_id++)
        {
#if (BSP_FEATURE_CTSU_VERSION == 2)
            *p_buf = *(p_ctsu_instance_ctrl->p_self_raw + element_id);
#endif
#if (BSP_FEATURE_CTSU_VERSION == 1)
            *p_buf = (p_ctsu_instance_ctrl->p_self_raw + element_id)->sen;
#endif
            p_buf++;
        }
    }
    else
    {
        *p_cnt = (uint8_t)(p_ctsu_instance_ctrl->num_elements * CTSU_CFG_NUM_SUMULTI * 2);

        for (element_id = 0; element_id < (p_ctsu_instance_ctrl->num_elements * CTSU_CFG_NUM_SUMULTI); element_id++)
        {
#if (BSP_FEATURE_CTSU_VERSION == 2)
            *p_buf = *(p_ctsu_instance_ctrl->p_mutual_raw + element_id * 2);
             p_buf++;
             
            *p_buf = *(p_ctsu_instance_ctrl->p_mutual_raw + element_id * 2 + 1);
#endif
#if (BSP_FEATURE_CTSU_VERSION == 1)
            *p_buf = (p_ctsu_instance_ctrl->p_mutual_raw + element_id)->pri_sen;
            p_buf++;
            *p_buf = (p_ctsu_instance_ctrl->p_mutual_raw + element_id)->snd_sen;
#endif
            p_buf++;
        }
    }

    return QE_SUCCESS;
} /* End of function R_TOUCH_GetRawData() */

/***********************************************************************************************************************
* Function Name: R_TOUCH_GetData
* Description  : This function gets the sensor values after correction and 4-deep moving average (filter) is applied.
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
    uint16_t element_id;

    touch_instance_ctrl_t *p_touch_instance_ctrl = (touch_instance_ctrl_t *)gp_touch_cfgs[method];
    ctsu_instance_ctrl_t *p_ctsu_instance_ctrl = (ctsu_instance_ctrl_t *)p_touch_instance_ctrl->p_ctsu_instance->p_ctrl;

#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if (method >= g_open_num_methods)
    {
        return QE_ERR_INVALID_ARG;
    }

    if ((NULL == p_buf) || (NULL == p_cnt))
    {
        return QE_ERR_NULL_PTR;
    }
#endif

    *p_cnt = (uint8_t)(p_ctsu_instance_ctrl->num_elements);

    if (CTSU_MODE_SELF_MULTI_SCAN == p_ctsu_instance_ctrl->p_ctsu_cfg->md)
    {
        for (element_id = 0; element_id < p_ctsu_instance_ctrl->num_elements; element_id++)
        {
            *p_buf = (p_ctsu_instance_ctrl->p_self_data + element_id)->int_data;
            p_buf++;
        }
    }
    else
    {
        for (element_id = 0; element_id < p_ctsu_instance_ctrl->num_elements; element_id++)
        {
            *p_buf = (p_ctsu_instance_ctrl->p_mutual_snd_data + element_id)->int_data - (p_ctsu_instance_ctrl->p_mutual_pri_data + element_id)->int_data;
            p_buf++;
        }
    }

    return QE_SUCCESS;
} /* End of function R_TOUCH_GetData() */

/***********************************************************************************************************************
* Function Name: R_TOUCH_GetBtnBaselines
* Description  : This function loads a button threshold and number of buttons.
* Arguments    : method -
*                    Method to get data for.
*                p_buf -
*                    Pointer to variable to load button-threshold.
*                p_cnt -
*                    Pointer to variable to load number of buttons.
* Return Value : QE_SUCCESS -
*                    Command performed successfully.
*                QE_ERR_INVALID_ARG -
*                    "wheel_id" is invalid.
*                QE_ERR_NULL_PTR -
*                    "p_buf" or "p_cnt" is NULL.
***********************************************************************************************************************/
qe_err_t R_TOUCH_GetBtnBaselines(uint8_t method, uint16_t *p_buf, uint8_t *p_cnt)
{
    touch_instance_ctrl_t *p_instance_ctrl = (touch_instance_ctrl_t *)gp_touch_cfgs[method];
    qe_err_t err = QE_SUCCESS;
    uint8_t i;

#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((p_buf == NULL) || (p_cnt == NULL))
    {
        return QE_ERR_NULL_PTR;
    }
    if (method > QE_NUM_METHODS)
    {
        return QE_ERR_INVALID_ARG;
    }
#endif

    for (i = 0; i < p_instance_ctrl->p_touch_cfg->num_buttons; i++)
    {
        *p_buf++ = *(p_instance_ctrl->binfo.p_reference + i);
    }

    *p_cnt = p_instance_ctrl->p_touch_cfg->num_buttons;

    return err;
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
    touch_instance_ctrl_t *p_instance_ctrl = (touch_instance_ctrl_t *)gp_touch_cfgs[method];
    qe_err_t err = QE_SUCCESS;

#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if (p_mask == NULL)
    {
        return QE_ERR_NULL_PTR;
    }
    if (method > QE_NUM_METHODS)
    {
        return QE_ERR_INVALID_ARG;
    }
#endif
    *p_mask = p_instance_ctrl->binfo.status;

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
    touch_instance_ctrl_t *p_instance_ctrl = (touch_instance_ctrl_t *)gp_touch_cfgs[slider_id >> 8];
    qe_err_t err = QE_SUCCESS;
    uint8_t index;

    index = slider_id & 0xFF;

#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if (p_position == NULL)
    {
        return QE_ERR_NULL_PTR;
    }
    if (index > p_instance_ctrl->p_touch_cfg->num_sliders)
    {
        return QE_ERR_INVALID_ARG;
    }
#endif

    if (p_instance_ctrl->sinfo.p_position == NULL)
    {
        return QE_ERR_NULL_PTR;
    }
    else
    {
        *p_position = *(p_instance_ctrl->sinfo.p_position + index);
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
    touch_instance_ctrl_t *p_instance_ctrl = (touch_instance_ctrl_t *)gp_touch_cfgs[wheel_id >> 8];
    qe_err_t err = QE_SUCCESS;
    uint8_t index;

    index = wheel_id & 0xFF;

#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if (p_position == NULL)
    {
        return QE_ERR_NULL_PTR;
    }
    if (index > p_instance_ctrl->p_touch_cfg->num_wheels)
    {
        return QE_ERR_INVALID_ARG;
    }
#endif

    if (p_instance_ctrl->winfo.p_position == NULL)
    {
        return QE_ERR_NULL_PTR;
    }
    else
    {
        *p_position = *(p_instance_ctrl->winfo.p_position + index);
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
*                    Cannot run function because CTSU is performing an operation.
***********************************************************************************************************************/
qe_err_t R_TOUCH_Close(void)
{
    qe_err_t err = QE_SUCCESS;
    fsp_err_t fsp_err;
    uint8_t i;

    for (i = 0; i < g_mlist.num_methods; i++)
    {
        fsp_err = RM_TOUCH_Close((touch_ctrl_t *)gp_touch_cfgs[i]);
        if (FSP_SUCCESS != fsp_err)
        {
            err = QE_ERR_BUSY;
            return err;
        }
    }

    /* Clear method variables */
    g_mlist.num_methods = 0;
    g_mlist.cur_index = 0;
    g_open_num_methods = 0;
    g_method_stor = 0;
    g_cycle_methods = 0;

    /* Clear interrupt flag */
    g_qe_touch_flag = 0;

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
R_BSP_PRAGMA_INLINE(R_TOUCH_GetVersion)
uint32_t R_TOUCH_GetVersion(void)
{
    return ((((uint32_t)QETOUCH_RX_VERSION_MAJOR) << 16) | (uint32_t)QETOUCH_RX_VERSION_MINOR);
}

#endif
