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
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
 * File Name    : r_uid_rx.c
 * Description  : This module implements the Unique ID Read API
 ******************************************************************************/
/*******************************************************************************
 * History : DD.MM.YYYY Version Description           
 *         : 01.12.2014 1.00    First Release
 *         : 20.05.2019 1.11     Supported the following compilers:
 *                               - GCC for Renesas RX
 *                               - IAR C/C++ Compiler for Renesas RX
 *                               Added comments "WAIT_LOOP"
 *                               Deleted the inline expansion of the 
 *                               R_UID_GetVersion function.
 *         : 10.06.2020 1.14     Modified comment of API function to Doxygen style.
 ******************************************************************************/

/*******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "platform.h"
#include "r_uid_rx_if.h"
#include "r_uid_rx.h"
#include "r_uid_ram.h"

/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static void uid_codecopy(void);

/* State variable for the UID API. */
static uid_status_t g_uid_state = UID_UNINITIALIZED;

/* Lock variable for using the software lock function */
static BSP_CFG_USER_LOCKING_TYPE g_uid_lock;


/**********************************************************************************************************************
 * Function Name: R_UID_Open
 *****************************************************************************************************************/ /**
 * @brief This function transfers the program to read the unique ID from the ROM to the RAM for using this module.
 * @retval    UID_SUCCESS       R_UID_Open executed successfully.
 * @retval    UID_ERR_LOCK_FUNC R_UID_Open or R_UID_Read executed while either R_UID_Open or R_UID_Read is executed.
 * @retval    UID_ERR_FAILURE   R_UID_Open executed twice or more.
 * @details   Prepares to read the unique ID. The program to read the unique ID is transferred from the ROM to the RAM.
 * @note      None
 */
uid_err_t R_UID_Open(void)
{
    /* Allow Initialization if not initialized or
     * if no operation is ongoing and re-initialization is desired */
    if (UID_UNINITIALIZED != g_uid_state)
    {
        /* API already initialized */
        return UID_ERR_FAILURE;
    }

    if (true != R_BSP_SoftwareLock(&g_uid_lock))
    {
        /* API already initialized */
        return UID_ERR_LOCK_FUNC;
    }

    /* Copy over parts of the API that needs to run out of RAM. */
    uid_codecopy();

    /* change status */
    g_uid_state = UID_READY;

    /* Release state so other operations can be performed. */
    R_BSP_SoftwareUnlock(&g_uid_lock);

    return UID_SUCCESS;
}

/**********************************************************************************************************************
 * Function Name: R_UID_Read
 *****************************************************************************************************************/ /**
 * @brief This function reads the unique ID from the extra area.
 * @param[in,out] pdest_addr Specify the pointer to store the unique ID.\n
              32-byte of the unique ID data is stored in the area specified with the pointer. \n
              The size of the area specified with the pointer must be 32 bytes or more.
 * @retval    UID_SUCCESS           Unique ID read successfully with R_UID_Read.
 * @retval    UID_ERR_UNINITIALIZED R_UID_Read executed before executing R_UID_Open.
 * @retval    UID_ERR_LOCK_FUNC     R_UID_Open or R_UID_Read executed while either R_UID_Open or R_UID_Read is executed.
 * @retval    UID_ERR_FAILURE       Failed to read the unique id with R_UID_Read.
 * @details   Reads the unique ID from the extra area and stores it in the specified area.
 * @note      Before executing this function, disable generating interrupt requests, or allocate the vector table and 
              the program for interrupt handler in the RAM, and change the value in the interrupt table register (INTB).
              Also do not generate a non-maskable interrupt request.
 */
uid_err_t R_UID_Read(uint8_t *pdest_addr)
{
    uid_err_t err = UID_SUCCESS;

    /* Check if API is not initialized */
    if (UID_READY != g_uid_state)
    {
        return UID_ERR_UNINITIALIZED;
    }

    /* Check to see if lock was successfully taken */
    if (true != R_BSP_HardwareLock(BSP_LOCK_FLASH))
    {
        return UID_ERR_LOCK_FUNC;
    }

    /* Jump to RAM for Unique ID Read */
    err = uid_read_ram(pdest_addr);

    /* Release hold on lock */
    R_BSP_HardwareUnlock(BSP_LOCK_FLASH);

    return err;
}

/**********************************************************************************************************************
 * Function Name: R_UID_GetVersion
 *****************************************************************************************************************/ /**
 * @brief This function returns the API version.
 * @return Version number
 * @details Returns the API version number.
 * @note None
 */
uint32_t R_UID_GetVersion(void)
{
    /* These version macros are defined in r_flash_if.h. */
    return ((((uint32_t) UID_RX_VERSION_MAJOR) << 16)
            | (uint32_t) UID_RX_VERSION_MINOR);
}

/*******************************************************************************
 * Function Name: uid_codecopy
 * Description  : Copies Flash API code from ROM to RAM. This function needs to
 *              : be called before any Unique ID read functions.
 * Arguments    : none
 * Return Value : none
 * NOTE         : This function does not have to execute from in RAM.
 ******************************************************************************/
static void uid_codecopy(void)
{

    /* Pointer to beginning of RAM section where API code will be copied to. */
    uint8_t * pram_section;

    /* Pointer to beginning of ROM section with API code. */
    uint8_t * prom_section;

    /* Number of bytes copied. */
    uint32_t bytes_copied;

#if defined(__CCRX__) || defined(__GNUC__)

    /* Initialize RAM section pointer. */
    R_BSP_SECTION_OPERATORS_INIT(RPFRAM);
    pram_section = (uint8_t *) R_BSP_SECTOP(RPFRAM);

    /* Initialize ROM section pointer. */
    R_BSP_SECTION_OPERATORS_INIT(PFRAM);
    prom_section = (uint8_t *) R_BSP_SECTOP(PFRAM);

    /* Copy over code from ROM to RAM. */
    /* WAIT_LOOP */
    for (bytes_copied = 0; bytes_copied < R_BSP_SECSIZE(PFRAM); bytes_copied++)
    {
        /* Copy over data 1 byte at a time. */
        pram_section[bytes_copied] = prom_section[bytes_copied];
    }

#elif defined(__ICCRX__)

    /* Initialize RAM section pointer. */
    R_BSP_SECTION_OPERATORS_INIT(PFRAM);
    pram_section = (uint8_t *) R_BSP_SECTOP(PFRAM);

    /* Initialize ROM section pointer. */
    R_BSP_SECTION_OPERATORS_INIT(PFRAM_init);
    prom_section = (uint8_t *) R_BSP_SECTOP(PFRAM_init);

    /* Copy over code from ROM to RAM. */
#if defined(__LIT)

    /* WAIT_LOOP */
    for (bytes_copied = 0; bytes_copied < R_BSP_SECSIZE(PFRAM_init); bytes_copied++)
    {
        /* Copy over data 1 byte at a time. */
        pram_section[bytes_copied] = prom_section[bytes_copied];
    }

#elif defined(__BIG)

    /* WAIT_LOOP */
    for (bytes_copied = 0; bytes_copied < R_BSP_SECSIZE(PFRAM_init); bytes_copied+=4)
    {
        /* Copy over data 4 byte at a time. */
        pram_section[bytes_copied] = prom_section[bytes_copied+3];
        pram_section[bytes_copied+1] = prom_section[bytes_copied+2];
        pram_section[bytes_copied+2] = prom_section[bytes_copied+1];
        pram_section[bytes_copied+3] = prom_section[bytes_copied];
    }

#endif


#endif

}

/* end of file */
