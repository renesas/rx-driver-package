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
*              :            0.99    Jim Page: Changed GAIN_100 to 1 (sets TUNING_UPPER/LOWER_LIMIT to 200/150)
*              : 04.10.2018 1.00    First Release
*              : 09.07.2019 1.10    Moved TUNING_UPPER/LOWER_LIMIT #defines to private.h.
*                                   Created enhanced algorithm for performing offset tuning and calculating reference
*                                     counts and moved this processing into Open().
*                                   Added TOUCH_CMD_GET_FAILED_SENSOR and TOUCH_CMD_GET_LAST_SCAN_METHOD
*                                     Control() commands.
*                                   Added r_touch_control_private().
*                                   Closed ext trigger timing window bug by adding disable/enable CTSU interrupts.
*                                   Removed g_correction_create error checking in touch_data_moving_average() because
*                                     Open() will return error prior to that check ever being reached.
*                                   Added error checking to correction_sensor_magnification_set() switch default.
*                                   Modified R_TOUCH_UpdateData() so it can be called when using software triggers.
*                                     (User will need to call R_CTSU_StartScan() in this case, like low power).
*                                   Added #pragma sections and removed g_calib_info[][].
*                                   Modified for GCC/IAR compatibility.
*              : 09.01.2020 1.11    Added Control() command TOUCH_CMD_CLEAR_TOUCH_STATES for low power applications.
*                                   Added control_private() command TOUCH_PCMD_CLEAR_TUNING_FLAGS.
*                                   Added API function R_TOUCH_GetBtnBaselines.
*                                   Added error code QE_ERR_UNSUPPORTED_CLK_CFG to R_TOUCH_Open().
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
#include "r_ctsu_qe_private.h"
#include "r_monitor.h"

#if (TUNING_UPPER_LIMIT > 500)
#error "TUNING_UPPER_LIMIT in r_touch_qe_private.h is too large."
#endif

#if (TUNING_LOWER_LIMIT > 500)
#error "TUNING_LOWER_LIMIT in r_touch_qe_private.h is too large."
#endif

#if (OT_MAX_SCAN_ATTEMPTS > 255)
#error "OT_MAX_SCAN_ATTEMPTS in touch_qe_private.h is too large"
#endif


/***********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/
#define TOUCH_PRV_0_SUCCESS         (0)
#define TOUCH_PRV_1_ERROR           (1)

#define TOUCH_PRV_SO0_OFFSET_MASK   (0x3FF)

#define TOUCH_PRV_ADD_TIME          (4)

#define TOUCH_PRV_0_BUTTON          (0)
#define TOUCH_PRV_1_MATRIX          (1)
#define TOUCH_PRV_2_SLIDER          (2)
#define TOUCH_PRV_3_WHEEL           (3)

#define TOUCH_PRV_LEVEL_USE_MAX_ADJ (1000)      /* level beyond which should tune with larger adjustments (arbitrary value) */

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
touch_tuning_t      g_touch_tuning_info[QE_NUM_METHODS];

touch_func_flag_t   g_touch_function[QE_NUM_METHODS];
touch_result_t      g_touch_all_result[QE_NUM_METHODS];
touch_func_param_t  g_touch_paramter[QE_NUM_METHODS];
volatile uint8_t    g_data_tim;                         /* Calibration data storage times */

uint16_t        g_slider_data[QE_SLIDER_MAX_ELEMENTS];
uint16_t        g_wheel_data[QE_WHEEL_MAX_ELEMENTS];
uint8_t         g_method_stor=0;
uint8_t         g_open_num_methods;
touch_mlist_t   g_mlist;

ctsu_cfg_t      **gp_ctsu_configs;      // ptr to array of ctsu configurations
touch_cfg_t     **gp_touch_configs;     // ptr to array of touch configurations
sldr_ctrl_t     *gp_sliderInfo;
wheel_ctrl_t    *gp_wheelInfo;


/***********************************************************************************************************************
* Private global variables and functions
***********************************************************************************************************************/
static touch_sensor_t   g_bad_sensor;   // offset tuning error details

static qe_trig_t    g_trigger;
static bool         g_cycle_methods;
static uint8_t      g_set_method;
static uint8_t      g_last_method;

static uint16_t R_Set_Cap_Touch_Result_Create(uint8_t method);
static uint16_t R_Get_Cap_Touch_Sensor_Data(uint8_t method, uint8_t mode, uint8_t index_num);
static void     touch_data_moving_average(uint8_t method);
static qe_err_t correction_sensor_magnification_set(uint8_t method);
static qe_err_t method_offset_tuning(uint8_t method);
static qe_err_t get_ctsu_err(uint8_t method, ctsu_status_t ctsu_status);
static uint16_t correction_sensor_cnt_create(uint8_t method, uint8_t ts_num, uint8_t number);
static uint16_t correction_pri_sensor_cnt_create(uint8_t method, uint8_t ts_num, uint8_t number);
static uint16_t correction_snd_sensor_cnt_create(uint8_t method, uint8_t ts_num, uint8_t number);
static qe_err_t update_data(void);
static qe_err_t r_touch_control_private(touch_pcmd_t pcmd, void *p_arg);


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif/***********************************************************************************************************************
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
*                QE_ERR_BUSY -
*                    CTSU already open and busy running.
*                QE_ERR_UNSUPPORTED_CLK_CFG-
*                    Unsupported clock configuration.
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
*                QE_ERR_OT_WINDOW_SIZE -
*                    Tuning window too small. SO0 adjustments keep counts outside of window.
*                QE_ERR_OT_MAX_ATTEMPTS -
*                    Maximum scans performed and all sensors still not in target window.
*                QE_ERR_UNSUPPORTED_CLK_CFG -
*                   Unsupported clock speed. Cannot perform CTSU correction.
***********************************************************************************************************************/
qe_err_t R_TOUCH_Open(ctsu_cfg_t *p_ctsu_cfgs[], touch_cfg_t *p_touch_cfgs[], uint8_t num_methods, qe_trig_t trigger)
{
    qe_err_t    err=QE_SUCCESS;
    uint8_t     i;
    uint8_t     method;
#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((NULL == p_ctsu_cfgs) || (NULL == p_touch_cfgs))
    {
        return QE_ERR_NULL_PTR;
    }

    if ((num_methods > QE_NUM_METHODS) || (trigger >= QE_TRIG_END_ENUM))
    {
        return QE_ERR_INVALID_ARG;
    }

    /* Do not parameter check configuration contents generated by QE Tool. */
#endif

    /* Copy initialized safety critical data from "D" ROM section into "R" RAM section */
#if ((defined(__CCRX__)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
    memcpy(__sectop("R_QE_TOUCH_SAFETY_RAMINIT"), __sectop("D_QE_TOUCH_SAFETY_RAMINIT"),
                ((uint32_t)__secend("D_QE_TOUCH_SAFETY_RAMINIT") - (uint32_t)__sectop("D_QE_TOUCH_SAFETY_RAMINIT")));
    memcpy(__sectop("R_QE_TOUCH_SAFETY_RAMINIT_2"), __sectop("D_QE_TOUCH_SAFETY_RAMINIT_2"),
               ((uint32_t)__secend("D_QE_TOUCH_SAFETY_RAMINIT_2") - (uint32_t)__sectop("D_QE_TOUCH_SAFETY_RAMINIT_2")));
#endif
    g_open_num_methods = num_methods;
    g_trigger = trigger;


    /* initialize system pointers */

    touch_parameter_address_set();
    ctsu_sensor_data_address_set();
#if (TOUCH_CFG_UPDATE_MONITOR == 1)
    monitor_init();
#endif

    /* open CTSU driver */
    err = R_CTSU_Open(p_ctsu_cfgs, num_methods, trigger);
    if (QE_SUCCESS == err)
    {
        touch_parameter_set(p_ctsu_cfgs, p_touch_cfgs);

        /* by default, set Touch to cycle through all methods, starting with method 0 */
        g_mlist.num_methods = num_methods;
        g_cycle_methods = true;
        for (i=0; i < num_methods; i++)
        {
            g_mlist.methods[i] = i;
        }

        if (1 == num_methods)
        {
            g_cycle_methods = false;
        }

        /* set up global pointers for method 0 */
        method = 0;
        g_method_stor = method;
        g_set_method = method;
        gp_ctsu_configs = p_ctsu_cfgs;
        gp_touch_configs = p_touch_cfgs;
        gp_sliderInfo = p_touch_cfgs[method]->p_sliders;
        gp_wheelInfo = p_touch_cfgs[method]->p_wheels;

        /* perform offset tuning and set button reference values */
        err = r_touch_control_private(TOUCH_PCMD_PERFORM_OFFSET_TUNING, NULL);

        if ((QE_SUCCESS == err) || (QE_ERR_OT_MAX_ATTEMPTS == err))
        {
            R_CTSU_Control(CTSU_CMD_SET_METHOD, 0, NULL);   // stay opened; set to method 0
        }
        else
        {
            R_CTSU_Close();
        }
    }
    else
    {
        g_bad_sensor.method = 0;            // correction uses method 0's scan buffer
        g_bad_sensor.element = 0;           // correction operates on first element only
    }

    return err;
} /* End of function R_TOUCH_Open() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: method_offset_tuning
* Description  : This function fine tunes the CTSU SO0 register for each sensor. It will perform up to
*                OT_MAX_SCAN_ATTEMPTS to get all sensors within its no-touch target range.
* Arguments    : method -
*                    Method to fine tune (gp_ctsu_configs[] and gp_touch_cfgs[] index)
* Return Value : QE_SUCCESS -
*                    All sensors in the method were fine tuned successfully.
*                QE_ERR_BUSY -
*                    CTSU busy. Likely this function called via main loop (outside Open) while another scan is running.
*                QE_ERR_UNSUPPORTED_CLK_CFG-
*                    Unsupported clock configuration.
*                QE_ERR_ABNORMAL_TSCAP-
*                    TSCAP error detected during scan
*                QE_ERR_SENSOR_OVERFLOW-
*                    Sensor overflow detected during scan
*                QE_ERR_OT_MAX_OFFSET -
*                    Cannot tune SO0 offset any higher
*                QE_ERR_OT_MIN_OFFSET -
*                    Cannot tune SO0 offset any lower
*                QE_ERR_OT_WINDOW_SIZE -
*                    Tuning window too small. SO0 adjustments keep counts outside of window.
*                QE_ERR_OT_MAX_ATTEMPTS -
*                    Maximum scans performed and all sensors still not in target window.
***********************************************************************************************************************/
static qe_err_t method_offset_tuning(uint8_t method)
{
    qe_err_t err = QE_SUCCESS;
    uint8_t  i;
    uint8_t  j;
    uint8_t  num_elements;
    uint8_t  num_tuned;
    uint16_t ref_cnt;
    uint16_t sensor_cnt;
    uint16_t so0_reg_val;
    uint16_t so0_offset;
    uint16_t so0_min_adj;           // so0 increment/decrement amount if sensor within 1000 counts of target
    uint16_t so0_max_adj;           // so0 increment/decrement amount if sensor over 1000 counts of target
    ctsu_state_t    ctsu_state;
    ctsu_status_t   ctsu_status;


    /* init variables */

    g_bad_sensor.method = method;
    num_elements = gp_ctsu_configs[method]->num_elements;
    if (TOUCH_SELF_MODE == g_key_info[method].mode)
    {
        ref_cnt = QE_07_2UA;
        so0_min_adj = 1;
        so0_max_adj = 2;
    }
    else /* mutual mode */
    {
        ref_cnt = QE_04_8UA;
        so0_min_adj = 2;
        so0_max_adj = 4;
    }


    /* SCAN UNTIL ALL SENSORS TUNED OR MAX ATTEMPTS REACHED */

    /* allow software scans if external trigger configured; do not call user callback function */
    r_ctsu_control_private(CTSU_PCMD_SET_OFFSET_TUNING_FLG, method, NULL);

    for (j=0; j < OT_MAX_SCAN_ATTEMPTS; j++)
    {
        /* START SCAN AND WAIT FOR COMPLETION */

        err = R_CTSU_StartScan();
        if (QE_ERR_BUSY == err)     // function called outside of Open() likely while another scan is running
        {
            break;
        }

        do
        {
            R_CTSU_Control(CTSU_CMD_GET_STATE, method, &ctsu_state);
        } while (CTSU_STATE_READY != ctsu_state);


        /* CHECK FOR HARDWARE SCAN ERRORS */
        R_CTSU_Control(CTSU_CMD_GET_STATUS, method, &ctsu_status);
        if (1 != ctsu_status.flag.data_update)
        {
            err = get_ctsu_err(method, ctsu_status);
            break;
        }

        /* Check for bad clock configuration */
        err = correction_sensor_magnification_set(method);
        if (QE_SUCCESS != err)
        {
            break;
        }


        /* PROCESS ALL UNTUNED ELEMENTS */

        /* get filtered (averaged) count values with correction applied for all elements */
        touch_data_moving_average(method);

        /* for each element */
        for (i=0, num_tuned=0; i < num_elements; i++ )
        {
            /* if current element not yet fine tuned */
            if (OT_WINDOW_TUNE_COUNT != g_touch_tuning_info[method].result[i])
            {
                /* get sensor count */
                if (TOUCH_SELF_MODE == g_key_info[method].mode)
                {
                    sensor_cnt = g_self_add_sensor_pt[method][i];
                }
                else /* mutual mode */
                {
                    sensor_cnt = g_mutual_add_pri_sensor_pt[method][i];
                }

                /* get SO0 offset value for sensor from CTSU */
                R_CTSU_ReadReg(CTSU_REG_SO0, i, &so0_reg_val);
                so0_offset = so0_reg_val & TOUCH_PRV_SO0_OFFSET_MASK;
                so0_reg_val &= (~TOUCH_PRV_SO0_OFFSET_MASK);

                /* if sensor count too high */
                if (sensor_cnt > (ref_cnt + TUNING_UPPER_LIMIT))
                {
                    /* if SO0 offset not maxed out */
                    if (0x3FF != so0_offset)
                    {
                        /* increment SO0 and write to CTSU */
                        if ((sensor_cnt > (ref_cnt + TOUCH_PRV_LEVEL_USE_MAX_ADJ)) && ((so0_offset + so0_max_adj) <= 0x3FF))
                        {
                            so0_offset += so0_max_adj;
                        }
                        else if ((so0_offset + so0_min_adj) <= 0x3FF)
                        {
                            so0_offset += so0_min_adj;
                        }
                        else
                        {
                            so0_offset++;
                        }

                        so0_reg_val |= so0_offset;
                        R_CTSU_WriteReg(CTSU_REG_SO0, i, so0_reg_val);

                        /* if change in direction, increment change count */
                        if (CTSUSO_INCREMENTING != g_touch_tuning_info[method].ctsuso[i])
                        {
                            g_current_sign_pt[method][i] += 1;
                        }
                        g_touch_tuning_info[method].ctsuso[i] = CTSUSO_INCREMENTING;    // set direction
                    }
                    else
                    {
                        err = QE_ERR_OT_MAX_OFFSET;
                        break;
                    }
                }
                /* else if sensor count too low */
                else if (sensor_cnt < (ref_cnt - TUNING_LOWER_LIMIT))
                {
                    /* if SO0 offset not already at min */
                    if (0 != so0_offset)
                    {
                        /* decrement SO0 and write to CTSU */
                        if ((sensor_cnt < (ref_cnt - TOUCH_PRV_LEVEL_USE_MAX_ADJ)) && ((so0_offset - so0_max_adj) >= 0))
                        {
                            so0_offset -= so0_max_adj;
                        }
                        else if ((so0_offset - so0_min_adj) >= 0)
                        {
                            so0_offset -= so0_min_adj;
                        }
                        else
                        {
                            so0_offset--;
                        }

                        so0_reg_val |= so0_offset;
                        R_CTSU_WriteReg(CTSU_REG_SO0, i, so0_reg_val);

                        /* if change in direction, increment change count */
                        if (CTSUSO_DECREMENTING != g_touch_tuning_info[method].ctsuso[i])
                        {
                            g_current_sign_pt[method][i] += 1;
                        }
                        g_touch_tuning_info[method].ctsuso[i] = CTSUSO_DECREMENTING;    // set direction
                    }
                    else
                    {
                        err = QE_ERR_OT_MIN_OFFSET;
                        break;
                    }
                }
                else
                {
                    /* else sensor between upper and lower limits*/
                    g_touch_tuning_info[method].result[i]++;
                    if (OT_WINDOW_TUNE_COUNT == g_touch_tuning_info[method].result[i])
                    {
                        num_tuned++;
                    }
                }

                /* if changed direction more than twice, tuning window too small */
                if (3 == g_current_sign_pt[method][i])
                {
                    err = QE_ERR_OT_WINDOW_SIZE;
                    break;
                }
            }
            else
            {
                num_tuned++;
            }
        } // end element tuning loop


        /* if an error was detected, stop scanning */
        if (QE_SUCCESS != err)
        {
            g_bad_sensor.element = i;
            break;
        }

        /* if all sensors tuned, stop scanning */
        if (num_tuned == num_elements)
        {
            g_touch_function[method].flag.tuning = 1;               // tuning complete for method
            break;
        }
    } // end scan attempt loop


    r_ctsu_control_private(CTSU_PCMD_CLR_OFFSET_TUNING_FLG, method, NULL);   // restore original triggering as appropriate

    /* if all sensors tuned, set reference values for BUTTONS to moving average */
    if (1 == g_touch_function[method].flag.tuning)
    {
        for (i=0; i < gp_touch_cfgs[method]->num_buttons; i++)
        {
            j = gp_touch_cfgs[method]->p_buttons[i].elem_index;
            if (TOUCH_SELF_MODE == g_key_info[method].mode)
            {
                g_key_info[method].ref[i] = g_self_add_sensor_pt[method][j];
            }
            else
            {
                g_key_info[method].ref[i] = g_mutual_sensor_diff_pt[method][j];
            }
        }
    }
    else if (j >= OT_MAX_SCAN_ATTEMPTS)
    {
        err = QE_ERR_OT_MAX_ATTEMPTS;

        /* find 1st of 1+ untuned elements */
        for (i=0; i < num_elements; i++)
        {
            if (0 == g_touch_tuning_info[method].result[i])
            {
                break;
            }
        }

        g_bad_sensor.element = i;
    }
    else
    {
        ;   // coding standard requirement
    }

    return err;
} /* End of function method_offset_tuning() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
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
    qe_err_t    err = QE_SUCCESS;
    uint8_t     i;
    uint8_t     index_multiplier;
    uint8_t     num_elements = gp_ctsu_configs[method]->num_elements;


    g_bad_sensor.method = method;
    index_multiplier = (TOUCH_SELF_MODE == g_key_info[method].mode) ? 2 : 4;

    if (ctsu_status.flag.icomp_error)       // TSCAP error
    {
        for (i=0; i < num_elements; i++)
        {
            /* One possible cause of a TSCAP error is an unconfigured pin. This gives a sensor count at or near 0. */
            if (gp_ctsu_configs[method]->p_scan_buf[i*index_multiplier] < 10)
            {
                break;
            }
        }

        g_bad_sensor.element = i;
        err = QE_ERR_ABNORMAL_TSCAP;
    }

    else if (ctsu_status.flag.sens_over)    // sensor overflow error
    {
        for (i=0; i < num_elements; i++)
        {
            if (0xFFFF == gp_ctsu_configs[method]->p_scan_buf[i*index_multiplier])
            {
                break;
            }
        }

        g_bad_sensor.element = i;
        err = QE_ERR_SENSOR_OVERFLOW;
    }
    else
    {
        ;   // coding standard requirement
    }

    return err;
} /* End of function get_ctsu_err() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_TOUCH_UpdateDataAndStartScan
* Description  : This function is used when software-trigger is specified at Open(). It should be called each time
*                a periodic timer expires. This function updates the sensor data, determines if any touches occurred,
*                sets the method to be run on the next scan if appropriate, and starts another scan.
* Arguments    : None
* Return Value : QE_SUCCESS -
*                    API initialized successfully.
*                QE_ERR_TRIGGER_TYPE -
*                    This function should not be called when using external triggers
*                QE_ERR_BUSY -
*                    CTSU busy. Likely need more time between calling this function/starting scans.
*                QE_ERR_ABNORMAL_TSCAP-
*                    TSCAP error detected during scan
*                QE_ERR_SENSOR_OVERFLOW-
*                    Sensor overflow detected during scan
***********************************************************************************************************************/
qe_err_t R_TOUCH_UpdateDataAndStartScan(void)
{
    qe_err_t    err=QE_SUCCESS;


    if (QE_TRIG_EXTERNAL == g_trigger)
    {
        return QE_ERR_TRIGGER_TYPE;
    }

    err = update_data();
    if (QE_ERR_BUSY != err)
    {
        R_CTSU_StartScan();
    }

    return err;

} /* End of function R_TOUCH_UpdateDataAndStartScan() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_TOUCH_UpdateData
* Description  : This function is used when external-trigger is specified at Open(), when only a single method is
*                running due to a TOUCH_CMD_SET_METHOD, or non-periodic scanning is implemented. It should be called
*                each time a scan completes. This function updates the sensor data, determines if any touches occurred,
*                and sets the method to be run on the next scan if appropriate.
* Arguments    : None
* Return Value : QE_SUCCESS -
*                    API initialized successfully.
*                QE_ERR_BUSY -
*                    CTSU busy. Likely need more time between calling this function/starting scans.
*                QE_ERR_ABNORMAL_TSCAP-
*                    TSCAP error detected during scan
*                QE_ERR_SENSOR_OVERFLOW-
*                    Sensor overflow detected during scan
***********************************************************************************************************************/
qe_err_t R_TOUCH_UpdateData(void)
{
    qe_err_t    err=QE_SUCCESS;

    err = update_data();

    return err;

} /* End of function R_TOUCH_UpdateData() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
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
    qe_err_t        err=QE_SUCCESS;
    ctsu_status_t   ctsu_status;
    ctsu_state_t    ctsu_state;
    bool            chg_method=false;


    /* Make sure scan is not currently running */

    r_ctsu_control_private(CTSU_PCMD_DISABLE_CTSU_INTS, g_method_stor, NULL);
    R_CTSU_Control(CTSU_CMD_GET_STATE, g_method_stor, &ctsu_state);
    if (CTSU_STATE_READY == ctsu_state)
    {
        g_last_method = g_method_stor;

        /* Verify scan completed successfully (no hardware errors) */
        R_CTSU_Control(CTSU_CMD_GET_STATUS, g_method_stor, &ctsu_status);
        if (1 == ctsu_status.flag.data_update)
        {
            /* update moving average for sensors */
            touch_data_moving_average(g_method_stor);

            /* save raw scan data */
            if (TOUCH_SELF_MODE == g_key_info[g_method_stor].mode)
            {
                memcpy(g_self_raw_data_pt[g_method_stor], g_self_ico_data_pt[g_method_stor], sizeof(self_ico_t) * g_key_info[g_method_stor].ena_num );
            }
            else
            {
                memcpy(g_mutual_raw_data_pt[g_method_stor], g_mutual_ico_data_pt[g_method_stor], sizeof(mutual_ico_t) * g_key_info[g_method_stor].ena_num );
            }

            r_ctsu_control_private(CTSU_PCMD_ENABLE_CTSU_INTS, g_method_stor, NULL);

            /* if this method has completed offset tuning, update touch judgments for all sensors */
            if (1 == g_touch_function[g_method_stor].flag.tuning)
            {
                R_Set_Cap_Touch_Result_Create(g_method_stor);
            }
#if (TOUCH_CFG_UPDATE_MONITOR == 1)
            monitor_update_data();
#endif
        }
        else
        {
            err = get_ctsu_err(g_method_stor, ctsu_status);
        }


        /* set next method as needed */

        if (true == g_cycle_methods)
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
            ;   /* leave current method running */
        }

        if (true == chg_method)
        {
            R_CTSU_Control(CTSU_CMD_SET_METHOD, g_method_stor, NULL);
            gp_sliderInfo = gp_touch_configs[g_method_stor]->p_sliders;
            gp_wheelInfo = gp_touch_configs[g_method_stor]->p_wheels;
        }


        correction_sensor_magnification_set(g_method_stor);
    }
    else
    {
        r_ctsu_control_private(CTSU_PCMD_ENABLE_CTSU_INTS, g_method_stor, NULL);
        err = QE_ERR_BUSY;
    }

    return err;

} /* End of function update_data() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: correction_sensor_magnification_set
* Description  :
* Arguments    : uint8_t method    : Measurement method(0-8)
* Return Value : none
***********************************************************************************************************************/
static qe_err_t correction_sensor_magnification_set(uint8_t method)
{
    uint8_t    loop;
    uint8_t    pt;
    uint16_t   snum;
    uint16_t   spda;
    uint16_t   mea_freq;
    uint16_t   ts_measure_time;
    uint16_t   reg_value;
    qe_err_t   err = QE_SUCCESS;


    /* loop through each element for the method */
    for (loop = 0, pt = 1; loop < g_key_info[method].ena_num; loop++)
    {
        R_CTSU_ReadReg(CTSU_REG_SO0, loop, &reg_value);
        snum = ((reg_value & 0xFC00) >> 10);
        R_CTSU_ReadReg(CTSU_REG_SO1, loop, &reg_value);
        spda = ((reg_value & 0x1F00) >> 8);

        mea_freq = (CTSU_INPUT_FREQUENCY_DIV * 10) / ((spda + 1) * 2);

        switch (mea_freq)
        {
            case 40:                                        /* snum=7   4.0MHz    32MHz / 8     24MHz / 6  */
            {
                ts_measure_time =  625 * (snum + 1);
                break;
            }
            case 33:                                        /* snum=7   3.3MHz    27MHz / 8  */
            {
                ts_measure_time =  750 * (snum + 1);
                break;
            }
            case 20:                                        /* snum=3   2.0MHz    32MHz / 16    24MHz / 12  */
            {
                ts_measure_time = 1250 * (snum + 1);
                break;
            }
            case 16:                                        /* snum=3   1.6MHz    27MHz / 16 */
            {
                ts_measure_time = 1500 * (snum + 1);
                break;
            }
            case 10:                                        /* snum=1   1.0MHz    32MHz / 32    24MHz / 24  */
            {
                ts_measure_time = 2500 * (snum + 1);
                break;
            }
            case 8:                                         /* snum=1   0.8MHz    27MHz / 32 */
            {
                ts_measure_time = 3000 * (snum + 1);
                break;
            }
            case 5:                                         /* snum=0   0.5MHz    32MHz / 64    24MHz / 48  */
            {
                ts_measure_time = 5000 * (snum + 1);
                break;
            }
            case 4:                                         /* snum=0   0.4MHz    27MHz / 64 */
            {
                ts_measure_time = 6000 * (snum + 1);
                break;
            }
            default:
            {
                err = QE_ERR_UNSUPPORTED_CLK_CFG;
                break;
            }
        }

        if (QE_SUCCESS == err)
        {
            if (5000 <= ts_measure_time)
            {
                /* cast to byte for less memory usage */
                *(g_key_info[method].counter_magni + loop) = (int8_t)(ts_measure_time / 500);
            }
            else
            {
                /* cast to byte for less memory usage */
                *(g_key_info[method].counter_magni + loop) = (int8_t)((50000 / ts_measure_time) * (-1));
            }
        }
        pt = pt + 3;
    }

    return err;
}    /* End of function correction_sensor_magnification_set() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_Get_Cap_Touch_Sensor_Data
* Description  : Sensor data get
* Arguments    : uint8_t  method     : Measurement method number(0-8)
*              : uint8_t  mode       : Function mode
*              : uint8_t  index_num  : Sensor number
* Return Value : uint16_t data       : Sensor data
***********************************************************************************************************************/
static uint16_t R_Get_Cap_Touch_Sensor_Data( uint8_t method, uint8_t mode, uint8_t index_num )
{
    uint16_t data;

    switch (mode)
    {
        case TOUCH_PRV_0_BUTTON:
        {
            data = *(g_self_sensor_cnt_pt[method] + (*(g_key_info[method].sensor_index + index_num)));
            break;
        }
        case TOUCH_PRV_2_SLIDER:
        {
            data = *(g_self_sensor_cnt_pt[method] + index_num);
            break;
        }
        case TOUCH_PRV_3_WHEEL:
        {
            data = *(g_self_sensor_cnt_pt[method] + index_num);
            break;
        }
        case TOUCH_PRV_1_MATRIX:
        {
            data = *(g_mutual_sensor_diff_pt[method] + index_num);
            break;
        }
        default:
        {
            data = 0xFFFF;
            break;
        }
    }
    return data;
} /* End of function R_Get_Cap_Touch_Sensor_Data() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_Set_Cap_Touch_Result_Create
* Description  : 
* Arguments    : uint8_t method    : Measurement method number(0-8)
* Return Value : uint8_t status    : TOUCH_PRV_0_SUCCESS
*              :                   : TOUCH_PRV_1_ERROR
***********************************************************************************************************************/
static uint16_t R_Set_Cap_Touch_Result_Create( uint8_t method )
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


    status = TOUCH_PRV_1_ERROR;

    if (TOUCH_SELF_MODE == g_key_info[method].mode)
    {
        if (0 != g_key_info[method].key_num)
        {
            for (loop = 0; loop < MCU_MAX_SENSORS; loop++)
            {
                if (KEY_ENABLE == touch_key_function_check( method, loop))
                {
                    sensor_val = R_Get_Cap_Touch_Sensor_Data(method, TOUCH_PRV_0_BUTTON, loop);
                    touch_key_decode(method, sensor_val, loop);
                }
            }

            for (loop = 0; loop < g_key_info[method].key_max_group; loop++)
            {
                g_touch_all_result[method].button[loop]  = *(g_key_info[method].touch_result  + loop);
                status = TOUCH_PRV_0_SUCCESS;
            }
        }

#if (QE_MAX_SLIDERS != 0)
        if (0 != gp_touch_configs[method]->num_sliders)
        {
            for (slider_id = 0; slider_id < gp_touch_configs[method]->num_sliders; slider_id++)
            {
                for (loop = 0; loop < gp_sliderInfo[slider_id].num_elements; loop++)
                {
                    sensor_val = R_Get_Cap_Touch_Sensor_Data(method, TOUCH_PRV_2_SLIDER, gp_sliderInfo[slider_id].elem_index[loop]);
                    g_slider_data[loop]  = sensor_val;
                }
                pos_value = slider_decode( slider_id );
                g_touch_all_result[method].slider[slider_id] = pos_value;
                status = TOUCH_PRV_0_SUCCESS;
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
                    sensor_val = R_Get_Cap_Touch_Sensor_Data(method, TOUCH_PRV_3_WHEEL, gp_wheelInfo[wheel_id].elem_index[loop]);
                    g_wheel_data[loop] = sensor_val;
                }
                pos_value = wheel_decode( wheel_id );
                g_touch_all_result[method].wheel[wheel_id] = pos_value;
                status = TOUCH_PRV_0_SUCCESS;
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
                sensor_val = R_Get_Cap_Touch_Sensor_Data(method, TOUCH_PRV_1_MATRIX, loop);
                touch_key_decode(method, sensor_val, loop);
            }

            for (loop = 0; loop < g_key_info[method].key_max_group; loop++)
            {
                g_touch_all_result[method].matrix[loop]  = *(g_key_info[method].touch_result  + loop);
                status = TOUCH_PRV_0_SUCCESS;
            }
        }
    }


    return status;
}    /* End of function R_Set_Cap_Touch_Result_Create() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: touch_data_moving_average
* Description  : touch data moving average
* Arguments    : uint8_t method    : Measurement method : 0-8
* Return Value : none
***********************************************************************************************************************/
static void touch_data_moving_average( uint8_t method )
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
                *(g_self_add_sensor_pt[method]  + pt) = correction_sensor_cnt_create( method, pt, (pt + offset));
            }
            else
            {
                *(g_mutual_add_pri_sensor_pt[method]  + pt) = correction_pri_sensor_cnt_create( method, pt, (pt + offset));
                *(g_mutual_add_snd_sensor_pt[method]  + pt) = correction_snd_sensor_cnt_create( method, pt, (pt + offset));
            }
        }
        else
        {
            if (TOUCH_SELF_MODE == g_key_info[method].mode)
            {
                scount_work      = *(g_self_add_sensor_pt[method]  + pt);                   /* Get Add data            */
                scount_work     -= (scount_work / TOUCH_PRV_ADD_TIME);                      /* Average / TOUCH_PRV_ADD_TIME      */
                correction_work  = correction_sensor_cnt_create( method, pt,(pt + offset));
                scount_work     += (correction_work / TOUCH_PRV_ADD_TIME);                  /* Add Now data / TOUCH_PRV_ADD_TIME */
               *(g_self_add_sensor_pt[method]  + pt) = scount_work;                         /* Data store for next     */
            }
            else
            {
                scount_work      = *(g_mutual_add_pri_sensor_pt[method]  + pt);             /* Get Add data            */
                scount_work2     = *(g_mutual_add_snd_sensor_pt[method]  + pt);             /* Get Add data            */
                scount_work     -= (scount_work   / TOUCH_PRV_ADD_TIME);                    /* Average / TOUCH_PRV_ADD_TIME      */
                scount_work2    -= (scount_work2  / TOUCH_PRV_ADD_TIME);                    /* Average / TOUCH_PRV_ADD_TIME      */
                correction_work  = correction_pri_sensor_cnt_create( method, pt, (pt + offset));
                correction_work2 = correction_snd_sensor_cnt_create( method, pt, (pt + offset));
                scount_work     += (correction_work   / TOUCH_PRV_ADD_TIME);                /* Add Now data / TOUCH_PRV_ADD_TIME */
                scount_work2    += (correction_work2  / TOUCH_PRV_ADD_TIME);                /* Add Now data / TOUCH_PRV_ADD_TIME */
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
            if ((*(g_mutual_add_snd_sensor_pt[method] + (pt))) > (*(g_mutual_add_pri_sensor_pt[method] + (pt))))
            {
                *(g_mutual_sensor_diff_pt[method] + pt) = ((*(g_mutual_add_snd_sensor_pt[method] + (pt))) - (*(g_mutual_add_pri_sensor_pt[method] + (pt))));
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


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: correction_sensor_cnt_create
* Description  :
* Arguments    : uint8_t method    : Maeasurement method : 0-8
*              : uint8_t number    : Key number
* Return Value : none
***********************************************************************************************************************/
static uint16_t correction_sensor_cnt_create( uint8_t method, uint8_t ts_num, uint8_t number)
{
    uint8_t  loop;
    uint8_t  loop_coef;
    uint32_t cmp_val;
    uint32_t correct_box;
    uint32_t correct_box2;
    uint16_t correction_sensor_cnt;

    if ( 0 > (*(g_key_info[method].counter_magni + ts_num)))
    {
        cmp_val     = (*(g_self_ico_sensor_pt[method] + number)) * ((*(g_key_info[method].counter_magni + ts_num)) * (-1));
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

        /* save count */
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

                        /* save count */
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

                        /* save count */
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

                        /* save count */
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

                        /*save count */
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


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: correction_pri_sensor_cnt_create
* Description  :
* Arguments    : uint8_t method    : Maeasurement method : 0-8
*              : uint8_t number    : Key number
* Return Value : none
***********************************************************************************************************************/
static uint16_t correction_pri_sensor_cnt_create( uint8_t method, uint8_t ts_num, uint8_t number)
{
    uint8_t  loop;
    uint8_t  loop_coef;
    uint32_t cmp_val;
    uint32_t correct_box;
    uint32_t correct_box2;
    uint16_t correction_pri_sensor_cnt;

    if ( 0 > (*(g_key_info[method].counter_magni + ts_num)))
    {
        cmp_val     = (*(g_mutual_ico_pri_sensor_pt[method] + number)) * ((*(g_key_info[method].counter_magni + ts_num)) * (-1));
        cmp_val     =  cmp_val / 10;
        correct_box = *(g_mutual_ico_pri_sensor_pt[method] + number);
    }
    else
    {
        cmp_val     = ((*(g_mutual_ico_pri_sensor_pt[method] + number)) * 10) / (*(g_key_info[method].counter_magni + ts_num));
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

        /* save count */
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

                        /* save count */
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

                        /* save count */
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

                        /* save count */
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

                        /* save count */
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


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: correction_snd_sensor_cnt_create
* Description  :
* Arguments    : uint8_t method    : Maeasurement method : 0-8
*              : uint8_t number    : Key number
* Return Value : none
***********************************************************************************************************************/
static uint16_t correction_snd_sensor_cnt_create( uint8_t method, uint8_t ts_num, uint8_t number)
{
    uint8_t  loop;
    uint8_t  loop_coef;
    uint32_t cmp_val;
    uint32_t correct_box;
    uint32_t correct_box2;
    uint16_t correction_snd_sensor_cnt;

    if ( 0 > (*(g_key_info[method].counter_magni + ts_num)))
    {
        cmp_val     = (*(g_mutual_ico_snd_sensor_pt[method] + number)) * ((*(g_key_info[method].counter_magni + ts_num)) * (-1));
        cmp_val     =  cmp_val / 10;
        correct_box = *(g_mutual_ico_snd_sensor_pt[method] + number);
    }
    else
    {
        cmp_val     = ((*(g_mutual_ico_snd_sensor_pt[method] + number)) * 10) / (*(g_key_info[method].counter_magni + ts_num));
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

        /* save count */
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

                        /* save count */
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

                        /* save count */
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

                        /* save count */
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

                        /* save count */
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


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: r_touch_control_private
* Description  : This function is used to perform special operations internal to the driver and is not available
*                for public use.
* Arguments    : cmd -
*                    TOUCH_PCMD_PERFORM_OFFSET_TUNING
*                       Performs offset tuning and sets reference count for all methods
*                    TOUCH_PCMD_CLEAR_TUNING_FLAGS
*                       Clears tuning flags so can perform offset tuning again if desired.
*                p_arg -
*                    Pointer to command-specific argument.
*                    TOUCH_PCMD_PERFORM_OFFSET_TUNING........    (none)
*                    TOUCH_PCMD_CLEAR_TUNING_FLAGS...........    (none)
*
* Return Value : QE_SUCCESS -
*                    Command performed successfully.
*                QE_ERR_INVALID_ARG -
*                    Command is invalid.
*                ** For OFFSET TUNING only **
*                QE_ERR_BUSY -
*                    CTSU busy. Likely this function called via main loop (outside Open) while another scan is running.
*                QE_ERR_UNSUPPORTED_CLK_CFG-
*                    Unsupported clock configuration.
*                QE_ERR_ABNORMAL_TSCAP-
*                    TSCAP error detected during scan
*                QE_ERR_SENSOR_OVERFLOW-
*                    Sensor overflow detected during scan
*                QE_ERR_OT_MAX_OFFSET -
*                    Cannot tune SO0 offset any higher
*                QE_ERR_OT_MIN_OFFSET -
*                    Cannot tune SO0 offset any lower
*                QE_ERR_OT_WINDOW_SIZE -
*                    Tuning window too small. SO0 adjustments keep counts outside of window.
*                QE_ERR_OT_MAX_ATTEMPTS -
*                    Maximum scans performed and all sensors still not in target window.
***********************************************************************************************************************/
static qe_err_t r_touch_control_private(touch_pcmd_t pcmd, void *p_arg)
{
    qe_err_t        err=QE_SUCCESS;
    uint8_t         i;
    uint8_t         j;


    switch (pcmd)
    {
        case TOUCH_PCMD_CLEAR_TUNING_FLAGS:
        {
            for (i=0; i < g_open_num_methods; i++)
            {
                for (j=0; j < gp_ctsu_configs[i]->num_elements; j++)
                {
                    g_touch_tuning_info[i].result[j] = 0;   // window tune count
                    g_touch_tuning_info[i].ctsuso[j] = 0;   // tuning direction
                }

                g_touch_function[i].flag.tuning = 0;        // show method not tuned
                g_touch_function[i].flag.average = 0;       // show method has no running average
            }

            break;
        }

        case TOUCH_PCMD_PERFORM_OFFSET_TUNING:
        {
            /* WARNING! BUSY will be returned if a scan is in progress when this is called.
             *          Be sure to set CTSU method to desired method when this completes,
             *            have the Touch method correspond, and ensure cycling/non-cycling
             *            is set up as needed.
             */
            for (i=0; (i < g_open_num_methods) && (QE_SUCCESS == err); i++)
            {
                err = R_CTSU_Control(CTSU_CMD_SET_METHOD, i, NULL);
                if (QE_SUCCESS == err)
                {
                    err = method_offset_tuning(i);
                }
            }

            break;
        }

        /* FUTURE: TOUCH_PCMD_GET_MONITORING_INFO to support stand-alone monitor
         * loads p_arg structure with:
         *   g_mon_req_buf address, MON_REQ_BUF_SIZE, g_mon_reply_buf address, and MON_REPLY_BUF_SIZE
         * Enclose this case with #if (TOUCH_CFG_UPDATE_MONITOR == 1) conditional compile
         * "Connection" software will use this information to transfer messages over USB or UART
         *   between these buffers and remote monitor.
         */
        default:
        {
            err = QE_ERR_INVALID_ARG;
            break;
        }
    }

    return err;
} /* End of function r_touch_control_private() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
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
    uint8_t         *p_set_method=(uint8_t *)p_arg;
    touch_mlist_t   *p_list=(touch_mlist_t *)p_arg;
    touch_sensor_t  *p_bad_sensor=(touch_sensor_t *)p_arg;
    uint8_t         *p_last_method=(uint8_t *)p_arg;
    uint8_t         *p_clr_states_method=(uint8_t *)p_arg;

    qe_err_t        err=QE_SUCCESS;
    uint8_t         i;

#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((TOUCH_CMD_CYCLE_ALL_METHODS != cmd) && (NULL == p_arg))
    {
        return QE_ERR_NULL_PTR;
    }

    if ((cmd >= TOUCH_CMD_END_ENUM)
     || (((TOUCH_CMD_SET_METHOD == cmd) || (TOUCH_CMD_CLEAR_TOUCH_STATES == cmd))
     && ((*p_set_method) >= g_open_num_methods)))
    {
        return QE_ERR_INVALID_ARG;
    }

    if (TOUCH_CMD_CYCLE_METHOD_LIST == cmd)
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
        case TOUCH_CMD_GET_FAILED_SENSOR:
        {
            *p_bad_sensor = g_bad_sensor;       // structure value assignment
            break;
        }
        case TOUCH_CMD_CLEAR_TOUCH_STATES:
        {
            /* clear buttons */
            for (i=0; i < g_key_info[*p_clr_states_method].key_num; i++)
            {
                g_key_info[*p_clr_states_method].touch_cnt[i] = 0;
                g_key_info[*p_clr_states_method].non_touch_cnt[i] = 0;
            }

            for (i=0; i < g_key_info[*p_clr_states_method].key_max_group; i++)
            {
                g_key_info[*p_clr_states_method].in_touch[i] = 0;
                g_key_info[*p_clr_states_method].out_touch[i] = 0;
                g_key_info[*p_clr_states_method].touch_result[i] = 0;
            }

            /* clear sliders and wheels */
#if (QE_MAX_SLIDERS != 0)
            for (i=0; i < gp_touch_configs[*p_clr_states_method]->num_sliders; i++)
            {
                gp_touch_configs[*p_clr_states_method]->p_sliders[i].value = 0xFFFF;
            }
#endif
#if (QE_MAX_WHEELS != 0)
            for (i=0; i < gp_touch_configs[*p_clr_states_method]->num_wheels; i++)
            {
                gp_touch_configs[*p_clr_states_method]->p_wheels[i].value = 0xFFFF;
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
            for (i=0; i < g_open_num_methods; i++)
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


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
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
    uint8_t         i;
    self_ico_t      *p_self_data;
    mutual_ico_t    *p_mutual_data;

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


    *p_cnt = g_key_info[method].ena_num;

    if (TOUCH_SELF_MODE == g_key_info[method].mode)
    {
        p_self_data = g_self_raw_data_pt[method];
        for (i=0; i < (*p_cnt); i++, p_self_data++)
        {
            *p_buf++ = p_self_data->sen;
        }
    }
    else
    {
        p_mutual_data = g_mutual_raw_data_pt[method];
        for (i=0; i < (*p_cnt); i++, p_mutual_data++)
        {
            *p_buf++ = p_mutual_data->pri_sen;
            *p_buf++ = p_mutual_data->snd_sen;
        }

        (*p_cnt) *= 2;
    }

    return QE_SUCCESS;
} /* End of function R_TOUCH_GetRawData() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
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
    uint8_t     i;
    uint16_t    *p_data;

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


    *p_cnt = g_key_info[method].ena_num;

    if (TOUCH_SELF_MODE == g_key_info[method].mode)
    {
        p_data = g_self_sensor_cnt_pt[method];
    }
    else
    {
        p_data = g_mutual_sensor_diff_pt[method];
    }

    for (i=0; i < (*p_cnt); i++)
    {
        *p_buf++ = *p_data++;
    }

    return QE_SUCCESS;
} /* End of function R_TOUCH_GetData() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_TOUCH_GetBtnBaselines
* Description  : This function gets the long term moving average values (values which touch judgment compares against)
*                for button elements only. The values are loaded into the buffer in their respective element index
*                locations (same as GetData()).
* Arguments    : method -
*                    Method to get data for.
*                p_buf -
*                    Pointer to buffer to load data into.
*                    (Non-button sensor locations are not loaded).
*                p_cnt -
*                    Pointer to variable to load word-size of buffer
* Return Value : QE_SUCCESS -
*                    Command performed successfully.
*                QE_ERR_INVALID_ARG -
*                    "method" is invalid.
*                QE_ERR_NULL_PTR -
*                    "p_buf" or "p_cnt" is NULL.
***********************************************************************************************************************/
qe_err_t R_TOUCH_GetBtnBaselines(uint8_t method, uint16_t *p_buf, uint8_t *p_cnt)
{
    uint8_t     i;
    uint8_t     j;


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

    /* load expected size of buffer passed in */
    *p_cnt = g_key_info[method].ena_num;


    /* poke button baseline values into passed-in buffer */
    for (i=0; i < gp_touch_cfgs[method]->num_buttons; i++)
    {
        j = gp_touch_cfgs[method]->p_buttons[i].elem_index;     // get buffer index
        p_buf[j] = g_key_info[method].ref[i];                   // poke baseline value
    }

    return QE_SUCCESS;
} /* End of function R_TOUCH_GetBtnBaselines() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
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
*                QE_ERR_OT_INCOMPLETE
*                    Offset tuning for method not complete
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

    if (NULL == p_mask)
    {
        return QE_ERR_NULL_PTR;
    }
#endif

    if (1 != g_touch_function[method].flag.tuning)
    {
        err = QE_ERR_OT_INCOMPLETE;
    }
    else
    {
        mask = 0;

        /* .touch_result is the mask in a 16-bit array of .key_max_group size */
        for (i=0; i < g_key_info[method].key_max_group; i++)
        {
            /* convert array into single long long */
            mask |= ((uint64_t)(*(g_key_info[method].touch_result + i)) << (16 * i));
        }

        *p_mask = mask;
    }

    return err;
} /* End of function R_TOUCH_GetAllBtnStates() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
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
*                QE_ERR_OT_INCOMPLETE
*                    Offset tuning for method not complete
***********************************************************************************************************************/
qe_err_t R_TOUCH_GetSliderPosition(uint16_t slider_id, uint16_t *p_position)
{
    uint8_t     method;
    uint8_t     index;
    qe_err_t    err=QE_SUCCESS;

    method = slider_id >> 8;    // high byte of id is method
    index = slider_id & 0xFF;   // low byte of id is slider number/index for method

#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((method >= g_open_num_methods)
     || (index >= gp_touch_configs[method]->num_sliders))
    {
        return QE_ERR_INVALID_ARG;
    }

    if (NULL == p_position)
    {
        return QE_ERR_NULL_PTR;
    }
#endif

    if (1 != g_touch_function[method].flag.tuning)
    {
        err = QE_ERR_OT_INCOMPLETE;
    }
    else
    {
        *p_position = gp_touch_configs[method]->p_sliders[index].value;
    }

    return err;
} /* End of function R_TOUCH_GetSliderPosition() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
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
*                QE_ERR_OT_INCOMPLETE
*                    Offset tuning for method not complete
***********************************************************************************************************************/
qe_err_t R_TOUCH_GetWheelPosition(uint16_t wheel_id, uint16_t *p_position)
{
    uint8_t     method;
    uint8_t     index;
    qe_err_t    err=QE_SUCCESS;

    method = wheel_id >> 8;     // high byte of id is method
    index = wheel_id & 0xFF;    // low byte of id is slider number/index for method

#if (TOUCH_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((method >= g_open_num_methods)
     || (index >= gp_touch_configs[method]->num_wheels))
    {
        return QE_ERR_INVALID_ARG;
    }

    if (NULL == p_position)
    {
        return QE_ERR_NULL_PTR;
    }
#endif


    if (1 != g_touch_function[method].flag.tuning)
    {
        err = QE_ERR_OT_INCOMPLETE;
    }
    else
    {
        *p_position = gp_touch_configs[method]->p_wheels[index].value;
    }

    return err;
} /* End of function R_TOUCH_GetWheelPosition() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
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
} /* End of function R_TOUCH_Close() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_TOUCH_GetVersion
* Description  : Returns the current version of this module. The version number is encoded where the top 2 bytes are the
*                major version number and the bottom 2 bytes are the minor version number. For example, Version 4.25
*                would be returned as 0x00040019.
* Arguments    : none
* Return Value : Version of this module.
***********************************************************************************************************************/
#if (!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5))
#pragma inline(R_TOUCH_GetVersion)
#else
R_BSP_PRAGMA_INLINE(R_TOUCH_GetVersion)
#endif
uint32_t R_TOUCH_GetVersion (void)
{
    /* These version macros are defined in r_flash_if.h. */
    return ((((uint32_t)QETOUCH_RX_VERSION_MAJOR) << 16) | (uint32_t)QETOUCH_RX_VERSION_MINOR);
} /* End of function R_TOUCH_GetVersion() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (TOUCH_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section
#elif (TOUCH_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE_END
#endif

