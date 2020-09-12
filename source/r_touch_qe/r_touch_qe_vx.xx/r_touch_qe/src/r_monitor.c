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
* History      : DD.MM.YYYY Version  Description
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
#pragma pack
typedef struct
{
    mon_req_hdr_t   hdr;
    mon_trailer_t   trailer;
} rd_req_t;
#pragma unpack


/***********************************************************************************************************************
External functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Global variables and functions
***********************************************************************************************************************/
uint8_t  g_mon_req_buf[MON_REQ_BUF_SIZE];
uint8_t  g_mon_reply_buf[MON_REPLY_BUF_SIZE];

uint8_t * process_read_request(rd_req_t *p_rd_req, uint8_t *p_data_buf, uint8_t toggle_value, uint8_t data_id);


/***********************************************************************************************************************
* Function Name: monitor_init
* Description  : This function
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
}


/***********************************************************************************************************************
* Function Name: monitor_update_data
* Description  : This function
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void monitor_update_data(void)
{
    mon_req_hdr_t   *p_hdr;
    rd_req_t        *p_rd_req;
    uint8_t         *p_data_buf;
    static uint8_t  data_id=1;
    static uint8_t  expected_toggle=1;      // 1st request from Tool must have toggle set to 0
    static uint16_t last_req_id=65535;


    p_hdr = (mon_req_hdr_t *)g_mon_req_buf;

    /* if data is requested */
    if (p_hdr->data_type != MON_DATA_STOP)
    {
        /* if it's a read request */
        if (p_hdr->flags & MON_FLG_READ_REQ)
        {
            p_rd_req = (rd_req_t *)g_mon_req_buf;

            /* if the request is ready (not in process of being written) */
            if (p_rd_req->hdr.s_id == p_rd_req->trailer.e_id)
            {
                /* if data needs to be updated */
                if ((last_req_id != (uint16_t)p_rd_req->hdr.s_id)   // new request
                 || (p_rd_req->hdr.flags & MON_FLG_AUTO_UPDATE))    // or repeat last request
                {
                    /* process main request */
                    p_data_buf = process_read_request(p_rd_req, g_mon_reply_buf, expected_toggle, data_id);

                    /* process optional secondary read request */
                    p_rd_req++;                                             // point to optional 2nd request
                    if ((p_hdr->flags & MON_FLG_2_PART_REQ)                 // if 1st request states 2-parts
                     && (p_rd_req->hdr.s_id == p_rd_req->trailer.e_id)      // and 2nd request shows ready (but could be 256 requests old)
                     && (p_rd_req->hdr.s_id == p_hdr->s_id))                // and matches 1st request ID
                    {
                        if (last_req_id != (uint16_t)p_hdr->s_id)           // if this is a new 2-part request
                        {
                            expected_toggle ^= 1;                           // change expected toggle value
                        }

                        /* if this is a current request (not from 256 requests ago) */
                        if (p_rd_req->trailer.part2_toggle == expected_toggle)
                        {
                            process_read_request(p_rd_req, p_data_buf, expected_toggle, data_id);
                        }
                    }

                    last_req_id = (uint16_t)p_hdr->s_id;
                    data_id++;
                }
            }
        }

        else /* write request */
        {

        }
    }

}


/***********************************************************************************************************************
* Function Name: process_read_request
* Description  : This function
* Arguments    :
* Return Value :
***********************************************************************************************************************/
uint8_t * process_read_request(rd_req_t *p_rd_req, uint8_t *p_data_buf, uint8_t toggle_value, uint8_t data_id)
{
    uint8_t     i,j;
    uint8_t     method=p_rd_req->hdr.method;
    mon_btn_t   *p_btn_data;
    uint16_t    *p_sldr_whl_data;
    btn_ctrl_t  *p_btn_ctrl;
    sldr_ctrl_t *p_sldr_ctrl;
    wheel_ctrl_t    *p_wheel_ctrl;
    mon_reply_hdr_t *p_hdr=(mon_reply_hdr_t *)p_data_buf;
    mon_trailer_t   *p_trailer;


    /* write header */
    p_hdr->s_id = data_id;
    p_hdr->format = 0;
    p_data_buf += sizeof(mon_reply_hdr_t);


    /* write button data (if any) */
    if ((p_rd_req->hdr.data_type == MON_DATA_BUTTONS) || (p_rd_req->hdr.data_type == MON_DATA_BTNS_AND_SLDRS))
    {
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
                if (g_key_info[method].mode == TOUCH_SELF_MODE)
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
    if ((p_rd_req->hdr.data_type == MON_DATA_SLIDERS) || (p_rd_req->hdr.data_type == MON_DATA_BTNS_AND_SLDRS))
    {
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
    else if (p_rd_req->hdr.data_type == MON_DATA_ACTIVE_METHODS)
    {
        *p_data_buf++ = g_mlist.num_methods;
        for (i=0; i < g_mlist.num_methods; i++)
        {
            *p_data_buf++ = g_mlist.methods[i];
        }
    }


    /* write trailer */
    p_trailer = (mon_trailer_t *)p_data_buf;
    p_trailer->part2_toggle = toggle_value;     // field only valid when a part-2 reply
    p_trailer->e_id = data_id;
    p_data_buf += sizeof(mon_trailer_t);

    return p_data_buf;
}

#endif // (TOUCH_CFG_UPDATE_MONITOR == 1)
