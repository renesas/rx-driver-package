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
* File Name    : r_qemonitor.c
* Version      : 1.00
* Device(s)    : R5F5113xAxFP,R5F5231xAxFP,R5F51305AxFN
* Description  : This file handles the QE Tool monitor updates.
***********************************************************************************************************************/

/***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 04.10.2018 1.00    First Release
*              : 16.10.2018 1.10    Fixed bug in monitor_update_data() where replies to the second command were not
*                                   always written.
*              : 29.04.2019 1.20    Modified for GCC/IAR compatibility.
***********************************************************************************************************************/
/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <string.h>
#include "platform.h"
#include "r_touch_qe_if.h"
#include "r_touch_qe_private.h"
#include "qe_common.h"
#include "r_monitor.h"


#if (TOUCH_CFG_UPDATE_MONITOR == 1)
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Note: When doing a structure of structures, the sub-structures (e.g. hdr/trailer) must be packed
 * even if they contain all uint8_t, or the sub-structures will be located on a 4-byte boundary.
 */
#if (!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5))
#pragma pack
typedef struct
{
    mon_req_hdr_t   hdr;
    mon_trailer_t   trailer;
} rd_req_t;
#pragma unpack
#else
R_BSP_PRAGMA_PACK
typedef struct
{
    mon_req_hdr_t   hdr;
    mon_trailer_t   trailer;
} rd_req_t;
R_BSP_PRAGMA_UNPACK
#endif


/***********************************************************************************************************************
External functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Global variables and functions
***********************************************************************************************************************/
uint8_t  g_mon_req_buf[MON_REQ_BUF_SIZE];
uint8_t  g_mon_reply_buf[MON_REPLY_BUF_SIZE];

static uint8_t * process_read_request(rd_req_t *p_rd_req, uint8_t *p_data_buf, uint8_t toggle_value, uint8_t data_id);


/***********************************************************************************************************************
* Function Name: monitor_init
* Description  : This function initializes the memory buffers prior to performing monitoring operations.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void monitor_init(void)
{
    mon_req_hdr_t   *p_hdr;

    /* initialize request buffer to not process any data */
    p_hdr = (mon_req_hdr_t *)g_mon_req_buf;
    p_hdr->data_type = MON_DATA_STOP;

    /* initialize data buffer to invalid data (request's first and last bytes don't match) */
    memset(g_mon_reply_buf, 0, MON_REPLY_MAX_SIZE);
    g_mon_reply_buf[0] = 1;

} /* End of function monitor_init() */


/***********************************************************************************************************************
* Function Name: monitor_update_data
* Description  : This function handles the high level logic of determining whether a new request is received or not,
*                whether to auto-update the reply for the last request, and whether there are two commands to be
*                processed or just one. Note that commands are received asynchronously and can be any where in the
*                process of being written when inspected. Matching starting and end IDs indicate that the message is
*                completely written.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void monitor_update_data(void)
{
    uint8_t         mon_req_copy_buf[MON_REQ_BUF_SIZE];
    mon_req_hdr_t   *p_hdr;
    rd_req_t        *p_rd_req;
    static uint8_t  *p_data_buf;
    static uint8_t  data_id=1;
    static uint8_t  expected_toggle=1;      // 1st request from Tool must have toggle set to 0
    static uint16_t last_req_id=65535;
    static bool     part2_pending=false;


    /* Inspect copy of request buffer so contents do not change while processing */
    memcpy(mon_req_copy_buf, g_mon_req_buf, MON_REQ_BUF_SIZE);

    /* cast start of buffer to header type */
    p_hdr = (mon_req_hdr_t *)mon_req_copy_buf;

    /* if data is requested */
    if (MON_DATA_STOP != p_hdr->data_type)
    {
        /* if it's a read request */
        if (p_hdr->flags & MON_FLG_READ_REQ)
        {
            /* cast buffer to read request format */
            p_rd_req = (rd_req_t *)mon_req_copy_buf;

            /* if the request is ready (not in process of being written) */
            if (p_rd_req->hdr.s_id == p_rd_req->trailer.e_id)
            {
                /* check if dual command, and set flag for later processing if so */
                if ((last_req_id != (uint16_t)p_rd_req->hdr.s_id)
                 && (0 != (p_hdr->flags & MON_FLG_2_PART_REQ)))
                {
                    part2_pending = true;
                    expected_toggle ^= 1;
                }

                /* if new request or repeat last command */
                if ((last_req_id != (uint16_t)p_rd_req->hdr.s_id)
                 || ((p_rd_req->hdr.flags & MON_FLG_AUTO_UPDATE) != 0))
                {
                    /* process main request */
                    data_id++;
                    p_data_buf = process_read_request(p_rd_req, g_mon_reply_buf, expected_toggle, data_id);

                    /* save id */
                    last_req_id = (uint16_t)p_hdr->s_id;
                }

                /* process optional secondary read request */
                p_rd_req++;
                if ((true == part2_pending)                             // if there is a 2nd request
                 && (p_rd_req->hdr.s_id == p_rd_req->trailer.e_id)      // and 2nd request shows ready (but could be n*256 requests old)
                 && (p_rd_req->hdr.s_id == p_hdr->s_id)                 // and matches 1st request ID
                 && (p_rd_req->trailer.part2_toggle == expected_toggle))// and not n*256 requests old
                {
                    process_read_request(p_rd_req, p_data_buf, expected_toggle, data_id);
                }
            }
        }

        else /* write request */
        {
            ;   // future placeholder
        }
    }

} /* End of function monitor_update_data() */


/***********************************************************************************************************************
* Function Name: process_read_request
* Description  : This function does the processing for a single monitoring request.
* Arguments    : p_rd_req -
*                    Pointer to request to process
*                p_data_buf -
*                    Pointer to buffer to write reply to
*                toggle_value -
*                    Value to use in the toggle-byte in the reply
*                data_id -
*                    Value to use for start and end IDs
* Return Value : Pointer to byte after last byte of reply.
***********************************************************************************************************************/
static uint8_t * process_read_request(rd_req_t *p_rd_req, uint8_t *p_data_buf, uint8_t toggle_value, uint8_t data_id)
{
    uint8_t     i;
    uint8_t     j;
    uint8_t     method=p_rd_req->hdr.method;
    mon_btn_t   *p_btn_data;
    uint16_t    *p_sldr_whl_data;
    btn_ctrl_t  *p_btn_ctrl;
    sldr_ctrl_t *p_sldr_ctrl;
    wheel_ctrl_t    *p_wheel_ctrl;

    /* cast start of buffer to reply header format */
    mon_reply_hdr_t *p_hdr=(mon_reply_hdr_t *)p_data_buf;
    mon_trailer_t   *p_trailer;


    /* write header */
    p_hdr->s_id = data_id;
    p_hdr->format = 0;
    p_data_buf += sizeof(mon_reply_hdr_t);


    /* write button data (if any) */
    if ((MON_DATA_BUTTONS == p_rd_req->hdr.data_type) || (MON_DATA_BTNS_AND_SLDRS == p_rd_req->hdr.data_type))
    {
        /* cast current location in buffer to button data */
        p_btn_data = (mon_btn_t *)p_data_buf;

        /* for each button possible */
        for (i=0; i < g_key_info[method].key_num; i++)
        {
            /* if data requested */
            if (p_rd_req->hdr.button_mask & (1ULL << i))
            {
                /* write data structure */
                p_btn_data->reference = g_key_info[method].ref[i];
                p_btn_data->touch_delta = g_key_info[method].user_thr[i];

                p_btn_ctrl = &gp_touch_cfgs[method]->p_buttons[i];
                if (TOUCH_SELF_MODE == g_key_info[method].mode)
                {
                    p_btn_data->primary = g_self_sensor_cnt_pt[method][p_btn_ctrl->elem_index];
                    p_btn_data->secondary = 0;
                }
                else
                {
                    p_btn_data->primary = g_mutual_add_pri_sensor_pt[method][p_btn_ctrl->elem_index];
                    p_btn_data->secondary = g_mutual_add_snd_sensor_pt[method][p_btn_ctrl->elem_index];
                }

                p_btn_data++;
                p_data_buf += sizeof(mon_btn_t);
            }
        }

        /* write on/off mask for all buttons */
        R_TOUCH_GetAllBtnStates(method, (uint64_t *)p_data_buf);
        p_data_buf += sizeof(uint64_t);
    }


    /* write slider/wheel data (if any) */
    if ((MON_DATA_SLIDERS == p_rd_req->hdr.data_type) || (MON_DATA_BTNS_AND_SLDRS == p_rd_req->hdr.data_type))
    {
        /* cast for data portion of reply */
        p_sldr_whl_data = (uint16_t *)p_data_buf;

        /* for each slider possible */
        for (i=0; i < gp_touch_cfgs[method]->num_sliders; i++)
        {
            /* if data requested */
            if (p_rd_req->hdr.slider_mask & (1 << i))
            {
                p_sldr_ctrl = &gp_touch_cfgs[method]->p_sliders[i];

                /* write position */
                *p_sldr_whl_data++ = p_sldr_ctrl->value;
                p_data_buf += sizeof(uint16_t);

                /* write sensor counts */
                if (p_rd_req->hdr.flags & MON_FLG_INCL_SLDR_COUNTS)
                {
                    for (j=0; j < p_sldr_ctrl->num_elements; j++)
                    {
                        *p_sldr_whl_data++ = g_self_sensor_cnt_pt[method][p_sldr_ctrl->elem_index[j]];
                        p_data_buf += sizeof(uint16_t);
                    }
                }
            }
        }

        /* for each wheel possible */
        for (i=0; i < gp_touch_cfgs[method]->num_wheels; i++)
        {
            if (p_rd_req->hdr.wheel_mask & (1 << i))
            {
                p_wheel_ctrl = &gp_touch_cfgs[method]->p_wheels[i];

                /* write position */
                *p_sldr_whl_data++ = p_wheel_ctrl->value;
                p_data_buf += sizeof(uint16_t);

                /* write sensor counts */
                if (p_rd_req->hdr.flags & MON_FLG_INCL_SLDR_COUNTS)
                {
                    for (j=0; j < p_wheel_ctrl->num_elements; j++)
                    {
                        *p_sldr_whl_data++ = g_self_sensor_cnt_pt[method][p_wheel_ctrl->elem_index[j]];
                        p_data_buf += sizeof(uint16_t);
                    }
                }
            }
        }
    }


    /* write active method numbers (IDs/indexes) */
    else if (MON_DATA_ACTIVE_METHODS == p_rd_req->hdr.data_type)
    {
        *p_data_buf++ = g_mlist.num_methods;
        for (i=0; i < g_mlist.num_methods; i++)
        {
            *p_data_buf++ = g_mlist.methods[i];
        }
    }
    else
    {
        ;   // coding standard requirement
    }

    /* write trailer */
    p_trailer = (mon_trailer_t *)p_data_buf;
    p_trailer->part2_toggle = toggle_value;     // field only valid when a part-2 reply
    p_trailer->e_id = data_id;
    p_data_buf += sizeof(mon_trailer_t);

    return p_data_buf;
} /* End of function process_read_request() */

#endif /* (TOUCH_CFG_UPDATE_MONITOR == 1) */
