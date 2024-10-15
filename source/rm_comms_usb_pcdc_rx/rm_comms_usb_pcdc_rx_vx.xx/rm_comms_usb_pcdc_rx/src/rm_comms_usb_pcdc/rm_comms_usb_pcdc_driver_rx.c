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
* Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "rm_comms_usb_pcdc.h"
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

/*******************************************************************************************************************//**
 * Private global variables
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Global variables
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Functions
 **********************************************************************************************************************/
fsp_err_t rm_comms_usb_pcdc_bus_open ( usb_ctrl_t *p_ctrl, usb_cfg_t *p_cfg );
fsp_err_t rm_comms_usb_pcdc_bus_close (usb_ctrl_t *p_ctrl);
fsp_err_t rm_comms_usb_pcdc_bus_read (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size);
fsp_err_t rm_comms_usb_pcdc_bus_write (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size);
fsp_err_t rm_comms_usb_pcdc_timer_open (cmt_instance_t *p_cmt);
fsp_err_t rm_comms_usb_pcdc_timer_start (cmt_instance_t *p_cmt);
fsp_err_t rm_comms_usb_pcdc_timer_close (cmt_instance_t *p_cmt);
/*******************************************************************************************************************//**
 * Function Name: rm_comms_usb_pcdc_bus_open
 * @brief Synchronize the return value between USB PCDC Middle-ware and Driver.
 * @retval FSP_SUCCESS
 **********************************************************************************************************************/
fsp_err_t rm_comms_usb_pcdc_bus_open ( usb_ctrl_t *p_ctrl, usb_cfg_t *p_cfg )
{
    fsp_err_t err;
    usb_err_t usb_err;

    /* Open USB */
    usb_err = R_USB_Open(p_ctrl, p_cfg);
    switch (usb_err)
    {
        case USB_SUCCESS:
            err = FSP_SUCCESS;
            break;
        case USB_ERR_PARA:
            err = FSP_ERR_USB_PARAMETER;
            break;
        case USB_ERR_BUSY:
            err = FSP_ERR_USB_BUSY;
            break;
        case USB_ERR_NOT_OPEN:
            err = FSP_ERR_USB_NOT_OPEN;
            break;
        case USB_ERR_NOT_SUSPEND:
            err = FSP_ERR_USB_NOT_SUSPEND;
            break;
        case USB_ERR_OVER:
            err = FSP_ERR_USB_SIZE_OVER;
            break;
        case USB_ERR_SHORT:
            err = FSP_ERR_USB_SIZE_SHORT;
            break;
        case USB_ERR_NG:
#if COMMS_USB_PCDC_DEVICE_USED == USB_PCDC_BASIC
        case USB_ERR_TMOUT:
#endif
            err = FSP_ERR_USB_FAILED;
            break;
        default:
            break;
    }
    return err;
}

/*******************************************************************************************************************//**
 * Function Name: rm_comms_usb_pcdc_bus_close
 * @brief Synchronize the return value between USB PCDC Middle-ware and Driver.
 * @retval FSP_SUCCESS
 **********************************************************************************************************************/
fsp_err_t rm_comms_usb_pcdc_bus_close (usb_ctrl_t *p_ctrl)
{
    fsp_err_t err;
    usb_err_t usb_err;

    /* Close USB */
    usb_err = R_USB_Close(p_ctrl);
    switch (usb_err)
    {
        case USB_SUCCESS:
            err = FSP_SUCCESS;
            break;
        case USB_ERR_PARA:
            err = FSP_ERR_USB_PARAMETER;
            break;
        case USB_ERR_BUSY:
            err = FSP_ERR_USB_BUSY;
            break;
        case USB_ERR_NOT_OPEN:
            err = FSP_ERR_USB_NOT_OPEN;
            break;
        case USB_ERR_NOT_SUSPEND:
            err = FSP_ERR_USB_NOT_SUSPEND;
            break;
        case USB_ERR_OVER:
            err = FSP_ERR_USB_SIZE_OVER;
            break;
        case USB_ERR_SHORT:
            err = FSP_ERR_USB_SIZE_SHORT;
            break;
        case USB_ERR_NG:
#if COMMS_USB_PCDC_DEVICE_USED == USB_PCDC_BASIC
        case USB_ERR_TMOUT:
#endif
            err = FSP_ERR_USB_FAILED;
            break;
        default:
            break;
    }
    return err;
}

/*******************************************************************************************************************//**
 * Function Name: rm_comms_usb_pcdc_bus_read
 * @brief Synchronize the return value between USB PCDC Middle-ware and Driver.
 * @retval FSP_SUCCESS
 **********************************************************************************************************************/
fsp_err_t rm_comms_usb_pcdc_bus_read (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size)
{
    fsp_err_t err;
    usb_err_t usb_err;

    /* Read data */
    p_ctrl->type = USB_PCDC;
    usb_err = R_USB_Read(p_ctrl, p_buf, size);
    switch (usb_err)
    {
        case USB_SUCCESS:
            err = FSP_SUCCESS;
            break;
        case USB_ERR_PARA:
            err = FSP_ERR_USB_PARAMETER;
            break;
        case USB_ERR_BUSY:
            err = FSP_ERR_USB_BUSY;
            break;
        case USB_ERR_NOT_OPEN:
            err = FSP_ERR_USB_NOT_OPEN;
            break;
        case USB_ERR_NOT_SUSPEND:
            err = FSP_ERR_USB_NOT_SUSPEND;
            break;
        case USB_ERR_OVER:
            err = FSP_ERR_USB_SIZE_OVER;
            break;
        case USB_ERR_SHORT:
            err = FSP_ERR_USB_SIZE_SHORT;
            break;
        case USB_ERR_NG:
#if COMMS_USB_PCDC_DEVICE_USED == USB_PCDC_BASIC
        case USB_ERR_TMOUT:
#endif
            err = FSP_ERR_USB_FAILED;
            break;
        default:
            break;
    }
    return err;
}

/*******************************************************************************************************************//**
 * Function Name: rm_comms_usb_pcdc_bus_write
 * @brief Synchronize the return value between USB PCDC Middle-ware and Driver.
 * @retval FSP_SUCCESS
 **********************************************************************************************************************/
fsp_err_t rm_comms_usb_pcdc_bus_write (usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint32_t size)
{
    fsp_err_t err;
    usb_err_t usb_err;

    /* Write data */
    p_ctrl->type = USB_PCDC;
    usb_err = R_USB_Write(p_ctrl, p_buf, size);
    switch (usb_err)
    {
        case USB_SUCCESS:
            err = FSP_SUCCESS;
            break;
        case USB_ERR_PARA:
            err = FSP_ERR_USB_PARAMETER;
            break;
        case USB_ERR_BUSY:
            err = FSP_ERR_USB_BUSY;
            break;
        case USB_ERR_NOT_OPEN:
            err = FSP_ERR_USB_NOT_OPEN;
            break;
        case USB_ERR_NOT_SUSPEND:
            err = FSP_ERR_USB_NOT_SUSPEND;
            break;
        case USB_ERR_OVER:
            err = FSP_ERR_USB_SIZE_OVER;
            break;
        case USB_ERR_SHORT:
            err = FSP_ERR_USB_SIZE_SHORT;
            break;
        case USB_ERR_NG:
#if COMMS_USB_PCDC_DEVICE_USED == USB_PCDC_BASIC
        case USB_ERR_TMOUT:
#endif
            err = FSP_ERR_USB_FAILED;
            break;
        default:
            break;
    }
    return err;
}

/*******************************************************************************************************************//**
 * Function Name: rm_comms_usb_pcdc_timer_open
 * @brief Setting timer for rm_comms_usb_pcdc
 * @retval FSP_SUCCESS
 **********************************************************************************************************************/
fsp_err_t rm_comms_usb_pcdc_timer_open (cmt_instance_t *p_cmt)
{
    bool ret = false;
    /* Open timer */
    ret = R_CMT_CreatePeriodicAssignChannelPriority(p_cmt->freqency_hz,
                                                    rm_comms_usb_pcdc_timer_handler,
                                                    p_cmt->channel,
                                                    p_cmt->priority);

    FSP_ERROR_RETURN(true == ret, FSP_ERR_ASSERTION);

    /* Pause the timer */
    ret = R_CMT_Control(p_cmt->channel,
                        CMT_RX_CMD_PAUSE,
                        NULL);
    FSP_ERROR_RETURN(true == ret, FSP_ERR_ASSERTION);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Function Name: rm_comms_usb_pcdc_timer_start
 * @brief Start timer
 * @retval FSP_SUCCESS
 **********************************************************************************************************************/
fsp_err_t rm_comms_usb_pcdc_timer_start (cmt_instance_t *p_cmt)
{
    bool ret = false;
    /* Start timer */
    ret = R_CMT_Control(p_cmt->channel,
                        CMT_RX_CMD_RESTART, 
                        NULL);
    FSP_ERROR_RETURN(true == ret, FSP_ERR_ASSERTION);
    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Function Name: rm_comms_usb_pcdc_timer_close
 * @brief Close timer
 * @retval FSP_SUCCESS
 **********************************************************************************************************************/
fsp_err_t rm_comms_usb_pcdc_timer_close (cmt_instance_t *p_cmt)
{
    bool ret = false;
    /* Stop timer */
    ret = R_CMT_Stop(p_cmt->channel);
    FSP_ERROR_RETURN(true == ret, FSP_ERR_ASSERTION);
    return FSP_SUCCESS;
}
