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
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_wdt_rx.c
* Description  : Functions for using WDT on RX devices.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           28.04.2016 1.00    First Release.
*           01.10.2017 1.10    A comment of MCU register setting was added.
*           20.10.2017 1.20    Changed global variable name, added comments and brackets 
*                              according to Renesas GSCE Coding Standards 5.00.
*           01.02.2019 1.40    Added support for RX72T, RX65N-64pin.
*                              Bug fix: R_WDT_Open(), R_IWDT_Open() invalidated if either module is in auto-start mode.
*           20.05.2019 2.00    Added support for GNUC and ICCRX.
*           15.08.2019 2.20    Fixed warnings in IAR.
*           30.12.2019 2.30    Added support RX66N, RX72N.
*                              Modified comment of API function to Doxygen style.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "platform.h"
/* Defines for WDT support */
#include "r_wdt_rx_config.h"
#include "r_wdt_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
#if ((BSP_CFG_OFS0_REG_VALUE & OFS0_WDT_DISABLED) == OFS0_WDT_DISABLED) /* Register start mode */
/* State variable for this package. */
static bool gs_already_opened = false;

/* Internal functions. */
static wdt_err_t wdt_init_register_start_mode(wdt_config_t *p_cfg);
#if (1 == WDT_CFG_PARAM_CHECKING_ENABLE)
static bool wdt_parameter_check(wdt_config_t *p_cfg);

#endif /* WDT_CFG_PARAM_CHECKING_ENABLE */
#endif /* BSP_CFG_OFS0_REG_VALUE */

static inline bool acquire_hw_lock(void);
static inline void release_hw_lock(void);

/***********************************************************************************************************************
* Function Name: R_WDT_Open
********************************************************************************************************************//**
* @brief This function initializes the WDT FIT module. This function must be executed before other API functions.
* This Open function is not used when WDT Auto-Start mode is enabled in the OFS0 register in r_bsp_config.h.
* @param[in] p_cfg - Pointer to configuration structure of type wdt_config_t.
* See Section 3.R_WDT_Open() in the application note for details
* @retval [WDT_SUCCESS]          - WDT initialized
* @retval [WDT_ERR_OPEN_IGNORED] - Error: The module has already been opened
* @retval [WDT_ERR_INVALID_ARG]  - Error: Argument is not valid
* @retval [WDT_ERR_NULL_PTR]     - Error: Received null pointer
* @retval [WDT_ERR_BUSY]         - Error: WDT resource is locked
* @details This function initializes associated WDT registers. Options can be selected per MCU
* @note The Open function is only available in Register-Start mode
* (BSP_CFG_OFS0_REG_VALUE = 0xFFFFFFFF in r_bsp_config.h).
* This function configures the WDT counter without starting the WDT counter.
* The WDT_CMD_REFRESH_COUNTING argument must be specified in the R_WDT_Control function to start the WDT counter.
* The R_WDT_Open() function should be called only once after a reset.
* Any additional calls will return WDT_ERR_OPEN_IGNORED.
* The setting to enable WDT underflow/refresh error interrupt in Interrupt Controller module (ICU)
* must be enabled when non-maskable interrupts are selected and Auto-Start mode is enabled.
* In both Auto-Start mode and Register-Start mode, the user application should have a function to handle this interrupt.
* In Register-Start mode, the function should be registered right after calling R_WDT_Open() as shown in the example above.
* In Auto-Start mode, the R_BSP_InterruptWrite() should occur right after enabling WDT interrupt.
* See Section 3.R_WDT_Open() in the application note for details
*/
#if ((BSP_CFG_OFS0_REG_VALUE & OFS0_WDT_DISABLED) == OFS0_WDT_DISABLED) /*Register start mode*/
wdt_err_t R_WDT_Open (void * const p_cfg)
{
    wdt_err_t   err;
    bool        ret;

    err = WDT_SUCCESS;

    /* CHECK ARGUMENTS */
#if (1 == WDT_CFG_PARAM_CHECKING_ENABLE)
    if (NULL == p_cfg)
    {
        return WDT_ERR_NULL_PTR;
    }

    /* WDT parameter checking */
    ret = wdt_parameter_check((wdt_config_t *)p_cfg);
    if (false == ret)
    {
        return WDT_ERR_INVALID_ARG;
    }
#endif

    ret = acquire_hw_lock();
    if (false == ret)
    {
        /* Lock not obtained, return error */
        return WDT_ERR_BUSY;
    }

    /* Lock obtained, we can change state */
    if (true == gs_already_opened)
    {
        /* Open function is already called */
        release_hw_lock();
        return WDT_ERR_OPEN_IGNORED;
    }

    /* Make setting to WDT register for initialization */
    err = wdt_init_register_start_mode((wdt_config_t *)p_cfg);

    /* Update status */
    gs_already_opened = true;

    release_hw_lock();
    return err;
}
/******************************************************************************
End of function R_WDT_Open
******************************************************************************/

/***********************************************************************************************************************
* Function Name: wdt_init_register_start_mode
* Description  : Make setting to WDT register for initialization.
* Arguments    : p_cfg -
*                    Pointer to configuration structure of type wdt_config_t.
* Return Value : WDT_SUCCESS -
*                    WDT is initialized successful.
***********************************************************************************************************************/
static wdt_err_t wdt_init_register_start_mode(wdt_config_t *p_cfg)
{
    /* Set Time-out period, Clock Division Ratio, Window Start/End Position */
    WDT.WDTCR.WORD = (((uint16_t)(p_cfg->timeout) | (uint16_t)(p_cfg->wdtcks_div)) | \
                     ((uint16_t)(p_cfg->window_start) | (uint16_t)(p_cfg->window_end)));

    /* Set reset output or NMI output */
    WDT.WDTRCR.BYTE = (uint8_t)p_cfg->timeout_control;

    if (WDT_TIMEOUT_NMI == p_cfg->timeout_control)
    {
        /* Enable NMI interrupt
         NMIER - Non-Maskable Interrupt Enable Register
         b0  WDTEN - WDT Underflow/Refresh Error Enable - WDT Underflow/Refresh Error interrupt is enabled */
        ICU.NMIER.BIT.WDTEN = 0x1;
    }

    return WDT_SUCCESS;
}
/******************************************************************************
End of function wdt_init_register_start_mode
******************************************************************************/

/***********************************************************************************************************************
* Function Name: wdt_parameter_check
* Description  : Check validity of input parameter of R_WDT_Open().
* Arguments    : p_cfg -
*                    Pointer to configuration structure of type wdt_config_t.
* Return Value : true -
*                    Input is valid.
*                false -
*                    Input is invalid.
***********************************************************************************************************************/
#if (1 == WDT_CFG_PARAM_CHECKING_ENABLE)
static bool wdt_parameter_check(wdt_config_t *p_cfg)
{
    bool ret = true;

    if (p_cfg->timeout >= WDT_NUM_TIMEOUTS)
    {
        ret = false;
    }

    if (((WDT_CLOCK_DIV_4    != p_cfg->wdtcks_div)  && \
         (WDT_CLOCK_DIV_64   != p_cfg->wdtcks_div)) && \
       (((WDT_CLOCK_DIV_128  != p_cfg->wdtcks_div)  && \
         (WDT_CLOCK_DIV_512  != p_cfg->wdtcks_div)) && \
        ((WDT_CLOCK_DIV_2048 != p_cfg->wdtcks_div)  && \
         (WDT_CLOCK_DIV_8192 != p_cfg->wdtcks_div))))
    {
        ret = false;
    }

    if (((WDT_WINDOW_END_75 != p_cfg->window_end)  && \
         (WDT_WINDOW_END_50 != p_cfg->window_end)) && \
        ((WDT_WINDOW_END_25 != p_cfg->window_end)  && \
         (WDT_WINDOW_END_0  != p_cfg->window_end)))
    {
        ret = false;
    }

    if (((WDT_WINDOW_START_25  != p_cfg->window_start)  && \
         (WDT_WINDOW_START_50  != p_cfg->window_start)) && \
        ((WDT_WINDOW_START_75  != p_cfg->window_start)  && \
         (WDT_WINDOW_START_100 != p_cfg->window_start)))
    {
        ret = false;
    }

    if ((WDT_TIMEOUT_RESET != p_cfg->timeout_control) && \
        (WDT_TIMEOUT_NMI   != p_cfg->timeout_control))
    {
        ret = false;
    }

    return ret;
}
/******************************************************************************
End of function wdt_parameter_check
******************************************************************************/
#endif /* WDT_CFG_PARAM_CHECKING_ENABLE */
#endif /* BSP_CFG_OFS0_REG_VALUE */


/***********************************************************************************************************************
* Function Name: R_WDT_Control
********************************************************************************************************************//**
* @brief This function gets the WDT status and refreshes the WDT down-counter. This function may be used in both
* Auto-Start and Register-Start modes.
* @param[in] cmd - Run command
* @param[in] p_status - Pointer to the storage of the counter and status flags. The following code is used for
*  the cmd argument.
* @code
* WDT_CMD_GET_STATUS,         // Get WDT status
* WDT_CMD_REFRESH_COUNTING,  // Refresh the counter
* @endcode
* @retval [WDT_SUCCESS]          - Command completed successfully.
* @retval [WDT_ERR_INVALID_ARG]  - Error: Argument is not valid.
* @retval [WDT_ERR_NULL_PTR]     - Error: Received null pointer.
* @retval [WDT_ERR_NOT_OPENED]   - Error: Open function has not yet been called.
* @retval [WDT_ERR_BUSY]         - Error: WDT resource is locked.
* @details
* If command WDT_CMD_REFRESH_COUNTING is selected, the watchdog counter is initialized to its starting value.
* If command WDT_CMD_GET_STATUS is selected, the WDT status (underflow error status, refresh error status,
* counter value) register is loaded into *p_status. The two high-order bits indicate whether a refresh error
* (b15) or an underflow occurred (b14). The remaining bits (b13 - b0) indicate the current counter value.
* @note The second argument is ignored for the WDT_CMD_REFRESH_COUNTING command.
*/
wdt_err_t R_WDT_Control(wdt_cmd_t const cmd, uint16_t * p_status)
{
    bool ret = false;

    /* CHECK ARGUMENTS */
#if (1 == WDT_CFG_PARAM_CHECKING_ENABLE)
    if ((WDT_CMD_REFRESH_COUNTING != cmd) && (WDT_CMD_GET_STATUS != cmd))
    {
        return WDT_ERR_INVALID_ARG;
    }

    if ((NULL == p_status) && (WDT_CMD_GET_STATUS == cmd))
    {
        return WDT_ERR_NULL_PTR;
    }
#endif

    ret = acquire_hw_lock();
    if (false == ret)
    {
        /* Lock not obtained, return error */
        return WDT_ERR_BUSY;
    }

    /* Lock obtained */
#if ((BSP_CFG_OFS0_REG_VALUE & OFS0_WDT_DISABLED) == OFS0_WDT_DISABLED) /* Register start mode */
    if (false == gs_already_opened)
    {
        /* Open function has not called yet */
        release_hw_lock();
        return WDT_ERR_NOT_OPENED;
    }
#endif

    switch (cmd)
    {
        case WDT_CMD_REFRESH_COUNTING:
            {
            /* Make settings to WDTRR register to refresh the counter
            WDTRR - WDT Refresh Register
            Refresh the down-counter of WDT */
            WDT.WDTRR = 0x00u;

            /* Set WDTRR register value to 0xFFu to refresh counting */
            WDT.WDTRR = 0xFFu;
            }
        break;

        case WDT_CMD_GET_STATUS:
            {
            /* Get WDT status from WDTSR register */
            *p_status = WDT.WDTSR.WORD;
            }
        break;

        default:
            {
                R_BSP_NOP();
            }
        break;
    }

    release_hw_lock();
    return WDT_SUCCESS;
}
/******************************************************************************
End of function R_WDT_Control
******************************************************************************/

/***********************************************************************************************************************
* Function Name: R_WDT_GetVersion
********************************************************************************************************************//**
* @brief This function returns the driver version number at runtime.
* @return Version number
* @details This function returns the driver version number.
* @note None.
*/
uint32_t  R_WDT_GetVersion (void)
{
    uint32_t const version = (WDT_RX_VERSION_MAJOR << 16) | WDT_RX_VERSION_MINOR;

    return version;
}
/******************************************************************************
End of function R_WDT_GetVersion
******************************************************************************/

/*****************************************************************************
* Function Name: acquire_hw_lock
* Description  : get the hardware lock BSP_LOCK_WDT.
* Arguments    : None.
* Return Value : true -
*                  the lock is acquired successfully
*                false -
*                   fail to get the lock
******************************************************************************/
static inline bool acquire_hw_lock(void)
{
    return R_BSP_HardwareLock(BSP_LOCK_WDT);
}
/******************************************************************************
End of function acquire_hw_lock
******************************************************************************/

/*****************************************************************************
* Function Name: release_hw_lock
* Description  : release hardware lock BSP_LOCK_WDT.
* Arguments    : None.
* Return Value : None.
******************************************************************************/
static inline void release_hw_lock(void)
{
    R_BSP_HardwareUnlock(BSP_LOCK_WDT);
}
/******************************************************************************
End of function release_hw_lock
******************************************************************************/
