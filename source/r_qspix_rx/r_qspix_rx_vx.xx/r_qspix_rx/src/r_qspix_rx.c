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
* Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 31.03.2021 1.00     First Release
*******************************************************************************/
/*******************************************************************************
* File Name    : r_qspix_rx.c
* Description  : Functions for using QSPIX on RX devices.
*******************************************************************************/
/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Access to peripherals and board defines. */
#include "platform.h"

/* Defines for QSPIX support */
#include "r_qspix_rx_if.h"

#if defined(BSP_MCU_RX671)
    #include "./targets/rx671/r_qspix_rx671_private.h"
#else
    #error "This MCU is not supported by the current r_qspix_rx module."
#endif


/*******************************************************************************
Private global variables and functions
*******************************************************************************/
/*----------------------------------------------------------------------------*/
/*  Main processing of QPSIX Driver API functions           */
/*----------------------------------------------------------------------------*/

/*******************************************************************************
 * Function Name: R_QSPIX_Open
 **************************************************************************/ /**
 * @brief Open the QSPIX driver module. After the driver is open, the QSPIX can be
 * accessed like internal flash memory starting at address 0x70000000.
 * @param[in] channel
 *             Channel of QSPIX need to open.
 * @param[in] p_cfg
 *             Pointer to a config structure.
 * @retval    QSPIX_SUCCESS             Processing completed without problem
 * @retval    QSPIX_ERR_OPENED          QSPIX was initialized already
 * @retval    QSPIX_ERR_INVALID_ARG     Invalid argument
 * @retval    QSPIX_ERR_NULL_PTR        Argument pointers are NULL
 * @retval    QSPIX_ERR_BUSY            The QSPIX resources are locked by another process
 * @details   Initializes an QSPIX channel for a particular mode.
 * @note      None
 */
qspix_err_t R_QSPIX_Open(uint8_t channel, qspix_cfg_t * p_cfg)
{
    qspix_err_t ret = QSPIX_SUCCESS;

#if QSPIX_CFG_PARAM_CHECKING_ENABLE
    /* ---- Check argument. ---- */
    ret = r_qspix_parameter_check(channel, p_cfg);
    if (QSPIX_SUCCESS != ret)
    {
        return ret;
    }
#endif
    /* ==== CALLS THE API FUNCTION ==== */
    ret = r_qspix_enable(channel, p_cfg);

    return ret;
}
/******************************************************************************
End of function R_QSPIX_Open
******************************************************************************/

/*******************************************************************************
 * Function Name: R_QSPIX_Close
 **************************************************************************/ /**
 * @brief Close the QSPIX driver module.
 * @param[in] channel
 *             Channel of QSPIX need to close.
 *
 * @retval    QSPIX_SUCCESS             Processing completed without problem
 * @retval    QSPIX_ERR_NOT_OPEN        QSPIX module is not initialized yet
 * @retval    QSPIX_ERR_INVALID_ARG     Invalid argument
 * @details   Disables the QSPIX channel and enters module-stop state.
 * @note      None
 */
qspix_err_t R_QSPIX_Close (uint8_t channel)
{
    qspix_err_t ret = QSPIX_SUCCESS;

#if QSPIX_CFG_PARAM_CHECKING_ENABLE
    /* ---- Check argument. ---- */
    ret = r_qspix_parameter_channel_check(channel);
    if (QSPIX_SUCCESS != ret)
    {
        return ret;
    }
#endif
    /* ==== CALLS THE API FUNCTION ==== */
    ret = r_qspix_disable(channel);

    return ret;
}
/******************************************************************************
End of function R_QSPIX_Close
******************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_QSPIX_Control
 *****************************************************************************************************************/ /**
 * @brief This function configures and controls the operating mode for the QSPIX channel.
 * @param[in] channel
 *             Channel of QSPIX need to change operating.
 * @param[in] p_cfg
 *             Pointer to a config structure.
 * @retval    QSPIX_SUCCESS             Processing completed without problem
 * @retval    QSPIX_ERR_NOT_OPEN        QSPIX module is not initialized yet
 * @retval    QSPIX_ERR_INVALID_ARG     Invalid argument
 * @retval    QSPIX_ERR_NULL_PTR        Argument pointers are NULL
 * @details   Modify operating of QSPIX channel for a particular mode.
 * @note      None
 */
qspix_err_t R_QSPIX_Control(uint8_t channel, qspix_cfg_t * p_cfg)
{
    qspix_err_t ret = QSPIX_SUCCESS;

#if QSPIX_CFG_PARAM_CHECKING_ENABLE
    /* ---- Check argument. ---- */
    ret = r_qspix_parameter_check(channel, p_cfg);
    if (QSPIX_SUCCESS != ret)
    {
        return ret;
    }
#endif
    /* ==== CALLS THE API FUNCTION ==== */
    ret = r_qspix_control(channel, p_cfg);

    return ret;
}
/******************************************************************************
End of function R_QSPIX_Control
******************************************************************************/

/******************************************************************************
 * Function Name: R_QSPIX_Read_Indirect
 *************************************************************************/ /**
 * @brief Reads raw data directly from the QSPIX. This API can only be called 
 * after R_QSPIX_DirectWrite with /n
 * read_after_write set to true.
 * @param[in] channel       Channel of QSPIX use to read data.
 * @param[in] p_des_addr    Pointer to store data
 * @param[in] bytes         Number of bytes to read
 * @retval    QSPIX_SUCCESS             Processing completed without problem
 * @retval    QSPIX_ERR_NOT_OPEN        QSPIX module is not initialized yet
 * @retval    QSPIX_ERR_INVALID_ARG     Invalid argument
 * @retval    QSPIX_ERR_NULL_PTR        Argument pointers are NULL
 * @details   Uses the QSPIX of the channel number specified by the argument channel to read data.
 * @note      None
 */
qspix_err_t R_QSPIX_Read_Indirect(uint8_t channel,
                                   uint8_t *p_des_addr,
                                   uint32_t bytes)
{
    qspix_err_t ret = QSPIX_SUCCESS;

    /* ==== CALLS THE API FUNCTION ==== */
    ret = r_qspix_read_indirect(channel, p_des_addr, bytes);
    return ret;
}
/******************************************************************************
End of function R_QSPIX_Read_Indirect
******************************************************************************/

/*****************************************************************************
 * Function Name: R_QSPIX_Write_Indirect
 *************************************************************************/ /**
 * @brief Writes raw data directly to the QSPIX.
 * @param[in] channel       Channel of QSPIX use to write data.
 * @param[in] p_src_addr    Pointer to data to write
 * @param[in] bytes         Number of bytes to write
 * @param[in] read_after_write         Whether or not to close SPI bus cycle
 * @retval    QSPIX_SUCCESS             Processing completed without problem
 * @retval    QSPIX_ERR_NOT_OPEN        QSPIX module is not initialized yet
 * @retval    QSPIX_ERR_INVALID_ARG     Invalid argument
 * @retval    QSPIX_ERR_NULL_PTR        Argument pointers are NULL
 * @details   Uses the QSPIX of the channel number specified by the argument channel to write data.
 * @note      None
 */
qspix_err_t R_QSPIX_Write_Indirect(uint8_t channel,
                                   uint8_t *p_src_addr,
                                   uint32_t bytes,
                                   bool read_after_write)
{
    qspix_err_t ret = QSPIX_SUCCESS;

    /* ==== CALLS THE API FUNCTION ==== */
    ret = r_qspix_write_indirect(channel,
                            p_src_addr,
                            bytes,
                            read_after_write);

    return ret;
}
/******************************************************************************
End of function R_QSPIX_Write_Indirect
******************************************************************************/

/*****************************************************************************
 * Function Name: R_QSPIX_Enter_XIP
 ************************************************************************/ /**
 * @brief Enters XIP (execute in place) mode.
 * @param[in] channel       Channel of QSPIX Enters XIP (execute in place) mode
 * @param[in] mode          XIP Mode
 * @retval    QSPIX_SUCCESS             Processing completed without problem
 * @retval    QSPIX_ERR_NOT_OPEN        QSPIX module is not initialized yet
 * @retval    QSPIX_ERR_HW              Hardware error
 * @note      None
 */
qspix_err_t R_QSPIX_Enter_XIP(uint8_t channel, uint8_t mode)
{
    qspix_err_t ret = QSPIX_SUCCESS;

    /* ==== CALLS THE API FUNCTION ==== */
    ret = r_qspix_xip(channel, mode, true);
    return ret;
}
/******************************************************************************
End of function R_QSPIX_Enter_XIP
******************************************************************************/

/*****************************************************************************
 * Function Name: R_QSPIX_Exit_XIP
 ************************************************************************/ /**
 * @brief Exit XIP (execute in place) mode.
 * @param[in] channel       Channel of QSPIX Exit XIP (execute in place) mode
 * @param[in] mode          XIP Mode
 * @retval    QSPIX_SUCCESS             Processing completed without problem
 * @retval    QSPIX_ERR_NOT_OPEN        QSPIX module is not initialized yet
 * @retval    QSPIX_ERR_HW              HW error.
 * @note      None
 */
qspix_err_t R_QSPIX_Exit_XIP(uint8_t channel, uint8_t mode)
{
    qspix_err_t ret = QSPIX_SUCCESS;

    /* ==== CALLS THE API FUNCTION ==== */
    ret = r_qspix_xip(channel, mode, false);
    return ret;
}
/******************************************************************************
End of function R_QSPIX_Exit_XIP
******************************************************************************/

/*****************************************************************************
 * Function Name: R_QSPIX_BankSet
 ************************************************************************/ /**
 * @brief Selects the bank to access. A bank is a 64MB sliding access window \n
 * into the QSPIX device flash memory space. To access chip address 0x4000000,\n
 * select bank 1, then read from internal flash address 0x70000000. To access\n
 * chip address 0x8001000, select bank 2, then read from internal flash\n
 * address 0x70001000. This function is not required for memory devices less\n
 *  than or equal to 512 Mb (64MB).
 * @param[in] channel       Channel for switch bank.
 * @param[in] bank          Bank need to set
 * @retval    QSPIX_SUCCESS             Processing completed without problem
 * @retval    QSPIX_ERR_INVALID_ARG     Arguments are invalid
 * @retval    QSPIX_ERR_NOT_OPEN        QSPIX module is not initialized yet
 * @note      None
 */
qspix_err_t R_QSPIX_BankSet (uint8_t channel, uint8_t bank)
{
    qspix_err_t ret = QSPIX_SUCCESS;

    /* ==== CALLS THE API FUNCTION ==== */
    ret = r_qspix_bankset(channel, bank);
    return ret;
}
/******************************************************************************
End of function R_QSPIX_BankSet
******************************************************************************/

/*****************************************************************************
 * Function Name: R_QSPIX_Set_Spi_Protocol
 ************************************************************************/ /**
 * @brief Selects the protocol to access. 
 * @param[in] channel       Channel for set protocol. 
 * @param[in] protocol      Protocol need to set.
 * @retval    QSPIX_SUCCESS             Processing completed without problem
 * @retval    QSPIX_ERR_INVALID_ARG     Arguments are invalid.
 * @retval    QSPIX_ERR_NOT_OPEN        QSPIX module is not initialized yet
 * @note      None
 */
qspix_err_t R_QSPIX_Set_Spi_Protocol(uint8_t channel, qspix_protocol_t protocol)
{
    qspix_err_t ret = QSPIX_SUCCESS;

    /* ==== CALLS THE API FUNCTION ==== */
    ret = r_qspix_set_spi_protocol(channel, protocol);
    return ret;
}
/******************************************************************************
End of function R_QSPIX_Set_Spi_Protocol
******************************************************************************/

/*****************************************************************************
 * Function Name: R_QSPIX_Get_Status
 ************************************************************************/ /**
 * @brief Get status of Prefetch, busy flag, ROM acess error flag
 * @param[in] cmd           Command to get status
 * @param[in] return_status Pointer store status corresponding with command
 * @retval    QSPIX_SUCCESS             Processing completed without problem
 * @retval    QSPIX_ERR_INVALID_COMMAND Command parameters are invalid.
 * @note      None
 */
qspix_err_t R_QSPIX_Get_Status(qspix_cmd_t cmd, uint8_t *return_status)
{
    qspix_err_t ret = QSPIX_SUCCESS;

    /* ==== CALLS THE API FUNCTION ==== */
    ret = r_qspix_get_status(cmd, return_status);
    return ret;
}
/******************************************************************************
End of function R_QSPIX_Get_Status
******************************************************************************/

/******************************************************************************
 * Function Name: R_QSPIX_GetVersion
 *************************************************************************/ /**
 * @brief This function returns the version number of the API.
 * @return    Version number
 * @details   This function returns the version number of this API.
 * @note      None
 */
uint32_t  R_QSPIX_GetVersion(void)
{
    uint32_t const  version = (QSPIX_VERSION_MAJOR << 16) | QSPIX_VERSION_MINOR;

    return version;
}
/******************************************************************************
End of function R_QSPIX_GetVersion
******************************************************************************/

/* End of File */

