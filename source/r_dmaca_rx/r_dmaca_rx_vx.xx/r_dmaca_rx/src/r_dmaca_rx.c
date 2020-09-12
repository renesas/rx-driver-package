/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
* File Name    : r_dmaca_rx.c
* Device(s)    : RX
* Tool-Chain   : Renesas RXC Toolchain v3.01.00
* OS           : not use
* H/W Platform : not use
* Description  : Functions for DMACA driver
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 30.06.2014 1.00    Initial release
*         : 27.08.2014 1.01    Not change program due to updating application note only.
*         : 30.01.2015 1.02    Added RX71M.
*         : 13.04.2015 1.03    Added RX231 and RX230.
*         : 30.09.2016 1.04    Added transfer_count and block_size to "case DMACA_CMD_STATUS_GET"
*         :                    of R_DMACA_Control().
*         : 31.07.2017 1.05    Supported RX65N-2MB.
*                              Fixed to correspond to Renesas coding rule.
*         : 28.09.2018 1.10    Supported RX66T.
*                              Add WAIT_LOOP comments.
*                              Added comments and brackets according to Renesas GSCE coding standard 5.0.
*         : 01.02.2019 1.20    Supported RX72T, RX65N-64pin.
*                              Added comments according to Renesas GSCE coding standard 5.0.
*         : 20.05.2019 2.00    Added support for GNUC and ICCRX.
*         : 28.06.2019 2.10    Supported RX23W.
*         : 15.08.2019 2.20    Supported RX72M.
*                              Fixed warnings in IAR.
*         : 30.12.2019 2.30    Modified comment of API function to Doxygen style.
*                              Fixed to comply with GSCE Coding Standards Rev.6.00.
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes>, "Project Includes"
*******************************************************************************/
/* Definitions for DMACA support */
#if defined(__CCRX__) || defined(__ICCRX__)
/* Intrinsic functions provided by compiler. */
#include <machine.h>
#elif defined(__GNUC__)
/* No header file for intrinsic functions. */
#endif /* defined(__CCRX__), defined(__GNUC__), defined(__ICCRX__) */
#include "r_dmaca_rx_if.h"
#include "./src/r_dmaca_rx_private.h"


/*******************************************************************************
Macro definitions
*******************************************************************************/

/*******************************************************************************
Typedef definitions
*******************************************************************************/


/*******************************************************************************
Private variables and functions
*******************************************************************************/
static bool r_dmaca_set_transfer_data(uint8_t channel, dmaca_transfer_data_cfg_t *p_cfg);
static dmaca_chk_locking_sw_t r_dmaca_check_locking_sw(void);

/*******************************************************************************
Exported global variables (to be accessed by other files)
*******************************************************************************/
/* This array holds callback functions. */
void *g_pdmaci_handlers[DMACA_NUM_CHANNELS];
uint8_t g_locking_sw[DMACA_NUM_CHANNELS];


/***********************************************************************************************************************
* Function Name: R_DMACA_Open
********************************************************************************************************************//**
* @brief This function is run after calling R_DMACA_Init() when using the APIs of the DMACA FIT module.
* @param[in] channel DMAC channel number.
* @retval DMACA_SUCCESS Successful operation
* @retval DMACA_ERR_INVALID_CH Channel is invalid.
* @retval DMACA_ERR_BUSY Resource has been locked by other process.
* @details Locks*1 the DMAC channel specified by the argument channel, then makes initial settings.
* Releases the DMAC from the module stop state, then activates the DMAC. Also, initializes the activation
* source selection register for the specified DMAC channel.\n
* Note: 1. The DMACA FIT module uses the r_bsp default lock function. As a result, the specified DMAC
* channel is in the locked state after a successful end.
* @note
* None.
*/
dmaca_return_t R_DMACA_Open(uint8_t channel)
{
#if (1 == DMACA_CFG_PARAM_CHECKING_ENABLE)
    /* Check argument. */
    if (false == r_dmaca_channel_valid_check(channel))
    {
        return DMACA_ERR_INVALID_CH;
    }
#endif /* DMACA_CFG_PARAM_CHECKING_ENABLE */

    /* ---- Lock DMACA hardware. ---- */
    if (false == R_BSP_HardwareLock((mcu_lock_t)(BSP_LOCK_DMAC0 + channel)))
    {
        /* Lock has already been acquired by another task. Need to try again later. */
        return DMACA_ERR_BUSY;
    }

    /* ---- Lock the DMACA channel. ---- */
    g_locking_sw[channel] = 1;

    /* Clear a Activation Request Select Register. */
    ICU_DMRSR(channel)  = 0x00;

    if (0x00 == ICU_DMRSR(channel))
    {
        /* do nothing */
    }

    /* Cancel module stop for DMAC and DTC. */
    r_dmaca_module_enable();

    /* Enable the DMAC activation. */
    DMACA_DMAST = DMACA_ACTIVE_ENABLE;

    /* Check DMAC activating status */
    if (DMACA_ACTIVE_ENABLE == DMACA_DMAST)
    {
        /* do nothing */
    }

    return DMACA_SUCCESS;
}
/**************************************************************************
* End of function R_DMACA_Open
*************************************************************************/

/***********************************************************************************************************************
* Function Name: R_DMACA_Close
********************************************************************************************************************//**
* @brief This function is used to release the resources of the DMAC channel currently in use.
* @param[in] channel DMAC channel number.
* @retval DMACA_SUCCESS Successful operation
* @retval DMACA_SUCCESS_OTHER_CH_BUSY Successful operation. Other DMAC channels are locked.
* @retval DMACA_SUCCESS_DTC_BUSY Successful operation. DTC is locked.
* @retval DMACA_ERR_INVALID_CH Channel is invalid.
* @retval DMACA_ERR_INTERNAL DMACA driver internal error.
* @details See Section 3 in the application note for details.
* @note When controlling the DTC without using the DTC FIT module, make sure to monitor the usage of the DTC and
* control locking and unlocking of the DTC so that calling this function does not set the DTC to the module stop
* state. Note that even if the DTC has not been activated, it is necessary to keep it in the locked state when not
* making DTC transfer settings.
*/
dmaca_return_t R_DMACA_Close(uint8_t channel)
{
    dmaca_chk_locking_sw_t result_chk_locking_sw = DMACA_ALL_CH_UNLOCKED_AND_DTC_UNLOCKED;

#if (1 == DMACA_CFG_PARAM_CHECKING_ENABLE)

    /* Check argument. */
    if (false == r_dmaca_channel_valid_check(channel))
    {
        return DMACA_ERR_INVALID_CH;
    }
#endif /* DMACA_CFG_PARAM_CHECKING_ENABLE */

    /* Clear a Activation Request Select Register. */
    ICU_DMRSR(channel)  = 0x00;

    if (0x00 == ICU_DMRSR(channel))
    {
        /* do nothing */
    }

    /* Disable DMA transfers. */
    DMACA_DMCNT(channel) = DMACA_TRANSFER_DISABLE;

    /* Unlock DMACA hardware to the specified channel. */
    R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_DMAC0 + channel));

    /* ---- Unlock the DMACA channel. ---- */
    g_locking_sw[channel] = 0;

    /* Check locking of DMACA and DTC. */
    result_chk_locking_sw = r_dmaca_check_locking_sw();

    switch (result_chk_locking_sw)
    {
        case DMACA_ALL_CH_UNLOCKED_AND_DTC_UNLOCKED: /* All DMAC channels and DTC are unlocked. */
        {
            /* Disable the DMAC activation. */
            DMACA_DMAST = DMACA_ACTIVE_DISABLE;

            /* Check DMAC activating status */
            if (DMACA_ACTIVE_DISABLE == DMACA_DMAST)
            {
                /* do nothing */
            }

            /* Set both DMAC and DTC to module stop state. */
            r_dmaca_module_disable();
            #if (1 == DMACA_CFG_USE_DTC_FIT_MODULE)
            R_BSP_HardwareUnlock((mcu_lock_t)BSP_LOCK_DTC);
            #endif
            break;
        }
        case DMACA_ALL_CH_UNLOCKED_BUT_DTC_LOCKED: /* All DMAC channels are unlocked. DTC is locked. */
        {
            /* Disable the DMAC activation. */
            DMACA_DMAST = DMACA_ACTIVE_DISABLE;

            /* Check DMAC activating status */
            if (DMACA_ACTIVE_DISABLE == DMACA_DMAST)
            {
                /* do nothing */
            }
            return DMACA_SUCCESS_DTC_BUSY;
            break;
        }
        case DMACA_LOCKED_CH_EXIST: /* Other DMAC channels are locked. */
        {
            return DMACA_SUCCESS_OTHER_CH_BUSY;
            break;
        }
        default:
        {
            return DMACA_ERR_INTERNAL;
            break;
        }
    }

    return DMACA_SUCCESS;
}
/**************************************************************************
* End of function R_DMACA_Close
*************************************************************************/

/***********************************************************************************************************************
* Function Name: R_DMACA_Create
********************************************************************************************************************//**
* @brief This function is used to make DMAC register settings and to specify the activation source.
* @param[in] channel DMAC channel number.
* @param[in] p_data_cfg Pointer to dmaca_transfer_data_cfg_t DMAC transfer information structure.
* See Section 3 in the application note for details.
* @retval DMACA_SUCCESS Successful operation
* @retval DMACA_ERR_INVALID_CH Channel is invalid.
* @retval DMACA_ERR_INVALID_ARG Parameters are invalid.
* @retval DMACA_ERR_NULL_PTR Argument pointers are NULL.
* @details References the dmaca_transfer_data_cfg_t DMAC transfer information structure passed as an argument and makes
* register settings for the specified DMAC channel. Also specifies the activation source for the DMAC channel.
* @note None.
*/
dmaca_return_t R_DMACA_Create(uint8_t channel, dmaca_transfer_data_cfg_t *p_data_cfg)
{
#if (1 == DMACA_CFG_PARAM_CHECKING_ENABLE)

    /* Check argument. */
    if (false == r_dmaca_channel_valid_check(channel))
    {
        return DMACA_ERR_INVALID_CH;
    }

    /* Check argument. */
    if ((uint32_t)NULL == (uint32_t)p_data_cfg)
    {
        return DMACA_ERR_NULL_PTR;
    }
#endif /* DMACA_CFG_PARAM_CHECKING_ENABLE */

    if (false == r_dmaca_set_transfer_data(channel, p_data_cfg))
    {
        /* Fail to apply configurations for Transfer data. */
        return DMACA_ERR_INVALID_ARG;
    }

    return DMACA_SUCCESS;
}
/**************************************************************************
* End of function R_DMACA_Create
*************************************************************************/

/***********************************************************************************************************************
* Function Name: R_DMACA_Control
********************************************************************************************************************//**
* @brief This function is used to control the operation of the DMAC. This function is run after calling R_DMACA_Open().
* @param[in] channel DMAC channel number.
* @param[in] command DMAC control command.
* See Section 3 in the application note for details.
* @param[in] p_stat Pointer to dmaca_stat_t DMAC status information structure.
* See Section 3 in the application note for details.
* @retval DMACA_SUCCESS Successful operation
* @retval DMACA_ERR_INVALID_CH Channel is invalid.
* @retval DMACA_ERR_INVALID_COMMAND Command is invalid.
* @retval DMACA_ERR_NULL_PTR Argument pointers are NULL.
* @retval DMACA_ERR_SOFTWARE_REQUESTED DMA transfer request by software has been generated already. See Section 3 in the application note for details.
* @retval DMACA_ERR_SOFTWARE_REQUEST_DISABLED Transfer Request Source is not Software. See Section 3 in the application note for details.
* @details See Section 3 in the application note for details.
* @note In the case of waiting for the transfer end by using DMAC channel 4-7 and an interrupt, please clear a transfer
*  escape interrupt flag (ESIF) or a transfer end interrupt flag (DTIF) using a callback function for transfer end
*  interrupts/transfer escape end interrupts.
*/
dmaca_return_t R_DMACA_Control(uint8_t channel, dmaca_command_t command, dmaca_stat_t *p_stat)
{
    uint8_t dmaca_interrupt_status = 0x00;
    uint8_t dmaca_soft_req_status  = 0x00;
    uint16_t dmaca_dmtmd = 0x0000;

#if (1 == DMACA_CFG_PARAM_CHECKING_ENABLE)

    /* Check argument. */
    if (false == r_dmaca_channel_valid_check(channel))
    {
        return DMACA_ERR_INVALID_CH;
    }
#endif /* DMACA_CFG_PARAM_CHECKING_ENABLE */

    switch (command)
    {
        case DMACA_CMD_ENABLE: /* Enable DMA transfer of the specified channel. */
        case DMACA_CMD_RESUME: /* Resume DMA transfer of the specified channel. */
        {
            /* Enable transfer for the corresponding channel */
            DMACA_DMCNT(channel) = DMACA_TRANSFER_ENABLE;

            /* Check transfer of the corresponding channel */
            if (DMACA_TRANSFER_ENABLE == DMACA_DMCNT(channel))
            {
                /* do nothing */
            }
            break;
        }
        case DMACA_CMD_DISABLE: /* Disable DMA transfer of the specified channel. */
        {
            /* Disable transfer for the corresponding channel */
            DMACA_DMCNT(channel) = DMACA_TRANSFER_DISABLE;

            /* Check transfer of the corresponding channel */
            if (DMACA_TRANSFER_DISABLE == DMACA_DMCNT(channel))
            {
                /* do nothing */
            }
            break;
        }
        case DMACA_CMD_ALL_ENABLE: /* Enable DMAC activation. */
        {
            /* Enable DMAC */
            DMACA_DMAST = DMACA_ACTIVE_ENABLE;

            /* Check DMAC activating status */
            if (DMACA_ACTIVE_ENABLE == DMACA_DMAST)
            {
                /* do nothing */
            }
            break;
        }
        case DMACA_CMD_ALL_DISABLE: /* Disable DMAC activation. */
        {
            /* Disable DMAC */
            DMACA_DMAST = DMACA_ACTIVE_DISABLE;

            /* Check DMAC activating status */
            if (DMACA_ACTIVE_DISABLE == DMACA_DMAST)
            {
                /* do nothing */
            }
            break;
        }
        case DMACA_CMD_SOFT_REQ_WITH_AUTO_CLR_REQ: /* DMA transfer by Software using DMREQ.SWREQ bit Auto Clear mode */
        {
            /* Check DMTMD.DCTG[1:0] bits. */
            if ((DMACA_DMTMD(channel) & DMACA_REQ_BIT_MASK) != DMACA_TRANSFER_REQUEST_SOFTWARE)
            {
                return DMACA_ERR_SOFTWARE_REQUEST_DISABLED;
            }

            /* Check a DMREQ.CLRS bit. */
            if (0 == (DMACA_DMREQ(channel) & DMACA_CLRS_BIT_MASK))
            {
                /* Check a DMREQ.SWREQ bit. */
                if (0 !=  (DMACA_DMREQ(channel) & DMACA_SWREQ_BIT_MASK))
                {
                    return DMACA_ERR_SOFTWARE_REQUESTED;
                }
            }

            /* DMA transfer by software is requested and request flag is cleared automatically. */
            DMACA_DMREQ(channel) = DMACA_SOFT_REQ_WITH_AUTO_CLR_REQ;

            /* Check DMREQ. */
            if (DMACA_SOFT_REQ_WITH_AUTO_CLR_REQ == DMACA_DMREQ(channel))
            {
                /* do nothing */
            }
            break;
        }
        case DMACA_CMD_SOFT_REQ_NOT_CLR_REQ: /* DMA transfer by Software */
        {
            /* Check DMTMD.DCTG[1:0] bits. */
            if ((DMACA_DMTMD(channel) & DMACA_REQ_BIT_MASK) != DMACA_TRANSFER_REQUEST_SOFTWARE)
            {
                return DMACA_ERR_SOFTWARE_REQUEST_DISABLED;
            }

            /* DMA transfer by software is requested and request flag is not cleared. */
            DMACA_DMREQ(channel) = DMACA_SOFT_REQ_NOT_CLR_REQ;

            /* Check DMREQ. */
            if (DMACA_SOFT_REQ_NOT_CLR_REQ == DMACA_DMREQ(channel))
            {
                /* do nothing */
            }
            break;
        }
        case DMACA_CMD_SOFT_REQ_CLR: /* Clear DMREQ.SWREQ bit. */
        {
            /* Check DMTMD.DCTG[1:0] bits. */
            if ((DMACA_DMTMD(channel) & DMACA_REQ_BIT_MASK) != DMACA_TRANSFER_REQUEST_SOFTWARE)
            {
                return DMACA_ERR_SOFTWARE_REQUEST_DISABLED;
            }

            /* Clear DMREQ.SWREQ bit. */
            DMACA_DMREQ(channel) &= (~DMACA_SWREQ_BIT_MASK);

            /* Check DMREQ. */
            if (0x00 == (DMACA_DMREQ(channel) & DMACA_SWREQ_BIT_MASK))
            {
                /* do nothing */
            }
            break;
        }

        case DMACA_CMD_STATUS_GET:
        {
#if (1 == DMACA_CFG_PARAM_CHECKING_ENABLE)
            /* Check argument. */
            if ((uint32_t)NULL == (uint32_t)p_stat)
            {
                return DMACA_ERR_NULL_PTR;
            }
#endif /* DMACA_CFG_PARAM_CHECKING_ENABLE */

            /* Get DMAC status register*/
            dmaca_interrupt_status = DMACA_DMSTS(channel);

            /* Get DMAC software Start register*/
            dmaca_soft_req_status = DMACA_DMREQ(channel);

            if (0 == (dmaca_interrupt_status & DMACA_ACT_BIT_MASK)) /* Get the current status of DMACA. */
            {
                /* DMACA operation is suspended. */
                p_stat->act_stat = false;
            }
            else
            {
                /* DMACA is operating. */
                p_stat->act_stat = true;
            }

            if (0 == (dmaca_interrupt_status & DMACA_DTIF_BIT_MASK)) /* Get the transfer end interrupt flag. */
            {
                /* A transfer end interrupt has not been generated. */
                p_stat->dtif_stat = false;
            }
            else
            {
                /* A transfer end interrupt has been generated. */
                p_stat->dtif_stat = true;
            }

            if (0 == (dmaca_interrupt_status & DMACA_ESIF_BIT_MASK)) /* Get the escape end interrupt flag. */
            {
                /* A transfer escape end interrupt has not been generated. */
                p_stat->esif_stat = false;
            }
            else
            {
                /* A transfer escape end interrupt has been generated. */
                p_stat->esif_stat = true;
            }

            if (0 == (dmaca_soft_req_status & DMACA_SWREQ_BIT_MASK)) /* Get the SWREQ flag. */
            {
                /* A software transfer is not requested. */
                p_stat->soft_req_stat = false;
            }
            else
            {
                /* A software transfer is requested. */
                p_stat->soft_req_stat = true;
            }

            /* Get DMAC transfer mode from the corresponding channel */
            dmaca_dmtmd = DMACA_DMTMD(channel);

            if ((DMACA_TRANSFER_MODE_REPEAT & dmaca_dmtmd) ||
                (DMACA_TRANSFER_MODE_BLOCK & dmaca_dmtmd))
            {
                /* Get transfer count from the corresponding channel */
                p_stat->transfer_count = (uint32_t)DMACA_DMCRB(channel);
            }
            else /* DMACA_TRANSFER_MODE_NORMAL */
            {
                /* Get transfer count from the corresponding channel */
                p_stat->transfer_count = (DMACA_INVALID_LOWER_BIT_MASK & DMACA_DMCRA(channel));
            }
            break;
        }
        case DMACA_CMD_ESIF_STATUS_CLR:
        {
            /* Clears Transfer Escape End Interrupt Flag. */
            DMACA_DMSTS(channel) = ~((uint8_t)DMACA_ESIF_BIT_MASK);

            /* Check DMAC status register */
            if (0x00 == DMACA_DMSTS(channel))
            {
                /* do nothing */
            }
            break;
        }
        case DMACA_CMD_DTIF_STATUS_CLR:
        {
            /* Clears Transfer Interrupt Flag. */
            DMACA_DMSTS(channel) = ~((uint8_t)DMACA_DTIF_BIT_MASK);

            /* Check DMAC status register */
            if (0x00 == DMACA_DMSTS(channel))
            {
                /* do nothing */
            }
            break;
        }
        default:
        {
            return DMACA_ERR_INVALID_COMMAND;
            break;
        }
    }

    return DMACA_SUCCESS;
}
/**************************************************************************
* End of function R_DMACA_Control
*************************************************************************/

/***********************************************************************************************************************
* Function Name: R_DMACA_Init
********************************************************************************************************************//**
* @brief This function is used to initialize the DMAC's internal information.
* @details Initializes the usage status of each DMA channel (internal information). Also, cancels the registered
* callback functions for all DMAC transfer end interrupts/transfer escape end interrupts (DMAC0I, DMAC1I, DMAC2I,
* DMAC3I, and DMAC74I). If DMAC transfer end interrupts/transfer escape end interrupts will be used, run the
* R_DMACA_Init() function beforehand, and then use the R_DMACA_Int_Callback() function (described below) to register
* the callback functions.
* @note When using the DMACA driver, run the R_DMACA_Init() function first. It is recommended to run at hardware setup
* operation.
*/
void R_DMACA_Init(void)
{
    uint32_t i;

    /* Initialize a callback for DMAC interrupts */
    /* WAIT_LOOP */
    for (i = 0; i < DMACA_NUM_CHANNELS; i++)
    {
        /* Initialize callback functions as FIT_NO_FUNC */
        g_pdmaci_handlers[i] = (void *)FIT_NO_FUNC;
        g_locking_sw[i] = 0;
    }
}
/**************************************************************************
* End of function R_DMACA_Init
*************************************************************************/

/***********************************************************************************************************************
* Function Name: R_DMACA_Int_Callback
********************************************************************************************************************//**
* @brief This function is used to register the callback function for the DMAC transfer end interrupt/transfer escape end
*  interrupt.
* @param[in] channel DMAC channel number.
* @param[in] p_callback Pointer to function that is called when a DMAC transfer end interrupt/transfer escape end
* interrupt occurs.
* @retval DMACA_SUCCESS Successful operation
* @retval DMACA_ERR_INVALID_CH Channel is invalid.
* @retval DMACA_ERR_INVALID_HANDLER_ADDR Invalid function address is set.
* @details Registers the callback function for the DMAC transfer end interrupt/transfer escape end interrupt of the
* specified channel. The registration of an already-registered callback function is canceled if FIT_NO_FUNC or NULL
* is passed as the callback argument. Also, the registration of an already-registered callback function is canceled
* if DMACA_ERR_INVALID_HANDLER_ADDR is returned.\n
* Note: The callback function arguments and return values should be of void type
* @note None
*/
dmaca_return_t R_DMACA_Int_Callback(uint8_t channel,  void * p_callback)
{
#if (1 == DMACA_CFG_PARAM_CHECKING_ENABLE)

    /* Check argument. */
    if (false == r_dmaca_channel_valid_check(channel))
    {
        return DMACA_ERR_INVALID_CH;
    }

    /* Check for valid address. */
    if (((uint32_t)NULL == (uint32_t)p_callback) || ((uint32_t)FIT_NO_FUNC == (uint32_t)p_callback))
    {
        /* Previously registered callback is unregistered */
        g_pdmaci_handlers[channel] = (void *)FIT_NO_FUNC;
        return DMACA_ERR_INVALID_HANDLER_ADDR;
    }
#endif /* DMACA_CFG_PARAM_CHECKING_ENABLE */

    g_pdmaci_handlers[channel] = p_callback;
    return DMACA_SUCCESS;
}
/**************************************************************************
* End of function R_DMACA_Int_Callback
*************************************************************************/

/***********************************************************************************************************************
* Function Name: R_DMACA_Int_Enable
********************************************************************************************************************//**
* @brief This function is used to enable DMAC transfer end interrupts/transfer escape end interrupts.
* @param[in] channel DMAC channel number.
* @param[in] priority DMAC transfer end interrupt/transfer escape end interrupt priority level.
* @retval DMACA_SUCCESS Successful operation
* @retval DMACA_ERR_INVALID_CH Channel is invalid.
* @details Enables the DMAC transfer end interrupt/transfer escape end interrupt for the specified channel.
* @note None
*/
dmaca_return_t R_DMACA_Int_Enable(uint8_t channel, uint8_t priority)
{
#if (1 == DMACA_CFG_PARAM_CHECKING_ENABLE)

    /* Check argument. */
    if (false == r_dmaca_channel_valid_check(channel))
    {
        return DMACA_ERR_INVALID_CH;
    }
#endif /* DMACA_CFG_PARAM_CHECKING_ENABLE */

    /* Set the DMACmI priority level. */
    /* Set the DMACmI interrupt Enable bit. */
    if (r_dmaca_int_enable(channel, priority) != DMACA_SUCCESS)
    {
        return DMACA_ERR_INVALID_CH;
    }

    return DMACA_SUCCESS;
}
/**************************************************************************
* End of function R_DMACA_Int_Enable
*************************************************************************/

/***********************************************************************************************************************
* Function Name: R_DMACA_Int_Disable
********************************************************************************************************************//**
* @brief This function is used to disable the DMAC transfer end interrupt/transfer escape end interrupt.
* @param[in] channel DMAC channel number.
* @retval DMACA_SUCCESS Successful operation
* @retval DMACA_ERR_INVALID_CH Channel is invalid.
* @details Disables the DMAC transfer end interrupt/transfer escape end interrupt for the specified channel.
* @note None
*/
dmaca_return_t R_DMACA_Int_Disable(uint8_t channel)
{
#if (1 == DMACA_CFG_PARAM_CHECKING_ENABLE)

    /* Check argument. */
    if (false == r_dmaca_channel_valid_check(channel))
    {
        return DMACA_ERR_INVALID_CH;
    }
#endif /* DMACA_CFG_PARAM_CHECKING_ENABLE */

    /* Clear the DMACmI interrupt Enable bit. */
    /* Clear the DMACmI priority level. */
    if (r_dmaca_int_disable(channel) != DMACA_SUCCESS)
    {
        return DMACA_ERR_INVALID_CH;
    }

    return DMACA_SUCCESS;
}
/**************************************************************************
* End of function R_DMACA_Int_Disable
*************************************************************************/

/***********************************************************************************************************************
* Function Name: R_DMACA_GetVersion
********************************************************************************************************************//**
* @brief This function is used to fetch the driver version information.
* @return Version number Upper 2 bytes: major version, lower 2 bytes: minor version.
* @details Returns the version information.
* @note None
*/
uint32_t R_DMACA_GetVersion(void)
{
    uint32_t version = 0;

    version = (DMACA_VERSION_MAJOR << 16) | DMACA_VERSION_MINOR;

    return version;
}
/**************************************************************************
* End of function R_DMACA_GetVersion
*************************************************************************/

/*******************************************************************************
* Function Name: r_dmaca_set_transfer_data
* Description  : Applies configurations to Transfer data area.
*                It is an internal function called by R_DMACA_Create().
*                and all arguments are validated in R_DMACA_Create().
* Arguments    : channel -
*                    Which channel to use
*                *p_data_cfg -
*                    Pointer to information of Transfer data
* Return Value : true -
*                    Apply configurations for Transfer data successfully.
*                false -
*                    Fail to apply configurations for Transfer data.
*******************************************************************************/
static bool r_dmaca_set_transfer_data(uint8_t channel, dmaca_transfer_data_cfg_t *p_cfg)
{
#if (1 == DMACA_CFG_PARAM_CHECKING_ENABLE)

    /* Check source address value. */
    if ((0x00000000 != ((uint32_t)p_cfg->p_src_addr & DMACA_INVALID_SRC_ADDR_MASK)) &&

                /* Check source address value. */
                (DMACA_INVALID_SRC_ADDR_MASK != ((uint32_t)p_cfg->p_src_addr & DMACA_INVALID_SRC_ADDR_MASK)))
    {
        return false;
    }

    /* Check destination address value. */
    if ((0x00000000 != ((uint32_t)p_cfg->p_des_addr & DMACA_INVALID_DES_ADDR_MASK)) &&

                /* Check destination address value. */
                (DMACA_INVALID_DES_ADDR_MASK != ((uint32_t)p_cfg->p_des_addr & DMACA_INVALID_DES_ADDR_MASK)))
    {
        return false;
    }

    /* Offset addition can be specified only for channel 0. */
    if (0 == channel)
    {
        if ((DMACA_SRC_ADDR_OFFSET == p_cfg->src_addr_mode) || (DMACA_DES_ADDR_OFFSET == p_cfg->des_addr_mode))
        {
            /* DMACA channel 0 offset address must be in 0x00000000h to 0x00FFFFFF and 0xFF000000 to 0xFFFFFFFF. */
            if ((0x00000000 != (p_cfg->offset_value & DMAC0_INVALID_OFFSET_VALUE_MASK)) &&
                (DMAC0_INVALID_OFFSET_VALUE_MASK != (p_cfg->offset_value & DMAC0_INVALID_OFFSET_VALUE_MASK)))
            {
                return false;
            }
        }
    }
    else
    {
        if ((DMACA_SRC_ADDR_OFFSET == p_cfg->src_addr_mode) || (DMACA_DES_ADDR_OFFSET == p_cfg->des_addr_mode))
        {
            return false;
        }
    }

    switch (p_cfg->transfer_mode) /* DMACA transfer mode */
    {
        case DMACA_TRANSFER_MODE_NORMAL: /* Normal mode */
        {
            /* Check transfer counter value. */
            if (DMACA_MAX_16BITS_COUNT_VAL_A < p_cfg->transfer_count)
            {
                /* The value of the 16bit counter is over the maximum. */
                return false;
            }
            break;
        }
        case DMACA_TRANSFER_MODE_REPEAT: /* Repeat transfer mode */
        case DMACA_TRANSFER_MODE_BLOCK: /* Block transfer mode */
        {
            /* Check transfer counter value. */
            if ((DMACA_MIN_COUNT_VAL > p_cfg->transfer_count) || (DMACA_MAX_16BITS_COUNT_VAL_B < p_cfg->transfer_count))
            {
                /* The value of the 16bit counter is over the maximum. */
                return false;
            }
            else if (DMACA_MAX_16BITS_COUNT_VAL_B == p_cfg->transfer_count)
            {
                /* The value of the 16bit counter is over less than the minimum and the maximum. */
                p_cfg->transfer_count = 0x00000000;
            }
            else
            {
                /* The value of the 16bit counter < maximum */
                /* Nothing */
            }

            /* Check block size value. */
            if ((DMACA_MIN_COUNT_VAL > p_cfg->block_size) || (DMACA_MAX_10BITS_COUNT_VAL < p_cfg->block_size))
            {
                /* The value of the 10bit counter is over less than the minimum and the maximum. */
                return false;
            }

            if (DMACA_MAX_10BITS_COUNT_VAL == p_cfg->block_size)
            {
                /* The value of the 10bit counter == maximum */
                p_cfg->block_size = 0x0000;
            }
            else /* The value of the 10bit counter < maximum */
            {
                /* Nothing */
            }

            /* Check source Address Extended Repeat setting. */
            if ((DMACA_REPEAT_BLOCK_SOURCE == p_cfg->repeat_block_side) &&
               (DMACA_SRC_ADDR_EXT_REP_AREA_NONE != p_cfg->src_addr_repeat_area))
            {
                return false;
            }

            /* Check destination Address Extended Repeat setting. */
            if ((DMACA_REPEAT_BLOCK_DESTINATION == p_cfg->repeat_block_side) &&
               (DMACA_DES_ADDR_EXT_REP_AREA_NONE != p_cfg->des_addr_repeat_area))
            {
                return false;
            }

            break;
        }
        default:
        {
            return false;
            break;
        }
    }
#endif /* DMACA_CFG_PARAM_CHECKING_ENABLE */

    /* Clear DMREQ register. */
    DMACA_DMREQ(channel) = 0x00;

   /* Disable DMA transfers. */
    DMACA_DMCNT(channel) = DMACA_TRANSFER_DISABLE;

    /* Set ICU.DMRSR register. */
    if (DMACA_TRANSFER_REQUEST_PERIPHERAL == p_cfg->request_source)
    {
        /* Set ICU.DMRSR register to Activation Source */
        ICU_DMRSR(channel) = (uint8_t)p_cfg->act_source;

        /* Check Activation Source */
        if ((uint8_t)p_cfg->act_source == ICU_DMRSR(channel))
        {
            /* do nothing */
        }
    }

    /* Set DMAMD register. */
    DMACA_DMAMD(channel) = ((((uint16_t)p_cfg->src_addr_mode | (uint16_t)p_cfg->src_addr_repeat_area) | (uint16_t)p_cfg->des_addr_mode) |
                                     (uint16_t)p_cfg->des_addr_repeat_area);

    /* Set DMTMD register. */
    DMACA_DMTMD(channel) = ((((uint16_t)p_cfg->transfer_mode | (uint16_t)p_cfg->repeat_block_side) | (uint16_t)p_cfg->data_size) |
                                     (uint16_t)p_cfg->request_source);

    /* Set DMSAR register. */
    DMACA_DMSAR(channel) = (volatile void R_BSP_EVENACCESS_SFR *)p_cfg->p_src_addr;

    /* Set DMSAR register. */
    DMACA_DMDAR(channel) = (volatile void R_BSP_EVENACCESS_SFR*)p_cfg->p_des_addr;
    switch (p_cfg->transfer_mode) /* DMACA transfer mode */
    {
        case DMACA_TRANSFER_MODE_NORMAL: /* Normal transfer mode */
        {
            /* Set DMCRA register. */
            DMACA_DMCRA(channel) = p_cfg->transfer_count;
            break;
        }
        case DMACA_TRANSFER_MODE_REPEAT: /* Repeat transfer mode */
        case DMACA_TRANSFER_MODE_BLOCK: /* Block transfer mode */
        {
            /* Set DMCRA register. */
            DMACA_DMCRA(channel) = (uint32_t)((p_cfg->block_size << 16) | p_cfg->block_size);

            /* Set DMCRB register. */
            DMACA_DMCRB(channel) = (uint16_t)p_cfg->transfer_count;
            break;
        }
        default:
        {
            return false;
            break;
        }
    }

    if (DMACA_CH0 == channel)
    {
        /* Set offset value for DMOFR register */
        DMAC0.DMOFR = p_cfg->offset_value;
    }

    /* Set DMCSL register. */
    DMACA_DMCSL(channel) = (uint8_t)p_cfg->interrupt_sel;
    
    /* Set DMINT register */
    DMACA_DMINT(channel) = (((((uint8_t)p_cfg->dtie_request | (uint8_t)p_cfg->esie_request) | (uint8_t)p_cfg->rptie_request) |
                                     (uint8_t)p_cfg->sarie_request) | (uint8_t)p_cfg->darie_request);

    /* Get DMAC interrupt setting */
    if ((uint8_t)p_cfg->dtie_request == DMACA_DMINT(channel))
    {
        /* do nothing */
    }

    return true;
}
/**************************************************************************
* End of function r_dmaca_set_transfer_data
*************************************************************************/

/*******************************************************************************
* Function Name: r_dmaca_check_locking_sw
* Description  : Check locking of DMACA and DTC.
* Arguments    : none -
* Return Value : DMACA_ALL_CH_UNLOCKED_AND_DTC_UNLOCKED -
*                    All DMAC channels and DTC are unlocked.
*                DMACA_ALL_CH_UNLOCKED_BUT_DTC_LOCKED -
*                    All DMAC channels are unlocked, but DTC is locked.
*                DMACA_LOCKED_CH_EXIST -
*                    Other DMAC channels are locked.
*******************************************************************************/
static dmaca_chk_locking_sw_t r_dmaca_check_locking_sw(void)
{
    dmaca_chk_locking_sw_t  ret = DMACA_ALL_CH_UNLOCKED_AND_DTC_UNLOCKED;

#if ((0 == BSP_CFG_USER_LOCKING_ENABLED) && (bsp_lock_t == BSP_CFG_USER_LOCKING_TYPE))

    uint32_t i = 0;
    uint32_t dmaca_lock_num = 0;

    /* Check locking of DMAC */
    /* WAIT_LOOP */
    for (i = 0; i < DMACA_NUM_CHANNELS; i++)
    {
        if (0x00 != g_locking_sw[i])
        {
            dmaca_lock_num ++;
        }
    }

    if (0 != dmaca_lock_num)
    {
        ret = DMACA_LOCKED_CH_EXIST;
    }
    else
    {
        #if (1 == DMACA_CFG_USE_DTC_FIT_MODULE)
        if (false == R_BSP_HardwareLock((mcu_lock_t)BSP_LOCK_DTC))
        {
            ret = DMACA_ALL_CH_UNLOCKED_BUT_DTC_LOCKED;
        }
        #else  /* defined(1 != DMACA_CFG_USE_DTC_FIT_MODULE) */
        /* User has to check the locking of DTC by themselves. */
        ret = r_dmaca_check_dtc_locking_byuser();
        #endif
    }

#else  /* defined(0 != BSP_CFG_USER_LOCKING_ENABLED)
          or defined(bsp_lock_t != BSP_CFG_USER_LOCKING_TYPE) */
    /* User has to check the locking of DMACA and DTC by themselves. */
    ret = r_dmaca_check_DMACA_DTC_locking_byUSER();
#endif

    return ret;
}
/**************************************************************************
* End of function r_dmaca_check_locking_sw
*************************************************************************/

/* End of File */

