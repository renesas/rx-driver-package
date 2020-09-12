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
 * File Name    : r_pdc_rx.c
 * Version      : 2.05
 * Description  : PDC module device driver
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.10.2016 2.00     First Release
 *         : 20.05.2019 2.02     Added support for GNUC and ICCRX.
 *                               Fixed coding style.
 *         : 30.07.2019 2.04     Added WAIT LOOP
 *         : 22.11.2019 2.05     Added support for atomic control.
 *                               Modified comment of API function to Doxygen style.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
/* Access to peripherals and board defines. */
#include "platform.h"

/* Defines for PDC support */
#include "r_pdc_rx_if.h"
#include "src/r_pdc_rx_private.h"

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/
/* This array holds callback functions. */
typedef void (*pcdfi_callback_t) (void *pdata);

/***********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Private variables and functions
 ***********************************************************************************************************************/
static bool is_opened = false; /* indicate whether PDC is opened */
static void * volatile ppcdfi_handlers; /* Pointer to event handler of PCDFI interrupt */
static pdc_cb_t cb_func; /* Pointer to callback functions */

/**
 * Private global function prototypes
 */
static void pdc_callback_receive_data_ready (void * pparam);
static void pdc_callback_frame_end (void * pparam);
static void pdc_callback_error (void * pparam);
static void pdc_error_processing (void);

static pdc_return_t pdc_reset (void);
static pdc_return_t pdc_initialize (pdc_data_cfg_t *p_data_cfg);
static pdc_return_t pdc_capture_start (void);
static pdc_return_t pdc_change_position_size (pdc_data_cfg_t *p_data_cfg);
static pdc_return_t pdc_int_setting (pdc_step_t step, pdc_data_cfg_t *p_data_cfg);
static pdc_return_t pdc_int_setting_sub (pdc_data_cfg_t *p_data_cfg);
static pdc_return_t pdc_clear_status (pdc_stat_t *p_stat);
static pdc_return_t pdc_get_status (pdc_stat_t *p_stat);
static void pdc_module_enable (void);
static void pdc_module_disable (void);

/**********************************************************************************************************************
 * Function Name: R_PDC_Open
 *****************************************************************************************************************/ /**
 * @brief This function initializes the PDC FIT module. It must be run before using the other API functions.
 * @param[in] *p_data_cfg
 *             Pointer to PDC settings data structure.\n 
 *             See section 3.1 in application note for members of referenced pdc_data_cfg_t structure and 
 *             their setting values.
 * @retval    PDC_SUCCESS         Processing finished successfully.
 * @retval    PDC_ERR_OPENED      R_PDC_Open has already been run.
 * @retval    PDC_ERR_INVALID_ARG Parameter values in PDC setting information are invalid.
 * @retval    PDC_ERR_NULL_PTR    Argument p_data_cfg is a NULL pointer.
 * @retval    PDC_ERR_LOCK_FUNC   The PDC has already been locked by another process.
 * @retval    PDC_ERR_INTERNAL    A module internal error was detected.
 * @retval    PDC_ERR_RST_TIMEOUT PDC reset was not canceled even after the specified amount of time elapsed.
 * @details   This function is performed to initialize the PDC. See section 3.1 in application note for details.
 * @note      This API function should be run when the device and the camera module are connected. Running this
 *            API function enables PIXCLK input and then resets the PDC, but this is because the reset will not
 *            complete if PIXCLK where the camera module output is not input to the device. If the return value
 *            PDC_ERR_RST_TIMEOUT is confirmed, check the settings and hardware configuration of the camera module.\n 
 *            An endianness setting is applied within this API function. The endianness setting should be selected
 *            to match the corresponding compiler setting. If the compiler endianness setting is little-endian, 
 *            the PDC endianness setting should be little-endian as well, and if the compiler endianness setting
 *            is big-endian, the PDC endianness setting should also be big-endian.\n 
 *            The arguments and return values of the registered callback function should be of type void.
 */
pdc_return_t R_PDC_Open (pdc_data_cfg_t *p_data_cfg)
{
    pdc_return_t ret;
    volatile bsp_int_err_t bsp_int_err;

    /* Check that the R_PDC_Open has been executed. */
    if (true == is_opened)
    {
        return PDC_ERR_OPENED;
    }

    /* Make sure that the argument is correct. */
    /* To cast in order to compare the address. There is no problem because the information is not lost even if the
     *  cast. */
    if ((uint32_t) NULL == (uint32_t) p_data_cfg)
    {
        return PDC_ERR_NULL_PTR;
    }

    /* Locks the PDC hardware resource using the r_bsp hardware locking function. */
    if (false == R_BSP_HardwareLock((mcu_lock_t) (BSP_LOCK_PDC)))
    {
        return PDC_ERR_LOCK_FUNC;
    }

    /* Cancels PDC module stop state. */
    pdc_module_enable();
    
    /* Performs registration of callback function for PCDFI interrupt used by the PDC. */
    ppcdfi_handlers = (void *) pdc_callback_receive_data_ready;

    /* Performs registration of callback function for PCFEI interrupt used by the PDC. */
    bsp_int_err = R_BSP_InterruptWrite(BSP_INT_SRC_BL0_PDC_PCFEI, pdc_callback_frame_end);
    if (BSP_INT_SUCCESS != bsp_int_err)
    {
        return PDC_ERR_INTERNAL;
    }

    /* Performs registration of callback function for PCERI interrupt used by the PDC. */
    bsp_int_err = R_BSP_InterruptWrite(BSP_INT_SRC_BL0_PDC_PCERI, pdc_callback_error);
    if (BSP_INT_SUCCESS != bsp_int_err)
    {
        return PDC_ERR_INTERNAL;
    }
    
    /* Set Pointer to PCDFI, PCEFI, PCERI interrupt callback functions */
    cb_func = p_data_cfg->p_callback;
    
    /* Makes settings for interrupts used by the PDC. */
    ret = pdc_int_setting(PDC_INITIALIZE, p_data_cfg);
    if (PDC_SUCCESS != ret)
    {
        return ret;
    }

    /* Initialize for PDC */
    ret = pdc_initialize(p_data_cfg);
    if (PDC_SUCCESS != ret)
    {
        return ret;
    }

    /* PDC module is initialized successfully in R_PDC_Open. */
    is_opened = true;

    return ret;
} /* End of function R_PDC_Open() */

/**********************************************************************************************************************
 * Function Name: R_PDC_Close
 *****************************************************************************************************************/ /**
 * @brief Ends operation by the PDC and puts it into the module stop state.
 * @retval    PDC_SUCCESS      Processing finished successfully.
 * @retval    PDC_ERR_NOT_OPEN R_PDC_Open has not been run.
 * @details   This function is performed to shut down the PDC. See section 3.2 in application note for details.
 * @note      Use this API function after running R_PDC_Open and confirming that the return value is PDC_SUCCESS.
 */
pdc_return_t R_PDC_Close (void)
{
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif

    /* Check that the R_PDC_Open has been executed. */
    if (false == is_opened)
    {
        return PDC_ERR_NOT_OPEN;
    }

    /* Disables interrupts (PCFEI, PCERI, and PCDFI) used by the PDC. */
    R_BSP_InterruptRequestDisable(VECT(PDC, PCDFI));
    PDC.PCCR0.LONG &= (~0x03F0);
    /* WAIT_LOOP */
    while (0 != PDC.PCCR0.BIT.DFIE)
    {
        /* Do Nothing */
    }
    IR(PDC, PCDFI)= 0; /* Interrupt request is cleared. */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    ICU.GENBL0.BIT.EN30 = 0; /* Interrupt request(GRPBL0.IS30) is also cleared.  */
    ICU.GENBL0.BIT.EN31 = 0; /* Interrupt request(GRPBL0.IS31) is also cleared.  */
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif

    /* Disables PDC operation.
     The PCCR0 register should only be set while the PCE bit in the PCCR1 register is 0. */
    PDC.PCCR1.BIT.PCE = PDC_DISABLE_OPERATION;

    /* Stops supply of parallel data transfer clock output (PCKO). */
    PDC.PCCR0.BIT.PCKOE = PDC_DISABLE_PCKO_OUTPUT;

    /* Disables pixel clock input from the image sensor. */
    PDC.PCCR0.BIT.PCKE = PDC_DISABLE_PIXCLK_INPUT;

    /* Stops PDC module. */
    pdc_module_disable();

    /* Cancels PDC hardware resource locking using the r_bsp hardware locking function. */
    R_BSP_HardwareUnlock((mcu_lock_t) (BSP_LOCK_PDC));

    /* PDC module is not initialized yet */
    is_opened = false;

    return PDC_SUCCESS;
} /* End of function R_PDC_Close() */

/**********************************************************************************************************************
 * Function Name: R_PDC_Control
 *****************************************************************************************************************/ /**
 * @brief This function performs processing according to control codes.
 * @param[in] command
 *             Control code. See section 3.3 in application note for the supported command values.
 * @param[in] *p_data_cfg
 *             Pointer to PDC settings data structure.\n 
 *             The arguments that are referenced differ according to the specified command. See section 3.3 in
 *             application note for details.
 * @param[in,out] *p_stat
 *             Pointer to PDC status structure.\n 
 *             The arguments that are referenced differ according to the specified command. See section 3.3 in
 *             application note for details.
 * @retval    PDC_SUCCESS             Processing finished successfully.
 * @retval    PDC_ERR_NOT_OPEN        R_PDC_Open has not been run.
 * @retval    PDC_ERR_INVALID_ARG     Setting value applied to PDC register is invalid.
 * @retval    PDC_ERR_INVALID_COMMAND The argument command is invalid.
 * @retval    PDC_ERR_NULL_PTR        The argument p_data_cfg or p_stat is a NULL pointer.
 * @retval    PDC_ERR_RST_TIMEOUT     PDC reset was not canceled even after the specified amount of time elapsed.
 * @details   See section 3.3 in application note for details.
 * @note      Running this API function when receive operation is in progress will overwrite the PDC registers,
 *            thereby causing receive operation to stop. Since running this API function before the frame-end interrupt
 *            is generated stops receive operation, capturing of image data is halted midway. To restart image capture,
 *            reset in the DMAC or DTC the pointer to the transfer destination in memory, then use the R_PDC_Control
 *            capture start command to restart capturing of image data.\n 
 *            When running R_PDC_Control with the command PDC_CMD_STATUS_CLR as an argument, set the status information
 *            to be cleared as "true" and the status information not to be cleared as "false". If these settings are not
 *            made before running R_PDC_Control, status information may be cleared in an unintended manner.
 */
pdc_return_t R_PDC_Control (pdc_command_t command, pdc_data_cfg_t * p_data_cfg, pdc_stat_t * p_stat)
{
    pdc_return_t ret = PDC_SUCCESS;

    /* Check that the R_PDC_Open has been executed. */
    if (false == is_opened)
    {
        return PDC_ERR_NOT_OPEN;
    }

    switch (command)
    {
        case PDC_CMD_CAPTURE_START :
            ret = pdc_capture_start();
        break;

        case PDC_CMD_CHANGE_POS_AND_SIZE :
            ret = pdc_change_position_size(p_data_cfg);
        break;

        case PDC_CMD_STATUS_GET :
            ret = pdc_get_status(p_stat);
        break;

        case PDC_CMD_STATUS_CLR :
            ret = pdc_clear_status(p_stat);
        break;

        case PDC_CMD_SET_INTERRUPT :
            ret = pdc_int_setting(PDC_CHANGE_SETTING, p_data_cfg);
        break;

        case PDC_CMD_DISABLE :
            PDC.PCCR1.BIT.PCE = PDC_DISABLE_OPERATION;
        break;

        case PDC_CMD_ENABLE :
            PDC.PCCR1.BIT.PCE = PDC_ENABLE_OPERATION;
        break;

        case PDC_CMD_RESET :
            PDC.PCCR1.BIT.PCE = PDC_DISABLE_OPERATION;
            ret = pdc_reset();
        break;

            /* Commands not supported */
        default :
            ret = PDC_ERR_INVALID_COMMAND;
        break;
    }
    return ret;
} /* End of function R_PDC_Control() */

/**********************************************************************************************************************
 * Function Name: R_PDC_GetFifoAddr
 *****************************************************************************************************************/ /**
 * @brief This function gets the FIFO address of the PDC.
 * @param[out] *p_fifo_addr
 *             Pointer to PDC FIFO address
 * @retval    PDC_SUCCESS      Processing finished successfully.
 * @retval    PDC_ERR_NOT_OPEN R_PDC_Open has not been run.
 * @retval    PDC_ERR_NULL_PTR Argument p_fifo_addr is a NULL pointer.
 * @details   Stores the address of the PDC receive data register (PCDR) in argument p_fifo_addr.
 * @note      None.
 */
pdc_return_t R_PDC_GetFifoAddr (uint32_t *p_fifo_addr)
{
    pdc_return_t ret = PDC_SUCCESS;

    /* Check that the R_PDC_Open has been executed. */
    if (false == is_opened)
    {
        return PDC_ERR_NOT_OPEN;
    }
    
    /* Make sure that the argument is correct. */
    /* To cast in order to compare the address. There is no problem because the information is not lost even if the
     *  cast. */
    if (NULL == p_fifo_addr)
    {
        return PDC_ERR_NULL_PTR;
    }

    /* Get pointer to PDC.PCDR resister address */
    *p_fifo_addr = (uint32_t)&PDC.PCDR.LONG;

    return ret;
} /* End of function R_PDC_GetFifoAddr() */

/**********************************************************************************************************************
 * Function Name: R_PDC_GetVersion
 *****************************************************************************************************************/ /**
 * @brief This function returns the API version number.
 * @return    Version number
 * @details   This function returns the version number of the currently installed PDC FIT module. The version number
 *            is encoded. The first two bytes contain the major version number and the last two bytes contain the minor
 *            version number. For example, if the version number is 4.25, the return value would be 0x00040019.
 * @note      None.
 */
uint32_t R_PDC_GetVersion (void)
{
    return ((((uint32_t) PDC_RX_VERSION_MAJOR) << 16) | ((uint32_t) PDC_RX_VERSION_MINOR));
} /* End of function R_PDC_GetVersion() */

/**
 * Private functions
 */
/***********************************************************************************************************************
 * Function Name: pdc_callback_receive_data_ready
 * Description  : Callback function of the received data ready interrupt.
 * Arguments    : none
 * Return Values: none
 ***********************************************************************************************************************/
static void pdc_callback_receive_data_ready (void * pparam)
{
    pdc_cb_arg_t cb_arg;

    if ((NULL != cb_func.pcb_receive_data_ready) && (FIT_NO_FUNC != cb_func.pcb_receive_data_ready))
    {
        cb_arg.event_id = PDC_EVT_ID_DATAREADY;
        (*cb_func.pcb_receive_data_ready)((void *) &cb_arg);
    }
} /* End of function pdc_callback_receive_data_ready() */

/***********************************************************************************************************************
 * Function Name: pdc_callback_frame_end
 * Description  : Callback function of the frame end interrupt.
 * Arguments    : none
 * Return Values: none
 ***********************************************************************************************************************/
static void pdc_callback_frame_end (void * pparam)
{
    pdc_cb_arg_t cb_arg;
    uint32_t timeout_count = 0;

    /* Check FIFO empty and UDRF flag. */
    do
    {
        if (PDC_SET_STATUS == PDC.PCSR.BIT.UDRF)
        {
            /* Clear the PCSR.FEF flag to 0. */
            if (PDC_SET_STATUS == PDC.PCSR.BIT.FEF)
            {
                PDC.PCSR.BIT.FEF = PDC_CLEAR_STATUS;
            }

            /* call function of error processing. */
            pdc_error_processing();
            return;
        }

        /* Check constant time is passed */
        if (timeout_count >= PDC_EMPTY_LOOP_VALUES)
        {
            /* Clear the PCSR.FEF flag to 0. */
            if (PDC_SET_STATUS == PDC.PCSR.BIT.FEF)
            {
                PDC.PCSR.BIT.FEF = PDC_CLEAR_STATUS;
            }
            
            /* Check registration frame end callback function */
            if ((NULL != cb_func.pcb_frame_end) && (FIT_NO_FUNC != cb_func.pcb_frame_end))
            {
                cb_arg.event_id = PDC_EVT_ID_TIMEOUT;
                (*cb_func.pcb_frame_end)((void *) &cb_arg);
            }
            return;
        }

        /* Increase time out counter */
        timeout_count += 1;
    } while (PDC_FIFO_NOT_EMPTY == PDC.PCSR.BIT.FEMPF);        /* WAIT_LOOP */

    /* After completion of the transfer, clear the PCE bit in the PCCR1 register to 0. */
    PDC.PCCR1.BIT.PCE = PDC_DISABLE_OPERATION;

    /* Clear the FEF flag in the PCSR register to 0. */
    if (PDC_SET_STATUS == PDC.PCSR.BIT.FEF)
    {
        PDC.PCSR.BIT.FEF = PDC_CLEAR_STATUS;
    }

    /* Check registration callback function for frame end interrupt */
    if ((NULL != cb_func.pcb_frame_end) && (FIT_NO_FUNC != cb_func.pcb_frame_end))
    {
        cb_arg.event_id = PDC_EVT_ID_FRAMEEND;
        (*cb_func.pcb_frame_end)((void *) &cb_arg);
    }

} /* End of function pdc_callback_frame_end() */

/***********************************************************************************************************************
 * Function Name: pdc_callback_error
 * Description  : Callback function of the error interrupt.
 * Arguments    : none
 * Return Values: none
 ***********************************************************************************************************************/
static void pdc_callback_error (void * pparam)
{
    /* call function of error processing. */
    pdc_error_processing();
} /* End of function pdc_callback_error() */

/***********************************************************************************************************************
 * Function Name: pdc_error_processing
 * Description  : error processing.
 * Arguments    : none
 * Return Values: none
 ***********************************************************************************************************************/
static void pdc_error_processing (void)
{
    pdc_cb_arg_t cb_arg;

    /* Disabling of PDC operation. */
    PDC.PCCR1.BIT.PCE = PDC_DISABLE_OPERATION;

    /* Check registration callback function for error interrupt */
    if ((NULL != cb_func.pcb_error) && (FIT_NO_FUNC != cb_func.pcb_error))
    {
        cb_arg.event_id = PDC_EVT_ID_ERROR;
        (*cb_func.pcb_error)((void *) &cb_arg);
    }

    /* Checking the overrun flag.(PCSR.OVRF) */
    if (PDC_SET_STATUS == PDC.PCSR.BIT.OVRF)
    {
        /* Check registration callback function for error interrupt */
        if ((NULL != cb_func.pcb_error) && (FIT_NO_FUNC != cb_func.pcb_error))
        {
            cb_arg.event_id = PDC_EVT_ID_OVERRUN;
            (*cb_func.pcb_error)((void *) &cb_arg);
        }

        /* Clear the PCSR.OVRF flag to 0. */
        PDC.PCSR.BIT.OVRF = PDC_CLEAR_STATUS;
    }

    /* Checking the underrun flag.(PCSR.UDRF) */
    if (PDC_SET_STATUS == PDC.PCSR.BIT.UDRF)
    {
        /* Check registration callback function for error interrupt */
        if ((NULL != cb_func.pcb_error) && (FIT_NO_FUNC != cb_func.pcb_error))
        {
            cb_arg.event_id = PDC_EVT_ID_UNDERRUN;
            (*cb_func.pcb_error)((void *) &cb_arg);
        }

        /* Clear the PCSR.UDRF flag to 0. */
        PDC.PCSR.BIT.UDRF = PDC_CLEAR_STATUS;
    }

    /* Checking the vertical line setting error flag.(PCSR.VERF) */
    if (PDC_SET_STATUS == PDC.PCSR.BIT.VERF)
    {
        /* Check registration callback function for error interrupt */
        if ((NULL != cb_func.pcb_error) && (FIT_NO_FUNC != cb_func.pcb_error))
        {
            cb_arg.event_id = PDC_EVT_ID_VERTICALLINE;
            (*cb_func.pcb_error)((void *) &cb_arg);
        }

        /* Clear the PCSR.VERF flag to 0. */
        PDC.PCSR.BIT.VERF = PDC_CLEAR_STATUS;
    }

    /* Checking the horizontal byte setting error flag.(PCSR.HERF) */
    if (PDC_SET_STATUS == PDC.PCSR.BIT.HERF)
    {
        /* Check registration callback function for error interrupt */
        if ((NULL != cb_func.pcb_error) && (FIT_NO_FUNC != cb_func.pcb_error))
        {
            cb_arg.event_id = PDC_EVT_ID_HORIZONTALBYTE;
            (*cb_func.pcb_error)((void *) &cb_arg);
        }

        /* Clear the PCSR.HERF flag to 0. */
        PDC.PCSR.BIT.HERF = PDC_CLEAR_STATUS;
    }

    /* Checking the underrun flag.(PCSR.UDRF) */
    if (PDC_SET_STATUS == PDC.PCSR.BIT.UDRF)
    {
        /* Check registration callback function for error interrupt */
        if ((NULL != cb_func.pcb_error) && (FIT_NO_FUNC != cb_func.pcb_error))
        {
            cb_arg.event_id = PDC_EVT_ID_UNDERRUN;
            (*cb_func.pcb_error)((void *) &cb_arg);
        }

        /* Clear the PCSR.UDRF flag to 0. */
        PDC.PCSR.BIT.UDRF = PDC_CLEAR_STATUS;
    }
} /* End of function pdc_error_processing() */

/***********************************************************************************************************************
 * Function Name: pdc_reset
 * Description  : Initialize internal state and PCSR resister in PDC.
 * Arguments    : none
 * Return Values: PDC_SUCCESS -
 *                    Processing finished successfully.
 *                PDC_ERR_RST_TIMEOUT -
 *                    PDC is resetting after constant time is passed.
 ***********************************************************************************************************************/
static pdc_return_t pdc_reset (void)
{
    uint32_t timeout_count = 0;

    /* Start reset of PDC */
    PDC.PCCR0.BIT.PRST = PDC_RESET;

    /* Check reset release in PDC */
    /* WAIT_LOOP */
    while (PDC_RESET_RELEASE != PDC.PCCR0.BIT.PRST)
    {
        /* Check constant time is passed */
        if (timeout_count >= PDC_RESET_LOOP_VALUES)
        {
            return PDC_ERR_RST_TIMEOUT;
        }

        /* Increase time out counter */
        timeout_count += 1;
    }
    
    timeout_count = 0;

    /* Wait for continuous reset limitation */
    /* WAIT_LOOP */
    while (timeout_count <= PDC_CONT_RESET_LOOP_VALUES)
    {
        /* Increase time out counter */
        timeout_count += 1;
    }

    return PDC_SUCCESS;
} /* End of function pdc_reset() */

/***********************************************************************************************************************
 * Function Name: pdc_int_setting
 * Description  : Makes settings for interrupts used by the PDC.
 * Arguments    : step -
 *                    Which step in setting interrupt
 *                p_data_cfg -
 *                    Pointer to PDC settings data structure
 * Return Values: PDC_SUCCESS -
 *                    Processing finished successfully.
 *                PDC_ERR_INVALID_ARG -
 *                    Parameter values in PDC setting information are invalid.
 *                PDC_ERR_NULL_PTR -
 *                    Argument p_data_cfg is a NULL pointer.
 ***********************************************************************************************************************/
static pdc_return_t pdc_int_setting (pdc_step_t step, pdc_data_cfg_t *p_data_cfg)
{
    pdc_return_t ret;
    pdc_data_cfg_t pcfg_data;

    /* Make sure that the argument is correct. */
    /* To cast in order to compare the address. There is no problem because the information is not lost even if the
     *  cast. */
    if ((uint32_t) NULL == (uint32_t) p_data_cfg)
    {
        return PDC_ERR_NULL_PTR;
    }

    switch (step)
    {
        case PDC_INITIALIZE :

            /* Make sure that the argument is correct. */
            pcfg_data.iupd_select = PDC_ALL_INT_UPDATE;

            /* Set the PCDFI interrupt priority level. */
            pcfg_data.priority.pcdfi_level = p_data_cfg->priority.pcdfi_level;

            pcfg_data.inticu_req = p_data_cfg->inticu_req;
            pcfg_data.intpdc_req = p_data_cfg->intpdc_req;

            if ((true == p_data_cfg->inticu_req.pcfei_ien) || (true == p_data_cfg->inticu_req.pceri_ien))
            {
                /* Set the GROUPBL0 interrupt priority level. */
                pcfg_data.priority.groupbl0_level = p_data_cfg->priority.groupbl0_level;
            }
            else
            {
                /* Remove GROUPBL0 interrupt priority level from update select */
                pcfg_data.iupd_select &= (~PDC_GROUPBL0_IPR_UPDATE);
            }

            /* call function of interrupt setting. */
            ret = pdc_int_setting_sub(&pcfg_data);
        break;

        case PDC_CHANGE_SETTING :

            /* call function of interrupt setting. */
            ret = pdc_int_setting_sub(p_data_cfg);
        break;

            /* Illegal step is not supported */
        default :
            ret = PDC_ERR_INVALID_ARG;
        break;
    }
    return ret;
} /* End of function pdc_int_setting() */

/***********************************************************************************************************************
 * Function Name: pdc_int_setting_sub
 * Description  : Makes settings for interrupts used by the PDC.
 * Arguments    : p_data_cfg -
 *                    Pointer to PDC settings data structure
 * Return Values: PDC_SUCCESS -
 *                    Processing finished successfully.
 *                PDC_ERR_INVALID_ARG -
 *                    Parameter values in PDC setting information are invalid.
 *                PDC_ERR_NULL_PTR -
 *                    Argument p_data_cfg is a NULL pointer.
 ***********************************************************************************************************************/
static pdc_return_t pdc_int_setting_sub (pdc_data_cfg_t *p_data_cfg)
{
    bsp_int_ctrl_t grpbl0;
    
    /* Initialize local variable */
    grpbl0.ipl = 0;
    
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif

    /* Make sure that the argument is correct. */
    /* To cast in order to compare the address. There is no problem because the information is not lost even if the
     *  cast. */
    if ((uint32_t) NULL == (uint32_t) p_data_cfg)
    {
        return PDC_ERR_NULL_PTR;
    }
    
    /* Set the GROUPBL0 interrupt priority level. */
    grpbl0.ipl = p_data_cfg->priority.groupbl0_level;
    
    if (PDC_PCDFI_IPR_UPDATE == (p_data_cfg->iupd_select & PDC_PCDFI_IPR_UPDATE))
    {
        /* Check the argument. When the argument is correct, set the PCDFI interrupt priority level. */
        if (PDC_IPR_LV_UPPER_LIMIT < p_data_cfg->priority.pcdfi_level)
        {
            return PDC_ERR_INVALID_ARG;
        }
        IPR(PDC, PCDFI)= p_data_cfg->priority.pcdfi_level;
    }

    if (PDC_GROUPBL0_IPR_UPDATE == (p_data_cfg->iupd_select & PDC_GROUPBL0_IPR_UPDATE))
    {
        /* Check the argument. When the argument is correct, set the GROUPBL0 interrupt priority level. */
        if (PDC_IPR_LV_UPPER_LIMIT < grpbl0.ipl)
        {
            return PDC_ERR_INVALID_ARG;
        }
        R_BSP_InterruptControl(BSP_INT_SRC_BL0_PDC_PCFEI, BSP_INT_CMD_GROUP_INTERRUPT_ENABLE,
                (void *) &grpbl0.ipl);
    }

    if (PDC_PCFEI_IEN_UPDATE == (p_data_cfg->iupd_select & PDC_PCFEI_IEN_UPDATE))
    {
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
        ICU.GENBL0.BIT.EN30 = 0; /* Interrupt request(GRPBL0.IS30) is also cleared.  */
        ICU.GENBL0.BIT.EN30 = p_data_cfg->inticu_req.pcfei_ien;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    }

    if (PDC_PCERI_IEN_UPDATE == (p_data_cfg->iupd_select & PDC_PCERI_IEN_UPDATE))
    {
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
        ICU.GENBL0.BIT.EN31 = 0; /* Interrupt request(GRPBL0.IS31) is also cleared.  */
        ICU.GENBL0.BIT.EN31 = p_data_cfg->inticu_req.pceri_ien;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    }

    if (PDC_PCDFI_IEN_UPDATE == (p_data_cfg->iupd_select & PDC_PCDFI_IEN_UPDATE))
    {
        IR(PDC, PCDFI)= 0; /* Interrupt request is cleared. */
        if (1 == p_data_cfg->inticu_req.pcdfi_ien)
        {
            R_BSP_InterruptRequestEnable(VECT(PDC, PCDFI));
        }
        else
        {
            R_BSP_InterruptRequestDisable(VECT(PDC, PCDFI));
        }
    }

    /* The PCCR0 register should only be set while the PCE bit in the PCCR1 register is 0. */
    PDC.PCCR1.BIT.PCE = PDC_DISABLE_OPERATION;

    if (PDC_DFIE_IEN_UPDATE == (p_data_cfg->iupd_select & PDC_DFIE_IEN_UPDATE))
    {
        PDC.PCCR0.BIT.DFIE = p_data_cfg->intpdc_req.dfie_ien;
    }

    if (PDC_FEIE_IEN_UPDATE == (p_data_cfg->iupd_select & PDC_FEIE_IEN_UPDATE))
    {
        PDC.PCCR0.BIT.FEIE = p_data_cfg->intpdc_req.feie_ien;
    }

    if (PDC_OVIE_IEN_UPDATE == (p_data_cfg->iupd_select & PDC_OVIE_IEN_UPDATE))
    {
        PDC.PCCR0.BIT.OVIE = p_data_cfg->intpdc_req.ovie_ien;
    }

    if (PDC_UDRIE_IEN_UPDATE == (p_data_cfg->iupd_select & PDC_UDRIE_IEN_UPDATE))
    {
        PDC.PCCR0.BIT.UDRIE = p_data_cfg->intpdc_req.udrie_ien;
    }

    if (PDC_VERIE_IEN_UPDATE == (p_data_cfg->iupd_select & PDC_VERIE_IEN_UPDATE))
    {
        PDC.PCCR0.BIT.VERIE = p_data_cfg->intpdc_req.verie_ien;
    }

    if (PDC_HERIE_IEN_UPDATE == (p_data_cfg->iupd_select & PDC_HERIE_IEN_UPDATE))
    {
        PDC.PCCR0.BIT.HERIE = p_data_cfg->intpdc_req.herie_ien;
    }
    return PDC_SUCCESS;
} /* End of function pdc_int_setting_sub() */

/***********************************************************************************************************************
 * Function Name: pdc_initialize
 * Description  : Makes initial settings for using the PDC.
 * Arguments    : p_data_cfg -
 *                    Pointer to PDC settings data structure
 * Return Values: PDC_SUCCESS -
 *                    Processing finished successfully.
 *                PDC_ERR_INVALID_ARG -
 *                    Argument p_data_cfg is a NULL pointer.
 ***********************************************************************************************************************/
static pdc_return_t pdc_initialize (pdc_data_cfg_t *p_data_cfg)
{
    pdc_return_t ret;
    uint16_t value;
    uint16_t pckdiv_value;

    /* Make sure that the argument is correct. */
    /* To cast in order to compare the address. There is no problem because the information is not lost even if the
     *  cast. */
    if ((uint32_t) NULL == (uint32_t) p_data_cfg)
    {
        return PDC_ERR_NULL_PTR;
    }

    if (PDC_VST_UPPER_LIMIT < p_data_cfg->capture_pos.vst_position)
    {
        return PDC_ERR_INVALID_ARG;
    }

    if (PDC_HST_UPPER_LIMIT < p_data_cfg->capture_pos.hst_position)
    {
        return PDC_ERR_INVALID_ARG;
    }

    if ((PDC_VSZ_LOWER_LIMIT > p_data_cfg->capture_size.vsz_size)
            || (PDC_VSZ_UPPER_LIMIT < p_data_cfg->capture_size.vsz_size))
    {
        return PDC_ERR_INVALID_ARG;
    }

    if ((PDC_HSZ_LOWER_LIMIT > p_data_cfg->capture_size.hsz_size)
            || (PDC_HSZ_UPPER_LIMIT < p_data_cfg->capture_size.hsz_size))
    {
        return PDC_ERR_INVALID_ARG;
    }

    /* The range of values that dealt with in the register is 12-bit.
     Value obtained by adding the 12-bit to 12-bit is 16 bits or less.
     Because the information is not lost within the range to be handled, can be cast. */
    value = (uint16_t) (p_data_cfg->capture_pos.vst_position + p_data_cfg->capture_size.vsz_size);
    if (PDC_VSTVSZ_MIX_UPPER_LIMIT < value)
    {
        return PDC_ERR_INVALID_ARG;
    }

    /* The range of values that dealt with in the register is 12-bit.
     Value obtained by adding the 12-bit to 12-bit is 16 bits or less.
     Because the information is not lost within the range to be handled, can be cast. */
    value = (uint16_t) (p_data_cfg->capture_pos.hst_position + p_data_cfg->capture_size.hsz_size);
    if (PDC_HSTHSZ_MIX_UPPER_LIMIT < value)
    {
        return PDC_ERR_INVALID_ARG;
    }

    /* The PCCR0 register should only be set while the PCE bit in the PCCR1 register is 0.
     The VCR register should be set while the PCE bit in the PCCR1 register is 0.
     The HCR register should be set while the PCE bit in the PCCR1 register is 0. */
    PDC.PCCR1.BIT.PCE = PDC_DISABLE_OPERATION;

    /* Calculate PCKO frequency division ratio */
    pckdiv_value = ((PDC_CFG_PCKO_DIV / 2) - 1);

    /* Set PCKO frequency division ratio */
    PDC.PCCR0.BIT.PCKDIV = pckdiv_value;

    /* Enables PCKO output */
    PDC.PCCR0.BIT.PCKOE = PDC_ENABLE_PCKO_OUTPUT;

    /* Enables PIXCLK input. */
    PDC.PCCR0.BIT.PCKE = PDC_ENABLE_PIXCLK_INPUT;

    /* Resets the PDC. */
    ret = pdc_reset();
    if (PDC_SUCCESS != ret)
    {
        return ret;
    }

    /* Sets the vertical and horizontal capture ranges. */
    PDC.VCR.BIT.VST = p_data_cfg->capture_pos.vst_position;
    PDC.HCR.BIT.HST = p_data_cfg->capture_pos.hst_position;

    PDC.VCR.BIT.VSZ = p_data_cfg->capture_size.vsz_size;
    PDC.HCR.BIT.HSZ = p_data_cfg->capture_size.hsz_size;

    /* Sets the VSYNC and HSYNC signal polarity. */
    PDC.PCCR0.BIT.VPS = p_data_cfg->vps_select;
    PDC.PCCR0.BIT.HPS = p_data_cfg->hps_select;

    /* Sets the endianness. */
#if (defined(__BIG) || defined(__RX_BIG_ENDIAN__))
    PDC.PCCR0.BIT.EDS = PDC_EDS_BIG_ENDIAN;
#else
    PDC.PCCR0.BIT.EDS = PDC_EDS_LITTLE_ENDIAN;
#endif

    return ret;
} /* End of function pdc_initialize() */

/***********************************************************************************************************************
 * Function Name: pdc_capture_start
 * Description  : After resetting interrupt settings and the PDC, enables PDC receive operation to start data capture.
 * Arguments    : none
 * Return Values: PDC_SUCCESS -
 *                    Processing finished successfully.
 *                PDC_ERR_RST_TIMEOUT -
 *                    PDC is resetting after constant time is passed.
 ***********************************************************************************************************************/
static pdc_return_t pdc_capture_start (void)
{
    pdc_return_t ret;
    uint32_t value;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif

    /* The PCCR0 register should only be set while the PCE bit in the PCCR1 register is 0. */
    PDC.PCCR1.BIT.PCE = PDC_DISABLE_OPERATION;

    /* Internal interrupt request is cleared. */
    value = PDC.PCCR0.LONG;
    PDC.PCCR0.BIT.DFIE = 0;
    PDC.PCCR0.BIT.FEIE = 0;
    PDC.PCCR0.BIT.OVIE = 0;
    PDC.PCCR0.BIT.UDRIE = 0;
    PDC.PCCR0.BIT.VERIE = 0;
    PDC.PCCR0.BIT.HERIE = 0;
    value &= 0x000007F0;
    PDC.PCCR0.LONG |= value;
    IR(PDC, PCDFI)= 0; /* Interrupt request is cleared. */
    if (1 == ICU.GENBL0.BIT.EN30)
    {
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
        ICU.GENBL0.BIT.EN30 = 0; /* Interrupt request(GRPBL0.IS30) is also cleared.  */
        ICU.GENBL0.BIT.EN30 = 1;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    }
    if (1 == ICU.GENBL0.BIT.EN31)
    {
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
        ICU.GENBL0.BIT.EN31 = 0; /* Interrupt request(GRPBL0.IS31) is also cleared.  */
        ICU.GENBL0.BIT.EN31 = 1;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
        R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    }

    /* Resets the PDC. */
    ret = pdc_reset();
    if (PDC_SUCCESS == ret)
    {
        /* Enabling of PDC operation. */
        PDC.PCCR1.BIT.PCE = PDC_ENABLE_OPERATION;
    }

    return ret;
} /* End of function pdc_capture_start() */

/***********************************************************************************************************************
 * Function Name: pdc_change_position_size
 * Description  : Resets the capture start position and size settings.
 * Arguments    : p_data_cfg -
 *                    Pointer to PDC settings data structure
 * Return Values: PDC_SUCCESS -
 *                    Processing finished successfully.
 *                PDC_ERR_INVALID_ARG -
 *                    Parameter values in PDC setting information are invalid.
 *                PDC_ERR_NULL_PTR -
 *                    Argument p_data_cfg is a NULL pointer.
 ***********************************************************************************************************************/
static pdc_return_t pdc_change_position_size (pdc_data_cfg_t *p_data_cfg)
{
    uint16_t value;

    /* Make sure that the argument is correct. */
    /* To cast in order to compare the address. There is no problem because the information is not lost even if the
     *  cast. */
    if ((uint32_t) NULL == (uint32_t) p_data_cfg)
    {
        return PDC_ERR_NULL_PTR;
    }

    if (PDC_VST_UPPER_LIMIT < p_data_cfg->capture_pos.vst_position)
    {
        return PDC_ERR_INVALID_ARG;
    }

    if (PDC_HST_UPPER_LIMIT < p_data_cfg->capture_pos.hst_position)
    {
        return PDC_ERR_INVALID_ARG;
    }

    if ((PDC_VSZ_LOWER_LIMIT > p_data_cfg->capture_size.vsz_size)
            || (PDC_VSZ_UPPER_LIMIT < p_data_cfg->capture_size.vsz_size))
    {
        return PDC_ERR_INVALID_ARG;
    }

    if ((PDC_HSZ_LOWER_LIMIT > p_data_cfg->capture_size.hsz_size)
            || (PDC_HSZ_UPPER_LIMIT < p_data_cfg->capture_size.hsz_size))
    {
        return PDC_ERR_INVALID_ARG;
    }

    /* The range of values that dealt with in the register is 12-bit.
     Value obtained by adding the 12-bit to 12-bit is 16 bits or less.
     Because the information is not lost within the range to be handled, can be cast. */
    value = (uint16_t) (p_data_cfg->capture_pos.vst_position + p_data_cfg->capture_size.vsz_size);
    if (PDC_VSTVSZ_MIX_UPPER_LIMIT < value)
    {
        return PDC_ERR_INVALID_ARG;
    }

    /* The range of values that dealt with in the register is 12-bit.
     Value obtained by adding the 12-bit to 12-bit is 16 bits or less.
     Because the information is not lost within the range to be handled, can be cast. */
    value = (uint16_t) (p_data_cfg->capture_pos.hst_position + p_data_cfg->capture_size.hsz_size);
    if (PDC_HSTHSZ_MIX_UPPER_LIMIT < value)
    {
        return PDC_ERR_INVALID_ARG;
    }

    /* The VCR register should be set while the PCE bit in the PCCR1 register is 0.
     The HCR register should be set while the PCE bit in the PCCR1 register is 0. */
    PDC.PCCR1.BIT.PCE = PDC_DISABLE_OPERATION;

    /* Sets the vertical and horizontal capture ranges. */
    PDC.VCR.BIT.VST = p_data_cfg->capture_pos.vst_position;
    PDC.HCR.BIT.HST = p_data_cfg->capture_pos.hst_position;

    PDC.VCR.BIT.VSZ = p_data_cfg->capture_size.vsz_size;
    PDC.HCR.BIT.HSZ = p_data_cfg->capture_size.hsz_size;

    return PDC_SUCCESS;
} /* End of function pdc_change_position_size() */

/***********************************************************************************************************************
 * Function Name: pdc_clear_status
 * Description  : Clears PDC status information indicated by argument p_stat.
 * Arguments    : p_stat -
 *                    Pointer to PDC status structure
 * Return Values: PDC_SUCCESS -
 *                    Processing finished successfully.
 *                PDC_ERR_NULL_PTR -
 *                    The argument p_data_cfg or p_stat is a NULL pointer.
 ***********************************************************************************************************************/
static pdc_return_t pdc_clear_status (pdc_stat_t *p_stat)
{
    volatile uint32_t read_data;

    /* Make sure that the argument is correct. */
    /* To cast in order to compare the address. There is no problem because the information is not lost even if the
     *  cast. */
    if ((uint32_t) NULL == (uint32_t) p_stat)
    {
        return PDC_ERR_NULL_PTR;
    }

    /* Read PCSR resister */
    read_data = PDC.PCSR.LONG;

    /* Clear PDC Status Register(PCSR). */
    if (true == p_stat->pcsr_stat.frame_end)
    {
        PDC.PCSR.BIT.FEF = 0;
    }
    if (true == p_stat->pcsr_stat.overrun)
    {
        PDC.PCSR.BIT.OVRF = 0;
    }
    if (true == p_stat->pcsr_stat.underrun)
    {
        PDC.PCSR.BIT.UDRF = 0;
    }
    if (true == p_stat->pcsr_stat.verf_error)
    {
        PDC.PCSR.BIT.VERF = 0;
    }
    if (true == p_stat->pcsr_stat.herf_error)
    {
        PDC.PCSR.BIT.HERF = 0;
    }
    return PDC_SUCCESS;
} /* End of function pdc_clear_status() */

/***********************************************************************************************************************
 * Function Name: pdc_get_status
 * Description  : Writes PDC status information to the pointer position indicated by argument p_stat.
 * Arguments    : p_stat -
 *                    Pointer to PDC status structure
 * Return Values: PDC_SUCCESS -
 *                    Processing finished successfully.
 *                PDC_ERR_NULL_PTR -
 *                    The argument p_data_cfg or p_stat is a NULL pointer.
 ***********************************************************************************************************************/
static pdc_return_t pdc_get_status (pdc_stat_t *p_stat)
{
    /* Make sure that the argument is correct. */
    /* To cast in order to compare the address. There is no problem because the information is not lost even if the
     *  cast. */
    if ((uint32_t) NULL == (uint32_t) p_stat)
    {
        return PDC_ERR_NULL_PTR;
    }

    /* Read the PDC Status Register(PCSR). */
    /* Since the value of the register is defined by the BIT, the value to be handled is 1 or 0.
     Because the information is not lost even if the cast to bool, it is okay to cast. */
    p_stat->pcsr_stat.frame_busy = (bool) PDC.PCSR.BIT.FBSY;
    p_stat->pcsr_stat.fifo_empty = (bool) PDC.PCSR.BIT.FEMPF;
    p_stat->pcsr_stat.frame_end = (bool) PDC.PCSR.BIT.FEF;
    p_stat->pcsr_stat.overrun = (bool) PDC.PCSR.BIT.OVRF;
    p_stat->pcsr_stat.underrun = (bool) PDC.PCSR.BIT.UDRF;
    p_stat->pcsr_stat.verf_error = (bool) PDC.PCSR.BIT.VERF;
    p_stat->pcsr_stat.herf_error = (bool) PDC.PCSR.BIT.HERF;

    /* Read the PDC Pin Monitor Register(PCMONR). */
    /* Since the value of the register is defined by the BIT, the value to be handled is 1 or 0.
     Because the information is not lost even if the cast to bool, it is okay to cast. */
    p_stat->pcmonr_stat.vsync = (bool) PDC.PCMONR.BIT.VSYNC;
    p_stat->pcmonr_stat.hsync = (bool) PDC.PCMONR.BIT.HSYNC;

    return PDC_SUCCESS;
} /* End of function pdc_get_status() */

/***********************************************************************************************************************
 * Function Name: pdc_module_enable
 * Description  : Cancels PDC module stop state.
 * Arguments    : none
 * Return Values: none
 ***********************************************************************************************************************/
static void pdc_module_enable (void)
{
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif
    /* Enable writing to MSTP registers. And release from the module-stop state of PDC.
     After that disable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    MSTP( PDC ) = 0;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

} /* End of function pdc_module_enable() */

/***********************************************************************************************************************
 * Function Name: pdc_module_disable
 * Description  : Stops PDC module.
 * Arguments    : none
 * Return Values: none
 ***********************************************************************************************************************/
static void pdc_module_disable (void)
{
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    bsp_int_ctrl_t int_ctrl;
#endif
    /* Enable writing to MSTP registers and stop module of PDC.
     After that disable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_DISABLE, &int_ctrl);
#endif
    MSTP( PDC ) = 1;
#if ((R_BSP_VERSION_MAJOR == 5) && (R_BSP_VERSION_MINOR >= 30)) || (R_BSP_VERSION_MAJOR >= 6)
    R_BSP_InterruptControl(BSP_INT_SRC_EMPTY, BSP_INT_CMD_FIT_INTERRUPT_ENABLE, &int_ctrl);
#endif
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

} /* End of function pdc_module_disable() */

/***********************************************************************************************************************
 * Outline      : Interrupt PCDFI handler
 * Function Name: pdc_pcdfi_isr
 * Description  : Types of interrupt requests from receive data ready(PCDFI).
 *                If the pointer to function is FIT_NO_FUNC or NULL,
 *                You should write a program under "do something" in this
 *                pdc_pcdfi_isr function.
 * Arguments    : none
 * Return Values: none
 ***********************************************************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(pdc_pcdfi_isr, VECT(PDC, PCDFI))
R_BSP_ATTRIB_STATIC_INTERRUPT void pdc_pcdfi_isr(void)
{
    /* To cast in order to compare the address. There is no problem because the information is not lost even if the
     *  cast. */
    if (((uint32_t) NULL != (uint32_t) ppcdfi_handlers) && ((uint32_t) FIT_NO_FUNC != (uint32_t) ppcdfi_handlers))
    {
        ((pcdfi_callback_t) ppcdfi_handlers)((void *) NULL);
    }
    else
    {
        /* do something */
    }
} /* End of function pdc_pcdfi_isr() */

/* End of File */
