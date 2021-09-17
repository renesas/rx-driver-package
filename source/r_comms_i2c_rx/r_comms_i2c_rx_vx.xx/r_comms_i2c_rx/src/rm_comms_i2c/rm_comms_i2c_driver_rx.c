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
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_comms_i2c_if.h"

/*******************************************************************************************************************//**
 * Macro definitions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Typedef definitions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Private function prototypes
 **********************************************************************************************************************/
static void rm_comms_i2c_process_in_callback(rm_comms_ctrl_t * const          p_api_ctrl,
                                             rm_comms_callback_args_t * const p_args);
static fsp_err_t rm_comms_i2c_bus_reconfigure(rm_comms_ctrl_t * const p_api_ctrl);
static rm_comms_event_t rm_comms_i2c_bus_callbackErrorCheck(rm_comms_ctrl_t const * p_api_ctrl);
#if BSP_CFG_RTOS
static fsp_err_t rm_comms_i2c_os_recursive_mutex_acquire(rm_comms_i2c_mutex_t const * p_mutex, uint32_t const timeout);
static fsp_err_t rm_comms_i2c_os_recursive_mutex_release(rm_comms_i2c_mutex_t const * p_mutex);
static fsp_err_t rm_comms_i2c_os_semaphore_acquire(rm_comms_i2c_semaphore_t const * p_semaphore,
                                                   uint32_t const                   timeout);
static fsp_err_t rm_comms_i2c_os_semaphore_release_from_ISR(rm_comms_i2c_semaphore_t const * p_semaphore);
#endif

/*******************************************************************************************************************//**
 * Private global variables
 **********************************************************************************************************************/
fsp_err_t rm_comms_i2c_bus_status_check(rm_comms_ctrl_t * const p_api_ctrl);
fsp_err_t rm_comms_i2c_bus_read(rm_comms_ctrl_t * const p_api_ctrl, uint8_t * const p_dest, uint32_t const bytes);
fsp_err_t rm_comms_i2c_bus_write(rm_comms_ctrl_t * const p_api_ctrl, uint8_t * const p_src, uint32_t const bytes);
fsp_err_t rm_comms_i2c_bus_write_read(rm_comms_ctrl_t * const            p_api_ctrl,
                                      rm_comms_write_read_params_t const write_read_params);

const mcu_lock_t riic_lock[3] = {
#if defined (RIIC0)
    BSP_LOCK_RIIC0,
#else
    BSP_NUM_LOCKS,
#endif
#if defined (RIIC1)
    BSP_LOCK_RIIC1,
#else
    BSP_NUM_LOCKS,
#endif
#if defined (RIIC2)
    BSP_LOCK_RIIC2
#else
    BSP_NUM_LOCKS,
#endif
};
const mcu_lock_t sci_iic_lock[13] = {
#if defined (SCI0)
    BSP_LOCK_SCI0,
#else
    BSP_NUM_LOCKS,
#endif
#if defined (SCI1)
    BSP_LOCK_SCI1,
#else
    BSP_NUM_LOCKS,
#endif
#if defined (SCI2)
    BSP_LOCK_SCI2,
#else
    BSP_NUM_LOCKS,
#endif
#if defined (SCI3)
    BSP_LOCK_SCI3,
#else
    BSP_NUM_LOCKS,
#endif
#if defined (SCI4)
    BSP_LOCK_SCI4,
#else
    BSP_NUM_LOCKS,
#endif
#if defined (SCI5)
    BSP_LOCK_SCI5,
#else
    BSP_NUM_LOCKS,
#endif
#if defined (SCI6)
    BSP_LOCK_SCI6,
#else
    BSP_NUM_LOCKS,
#endif
#if defined (SCI7)
    BSP_LOCK_SCI7,
#else
    BSP_NUM_LOCKS,
#endif
#if defined (SCI8)
    BSP_LOCK_SCI8,
#else
    BSP_NUM_LOCKS,
#endif
#if defined (SCI9)
    BSP_LOCK_SCI9,
#else
    BSP_NUM_LOCKS,
#endif
#if defined (SCI10)
    BSP_LOCK_SCI10,
#else
    BSP_NUM_LOCKS,
#endif
#if defined (SCI11)
    BSP_LOCK_SCI11,
#else
    BSP_NUM_LOCKS,
#endif
#if defined (SCI12)
    BSP_LOCK_SCI12
#else
    BSP_NUM_LOCKS,
#endif
};

/*******************************************************************************************************************//**
 * Global variables
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Checks if the I2C driver module is open.
 *
 * @retval FSP_SUCCESS                  successfully configured.
 * @retval FSP_ERR_COMMS_BUS_NOT_OPEN   I2C driver is not open.
 **********************************************************************************************************************/
fsp_err_t rm_comms_i2c_bus_status_check(rm_comms_ctrl_t * const p_api_ctrl)
{
    rm_comms_i2c_instance_ctrl_t * p_ctrl            = (rm_comms_i2c_instance_ctrl_t *) p_api_ctrl;
    i2c_driver_instance_t        * p_driver_instance = (i2c_driver_instance_t *) p_ctrl->p_bus->p_driver_instance;
    bsp_lock_t                     lock_i2c;

    if (COMMS_DRIVER_I2C == p_driver_instance->driver_type)
    {
#if (COMMS_I2C_CFG_DRIVER_I2C)
        riic_info_t * p_i2c_info = (riic_info_t *)p_driver_instance->p_info;

        lock_i2c = g_bsp_Locks[riic_lock[p_i2c_info->ch_no]];
#endif        
    }
    else if (COMMS_DRIVER_SCI_I2C == p_driver_instance->driver_type)
    {
#if (COMMS_I2C_CFG_DRIVER_SCI_I2C)
        sci_iic_info_t * p_i2c_info = (sci_iic_info_t *)p_driver_instance->p_info;

        lock_i2c = g_bsp_Locks[sci_iic_lock[p_i2c_info->ch_no]];
#endif
    }
    FSP_ERROR_RETURN(true == lock_i2c.lock, FSP_ERR_COMMS_BUS_NOT_OPEN);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Function Name: rm_comms_i2c_bus_read
 * @brief Read data from the device using the I2C driver module.
 * @param[in]  p_api_ctrl           Pointer to instance control structure
 *             p_dest               Pointer to destination buffer
 *             bytes                Number of destination data
 * @retval FSP_SUCCESS              Successfully started.
 ***********************************************************************************************************************/
fsp_err_t rm_comms_i2c_bus_read(rm_comms_ctrl_t * const p_api_ctrl, uint8_t * const p_dest, uint32_t const bytes)
{
    fsp_err_t err = FSP_SUCCESS;
    rm_comms_i2c_instance_ctrl_t    * p_ctrl            = (rm_comms_i2c_instance_ctrl_t *) p_api_ctrl;
    i2c_driver_instance_t           * p_driver_instance = (i2c_driver_instance_t *) p_ctrl->p_bus->p_driver_instance;

    err = rm_comms_i2c_bus_reconfigure(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    if (COMMS_DRIVER_I2C == p_driver_instance->driver_type)
    {
#if (COMMS_I2C_CFG_DRIVER_I2C)
        riic_return_t i2c_err;
        riic_info_t * p_i2c_info = (riic_info_t *)p_driver_instance->p_info;

        /* Set address data */
        p_i2c_info->p_data1st = FIT_NO_PTR;
        p_i2c_info->cnt1st    = 0U;

        /* Set transfer data */
        if (0 < bytes)
        {
            p_i2c_info->p_data2nd = p_dest;
            p_i2c_info->cnt2nd    = bytes;
        }
        else
        {
            p_i2c_info->p_data2nd = FIT_NO_PTR;
            p_i2c_info->cnt2nd    = 0;
        }

        /* Read data */
        i2c_err = R_RIIC_MasterReceive(p_i2c_info);
        switch (i2c_err)
        {
            case RIIC_SUCCESS:
                err = FSP_SUCCESS;
                break;
            case RIIC_ERR_INVALID_CHAN:
                err = FSP_ERR_INVALID_CHANNEL;
                break;
            case RIIC_ERR_INVALID_ARG:
                err = FSP_ERR_INVALID_ARGUMENT;
                break;
            case RIIC_ERR_NO_INIT:
                err = FSP_ERR_NOT_OPEN;
                break;
            case RIIC_ERR_BUS_BUSY:
                err = FSP_ERR_IN_USE;
                break;
            case RIIC_ERR_AL:
            case RIIC_ERR_TMO:
            case RIIC_ERR_OTHER:
            default:
                err = FSP_ERR_ASSERTION;
                break;
        }
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif        
    }
    else if (COMMS_DRIVER_SCI_I2C == p_driver_instance->driver_type)
    {
#if (COMMS_I2C_CFG_DRIVER_SCI_I2C)
        sci_iic_return_t i2c_err;
        sci_iic_info_t * p_i2c_info = (sci_iic_info_t *)p_driver_instance->p_info;

        /* Set address data */
        p_i2c_info->p_data1st = FIT_NO_PTR;
        p_i2c_info->cnt1st    = 0U;

        /* Set transfer data */
        if (0 < bytes)
        {
            p_i2c_info->p_data2nd = p_dest;
            p_i2c_info->cnt2nd    = bytes;
        }
        else
        {
            p_i2c_info->p_data2nd = FIT_NO_PTR;
            p_i2c_info->cnt2nd    = 0;
        }

        /* Read data */
        i2c_err = R_SCI_IIC_MasterReceive(p_i2c_info);
        switch (i2c_err)
        {
            case SCI_IIC_SUCCESS:
                err = FSP_SUCCESS;
                break;
            case SCI_IIC_ERR_INVALID_CHAN:
                err = FSP_ERR_INVALID_CHANNEL;
                break;
            case SCI_IIC_ERR_INVALID_ARG:
                err = FSP_ERR_INVALID_ARGUMENT;
                break;
            case SCI_IIC_ERR_NO_INIT:
                err = FSP_ERR_NOT_OPEN;
                break;
            case SCI_IIC_ERR_BUS_BUSY:
                err = FSP_ERR_IN_USE;
                break;
            case SCI_IIC_ERR_OTHER:
            default:
                err = FSP_ERR_ASSERTION;
                break;
        }
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif 
    }

#if BSP_CFG_RTOS
    if (NULL != p_ctrl->p_bus->p_blocking_semaphore)
    {
        /* Acquire a semaphore for blocking */
        err = rm_comms_i2c_os_semaphore_acquire(p_ctrl->p_bus->p_blocking_semaphore, p_ctrl->p_cfg->semaphore_timeout);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
    if (NULL != p_ctrl->p_bus->p_bus_recursive_mutex)
    {
        /* Release a mutex for bus */
    	err = rm_comms_i2c_os_recursive_mutex_release(p_ctrl->p_bus->p_bus_recursive_mutex);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
#endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Function Name: comms_i2c_driver_write
 * @brief Write data to the device using the I2C driver module.
 * @param[in]  p_api_ctrl           Pointer to instance control structure
 *             p_src                Pointer to source buffer
 *             bytes                Number of source data
 * @retval FSP_SUCCESS              Successfully started.
 ***********************************************************************************************************************/
fsp_err_t rm_comms_i2c_bus_write(rm_comms_ctrl_t * const p_api_ctrl, uint8_t * const p_src, uint32_t const bytes)
{
    fsp_err_t err = FSP_SUCCESS;
    rm_comms_i2c_instance_ctrl_t * p_ctrl            = (rm_comms_i2c_instance_ctrl_t *) p_api_ctrl;
    i2c_driver_instance_t        * p_driver_instance = (i2c_driver_instance_t *) p_ctrl->p_bus->p_driver_instance;

    /* Reconfigure a bus */
    err = rm_comms_i2c_bus_reconfigure(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    if (COMMS_DRIVER_I2C == p_driver_instance->driver_type)
    {
#if (COMMS_I2C_CFG_DRIVER_I2C)
        riic_return_t i2c_err;
        riic_info_t * p_i2c_info = (riic_info_t *)p_driver_instance->p_info;

        /* Set address data */
        p_i2c_info->p_data1st = FIT_NO_PTR;
        p_i2c_info->cnt1st    = 0U;

        /* Set transfer data */
        if (0 < bytes)
        {
            p_i2c_info->p_data2nd = p_src;
            p_i2c_info->cnt2nd    = bytes;
        }
        else
        {
            p_i2c_info->p_data2nd = FIT_NO_PTR;
            p_i2c_info->cnt2nd    = 0;
        }

        /* Write data */
        i2c_err = R_RIIC_MasterSend(p_i2c_info);
        switch (i2c_err)
        {
            case RIIC_SUCCESS:
                err = FSP_SUCCESS;
                break;
            case RIIC_ERR_INVALID_CHAN:
                err = FSP_ERR_INVALID_CHANNEL;
                break;
            case RIIC_ERR_INVALID_ARG:
                err = FSP_ERR_INVALID_ARGUMENT;
                break;
            case RIIC_ERR_NO_INIT:
                err = FSP_ERR_NOT_OPEN;
                break;
            case RIIC_ERR_BUS_BUSY:
                err = FSP_ERR_IN_USE;
                break;
            case RIIC_ERR_AL:
            case RIIC_ERR_TMO:
            case RIIC_ERR_OTHER:
            default:
                err = FSP_ERR_ASSERTION;
                break;
        }
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif        
    }
    else if (COMMS_DRIVER_SCI_I2C == p_driver_instance->driver_type)
    {
#if (COMMS_I2C_CFG_DRIVER_SCI_I2C)
        sci_iic_return_t i2c_err;
        sci_iic_info_t * p_i2c_info = (sci_iic_info_t *)p_driver_instance->p_info;

        /* Set address data */
        p_i2c_info->p_data1st = FIT_NO_PTR;
        p_i2c_info->cnt1st    = 0U;

        /* Set transfer data */
        if (0 < bytes)
        {
            p_i2c_info->p_data2nd = p_src;
            p_i2c_info->cnt2nd    = bytes;
        }
        else
        {
            p_i2c_info->p_data2nd = FIT_NO_PTR;
            p_i2c_info->cnt2nd    = 0;
        }

        /* Write data */
        i2c_err = R_SCI_IIC_MasterSend(p_i2c_info);
        switch (i2c_err)
        {
            case SCI_IIC_SUCCESS:
                err = FSP_SUCCESS;
                break;
            case SCI_IIC_ERR_INVALID_CHAN:
                err = FSP_ERR_INVALID_CHANNEL;
                break;
            case SCI_IIC_ERR_INVALID_ARG:
                err = FSP_ERR_INVALID_ARGUMENT;
                break;
            case SCI_IIC_ERR_NO_INIT:
                err = FSP_ERR_NOT_OPEN;
                break;
            case SCI_IIC_ERR_BUS_BUSY:
                err = FSP_ERR_IN_USE;
                break;
            case SCI_IIC_ERR_OTHER:
            default:
                err = FSP_ERR_ASSERTION;
                break;
        }
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif 
    }

#if BSP_CFG_RTOS
    if (NULL != p_ctrl->p_bus->p_blocking_semaphore)
    {
        /* Acquire a semaphore for blocking */
        err = rm_comms_i2c_os_semaphore_acquire(p_ctrl->p_bus->p_blocking_semaphore, p_ctrl->p_cfg->semaphore_timeout);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
    if (NULL != p_ctrl->p_bus->p_bus_recursive_mutex)
    {
        /* Release a mutex for bus */
    	err = rm_comms_i2c_os_recursive_mutex_release(p_ctrl->p_bus->p_bus_recursive_mutex);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
#endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Function Name: rm_comms_i2c_driver_write_read
 * @brief Writes to I2C bus, then reads with restart.
 * @param[in]  p_api_ctrl           Pointer to instance control structure
 *             write_read_params    Write / read buffer structure
 * @retval FSP_SUCCESS              Successfully started.
 ***********************************************************************************************************************/
fsp_err_t rm_comms_i2c_bus_write_read (rm_comms_ctrl_t * const p_api_ctrl, rm_comms_write_read_params_t write_read_params)
{
    fsp_err_t err = FSP_SUCCESS;
    rm_comms_i2c_instance_ctrl_t * p_ctrl            = (rm_comms_i2c_instance_ctrl_t *) p_api_ctrl;
    i2c_driver_instance_t        * p_driver_instance = (i2c_driver_instance_t *) p_ctrl->p_bus->p_driver_instance;

    /* Reconfigure a bus */
    err = rm_comms_i2c_bus_reconfigure(p_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    if (COMMS_DRIVER_I2C == p_driver_instance->driver_type)
    {
#if (COMMS_I2C_CFG_DRIVER_I2C)
        riic_return_t i2c_err;
        riic_info_t * p_i2c_info = (riic_info_t *)p_driver_instance->p_info;

        /* Set address data */
        if (0 < write_read_params.src_bytes)
        {
            p_i2c_info->p_data1st = write_read_params.p_src;
            p_i2c_info->cnt1st    = write_read_params.src_bytes;
        }
        else
        {
            p_i2c_info->p_data1st = FIT_NO_PTR;
            p_i2c_info->cnt1st    = 0;
        }

        /* Set transfer data */
        if (0 < write_read_params.dest_bytes)
        {
            p_i2c_info->p_data2nd = write_read_params.p_dest;
            p_i2c_info->cnt2nd    = write_read_params.dest_bytes;
        }
        else
        {
            p_i2c_info->p_data2nd = FIT_NO_PTR;
            p_i2c_info->cnt2nd    = 0;
        }

        /* Read data */
        i2c_err = R_RIIC_MasterReceive(p_i2c_info);
        switch (i2c_err)
        {
            case RIIC_SUCCESS:
                err = FSP_SUCCESS;
                break;
            case RIIC_ERR_INVALID_CHAN:
                err = FSP_ERR_INVALID_CHANNEL;
                break;
            case RIIC_ERR_INVALID_ARG:
                err = FSP_ERR_INVALID_ARGUMENT;
                break;
            case RIIC_ERR_NO_INIT:
                err = FSP_ERR_NOT_OPEN;
                break;
            case RIIC_ERR_BUS_BUSY:
                err = FSP_ERR_IN_USE;
                break;
            case RIIC_ERR_AL:
            case RIIC_ERR_TMO:
            case RIIC_ERR_OTHER:
            default:
                err = FSP_ERR_ASSERTION;
                break;
        }
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif        
    }
    else if (COMMS_DRIVER_SCI_I2C == p_driver_instance->driver_type)
    {
#if (COMMS_I2C_CFG_DRIVER_SCI_I2C)
        sci_iic_return_t i2c_err;
        sci_iic_info_t * p_i2c_info = (sci_iic_info_t *)p_driver_instance->p_info;

        /* Set address data */
        if (0 < write_read_params.src_bytes)
        {
            p_i2c_info->p_data1st = write_read_params.p_src;
            p_i2c_info->cnt1st    = write_read_params.src_bytes;
        }
        else
        {
            p_i2c_info->p_data1st = FIT_NO_PTR;
            p_i2c_info->cnt1st    = 0;
        }

        /* Set transfer data */
        if (0 < write_read_params.dest_bytes)
        {
            p_i2c_info->p_data2nd = write_read_params.p_dest;
            p_i2c_info->cnt2nd    = write_read_params.dest_bytes;
        }
        else
        {
            p_i2c_info->p_data2nd = FIT_NO_PTR;
            p_i2c_info->cnt2nd    = 0;
        }

        /* Read data */
        i2c_err = R_SCI_IIC_MasterReceive(p_i2c_info);
        switch (i2c_err)
        {
            case SCI_IIC_SUCCESS:
                err = FSP_SUCCESS;
                break;
            case SCI_IIC_ERR_INVALID_CHAN:
                err = FSP_ERR_INVALID_CHANNEL;
                break;
            case SCI_IIC_ERR_INVALID_ARG:
                err = FSP_ERR_INVALID_ARGUMENT;
                break;
            case SCI_IIC_ERR_NO_INIT:
                err = FSP_ERR_NOT_OPEN;
                break;
            case SCI_IIC_ERR_BUS_BUSY:
                err = FSP_ERR_IN_USE;
                break;
            case SCI_IIC_ERR_OTHER:
            default:
                err = FSP_ERR_ASSERTION;
                break;
        }
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif 
    }

#if BSP_CFG_RTOS
    if (NULL != p_ctrl->p_bus->p_blocking_semaphore)
    {
        /* Acquire a semaphore for blocking */
        err = rm_comms_i2c_os_semaphore_acquire(p_ctrl->p_bus->p_blocking_semaphore, p_ctrl->p_cfg->semaphore_timeout);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
    if (NULL != p_ctrl->p_bus->p_bus_recursive_mutex)
    {
        /* Release a mutex for bus */
    	err = rm_comms_i2c_os_recursive_mutex_release(p_ctrl->p_bus->p_bus_recursive_mutex);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
#endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Function Name: rm_comms_i2c_callback
 * @brief Common callback function called in the I2C driver callback function.
 * @param[in]  p_args                      Pointer to I2C callback arguments structure
 * @param[in]  p_api_ctrl                  Pointer to instance control structure
 ***********************************************************************************************************************/
void rm_comms_i2c_callback (rm_comms_ctrl_t const * p_api_ctrl)
{
    rm_comms_i2c_instance_ctrl_t * p_ctrl     = (rm_comms_i2c_instance_ctrl_t *) p_api_ctrl;
    rm_comms_callback_args_t       comms_i2c_args;

    /* Set event */
    comms_i2c_args.event = rm_comms_i2c_bus_callbackErrorCheck(p_ctrl);

    /* Release semaphore in OS or call user callback function */
    rm_comms_i2c_process_in_callback(p_ctrl, &comms_i2c_args);
}

/*******************************************************************************************************************//**
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Internal Communications Middleware I2C driver private function.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Function Name: rm_comms_i2c_process_in_callback
 * @brief Process in callback function. Release semaphores in RTOS and call user callback.
 * @param[in] p_api_ctrl            Pointer to instance control structure
 *            p_args                Pointer to callback arguments structure
 * @retval FSP_SUCCESS              Successfully started.
 ***********************************************************************************************************************/
 static void rm_comms_i2c_process_in_callback (rm_comms_ctrl_t * const          p_api_ctrl,
                                              rm_comms_callback_args_t * const p_args)
{
    rm_comms_i2c_instance_ctrl_t * p_ctrl = (rm_comms_i2c_instance_ctrl_t *) p_api_ctrl;
#if BSP_CFG_RTOS
    if (NULL != p_ctrl->p_bus->p_blocking_semaphore)
    {
        /* Release a semaphore for blocking */
    	rm_comms_i2c_os_semaphore_release_from_ISR(p_ctrl->p_bus->p_blocking_semaphore);
    }
#endif
    if (NULL != p_ctrl->p_callback)
    {
        /* Call user callback */
        p_ctrl->p_callback(p_args);
    }
}

/*******************************************************************************************************************//**
 * Function Name: rm_comms_i2c_bus_callbackErrorCheck
 * @brief Check error in the callback.
 * @param[in]  p_api_ctrl                  Pointer to instance control structure
 * @retval FSP_SUCCESS              Successfully started.
 ***********************************************************************************************************************/
static rm_comms_event_t rm_comms_i2c_bus_callbackErrorCheck(rm_comms_ctrl_t const * p_api_ctrl)
{
    rm_comms_event_t event = RM_COMMS_EVENT_OPERATION_COMPLETE;
    rm_comms_i2c_instance_ctrl_t * p_ctrl            = (rm_comms_i2c_instance_ctrl_t *) p_api_ctrl;
    i2c_driver_instance_t        * p_driver_instance = (i2c_driver_instance_t *) p_ctrl->p_bus->p_driver_instance;
    
    if (COMMS_DRIVER_I2C == p_driver_instance->driver_type)
    {
#if (COMMS_I2C_CFG_DRIVER_I2C)
        riic_info_t * p_i2c_info = (riic_info_t *)p_driver_instance->p_info;

        if (RIIC_FINISH != p_i2c_info->dev_sts)
        {
            event = RM_COMMS_EVENT_ERROR;
        }
        FSP_ERROR_RETURN(RM_COMMS_EVENT_OPERATION_COMPLETE == event, event);
#endif 
    }
    else if (COMMS_DRIVER_SCI_I2C == p_driver_instance->driver_type)
    {
#if (COMMS_I2C_CFG_DRIVER_SCI_I2C)
        sci_iic_info_t * p_i2c_info = (sci_iic_info_t *)p_driver_instance->p_info;

        if (SCI_IIC_FINISH != p_i2c_info->dev_sts)
        {
            event = RM_COMMS_EVENT_ERROR;
        }
        FSP_ERROR_RETURN(RM_COMMS_EVENT_OPERATION_COMPLETE == event, event);
#endif 
    }

    return event;
}

/*******************************************************************************************************************//**
 * @brief Reconfigure I2C bus.
 *
 * @retval FSP_SUCCESS              successfully configured.
 **********************************************************************************************************************/
static fsp_err_t rm_comms_i2c_bus_reconfigure (rm_comms_ctrl_t * const p_api_ctrl)
{
#if BSP_CFG_RTOS
    fsp_err_t                         err               = FSP_SUCCESS;
#endif
    rm_comms_i2c_instance_ctrl_t    * p_ctrl            = (rm_comms_i2c_instance_ctrl_t *) p_api_ctrl;
    rm_comms_i2c_bus_extended_cfg_t * p_extend          = (rm_comms_i2c_bus_extended_cfg_t*) p_ctrl->p_cfg->p_extend;
    i2c_driver_instance_t           * p_driver          = (i2c_driver_instance_t *) p_extend->p_driver_instance;
    rm_comms_i2c_device_cfg_t       * p_lower_level_cfg = (rm_comms_i2c_device_cfg_t *) p_ctrl->p_cfg->p_lower_level_cfg;

#if BSP_CFG_RTOS
    if (NULL != p_ctrl->p_bus->p_bus_recursive_mutex)
    {
        /* Acquire a mutex */
        err = rm_comms_i2c_os_recursive_mutex_acquire(p_ctrl->p_bus->p_bus_recursive_mutex, p_ctrl->p_bus->bus_timeout);
        FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    }
#endif

    if (COMMS_DRIVER_I2C == p_driver->driver_type)
    {
#if (COMMS_I2C_CFG_DRIVER_I2C)
        if (p_ctrl->p_bus->p_current_ctrl != p_ctrl)
        {
            riic_info_t * p_info = (riic_info_t *) p_driver->p_info;

            /* Update a slave address */
            *p_info->p_slv_adr = (uint8_t) p_lower_level_cfg->slave_address;

            /* Update control block on bus */
            p_ctrl->p_bus->p_current_ctrl = (rm_comms_ctrl_t *) p_ctrl;

            /* Set callback function and current control block */
            p_info->callbackfunc = p_driver->callback;
        }
#endif
    }
    else if (COMMS_DRIVER_SCI_I2C == p_driver->driver_type)
    {
#if (COMMS_I2C_CFG_DRIVER_SCI_I2C)
        if (p_ctrl->p_bus->p_current_ctrl != p_ctrl)
        {
            sci_iic_info_t * p_info = (sci_iic_info_t *) p_driver->p_info;

            /* Update a slave address */
            *p_info->p_slv_adr = (uint8_t) p_lower_level_cfg->slave_address;

            /* Update control block on bus */
            p_ctrl->p_bus->p_current_ctrl = (rm_comms_ctrl_t *) p_ctrl;

            /* Set callback function and current control block */
            p_info->callbackfunc = p_driver->callback;
        }
#endif
    }

    return FSP_SUCCESS;
}

#if BSP_CFG_RTOS

/*******************************************************************************************************************//**
 * @brief Acquire a recursive mutex.
 *
 * @retval FSP_SUCCESS              successfully configured.
 * @retval FSP_ERR_INTERNAL         RTOS internal error.
 *********************************************************************************************************************/
static fsp_err_t rm_comms_i2c_os_recursive_mutex_acquire (rm_comms_i2c_mutex_t const * p_mutex, uint32_t const timeout)
{
 #if BSP_CFG_RTOS == 1                 // ThreadX
    UINT status;
    status = tx_mutex_get(p_mutex->p_mutex_handle, (ULONG) timeout);
    FSP_ERROR_RETURN(TX_SUCCESS == status, FSP_ERR_INTERNAL);
 #elif BSP_CFG_RTOS == 2               // FreeRTOS
    BaseType_t sem_err;
    sem_err = xSemaphoreTake(*(p_mutex->p_mutex_handle), (TickType_t) timeout);
    FSP_ERROR_RETURN(pdTRUE == sem_err, FSP_ERR_INTERNAL);
 #endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Release a recursive mutex.
 *
 * @retval FSP_SUCCESS              successfully configured.
 * @retval FSP_ERR_INTERNAL         RTOS internal error.
 *********************************************************************************************************************/
static fsp_err_t rm_comms_i2c_os_recursive_mutex_release (rm_comms_i2c_mutex_t const * p_mutex)
{
 #if BSP_CFG_RTOS == 1                 // ThreadX
    UINT status;
    status = tx_mutex_put(p_mutex->p_mutex_handle);
    FSP_ERROR_RETURN(TX_SUCCESS == status, FSP_ERR_INTERNAL);
 #elif BSP_CFG_RTOS == 2               // FreeRTOS
    BaseType_t sem_err;
    sem_err = xSemaphoreGive(*(p_mutex->p_mutex_handle));
    FSP_ERROR_RETURN(pdTRUE == sem_err, FSP_ERR_INTERNAL);
 #endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Acquire a semaphore.
 *
 * @retval FSP_SUCCESS              successfully configured.
 * @retval FSP_ERR_INTERNAL         RTOS internal error.
 *********************************************************************************************************************/
static fsp_err_t rm_comms_i2c_os_semaphore_acquire (rm_comms_i2c_semaphore_t const * p_semaphore,
                                                    uint32_t const                   timeout)
{
 #if BSP_CFG_RTOS == 1                 // ThreadX
    UINT status;
    status = tx_semaphore_get(p_semaphore->p_semaphore_handle, (ULONG) timeout);
    FSP_ERROR_RETURN(TX_SUCCESS == status, FSP_ERR_INTERNAL);
 #elif BSP_CFG_RTOS == 2               // FreeRTOS
    BaseType_t sem_err;
    sem_err = xSemaphoreTake(*(p_semaphore->p_semaphore_handle), (TickType_t) timeout);
    FSP_ERROR_RETURN(pdTRUE == sem_err, FSP_ERR_INTERNAL);
 #endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Release a semaphore from an interrupt.
 *
 * @retval FSP_SUCCESS              successfully configured.
 * @retval FSP_ERR_INTERNAL         RTOS internal error.
 *********************************************************************************************************************/
static fsp_err_t rm_comms_i2c_os_semaphore_release_from_ISR (rm_comms_i2c_semaphore_t const * p_semaphore)
{
 #if BSP_CFG_RTOS == 1                 // ThreadX
    UINT status;
    status = tx_semaphore_put(p_semaphore->p_semaphore_handle);
    FSP_ERROR_RETURN(TX_SUCCESS == status, FSP_ERR_INTERNAL);
 #elif BSP_CFG_RTOS == 2               // FreeRTOS
    BaseType_t sem_err;
    sem_err = xSemaphoreGiveFromISR(*(p_semaphore->p_semaphore_handle), NULL);
    FSP_ERROR_RETURN(pdTRUE == sem_err, FSP_ERR_INTERNAL);
 #endif

    return FSP_SUCCESS;
}

#endif
