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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_ctsu_qe.c
* Description  : This file implements the QE CTSU API
***********************************************************************************************************************/
/***********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 04.10.2018 1.00    First Release
*              : 09.07.2019 1.10    Added QE_ERR_ABNORMAL_TSCAP, QE_ERR_SENSOR_OVERFLOW, and QE_ERR_SENSOR_SATURATION
*                                     return codes to Open() (error detected during correction)
*                                   Modified to allow R_CTSU_StartScan() to be called in external trigger mode IF
*                                     Touch is performing offset tuning.
*                                   Set ctsu state to RUN (ctsu now busy) in ctsu_ctsuwr_isr() so Touch can detect
*                                     if it missed processing a scan when using external triggers.
*                                   Added Control() commands CTSU_CMD_GET_METHOD_MODE and CTSU_CMD_GET_SCAN_INFO.
*                                   Added 128us delay after setting CTSUPON in init_method().
*                                   Added #pragma sections and ported to GCC/IAR.
*              : 09.01.2020 1.11    Fixed bug where custom callback function was called twice.
*                                     Removed call from ctsu_ctsufn_isr().
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_ctsu_qe_if.h"
#include "r_ctsu_qe_private.h"
#include "qe_common.h"
#include "r_dtc.h"
#include "r_ctsu_qe_pinset.h"


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
External functions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
ctsu_ctrl_t g_ctsu_ctrl = { .lock = 0,
                            .open = false,
                            .p_callback = NULL
                          };
ctsu_cfg_t *gp_cur_cfg;                             // ptr to current configuration/method
volatile scan_info_t g_pvt_scan_info[QE_NUM_METHODS];

static void init_method(uint8_t method);


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_CTSU_Open
* Description  : This function initializes the CTSU peripheral, and makes the first configuration in the array passed in
*                the active configuration.
* Arguments    : p_ctsu_cfgs
*                    Pointer to array of scan configurations (must be global) generated by QE Tool
*                num_methods
*                    Number of methods in p_method array
*                trigger
*                    Specifies trigger source to start scan
* Return Value : QE_SUCCESS -
*                    API initialized successfully.
*                QE_ERR_NULL_PTR
*                    Missing argument pointer
*                QE_ERR_INVALID_ARG
*                    "num_methods" or "trigger" is invalid for configuration
*                QE_ERR_BUSY
*                    Cannot run function because another operation is in progress.
*                QE_ERR_ALREADY_OPEN -
*                    Open() called without an intermediate call to Close().
*                QE_ERR_ABNORMAL_TSCAP
*                   TSCAP error detected during correction
*                QE_ERR_SENSOR_OVERFLOW
*                   Sensor overflow error detected during correction
*                QE_ERR_SENSOR_SATURATION -
*                   Initial sensor value beyond linear portion of correction curve
*                QE_ERR_UNSUPPORTED_CLK_CFG -
*                   Unsupported clock speed. Cannot perform CTSU correction.
***********************************************************************************************************************/
qe_err_t R_CTSU_Open(ctsu_cfg_t *p_ctsu_cfgs[], uint8_t num_methods, qe_trig_t trigger)
{
    qe_err_t err = QE_SUCCESS;

#if (CTSU_CFG_PARAM_CHECKING_ENABLE)
    if (NULL == p_ctsu_cfgs)
    {
        return QE_ERR_NULL_PTR;
    }

    if ((num_methods > QE_NUM_METHODS)
     || (trigger >= QE_TRIG_END_ENUM)
     || ((QE_TRIG_EXTERNAL == trigger) && (CTSU_CFG_USE_DTC == 1)))
    {
        return QE_ERR_INVALID_ARG;
    }

    /* NOTE: Do not verify configuration content. Should be generated by QE Tool and be bug-free. */
#endif

    /* Check if another API call is in progress */
    if (R_BSP_SoftwareLock(&g_ctsu_ctrl.lock) == false)
    {
        return QE_ERR_BUSY;
    }

    /* See if driver is already open (must Close() before doing an Open() on a another configuration) */
    if (true == g_ctsu_ctrl.open)
    {
        R_BSP_SoftwareUnlock(&g_ctsu_ctrl.lock);
        return QE_ERR_ALREADY_OPEN;
    }

    /* Get CTSU out of stop state (supply power/clock) */
    R_BSP_RegisterProtectDisable (BSP_REG_PROTECT_LPC_CGC_SWR);
    MSTP(CTSU) = 0;
    R_BSP_RegisterProtectEnable (BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Initialize driver control structure */
    g_ctsu_ctrl.p_methods = p_ctsu_cfgs;                  // NOTE: points to global created by QE tool
    g_ctsu_ctrl.num_methods = num_methods;
    g_ctsu_ctrl.rd_index = 0;
    g_ctsu_ctrl.wr_index = 0;
    g_ctsu_ctrl.trigger = trigger;

    /* Enable interrupts for write, read, & scan done */
    IR(CTSU,CTSUWR)= 0;
    IR(CTSU,CTSURD)= 0;
    IR(CTSU,CTSUFN)= 0;
    IPR(CTSU,CTSUWR)= CTSU_CFG_INT_PRIORITY_LEVEL;
    IPR(CTSU,CTSURD)= CTSU_CFG_INT_PRIORITY_LEVEL;
    IPR(CTSU,CTSUFN)= CTSU_CFG_INT_PRIORITY_LEVEL;
#if (!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5))
    IEN(CTSU,CTSUWR)= 1;
    IEN(CTSU,CTSURD)= 1;
    IEN(CTSU,CTSUFN)= 1;
#else
    R_BSP_InterruptRequestEnable(VECT(CTSU,CTSUWR));
    R_BSP_InterruptRequestEnable(VECT(CTSU,CTSURD));
    R_BSP_InterruptRequestEnable(VECT(CTSU,CTSUFN));
#endif

    /* Set current configuration to the first method.
     * The correction method needs it to find the first active channel to calculate gain with.
     * It also uses its scan buffer for interrupts instead of having a temporary buffer.
     * None of method 0's configuration values are altered.
     */
    gp_cur_cfg = p_ctsu_cfgs[0];

    /* Discharge (make GPIO output) and charge (configure for TSCAP) TSCAP pin */
    R_Set_CTSU_TSCAP_Discharge();
    R_BSP_SoftwareDelay(30,BSP_DELAY_MICROSECS);
    R_Set_CTSU_TSCAP_Charge();
    R_BSP_SoftwareDelay(500, BSP_DELAY_MICROSECS);

    /* Perform sensor gain correction */
    err = correction_CTSU_sensor_ico();
    if (QE_SUCCESS != err)
    {
        R_BSP_SoftwareUnlock(&g_ctsu_ctrl.lock);
        R_CTSU_Close();
    }
    else
    {
        /* Initialize driver registers to first method (default) */
        init_method(0);

        /* Initialize DTC */
#if (CTSU_CFG_USE_DTC == 1)
        dtc_ctsu_init();
#endif
        /* Do not set up for external triggers here even if configured.
         * 1) Would be changed immediately to SW trigger by Touch for offset tuning.
         * 2) If ext trig was set up before Open, which it shouldn't, a scan may occur before
         *    Touch Open() is complete.
         */

        /* Mark driver as open */
        g_ctsu_ctrl.open = true;
        R_BSP_SoftwareUnlock(&g_ctsu_ctrl.lock);
    }

    return err;

} /* End of function R_CTSU_Open() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: init_method
* Description  : This functions initializes the registers for the method configuration provided.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void init_method(uint8_t method)
{
    ctsu_cfg_t *p_scan_cfg = g_ctsu_ctrl.p_methods[method];


    /* Save method number */
    g_ctsu_ctrl.cur_method = method;

    /* Write CTSU Control Register 1 and save mode */
    CTSU.CTSUCR1.BYTE = p_scan_cfg->ctsucr1;        // MD, CLK, ATUNE1, ATUNE0, CSW, PON
    while (1 != CTSU.CTSUCR1.BIT.CTSUPON)           /* Wait for charging to stabilize */
    {
        ;
    }
    R_BSP_SoftwareDelay(128, BSP_DELAY_MICROSECS);

    /* get enum equivalent of mode */
    g_ctsu_ctrl.mode = (ctsu_mode_t) CTSU.CTSUCR1.BIT.CTSUMD;


    /* Set static registers */
    CTSU.CTSUSDPRS.BYTE = p_scan_cfg->ctsusdprs;    // Synchronous Noise Reduction Setting
    CTSU.CTSUSST.BYTE = 0x10;                       // Sensor Stabilization- ALWAYS 0x10 as per HW Manual
    CTSU.CTSUDCLKC.BYTE = 0x30;                     // High Pass Noise Reduction- ALWAYS 0x30 as per HW Manual

    CTSU.CTSUCHAC0.BYTE = p_scan_cfg->ctsuchac0;    // Channel Enable Control: TS0-TS7
    CTSU.CTSUCHAC1.BYTE = p_scan_cfg->ctsuchac1;    // Channel Enable Control: TS8-TS15
#if (CTSU_HAS_LARGE_REG_SET)
    CTSU.CTSUCHAC2.BYTE = p_scan_cfg->ctsuchac2;    // Channel Enable Control: TS16-TS23
    CTSU.CTSUCHAC3.BYTE = p_scan_cfg->ctsuchac3;    // Channel Enable Control: TS24-TS31
    CTSU.CTSUCHAC4.BYTE = p_scan_cfg->ctsuchac4;    // Channel Enable Control: TS32-TS35
#endif

    if (CTSU_MODE_SELF_SINGLE_SCAN == g_ctsu_ctrl.mode)
    {
        CTSU.CTSUMCH0.BYTE = p_scan_cfg->p_elem_ch[0].rx_chan;    // Only sensor to scan
    }

    if (CTSU_MODE_MUTUAL_FULL_SCAN == g_ctsu_ctrl.mode)
    {
        CTSU.CTSUCHTRC0.BYTE = p_scan_cfg->ctsuchtrc0;  // Channel Transmit Control: TS0-TS7
        CTSU.CTSUCHTRC1.BYTE = p_scan_cfg->ctsuchtrc1;  // Channel Transmit Control: TS8-TS15
#if (CTSU_HAS_LARGE_REG_SET)
        CTSU.CTSUCHTRC2.BYTE = p_scan_cfg->ctsuchtrc2;  // Channel Transmit Control: TS16-TS23
        CTSU.CTSUCHTRC3.BYTE = p_scan_cfg->ctsuchtrc3;  // Channel Transmit Control: TS24-TS31
        CTSU.CTSUCHTRC4.BYTE = p_scan_cfg->ctsuchtrc4;  // Channel Transmit Control: TS32-TS35
#endif
    }

#if (CTSU_HAS_TXVSEL)
    CTSU.CTSUCR0.BIT.CTSUTXVSEL = p_scan_cfg->txvsel;
#endif

    /* Initialize CTSU hardware control block */
    CTSU.CTSUCR0.BIT.CTSUINIT = 1;

    /* Set global pointer to this configuration */
    gp_cur_cfg = p_scan_cfg;
    return;

} /* End of function init_method() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_CTSU_StartScan
* Description  : This functions starts the scan for the configured sensors. This function should only be called AFTER
*                correction completes (this function has possible DTC use, will not run when external triggers are
*                configured, and the lock is already set from Open() ).
* Arguments    : None
* Return Value : QE_SUCCESS
*                    Scan started
*                QE_ERR_TRIGGER_TYPE
*                    Function should not be called to start scans when using external triggers
*                QE_ERR_BUSY
*                    Cannot run function because another operation is in progress.
***********************************************************************************************************************/
qe_err_t R_CTSU_StartScan(void)
{

    /* Return error if in external trigger mode and Touch is not doing offset tuning */
    if ((QE_TRIG_EXTERNAL == g_ctsu_ctrl.trigger) && (0 == gp_cur_cfg->ctsu_status.flag.offset_tuning))
    {
        return QE_ERR_TRIGGER_TYPE;
    }

    /* Check if another API call is in progress */
    if ((R_BSP_SoftwareLock(&g_ctsu_ctrl.lock) == false)
     && (false == g_correction_mode))
    {
        return QE_ERR_BUSY;
    }

    g_ctsu_ctrl.state = CTSU_STATE_RUN;

    /* Setup addresses for DTC interrupt transfers */
#if (CTSU_CFG_USE_DTC == 1)

    /* cast pointers to absolute addresses */
    dtc_ctsu_xfer_addrs((uint32_t)gp_cur_cfg->p_elem_regs, (uint32_t)gp_cur_cfg->p_scan_buf);
#endif

    /* Start scan */
    CTSU.CTSUCR0.BIT.CTSUSTRT = 1;

    /* (operation unlocked at interrupt level) */

    return QE_SUCCESS;

} /* End of function R_CTSU_StartScan() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: ctsu_ctsuwr_isr
* Description  : CTSU WR interrupt handler. This service routine sets the tuning for the next element to be scanned by
*                hardware.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if (!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5))
#pragma interrupt (ctsu_ctsuwr_isr(vect=VECT(CTSU,CTSUWR)))
static void ctsu_ctsuwr_isr(void)
#else
R_BSP_PRAGMA_STATIC_INTERRUPT(ctsu_ctsuwr_isr, VECT(CTSU,CTSUWR))
R_BSP_ATTRIB_STATIC_INTERRUPT void ctsu_ctsuwr_isr(void)
#endif
{
    uint8_t     i;

    if (true == g_correction_mode)  // _1_CORRECTION
    {
        /* 32 scans, txd[2] modified, then 32 scans */
        CTSU.CTSUSSC.WORD = g_correction_dtc_txd[0];
        CTSU.CTSUSO0.WORD = g_correction_dtc_txd[1];
        CTSU.CTSUSO1.WORD = g_correction_dtc_txd[2];
    }
    else /* _0_NORMAL */
    {
        /* In case using external trigger, set state to RUN so Touch knows buffer being updated.
         * NOTE: dtc does not work with ext trigger, so this interrupt will occur.
         */
        g_ctsu_ctrl.state = CTSU_STATE_RUN;

        /* get current channel index and write settings for current element */
        i = g_ctsu_ctrl.wr_index++;
        CTSU.CTSUSSC.WORD = gp_cur_cfg->p_elem_regs[i].ctsussc;
        CTSU.CTSUSO0.WORD = gp_cur_cfg->p_elem_regs[i].ctsuso0;
        CTSU.CTSUSO1.WORD = gp_cur_cfg->p_elem_regs[i].ctsuso1;
    }

} /* End of function ctsu_ctsuwr_isr() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: ctsu_ctsurd_isr
* Description  : CTSU RD interrupt handler. This service routine reads the sensor count and reference counter for
*                the current element and places the value in the scan data buffer. Note that the reference counter
*                does not work properly but is saved anyway for backward compatibility and potential future use.
*                Additionally, the SC register cannot be read again until RC is read.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if (!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5))
#pragma interrupt (ctsu_ctsurd_isr(vect=VECT(CTSU,CTSURD)))
static void ctsu_ctsurd_isr(void)
#else
R_BSP_PRAGMA_STATIC_INTERRUPT(ctsu_ctsurd_isr, VECT(CTSU,CTSURD))
R_BSP_ATTRIB_STATIC_INTERRUPT void ctsu_ctsurd_isr(void)
#endif
{
    /* read current channel/element value */
    gp_cur_cfg->p_scan_buf[g_ctsu_ctrl.rd_index++] = CTSU.CTSUSC.WORD;

    /* Store the reference counter for possible future use. Register must be read or scan will hang. */
    gp_cur_cfg->p_scan_buf[g_ctsu_ctrl.rd_index++] = CTSU.CTSURC.WORD;

} /* End of function ctsu_ctsurd_isr() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: ctsu_ctsufn_isr
* Description  : CTSU FN interrupt handler. This service routine occurs when all elements have been scanned (finished).
*                The user's callback function is called if available.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if (!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5))
#pragma interrupt (ctsu_ctsufn_isr(vect=VECT(CTSU,CTSUFN)))
static void ctsu_ctsufn_isr(void)
#else
R_BSP_PRAGMA_STATIC_INTERRUPT(ctsu_ctsufn_isr, VECT(CTSU,CTSUFN))
R_BSP_ATTRIB_STATIC_INTERRUPT void ctsu_ctsufn_isr(void)
#endif
{

    /* Call main interrupt handler (primarily for correction) */
    CTSUInterrupt();

    /* reset indexes */
    g_ctsu_ctrl.wr_index = 0;
    g_ctsu_ctrl.rd_index = 0;

    /* mark CTSU as no longer busy */
    R_BSP_SoftwareUnlock(&g_ctsu_ctrl.lock);

} /* End of function ctsu_ctsufn_isr() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_CTSU_ReadButton
* Description  : This function gets the scan data associated with the specified button.
* Arguments    : p_btn_ctrl
*                    Pointer to configuration for button to read.
*                p_value1
*                    Pointer to load primary button value
*                p_value2
*                    Pointer to load secondary button value in mutual mode only. Can be NULL in other modes.
* Return Value : QE_SUCCESS
*                    Button data read.
*                QE_ERR_NULL_PTR
*                    Missing argument pointer.
*                QE_ERR_INVALID_ARG
*                    Unrecognized element id
*                QE_ERR_BUSY
*                    Cannot run function because another operation is in progress.
***********************************************************************************************************************/
qe_err_t R_CTSU_ReadButton(btn_ctrl_t *p_btn_ctrl, uint16_t *p_value1, uint16_t *p_value2)
{
    qe_err_t err = QE_SUCCESS;

#if (CTSU_CFG_PARAM_CHECKING_ENABLE)
    if ((NULL == p_btn_ctrl)
     || (NULL == p_value1)
     || ((NULL == p_value2) && (CTSU_MODE_MUTUAL_FULL_SCAN == g_ctsu_ctrl.mode)))
    {
        return QE_ERR_NULL_PTR;
    }
#endif

    /* Load value(s) from scan buffer */
    err = R_CTSU_ReadElement(p_btn_ctrl->elem_index, p_value1, p_value2);

    return err;

} /* End of function R_CTSU_ReadButton() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_CTSU_ReadSlider
* Description  : This function gets the scan data associated with the specified slider/wheel.
* Arguments    : p_sldr_ctrl
*                    Pointer to configuration for slider/wheel to read.
*                p_buf1
*                    Pointer to array to load primary values
*                p_buf2
*                    Pointer to array to load secondary values in mutual mode only. Can be NULL in other modes.
*
* Return Value : QE_SUCCESS
*                    Slider/wheel data read.
*                QE_ERR_NULL_PTR
*                    Missing argument pointer.
*                QE_ERR_INVALID_ARG
*                    Unrecognized element id
*                QE_ERR_BUSY
*                    Cannot run function because another operation is in progress.
***********************************************************************************************************************/
qe_err_t R_CTSU_ReadSlider(sldr_ctrl_t *p_sldr_ctrl, uint16_t *p_buf1, uint16_t *p_buf2)
{
    qe_err_t  err;
    uint32_t    i;

#if (CTSU_CFG_PARAM_CHECKING_ENABLE)
    if ((NULL == p_sldr_ctrl)
     || (NULL == p_buf1)
     || ((NULL == p_buf2) && (CTSU_MODE_MUTUAL_FULL_SCAN == g_ctsu_ctrl.mode)))
    {
        return QE_ERR_NULL_PTR;
    }
#endif

    /* Load values from scan buffer */
    for (i=0; i < p_sldr_ctrl->num_elements; i++, p_buf1++, p_buf2++)
    {
        err = R_CTSU_ReadElement(p_sldr_ctrl->elem_index[i], p_buf1, p_buf2);
        if (QE_SUCCESS != err)
        {
            break;
        }
    }

    return err;

} /* End of function R_CTSU_ReadSlider() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_CTSU_ReadElement
* Description  : This function returns the data scanned for the specified element/channel(s).
* Arguments    : element -
*                    Element associated with sensor/sensor pair (mutual)
*                p_value1 -
*                    Pointer to variable to load with primary value
*                p_value2 -
*                    Pointer to variable to load with secondary value
* Return Value : QE_SUCCESS
*                    Register value read
*                QE_ERR_INVALID_ARG
*                    Unrecognized element id
*                QE_ERR_NULL_PTR
*                    Missing argument pointer
*                QE_ERR_BUSY
*                    Cannot run function because another operation is in progress.
***********************************************************************************************************************/
qe_err_t R_CTSU_ReadElement(uint8_t element, uint16_t *p_value1, uint16_t *p_value2)
{
    qe_err_t err = QE_SUCCESS;
    uint16_t    i;

#if (CTSU_CFG_PARAM_CHECKING_ENABLE)
    if (element >= gp_cur_cfg->num_elements)
    {
        return QE_ERR_INVALID_ARG;
    }

    if ((NULL == p_value1)
     || ((NULL == p_value2) && (CTSU_MODE_MUTUAL_FULL_SCAN == g_ctsu_ctrl.mode)))
    {
        return QE_ERR_NULL_PTR;
    }
#endif

    /* Check if another API call is in progress */
    if (R_BSP_SoftwareLock(&g_ctsu_ctrl.lock) == false)
    {
        return QE_ERR_BUSY;
    }


    /* Get scan buffer index and fetch data value */
    i = (CTSU_MODE_MUTUAL_FULL_SCAN == g_ctsu_ctrl.mode) ? (element*4) : (element*2);

    *p_value1 = gp_cur_cfg->p_scan_buf[i];
    if (CTSU_MODE_MUTUAL_FULL_SCAN == g_ctsu_ctrl.mode)
    {
        *p_value2 = gp_cur_cfg->p_scan_buf[i+2];
    }


    R_BSP_SoftwareUnlock(&g_ctsu_ctrl.lock);
    return err;

} /* End of function R_CTSU_ReadElement() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_CTSU_ReadData
* Description  : This function gets the scan data (strips out reference data).
* Arguments    : p_buf
*                    Pointer to buffer to load scan data into
*                p_cnt
*                    Pointer to variable to load number of words placed into buffer
* Return Value : QE_SUCCESS
*                    Scan data read.
*                QE_ERR_NULL_PTR
*                    Missing argument pointer
*                QE_ERR_BUSY
*                    Cannot run function because another operation is in progress.
***********************************************************************************************************************/
qe_err_t R_CTSU_ReadData(uint16_t *p_buf, uint16_t *p_cnt)
{
    qe_err_t  err = QE_SUCCESS;
    uint16_t    *p_src;
    uint32_t    i;

#if (CTSU_CFG_PARAM_CHECKING_ENABLE)
    if ((NULL == p_buf) || (NULL == p_cnt))
    {
        return QE_ERR_NULL_PTR;
    }
#endif

    /* Check if another API call is in progress */
    if (R_BSP_SoftwareLock(&g_ctsu_ctrl.lock) == false)
    {
        return QE_ERR_BUSY;
    }


    /* Load count (excluding reference counter entries) */
    *p_cnt = (uint16_t) gp_cur_cfg->num_elements;
    if (CTSU_MODE_MUTUAL_FULL_SCAN == g_ctsu_ctrl.mode)
    {
        (*p_cnt) *= 2;
    }

    /* Load scan buffer data (excluding reference counter entries) */
    p_src = gp_cur_cfg->p_scan_buf;
    for (i=0; i < (*p_cnt); i++, p_buf++, p_src+=2)
    {
        *p_buf = *p_src;
    }

    R_BSP_SoftwareUnlock(&g_ctsu_ctrl.lock);
    return err;

} /* End of function R_CTSU_ReadData() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_CTSU_ReadReg
* Description  : This function returns the contents of the specified hardware register, except for the CTSUSSC, SO0, and
*                SO1 cases, where the configuration values assigned to those registers as per the specified key are read
*                instead.
* Arguments    : reg -
*                    ID for register to read
*                element -
*                    For CTSUSSC,SO0,SO1 only, element associated with sensor/sensor pair (mutual)
*                p_value -
*                    Pointer to variable to load with register value
* Return Value : QE_SUCCESS
*                    Register value read.
*                QE_ERR_NULL_PTR
*                    Missing argument pointer
*                QE_ERR_INVALID_ARG
*                    Unrecognized register or element id
*                QE_ERR_BUSY
*                    Cannot run function because another operation is in progress.
***********************************************************************************************************************/
qe_err_t R_CTSU_ReadReg(ctsu_reg_t reg, uint8_t element, uint16_t *p_value)
{
    qe_err_t err = QE_SUCCESS;

#if (CTSU_CFG_PARAM_CHECKING_ENABLE)
    if ((reg >= CTSU_REG_END_ENUM) || (element >= gp_cur_cfg->num_elements))
    {
        return QE_ERR_INVALID_ARG;
    }

    if (NULL == p_value)
    {
        return QE_ERR_NULL_PTR;
    }
#endif

    /* Check if another API call is in progress */
    if (R_BSP_SoftwareLock(&g_ctsu_ctrl.lock) == false)
    {
        return QE_ERR_BUSY;
    }


    /* Read register value */
    switch (reg)
    {
        case CTSU_REG_CR0:
        {
            /* cast byte register to word argument */
            *p_value = (uint16_t) CTSU.CTSUCR0.BYTE;
            break;
        }
        case CTSU_REG_CR1:
        {
            /* cast byte register to word argument */
            *p_value = (uint16_t) CTSU.CTSUCR1.BYTE;
            break;
        }
        case CTSU_REG_SDPRS:
        {
            /* cast byte register to word argument */
            *p_value = (uint16_t) CTSU.CTSUSDPRS.BYTE;
            break;
        }
        case CTSU_REG_MCH0:
        {
            /* cast byte register to word argument */
            *p_value = (uint16_t) CTSU.CTSUMCH0.BYTE;
            break;
        }
        case CTSU_REG_MCH1:
        {
            /* cast byte register to word argument */
            *p_value = (uint16_t) CTSU.CTSUMCH1.BYTE;
            break;
        }
        case CTSU_REG_CHAC0:
        {
            /* cast byte register to word argument */
            *p_value = (uint16_t) CTSU.CTSUCHAC0.BYTE;
            break;
        }
        case CTSU_REG_CHAC1:
        {
            /* cast byte register to word argument */
            *p_value = (uint16_t) CTSU.CTSUCHAC1.BYTE;
            break;
        }
#if (CTSU_HAS_LARGE_REG_SET)
        case CTSU_REG_CHAC2:
        {
            /* cast byte register to word argument */
            *p_value = (uint16_t) CTSU.CTSUCHAC2.BYTE;
            break;
        }
        case CTSU_REG_CHAC3:
        {
            /* cast byte register to word argument */
            *p_value = (uint16_t) CTSU.CTSUCHAC3.BYTE;
            break;
        }
        case CTSU_REG_CHAC4:
        {
            /* cast byte register to word argument */
            *p_value = (uint16_t) CTSU.CTSUCHAC4.BYTE;
            break;
        }
#endif

        case CTSU_REG_CHTRC0:
        {
            /* cast byte register to word argument */
            *p_value = (uint16_t) CTSU.CTSUCHTRC0.BYTE;
            break;
        }
        case CTSU_REG_CHTRC1:
        {
            /* cast byte register to word argument */
            *p_value = (uint16_t) CTSU.CTSUCHTRC1.BYTE;
            break;
        }
#if (CTSU_HAS_LARGE_REG_SET)
        case CTSU_REG_CHTRC2:
        {
            /* cast byte register to word argument */
            *p_value = (uint16_t) CTSU.CTSUCHTRC2.BYTE;
            break;
        }
        case CTSU_REG_CHTRC3:
        {
            /* cast byte register to word argument */
            *p_value = (uint16_t) CTSU.CTSUCHTRC3.BYTE;
            break;
        }
        case CTSU_REG_CHTRC4:
        {
            /* cast byte register to word argument */
            *p_value = (uint16_t) CTSU.CTSUCHTRC4.BYTE;
            break;
        }
#endif

        case CTSU_REG_ST:
        {
            /* cast byte register to word argument */
            *p_value = (uint16_t) CTSU.CTSUST.BYTE;
            break;
        }
        case CTSU_REG_SSC:
        {
            *p_value = gp_cur_cfg->p_elem_regs[element].ctsussc;
            break;
        }
        case CTSU_REG_SO0:
        {
            *p_value = gp_cur_cfg->p_elem_regs[element].ctsuso0;
            break;
        }
        case CTSU_REG_SO1:
        {
            *p_value = gp_cur_cfg->p_elem_regs[element].ctsuso1;
            break;
        }
        case CTSU_REG_SC:
        {
            *p_value = CTSU.CTSUSC.WORD;
            break;
        }
        case CTSU_REG_RC:
        {
            *p_value = CTSU.CTSURC.WORD;
            break;
        }
        case CTSU_REG_ERRS:
        {
            *p_value = CTSU.CTSUERRS.WORD;
            break;
        }
        default:
        {
            err = QE_ERR_INVALID_ARG;
            break;
        }
    }

    R_BSP_SoftwareUnlock(&g_ctsu_ctrl.lock);
    return err;

} /* End of function R_CTSU_ReadReg() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_CTSU_WriteReg
* Description  : This function writes the passed in value to the specified hardware register, except for the CTSUSSC,
*                SO0, and SO1 cases, where the value is written to the configuration variables associated with the
*                specified key instead.
* Arguments    : reg -
*                    ID for register to read
*                element -
*                    For CTSUSSC,SO0,SO1 only, element associated with sensor/sensor pair (mutual)
*                value -
*                    Value to write to register
* Return Value : QE_SUCCESS
*                    Register value written.
*                QE_CTSU_INVALID_ARG
*                    Unsupported register, or unrecognized register or key id.
*                QE_ERR_BUSY
*                    Cannot run function because another operation is in progress.
***********************************************************************************************************************/
qe_err_t R_CTSU_WriteReg(ctsu_reg_t reg, uint8_t element, uint16_t value)
{
    /* cast argument to byte for 8-bit registers */
    uint8_t   byte_value = (uint8_t) value;
    qe_err_t  err = QE_SUCCESS;

#if (CTSU_CFG_PARAM_CHECKING_ENABLE)
    if ((CTSU_REG_MCH1 == reg)
     || ((CTSU_REG_MCH0 == reg) && (CTSU_MODE_SELF_SINGLE_SCAN != g_ctsu_ctrl.mode))
     || (reg >= CTSU_REG_END_ENUM)
     || (element >= gp_cur_cfg->num_elements))
    {
        return QE_ERR_INVALID_ARG;
    }
#endif

    /* Check if another API call is in progress */
    if (R_BSP_SoftwareLock(&g_ctsu_ctrl.lock) == false)
    {
        return QE_ERR_BUSY;
    }


    /* Write register value */
    switch (reg)
    {
        case CTSU_REG_CR0:
        {
            CTSU.CTSUCR0.BYTE = byte_value;
            break;
        }
        case CTSU_REG_CR1:
        {
            CTSU.CTSUCR1.BYTE = byte_value;
            break;
        }
        case CTSU_REG_SDPRS:
        {
            CTSU.CTSUSDPRS.BYTE = byte_value;
            break;
        }
        case CTSU_REG_MCH0:
        {
            CTSU.CTSUMCH0.BYTE = byte_value;       // user may need to set CTSUINIT afterwards
            break;
        }
        case CTSU_REG_CHAC0:
        {
            CTSU.CTSUCHAC0.BYTE = byte_value;
            break;
        }
        case CTSU_REG_CHAC1:
        {
            CTSU.CTSUCHAC1.BYTE = byte_value;
            break;
        }
#if (CTSU_HAS_LARGE_REG_SET)
        case CTSU_REG_CHAC2:
        {
            CTSU.CTSUCHAC2.BYTE = byte_value;
            break;
        }
        case CTSU_REG_CHAC3:
        {
            CTSU.CTSUCHAC3.BYTE = byte_value;
            break;
        }
        case CTSU_REG_CHAC4:
        {
            CTSU.CTSUCHAC4.BYTE = byte_value;
            break;
        }
#endif
        case CTSU_REG_CHTRC0:
        {
            CTSU.CTSUCHTRC0.BYTE = byte_value;
            break;
        }
        case CTSU_REG_CHTRC1:
        {
            CTSU.CTSUCHTRC1.BYTE = byte_value;
            break;
        }
#if (CTSU_HAS_LARGE_REG_SET)
        case CTSU_REG_CHTRC2:
        {
            CTSU.CTSUCHTRC2.BYTE = byte_value;
            break;
        }
        case CTSU_REG_CHTRC3:
        {
            CTSU.CTSUCHTRC3.BYTE = byte_value;
            break;
        }
        case CTSU_REG_CHTRC4:
        {
            CTSU.CTSUCHTRC4.BYTE = byte_value;
            break;
        }
#endif
        case CTSU_REG_ST:
        {
            CTSU.CTSUST.BYTE = byte_value;         // user may need to set CTSUINIT afterwards
            break;
        }
        case CTSU_REG_SSC:
        {
            gp_cur_cfg->p_elem_regs[element].ctsussc = value;
            break;
        }
        case CTSU_REG_SO0:
        {
            gp_cur_cfg->p_elem_regs[element].ctsuso0 = value;
            break;
        }
        case CTSU_REG_SO1:
        {
            gp_cur_cfg->p_elem_regs[element].ctsuso1 = value;
            break;
        }
#ifdef BSP_MCU_RX23_ALL
        case CTSU_REG_SC:
        {
            CTSU.CTSUSC.WORD = value;                   // user may need to set CTSUINIT afterwards
            break;
        }
        case CTSU_REG_RC:
        {
            CTSU.CTSURC.WORD = value;                   // user may need to set CTSUINIT afterwards
            break;
        }
#endif
        case CTSU_REG_ERRS:
        {
            CTSU.CTSUERRS.WORD = value;
            break;
        }
        default:
        {
            err = QE_ERR_INVALID_ARG;
            break;
        }
    }

    R_BSP_SoftwareUnlock(&g_ctsu_ctrl.lock);
    return err;

} /* End of function R_CTSU_WriteReg() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: r_ctsu_control_private
* Description  : This function processes driver special-operation commands for Touch layer interface only.
*                This should not be exposed to the user.
* Arguments    : pcmd -
*                    Private command to perform.
*                method -
*                    Method to perform command on. Typically QE_CUR_METHOD.
*                p_arg -
*                    Pointer to arguments specific to the command. Can be NULL.
* Return Value : QE_SUCCESS -
*                    Command completed successfully
*                QE_ERR_INVALID_ARG -
*                    Command or method unknown.
*                QE_ERR_NULL_PTR -
*                    Missing argument (p_arg).
***********************************************************************************************************************/
qe_err_t r_ctsu_control_private(ctsu_pcmd_t pcmd, uint8_t method, void *p_arg)
{
    qe_err_t        err=QE_SUCCESS;

    /* FUTURE CMD: may require software lock */


    if (QE_CUR_METHOD == method)
    {
        method = g_ctsu_ctrl.cur_method;
    }


    switch (pcmd)
    {
        case CTSU_PCMD_DISABLE_CTSU_INTS:
        {
#if (!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5))
            IEN(CTSU,CTSUWR)= 0;
            IEN(CTSU,CTSURD)= 0;
            IEN(CTSU,CTSUFN)= 0;
#else
            R_BSP_InterruptRequestDisable(VECT(CTSU,CTSUWR));
            R_BSP_InterruptRequestDisable(VECT(CTSU,CTSURD));
            R_BSP_InterruptRequestDisable(VECT(CTSU,CTSUFN));
#endif
            break;
        }
        case CTSU_PCMD_ENABLE_CTSU_INTS:
        {
#if (!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5))
            IEN(CTSU,CTSUWR)= 1;
            IEN(CTSU,CTSURD)= 1;
            IEN(CTSU,CTSUFN)= 1;
#else
            R_BSP_InterruptRequestEnable(VECT(CTSU,CTSUWR));
            R_BSP_InterruptRequestEnable(VECT(CTSU,CTSURD));
            R_BSP_InterruptRequestEnable(VECT(CTSU,CTSUFN));
#endif
            break;
        }
        case CTSU_PCMD_SET_OFFSET_TUNING_FLG:
        {
            g_ctsu_ctrl.p_methods[method]->ctsu_status.flag.offset_tuning = 1;  // do not allow scan-done callback
            CTSU.CTSUCR0.BIT.CTSUCAP = 0;                                       // configure for software triggering
            break;
        }
        case CTSU_PCMD_CLR_OFFSET_TUNING_FLG:
        {
            if (QE_TRIG_EXTERNAL == g_ctsu_ctrl.trigger)    // restore external trigger operation if was configured
            {
                CTSU.CTSUCR0.BIT.CTSUCAP = 1;               // specify external trigger usage
                CTSU.CTSUCR0.BIT.CTSUSTRT = 1;              // arm external trigger detect
            }
            g_ctsu_ctrl.p_methods[method]->ctsu_status.flag.offset_tuning = 0;

            break;
        }
        default:
        {
            err = QE_ERR_INVALID_ARG;
        }
    }

    return err;
} /* End of function r_ctsu_control_private() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_CTSU_Control
* Description  : This function processes driver special-operation commands.
* Arguments    : cmd -
*                    Command to perform.
*                method -
*                    Method to perform command on. Typically QE_CUR_METHOD.
*                p_arg -
*                    Pointer to arguments specific to the command.
*                    CTSU_CMD_SET_CALLBACK          void (*)(ctsu_isr_evt_t, void *)
*                    CTSU_CMD_GET_STATE             ctsu_state_t *
*                    CTSU_CMD_GET_STATUS            ctsu_status_t *
*                    CTSU_CMD_SET_METHOD            NULL
*                    CTSU_CMD_GET_METHOD_MODE       ctsu_mode_t *
*                    CTSU_CMD_GET_SCAN_INFO         scan_info_t *
*                    CTSU_CMD_SNOOZE_ENABLE         NULL
*                    CTSU_CMD_SNOZZE_DISABLE        NULL
*
* Return Value : QE_SUCCESS -
*                    Command completed successfully
*                QE_ERR_INVALID_ARG -
*                    Command or method unknown.
*                QE_ERR_NULL_PTR -
*                    Missing argument (p_arg).
***********************************************************************************************************************/
qe_err_t R_CTSU_Control(ctsu_cmd_t cmd, uint8_t method, void *p_arg)
{
    qe_err_t        err=QE_SUCCESS;
    uint8_t         byte;
    bool            do_unlock = false;
    ctsu_mode_t     *p_mode=(ctsu_mode_t *)p_arg;
    ctsu_status_t   *p_status=(ctsu_status_t *)p_arg;
    ctsu_state_t    *p_state=(ctsu_state_t *)p_arg;
    scan_info_t     *p_scan_info=(scan_info_t *)p_arg;


#if (CTSU_CFG_PARAM_CHECKING_ENABLE)
    if (cmd >= CTSU_CMD_END_ENUM)
    {
        return QE_ERR_INVALID_ARG;      // command out of range
    }

    if (((CTSU_CMD_SET_CALLBACK != cmd) && (CTSU_CMD_GET_STATE != cmd) &&
         (CTSU_CMD_SNOOZE_ENABLE != cmd) && (CTSU_CMD_SNOOZE_DISABLE != cmd))
      && ((method >= g_ctsu_ctrl.num_methods) && (QE_CUR_METHOD != method)))
    {
        return QE_ERR_INVALID_ARG;      // method out of range
    }

    if (((CTSU_CMD_GET_STATE == cmd) || (CTSU_CMD_GET_STATUS == cmd)
     || (CTSU_CMD_SET_CALLBACK == cmd) || (CTSU_CMD_GET_SCAN_INFO == cmd))
     && (NULL == p_arg))
    {
        return QE_ERR_NULL_PTR;         // missing p_arg argument
    }
#endif

    /* if not a Get command */
    if ((CTSU_CMD_GET_STATE != cmd) && (CTSU_CMD_GET_STATUS != cmd) && (CTSU_CMD_GET_SCAN_INFO != cmd))
    {
        /* Check if another API call is in progress/get lock */
        if (R_BSP_SoftwareLock(&g_ctsu_ctrl.lock) == false)
        {
            return QE_ERR_BUSY;         // failed to get lock
        }
        else
        {
            do_unlock = true;           // got lock; set flag to unlock driver after command processed
        }
    }

    if (QE_CUR_METHOD == method)
    {
        method = g_ctsu_ctrl.cur_method;
    }

    switch (cmd)
    {
        /* Method independent commands */

        case CTSU_CMD_SET_CALLBACK:
        {
            /* cast argument to function prototype */
            g_ctsu_ctrl.p_callback = (void (*)(ctsu_isr_evt_t, void *))p_arg;
            break;
        }
        case CTSU_CMD_GET_STATE:
        {
            *p_state = g_ctsu_ctrl.state;
            break;
        }

        /* Method dependent commands */

        case CTSU_CMD_GET_STATUS:
        {
            *p_status = g_ctsu_ctrl.p_methods[method]->ctsu_status;
            break;
        }
        case CTSU_CMD_SET_METHOD:
        {
            init_method(method);
            break;
        }
        case CTSU_CMD_GET_METHOD_MODE:
        {
            byte = gp_ctsu_cfgs[method]->ctsucr1;

            /* get CTSU.CTSUCR1.BIT.CTSUMD (bits 6 and 7) */
            *p_mode = (ctsu_mode_t) (byte >> 6);
            break;
        }
        case CTSU_CMD_GET_SCAN_INFO:
        {
            *p_scan_info = g_pvt_scan_info[method];       // structure value assignment
            break;
        }
        case CTSU_CMD_SNOOZE_ENABLE:
        {
#if BSP_MCU_SERIES_RX100
            CTSU.CTSUCR0.BIT.CTSUSNZ = 1;
#else
            err = QE_ERR_INVALID_ARG;                     // snooze unavailable on RX231/23W
#endif
            break;
        }
        case CTSU_CMD_SNOOZE_DISABLE:
        {
#if BSP_MCU_SERIES_RX100
            CTSU.CTSUCR0.BIT.CTSUSNZ = 0;
            R_BSP_SoftwareDelay(16, BSP_DELAY_MICROSECS); // wait for CTSU "wake up"; REQUIRED BY HARDWARE!
#else
            err = QE_ERR_INVALID_ARG;                     // snooze unavailable on RX231/23W
#endif
            break;
        }
        default:
        {
            err = QE_ERR_INVALID_ARG;
        }
    }

    if (true == do_unlock)
    {
        R_BSP_SoftwareUnlock(&g_ctsu_ctrl.lock);
    }

    return err;

} /* End of function R_CTSU_Control() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_CTSU_Close
* Description  : This function shuts down the CTSU peripheral.
* Arguments    : None
* Return Value : QE_SUCCESS -
*                    Peripheral shut down successfully.
*                QE_ERR_BUSY
*                    Cannot run function because another operation is in progress.
***********************************************************************************************************************/
qe_err_t R_CTSU_Close(void)
{
    qe_err_t err = QE_SUCCESS;


    /* Check if another API call is in progress */
    if (R_BSP_SoftwareLock(&g_ctsu_ctrl.lock) == false)
    {
        return QE_ERR_BUSY;
    }

    /* Disable interrupts for write, read, & scan done */
#if (!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5))
    IEN(CTSU,CTSUWR)= 0;
    IEN(CTSU,CTSURD)= 0;
    IEN(CTSU,CTSUFN)= 0;
#else
    R_BSP_InterruptRequestDisable(VECT(CTSU,CTSUWR));
    R_BSP_InterruptRequestDisable(VECT(CTSU,CTSURD));
    R_BSP_InterruptRequestDisable(VECT(CTSU,CTSUFN));
#endif


    /* Turn off power and capacitance switch */
    CTSU.CTSUCR1.BIT.CTSUPON = 0;
    CTSU.CTSUCR1.BIT.CTSUCSW = 0;

    /* Put CTSU peripheral into stop state (stop power/clock) */
    R_BSP_RegisterProtectDisable (BSP_REG_PROTECT_LPC_CGC_SWR);
    MSTP(CTSU) = 1;
    R_BSP_RegisterProtectEnable (BSP_REG_PROTECT_LPC_CGC_SWR);

    g_ctsu_ctrl.open = false;
    R_BSP_SoftwareUnlock(&g_ctsu_ctrl.lock);
    return err;

} /* End of function R_CTSU_Close() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section _QE_TOUCH_DRIVER
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE(P, _QE_TOUCH_DRIVER)
#endif
/***********************************************************************************************************************
* Function Name: R_CTSU_GetVersion
* Description  : Returns the current version of this module. The version number is encoded where the top 2 bytes are the
*                major version number and the bottom 2 bytes are the minor version number. For example, Version 4.25
*                would be returned as 0x00040019.
* Arguments    : none
* Return Value : Version of this module.
***********************************************************************************************************************/
#if (!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5))
#pragma inline(R_CTSU_GetVersion)
#else
R_BSP_PRAGMA_INLINE(R_CTSU_GetVersion)
#endif

uint32_t R_CTSU_GetVersion (void)
{
    /* These version macros are defined in r_ctsu_rx_if.h. */
    return ((((uint32_t)QECTSU_RX_VERSION_MAJOR) << 16) | (uint32_t)QECTSU_RX_VERSION_MINOR);
} /* End of function R_CTSU_GetVersion() */


#if ((!defined(R_BSP_VERSION_MAJOR) || (R_BSP_VERSION_MAJOR < 5)) && (CTSU_CFG_SAFETY_LINKAGE_ENABLE))
#pragma section
#elif (CTSU_CFG_SAFETY_LINKAGE_ENABLE)
R_BSP_ATTRIB_SECTION_CHANGE_END
#endif

