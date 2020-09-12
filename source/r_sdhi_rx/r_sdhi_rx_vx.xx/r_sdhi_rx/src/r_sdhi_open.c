/**********************************************************************************************************************
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
* Copyright (C) 2017(2019) Renesas Electronics Corporation. All rights reserved.
**********************************************************************************************************************/
/**********************************************************************************************************************
* System Name  : SDHI Driver
* File Name    : r_sdhi_open.c
* Version      : 2.06
* Device       : RX
* Abstract     : API & Sub module
* Tool-Chain   : For RX e2_studio
* OS           : not use
* H/W Platform : RSK board for RX
* Description  : Interface file for SDHI API for RX
* Limitation   : None
**********************************************************************************************************************/
/**********************************************************************************************************************
* History      : DD.MM.YYYY Version Description
*              : 31.07.2017 2.00    First Release
*              : 20.05.2019 2.04    Added support for GNUC and ICCRX.
*                                   Fixed coding style. 
*              : 30.07.2019 2.05    Added  WAIT LOOP 
*              : 22.11.2019 2.06    Modified comment of API function to Doxygen style.
**********************************************************************************************************************/

/**********************************************************************************************************************
Includes <System Includes> , "Project Includes"
**********************************************************************************************************************/
#include "r_sdhi_rx_if.h"
#include ".\src\r_sdhi_rx_private.h"

/**********************************************************************************************************************
Macro definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Typedef definitions
**********************************************************************************************************************/

/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/
sdhi_sdhndl_t * g_sdhi_handle[((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t))];
sdhi_sdhndl_t g_sdhi_work_area[((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t))];

/**********************************************************************************************************************
 * Function Name: R_SDHI_Open
 *****************************************************************************************************************/ /**
 * @brief This function is run first when utilizing the API functions provided by the SDHI FIT module.
 * @param[in] channel
 *             Channel number : SDHI channel number to be used (starting from 0)
 * @retval    SDHI_SUCCESS Successful operation
 * @retval    SDHI_ERR     General error
 * @details   Obtains the SDHI channel resource specified by the argument channel, and initializes the SDHI FIT module
 *            and SDHI channel. Also, takes exclusive possession of the SDHI channel resource.
 * @note      The value after initialization of the swap control register (SDSWAP) depends on the endian setting.\n 
 *            \li Little endian: 0x00000000 (Swap write/read: Disable)
 *            \li Big endian: 0x000000c0 (Swap write/read: Enable)
 *
 *            Pin settings must be entered before running this function. 
 *            See Section 4 in application note for details.\n 
 *            If this function does not complete successfully, library functions other than R_SDHI_GetVersion(), 
 *            R_SDHI_Log(), and R_SDHI_SetLogHdlAddress() cannot be used.\n 
 *            The pin states remain unchanged before and after this function is run.
 */
sdhi_status_t R_SDHI_Open(uint32_t channel)
{
    sdhi_sdhndl_t   * p_hndl = 0;
    uint32_t          sdsts1 = 0;
    uint32_t          sdhi_sdopt_init = 0;
    uint32_t        * p_ptr = 0;
    int16_t           i = 0;
    bool              chk = false;

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_OPEN, __LINE__);
        return SDHI_ERR;
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    /*  Get the SDHI hardware resource.  */
    chk = r_sdhi_dev_hardware_lock(channel);

    if (false == chk)
    {
        /* Lock has already been acquired by another task. Needs to try again later. */
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_OPEN, __LINE__);
        return SDHI_ERR;
    }

    /*  Initialize the peripheral module.  */
    r_sdhi_dev_init(channel);

    /* Disable all interrupts. */
    R_SDHI_DisableIcuInt(channel, SDHI_HWINT_ACCESS_CD | SDHI_HWINT_BUFFER);

    p_hndl = (sdhi_sdhndl_t *)g_sdhi_work_area;

    g_sdhi_handle[channel] = p_hndl;

    /* ---- Clear the work memory to '0'. --- */
    p_ptr = (uint32_t *)p_hndl;

    /* WAIT_LOOP */
    for (i = (sizeof(sdhi_sdhndl_t) / sizeof(uint32_t)); i > 0; i--)
    {
        *p_ptr++ = 0;
    }

    /* ---- Set the SDHI register base address. ---- */
    p_hndl->reg_base = (uint32_t)g_sdhi_ip_base[channel];

    /*  Initialize the SDHI.  */
    SDHI_OUTREG(p_hndl, SDHI_SDRST, SDHI_SDRST_ENABLE);
    SDHI_OUTREG(p_hndl, SDHI_SDRST, SDHI_SDRST_DISABLE);
    SDHI_OUTREG(p_hndl, SDHI_SDIMSK1, SDHI_SDIMSK1_INIT);
    SDHI_OUTREG(p_hndl, SDHI_SDIMSK2, SDHI_SDIMSK2_INIT);
    SDHI_OUTREG(p_hndl, SDHI_SDIOIMSK, SDHI_SDIOIMSK_INIT);
    SDHI_OUTREG(p_hndl, SDHI_SDIOMD, SDHI_SDIOMD_CLEAR);
    sdsts1 = SDHI_INREG(p_hndl, SDHI_SDSTS1);
    SDHI_OUTREG(p_hndl, SDHI_SDSTS1, (sdsts1 & ~SDHI_SDIMSK1_TRNS_RESP));
    SDHI_OUTREG(p_hndl, SDHI_SDSTS2, SDHI_SDIMSK2_BIT11);
    SDHI_OUTREG(p_hndl, SDHI_SDIOSTS, SDHI_SDIOSTS_INIT);
    if (SDHI_SDRST_DISABLE == SDHI_INREG(p_hndl, SDHI_SDRST))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }
    
    sdhi_sdopt_init = r_sdhi_dev_sdopt_init();
    SDHI_OUTREG(p_hndl, SDHI_SDOPT, sdhi_sdopt_init);
#if (SDHI_BIG_ENDIAN)
    SDHI_OUTREG(p_hndl, SDHI_SDSWAP, SDHI_SDSWAP_ENABLE);
#else
    SDHI_OUTREG(p_hndl, SDHI_SDSWAP, SDHI_SDSWAP_DISABLE);
#endif
    if (SDHI_INREG(p_hndl, SDHI_SDSTS1) == sdsts1)
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    return SDHI_SUCCESS;
    
} /* End of function R_SDHI_Open() */

/**********************************************************************************************************************
 * Function Name: R_SDHI_Close
 *****************************************************************************************************************/ /**
 * @brief This function releases the resource currently in use by the SDHI FIT module.
 * @param[in] channel
 *             Channel number : SDHI channel number to be used (starting from 0)
 * @retval    SDHI_SUCCESS Successful operation
 * @retval    SDHI_ERR     General error
 * @details   Ends all processing by the SDHI FIT module, and releases the SDHI channel resource specified by 
 *            the argument channel.\n 
 *            The SDHI channel is then set to the module stop state.\n 
 *            Insertion-extraction interrupts are disabled after this function runs.
 * @note      Pin settings must be entered before running this function. See Section 4 in application note for details.
 *            Before running this function, initialization processing by the R_SDHI_Open() function is required.\n 
 *            The pin states remain unchanged before and after this function is run.
 */
sdhi_status_t R_SDHI_Close(uint32_t channel)
{
    sdhi_sdhndl_t   * p_hndl = 0;
    uint32_t          sdsts1 = 0;

#if (SDHI_CFG_PARAM_CHECKING_ENABLE == 1)
    if ((uint32_t)((sizeof(g_sdhi_ip_base)) / sizeof(uint32_t)) <= channel)
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_OPEN, __LINE__);
        return SDHI_ERR;
    }

    if (0 == g_sdhi_handle[channel])
    {
        SDHI_LOG_FUNC(SDHI_DEBUG_ERR_ID, (uint32_t)SDHI_OPEN, __LINE__);
        return SDHI_ERR;  /* Not initialized */
    }
#endif /* SDHI_CFG_PARAM_CHECKING_ENABLE */

    p_hndl = SDHI_GET_HNDL(channel);

    /* Disable all interrupts. */
    R_SDHI_DisableIcuInt(channel, SDHI_HWINT_ACCESS_CD | SDHI_HWINT_BUFFER);

    /* Reset the SDHI. */
    SDHI_OUTREG(p_hndl, SDHI_SDRST, SDHI_SDRST_ENABLE);
    SDHI_OUTREG(p_hndl, SDHI_SDRST, SDHI_SDRST_DISABLE);
    SDHI_OUTREG(p_hndl, SDHI_SDIMSK1, SDHI_SDIMSK1_INIT);
    SDHI_OUTREG(p_hndl, SDHI_SDIMSK2, SDHI_SDIMSK2_INIT);
    SDHI_OUTREG(p_hndl, SDHI_SDIOIMSK, SDHI_SDIOIMSK_INIT);
    SDHI_OUTREG(p_hndl, SDHI_SDIOMD, SDHI_SDIOMD_CLEAR);
    sdsts1 = SDHI_INREG(p_hndl, SDHI_SDSTS1);
    SDHI_OUTREG(p_hndl, SDHI_SDSTS1, (sdsts1 & ~SDHI_SDIMSK1_TRNS_RESP));
    SDHI_OUTREG(p_hndl, SDHI_SDSTS2, SDHI_SDIMSK2_BIT11);
    SDHI_OUTREG(p_hndl, SDHI_SDIOSTS, SDHI_SDIOSTS_INIT);

    if (SDHI_SDIOSTS_INIT == SDHI_INREG(p_hndl, SDHI_SDSTS1))
    {
        R_BSP_NOP();    /* Wait for the write completion. */
    }

    /* Finish the peripheral module. */
    r_sdhi_dev_finalize(channel);

    g_sdhi_handle[channel] = 0;    /* Destroy the SD Handle. */

    /* Release the SDHI hardware resource. */
    r_sdhi_dev_hardware_unlock(channel);

    return SDHI_SUCCESS;

} /* End of function R_SDHI_Close() */

/* End of File */
