/**********************************************************************************************************************
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
 *
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : PIDConf.c
 * Version      : 1.30
 * Description  : Touch panel configuration.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 31.07.2020 1.00     First Release
 *         : 04.09.2020 1.10     Update to adjust r_emwin_rx_config.h file.
 *         : 11.12.2020 1.20     Update to adjust emWin v6.14g. Modify multi-touch and timer function.
 *                               Adjust GCC and IAR compilers.
 *         : 31.03.2021 1.30     Update to adjust the spec of Smart Configurator and QE for Display.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "GUI.h"
#include "GUIConf.h"

#if (GUI_SUPPORT_TOUCH == 1)
#include "platform.h"
#include "r_sci_iic_rx_if.h"
#include "r_cmt_rx_if.h"
#include "r_gpio_rx_if.h"
#include "r_emwin_rx_config.h"
#endif

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/
#if (GUI_SUPPORT_TOUCH == 1)
/* Holds information about coordinates and ID. */
typedef struct
{
    uint8_t x_high;       /* Bit 6..7 - EventFlag, Bit 0..3 x_high */
    uint8_t x_low;
    uint8_t y_high;       /* Bit 4..7 - TouchID, Bit 0..3 y_high */
    uint8_t y_low;
    uint8_t id;
} st_touch_data_t;

/* Holds information about different touch points, mode, gesture and number of points. */
typedef struct
{
    uint8_t device_mode;
    uint8_t gesture_id;
    uint8_t num_points;
} st_report_data_t;

/*  Used by this module to calculate the different MT flags and events. */
typedef struct
{
    uint16_t x_pos;
    uint16_t y_pos;
    uint8_t  id;
} st_point_data_t;
#endif

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
#if (GUI_SUPPORT_TOUCH == 1)
static int32_t s_layer_index;
static uint8_t s_a_buffer[GUI_PID_BUFFER_SIZE * 6 + 3] = {0};

static sci_iic_info_t s_iic_info;
static uint8_t s_iic_busy;

#if (EMWIN_USE_MULTITOUCH == 1)
static uint8_t s_a_active_ids[EMWIN_MAX_NUM_TOUCHPOINTS];
#endif
#endif

#if (GUI_SUPPORT_TOUCH == 1)
#if (EMWIN_USE_MULTITOUCH == 0)
/**********************************************************************************************************************
 * Function Name: cb_sci_iic
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void cb_sci_iic(void)
{
    sci_iic_mcu_status_t      iic_status;
    volatile sci_iic_return_t ret;

    ret = R_SCI_IIC_GetStatus(&s_iic_info, &iic_status);
    if ((SCI_IIC_SUCCESS == ret) && (1 == iic_status.BIT.NACK))
    {
        static GUI_PID_STATE s_state_pid;
        static int32_t       s_is_touched;
        st_report_data_t     report;
        st_touch_data_t      touch_point;
        uint8_t              * p_buffer;

        /* Handle just one touch info. */
        s_state_pid.Layer  = s_layer_index;          /* Set layer who should handle touch */
        p_buffer           =  s_a_buffer;
        report.device_mode = *p_buffer++;            /* Get device mode, 000b - Work Mode, 001b - Factory Mode */
        report.gesture_id  = *p_buffer++;            /* GestureID:  0x10 Move UP
                                                      *             0x14 Move Left
                                                      *             0x18 Move Down
                                                      *             0x1C Move Right
                                                      *             0x48 Zoom In
                                                      *             0x49 Zoom Out
                                                      *             0x00 No Gesture */
        report.num_points   = *p_buffer++;           /* Number of points */
        touch_point.x_high  = (*p_buffer ++) & 0x0F; /* Get the upper 4 bits of the x position */
        touch_point.x_low   = *p_buffer++;           /* and the lower 8 bits */
        touch_point.y_high  = (*p_buffer ++) & 0x0F; /* Get the upper 4 bits of the y position */
        touch_point.y_low   = *p_buffer++;           /* and the lower 8 bits */

        /* Check if we have a touch detected */
        if (report.num_points)
        {
            s_is_touched        = 1; /* Remember that we have a touch, needed for generating up events */
            s_state_pid.Pressed = 1; /* State is pressed */

            /* Shift bits for x- and y- coordinate to the correct position */
            s_state_pid.x       = (((touch_point.x_high & 0x0F) << 8) | touch_point.x_low);
            s_state_pid.y       = (((touch_point.y_high & 0x0F) << 8) | touch_point.y_low);

            /* Pass touch data to emWin */
            GUI_TOUCH_StoreStateEx(&s_state_pid);
        }
        else
        {
            if (s_is_touched)
            {                            /* If we had a touch, */
                s_is_touched        = 0; /* now we don't. */
                s_state_pid.Pressed = 0; /* So, state is not pressed. */

                /* Tell emWin */
                GUI_TOUCH_StoreStateEx(&s_state_pid);
            }
        }
        s_iic_busy = 0;
    }
}
/**********************************************************************************************************************
 * End of function cb_sci_iic
 *********************************************************************************************************************/
#else /* EMWIN_USE_MULTITOUCH == 0 */

/**********************************************************************************************************************
 * Function Name: active_id_found
 * Description  : Checks if the given ID is part of the currently active IDs.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static int32_t active_id_found(uint8_t id)
{
    int32_t i;
    uint8_t * p_id;

    p_id = s_a_active_ids;
    i    = GUI_COUNTOF(s_a_active_ids);
    do
    {
        if (*p_id++ == id)
        {
            return 1;
        }
    } while (--i); /* WAIT_LOOP */

    return 0;
}
/**********************************************************************************************************************
 * End of function active_id_found
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: store_id
 * Description  : Finds a free 'slot' for the given ID and puts it to the active IDs.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static int32_t store_id(uint8_t id)
{
    int32_t i;
    uint8_t * p_id;

    p_id = s_a_active_ids;
    i    = GUI_COUNTOF(s_a_active_ids);
    do
    {
        if (*p_id == 0)
        {
            *p_id = id;
            return 0;
        }
        p_id++;
    } while (--i); /* WAIT_LOOP */

    return 1;
}
/**********************************************************************************************************************
 * End of function store_id
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: current_id_found
 * Description  : Checks if the given ID is part of the given st_point_data_t array.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static int32_t current_id_found(uint8_t id, st_point_data_t * p_point_data, int32_t num_points)
{
    st_point_data_t * p_i;
    int32_t i;

    /* WAIT_LOOP */
    for (p_i = p_point_data, i = 0; i < num_points; i++, p_i++)
    {
        if (p_i->id == id)
        {
            return 1;
        }
    }

    return 0;
}
/**********************************************************************************************************************
 * End of function current_id_found
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: create_input
 * Description  : .
 * Arguments    : .
 * Return Value : Fills the given GUI_MTOUCH_INPUT structure with the given coordinates
 *                of the st_point_data_t data structure. In case of UP events there is no
 *                data and only the given ID is used.
 *********************************************************************************************************************/
static void create_input(uint16_t flags, uint8_t id, st_point_data_t * p_point_data, GUI_MTOUCH_INPUT * p_input)
{
    if (p_point_data)
    {
        p_input->x  = p_point_data->x_pos;
        p_input->y  = p_point_data->y_pos;
        p_input->Id = p_point_data->id;
    }
    else
    {
        p_input->Id = id;
    }
    p_input->Flags = flags;
}
/**********************************************************************************************************************
 * End of function create_input
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: create_up_inputs
 * Description  : Checks if the already existing active touch points exist in current
 *                data. For each non existing active ID in the current data an UP event
 *                is created.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void create_up_inputs(st_point_data_t * p_point_data, int32_t num_points,
                            GUI_MTOUCH_INPUT ** pp_input, int32_t * p_num_inputs)
{
    GUI_MTOUCH_INPUT * p_input;
    int32_t i;
    uint8_t * p_id;
    uint8_t id;

    p_input = *pp_input;
    p_id = s_a_active_ids;
    i   = GUI_COUNTOF(s_a_active_ids);
    do
    {
        id = *p_id;
        if (id)
        {
            if (current_id_found(id, p_point_data, num_points) == 0)
            {
                create_input(GUI_MTOUCH_FLAG_UP, id, NULL, p_input);
                (*p_num_inputs)++;
                p_input++;
                *p_id = 0;
            }
        }
        p_id++;
    } while (--i); /* WAIT_LOOP */
    *pp_input = p_input;
}
/**********************************************************************************************************************
 * End of function create_up_inputs
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: create_move_and_down_inputs
 * Description  : Fills the given array of GUI_MTOUCH_INPUT structures with data of current
 *                st_point_data_t array. If an item already exist a MOVE event is created,
 *                otherwise a DOWM event.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void create_move_and_down_inputs(st_point_data_t * p_point_data, int32_t num_points, GUI_MTOUCH_INPUT * p_input)
{
    int32_t i;
    int32_t found;

    /* WAIT_LOOP */
    for (i = 0; i < num_points; i++, p_point_data++, p_input++)
    {
        found       = active_id_found(p_point_data->id);
        p_input->x  = p_point_data->x_pos;
        p_input->y  = p_point_data->y_pos;
        p_input->Id = p_point_data->id;
        if (found)
        {
            p_input->Flags = GUI_MTOUCH_FLAG_MOVE;
        }
        else
        {
            p_input->Flags = GUI_MTOUCH_FLAG_DOWN;
            store_id(p_point_data->id);
        }
    }
}
/**********************************************************************************************************************
 * End of function create_move_and_down_inputs
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: cb_sci_iic
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void cb_sci_iic(void)
{
    sci_iic_mcu_status_t      iic_status;
    volatile sci_iic_return_t ret;
    GUI_MTOUCH_INPUT          * p_input;
    GUI_MTOUCH_EVENT          event;
    GUI_MTOUCH_INPUT          a_input[EMWIN_MAX_NUM_TOUCHPOINTS];
    st_point_data_t           a_point_data[EMWIN_MAX_NUM_TOUCHPOINTS];
    st_report_data_t          report;
    st_touch_data_t           touch_point;
    int32_t                   num_inputs;
    int32_t                   i;
    uint8_t                   num_points = 0;
    int32_t                   x_coord;
    int32_t                   y_coord;
    uint8_t                   * p_buffer;

    ret = R_SCI_IIC_GetStatus(&s_iic_info, &iic_status);
    if ((SCI_IIC_SUCCESS == ret) && (1 == iic_status.BIT.NACK))
    {
        p_buffer           =  s_a_buffer;
        report.device_mode = *p_buffer++;            /* Get device mode, 000b - Work Mode, 001b - Factory Mode */
        report.gesture_id  = *p_buffer++;            /* GestureID:  0x10 Move UP
                                                      *             0x14 Move Left
                                                      *             0x18 Move Down
                                                      *             0x1C Move Right
                                                      *             0x48 Zoom In
                                                      *             0x49 Zoom Out
                                                      *             0x00 No Gesture */
        report.num_points  = *p_buffer++;            /* Number of points */
        if(report.num_points <= EMWIN_MAX_NUM_TOUCHPOINTS)
        {
            num_points     =  report.num_points;
        }

        /* Reading point data is only required if there is a touch point */
        if (num_points)
        {
            /* Get coordinates and IDs from buffer */
            /* WAIT_LOOP */
            for (i = 0; i < num_points; i++)
            {
                touch_point.x_high  = (*p_buffer++) & 0x0F; /* Get the upper 4 bits of the x position */
                touch_point.x_low   =  *p_buffer++;         /* and the lower 8 bits */
                touch_point.id      = (*p_buffer)   & 0xF0; /* Extract the touch point ID */
                touch_point.y_high  = (*p_buffer++) & 0x0F; /* Get the upper 4 bits of the y position */
                touch_point.y_low   =  *p_buffer++;         /* and the lower 8 bits */

                /* Increment buffer twice since we have two dummy bytes */
                p_buffer++;
                p_buffer++;

                /* Calculate coordinate values */
                x_coord = ((touch_point.x_high & 0x0F) << 8 | touch_point.x_low);
                y_coord = ((touch_point.y_high & 0x0F) << 8 | touch_point.y_low);

                /* Add 1 to ID because TC counts from 0 and emWin can't handle an ID with 0 */
                a_point_data[i].id    = touch_point.id + 1;
                a_point_data[i].x_pos = x_coord;
                a_point_data[i].y_pos = y_coord;
            }
        }

        /* Independent of num_points check if UP-inputs need to be generated */
        p_input    = a_input;
        num_inputs = 0;
        create_up_inputs(a_point_data, num_points, &p_input, &num_inputs);

        /* Create MOVE- and DOWN-inputs only for current points */
        if (num_points)
        {
            create_move_and_down_inputs(a_point_data, num_points, p_input);
            num_inputs += num_points;
        }

        /* If any input exists, store an event into emWin buffer */
        if (num_inputs)
        {
            event.LayerIndex = s_layer_index;
            event.NumPoints  = num_inputs;
            GUI_MTOUCH_StoreEvent(&event, a_input);
        }
        s_iic_busy = 0;
    }
}
/**********************************************************************************************************************
 * End of function cb_sci_iic
 *********************************************************************************************************************/
#endif /* EMWIN_USE_MULTITOUCH == 0 */

/**********************************************************************************************************************
 * Function Name: exec
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void exec(void)
{
    /* Read all touch points to clear the buffer on TC side */
    static uint8_t s_slave_addr_eeprom[] = { EMWIN_SLAVE_ADDRESS }; /* Slave address */
    static uint8_t s_access_addr1[]      = { 0x00 };                /* 1st data field */
    volatile sci_iic_return_t ret;

    if (s_iic_busy)
    {
        return;
    }

    /* Sets IIC Information */
    s_iic_info.p_slv_adr    = s_slave_addr_eeprom;
    s_iic_info.p_data1st    = s_access_addr1;
    s_iic_info.p_data2nd    = s_a_buffer;
    s_iic_info.dev_sts      = SCI_IIC_NO_INIT;
    s_iic_info.cnt1st       = sizeof(s_access_addr1);
    s_iic_info.cnt2nd       = sizeof(s_a_buffer);
    s_iic_info.callbackfunc = &cb_sci_iic;

    /* Start Master Receive */
    ret = R_SCI_IIC_MasterReceive(&s_iic_info);
    if (SCI_IIC_SUCCESS == ret)
    {
        s_iic_busy = 1;
    }
}
/**********************************************************************************************************************
 * End of function exec
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: cb_timer
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static void cb_timer(void * p_data)
{
    exec();
}
/**********************************************************************************************************************
 * End of function cb_timer
 *********************************************************************************************************************/
#endif /* GUI_SUPPORT_TOUCH == 1 */

/**********************************************************************************************************************
 * Function Name: PID_X_SetLayerIndex
 * Description  : Set layer index.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void PID_X_SetLayerIndex(int layer_index)
{
#if (GUI_SUPPORT_TOUCH == 1)
    s_layer_index = layer_index;
#endif
}
/**********************************************************************************************************************
 * End of function PID_X_SetLayerIndex
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: PID_X_Init
 * Description  : Used to initialize Pointer Input Device.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void PID_X_Init(void)
{
#if (GUI_SUPPORT_TOUCH == 1)
    uint32_t channel;
    sci_iic_return_t sci_ret;

    GUI_X_Delay(300);

#if (EMWIN_USE_TOUCH_IC_RESET_PIN == 1)
    /* Reset touch ic */
    R_GPIO_PinDirectionSet(EMWIN_TOUCH_IC_RESET_PIN, GPIO_DIRECTION_OUTPUT);
    R_GPIO_PinWrite(EMWIN_TOUCH_IC_RESET_PIN, GPIO_LEVEL_LOW);
    GUI_X_Delay(10);
    R_GPIO_PinWrite(EMWIN_TOUCH_IC_RESET_PIN, GPIO_LEVEL_HIGH);
    GUI_X_Delay(300);
#endif

    /* Sets IIC Information */
    s_iic_info.ch_no = EMWIN_SCI_IIC_NUMBER;

    /* SCI open */
    sci_ret = R_SCI_IIC_Open(&s_iic_info);
    if (SCI_IIC_SUCCESS != sci_ret)
    {
        return; /* Error */
    }

    /* Create timer for executing touch */
    R_CMT_CreatePeriodic(50, cb_timer, &channel);
#if (EMWIN_USE_MULTITOUCH == 1)
    GUI_MTOUCH_Enable(1);
#endif
#endif
}
/**********************************************************************************************************************
 * End of function PID_X_Init
 *********************************************************************************************************************/

#if (GUI_SUPPORT_TOUCH == 1)
/**********************************************************************************************************************
 * Function Name: GUI_TOUCH_X_ActivateX
 * Description  : This routine is not implemented.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void GUI_TOUCH_X_ActivateX(void)
{
    /* This routine is called from guI_TOUCH_Exec() to activate the measurement of the X-axis. GUI_TOUCH_X_ActivateX()
     * swithces on the measurement voltage to the X-axis. Switching on the voltage in X means the value for the Y-axis
     * can be measured. */
    R_BSP_NOP();
}
/**********************************************************************************************************************
 * End of function GUI_TOUCH_X_ActivateX
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: GUI_TOUCH_X_ActivateY
 * Description  : This routine is not implemented.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
void GUI_TOUCH_X_ActivateY(void)
{
    /* This routine is called from guI_TOUCH_Exec() to activate the measurement of the Y-axis. GUI_TOUCH_X_ActivateY()
     * swithces on the measurement voltage to the Y-axis. Switching on the voltage in Y means the value for the X-axis
     * can be measured. */
    R_BSP_NOP();
}
/**********************************************************************************************************************
 * End of function GUI_TOUCH_X_ActivateY
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: GUI_TOUCH_X_MeasureX
 * Description  : This routine is not implemented.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
int GUI_TOUCH_X_MeasureX(void)
{
    /* This routine is called from GUI_TOUCH_Exec() to return the measurement values from the A/D converter for the
    X-axis. */
    return 0;
}
/**********************************************************************************************************************
 * End of function GUI_TOUCH_X_MeasureX
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: GUI_TOUCH_X_MeasureY
 * Description  : This routine is not implemented.
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
int GUI_TOUCH_X_MeasureY(void)
{
    /* This routine is called from GUI_TOUCH_Exec() to return the measurement values from the A/D converter for the
    Y-axis. */
    return 0;
}
/**********************************************************************************************************************
 * End of function GUI_TOUCH_X_MeasureY
 *********************************************************************************************************************/
#endif /* GUI_SUPPORT_TOUCH == 1 */
