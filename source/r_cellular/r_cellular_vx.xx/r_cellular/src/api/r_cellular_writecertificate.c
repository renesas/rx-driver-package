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
 * Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
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

#if (CELLULAR_IMPLEMENT_TYPE == 'B')
/**********************************************************************************************************************
 * Private (static) variables and functions
 *********************************************************************************************************************/
static e_cellular_err_t cellular_write_certificate (st_cellular_ctrl_t * const p_ctrl,
                                    const e_cellular_nvm_type_t data_type,
                                    const uint8_t index, const uint8_t * p_data, const uint32_t size);

static uint32_t cellular_send_size_check (st_cellular_ctrl_t * const p_ctrl, const int32_t length,
                                            const int32_t complete_length);

/*************************************************************************************************
 * Function Name  @fn            R_CELLULAR_WriteCertificate
 ************************************************************************************************/
e_cellular_err_t R_CELLULAR_WriteCertificate(st_cellular_ctrl_t * const p_ctrl, const e_cellular_nvm_type_t data_type,
                                    const uint8_t index, const uint8_t * p_data, const uint32_t size)
{
    e_cellular_err_t ret = CELLULAR_SUCCESS;
    e_cellular_err_semaphore_t semaphore_ret = CELLULAR_SEMAPHORE_SUCCESS;

    if ((NULL == p_ctrl) ||
            ((CELLULAR_NVM_TYPE_CERTIFICATE != data_type) && (CELLULAR_NVM_TYPE_PRIVATEKEY != data_type)) ||
            (CELLULAR_MAX_NVM_CERTIFICATE_INDEX < index) || (NULL == p_data) ||
            (CELLULAR_NVM_CERTIFICATE_SIZE_L > size) || (CELLULAR_NVM_CERTIFICATE_SIZE_H < size))
    {
        ret = CELLULAR_ERR_PARAMETER;
    }
    else
    {
        if (CELLULAR_SYSTEM_CLOSE == p_ctrl->system_state)
        {
            ret = CELLULAR_ERR_NOT_OPEN;
        }
        else
        {
            R_BSP_NOP();
        }
    }


    if (CELLULAR_SUCCESS == ret)
    {
        semaphore_ret = cellular_take_semaphore(p_ctrl->at_semaphore);
        if (CELLULAR_SEMAPHORE_SUCCESS != semaphore_ret)
        {
            ret = CELLULAR_ERR_OTHER_ATCOMMAND_RUNNING;
        }
    }

    if (CELLULAR_SUCCESS == ret)
    {
        ret = cellular_write_certificate(p_ctrl, data_type, index, p_data, size);
        cellular_give_semaphore(p_ctrl->at_semaphore);
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
                                    const uint8_t index, const uint8_t * p_data, const uint32_t size)
{

    uint32_t complete_length = 0;
    uint32_t send_size = 0;
    sci_err_t sci_ret = SCI_SUCCESS;
    e_cellular_err_t ret = CELLULAR_SUCCESS;
    e_cellular_timeout_check_t timeout = CELLULAR_NOT_TIMEOUT;
    e_cellular_atc_return_t     atc_res = ATC_RETURN_NONE;

    ret = atc_sqnsnvw(p_ctrl, data_type, index, size);
    if (CELLULAR_SUCCESS != ret)
    {
        goto cellular_write_certificate_fail;
    }

    cellular_set_atc_number(p_ctrl, ATC_SQNSSENDEXT_END);

    while (complete_length < size)
    {
        cellular_timeout_init(&p_ctrl->sci_ctrl.timeout_ctrl, p_ctrl->sci_ctrl.atc_timeout);
        send_size = cellular_send_size_check(p_ctrl, size, complete_length);

        p_ctrl->sci_ctrl.tx_end_flg = CELLULAR_TX_END_FLAG_OFF;

        sci_ret = R_SCI_Send(p_ctrl->sci_ctrl.sci_hdl, (uint8_t SCI_FAR *)p_data + complete_length, send_size);

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
    }

    cellular_timeout_init(&p_ctrl->sci_ctrl.timeout_ctrl, p_ctrl->sci_ctrl.atc_timeout);
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
    {
        return ret;
    }

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
static uint32_t cellular_send_size_check(st_cellular_ctrl_t * const p_ctrl, const int32_t length,
                                            const int32_t complete_length)
{
    uint32_t send_size = 0;

    if ((length - complete_length) > p_ctrl->sci_ctrl.tx_buff_size)
    {
        send_size = p_ctrl->sci_ctrl.tx_buff_size;
    }
    else
    {
        send_size = (length - complete_length);
    }

    if (send_size > p_ctrl->sci_ctrl.tx_process_size)
    {
        send_size = p_ctrl->sci_ctrl.tx_process_size;
    }

    return send_size;
}
/**********************************************************************************************************************
 * End of function cellular_send_size_check
 *********************************************************************************************************************/
#endif /* (CELLULAR_IMPLEMENT_TYPE == 'B') */
