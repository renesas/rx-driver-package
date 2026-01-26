/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/**********************************************************************************************************************
* File Name    : r_fwupcomm_api.c
* Description  : API source file for the fwupcomm module.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           20.05.2025 1.00    Initial Release
*           11.11.2025 1.10    Added SPI communication support.
***********************************************************************************************************************/


#include <string.h>

#include "r_fwupcomm_if.h"
#include "r_fwupcomm_private.h"


#define FWUPCOMM_SEND_PACKET_BUFFER_SIZE    (FWUPCOMM_CFG_SEND_PACKET_BUFFER_SIZE)

#if (FWUPCOMM_CFG_DEVICE_PRIMARY == 1)  /* Primary */
#if (FWUPCOMM_COMM_IF  == FWUPCOMM_COMM_IF_SPI)  /* SPI I/F */
/* For Primary MCU using SPI, a size of a packet recv buffer is the same as the size of packet send buffer, due to full-duplex. */
#define FWUPCOMM_RECV_PACKET_BUFFER_SIZE    (FWUPCOMM_CFG_RECV_PACKET_BUFFER_SIZE)
#else /* UART I/F, etc. */
/* For primary MCU, a size of a packet recv buffer is needed only a size of the command header. */
#define FWUPCOMM_RECV_PACKET_BUFFER_SIZE    (FWUPCOMM_CMD_HEADER_SIZE)
#endif  /* SPI I/F / UART I/F */

#else  /* Secondary */
/* For secondary MCU, a size of a packet recv buffer is needed a size of the command header and command data. */
#define FWUPCOMM_RECV_PACKET_BUFFER_SIZE    (FWUPCOMM_CFG_RECV_PACKET_BUFFER_SIZE)
#endif  /* Primary / Secondary */


/* Buffer size of the send packet */
static uint32_t s_send_packet_buffer[FWUPCOMM_CFG_SEND_PACKET_BUFFER_SIZE / 4];

/* Buffer size of the receive packet */
static uint32_t s_recv_packet_buffer[FWUPCOMM_CFG_RECV_PACKET_BUFFER_SIZE / 4];


static void r_fwupcomm_resp_timeout_timer_cb (r_fwupcomm_hdl_t * hdl);

static uint8_t r_fwupcomm_serialize_cmd_info (r_fwupcomm_packet_type_t type,
                                            uint8_t class,
                                            uint8_t id);

static uint8_t r_fwupcomm_serialize_cmd_option (r_fwupcomm_cmd_option_t option);

static fwupcomm_err_t r_fwupcomm_ch_send_with_timeout (r_fwupcomm_hdl_t * hdl,
                                                        uint32_t timeout_ms);

#if (FWUPCOMM_CFG_DEVICE_PRIMARY == 1)
static fwupcomm_err_t r_fwupcomm_cmd_send_internal (r_fwupcomm_hdl_t * hdl,
                                                    r_fwupcomm_cmd_instr_t * cmd_instr);

static fwupcomm_err_t r_fwupcomm_generate_cmd (r_fwupcomm_hdl_t * hdl,
                                                r_fwupcomm_cmd_info_t * cmd,
                                                uint8_t cmd_id);

static fwupcomm_err_t r_fwupcomm_parse_response_header (r_fwupcomm_hdl_t * hdl,
                                                        r_fwupcomm_cmd_info_t * cmd,
                                                        r_fwupcomm_resp_info_t * resp,
                                                        uint8_t cmd_id);

static fwupcomm_err_t r_fwupcomm_receive_response (r_fwupcomm_hdl_t * hdl,
                                                    r_fwupcomm_cmd_instr_t * cmd_instr,
                                                    uint8_t cmd_id);

#if (BSP_CFG_RTOS_USED == 1)  /* FreeRTOS is used. */
static void r_fwupcomm_process_cmd_task (void * pvParameters);
#endif /* (BSP_CFG_RTOS_USED == 1) */

#elif (FWUPCOMM_CFG_DEVICE_PRIMARY == 0)
static fwupcomm_err_t r_fwupcomm_parse_cmd_header (r_fwupcomm_hdl_t * hdl,
                                                    r_fwupcomm_cmd_info_t * cmd,
                                                    uint8_t * cmd_id);

static fwupcomm_err_t r_fwupcomm_receive_cmd (r_fwupcomm_hdl_t * hdl,
                                                r_fwupcomm_cmd_info_t * cmd,
                                                uint8_t * cmd_id);

static fwupcomm_err_t r_fwupcomm_handle_incoming_cmd (r_fwupcomm_hdl_t * hdl,
                                                        r_fwupcomm_cmd_info_t * cmd,
                                                        r_fwupcomm_resp_info_t * resp);

static fwupcomm_err_t r_fwupcomm_generate_resp (r_fwupcomm_hdl_t * hdl,
                                                r_fwupcomm_cmd_info_t * cmd,
                                                r_fwupcomm_resp_info_t * resp,
                                                uint8_t cmd_id);

static fwupcomm_err_t r_fwupcomm_generate_err_resp (r_fwupcomm_hdl_t * hdl,
                                                    r_fwupcomm_cmd_info_t * cmd);

#endif


/**********************************************************************************************************************
 * Function Name: R_FWUPCOMM_Open
 * Description  : Opens a communications channel for use by or within this module. This function must be executed 
 *                before other API functions are used.
 * Arguments    : hdl: Handler of the module
 *              : cfg: Structure variable with information required for initializing modules
 * Return Value : FWUPCOMM_SUCCESS            : The channel was successfully initialized.
 *              : FWUPCOMM_ERR_INVALID_PTR    : The pointer passed as an argument was NULL.
 *              : FWUPCOMM_ERR_ALREADY_OPEN   : The module has already been initialized.
 *              : FWUPCOMM_ERR_CH_ALREADY_OPEN: The communications channel has already been opened.
 *              : FWUPCOMM_ERR_NOT_OPEN       : Initializing the communication channel failed.
 *********************************************************************************************************************/
fwupcomm_err_t R_FWUPCOMM_Open(r_fwupcomm_hdl_t * hdl, void * cfg)
{
    fwupcomm_err_t     err = FWUPCOMM_SUCCESS;
    r_fwupcomm_cfg_t * p_cfg;

#if FWUPCOMM_CFG_PARAM_CHECKING_ENABLE
    if ((NULL == hdl) || (NULL == cfg))
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }
#endif

    /* Check module open flag is false. */
    if (true == hdl->open_flag)
    {
        return FWUPCOMM_ERR_ALREADY_OPEN;
    }

    p_cfg = (r_fwupcomm_cfg_t *)cfg;  /* Casting to r_fwupcomm_cfg_t * type is valid */

    if ((NULL == p_cfg->timer.start) || (NULL == p_cfg->timer.stop))
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }

    /* Init module handler. */
    memset(hdl, 0x00, sizeof(r_fwupcomm_hdl_t));

    /* Set and init Timer interface. */
    hdl->timer.start        = p_cfg->timer.start;
    hdl->timer.stop         = p_cfg->timer.stop;
    hdl->timer_timeout_flag = false;

    /* Init packet buffers. */
    hdl->send_buf.data = (uint8_t *)s_send_packet_buffer;
    hdl->send_buf.size = 0;
    hdl->recv_buf.data = (uint8_t *)s_recv_packet_buffer;
    hdl->recv_buf.size = 0;

    memset((void *)hdl->send_buf.data, 0x00, FWUPCOMM_SEND_PACKET_BUFFER_SIZE);
    memset((void *)hdl->recv_buf.data, 0x00, FWUPCOMM_RECV_PACKET_BUFFER_SIZE);

#if (BSP_CFG_RTOS_USED == 1) && (FWUPCOMM_CFG_DEVICE_PRIMARY == 1)  /* Primary MCU and FreeRTOS is used. */
    /* Create queue for exchange of command instruction info. */
    hdl->cmd_queue = xQueueCreate(FWUPCOMM_CMD_QUEUE_SIZE, sizeof(r_fwupcomm_cmd_instr_t *));

    if (NULL == hdl->cmd_queue)
    {
        /* Init module handler. */
        memset(hdl, 0x00, sizeof(r_fwupcomm_hdl_t));

        return FWUPCOMM_ERR_NOT_OPEN;
    }

    /* Create task to process command communication. */
    BaseType_t ret = xTaskCreate(r_fwupcomm_process_cmd_task,
                                    "FWUPCOMM",
                                    (FWUPCOMM_PROCESS_CMD_TASK_STACK_SIZE * 5),
                                    hdl,
                                    FWUPCOMM_PROCESS_CMD_TASK_PRIORITY,
                                    &hdl->task_hdl);

    if (pdPASS != ret)
    {
        /* Delete command queue. */
        vQueueDelete(hdl->cmd_queue);

        /* Init module handler. */
        memset(hdl, 0x00, sizeof(r_fwupcomm_hdl_t));

        return FWUPCOMM_ERR_NOT_OPEN;
    }
#endif /* (BSP_CFG_RTOS_USED == 1) && (FWUPCOMM_CFG_DEVICE_PRIMARY == 1) */

    /* Open communication interface. */
    err = r_fwupcomm_ch_api.open(hdl);

    if (FWUPCOMM_SUCCESS == err)
    {
        /* Raise module open flag. */
        hdl->open_flag = true;

        return FWUPCOMM_SUCCESS;
    }
    else
    {
#if (BSP_CFG_RTOS_USED == 1) && (FWUPCOMM_CFG_DEVICE_PRIMARY == 1)  /* Primary MCU and FreeRTOS is used. */
        /* Delete process command task. */
        vTaskDelete(hdl->task_hdl);

        /* Delete command queue. */
        vQueueDelete(hdl->cmd_queue);
#endif /* (BSP_CFG_RTOS_USED == 1) && (FWUPCOMM_CFG_DEVICE_PRIMARY == 1) */

        /* Init module handler. */
        memset(hdl, 0x00, sizeof(r_fwupcomm_hdl_t));

        if (FWUPCOMM_ERR_CH_ALREADY_OPEN == err)
        {
            return FWUPCOMM_ERR_CH_ALREADY_OPEN;
        }
        else
        {
            return FWUPCOMM_ERR_NOT_OPEN;
        }
    }
}
/**********************************************************************************************************************
 End of function R_FWUPCOMM_Open
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: R_FWUPCOMM_Close
 * Description  : Closes a communications channel for use by or within this module.
 * Argument     : hdl: Handler of the module
 * Return Value : FWUPCOMM_SUCCESS            : Closing was successful.
 *              : FWUPCOMM_ERR_NOT_OPEN       : The module has not been opened.
 *              : FWUPCOMM_ERR_INVALID_PTR    : The pointer passed as an argument is NULL.
 *********************************************************************************************************************/
fwupcomm_err_t R_FWUPCOMM_Close(r_fwupcomm_hdl_t * hdl)
{
#if FWUPCOMM_CFG_PARAM_CHECKING_ENABLE
    if (NULL == hdl)
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }
#endif

    /* Check module open flag is true. */
    if (false == hdl->open_flag)
    {
        return FWUPCOMM_ERR_NOT_OPEN;
    }

    /* Close communication interface. */
    r_fwupcomm_ch_api.close(hdl);

#if (BSP_CFG_RTOS_USED == 1) && (FWUPCOMM_CFG_DEVICE_PRIMARY == 1)  /* Primary MCU and FreeRTOS is used. */
    /* Delete process command task. */
    vTaskDelete(hdl->task_hdl);

    /* Delete command queue. */
    vQueueDelete(hdl->cmd_queue);
#endif /* (BSP_CFG_RTOS_USED == 1) && (FWUPCOMM_CFG_DEVICE_PRIMARY == 1) */

    memset(hdl, 0x00, sizeof(r_fwupcomm_hdl_t));

    return FWUPCOMM_SUCCESS;
}
/**********************************************************************************************************************
 End of function R_FWUPCOMM_Close
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_resp_timeout_timer_cb
 * Description  : Callback function for the timer.
 * Argument     : hdl: Handler of the module
 *********************************************************************************************************************/
static void r_fwupcomm_resp_timeout_timer_cb(r_fwupcomm_hdl_t * hdl)
{
    hdl->timer_timeout_flag = true;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_resp_timeout_timer_cb
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_ch_send_with_timeout
 * Description  : Sends a packet with timeout.
 * Arguments    : hdl       : Handler of the module
 *              : timeout_ms: Timeout time for sending packet
 *********************************************************************************************************************/
static fwupcomm_err_t r_fwupcomm_ch_send_with_timeout(r_fwupcomm_hdl_t * hdl,
                                                        uint32_t timeout_ms)
{
    fwupcomm_err_t err = FWUPCOMM_SUCCESS;

#if FWUPCOMM_CFG_PARAM_CHECKING_ENABLE
    if (NULL == hdl)
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }
#endif

    /* Initialize timer timeout flag and start timer. */
    hdl->timer_timeout_flag = false;
    hdl->timer.start(hdl, timeout_ms, r_fwupcomm_resp_timeout_timer_cb);

    /* WAIT_LOOP */
    do
    {
        /* Continue to execute the send function of communication interface
         * until the function returns error or the process times out. */
        err = r_fwupcomm_ch_api.send(hdl, hdl->send_buf.data, hdl->send_buf.size);
    } while ((FWUPCOMM_ERR_CH_SEND_BUSY == err) && (false == hdl->timer_timeout_flag));

    /* Stop timeout timer. */
    hdl->timer.stop();
    hdl->timer_timeout_flag = false;

    if (FWUPCOMM_SUCCESS != err)
    {
        err = FWUPCOMM_ERR_CH_SEND;
    }

    return err;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_ch_send_with_timeout
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_serialize_cmd_info
 * Description  : Serializes command information into a single byte.
 * Arguments    : type : command type
 *              : class: command class
 *              : id   : command ID
 *********************************************************************************************************************/
static uint8_t r_fwupcomm_serialize_cmd_info(r_fwupcomm_packet_type_t type,
                                            uint8_t class,
                                            uint8_t id)
{
    return (uint8_t)((id & 0x0F)|(class << 4)|((uint8_t)type << 7));  /* Casting to uint8_t type is valid */
}
/**********************************************************************************************************************
 End of function r_fwupcomm_serialize_cmd_info
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_serialize_cmd_option
 * Description  : Serializes command option information into a single byte.
 * Argument     : option: command option
 *********************************************************************************************************************/
static uint8_t r_fwupcomm_serialize_cmd_option(r_fwupcomm_cmd_option_t option)
{
    return (uint8_t)(0x00|(option.no_resp << 7));  /* Casting to uint8_t type is valid */
}
/**********************************************************************************************************************
 End of function r_fwupcomm_serialize_cmd_option
 *********************************************************************************************************************/


#if (FWUPCOMM_CFG_DEVICE_PRIMARY == 1)
/**********************************************************************************************************************
 * Function Name: R_FWUPCOMM_CmdSend
 * Description  : Sends a command for the secondary MCU and receives a corresponding response.
 * Arguments    : hdl      : Handler of the module
 *              : cmd_instr: Structure variable with information on the command to be sent 
 *                           and the destination for storing the response
 * Return Value : FWUPCOMM_SUCCESS:               The command was successfully completed.
 *              : FWUPCOMM_ERR_NOT_OPEN:          The module has not been opened.
 *              : FWUPCOMM_ERR_INVALID_PTR:       The pointer passed as an argument was NULL.
 *              : FWUPCOMM_ERR_INVALID_ARG:       The parameter passed as an argument was invalid.
 *              : FWUPCOMM_ERR_CH_SEND:           Sending of data in the communications channel failed.
 *              : FWUPCOMM_ERR_CH_RECV:           Receiving of data from the communications channel failed.
 *              : FWUPCOMM_ERR_RECV_RESP_TIMEOUT: A timeout occurred before a response was received.
 *********************************************************************************************************************/
fwupcomm_err_t R_FWUPCOMM_CmdSend(r_fwupcomm_hdl_t * hdl, r_fwupcomm_cmd_instr_t * cmd_instr)
{
    fwupcomm_err_t err      = FWUPCOMM_SUCCESS;


#if FWUPCOMM_CFG_PARAM_CHECKING_ENABLE
    if ((NULL == hdl) || (NULL == cmd_instr))
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }
    if (cmd_instr->cmd.class >= FWUPCOMM_CMD_NUM_CLASS)
    {
        return FWUPCOMM_ERR_INVALID_ARG;
    }
#endif

#if (BSP_CFG_RTOS_USED == 0)  /* RTOS is not used. */
    err = r_fwupcomm_cmd_send_internal(hdl, cmd_instr);
#elif (BSP_CFG_RTOS_USED == 1)  /* FreeRTOS is used. */
    BaseType_t ret;
    uint32_t   notified_value;

    if (NULL == hdl->cmd_queue)
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }

    /* Send the command instruction info to the process command task. */
    ret = xQueueSendToBack(hdl->cmd_queue, &(cmd_instr), pdMS_TO_TICKS(cmd_instr->timeout_ms));

    if (pdTRUE == ret)
    {
        /* Wait for command communication to finish. */
        ret = xTaskNotifyWait(0, UINT32_MAX, &notified_value, portMAX_DELAY);

        if (pdTRUE == ret)
        {
            /* Receive the return value of r_fwupcomm_cmd_send_internal function. */
            err = (fwupcomm_err_t)notified_value;  /* Casting to fwupcomm_err_t type is valid */
        }
        else
        {
            /* Normally does not occur. */
            return FWUPCOMM_ERR_CH_SEND_BUSY;
        }
    }
    else
    {
        /* Command queue is full. */
        return FWUPCOMM_ERR_CH_SEND_BUSY;
    }

#endif /* (BSP_CFG_RTOS_USED == 1) */

    return err;
}
/**********************************************************************************************************************
 End of function R_FWUPCOMM_CmdSend
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_cmd_send_internal
 * Description  : Internal function to send command and receive response.
 * Arguments    : hdl      : Handler of the module
 *              : cmd_instr: Command instruction info
 *********************************************************************************************************************/
static fwupcomm_err_t r_fwupcomm_cmd_send_internal(r_fwupcomm_hdl_t * hdl, r_fwupcomm_cmd_instr_t * cmd_instr)
{
    static uint8_t s_cmd_id = 0;
    fwupcomm_err_t err      = FWUPCOMM_SUCCESS;

    /* UART: Flush channel Rx buffer first. */
    /* SPI:  Do nothing. */
    r_fwupcomm_ch_api.rx_reset(hdl);

    s_cmd_id          = (s_cmd_id + 1) & 0x0F;

    err = r_fwupcomm_generate_cmd(hdl, &cmd_instr->cmd, s_cmd_id);

    if (FWUPCOMM_SUCCESS == err)
    {
        err = r_fwupcomm_ch_send_with_timeout(hdl, FWUPCOMM_CFG_CMD_SEND_TIMEOUT);
    }

    if (FWUPCOMM_SUCCESS == err)
    {
        /* Send dummy data 0x00 and receive the response */
        err = r_fwupcomm_receive_response(hdl, cmd_instr, s_cmd_id);
    }

    /* Clear buffers. */
    memset((void *)hdl->send_buf.data, 0x00, FWUPCOMM_SEND_PACKET_BUFFER_SIZE);
    memset((void *)hdl->recv_buf.data, 0x00, FWUPCOMM_RECV_PACKET_BUFFER_SIZE);

    hdl->send_buf.size = 0;
    hdl->recv_buf.size = 0;

    /* UART: Flush channel Rx buffer at the end. */
    /* SPI:  Setup the reception and wait for the clock at the end . */
    r_fwupcomm_ch_api.rx_reset(hdl);

    return err;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_cmd_send_internal
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_generate_cmd
 * Description  : Generates a command packet.
 * Arguments    : hdl   : Handler of the module
 *              : cmd   : Command information
 *              : cmd_id: Command ID
 *********************************************************************************************************************/
static fwupcomm_err_t r_fwupcomm_generate_cmd(r_fwupcomm_hdl_t * hdl,
                                                r_fwupcomm_cmd_info_t * cmd,
                                                uint8_t cmd_id)
{
#if FWUPCOMM_CFG_PARAM_CHECKING_ENABLE
    if ((NULL == hdl) || (NULL == cmd))
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }
#endif

    if (cmd->class >= FWUPCOMM_CMD_NUM_CLASS)
    {
        return FWUPCOMM_ERR_INVALID_ARG;
    }

    const FWUPCOMM_FAR r_fwupcomm_cmd_table_t * p_cmd_table = r_fwupcomm_cmd_def_table_list[cmd->class].table;

    if (cmd->name >= r_fwupcomm_cmd_def_table_list[cmd->class].num_cmd)
    {
        return FWUPCOMM_ERR_INVALID_ARG;
    }

    /* Check command data size is not larger than defined size. */
    if (p_cmd_table[cmd->name].cmd_data_max_size < cmd->data_size)
    {
        return FWUPCOMM_ERR_INVALID_ARG;
    }

    hdl->send_buf.size = FWUPCOMM_CMD_HEADER_SIZE + cmd->data_size;

    hdl->send_buf.data[0] = cmd->device_address;
    hdl->send_buf.data[1] = (uint8_t)FWUPCOMM_CFG_CMD_VER;  /* Casting to uint8_t type is valid */
    hdl->send_buf.data[2] = r_fwupcomm_serialize_cmd_info(FWUPCOMM_PACKET_TYPE_CMD, cmd->class, cmd_id);
    hdl->send_buf.data[3] = r_fwupcomm_serialize_cmd_option(cmd->option);
    hdl->send_buf.data[4] = p_cmd_table[cmd->name].value;
    hdl->send_buf.data[5] = cmd->arg;
    hdl->send_buf.data[6] = UINT16_LSB(cmd->data_size);
    hdl->send_buf.data[7] = UINT16_MSB(cmd->data_size);

    if (0 != cmd->data_size)
    {
        if (NULL != cmd->data)
        {
            MEMCPY(&hdl->send_buf.data[FWUPCOMM_CMD_HEADER_CMD_DATA], cmd->data, cmd->data_size);
        }
        else
        {
            return FWUPCOMM_ERR_INVALID_PTR;
        }
    }

    return FWUPCOMM_SUCCESS;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_generate_cmd
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_parse_response_header
 * Description  : Parses the response header.
 * Arguments    : hdl   : Handler of the module
 *              : cmd   : Command information
 *              : resp  : Response information
 *              : cmd_id: Command ID
 *********************************************************************************************************************/
static fwupcomm_err_t r_fwupcomm_parse_response_header(r_fwupcomm_hdl_t * hdl,
                                                        r_fwupcomm_cmd_info_t * cmd,
                                                        r_fwupcomm_resp_info_t * resp,
                                                        uint8_t cmd_id)
{
    uint8_t cmd_info;

#if FWUPCOMM_CFG_PARAM_CHECKING_ENABLE
    if ((NULL == hdl) || (NULL == cmd) || (NULL == resp))
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }
#endif

    if (cmd->class >= FWUPCOMM_CMD_NUM_CLASS)
    {
        return FWUPCOMM_ERR_INVALID_ARG;
    }

    if (r_fwupcomm_cmd_def_table_list[cmd->class].num_cmd <= cmd->name)
    {
        return FWUPCOMM_ERR_INVALID_ARG;
    }

    const FWUPCOMM_FAR r_fwupcomm_cmd_table_t * p_cmd_table = r_fwupcomm_cmd_def_table_list[cmd->class].table;

    /* Analyze response header. */
    /* Check device address. */
    if (hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_DEVICE_ADDR] != cmd->device_address)
    {
        return FWUPCOMM_ERR_INVALID_RESP;
    }

    /* Check command version. */
    if (FWUPCOMM_CFG_CMD_VER != hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_CMD_VER])
    {
        return FWUPCOMM_ERR_INVALID_RESP;
    }

    cmd_info = r_fwupcomm_serialize_cmd_info(FWUPCOMM_PACKET_TYPE_RESP, cmd->class, cmd_id);

    /* Check command category and ID. */
    if (hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_CMD_INFO] != cmd_info)
    {
        return FWUPCOMM_ERR_INVALID_RESP;
    }

    /* Check command value. */
    if (hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_CMD] != p_cmd_table[cmd->name].value)
    {
        return FWUPCOMM_ERR_INVALID_RESP;
    }

    /* Store command result and response data size to resp info. */
    resp->result    = hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_CMD_ARG];
    resp->data_size = (uint16_t)(hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_CMD_DATA_LEN] +
                                        (hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_CMD_DATA_LEN + 1] << 8));

    /* Check response data size is not larger than defined size. */
    if (p_cmd_table[cmd->name].resp_data_max_size < resp->data_size)
    {
        return FWUPCOMM_ERR_INVALID_RESP;
    }

    return FWUPCOMM_SUCCESS;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_parse_response_header
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_receive_response
 * Description  : Receives a response packet.
 * Arguments    : hdl      : Handler of the module
 *              : cmd_instr: Command instruction info
 *              : cmd_id   : Command ID
 *********************************************************************************************************************/
static fwupcomm_err_t r_fwupcomm_receive_response(r_fwupcomm_hdl_t * hdl,
                                                    r_fwupcomm_cmd_instr_t * cmd_instr,
                                                    uint8_t cmd_id)
{
    fwupcomm_err_t           err = FWUPCOMM_SUCCESS;
    r_fwupcomm_cmd_info_t  * p_cmd;
    r_fwupcomm_resp_info_t * p_resp;

#if FWUPCOMM_CFG_PARAM_CHECKING_ENABLE
    if ((NULL == hdl) || (NULL == cmd_instr))
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }
#endif

    p_cmd  = &cmd_instr->cmd;
    p_resp = &cmd_instr->resp;

    /* Initialize timer timeout flag and start timer. */
    hdl->timer_timeout_flag = false;
    hdl->timer.start(hdl, cmd_instr->timeout_ms, r_fwupcomm_resp_timeout_timer_cb);

    /* Receive response header until request size is reached or timeout. */
    /* WAIT_LOOP */
    do
    {
        err = r_fwupcomm_ch_api.recv(hdl, hdl->recv_buf.data, FWUPCOMM_CMD_HEADER_SIZE);
    } while ((FWUPCOMM_ERR_CH_RECV_NO_DATA == err) && (false == hdl->timer_timeout_flag));

    if (FWUPCOMM_SUCCESS == err)
    {
        err = r_fwupcomm_parse_response_header(hdl, p_cmd, p_resp, cmd_id);
    }
    else if (FWUPCOMM_ERR_CH_RECV_NO_DATA == err)
    {
        err = FWUPCOMM_ERR_RECV_RESP_TIMEOUT;
    }
    else
    {
        err = FWUPCOMM_ERR_CH_RECV;
    }

    /* Response data reception if response header parsing is successful
     * and response data size is not 0. */
    if ((FWUPCOMM_SUCCESS == err) && (0 != p_resp->data_size))
    {
        if (NULL != p_resp->data)
        {
            /* Receive response data until request size is reached or timeout. */
            /* WAIT_LOOP */
            do
            {
                err = r_fwupcomm_ch_api.recv(hdl, p_resp->data, p_resp->data_size);
            } while ((FWUPCOMM_ERR_CH_RECV_NO_DATA == err) && (false == hdl->timer_timeout_flag));

            if (FWUPCOMM_SUCCESS != err)
            {
                err = FWUPCOMM_ERR_CH_RECV;
            }
        }
        else
        {
            err = FWUPCOMM_ERR_INVALID_PTR;
        }
    }

    /* Stop timeout timer. */
    hdl->timer.stop();
    hdl->timer_timeout_flag = false;

    return err;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_receive_response
 *********************************************************************************************************************/

#if (BSP_CFG_RTOS_USED == 1)  /* FreeRTOS is used. */
/**********************************************************************************************************************
 * Function Name: r_fwupcomm_process_cmd_task
 * Description  : Task function to process command communication.
 * Argument     : pvParameters: Handler of the fwupcomm module
 *********************************************************************************************************************/
static void r_fwupcomm_process_cmd_task(void * pvParameters)
{
    fwupcomm_err_t           err = FWUPCOMM_SUCCESS;
    r_fwupcomm_cmd_instr_t * p_cmd_instr;

    /* Casting to r_fwupcomm_hdl_t * type is valid */
    r_fwupcomm_hdl_t *       p_hdl = (r_fwupcomm_hdl_t *)pvParameters;

    while (1)
    {
        /* Receive command instruction info from tasks through command send API function
         * one at a time for exclusive control of command communication. */
        xQueueReceive(p_hdl->cmd_queue, &(p_cmd_instr), portMAX_DELAY);

        if (NULL != p_cmd_instr)
        {
            /* Send command and receive response. */
            err = r_fwupcomm_cmd_send_internal(p_hdl, p_cmd_instr);

            if (NULL != p_cmd_instr->notify_task)
            {
                /* Notify the caller of the command send API function. */
                xTaskNotify(p_cmd_instr->notify_task, (uint32_t)err, eSetValueWithOverwrite);
            }

            /* Communication interface error */
            if ((FWUPCOMM_ERR_CH_SEND == err) || (FWUPCOMM_ERR_CH_RECV == err))
            {
                /* Close communication interface. */
                r_fwupcomm_ch_api.close(p_hdl);

                /* Re-open communication interface. */
                r_fwupcomm_ch_api.open(p_hdl);
            }
        }
    }

    vTaskDelete(NULL);
}
/**********************************************************************************************************************
 End of function r_fwupcomm_process_cmd_task
 *********************************************************************************************************************/
#endif /* (BSP_CFG_RTOS_USED == 1) */

#elif (FWUPCOMM_CFG_DEVICE_PRIMARY == 0)


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_parse_cmd_header
 * Description  : Parses the command header.
 * Arguments    : hdl   : Handler of the module
 *              : cmd   : Command information
 *              : cmd_id: Command ID
 *********************************************************************************************************************/
static fwupcomm_err_t r_fwupcomm_parse_cmd_header(r_fwupcomm_hdl_t * hdl,
                                                    r_fwupcomm_cmd_info_t * cmd,
                                                    uint8_t * cmd_id)
{
    fwupcomm_err_t err = FWUPCOMM_SUCCESS;
    uint8_t        cmd_name_value;
    uint8_t        num_cmd;
    uint8_t        cmd_option_value;

#if FWUPCOMM_CFG_PARAM_CHECKING_ENABLE
    if (NULL == hdl)
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }
#endif

    /* Analyze command header. */
    /* Check device address. */
    if ((FWUPCOMM_CFG_DEVICE_ADDRESS != hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_DEVICE_ADDR])
         &&(FWUPCOMM_BROADCAST_DEVICE_ADDRESS != hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_DEVICE_ADDR]))
    {
        /* Not a command addressed to me. */
        return FWUPCOMM_ERR_NO_CMD;
    }

    /* Check command version. */
    if (FWUPCOMM_CFG_CMD_VER != hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_CMD_VER])
    {
        err = FWUPCOMM_ERR_INVALID_CMD;
    }

    /*  Format of the 3rd byte of the command header.  */
    /*  7 6 5 4 3 2 1 0                                  */
    /*  ^               : command request/response       */
    /*    ^ ^ ^         : command class                  */
    /*          ^ ^ ^ ^ : command ID                     */
    /* Check command is request. */
    if (((hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_CMD_INFO] >> 7) & 0x01) != (uint8_t)FWUPCOMM_PACKET_TYPE_CMD)
    {
        err = FWUPCOMM_ERR_INVALID_CMD;
    }

    cmd->device_address = hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_DEVICE_ADDR];
    cmd->class          = (hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_CMD_INFO] >> 4) & 0x07; /* b4-6 */
    *cmd_id             = hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_CMD_INFO] & 0x0F;        /* b0-3 */
    cmd_option_value    = hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_CMD_OPTION];

    cmd->option.no_resp = (cmd_option_value >> 7) & 0x01;

    /* Check command class is not larger than defined. */
    if (cmd->class >= FWUPCOMM_CMD_NUM_CLASS)
    {
        err = FWUPCOMM_ERR_INVALID_CMD;
    }

    /* Continue parsing the lower bytes only if the upper 4 bytes are successfully parsed. */
    if (FWUPCOMM_SUCCESS == err)
    {
        /* Convert actual value of command into serial number and store it. */
        const FWUPCOMM_FAR r_fwupcomm_cmd_table_t * p_cmd_table = r_fwupcomm_cmd_def_table_list[cmd->class].table;
        num_cmd                                    = r_fwupcomm_cmd_def_table_list[cmd->class].num_cmd;
        cmd_name_value                             = hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_CMD];

        for (uint8_t cmd_name = 0; cmd_name < num_cmd; cmd_name++)
        {
            if (cmd_name_value == p_cmd_table[cmd_name].value)
            {
                cmd->name = cmd_name;
                break;
            }

            /* Received command is not defined. */
            if ((cmd_name + 1) == num_cmd)
            {
                err = FWUPCOMM_ERR_INVALID_CMD;
            }
        }

        if (FWUPCOMM_SUCCESS == err)
        {
            cmd->arg       = hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_CMD_ARG];
            cmd->data_size = (uint16_t)(hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_CMD_DATA_LEN] +
                                        (hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_CMD_DATA_LEN + 1] << 8));

            /* Check command data size is not larger than defined size. */
            if (p_cmd_table[cmd->name].cmd_data_max_size < cmd->data_size)
            {
                err = FWUPCOMM_ERR_INVALID_CMD;
            }
        }
    }

    return err;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_parse_cmd_header
 *********************************************************************************************************************/



/**********************************************************************************************************************
 * Function Name: r_fwupcomm_receive_cmd
 * Description  : Receives a command packet.
 * Arguments    : hdl   : Handler of the module
 *              : cmd   : Command information
 *              : cmd_id: Command ID
 *********************************************************************************************************************/
static fwupcomm_err_t r_fwupcomm_receive_cmd(r_fwupcomm_hdl_t * hdl, r_fwupcomm_cmd_info_t * cmd, uint8_t * cmd_id)
{
    fwupcomm_err_t err = FWUPCOMM_SUCCESS;

#if FWUPCOMM_CFG_PARAM_CHECKING_ENABLE
    if ((NULL == hdl) || (NULL == cmd))
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }
#endif

    /* Receive command header until request size is reached or timeout. */
    err = r_fwupcomm_ch_api.recv(hdl, hdl->recv_buf.data, FWUPCOMM_CMD_HEADER_SIZE);

    if (FWUPCOMM_SUCCESS == err)
    {
        /* If header is received, parse and analyze cmd header. */
        err = r_fwupcomm_parse_cmd_header(hdl, cmd, cmd_id);
    }
    else if (FWUPCOMM_ERR_CH_RECV_NO_DATA == err)
    {
        /* Reception of header timed out. Not error. */
        err = FWUPCOMM_ERR_NO_CMD;
    }
    else
    {
        /* Receive function of communication interface returned error. */
        err = FWUPCOMM_ERR_CH_RECV;
    }

    /* Command data reception if command header parsing is successful
     * and command data size is not 0. */
    if ((FWUPCOMM_SUCCESS == err) && (0 != cmd->data_size))
    {
        /* Initialize timer timeout flag and start timer. */
        hdl->timer_timeout_flag = false;
        hdl->timer.start(hdl, FWUPCOMM_CFG_CMD_RECV_TIMEOUT, r_fwupcomm_resp_timeout_timer_cb);

        /* Receive command data until request size is reached or timeout. */
        /* WAIT_LOOP */
        do
        {
            /* Continue to execute the receive function of communication interface
             * until the function returns error or the process times out. */
            err = r_fwupcomm_ch_api.recv(hdl, &hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_CMD_DATA], cmd->data_size);
        } while ((FWUPCOMM_ERR_CH_RECV_NO_DATA == err) && (false == hdl->timer_timeout_flag));

        if (FWUPCOMM_SUCCESS == err)
        {
            /* Store pointer to command data. */
            cmd->data = &hdl->recv_buf.data[FWUPCOMM_CMD_HEADER_CMD_DATA];
        }
        else if (FWUPCOMM_ERR_CH_RECV_NO_DATA == err)
        {
            /* Reception timed out. */
            err = FWUPCOMM_ERR_INVALID_CMD;
        }
        else
        {
            /* Receive function returned error. */
            err = FWUPCOMM_ERR_CH_RECV;
        }

        /* Stop timeout timer. */
        hdl->timer.stop();
        hdl->timer_timeout_flag = false;
    }

    return err;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_receive_cmd
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_handle_incoming_cmd
 * Description  : Handles incoming command.
 * Arguments    : hdl : Handler of the module
 *              : cmd : Command information
 *              : resp: Response information
 *********************************************************************************************************************/
static fwupcomm_err_t r_fwupcomm_handle_incoming_cmd(r_fwupcomm_hdl_t * hdl,
                                                        r_fwupcomm_cmd_info_t * cmd,
                                                        r_fwupcomm_resp_info_t * resp)
{
    R_FWUPCOMM_CmdHandler_t cmd_handler;

#if FWUPCOMM_CFG_PARAM_CHECKING_ENABLE
    if ((NULL == hdl) || (NULL == cmd) || (NULL == resp))
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }
#endif

    /* Check received command class is valid. */
    if (cmd->class < FWUPCOMM_CMD_NUM_CLASS)
    {
        cmd_handler = r_fwupcomm_cmd_handler_list[cmd->class];

        /* Execute command handler. */
        if (NULL != cmd_handler)
        {
            cmd_handler(cmd, resp);
        }
    }
    else
    {
        return FWUPCOMM_ERR_INVALID_CMD;
    }

    return FWUPCOMM_SUCCESS;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_handle_incoming_cmd
 *********************************************************************************************************************/



/**********************************************************************************************************************
 * Function Name: r_fwupcomm_generate_resp
 * Description  : Generates a response packet.
 * Arguments    : hdl   : Handler of the module
 *              : cmd   : Command information
 *              : resp  : Response information
 *              : cmd_id: Command ID
 *********************************************************************************************************************/
static fwupcomm_err_t r_fwupcomm_generate_resp(r_fwupcomm_hdl_t * hdl,
                                                r_fwupcomm_cmd_info_t * cmd,
                                                r_fwupcomm_resp_info_t * resp,
                                                uint8_t cmd_id)
{
#if FWUPCOMM_CFG_PARAM_CHECKING_ENABLE
    if ((NULL == hdl) || (NULL == cmd) || (NULL == resp))
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }
#endif

    if (cmd->class >= FWUPCOMM_CMD_NUM_CLASS)
    {
        return FWUPCOMM_ERR_INVALID_ARG;
    }

    /* Get the definition table of the command. */
    const FWUPCOMM_FAR r_fwupcomm_cmd_table_t * p_cmd_table = r_fwupcomm_cmd_def_table_list[cmd->class].table;

    /* Check the command is not exceed defined number of command.  */
    if (cmd->name >= r_fwupcomm_cmd_def_table_list[cmd->class].num_cmd)
    {
        return FWUPCOMM_ERR_INVALID_ARG;
    }

    /* Check response data size is not larger than defined size. */
    if (p_cmd_table[cmd->name].resp_data_max_size < resp->data_size)
    {
        return FWUPCOMM_ERR_INVALID_ARG;
    }

    /* packet size */
    hdl->send_buf.size = FWUPCOMM_CMD_HEADER_SIZE + resp->data_size;

    hdl->send_buf.data[0] = FWUPCOMM_CFG_DEVICE_ADDRESS;
    hdl->send_buf.data[1] = (uint8_t)FWUPCOMM_CFG_CMD_VER;  /* Casting to uint8_t type is valid */
    hdl->send_buf.data[2] = r_fwupcomm_serialize_cmd_info(FWUPCOMM_PACKET_TYPE_RESP, cmd->class, cmd_id);
    hdl->send_buf.data[3] = r_fwupcomm_serialize_cmd_option(cmd->option);
    hdl->send_buf.data[4] = p_cmd_table[cmd->name].value;
    hdl->send_buf.data[5] = resp->result;
    hdl->send_buf.data[6] = UINT16_LSB(resp->data_size);
    hdl->send_buf.data[7] = UINT16_MSB(resp->data_size);

    /* Copy the response data to packet buffer. */
    if (0 != resp->data_size)
    {
        if (NULL != resp->data)
        {
            MEMCPY(&hdl->send_buf.data[FWUPCOMM_CMD_HEADER_CMD_DATA], resp->data, resp->data_size);
        }
        else
        {
            return FWUPCOMM_ERR_INVALID_PTR;
        }
    }

    return FWUPCOMM_SUCCESS;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_generate_resp
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: r_fwupcomm_generate_err_resp
 * Description  : Generates an error response packet.
 * Arguments    : hdl: Handler of the module
 *              : cmd: Command information
 *********************************************************************************************************************/
static fwupcomm_err_t r_fwupcomm_generate_err_resp(r_fwupcomm_hdl_t * hdl,
                                                    r_fwupcomm_cmd_info_t * cmd)
{
#if FWUPCOMM_CFG_PARAM_CHECKING_ENABLE
    if ((NULL == hdl) || (NULL == cmd))
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }
#endif

    /* packet size */
    hdl->send_buf.size = FWUPCOMM_CMD_HEADER_SIZE;       /* No response data. */

    hdl->send_buf.data[0] = FWUPCOMM_CFG_DEVICE_ADDRESS; /* My device address. */
    hdl->send_buf.data[1] = (uint8_t)FWUPCOMM_CFG_CMD_VER;   /* Command version used by itself. */
    hdl->send_buf.data[2] = hdl->recv_buf.data[2];       /* Same value as received command. */
    hdl->send_buf.data[3] = hdl->recv_buf.data[3];       /* Same value as received command. */
    hdl->send_buf.data[4] = hdl->recv_buf.data[4];       /* Same value as received command. */
    hdl->send_buf.data[5] = hdl->recv_buf.data[5];       /* Same value as received command. */
    hdl->send_buf.data[6] = 0;                           /* Response data size is zero. */
    hdl->send_buf.data[7] = 0;                           /* Response data size is zero. */

    return FWUPCOMM_SUCCESS;
}
/**********************************************************************************************************************
 End of function r_fwupcomm_generate_err_resp
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: R_FWUPCOMM_ProcessCmdLoop
 * Description  : Receives a command from the primary MCU, runs the corresponding handler, 
 *                and sends the result of executing the command. Periodically execute this function 
 *                in the secondary MCU while it is waiting for commands.
 * Argument     : hdl: Handler of the module
 * Return Value : FWUPCOMM_SUCCESS:         The channel was successfully initialized.
 *              : FWUPCOMM_ERR_NOT_OPEN:    The module has not been opened.
 *              : FWUPCOMM_ERR_INVALID_PTR: The pointer passed as an argument was NULL.
 *              : FWUPCOMM_ERR_INVALID_ARG: The parameter passed as an argument was invalid.
 *              : FWUPCOMM_ERR_NO_CMD:      No command was received.
 *              : FWUPCOMM_ERR_INVALID_CMD: An invalid command was received.
 *              : FWUPCOMM_ERR_CH_SEND:     Sending of data in the communications channel failed.
 *              : FWUPCOMM_ERR_CH_RECV:     Receiving of data from the communications channel failed.
 *********************************************************************************************************************/
fwupcomm_err_t R_FWUPCOMM_ProcessCmdLoop(r_fwupcomm_hdl_t * hdl)
{
    fwupcomm_err_t         err      = FWUPCOMM_SUCCESS;
    uint8_t                s_cmd_id = 0;
    r_fwupcomm_cmd_info_t  cmd      = {0};
    r_fwupcomm_resp_info_t resp     = {0};

#if FWUPCOMM_CFG_PARAM_CHECKING_ENABLE
    if (NULL == hdl)
    {
        return FWUPCOMM_ERR_INVALID_PTR;
    }
#endif

    /* Check module open flag is true. */
    if (false == hdl->open_flag)
    {
        return FWUPCOMM_ERR_NOT_OPEN;
    }

    /* Command reception. */
    err = r_fwupcomm_receive_cmd(hdl, &cmd, &s_cmd_id);

    if (FWUPCOMM_SUCCESS == err)
    {
        /* Call handler of the command received. */
        err = r_fwupcomm_handle_incoming_cmd(hdl, &cmd, &resp);
    }

    /* Don't send response packet if no_resp option is enabled. */
    if (0 == cmd.option.no_resp)
    {
        if (FWUPCOMM_SUCCESS == err)
        {
            /* Generate response packet with the result of the handler. */
            err = r_fwupcomm_generate_resp(hdl, &cmd, &resp, s_cmd_id);
        }
        else if (FWUPCOMM_ERR_INVALID_CMD == err)
        {
            /* Generate error response packet when command is invalid. */
            err = r_fwupcomm_generate_err_resp(hdl, &cmd);
        }
        else
        {
            ;
        }

        if (FWUPCOMM_SUCCESS == err)
        {
            /* Send response packet. */
            err = r_fwupcomm_ch_send_with_timeout(hdl, FWUPCOMM_CFG_CMD_SEND_TIMEOUT);

            /* Clear buffers. */
            memset((void *)hdl->send_buf.data, 0x00, FWUPCOMM_SEND_PACKET_BUFFER_SIZE);
            memset((void *)hdl->recv_buf.data, 0x00, FWUPCOMM_RECV_PACKET_BUFFER_SIZE);

            hdl->send_buf.size = 0;
            hdl->recv_buf.size = 0;

            /* UART: Flush channel Rx buffer. */
            /* SPI:  Setup the request reception and wait for the clock. */
            r_fwupcomm_ch_api.rx_reset(hdl);
        }
    }

    return err;
}
/**********************************************************************************************************************
 End of function R_FWUPCOMM_ProcessCmdLoop
 *********************************************************************************************************************/

#endif
