/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "rm_comms_uart.h"
#include "../rm_comms_lock/rm_comms_lock.h"

/*******************************************************************************************************************//**
 * Macro definitions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Typedef definitions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Private function prototypes
 **********************************************************************************************************************/
static void rm_comms_uart_notify_application (rm_comms_uart_instance_ctrl_t const * p_ctrl, rm_comms_uart_event_t event);

/*******************************************************************************************************************//**
 * Private global variables
 **********************************************************************************************************************/
fsp_err_t rm_comms_uart_bus_open (rm_comms_uart_ctrl_t * const p_api_ctrl);
fsp_err_t rm_comms_uart_bus_close (rm_comms_uart_ctrl_t * const p_api_ctrl);
fsp_err_t rm_comms_uart_bus_read (rm_comms_uart_ctrl_t * const p_api_ctrl, uint8_t * const p_dest, uint32_t const bytes);
fsp_err_t rm_comms_uart_bus_write (rm_comms_uart_ctrl_t * const p_api_ctrl, uint8_t * const p_src, uint32_t const bytes);

void rm_comms_uart_callback (rm_comms_uart_ctrl_t const * p_api_ctrl, rm_comms_uart_event_t event);

/*******************************************************************************************************************//**
 * Global variables
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: rm_comms_uart_bus_open
 * @brief Configures the UART modes and starts the SCI channel using R_Config_SCIX_Start(). ("X": Channel No. (0- ))
 * @retval FSP_SUCCESS
 **********************************************************************************************************************/
fsp_err_t rm_comms_uart_bus_open(rm_comms_uart_ctrl_t * const p_api_ctrl)
{
    rm_comms_uart_instance_ctrl_t    * p_ctrl            = (rm_comms_uart_instance_ctrl_t *) p_api_ctrl;
    uart_instance_t                  * p_driver_instance = (uart_instance_t *) p_ctrl->p_extend->p_uart;

    /* Open */
    p_driver_instance->p_app_api->open();

    return FSP_SUCCESS;
}
/**********************************************************************************************************************
 * End of function rm_comms_uart_bus_open
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: rm_comms_uart_bus_close
 * @brief Stops the SCI channel using R_Config_SCIX_Stop(). ("X": Channel No. (0- ))
 * @retval FSP_SUCCESS
 **********************************************************************************************************************/
fsp_err_t rm_comms_uart_bus_close(rm_comms_uart_ctrl_t * const p_api_ctrl)
{
    rm_comms_uart_instance_ctrl_t    * p_ctrl            = (rm_comms_uart_instance_ctrl_t *) p_api_ctrl;
    uart_instance_t                  * p_driver_instance = (uart_instance_t *) p_ctrl->p_extend->p_uart;

    /* Close */
    p_driver_instance->p_app_api->close();

    return FSP_SUCCESS;
}
/**********************************************************************************************************************
 * End of function rm_comms_uart_bus_close
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: rm_comms_uart_bus_read
 * @brief Read data from the device using the UART driver module.
 * @param[in]  p_api_ctrl           Pointer to instance control structure
 *             p_dest               Pointer to destination buffer
 *             bytes                Number of destination data
 * @retval FSP_SUCCESS              Successfully started.
 ***********************************************************************************************************************/
fsp_err_t rm_comms_uart_bus_read(rm_comms_uart_ctrl_t * const p_api_ctrl, uint8_t * const p_dest, uint32_t const bytes)
{
    rm_comms_uart_instance_ctrl_t    * p_ctrl            = (rm_comms_uart_instance_ctrl_t *) p_api_ctrl;
    uart_instance_t                  * p_driver_instance = (uart_instance_t *) p_ctrl->p_extend->p_uart;
    MD_STATUS                          uart_err;

    fsp_err_t                          err;

    /* Read data */
    uart_err = (p_driver_instance->p_module_api->read)(p_dest, (uint16_t)bytes);
    switch (uart_err)
    {
        case MD_OK:
            err = FSP_SUCCESS;
            break;
        case MD_ARGERROR:
            err = FSP_ERR_INVALID_ARGUMENT;
            break;
        default:
            err = FSP_ERR_ASSERTION;
            break;
    }
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    return FSP_SUCCESS;
}
/**********************************************************************************************************************
 * End of function rm_comms_uart_bus_read
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: rm_comms_uart_bus_write
 * @brief Write data to the device using the UART driver module.
 * @param[in]  p_api_ctrl           Pointer to instance control structure
 *             p_src                Pointer to source buffer
 *             bytes                Number of source data
 * @retval FSP_SUCCESS              Successfully started.
 ***********************************************************************************************************************/
fsp_err_t rm_comms_uart_bus_write(rm_comms_uart_ctrl_t * const p_api_ctrl, uint8_t * const p_src, uint32_t const bytes)
{
    rm_comms_uart_instance_ctrl_t    * p_ctrl            = (rm_comms_uart_instance_ctrl_t *) p_api_ctrl;
    uart_instance_t                  * p_driver_instance = (uart_instance_t *) p_ctrl->p_extend->p_uart;
    MD_STATUS                          uart_err;

    fsp_err_t                          err;

    /* Write data */
    uart_err = (p_driver_instance->p_module_api->write)(p_src, (uint16_t)bytes);
    switch (uart_err)
    {
        case MD_OK:
            err = FSP_SUCCESS;
            break;
        case MD_ARGERROR:
            err = FSP_ERR_INVALID_ARGUMENT;
            break;
        case MD_ERROR:
        default:
            err = FSP_ERR_ASSERTION;
            break;
    }
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    return FSP_SUCCESS;
}
/**********************************************************************************************************************
 * End of function rm_comms_uart_bus_write
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: rm_comms_uart_notify_application
 * @brief Notifies user application. This function is called in rm_comms_uart_callback().
 * @param[in]  p_ctrl               Pointer to instance control structure
 *             event
 * @retval None
 **********************************************************************************************************************/
static void rm_comms_uart_notify_application(rm_comms_uart_instance_ctrl_t const * p_ctrl, rm_comms_uart_event_t event)
{
    if (p_ctrl->p_callback)
    {
        rm_comms_uart_callback_args_t args =
        {
            .p_context = p_ctrl->p_context,
            .event     = event,
        };

        p_ctrl->p_callback(&args);
    }
}
/**********************************************************************************************************************
 * End of function rm_comms_uart_notify_application
 *********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: rm_comms_uart_callback
 * @brief Common callback function called in the UART driver callback function.
 * @param[in]  p_api_ctrl           Pointer to instance control structure
 *             event
 * @retval None
 **********************************************************************************************************************/
void rm_comms_uart_callback(rm_comms_uart_ctrl_t const * p_api_ctrl, rm_comms_uart_event_t event)
{
    rm_comms_uart_instance_ctrl_t    * p_ctrl            = (rm_comms_uart_instance_ctrl_t *) p_api_ctrl;
#if BSP_CFG_RTOS
    rm_comms_uart_extended_cfg_t const * p_extend = p_ctrl->p_extend;
#endif

    /* Set events */
    switch (event)
    {
        case RM_COMMS_UART_EVENT_TX_OPERATION_COMPLETE:
#if BSP_CFG_RTOS
            if (NULL != p_extend->p_tx_semaphore)
            {
                rm_comms_semaphore_release(p_extend->p_tx_semaphore);
            }
#endif
            rm_comms_uart_notify_application(p_ctrl, RM_COMMS_UART_EVENT_TX_OPERATION_COMPLETE);
            break;

        case RM_COMMS_UART_EVENT_RX_OPERATION_COMPLETE:
#if BSP_CFG_RTOS
            if (NULL != p_extend->p_rx_semaphore)
            {
                rm_comms_semaphore_release(p_extend->p_rx_semaphore);
            }
#endif
            rm_comms_uart_notify_application(p_ctrl, RM_COMMS_UART_EVENT_RX_OPERATION_COMPLETE);
            break;

        case RM_COMMS_UART_EVENT_OPERATION_COMPLETE:
            break;

        default:
#if BSP_CFG_RTOS
            if (NULL != p_extend->p_tx_semaphore)
            {
                rm_comms_semaphore_release(p_extend->p_tx_semaphore);
            }

            if (NULL != p_extend->p_rx_semaphore)
            {
                rm_comms_semaphore_release(p_extend->p_rx_semaphore);
            }
#endif
            rm_comms_uart_notify_application(p_ctrl, RM_COMMS_UART_EVENT_ERROR);
            break;
    }
}
/**********************************************************************************************************************
 * End of function rm_comms_uart_callback
 *********************************************************************************************************************/
