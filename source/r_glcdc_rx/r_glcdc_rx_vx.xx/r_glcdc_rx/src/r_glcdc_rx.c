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
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_glcdc_rx.c
 * Version      : 1.30
 * Description  : GLCDC API functions.
 ************************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version   Description
 *         : 01.10.2017 1.00      First Release
 *         : 04.04.2019 1.10      Added the comment to loop statement.
 *                                Added support for GNUC and ICCRX.
 *                                Deleted the inline expansion of R_GLCDC_GetVersion.
 *         : 20.09.2019 1.30      Changed the comment based on Doxygen format.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 Includes <System Includes> , "Project Includes"
 ************************************************************************************************************************/
#include "r_glcdc_private.h"

/***********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/
/* GLCD control block */
glcdc_ctrl_t g_ctrl_blk =
{
    GLCDC_STATE_CLOSED,
    false,
    { 0U, 0U }, 0U, 0U,
    { false, false },
    NULL,
    false,
    { false, false, false } };

/***********************************************************************************************************************
 Private global variables and functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *Function Name: R_GLCDC_Open
 *******************************************************************************************************************//**
 *@brief This function initializes the GLCDC FIT module. This function must be called before calling
 *any other API functions.
 *@param[in] *p_cfg Pointer to the GLCDC setting data structure.
 *@retval GLCDC_SUCCESS                    Processing has been completed successfully.
 *@retval GLCDC_ERR_INVALID_PTR            The p_cfg parameter is NULL pointer.
 *@retval GLCDC_ERR_LOCK_FUNC              GLCDC resource is used by another process
 *@retval GLCDC_ERR_INVALID_ARG            The argument for the GLCDC setting data is invalid.
 *@retval GLCDC_ERR_INVALID_MODE           Function cannot be executed in this mode.
 *@retval GLCDC_ERR_INVALID_TIMING_SETTING Timing setting of the panel output signal is invalid.
 *@retval GLCDC_ERR_INVALID_LAYER_SETTING  Graphics screen setting is invalid.
 *@retval GLCDC_ERR_INVALID_ALIGNMENT      Start address of the frame buffer is invalid.
 *@retval GLCDC_ERR_INVALID_GAMMA_SETTING  Gamma correction setting is invalid.
 *@retval GLCDC_ERR_INVALID_CLUT_ACCESS    CLUT memory setting is invalid.
 *@retval GLCDC_ERR_INVALID_BLEND_SETTING  Setting for blending is invalid.
 *@details This function releases the GLCDC module-stop state and software reset to enable the GLCDC operation.
 *Then it specifies the panel clock, the panel output signal timing, background screen, graphics screen, CLUT memory,
 *output data format, correction processing, and interrupts used by the GLCDC.
 *This function can be executed when the mode is 'GLCDC_STATE_CLOSED'.
 *When processing in this function has been completed successfully, a transition is made to
 *'GLCDC_STATE_NOT_DISPLAYING'.
 *@note (1) If the target graphics screen is disabled by setting p_base to NULL in this function\n
 *The graphics screen setting in the R_GLCDC_LayerChange function and CLUT memory updates in the R_GLCDC_ClutUpdate
 *function becomes disabled.
 *To enable the disabled graphics, execute the R_GLCDC_Open function again and set the target graphics screen
 *to be enabled.\n
 *(2) Notes on macro line offset setting\n
 *On the hardware specification, since data is read from the frame buffer for every 64 bytes, set a multiple of 64 for
 *structure member input.offset (macro line offset).
 *If it is not possible to observe this restriction, refer to the application note for details.
 **********************************************************************************************************************/
glcdc_err_t R_GLCDC_Open(glcdc_cfg_t const * const p_cfg)
{
    uint32_t frame;
    glcdc_interrupt_cfg_t initial_interrupt;

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)
    glcdc_err_t err = GLCDC_SUCCESS;
#endif

    /* Status check */
    if (GLCDC_STATE_CLOSED != g_ctrl_blk.state)
    {
        return GLCDC_ERR_INVALID_MODE;
    }

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)
    /* Argument check */
    if (NULL == p_cfg)
    {
        return GLCDC_ERR_INVALID_PTR;
    }
#endif

    /* Store position information to the control block (it is necessary to set the layer and blending section later) */
    g_ctrl_blk.active_start_pos.x = (int16_t) (p_cfg->output.htiming.back_porch + p_cfg->output.htiming.sync_width);
    g_ctrl_blk.active_start_pos.y = (int16_t) (p_cfg->output.vtiming.back_porch + p_cfg->output.vtiming.sync_width);
    g_ctrl_blk.hsize = p_cfg->output.htiming.display_cyc;
    g_ctrl_blk.vsize = p_cfg->output.vtiming.display_cyc;

    /* Subscribe each interrupt function  */
    if (false == g_ctrl_blk.is_entry)
    {
        R_BSP_InterruptWrite (BSP_INT_SRC_AL1_GLCDC_VPOS, (bsp_int_cb_t) r_glcdc_line_detect_isr);
        R_BSP_InterruptWrite (BSP_INT_SRC_AL1_GLCDC_GR1UF, (bsp_int_cb_t) r_glcdc_underflow_1_isr);
        R_BSP_InterruptWrite (BSP_INT_SRC_AL1_GLCDC_GR2UF, (bsp_int_cb_t) r_glcdc_underflow_2_isr);

        g_ctrl_blk.is_entry = true;
    }

    /* Save status of frame buffer read enable */
    if (NULL == p_cfg->input[GLCDC_FRAME_LAYER_1].p_base)
    {
        g_ctrl_blk.graphics_read_enable[GLCDC_FRAME_LAYER_1] = false;
    }
    else
    {
        g_ctrl_blk.graphics_read_enable[GLCDC_FRAME_LAYER_1] = true;
    }

    if (NULL == p_cfg->input[GLCDC_FRAME_LAYER_2].p_base)
    {
        g_ctrl_blk.graphics_read_enable[GLCDC_FRAME_LAYER_2] = false;
    }
    else
    {
        g_ctrl_blk.graphics_read_enable[GLCDC_FRAME_LAYER_2] = true;
    }

    /* Save callback function */
    g_ctrl_blk.p_callback = p_cfg->p_callback;

    /* Save setting of interrupt */
    g_ctrl_blk.interrupt.vpos_enable = p_cfg->interrupt.vpos_enable;
    g_ctrl_blk.interrupt.gr1uf_enable = p_cfg->interrupt.gr1uf_enable;
    g_ctrl_blk.interrupt.gr2uf_enable = p_cfg->interrupt.gr2uf_enable;

    /* If one of the interrupt setting is enable, setting value is set after first vpos interrupt */
    if ((true == p_cfg->interrupt.vpos_enable) || (true == p_cfg->interrupt.gr1uf_enable)
            || (true == p_cfg->interrupt.gr2uf_enable))
    {
        g_ctrl_blk.first_vpos_interrupt_flag = false;
        initial_interrupt.vpos_enable = true;
        initial_interrupt.gr1uf_enable = false;
        initial_interrupt.gr2uf_enable = false;
    }
    else
    {
        g_ctrl_blk.first_vpos_interrupt_flag = true;
        initial_interrupt.vpos_enable = false;
        initial_interrupt.gr1uf_enable = false;
        initial_interrupt.gr2uf_enable = false;
    }

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)
    /* Check parameters */
    err = r_glcdc_open_param_check (p_cfg);
    if (GLCDC_SUCCESS != err)
    {
        return err;
    }
#endif

    /* Check GLCDC resource is locked by another process */
    if (false == R_BSP_HardwareLock ((mcu_lock_t) BSP_LOCK_GLCDC))
    {
        return GLCDC_ERR_LOCK_FUNC;
    }

    /* Supply the peripheral clock to the GLCD module */
    r_glcdc_power_on ();

    /* Release GLCD from a SW reset status. */
    r_glcdc_release_software_reset ();

    /* Set the dot clock frequency */
    r_glcdc_clock_set (p_cfg);

    /* Set the panel signal timing */
    r_glcdc_sync_signal_set (p_cfg);

    /* Configure the background screen */
    r_glcdc_background_screen_set (p_cfg);

    /* Configure the graphics plane layers */

    /* WAIT_LOOP */
    for (frame = 0; frame <= GLCDC_FRAME_LAYER_2; frame++)
    {
        r_glcdc_graphics_layer_set (&p_cfg->input[frame], (glcdc_frame_layer_t) frame);
        r_glcdc_blend_condition_set (&p_cfg->blend[frame], (glcdc_frame_layer_t) frame);
        r_glcdc_graphics_chromakey_set (&p_cfg->chromakey[frame], (glcdc_frame_layer_t) frame);
        r_glcdc_clut_update (&p_cfg->clut[frame], (glcdc_frame_layer_t) frame);
    }

    /* Configure the output control block */
    r_glcdc_output_block_set (p_cfg);

    /* Configure the color correction setting (brightness, brightness and gamma correction) */
    r_glcdc_brightness_correction (&p_cfg->output.brightness);
    r_glcdc_contrast_correction (&p_cfg->output.contrast);
    r_glcdc_gamma_correction (&p_cfg->output.gamma);

    /* Set the line number which is suppose to happen the line detect interrupt */
    r_glcdc_line_detect_number_set (
            (uint16_t) (((p_cfg->output.vtiming.sync_width + p_cfg->output.vtiming.back_porch)
                    + p_cfg->output.vtiming.display_cyc) + BG_PLANE_HSYNC_POS_MIN));

    /* Enable the GLCD detections and interrupts */
    r_glcdc_detect_setting (&p_cfg->detection);
    r_glcdc_interrupt_setting (&initial_interrupt);

    /* Allow reading of graphics data */
    r_glcdc_graphics_read_enable ();

    /* Change GLCDC driver state */
    g_ctrl_blk.state = GLCDC_STATE_NOT_DISPLAYING;

    return GLCDC_SUCCESS;
} /* End of function R_GLCDC_Open() */

/***********************************************************************************************************************
 *Function Name: R_GLCDC_Close
 *******************************************************************************************************************//**
 *@brief This function closes the GLCDC FIT module.
 *@param None.
 *@retval GLCDC_SUCCESS             Processing has been completed successfully.
 *@retval GLCDC_ERR_NOT_OPEN        R_GLCDC_Open has not been executed.
 *@retval GLCDC_ERR_INVALID_MODE    Function cannot be executed in this mode.
 *@details To close the GLCDC FIT module, this function disables interrupts used by the GLCDC. Then it executes
 *the software reset and place the GLCDC in the module-stop state. This function can be executed when the mode
 *is 'GLCDC_STATE_NOT_DISPLAYING'.
 *When processing in this function has been completed successfully, a transition is made to 'GLCDC_STATE_CLOSED'.
 *@note When this function is executed, registers except registers associated with the CLUT memory are initialized.
 *To enable GLCDC operation again, specify necessary settings again when the R_GLCDC_Open function is executed.
 **********************************************************************************************************************/
glcdc_err_t R_GLCDC_Close(void)
{
    glcdc_interrupt_cfg_t interrupt;
    glcdc_detect_cfg_t detection;

    /* Status check */
    if (GLCDC_STATE_CLOSED == g_ctrl_blk.state)
    {
        return GLCDC_ERR_NOT_OPEN;
    }
    if (GLCDC_STATE_DISPLAYING == g_ctrl_blk.state)
    {
        return GLCDC_ERR_INVALID_MODE;
    }

    /* Disable the GLCD detects */
    detection.vpos_detect = false;
    detection.gr1uf_detect = false;
    detection.gr2uf_detect = false;
    r_glcdc_detect_setting (&detection);

    /* interrupt disable */
    interrupt.vpos_enable = false;
    interrupt.gr1uf_enable = false;
    interrupt.gr2uf_enable = false;

    r_glcdc_interrupt_setting (&interrupt);

    /* Reset the GLCD hardware */
    r_glcdc_software_reset ();

    /* Halt the peripheral clock to the GLCD module */
    r_glcdc_power_off ();

    /* Release GLCDC resource */
    R_BSP_HardwareUnlock ((mcu_lock_t) BSP_LOCK_GLCDC);

    /* Change GLCDC driver state */
    g_ctrl_blk.state = GLCDC_STATE_CLOSED;

    return GLCDC_SUCCESS;
} /* End of function R_GLCDC_Close() */

/***********************************************************************************************************************
 *Function Name: R_GLCDC_Control
 *******************************************************************************************************************//**
 *@brief This function performs processing according to the control command.
 *@param[in] cmd Control command to specify.
 *@param[in] p_args Pointer to the setting parameters structure.
 *@retval GLCDC_SUCCESS           Processing has been completed successfully.
 *@retval GLCDC_ERR_INVALID_PTR   The p_args parameter is NULL pointer.
 *@retval GLCDC_ERR_INVALID_ARG   The argument set is invalid.
 *@retval GLCDC_ERR_INVALID_MODE  Function cannot be executed in this mode.
 *@retval GLCDC_ERR_NOT_OPEN      R_GLCDC_Open has not been executed.
 *@retval GLCDC_ERR_INVALID_UPDATE_TIMING Update timing of the register is invalid.
 *@details This function performs GLCDC control processing according to the control command.
 *@note When the GLCDC_CMD_STOP_DISPLAY command is executed, the GLCDC stops its operation after the frame end of
 *background generating block. To enable GLCDC operation again, wait for the frame end of an output signal to
 *the LCD panel, and then enable GLCDC operation.
 *Otherwise, the GLCDC may not operate correctly depending on the LCD panel used.
 **********************************************************************************************************************/
glcdc_err_t R_GLCDC_Control(glcdc_control_cmd_t cmd, void const * const p_args)
{
    glcdc_detect_cfg_t *p_detection;

    if (GLCDC_STATE_CLOSED == g_ctrl_blk.state)
    {
        return GLCDC_ERR_NOT_OPEN;
    }

    switch (cmd)
    {
        case GLCDC_CMD_START_DISPLAY:

            if (GLCDC_STATE_DISPLAYING == g_ctrl_blk.state)
            {
                return GLCDC_ERR_INVALID_MODE;
            }

            /* Change GLCDC driver state */
            g_ctrl_blk.state = GLCDC_STATE_DISPLAYING;

            /* Start to output the vertical and horizontal synchronization signals and screen data. */
            r_glcdc_bg_operation_enable ();

            break;

        case GLCDC_CMD_STOP_DISPLAY:

            if (GLCDC_STATE_NOT_DISPLAYING == g_ctrl_blk.state)
            {
                return GLCDC_ERR_INVALID_MODE;
            }

            /* Return immediately if the register is being updated */
            if (true == r_glcdc_is_gr_plane_updating (GLCDC_FRAME_LAYER_1))
            {
                return GLCDC_ERR_INVALID_UPDATE_TIMING;
            }
            if (true == r_glcdc_is_gr_plane_updating (GLCDC_FRAME_LAYER_2))
            {
                return GLCDC_ERR_INVALID_UPDATE_TIMING;
            }
            if (true == r_glcdc_is_output_ctrl_updating ())
            {
                return GLCDC_ERR_INVALID_UPDATE_TIMING;
            }
            if (true == r_glcdc_is_gamma_updating ())
            {
                return GLCDC_ERR_INVALID_UPDATE_TIMING;
            }
            if (true == r_glcdc_is_register_reflecting ())
            {
                return GLCDC_ERR_INVALID_UPDATE_TIMING;
            }

            /* Stop outputting the vertical and horizontal synchronization signals and screen data. */
            r_glcdc_bg_operation_disable ();

            /* status update */
            g_ctrl_blk.state = GLCDC_STATE_NOT_DISPLAYING;

            break;

        case GLCDC_CMD_SET_INTERRUPT:

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)
            if (NULL == p_args)
            {
                return GLCDC_ERR_INVALID_PTR;
            }
#endif

            if (false == g_ctrl_blk.first_vpos_interrupt_flag)
            {
                return GLCDC_ERR_INVALID_UPDATE_TIMING;
            }

            /* interrupt setting */
            r_glcdc_interrupt_setting ((glcdc_interrupt_cfg_t *) p_args);

            break;

        case GLCDC_CMD_CLR_DETECTED_STATUS:

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)
            if (NULL == p_args)
            {
                return GLCDC_ERR_INVALID_PTR;
            }
#endif
            p_detection = (glcdc_detect_cfg_t *) p_args;

            if (true == p_detection->vpos_detect)
            {
                r_glcdc_vpos_int_status_clear ();
            }
            if (true == p_detection->gr1uf_detect)
            {
                r_glcdc_gr1uf_int_status_clear ();
            }
            if (true == p_detection->gr2uf_detect)
            {
                r_glcdc_gr2uf_int_status_clear ();
            }

            break;

        case GLCDC_CMD_CHANGE_BG_COLOR:

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)
            if (NULL == p_args)
            {
                return GLCDC_ERR_INVALID_PTR;
            }
#endif

            r_glcdc_bg_color_setting ((glcdc_color_t *) p_args);

            break;

        default:
            return GLCDC_ERR_INVALID_ARG;
    }

    return GLCDC_SUCCESS;
} /* End of function R_GLCDC_Control() */

/***********************************************************************************************************************
 *Function Name: R_GLCDC_LayerChange
 *******************************************************************************************************************//**
 *@brief This function changes operation of graphics 1 and graphics 2.
 *@param[in] frame Graphics screen to change operation.
 *@param[in] p_args Pointer to the setting parameters structure.
 *@retval GLCDC_SUCCESS                    Processing has been completed successfully.
 *@retval GLCDC_ERR_INVALID_PTR            The p_args parameter is NULL pointer.
 *@retval GLCDC_ERR_INVALID_ARG            The argument set is invalid.
 *@retval GLCDC_ERR_INVALID_MODE           Function cannot be executed in this mode.
 *@retval GLCDC_ERR_NOT_OPEN               R_GLCDC_Open has not been executed.
 *@retval GLCDC_ERR_INVALID_UPDATE_TIMING  Update timing of the register is invalid.
 *@retval GLCDC_ERR_INVALID_LAYER_SETTING Graphics screen setting is invalid.
 *@retval GLCDC_ERR_INVALID_ALIGNMENT     Start address of the frame buffer is invalid.
 *@retval GLCDC_ERR_INVALID_BLEND_SETTING Setting for blending is invalid.
 *@details This function changes operation of graphics 1 and 2.This function can be executed when the mode is
 *'GLCDC_STATE_DISPLAYING'. The mode remains unchanged after processing in this function is complete.
 *@note None.
 **********************************************************************************************************************/
glcdc_err_t R_GLCDC_LayerChange(glcdc_frame_layer_t frame, glcdc_runtime_cfg_t const * const p_args)
{

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)
    glcdc_err_t err = GLCDC_SUCCESS;
#endif

    if (GLCDC_STATE_CLOSED == g_ctrl_blk.state)
    {
        return GLCDC_ERR_NOT_OPEN;
    }
    if (GLCDC_STATE_NOT_DISPLAYING == g_ctrl_blk.state)
    {
        return GLCDC_ERR_INVALID_MODE;
    }

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)
    if (NULL == p_args)
    {
        return GLCDC_ERR_INVALID_PTR;
    }
    if ((GLCDC_FRAME_LAYER_1 != frame) && (GLCDC_FRAME_LAYER_2 != frame))
    {
        return GLCDC_ERR_INVALID_ARG;
    }
#endif

    if (false == g_ctrl_blk.graphics_read_enable[frame])
    {
        return GLCDC_ERR_INVALID_ARG;
    }

    /* Return immediately if the register updating is in progress. */
    if (true == r_glcdc_is_gr_plane_updating (frame))
    {
        return GLCDC_ERR_INVALID_UPDATE_TIMING;
    }
    if (true == r_glcdc_is_register_reflecting ())
    {
        return GLCDC_ERR_INVALID_UPDATE_TIMING;
    }

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)

    err = r_glcdc_param_check_layer (p_args);

    if (GLCDC_SUCCESS != err)
    {
        return err;
    }

#endif

    /* Configure the graphics plane layers */
    r_glcdc_graphics_layer_set (&p_args->input, frame);

    /* Configure the alpha blending condition */
    r_glcdc_blend_condition_set (&p_args->blend, frame);

    /* Configure the chroma key */
    r_glcdc_graphics_chromakey_set (&p_args->chromakey, frame);

    /* Reflect the graphics module register value to the GLCD internal operations
     *  (at the timing of the next Vsync assertion) */
    r_glcdc_gr_plane_update (frame);

    return GLCDC_SUCCESS;
} /* End of function R_GLCDC_LayerChange() */

/***********************************************************************************************************************
 *Function Name: R_GLCDC_ColorCorrection
 *******************************************************************************************************************//**
 *@brief This function changes settings for brightness, contrast, and gamma correction of the GLCDC.
 *@param[in] cmd Command to change the setting
 *@param[in] p_args Pointer to the setting parameter structure
 *@retval GLCDC_SUCCESS                    Processing has been completed successfully.
 *@retval GLCDC_ERR_INVALID_PTR            The p_args parameter is NULL pointer.
 *@retval GLCDC_ERR_INVALID_ARG            The argument set is invalid.
 *@retval GLCDC_ERR_INVALID_MODE           Function cannot be executed in this mode.
 *@retval GLCDC_ERR_NOT_OPEN               R_GLCDC_Open has not been executed.
 *@retval GLCDC_ERR_INVALID_UPDATE_TIMING  Update timing of the register is invalid.
 *@retval GLCDC_ERR_INVALID_GAMMA_SETTING  Gamma correction setting is invalid.
 *@details This function changes settings for brightness, contrast, and gamma correction of the GLCDC. The setting to be
 *changed is determined according to the first argument of this function. This function can be executed when the mode
 *is 'GLCDC_STATE_DISPLAYING'. The mode remains unchanged after processing for this command is complete.
 *@note None.
 **********************************************************************************************************************/
glcdc_err_t R_GLCDC_ColorCorrection(glcdc_correction_cmd_t cmd, void const * const p_args)
{
    glcdc_correction_t *p_correction;

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)
    glcdc_err_t err = GLCDC_SUCCESS;
#endif

    if (GLCDC_STATE_CLOSED == g_ctrl_blk.state)
    {
        return GLCDC_ERR_NOT_OPEN;
    }
    if (GLCDC_STATE_NOT_DISPLAYING == g_ctrl_blk.state)
    {
        return GLCDC_ERR_INVALID_MODE;
    }

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)
    if (NULL == p_args)
    {
        return GLCDC_ERR_INVALID_PTR;
    }
#endif

    /* Return immediately if the register updating are performed */
    if (true == r_glcdc_is_output_ctrl_updating ())
    {
        return GLCDC_ERR_INVALID_UPDATE_TIMING;
    }
    if (true == r_glcdc_is_gamma_updating ())
    {
        return GLCDC_ERR_INVALID_UPDATE_TIMING;
    }
    if (true == r_glcdc_is_register_reflecting ())
    {
        return GLCDC_ERR_INVALID_UPDATE_TIMING;
    }

    switch (cmd)
    {
        case GLCDC_CORRECTION_CMD_SET_ALL:

            p_correction = (glcdc_correction_t *) p_args;

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)
            err = r_glcdc_param_check_brightness (&p_correction->brightness);
            if (GLCDC_SUCCESS != err)
            {
                return err;
            }

            err = r_glcdc_param_check_gamma (&p_correction->gamma);
            if (GLCDC_SUCCESS != err)
            {
                return err;
            }
#endif

            /* Configure the brightness and contrast and gamma correction register setting. */
            r_glcdc_brightness_correction (&p_correction->brightness);
            r_glcdc_contrast_correction (&p_correction->contrast);
            r_glcdc_gamma_correction (&p_correction->gamma);

            /* Reflect the output block section and gamma register setting. */
            r_glcdc_output_ctrl_update ();
            r_glcdc_gamma_update ();

            break;

        case GLCDC_CORRECTION_CMD_BRIGHTNESS:

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)
            err = r_glcdc_param_check_brightness ((glcdc_brightness_t *) p_args);
            if (GLCDC_SUCCESS != err)
            {
                return err;
            }
#endif
            /* Configure the brightness correction register setting. */
            r_glcdc_brightness_correction ((glcdc_brightness_t *) p_args);

            /* Reflect the output block section register setting. */
            r_glcdc_output_ctrl_update ();

            break;

        case GLCDC_CORRECTION_CMD_CONTRAST:

            /* Configure the contrast correction register setting. */
            r_glcdc_contrast_correction ((glcdc_contrast_t *) p_args);

            /* Reflect the output block section register setting. */
            r_glcdc_output_ctrl_update ();

            break;

        case GLCDC_CORRECTION_CMD_GAMMA:

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)
            err = r_glcdc_param_check_gamma ((glcdc_gamma_correction_t *) p_args);
            if (GLCDC_SUCCESS != err)
            {
                return err;
            }
#endif
            /* Configure the gamma correction register setting. */
            r_glcdc_gamma_correction ((glcdc_gamma_correction_t *) p_args);

            /* Reflect the gamma register setting. */
            r_glcdc_gamma_update ();

            break;

        default:
            return GLCDC_ERR_INVALID_ARG;
    }

    return GLCDC_SUCCESS;
} /* End of function R_GLCDC_ColorCorrection() */

/***********************************************************************************************************************
 *Function Name:R_GLCDC_ClutUpdate
 *******************************************************************************************************************//**
 *@brief This function updates the CLUT memory of the GLCDC.
 *@param[in] frame Graphics screen to change operation
 *@param[in] p_clut_cfg Pointer to the CLUT memory structure
 *@retval GLCDC_SUCCESS                    Processing has been completed successfully.
 *@retval GLCDC_ERR_INVALID_PTR            The p_clut_cfg parameter is NULL pointer.
 *@retval GLCDC_ERR_INVALID_ARG            The argument set is invalid.
 *@retval GLCDC_ERR_INVALID_MODE           Function cannot be executed in this mode.
 *@retval GLCDC_ERR_NOT_OPEN               R_GLCDC_Open has not been executed.
 *@retval GLCDC_ERR_INVALID_UPDATE_TIMING  Update timing of the register is invalid.
 *@retval GLCDC_ERR_INVALID_CLUT_ACCESS    CLUT memory setting is invalid.
 *@details This function updates the CLUT memory of the GLCDC. This function can be executed when the mode is
 *'GLCDC_STATE_DISPLAYING'. The mode remains unchanged after processing in this function is complete.
 *@note None.
 **********************************************************************************************************************/
glcdc_err_t R_GLCDC_ClutUpdate(glcdc_frame_layer_t frame, glcdc_clut_cfg_t const * const p_clut_cfg)
{

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)
    glcdc_err_t err = GLCDC_SUCCESS;
#endif

    /* Status check */
    if (GLCDC_STATE_CLOSED == g_ctrl_blk.state)
    {
        return GLCDC_ERR_NOT_OPEN;
    }
    if (GLCDC_STATE_NOT_DISPLAYING == g_ctrl_blk.state)
    {
        return GLCDC_ERR_INVALID_MODE;
    }

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)
    if (NULL == p_clut_cfg)
    {
        return GLCDC_ERR_INVALID_PTR;
    }
    if ((GLCDC_FRAME_LAYER_1 != frame) && (GLCDC_FRAME_LAYER_2 != frame))
    {
        return GLCDC_ERR_INVALID_ARG;
    }
#endif

    if (false == g_ctrl_blk.graphics_read_enable[frame])
    {
        return GLCDC_ERR_INVALID_ARG;
    }

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)
    err = r_glcdc_param_check_clut (p_clut_cfg);
    if (GLCDC_SUCCESS != err)
    {
        return err;
    }

#endif

    /* Return immediately if the register updating is in progress. */
    if (true == r_glcdc_is_gr_plane_updating (frame))
    {
        return GLCDC_ERR_INVALID_UPDATE_TIMING;
    }
    if (true == r_glcdc_is_register_reflecting ())
    {
        return GLCDC_ERR_INVALID_UPDATE_TIMING;
    }

    /* Update a color palette */
    r_glcdc_clut_update (p_clut_cfg, frame);

    /* Reflect the graphics module register value */
    r_glcdc_gr_plane_update (frame);

    return GLCDC_SUCCESS;
} /* End of function R_GLCDC_ClutUpdate() */

/***********************************************************************************************************************
 *Function Name: R_GLCDC_GetStatus
 *******************************************************************************************************************//**
 *@brief This function obtains the GLCDC status.
 *@param[out] p_status Pointer to the structure which stores the obtained status.
 *@retval GLCDC_SUCCESS           Processing has been completed successfully.
 *@retval GLCDC_ERR_INVALID_PTR   The p_status parameter is NULL pointer.
 *@retval GLCDC_ERR_NOT_OPEN      R_GLCDC_Open has not been executed.
 *@details This function obtains the GLCDC status. The obtained status is written to the p_status structure passed
 *with the argument. This function can be called at any time after the R_GLCDC_Open function is executed.
 *The mode remains unchanged after processing in this function is complete.
 *@note None.
 **********************************************************************************************************************/

glcdc_err_t R_GLCDC_GetStatus(glcdc_status_t * const p_status)
{
    uint8_t frame;

    if (GLCDC_STATE_CLOSED == g_ctrl_blk.state)
    {
        return GLCDC_ERR_NOT_OPEN;
    }

#if (GLCDC_CFG_PARAM_CHECKING_ENABLE)
    if (NULL == p_status)
    {
        return GLCDC_ERR_INVALID_PTR;
    }
#endif

    /* Return the GLCD driver state */
    p_status->state = g_ctrl_blk.state;

    /* Return the fading status for the layers */

    /* WAIT_LOOP */
    for (frame = 0; frame < GLCDC_FRAME_LAYER_NUM; frame++)
    {
        if (true == r_glcdc_is_gr_plane_updating ((glcdc_frame_layer_t) frame))
        {
            /* Fading status is uncertain (the reason is described above). */
            p_status->fade_status[frame] = GLCDC_FADE_STATUS_UNCERTAIN;
        }
        else
        {
            if (true == r_glcdc_is_fading ((glcdc_frame_layer_t) frame))
            {
                p_status->fade_status[frame] = GLCDC_FADE_STATUS_FADING_UNDERWAY;
            }
            else
            {
                p_status->fade_status[frame] = GLCDC_FADE_STATUS_NOT_UNDERWAY;
            }
        }
    }

    /* Return the detected status for the vpos, gr1uf, gr2uf */
    if (true == r_glcdc_vpos_int_status_check ())
    {
        p_status->state_vpos = GLCDC_DETECTED;
    }
    else
    {
        p_status->state_vpos = GLCDC_NOT_DETECTED;
    }
    if (true == r_glcdc_gr1uf_int_status_check ())
    {
        p_status->state_gr1uf = GLCDC_DETECTED;
    }
    else
    {
        p_status->state_gr1uf = GLCDC_NOT_DETECTED;
    }
    if (true == r_glcdc_gr2uf_int_status_check ())
    {
        p_status->state_gr2uf = GLCDC_DETECTED;
    }
    else
    {
        p_status->state_gr2uf = GLCDC_NOT_DETECTED;
    }

    return GLCDC_SUCCESS;
} /* End of function R_GLCDC_GetStatus() */

/***********************************************************************************************************************
 *Function Name: R_GLCDC_GetVersion
 *******************************************************************************************************************//**
 *@brief This function returns the current version of this API.
 *@param None.
 *@retval Version of this API.
 *@details This function will return the version of the currently running API. The version number is encoded where the
 *top 2 bytes are the major version number and the bottom 2 bytes are the minor version number.
 *For example, Version 4.25 would be returned as 0x00040019.
 *@note None.
 **********************************************************************************************************************/

uint32_t R_GLCDC_GetVersion(void)
{
    return ((((uint32_t) GLCDC_RX_VERSION_MAJOR) << 16) | ((uint32_t) GLCDC_RX_VERSION_MINOR));
} /* End of function R_GLCDC_GetVersion() */

/* End of File */
