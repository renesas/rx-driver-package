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
 * Copyright (C) 2014(2020) Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_usb_hmsc_api.c
 * Description  : USB Host MSC Driver API
 ***********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 08.01.2014 1.00 First Release
 *         : 26.12.2014 1.10 RX71M is added
 *         : 30.09.2015 1.11 RX63N/RX631 is added.
 *         : 30.09.2016 1.20 RX65N/RX651 is added.
 *         : 30.09.2017 1.22 Support USB_ATAPI_MODE_SELECT10, Non support MODE_SELECT6.
 *                           Change Arguments for "R_USB_HmscDriverStart()","usb_hmsc_mode_sense10()"
 *         : 31.03.2018 1.23 Supporting Smart Configurator
 *         : 01.03.2020 1.30 RX72N/RX66N is added and uITRON is supported.
************************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
************************************************************************************************************************/

#include "r_usb_basic_if.h"
#include "r_usb_typedef.h"
#include "r_usb_hmsc_if.h"
#include "r_usb_extern.h"
#include "r_usb_hmsc.h"

#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
#include "r_rtos_abstract.h"
#include "r_usb_cstd_rtos.h"
#endif /* (BSP_CFG_RTOS_USED != 0) */

/**********************************************************************************************************************
 Exported global variables (to be accessed by other files)
************************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name   : R_USB_HmscStrgCmd
 *******************************************************************************************************************//**
 * @brief  Issues a Mass Storage command
 * @param[in]      p_ctrl      Pointer to usb_ctrl_t structure area
 * @param[in, out] p_buf         Pointer to data area
 * @param[in]      command     Mass storage command
 * @retval     USB_SUCCESS     Successfully completed
 * @retval     USB_ERR_PARA    Parameter error
 * @retval     USB_ERR_NG      Other error
 * @details The Mass Storage command assigned to the argument (command) is issued to the MSC device that is specifed 
 * by the members (address and module) in the argument (p_ctrl).
 * @note The argument (p_ctrl) is not supported when using RX100/RX200 series MCU.
 */
usb_err_t   R_USB_HmscStrgCmd(usb_ctrl_t *p_ctrl, uint8_t *p_buf, uint16_t command)
{
    usb_err_t   err;
    usb_info_t  info;
    usb_utr_t   utr;
    uint8_t     side;
    uint16_t    ret;

    utr.ip = p_ctrl->module;
    utr.ipp = usb_hstd_get_usb_ip_adr(utr.ip);

#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
    if (USB_NULL == p_ctrl)
    {
        return USB_ERR_PARA;
    }

    /* Argument Checking */
    if( (USB_IP0 != p_ctrl->module) && (USB_IP1 != p_ctrl->module) )
    {
        return USB_ERR_PARA;
    }

#if defined(BSP_MCU_RX65N) || defined(BSP_MCU_RX63T) || defined(BSP_MCU_RX63N)
    if( USB_IP1 == p_ctrl->module)
    {
        return USB_ERR_PARA;
    }
#endif /* defined(BSP_MCU_RX65N) || defined(BSP_MCU_RX63T) || defined(BSP_MCU_RX63N) */

#endif /* #if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */

    err = R_USB_GetInformation(p_ctrl, &info);
    if (USB_STS_CONFIGURED != info.status)
    {
        return USB_ERR_NG;
    }

    err = R_USB_HmscGetDriveNo(p_ctrl, &side);
    if (USB_SUCCESS != err)
    {
        return USB_ERR_NG;
    }

    ret = usb_hmsc_strg_user_command(&utr, side, command, p_buf, usb_hmsc_strg_cmd_complete);
    if (USB_PAR == ret)
    {
#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
        return USB_ERR_PARA;
#endif /* #if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */
    }
    if (USB_OK != ret)
    {
#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
        if (USB_HMSC_CSW_ERR == utr.result)
        {
            p_ctrl->status  = USB_CSW_FAIL;
            p_ctrl->module  = utr.ip;       /* Module number setting */
            p_ctrl->pipe    = utr.keyword;  /* Pipe number setting */
            p_ctrl->address = usb_hstd_get_devsel(&utr, p_ctrl->pipe) >> 12;
            p_ctrl->size = 0;
#if (BSP_CFG_RTOS_USED == 1)                /* FreeRTOS */
            p_ctrl->p_data = (void *)xTaskGetCurrentTaskHandle();
#endif /* (BSP_CFG_RTOS_USED == 1) */
            usb_set_event(USB_STS_MSC_CMD_COMPLETE, p_ctrl); /* Set Event(USB receive complete)  */
            return USB_SUCCESS;
        }
        else
        {
            return USB_ERR_NG;
        }
#else  /* (BSP_CFG_RTOS_USED != 0) */
        return USB_ERR_NG;
#endif /* (BSP_CFG_RTOS_USED != 0) */
    }

#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
    else
    {
        p_ctrl->module  = utr.ip;       /* Module number setting */
        p_ctrl->pipe    = utr.keyword;  /* Pipe number setting */
        p_ctrl->address = usb_hstd_get_devsel(&utr, p_ctrl->pipe) >> 12;
        p_ctrl->size = 0;

        switch (utr.result)
        {
            case USB_HMSC_OK:
                p_ctrl->size    = utr.tranlen;
                p_ctrl->status  = USB_CSW_SUCCESS;
            break;

            case USB_HMSC_CSW_ERR:
                p_ctrl->status  = USB_CSW_FAIL;
            break;

            case USB_HMSC_CSW_PHASE_ERR:
                p_ctrl->status  = USB_CSW_PHASE;
            break;

            default:
                p_ctrl->status  = USB_CSW_FAIL;
            break;
        }
#if (BSP_CFG_RTOS_USED == 1)             /* FreeRTOS */
        p_ctrl->p_data = (void *)xTaskGetCurrentTaskHandle();
#endif /* (BSP_CFG_RTOS_USED == 1) */
        usb_set_event(USB_STS_MSC_CMD_COMPLETE, p_ctrl); /* Set Event(USB receive complete)  */
    }
#endif  /* (BSP_CFG_RTOS_USED != 0) */

    return USB_SUCCESS;

}
/**********************************************************************************************************************
 End of function R_USB_HmscStrgCmd
************************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name   : R_USB_HmscGetDriveNo
 *******************************************************************************************************************//**
 * @brief  Obtains the drive number
 * @param[in]      p_ctrl          Pointer to usb_ctrl_t structure area
 * @param[out]     p_drive         Pointer to the area to store the drive number
 * @retval         USB_SUCCESS     Successfully completed
 * @retval         USB_ERR_PARA    Parameter error
 * @retval         USB_ERR_NG      Other error
 * @details Based on the information assigned to the usb_crtl_t structure (the member module and address), 
 * obtains the related drive number. The drive number is stored in the area indicated by the argument (p_drive).
 * @note This API is not supported when using RX100/RX200 series MCU.
 */
usb_err_t     R_USB_HmscGetDriveNo(usb_ctrl_t *p_ctrl, uint8_t *p_drive)
{
    usb_info_t  info;
    usb_utr_t   utr;
    volatile uint16_t   address;

    utr.ip = p_ctrl->module;
    utr.ipp = usb_hstd_get_usb_ip_adr(utr.ip);

#if USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE
    if (USB_NULL == p_ctrl)
    {
        return USB_ERR_PARA;
    }

    /* Argument Checking */
    if( (USB_IP0 != p_ctrl->module) && (USB_IP1 != p_ctrl->module) )
    {
        return USB_ERR_PARA;
    }

#if defined(BSP_MCU_RX65N) || defined(BSP_MCU_RX63N) || defined(BSP_MCU_RX63T) || defined(BSP_MCU_RX72T)\
     || defined(BSP_MCU_RX72M) || defined(BSP_MCU_RX72N) || defined(BSP_MCU_RX66N)
    if( USB_IP1 == p_ctrl->module)
    {
        return USB_ERR_PARA;
    }
#endif /* defined(BSP_MCU_RX65N) || defined(BSP_MCU_RX63N) || defined(BSP_MCU_RX63T) || defined(BSP_MCU_RX72T)\
     || defined(BSP_MCU_RX72M) || defined(BSP_MCU_RX72N) || defined(BSP_MCU_RX66N) */

#endif  /* USB_CFG_PARAM_CHECKING == USB_CFG_ENABLE */

    R_USB_GetInformation(p_ctrl, &info);
    if (USB_STS_CONFIGURED != info.status)
    {
        return USB_ERR_NG;
    }

    address = p_ctrl->address;
    if (USB_IP1 == p_ctrl->module)
    {
        address |= USBA_ADDRESS_OFFSET;
    }

    *p_drive = (uint8_t)usb_hmsc_ref_drvno(address);
    if (USB_ERROR == *p_drive)
    {
        return USB_ERR_NG;
    }

    return USB_SUCCESS;

}
/**********************************************************************************************************************
 End of function R_USB_HmscGetDriveNo
************************************************************************************************************************/

#if (BSP_CFG_RTOS_USED != 0)        /* Use RTOS */
/***********************************************************************************************************************
 * Function Name   : R_USB_HmscGetSem
 *******************************************************************************************************************//**
 * @brief      Gets a semaphore (Only RTOS)
 * @details    Gets a specific semaphore which is used in HMSC driver.
 * @note This API is not supported when using RX100/RX200 series MCU.
 */
void     R_USB_HmscGetSem(void)
{
    rtos_get_semaphore (&g_rtos_usb_hmsc_sem_id, RTOS_FOREVER);
}
/**********************************************************************************************************************
 End of function R_USB_HmscGetSem
************************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name   : R_USB_HmscRelSem
 *******************************************************************************************************************//**
 * @brief      Releases a semaphore (Only RTOS)
 * @details    Releases a specific semaphore which is used in HMSC driver.
 * @note This API is not supported when using RX100/RX200 series MCU.
 */
void     R_USB_HmscRelSem(void)
{
    rtos_release_semaphore (&g_rtos_usb_hmsc_sem_id);
}
/**********************************************************************************************************************
 End of function R_USB_HmscRelSem
************************************************************************************************************************/
#endif /* (BSP_CFG_RTOS_USED != 0) */

/**********************************************************************************************************************
 End  Of File
************************************************************************************************************************/
