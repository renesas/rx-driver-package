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
 * Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_cellular_writecertificate.c
 * Description  : Function to write certificate file to NVM of cellular controller.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "cellular_private_api.h"
#include "cellular_freertos.h"
#include "at_command.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/
static e_cellular_err_t cellular_write_certificate (st_cellular_ctrl_t * const p_ctrl,
                                                        const e_cellular_nvm_type_t data_type,
                                                        const uint8_t index,
                                                        const uint8_t * p_data,
                                                        const uint32_t size);
static uint16_t cellular_send_size_check (st_cellular_ctrl_t * const p_ctrl,
                                            const uint32_t length,
                                            const uint32_t complete_length);
#if CELLULAR_CFG_CTS_SW_CTRL == 1
static e_cellular_timeout_check_t cellular_tx_flag_check (st_cellular_ctrl_t * const p_ctrl);
#endif

/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_WriteCertificate
 ************************************************************************************************/
e_cellular_err_t R_CELLULAR_WriteCertificate(st_cellular_ctrl_t * const p_ctrl,
                                                const e_cellular_nvm_type_t data_type,
                                                const uint8_t index,
                                                const uint8_t * p_data,
                                                const uint32_t size)
{
    uint32_t                   preemption    = 0;
    e_cellular_err_t           ret           = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_SUCCESS;

    preemption = cellular_interrupt_disable();
    if ((NULL == p_ctrl) ||
        ((CELLULAR_NVM_TYPE_CERTIFICATE != data_type) && (CELLULAR_NVM_TYPE_PRIVATEKEY != data_type)) ||
            (CELLULAR_MAX_NVM_CERTIFICATE_INDEX < index) || (NULL == p_data) ||
            (CELLULAR_NVM_CERTIFICATE_SIZE_L > size) || (CELLULAR_NVM_CERTIFICATE_SIZE_H < size))
    {
        ret = CELLULAR_ERR_PARAMETER;
    }
    else
    {
        if (0 != (p_ctrl->running_api_count % 2))
        {
            ret = CELLULAR_ERR_OTHER_API_RUNNING;
        }
        else if (CELLULAR_SYSTEM_CLOSE == p_ctrl->system_state)
        {
            ret = CELLULAR_ERR_NOT_OPEN;
        }
        else
        {
            p_ctrl->running_api_count += 2;
        }
    }
    cellular_interrupt_enable(preemption);

    if (CELLULAR_SUCCESS == ret)
    {
        semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
        if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
        {
            ret = cellular_write_certificate(p_ctrl, data_type, index, p_data, size);
            cellular_give_semaphore(p_ctrl->at_semaphore);
        }
        else
        {
            ret = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
        }
        p_ctrl->running_api_count -= 2;
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function R_CELLULAR_WriteCertificate
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            cellular_write_certificate
 * Description    @details       Send data to the module.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     data_type -
 *                                  Socket number for sending data.
 *                @param[in]     index -
 *                                  Pointer to send data.
 *                @param[in]     p_data -
 *                                  Pointer to send data.
 *                @param[in]     size -
 *                                  Send data size.
 * Return Value   @retval        CELLULAR_SUCCESS -
 *                                  Successfully.
 *                @retval        CELLULAR_ERR_PARAMETER -
 *                                  Argument is abnormal.
 *                @retval        CELLULAR_ERR_SOCKET_NUM -
 *                                  No available socket.
 *                @retval        CELLULAR_ERR_MODULE_COM -
 *                                  Communication with module failed.
 *                @retval        CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING -
 *                                  Other AT commands are running.
 ************************************************************************************************/
static e_cellular_err_t cellular_write_certificate(st_cellular_ctrl_t * const p_ctrl,
                                                    const e_cellular_nvm_type_t data_type,
                                                    const uint8_t index,
                                                    const uint8_t * p_data,
                                                    const uint32_t size)
{
#if CELLULAR_CFG_CTS_SW_CTRL == 1
    uint16_t                   sci_send_size   = 0;
#endif
    uint32_t                   complete_length = 0;
    uint16_t                   send_size       = 0;
    sci_err_t                  sci_ret         = SCI_SUCCESS;
    e_cellular_err_t           ret             = CELLULAR_SUCCESS;
    e_cellular_timeout_check_t timeout         = CELLULAR_NOT_TIMEOUT;
    e_cellular_atc_return_t    atc_res         = ATC_RETURN_NONE;
    e_cellular_err_semaphore_t semaphore_ret   = CELLULAR_SEMAPHORE_ERR_TAKE;

    cellular_timeout_init(&p_ctrl->sci_ctrl.timeout_ctrl, p_ctrl->sci_ctrl.atc_timeout);

    if (CELLULAR_PSM_ACTIVE == p_ctrl->ring_ctrl.psm)
    {
        while (1)
        {
            semaphore_ret = cellular_take_semaphore(p_ctrl->ring_ctrl.rts_semaphore);
            if (CELLULAR_SEMAPHORE_SUCCESS == semaphore_ret)
            {
                break;
            }
            cellular_delay_task(1);
        }
#if CELLULAR_CFG_CTS_SW_CTRL == 1
        cellular_rts_hw_flow_enable();
#else
        cellular_rts_ctrl(0);
#endif
#ifdef CELLULAR_RTS_DELAY
        cellular_delay_task(CELLULAR_RTS_DELAYTIME);
#endif
    }

    ret = atc_sqnsnvw(p_ctrl, data_type, index, size);
    if (CELLULAR_SUCCESS != ret)
    {
        goto cellular_write_certificate_fail;
    }

    cellular_set_atc_number(p_ctrl, ATC_SQNSSENDEXT_END);

    while (complete_length < size)
    {
        send_size = cellular_send_size_check(p_ctrl, size, complete_length);
#if CELLULAR_CFG_CTS_SW_CTRL == 0
        p_ctrl->sci_ctrl.tx_end_flg = CELLULAR_TX_END_FLAG_OFF;

        sci_ret = R_SCI_Send(p_ctrl->sci_ctrl.sci_hdl, (uint8_t *)p_data + complete_length, send_size); //cast

        if (SCI_SUCCESS == sci_ret)
        {
            /* tx_end_flg check loop */
            while (1)
            {
                if (CELLULAR_TX_END_FLAG_ON == p_ctrl->sci_ctrl.tx_end_flg)
                {
                    complete_length += send_size;
                    break;   /* Break the tx_end_flg check loop */
                }

                timeout = cellular_check_timeout(&p_ctrl->sci_ctrl.timeout_ctrl);
                if (CELLULAR_TIMEOUT == timeout)
                {
                    ret = CELLULAR_ERR_MODULE_COM;
                    goto cellular_write_certificate_fail;
                }
            }
        }
        else
        {
            ret = CELLULAR_ERR_MODULE_COM;
            goto cellular_write_certificate_fail;
        }
#else
        sci_send_size = 0;

        do
        {
            if (1 != CELLULAR_GET_PIDR(CELLULAR_CFG_CTS_PORT, CELLULAR_CFG_CTS_PIN))
            {
                p_ctrl->sci_ctrl.tx_end_flg = CELLULAR_TX_END_FLAG_OFF;

                sci_ret = R_SCI_Send(p_ctrl->sci_ctrl.sci_hdl,
                                        (uint8_t *)p_data + complete_length + sci_send_size, 1);    //cast
                if (SCI_SUCCESS != sci_ret)
                {
                    ret = CELLULAR_ERR_MODULE_COM;
                    goto cellular_write_certificate_fail;
                }
                else
                {
                    timeout = cellular_tx_flag_check(p_ctrl);
                    if (CELLULAR_TIMEOUT == timeout)
                    {
                        ret = CELLULAR_ERR_MODULE_COM;
                        goto cellular_write_certificate_fail;
                    }
                    else
                    {
                        sci_send_size++;
                    }
                }
            }
        } while (sci_send_size < send_size);

        complete_length += sci_send_size;
#endif  /* CELLULAR_CFG_CTS_SW_CTRL == 0 */
    }

    while (1)
    {
        atc_res = cellular_get_atc_response(p_ctrl);
        if (ATC_RETURN_NONE != atc_res)
        {
            if (ATC_RETURN_OK == atc_res)
            {
                break;
            }
            else
            {
                ret = CELLULAR_ERR_MODULE_COM;
                goto cellular_write_certificate_fail;
            }
        }

        timeout = cellular_check_timeout(&p_ctrl->sci_ctrl.timeout_ctrl);
        if (CELLULAR_TIMEOUT == timeout)
        {
            ret = CELLULAR_ERR_MODULE_COM;
            goto cellular_write_certificate_fail;
        }
    }

cellular_write_certificate_fail:
    if (CELLULAR_PSM_ACTIVE == p_ctrl->ring_ctrl.psm)
    {
#if CELLULAR_CFG_CTS_SW_CTRL == 1
        cellular_rts_hw_flow_disable();
#endif
        cellular_rts_ctrl(1);
        cellular_give_semaphore(p_ctrl->ring_ctrl.rts_semaphore);
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_write_certificate
 *********************************************************************************************************************/

/*************************************************************************************************
 * Function Name  @fn            cellular_send_size_check
 * Description    @details       Check the size of the data to be sent.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     length -
 *                                  Send data size.
 *                @param[in]     complete_length -
 *                                  Size of the completed transmission to the module.
 * Return Value   @retval        More than 1 -
 *                                  Successfully size check.
 ************************************************************************************************/
static uint16_t cellular_send_size_check(st_cellular_ctrl_t * const p_ctrl,
                                            const uint32_t length,
                                            const uint32_t complete_length)
{
    uint32_t send_size = 0;
    uint16_t ret       = 0;

    if ((length - complete_length) > p_ctrl->sci_ctrl.tx_buff_size)
    {
        send_size = p_ctrl->sci_ctrl.tx_buff_size;
    }
    else
    {
        send_size = (length - complete_length);
    }

    if (send_size > p_ctrl->sci_ctrl.tx_process_size) //tx_process_size is max 1500
    {
        ret = p_ctrl->sci_ctrl.tx_process_size;
    }
    else
    {
        ret = (uint16_t)send_size; // send_size is less than 1500
    }

    return ret;
}
/**********************************************************************************************************************
 * End of function cellular_send_size_check
 *********************************************************************************************************************/

#if CELLULAR_CFG_CTS_SW_CTRL == 1
/*************************************************************************************************
 * Function Name  @fn            cellular_tx_flag_check
 * Description    @details       Wait for transmission completion flag.
 * Arguments      @param[in/out] p_ctrl -
 *                                  Pointer to managed structure.
 *                @param[in]     socket_no -
 *                                  Socket number for sending data.
 * Return Value   @retval        CELLULAR_NOT_TIMEOUT -
 *                                  Successfully flag check.
 *                @retval        CELLULAR_TIMEOUT -
 *                                  Time out.
 ************************************************************************************************/
static e_cellular_timeout_check_t cellular_tx_flag_check(st_cellular_ctrl_t * const p_ctrl)
{
    e_cellular_timeout_check_t timeout = CELLULAR_NOT_TIMEOUT;

    st_cellular_time_ctrl_t * const p_cellular_timeout_ctrl = &p_ctrl->sci_ctrl.timeout_ctrl;

    while (1)
    {
        if (CELLULAR_TX_END_FLAG_ON == p_ctrl->sci_ctrl.tx_end_flg)
        {
            break;
        }
#if CELLULAR_CFG_CTS_SW_CTRL == 0
        timeout = cellular_check_timeout(p_cellular_timeout_ctrl);

        if (CELLULAR_TIMEOUT == timeout)
        {
            break;
        }

        cellular_delay_task(1);
#else
        p_cellular_timeout_ctrl->this_time = cellular_get_tickcount();

        if (CELLULAR_TIMEOUT_NOT_OVERFLOW == p_cellular_timeout_ctrl->timeout_overflow_flag)
        {
            if ((p_cellular_timeout_ctrl->this_time >= p_cellular_timeout_ctrl->end_time) ||
                    (p_cellular_timeout_ctrl->this_time < p_cellular_timeout_ctrl->start_time))
            {
                timeout = CELLULAR_TIMEOUT;
                break;
            }
        }
        else
        {
            if ((p_cellular_timeout_ctrl->this_time < p_cellular_timeout_ctrl->start_time) &&
                    (p_cellular_timeout_ctrl->this_time >= p_cellular_timeout_ctrl->end_time))
            {
                timeout = CELLULAR_TIMEOUT;
                break;
            }
        }

        R_BSP_NOP();
#endif  /* CELLULAR_CFG_CTS_SW_CTRL == 0 */
    }

    return timeout;
}
/**********************************************************************************************************************
 * End of function cellular_tx_flag_check
 *********************************************************************************************************************/
#endif  /* CELLULAR_CFG_CTS_SW_CTRL == 1 */
