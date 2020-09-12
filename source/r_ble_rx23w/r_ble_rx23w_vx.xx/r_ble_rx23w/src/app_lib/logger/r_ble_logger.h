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

/*******************************************************************************************************************//**
 * @file
 * @defgroup logger Logger Library
 * @{
 * @ingroup app_lib
 * @brief Logger Library
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 23.08.2019 1.00    First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include "r_ble_rx23w_config.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#ifndef R_BLE_LOGGER_H
#define R_BLE_LOGGER_H

/** @defgroup logger_macro Macros 
 *  @{
 *  @brief Macro definition
 */
/*******************************************************************************************************************//**
 * @def BLE_DEFAULT_LOG_LEVEL
 * @brief Set default log level used throughout the project.
 * @details The default value is 3 to output all log level.
***********************************************************************************************************************/
#ifndef BLE_DEFAULT_LOG_LEVEL
#define BLE_DEFAULT_LOG_LEVEL (BLE_CFG_LOG_LEVEL)
#endif

/*******************************************************************************************************************//**
 * @def BLE_LOG_TAG
 * @brief Set module specific log level.
 * @details The default value is "----".
***********************************************************************************************************************/
#ifndef BLE_LOG_TAG
#define BLE_LOG_TAG "----"
#endif

/*******************************************************************************************************************//**
 * @def BLE_LOG_LEVEL
 * @brief Set module specific log level.
 * @details The default value is BLE_DEFAULT_LEVEL.
***********************************************************************************************************************/
#ifndef BLE_LOG_LEVEL
#define BLE_LOG_LEVEL (BLE_DEFAULT_LOG_LEVEL)
#endif

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/** @brief Log level. */
typedef enum
{
    BLE_LOG_LEVEL_ERR = 1,
    BLE_LOG_LEVEL_WRN = 2,
    BLE_LOG_LEVEL_DBG = 3,
} e_lib_logger_level;

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

#if (BLE_DEFAULT_LOG_LEVEL != 0) && (BLE_CFG_HCI_MODE_EN == 0)

/** @cond INTERNAL */
#define BLE_LOG(level_val, level_tag, ...)                              \
    do {                                                                \
        if (BLE_LOG_LEVEL >= level_val)                                 \
        {                                                               \
            printf("%s: [%s] (%s:%d) ", BLE_LOG_TAG, level_tag,         \
                   __func__, __LINE__);                                 \
            printf(__VA_ARGS__);                                        \
            printf("\n");                                               \
        }                                                               \
    } while (0)
/** @endcond */

/*******************************************************************************************************************//**
 * @def BLE_LOG_ERR
 * @brief Output log in error log level.
 * @details This macro is only usable when \ref BLE_LOG_LEVEL is \ref BLE_LOG_LEVEL_DBG or higher.
***********************************************************************************************************************/
#define BLE_LOG_ERR(...) BLE_LOG(BLE_LOG_LEVEL_ERR, "ERR", __VA_ARGS__)

/*******************************************************************************************************************//**
 * @def BLE_LOG_WRN
 * @brief Output log in warning log level.
 * @details This macro is only usable when BLE_LOG_LEVEL is BLE_LOG_LEVEL_DBG or higher.
***********************************************************************************************************************/
#define BLE_LOG_WRN(...) BLE_LOG(BLE_LOG_LEVEL_WRN, "WRN", __VA_ARGS__)

/*******************************************************************************************************************//**
 * @def BLE_LOG_DBG
 * @brief Output log in debug log level.
 * @details This macro is only usable when BLE_LOG_LEVEL is BLE_LOG_LEVEL_DBG or higher.
***********************************************************************************************************************/
#define BLE_LOG_DBG(...) BLE_LOG(BLE_LOG_LEVEL_DBG, "DBG", __VA_ARGS__)

#else /* (BLE_DEFAULT_LOG_LEVEL != 0) && (BLE_CFG_HCI_MODE_EN == 0) */

#define BLE_LOG(level, format, ...)
#define BLE_LOG_ERR(...)
#define BLE_LOG_WRN(...)
#define BLE_LOG_DBG(...)

#endif /* (BLE_DEFAULT_LOG_LEVEL != 0) && (BLE_CFG_HCI_MODE_EN == 0) */

/** @defgroup logger_func Functions
 *  @{
 *  @brief Function definition
 */
/*******************************************************************************************************************//**
 * @brief Convert BD address in bytes array into string.
 * @details
 * This function shall be called only in BLE_LOG_XXX argument. Because
 * the return value of this function is the pointer to the interval static buffer
 * which retains the BD address string. 
 * 
 * @param[in] p_addr BD address in byte array
 * @param[in] addr_type BD address type
 * @return BD address in string format
***********************************************************************************************************************/
char *BLE_BD_ADDR_STR(uint8_t *p_addr, uint8_t addr_type);

/*******************************************************************************************************************//**
 * @brief Convert UUID in bytes array into string.
 * @details This function shall be called only in BLE_LOG_XXX argument. Because
 * the return value of this function is the pointer to the interval static buffer
 * which retains the BD address string. 
 *
 * @param[in] p_uuid UUID in byte array
 * @param[in] uuid_type UUID type
 * @return UUID in string format
***********************************************************************************************************************/
char *BLE_UUID_STR(uint8_t *p_uuid, uint8_t uuid_type);
/*@}*/

#endif /* R_BLE_LOGGER_H */
/*@}*/
