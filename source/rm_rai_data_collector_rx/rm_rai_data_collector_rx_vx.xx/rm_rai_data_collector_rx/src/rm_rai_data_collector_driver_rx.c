/***********************************************************************************************************************
 * Copyright [2023] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "rm_rai_data_collector.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
void rm_rai_data_collector_prepare_next_buffer (rai_data_collector_instance_ctrl_t * const p_ctrl, int32_t channel);

fsp_err_t rm_rai_data_collector_timer_open       (rai_data_collector_ctrl_t          * const p_api_ctrl);
fsp_err_t rm_rai_data_collector_timer_close      (rai_data_collector_ctrl_t          * const p_api_ctrl);
fsp_err_t rm_rai_data_collector_timer_start      (rai_data_collector_ctrl_t          * const p_api_ctrl);
fsp_err_t rm_rai_data_collector_timer_stop       (rai_data_collector_ctrl_t          * const p_api_ctrl);
fsp_err_t rm_rai_data_collector_dtc_open         (rai_data_collector_ctrl_t          * const p_api_ctrl);
fsp_err_t rm_rai_data_collector_dtc_close        (rai_data_collector_ctrl_t          * const p_api_ctrl);
fsp_err_t rm_rai_data_collector_dtc_enable       (rai_data_collector_ctrl_t          * const p_api_ctrl);
fsp_err_t rm_rai_data_collector_dtc_disable      (rai_data_collector_ctrl_t          * const p_api_ctrl);
fsp_err_t rm_rai_data_collector_dtc_start        (rai_data_collector_ctrl_t          * const p_api_ctrl);
fsp_err_t rm_rai_data_collector_dtc_stop         (rai_data_collector_ctrl_t          * const p_api_ctrl);
fsp_err_t rm_rai_data_collector_dtc_reconfigure  (rai_data_collector_ctrl_t          * const p_api_ctrl, 
                                                  transfer_info_t                    * const p_info);
fsp_err_t rm_rai_data_collector_dtc_src_set      (rai_data_collector_ctrl_t          * const p_api_ctrl, 
                                                  int32_t                                    channel,
                                                  void const                         *       p_src);
void rm_rai_data_collector_dtc_update_info       (rai_data_collector_instance_ctrl_t * const p_ctrl, uint8_t channel);
void rm_rai_data_collector_channel_ready_mask_set(rai_data_collector_ctrl_t          * const p_api_ctrl);

/*******************************************************************************************************************//**
 * Open timer module.
 *
 * @retval FSP_SUCCESS                  Data Collector successfully configured.
 * @retval FSP_ERR_ASSERTION            One or more pointers point to NULL or callback is NULL.
 ***********************************************************************************************************************/
fsp_err_t rm_rai_data_collector_timer_open(rai_data_collector_ctrl_t * const p_api_ctrl)
{
    bool ret = false;
    rai_data_collector_instance_ctrl_t * p_ctrl = (rai_data_collector_instance_ctrl_t *) p_api_ctrl;

    /* Open timer */
    ret = R_CMT_CreatePeriodicAssignChannelPriority(p_ctrl->p_cfg->p_snapshot_cfg->p_timer->freqency_hz,
                                                    p_ctrl->p_cfg->p_snapshot_cfg->p_timer->callback,
                                                    p_ctrl->p_cfg->p_snapshot_cfg->p_timer->channel,
                                                    p_ctrl->p_cfg->p_snapshot_cfg->p_timer->priority);
    FSP_ERROR_RETURN(true == ret, FSP_ERR_ASSERTION);

    /* Pause the timer */
    ret = R_CMT_Control(p_ctrl->p_cfg->p_snapshot_cfg->p_timer->channel,
                        CMT_RX_CMD_PAUSE, 
                        NULL);
    FSP_ERROR_RETURN(true == ret, FSP_ERR_ASSERTION);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Close timer module.
 *
 * @retval FSP_SUCCESS                  Data Collector successfully configured.
 * @retval FSP_ERR_ASSERTION            One or more pointers point to NULL or callback is NULL.
 ***********************************************************************************************************************/
fsp_err_t rm_rai_data_collector_timer_close(rai_data_collector_ctrl_t * const p_api_ctrl)
{
    bool ret = false;
    rai_data_collector_instance_ctrl_t * p_ctrl = (rai_data_collector_instance_ctrl_t *) p_api_ctrl;

    /* Close timer */
    ret = R_CMT_Stop(p_ctrl->p_cfg->p_snapshot_cfg->p_timer->channel);
    FSP_ERROR_RETURN(true == ret, FSP_ERR_ASSERTION);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Start timer module.
 *
 * @retval FSP_SUCCESS                  Data Collector successfully configured.
 * @retval FSP_ERR_ASSERTION            One or more pointers point to NULL or callback is NULL.
 ***********************************************************************************************************************/
fsp_err_t rm_rai_data_collector_timer_start(rai_data_collector_ctrl_t * const p_api_ctrl)
{
    bool ret = false;
    rai_data_collector_instance_ctrl_t * p_ctrl = (rai_data_collector_instance_ctrl_t *) p_api_ctrl;

    /* Start timer */
    ret = R_CMT_Control(p_ctrl->p_cfg->p_snapshot_cfg->p_timer->channel,
                        CMT_RX_CMD_RESTART, 
                        NULL);
    FSP_ERROR_RETURN(true == ret, FSP_ERR_ASSERTION);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Stop timer module.
 *
 * @retval FSP_SUCCESS                  Data Collector successfully configured.
 * @retval FSP_ERR_ASSERTION            One or more pointers point to NULL or callback is NULL.
 ***********************************************************************************************************************/
fsp_err_t rm_rai_data_collector_timer_stop(rai_data_collector_ctrl_t * const p_api_ctrl)
{
    bool ret = false;
    rai_data_collector_instance_ctrl_t * p_ctrl = (rai_data_collector_instance_ctrl_t *) p_api_ctrl;

    /* Pause timer */
    ret = R_CMT_Control(p_ctrl->p_cfg->p_snapshot_cfg->p_timer->channel,
                        CMT_RX_CMD_PAUSE, 
                        NULL);
    FSP_ERROR_RETURN(true == ret, FSP_ERR_ASSERTION);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Open DTC module.
 *
 * @retval FSP_SUCCESS                  Data Collector successfully configured.
 ***********************************************************************************************************************/
fsp_err_t rm_rai_data_collector_dtc_open(rai_data_collector_ctrl_t * const p_api_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;
    dtc_err_t ret = DTC_SUCCESS;
    rai_data_collector_instance_ctrl_t * p_ctrl = (rai_data_collector_instance_ctrl_t *) p_api_ctrl;
    transfer_instance_t * p_transfer;
	uint8_t   ien;


    /* Open DTC module */
    ret = R_DTC_Open();
    switch (ret)
    {
        case DTC_SUCCESS:
            err = FSP_SUCCESS;
            break;
        case DTC_ERR_OPENED:
            err = FSP_ERR_ALREADY_OPEN;
            break;
        case DTC_ERR_BUSY:
            err = FSP_ERR_IN_USE;
            break;        
        default:
            err = FSP_ERR_ASSERTION;
            break;
    }
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);


	ien = RM_RAI_DATA_COLLECTOR_CFG_DC0_TIMER_IEN;
	RM_RAI_DATA_COLLECTOR_CFG_DC0_TIMER_IEN = 0;

    /* Configure DTC module */
    p_transfer = (transfer_instance_t *)p_ctrl->p_cfg->p_snapshot_cfg->p_transfer;
    p_transfer->p_data_cfg = p_ctrl->p_extend->p_transfer_info;

    ret = R_DTC_Create(p_ctrl->p_cfg->p_snapshot_cfg->p_transfer->act_source,
                       p_ctrl->p_cfg->p_snapshot_cfg->p_transfer->p_transfer_data,
                       p_ctrl->p_cfg->p_snapshot_cfg->p_transfer->p_data_cfg,
                       p_ctrl->p_cfg->p_snapshot_cfg->p_transfer->chain_transfer_nr - 1);

	RM_RAI_DATA_COLLECTOR_CFG_DC0_TIMER_IEN = ien;

    switch (ret)
    {
        case DTC_SUCCESS:
            err = FSP_SUCCESS;
            break;
        case DTC_ERR_NOT_OPEN:
            err = FSP_ERR_NOT_OPEN;
            break;
        case DTC_ERR_INVALID_ARG:
            err = FSP_ERR_INVALID_ARGUMENT;
            break;
        case DTC_ERR_NULL_PTR:
            err = FSP_ERR_INVALID_POINTER;
            break;
        default:
            err = FSP_ERR_ASSERTION;
            break;
    }
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Close DTC module.
 *
 * @retval FSP_SUCCESS                  Data Collector successfully configured.
 ***********************************************************************************************************************/
fsp_err_t rm_rai_data_collector_dtc_close(rai_data_collector_ctrl_t * const p_api_ctrl)
{
    FSP_PARAMETER_NOT_USED(p_api_ctrl);

    /* Close DTC module */
    R_DTC_Close();

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Enable DTC module.
 *
 * @retval FSP_SUCCESS                  Data Collector successfully configured.
 ***********************************************************************************************************************/
fsp_err_t rm_rai_data_collector_dtc_enable(rai_data_collector_ctrl_t * const p_api_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;
    dtc_err_t ret = DTC_SUCCESS;
    rai_data_collector_instance_ctrl_t * p_ctrl = (rai_data_collector_instance_ctrl_t *) p_api_ctrl;
    dtc_cmd_arg_t args;

    /* Enable DTC module */
    args.act_src = p_ctrl->p_cfg->p_snapshot_cfg->p_transfer->act_source;
    ret = R_DTC_Control(DTC_CMD_ACT_SRC_ENABLE, NULL, &args);
    switch (ret)
    {
        case DTC_SUCCESS:
            err = FSP_SUCCESS;
            break;
        case DTC_ERR_NOT_OPEN:
            err = FSP_ERR_NOT_OPEN;
            break;
        case DTC_ERR_INVALID_COMMAND:
            err = FSP_ERR_INVALID_ARGUMENT;
            break;
        case DTC_ERR_NULL_PTR:
            err = FSP_ERR_INVALID_POINTER;
            break;
        case DTC_ERR_ACT:
            err = FSP_ERR_IN_USE;
            break; 
        default:
            err = FSP_ERR_ASSERTION;
            break;
    }
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Disable DTC module.
 *
 * @retval FSP_SUCCESS                  Data Collector successfully configured.
 ***********************************************************************************************************************/
fsp_err_t rm_rai_data_collector_dtc_disable(rai_data_collector_ctrl_t * const p_api_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;
    dtc_err_t ret = DTC_SUCCESS;
    rai_data_collector_instance_ctrl_t * p_ctrl = (rai_data_collector_instance_ctrl_t *) p_api_ctrl;
    dtc_cmd_arg_t args;

    /* Disable DTC module */
    args.act_src = p_ctrl->p_cfg->p_snapshot_cfg->p_transfer->act_source;
    ret = R_DTC_Control(DTC_CMD_ACT_SRC_DISABLE, NULL, &args);
    switch (ret)
    {
        case DTC_SUCCESS:
            err = FSP_SUCCESS;
            break;
        case DTC_ERR_NOT_OPEN:
            err = FSP_ERR_NOT_OPEN;
            break;
        case DTC_ERR_INVALID_COMMAND:
            err = FSP_ERR_INVALID_ARGUMENT;
            break;
        case DTC_ERR_NULL_PTR:
            err = FSP_ERR_INVALID_POINTER;
            break;
        case DTC_ERR_ACT:
            err = FSP_ERR_IN_USE;
            break; 
        default:
            err = FSP_ERR_ASSERTION;
            break;
    }
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Start DTC module.
 *
 * @retval FSP_SUCCESS                  Data Collector successfully configured.
 ***********************************************************************************************************************/
fsp_err_t rm_rai_data_collector_dtc_start(rai_data_collector_ctrl_t * const p_api_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;
    dtc_err_t ret = DTC_SUCCESS;

    /* Start DTC module */
    ret = R_DTC_Control(DTC_CMD_DTC_START, NULL, NULL);
    switch (ret)
    {
        case DTC_SUCCESS:
            err = FSP_SUCCESS;
            break;
        case DTC_ERR_NOT_OPEN:
            err = FSP_ERR_NOT_OPEN;
            break;
        case DTC_ERR_INVALID_COMMAND:
            err = FSP_ERR_INVALID_ARGUMENT;
            break;
        case DTC_ERR_NULL_PTR:
            err = FSP_ERR_INVALID_POINTER;
            break;
        case DTC_ERR_ACT:
            err = FSP_ERR_IN_USE;
            break; 
        default:
            err = FSP_ERR_ASSERTION;
            break;
    }
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Stop DTC module.
 *
 * @retval FSP_SUCCESS                  Data Collector successfully configured.
 ***********************************************************************************************************************/
fsp_err_t rm_rai_data_collector_dtc_stop(rai_data_collector_ctrl_t * const p_api_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;
    dtc_err_t ret = DTC_SUCCESS;

    /* Stop DTC module */
    ret = R_DTC_Control(DTC_CMD_DTC_STOP, NULL, NULL);
    switch (ret)
    {
        case DTC_SUCCESS:
            err = FSP_SUCCESS;
            break;
        case DTC_ERR_NOT_OPEN:
            err = FSP_ERR_NOT_OPEN;
            break;
        case DTC_ERR_INVALID_COMMAND:
            err = FSP_ERR_INVALID_ARGUMENT;
            break;
        case DTC_ERR_NULL_PTR:
            err = FSP_ERR_INVALID_POINTER;
            break;
        case DTC_ERR_ACT:
            err = FSP_ERR_IN_USE;
            break; 
        default:
            err = FSP_ERR_ASSERTION;
            break;
    }
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Reconfigure DTC module.
 *
 * @retval FSP_SUCCESS                  Data Collector successfully configured.
 ***********************************************************************************************************************/
fsp_err_t rm_rai_data_collector_dtc_reconfigure(rai_data_collector_ctrl_t * const p_api_ctrl, transfer_info_t * const p_info)
{
    fsp_err_t err = FSP_SUCCESS;
    dtc_err_t ret = DTC_SUCCESS;
    rai_data_collector_instance_ctrl_t * p_ctrl = (rai_data_collector_instance_ctrl_t *) p_api_ctrl;
    transfer_instance_t * p_transfer;
	uint8_t   ien;

	ien = RM_RAI_DATA_COLLECTOR_CFG_DC0_TIMER_IEN;
	RM_RAI_DATA_COLLECTOR_CFG_DC0_TIMER_IEN = 0;

	/* Reconfigure DTC module */
    p_transfer = (transfer_instance_t *)p_ctrl->p_cfg->p_snapshot_cfg->p_transfer;
    p_transfer->p_data_cfg = p_info;
    ret = R_DTC_Create(p_ctrl->p_cfg->p_snapshot_cfg->p_transfer->act_source,
                       p_ctrl->p_cfg->p_snapshot_cfg->p_transfer->p_transfer_data,
                       p_ctrl->p_cfg->p_snapshot_cfg->p_transfer->p_data_cfg,
                       p_ctrl->p_cfg->p_snapshot_cfg->p_transfer->chain_transfer_nr - 1);

    RM_RAI_DATA_COLLECTOR_CFG_DC0_TIMER_IEN = ien;

	switch (ret)
    {
        case DTC_SUCCESS:
            err = FSP_SUCCESS;
            break;
        case DTC_ERR_NOT_OPEN:
            err = FSP_ERR_NOT_OPEN;
            break;
        case DTC_ERR_INVALID_ARG:
            err = FSP_ERR_INVALID_ARGUMENT;
            break;
        case DTC_ERR_NULL_PTR:
            err = FSP_ERR_INVALID_POINTER;
            break;
        default:
            err = FSP_ERR_ASSERTION;
            break;
    }
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Set pointer to src.
 *
 * @retval FSP_SUCCESS                  Data Collector successfully configured.
 ***********************************************************************************************************************/
fsp_err_t rm_rai_data_collector_dtc_src_set(rai_data_collector_ctrl_t * const p_api_ctrl,
                                            int32_t                           channel,
                                            void const                *       p_src)
{
	rai_data_collector_instance_ctrl_t * p_ctrl = (rai_data_collector_instance_ctrl_t *) p_api_ctrl;

    p_ctrl->p_extend->p_transfer_info[channel].source_addr = (uint32_t)p_src;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Update info for DTC.
 ***********************************************************************************************************************/
void rm_rai_data_collector_dtc_update_info (rai_data_collector_instance_ctrl_t * const p_ctrl, uint8_t channel)
{
    /* Snapshot mode channels shall have the same accumulated length and idx */
    transfer_info_t * p_info = p_ctrl->p_extend->p_transfer_info;

    rm_rai_data_collector_prepare_next_buffer(p_ctrl, channel);

    uint32_t offset = p_ctrl->p_extend->p_ping_pong_buf_hnd[channel].buf_idx * p_ctrl->p_cfg->required_frame_len;
    for (uint8_t i = 0; i < p_ctrl->p_cfg->p_snapshot_cfg->channels; i++)
    {
        if (p_ctrl->p_extend->p_ping_pong_buf[i + p_ctrl->p_cfg->p_data_feed_cfg->channels].data_type !=
            RAI_DATA_COLLECTOR_DATA_TYPE_DOUBLE)
        {
            p_info[i].transfer_count = (uint32_t) p_ctrl->p_cfg->p_snapshot_cfg->transfer_len;
            p_info[i].dest_addr =
                (uint32_t)((uint8_t *) p_ctrl->p_extend->p_ping_pong_buf[i + p_ctrl->p_cfg->p_data_feed_cfg->channels].p_buf +
                ((offset + p_ctrl->p_extend->p_ping_pong_buf_hnd[channel].accumulated_len) <<
                    (p_info[i].data_size >> 4)));
        }
        else
        {
            /* The unit transfer size of DTC is 1 byte, 2bytes or 4 bytes. The transfer len for type "double" needs to be doubled
             * when the unit transfer size is initialized to 4 Bytes.
             */
            p_info[i].transfer_count = (uint32_t) (p_ctrl->p_cfg->p_snapshot_cfg->transfer_len << 1);
            p_info[i].dest_addr =
                (uint32_t)((uint8_t *) p_ctrl->p_extend->p_ping_pong_buf[i + p_ctrl->p_cfg->p_data_feed_cfg->channels].p_buf +
                ((offset + p_ctrl->p_extend->p_ping_pong_buf_hnd[channel].accumulated_len) << 3));
        }

        p_info[i].block_size = 1;
    }

    p_ctrl->p_extend->p_ping_pong_buf_hnd[channel].accumulated_len += p_ctrl->p_cfg->p_snapshot_cfg->transfer_len;

    /* Reconfigure */
    rm_rai_data_collector_dtc_reconfigure(p_ctrl, p_info);
}

/*******************************************************************************************************************//**
 * Set channel ready mask.
 ***********************************************************************************************************************/
void rm_rai_data_collector_channel_ready_mask_set(rai_data_collector_ctrl_t * const p_api_ctrl)
{
    rai_data_collector_instance_ctrl_t * p_ctrl = (rai_data_collector_instance_ctrl_t *) p_api_ctrl;
    rai_data_collector_cfg_t           * p_cfg = (rai_data_collector_cfg_t *)p_ctrl->p_cfg;
    uint32_t i;
    uint32_t channels = p_cfg->p_data_feed_cfg->channels + (p_cfg->p_snapshot_cfg->channels > 0 ? 1 : 0);

    for (i = 0; i < channels; i++)
    {
        p_cfg->channel_ready_mask |= (1 << i);
    }
}

/*******************************************************************************************************************//**
 * Callback function.
 ***********************************************************************************************************************/
void rm_rai_data_collector_dtc_end_cb (rai_data_collector_ctrl_t const * p_api_ctrl)
{
    rai_data_collector_instance_ctrl_t * p_ctrl = (rai_data_collector_instance_ctrl_t *) p_api_ctrl;

    rm_rai_data_collector_dtc_update_info(p_ctrl, p_ctrl->p_cfg->p_data_feed_cfg->channels);
    rm_rai_data_collector_dtc_enable(p_ctrl);
}
