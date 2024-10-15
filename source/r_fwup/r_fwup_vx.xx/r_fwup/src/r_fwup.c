/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2023-2024 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_fwup.c
 * Description  : Functions for the Firmware update module.
 **********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 20.07.2023 2.00    First Release
 *         : 29.09.2023 2.01    Fixed log messages.
 *                              Add parameter checking.
 *                              Added arguments to R_FWUP_WriteImageProgram API.
 *         : 28.03.2024 2.02    Update wrapper functions.
 *         : 09.04.2024 2.03    Fixed wrapper function.
 *         : 15.10.2024 2.04    Fixed wrapper function.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include "r_fwup_if.h"
#include "r_fwup_private.h"
#include "r_fwup_wrap_com.h"
#include "r_fwup_wrap_flash.h"
#include "r_fwup_wrap_verify.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#define FWUP_READ_BUF_SIZE                     (128U)
#define FWUP_COPY_BUF_SIZE                     (FWUP_CFG_CF_W_UNIT_SIZE)
#define FWUP_VERI_BUF_SIZE                     (128U)
#define FWUP_WRITE_HEADER_BUF_SIZE             (128U)
#define FWUP_IMAGE_FLAG                        (0xFEU)
#define FWUP_HASH_BYTES                        (32U)


/* Buffer area */
#if (FWUP_CFG_UPDATE_MODE == FWUP_SINGLE_BANK_W_BUFFER_EXT)
#define FWUP_BUF_AREA_ADDR_L                   (FWUP_CFG_EXT_BUF_AREA_ADDR_L)
#define FWUP_BUF_AREA_BLK_SIZE                 (FWUP_CFG_EXT_BUF_AREA_BLK_SIZE)
#define FWUP_BUF_AREA_NUM_BLKS                 (FWUP_CFG_AREA_SIZE / FWUP_BUF_AREA_BLK_SIZE)
#else
#define FWUP_BUF_AREA_ADDR_L                   (FWUP_CFG_BUF_AREA_ADDR_L)
#define FWUP_BUF_AREA_BLK_SIZE                 (FWUP_CFG_CF_BLK_SIZE)
#define FWUP_BUF_AREA_NUM_BLKS                 (FWUP_CFG_AREA_SIZE / FWUP_BUF_AREA_BLK_SIZE)
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_SINGLE_BANK_W_BUFFER_EXT) */

/* Data flash */
#define FWUP_DF_BLK_SIZE                       (FWUP_CFG_DF_BLK_SIZE)
#define FWUP_DF_NUM_BLKS                       (FWUP_CFG_DF_NUM_BLKS)
#define FWUP_DF_NUM_BYTES                      (FWUP_DF_BLK_SIZE * FWUP_DF_NUM_BLKS)

/**********************************************************************************************************************
 Local Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
 *********************************************************************************************************************/
/*
 * functions
 */
static e_fwup_err_t check_initial_rcv (e_fwup_area_t area);
static bool check_magic_code (e_fwup_area_t area);
static uint32_t get_offset_from_install_area (uint32_t addr);
static bool check_addr_range (uint32_t addr);
static e_fwup_err_t write_image_header (e_fwup_area_t area, uint8_t **p_buf, uint32_t *p_bufsz, uint32_t hdr_size);
static e_fwup_err_t write_image_prog (e_fwup_area_t area, uint8_t *p_buf, uint32_t buf_size);
static e_fwup_err_t write_image_offset_prog (e_fwup_area_t area, uint8_t *p_buf, uint32_t offset, uint32_t buf_size);
static e_fwup_err_t erase_area (e_fwup_area_t area);
static e_fwup_err_t write_area (e_fwup_area_t area, uint8_t **p_buf, uint32_t *p_bufsz, uint32_t offset, uint32_t size);
static e_fwup_err_t write_area_offset (e_fwup_area_t area, uint8_t **p_buf, uint32_t *p_bufsz, uint32_t offset, uint32_t size, uint32_t *p_write_offset, uint32_t *p_write_size);
static void read_area (e_fwup_area_t area, uint32_t * p_buf, uint32_t area_offset, uint32_t size);
static e_fwup_err_t copy_to_main_area (void);
static int32_t sha256_update (e_fwup_area_t area, void * vp_ctx, uint32_t area_offset, uint32_t datalen);
static uint8_t * hash_sha256 (e_fwup_area_t area);
static uint32_t get_flash_write_addr(e_fwup_area_t area, uint32_t buf_sz_tmp, uint32_t rsu_offset, uint32_t *p_write_offset, uint32_t *p_write_address, uint32_t *p_write_size);

/*
 * variables
 */
/* Magic code */
#if (FWUP_CFG_FWUPV1_COMPATIBLE == 0)
S_C_CH_FAR BOOT_LOADER_MAGIC_CODE[] = "RELFWV2";
#else
S_C_CH_FAR BOOT_LOADER_MAGIC_CODE[] = "Renesas";
#endif

#if (FWUP_CFG_PRINTF_DISABLE == 0)
/* for debug logging */
S_C_CH_FAR MSG_MAIN[] = "main";
S_C_CH_FAR MSG_BUFFER[] = "buffer";
S_C_CH_FAR MSG_OK[] = "OK\r\n";
S_C_CH_FAR MSG_NG[] = "NG\r\n";
S_C_CH_FAR MSG_VERIFY_INSTALL_AREA[] = "verify install area %s [%s]...";
S_C_CH_FAR MSG_WRITE_OK[]    = "W 0x%lX, %d ... OK\r\n";
#endif /* (FWUP_CFG_PRINTF_DISABLE == 0) */

/* user program size */
static uint32_t s_image_size = 0;
static uint32_t s_write_current_size = 0;

/* Flags */
static uint8_t s_initial_rcv_flg = 0;
static uint8_t s_img_header_write_flg = 0;
static uint8_t s_prg_list_write_flg = 0;
static uint8_t s_img_prog_write_flg = 0;

/* wrote counter */
static uint32_t s_wrote_counter = 0;

/*
 * API
 */
/**********************************************************************************************************************
 * Function Name: R_FWUP_Open
 * Description  : Open r_fwup module
 * Arguments    : None
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash open error
 *********************************************************************************************************************/
e_fwup_err_t R_FWUP_Open(void)
{
    /* Open internal flash */
    if (FWUP_SUCCESS != r_fwup_wrap_flash_open())
    {
        return (FWUP_ERR_FLASH);
    }

    s_image_size = 0;
    s_write_current_size = 0;
    s_initial_rcv_flg = 0;
    s_img_header_write_flg = 0;
    s_prg_list_write_flg = 0;
    s_img_prog_write_flg = 0;
    s_wrote_counter = 0;

#if (FWUP_CFG_UPDATE_MODE == FWUP_SINGLE_BANK_W_BUFFER_EXT)
    /* Open external flash */
    if (FWUP_SUCCESS != r_fwup_wrap_ext_flash_open())
    {
        return (FWUP_ERR_FLASH);
    }
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_SINGLE_BANK_W_BUFFER_EXT) */

    return (FWUP_SUCCESS);
}
/**********************************************************************************************************************
 End of function R_FWUP_Open
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_FWUP_Close
 * Description  : Close r_fwup module
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void R_FWUP_Close(void)
{
    /* Close internal flash */
    r_fwup_wrap_flash_close();

#if (FWUP_CFG_UPDATE_MODE == FWUP_SINGLE_BANK_W_BUFFER_EXT)
    /* Close external flash */
    r_fwup_wrap_ext_flash_close();
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_SINGLE_BANK_W_BUFFER_EXT) */
}
/**********************************************************************************************************************
 End of function R_FWUP_Close
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_FWUP_IsExistImage
 * Description  : Check existing image program
 * Arguments    : area    : FWUP_AREA_MAIN/FWUP_AREA_BUFFER
 * Return Value : true    : image program is found
 *                false   : image program is not found
 *********************************************************************************************************************/
bool R_FWUP_IsExistImage(e_fwup_area_t area)
{
    /* Parameter check. */
    if ((FWUP_AREA_MAIN != area) && (FWUP_AREA_BUFFER != area))
    {
        return (false);
    }

    return (check_magic_code(area));
}
/**********************************************************************************************************************
 End of function R_FWUP_IsExistImage
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_FWUP_EraseArea
 * Description  : Erasing area.
 * Arguments    : area           : FWUP_AREA_MAIN/FWUP_AREA_BUFFER/FWUP_AREA_DATA_FLASH
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash erase error
 *********************************************************************************************************************/
e_fwup_err_t R_FWUP_EraseArea(e_fwup_area_t area)
{
    return (erase_area(area));
}
/**********************************************************************************************************************
 End of function R_FWUP_EraseArea
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_FWUP_GetImageSize
 * Description  : Get image size
 * Arguments    : None
 * Return Value : 0     : calculating image size.
 *                other : image size
 *********************************************************************************************************************/
uint32_t R_FWUP_GetImageSize(void)
{
    /* Received Program list? */
    if (0 == s_prg_list_write_flg)
    {
        return (0);
    }
    return (s_image_size);
}
/**********************************************************************************************************************
 End of function R_FWUP_GetImageSize
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_FWUP_WriteImageHeader
 * Description  : Write image header.
 * Arguments    : area             : FWUP_AREA_MAIN/FWUP_AREA_BUFFER
 *                p_sig_type       : signature type
 *                p_sig            : signature
 *                sig_size         : signature size
 * Return Value : FWUP_SUCCESS     : success
 *                FWUP_ERR_FLASH   : flash write error
 *                FWUP_ERR_FAILURE : invalid parameter
 *********************************************************************************************************************/
e_fwup_err_t R_FWUP_WriteImageHeader(e_fwup_area_t area, uint8_t FWUP_FAR *p_sig_type,
                                     uint8_t FWUP_FAR *p_sig, uint32_t sig_size)
{
    uint32_t buf[FWUP_WRITE_HEADER_BUF_SIZE / sizeof(uint32_t)] = {0};
    uint8_t *p_buf_tmp = (uint8_t *)buf;
    uint32_t bufsz_tmp = sizeof (buf);
    st_fw_header_t *p_hdr = (st_fw_header_t *)buf;

    /* Parameter check. */
    if ((FWUP_AREA_MAIN != area) && (FWUP_AREA_BUFFER != area))
    {
        return (FWUP_ERR_FAILURE);
    }

    /* already exist header? */
    if (true == check_magic_code(area))
    {
        return (FWUP_SUCCESS);
    }

    if (FWUP_SIG_SIZE_MAX < sig_size)
    {
        return (FWUP_ERR_FAILURE);
    }

    /* Write to header */
    MEMCPY(p_hdr->magic_code, BOOT_LOADER_MAGIC_CODE, FWUP_MAGIC_CODE_LEN);
    p_hdr->image_flag = FWUP_IMAGE_FLAG;
    p_hdr->sig_size = sig_size;
    MEMCPY(p_hdr->sig_type, p_sig_type, STRLEN((const char FWUP_FAR *)p_sig_type));
    MEMCPY(p_hdr->sig, p_sig, sig_size);
    return (write_image_header(area, &p_buf_tmp, &bufsz_tmp, sizeof(buf)));
}
/**********************************************************************************************************************
 End of function R_FWUP_WriteImageHeader
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_FWUP_WriteImageProgram
 * Description  : write image program
 * Arguments    : area             : FWUP_AREA_MAIN/FWUP_AREA_BUFFER
 *                p_buf            : Image program
 *                offset           : Offset from beginning of RSU file image
 *                buf_size         : buffer size
 * Return Value : FWUP_SUCCESS     : updated image program
 *                FWUP_PROGRESS    : need more image program data
 *                FWUP_ERR_FLASH   : flash write error
 *                FWUP_ERR_FAILURE : invalid parameter.
 *********************************************************************************************************************/
e_fwup_err_t R_FWUP_WriteImageProgram(e_fwup_area_t area, uint8_t *p_buf, uint32_t offset, uint32_t buf_size)
{
    /* Parameter check. */
    if ((FWUP_AREA_MAIN != area) && (FWUP_AREA_BUFFER != area))
    {
        return (FWUP_ERR_FAILURE);
    }
    return (write_image_offset_prog(area, p_buf, offset, buf_size));
}
/**********************************************************************************************************************
 End of function R_FWUP_WriteImageProgram
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_FWUP_WriteImage
 * Description  : write image file
 * Arguments    : area             : FWUP_AREA_MAIN/FWUP_AREA_BUFFER
 *                p_buf            : User program data
 *                buf_size         : buffer size
 * Return Value : FWUP_SUCCESS     : updated image file
 *                FWUP_PROGRESS    : need more image file data
 *                FWUP_ERR_FLASH   : flash write error
 *                FWUP_ERR_FAILURE : invalid parameter
 *********************************************************************************************************************/
e_fwup_err_t R_FWUP_WriteImage(e_fwup_area_t area, uint8_t *p_buf, uint32_t buf_size)
{
    uint8_t *p_buf_tmp = p_buf;
    uint32_t bufsz_tmp = buf_size;
    e_fwup_area_t area_tmp;

    /* Parameter check. */
    if ((FWUP_AREA_MAIN != area) && (FWUP_AREA_BUFFER != area))
    {
        return (FWUP_ERR_FAILURE);
    }

#if (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK)
    area_tmp = FWUP_AREA_BUFFER;
#else
    area_tmp = area;
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK) */

    /* Initial receiving? */
    if (0 == s_initial_rcv_flg)
    {
        /* check magic code */
        if (0 != MEMCMP(p_buf, BOOT_LOADER_MAGIC_CODE, FWUP_MAGIC_CODE_LEN))
        {
            return (FWUP_ERR_FAILURE);
        }

        /* area erase */
        if (FWUP_SUCCESS != erase_area(area_tmp))
        {
            return (FWUP_ERR_FLASH);
        }
        s_initial_rcv_flg = 1;
    }

    /* Write image header */
    if (FWUP_ERR_FLASH == write_image_header(area_tmp, &p_buf_tmp, &bufsz_tmp, sizeof(st_fw_header_t)))
    {
        return (FWUP_ERR_FLASH);
    }

    /* Write image program */
    return (write_image_prog(area, p_buf_tmp, bufsz_tmp));
}
/**********************************************************************************************************************
 End of function R_FWUP_WriteImage
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_FWUP_VerifyImage
 * Description  : verify image program
 * Arguments    : area             : FWUP_AREA_MAIN/FWUP_AREA_BUFFER
 * Return Value : FWUP_SUCCESS     : success
 *                FWUP_ERR_VERIFY  : validation failed
 *                FWUP_ERR_FAILURE : invalid parameter
 *********************************************************************************************************************/
e_fwup_err_t R_FWUP_VerifyImage(e_fwup_area_t area)
{
    uint8_t *p_hash;
    uint32_t buf[FWUP_VERI_BUF_SIZE / sizeof(uint32_t)];
    st_fw_header_t *p_hdr = (st_fw_header_t *)buf;

    /* Parameter check. */
    if ((FWUP_AREA_MAIN != area) && (FWUP_AREA_BUFFER != area))
    {
        return (FWUP_ERR_FAILURE);
    }

    /* Firmware verification for the signature type. */
    read_area(area, buf, 0, FWUP_VERI_BUF_SIZE);
    FWUP_LOG_DBG(MSG_VERIFY_INSTALL_AREA, (FWUP_AREA_MAIN == area) ? MSG_MAIN : MSG_BUFFER, p_hdr->sig_type);

    p_hash = hash_sha256(area);
    if (0 != r_fwup_wrap_verify_ecdsa(p_hash, p_hdr->sig_type, p_hdr->sig, p_hdr->sig_size))
    {
        FWUP_LOG_DBG(MSG_NG);
        return (FWUP_ERR_VERIFY);
    }

    FWUP_LOG_DBG(MSG_OK);
    return (FWUP_SUCCESS);
}
/**********************************************************************************************************************
 End of function R_FWUP_VerifyImage
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_FWUP_ActivateImage
 * Description  : Activate New image program
 * Arguments    : None
 * Return Value : FWUP_SUCCESS     : success
 *                FWUP_ERR_FLASH   : flash error
 *********************************************************************************************************************/
e_fwup_err_t R_FWUP_ActivateImage(void)
{
#if (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK)
 #if (FWUP_CFG_FWUPV1_COMPATIBLE == 0)
    /* Bank swap. */
    return (r_fwup_wrap_bank_swap());
 #else
    return (FWUP_SUCCESS);
 #endif /* (FWUP_CFG_FWUPV1_COMPATIBLE == 0) */
#else
 #if (FWUP_CFG_FUNCTION_MODE == FWUP_FUNC_BOOTLOADER)
    /* Copy buffer area to main area. */
    return (copy_to_main_area());
 #else
    return (FWUP_SUCCESS);
 #endif
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK) */
}
/**********************************************************************************************************************
 End of function R_FWUP_ActivateImage
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_FWUP_ExecImage
 * Description  : Execute image program
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void R_FWUP_ExecImage(void)
{
    uint32_t vect_addr;

    vect_addr = *(uint32_t*)(FWUP_CFG_MAIN_AREA_ADDR_L + (FWUP_CFG_AREA_SIZE - 4U));
    r_fwup_wrap_disable_interrupt();
    ((void (*)(void))vect_addr)();
}
/**********************************************************************************************************************
 End of function R_FWUP_ExecImage
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_FWUP_SoftwareReset
 * Description  : Execute software reset.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void R_FWUP_SoftwareReset(void)
{
    r_fwup_wrap_software_reset();
}
/**********************************************************************************************************************
 End of function R_FWUP_SoftwareReset
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_FWUP_SoftwareDelay
 * Description  : Software delay timer
 * Arguments    : delay : delay count
 *                units : FWUP_DELAY_MICROSECS/FWUP_DELAY_MILLISECS/FWUP_DELAY_SECS
 * Return Value : 1     : success
 *                other : error
 *********************************************************************************************************************/
uint32_t R_FWUP_SoftwareDelay(uint32_t delay, e_fwup_delay_units_t units)
{
    return (r_fwup_wrap_software_delay(delay, units));
}
/**********************************************************************************************************************
 End of function R_FWUP_SoftwareDelay
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_FWUP_GetVersion
 * Description  : Get r_fwup module version
 * Arguments    : None
 * Return Value : The version number is encoded where the top 2 bytes are the major version number
 *                and the bottom 2 bytes are the minor version number.
 *                For example, Version 4.25 would be returned as 0x00040019.
 *********************************************************************************************************************/
uint32_t R_FWUP_GetVersion(void)
{
    /* These version macros are defined in r_fwup_if.h. */
    return (((((uint32_t)FWUP_VERSION_MAJOR) << 16) | (uint32_t)FWUP_VERSION_MINOR));
}
/**********************************************************************************************************************
 End of function R_FWUP_GetVersion
 *********************************************************************************************************************/

/*
 * Static functions
 */
/**********************************************************************************************************************
 * Function Name: check_initial_rcv
 * Description  : check data receiving in first time
 * Arguments    : area           : FWUP_AREA_MAIN/FWUP_AREA_BUFFER
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash erase error
 *********************************************************************************************************************/
static e_fwup_err_t check_initial_rcv(e_fwup_area_t area)
{
    /* Initial receiving? */
    if (0 == s_initial_rcv_flg)
    {
        /* area erase */
        if (FWUP_SUCCESS != erase_area(area))
        {
            return (FWUP_ERR_FLASH);
        }
        s_initial_rcv_flg = 1;
    }
    return (FWUP_SUCCESS);
}
/**********************************************************************************************************************
 End of function check_initial_rcv
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: check_magic_code
 * Description  : check magic code on MAIN_AREA/BUFFER_AREA
 * Arguments    : area     : FWUP_AREA_MAIN/FWUP_AREA_BUFFER
 * Return Value : true     : magic code matched
 *                false    : magic code unmatched
 *********************************************************************************************************************/
static bool check_magic_code(e_fwup_area_t area)
{
    uint32_t buf[2] = {0};

    /* Compare magic_code */
    read_area(area, (uint32_t *)&buf, 0, FWUP_MAGIC_CODE_LEN);
    if (0 != MEMCMP(buf, BOOT_LOADER_MAGIC_CODE, FWUP_MAGIC_CODE_LEN))
    {
        return (false);
    }
    return (true);
}
/**********************************************************************************************************************
 End of function check_magic_code
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: check_addr_range
* Description  : check addr is in range.
* Arguments    : addr  : flash address
* Return Value : true  : addr is in range.
*                false : addr is out of range.
**********************************************************************************************************************/
static bool check_addr_range(uint32_t addr)
{
    /* CF : Flash address -> install area offset */
    if (((FWUP_CFG_MAIN_AREA_ADDR_L + sizeof (st_fw_ctrlblk_t)) <= addr)
            && (addr <= (FWUP_CFG_MAIN_AREA_ADDR_L + FWUP_CFG_AREA_SIZE - 1)))
    {
        return (true);
    }

#if (FWUP_CFG_UPDATE_MODE != FWUP_SINGLE_BANK_WO_BUFFER)
    if (((FWUP_CFG_BUF_AREA_ADDR_L + sizeof (st_fw_ctrlblk_t)) <= addr)
            && (addr <= (FWUP_CFG_BUF_AREA_ADDR_L + FWUP_CFG_AREA_SIZE - 1)))
    {
        return (true);
    }
#endif /* (FWUP_CFG_UPDATE_MODE != FWUP_SINGLE_BANK_WO_BUFFER) */

    if ((0 < FWUP_DF_NUM_BLKS) &&
            (FWUP_CFG_DF_ADDR_L <= addr) && (addr < (FWUP_CFG_DF_ADDR_L + FWUP_DF_NUM_BYTES)))
    {
        return (true);
    }

    return (false);
}
/**********************************************************************************************************************
 End of function check_addr_range
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: get_offset_from_install_area
* Description  : flash address into offset in area.
* Arguments    : addr : flash address
* Return Value : offset in area
**********************************************************************************************************************/
static uint32_t get_offset_from_install_area(uint32_t addr)
{
    uint32_t ret_addr = 0;

    /* CF : Flash address -> install area offset */
    if ((FWUP_CFG_MAIN_AREA_ADDR_L <= addr) && (addr <= (FWUP_CFG_MAIN_AREA_ADDR_L + FWUP_CFG_AREA_SIZE - 1)))
    {
        ret_addr = addr - FWUP_CFG_MAIN_AREA_ADDR_L;
    }
#if (FWUP_CFG_UPDATE_MODE != FWUP_SINGLE_BANK_WO_BUFFER)
    else if ((FWUP_CFG_BUF_AREA_ADDR_L <= addr) && (addr <= (FWUP_CFG_BUF_AREA_ADDR_L + FWUP_CFG_AREA_SIZE - 1)))
    {
        ret_addr = addr - FWUP_CFG_BUF_AREA_ADDR_L;
    }
    else
    {
        ; /* Do nothing */
    }
#endif /* (FWUP_CFG_UPDATE_MODE != FWUP_SINGLE_BANK_WO_BUFFER) */
    return (ret_addr);
}
/**********************************************************************************************************************
 End of function get_offset_from_install_area
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: write_image_header
 * Description  : write image header from received buffer
 * Arguments    : area           : FWUP_AREA_MAIN/FWUP_AREA_BUFFER
 *                p_buf          : Image header
 *                p_bufsz        : buffer size
 *                hdr_size       : Header size
 * Return Value : FWUP_SUCCESS   : updated image program
 *                FWUP_PROGRESS  : need more image program data
 *                FWUP_ERR_FLASH : flash error
 *********************************************************************************************************************/
static e_fwup_err_t write_image_header(e_fwup_area_t area, uint8_t **p_buf, uint32_t *p_bufsz, uint32_t hdr_size)
{
    e_fwup_err_t ret_val = FWUP_PROGRESS;

    /* Already wrote? */
    if (0 == s_img_header_write_flg)
    {
        ret_val = write_area(area, p_buf, p_bufsz, 0, hdr_size);
        if (FWUP_SUCCESS != ret_val)
        {
            return (ret_val);
        }
        s_image_size += hdr_size;
        s_img_header_write_flg = 1;
    }
    return (FWUP_SUCCESS);
}
/**********************************************************************************************************************
 End of function write_image_header
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: write_image_prog
 * Description  : write image header from received buffer
 * Arguments    : area           : FWUP_AREA_MAIN/FWUP_AREA_BUFFER
 *                p_buf          : Image program
 *                buf_size       : buffer size
 * Return Value : FWUP_SUCCESS   : updated image program
 *                FWUP_PROGRESS  : need more image program data
 *                FWUP_ERR_FLASH : flash write error
 *********************************************************************************************************************/
static e_fwup_err_t write_image_prog(e_fwup_area_t area, uint8_t *p_buf, uint32_t buf_size)
{
    e_fwup_err_t ret_val = FWUP_PROGRESS;
    uint8_t *p_buf_tmp = p_buf;
    uint32_t buf_sz_tmp = buf_size;
    st_fw_desc_t dc;
    e_fwup_area_t area_tmp = area;
    e_fwup_area_t area_tmp_bak;
    static uint8_t fw_cnt = 0;
    uint32_t area_offset;

#if (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK)
    area_tmp = FWUP_AREA_BUFFER;
#else
    area_tmp = area;
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK) */

    /* No buffer */
    if (0 == buf_size)
    {
        return (FWUP_PROGRESS);
    }

    if (FWUP_SUCCESS != check_initial_rcv(area_tmp))
    {
        return (FWUP_ERR_FLASH);
    }

    /* Write descriptor */
    if (0 == s_prg_list_write_flg)
    {
        area_offset = sizeof(st_fw_header_t);
        ret_val = write_area(area_tmp, &p_buf_tmp, &buf_sz_tmp, sizeof(st_fw_header_t), sizeof(st_fw_desc_t));
        if (FWUP_SUCCESS != ret_val)
        {
            return (ret_val);
        }
        s_image_size += sizeof(st_fw_desc_t);

        /* get Image size */
        read_area(area_tmp, (uint32_t *)&dc, area_offset, sizeof(st_fw_desc_t));
        /* WAIT_LOOP */
        for (uint32_t i = 0; i < dc.n; i++)
        {
            /* write address in range? */
            if (false == check_addr_range(dc.fw[i].addr))
            {
                erase_area(area_tmp);
                return (FWUP_ERR_FLASH);
            }
            s_image_size += dc.fw[i].size;
        }

        /* Write flag is on */
        s_prg_list_write_flg = 1;

        /* IS there more buffer ? */
        if (0 == buf_sz_tmp)
        {
            return (FWUP_PROGRESS);
        }
    }

    /* write user program */
    if (0 == s_img_prog_write_flg)
    {
        /* WAIT_LOOP */
        while(1)
        {
            /* Get N, addr, size */
            area_tmp_bak = area_tmp;
            read_area(area_tmp, (uint32_t *)&dc, sizeof(st_fw_header_t), sizeof(st_fw_desc_t));
            if ((FWUP_CFG_DF_ADDR_L <= dc.fw[fw_cnt].addr) &&
                (dc.fw[fw_cnt].addr < (FWUP_CFG_DF_ADDR_L + FWUP_DF_NUM_BYTES)))
            {
                /* DF*/
                area_offset = dc.fw[fw_cnt].addr - FWUP_CFG_DF_ADDR_L;
                area_tmp = FWUP_AREA_DATA_FLASH;
            }
            else
            {
                /* CF : Flash address -> install area offset */
                area_offset = get_offset_from_install_area(dc.fw[fw_cnt].addr);
#if (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK)
                area_tmp = FWUP_AREA_BUFFER;
#else
                area_tmp = area;
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK) */
            }

            ret_val = write_area(area_tmp, &p_buf_tmp, &buf_sz_tmp, area_offset, dc.fw[fw_cnt].size);
            if (FWUP_SUCCESS != ret_val)
            {
                return (ret_val);
            }

            /* Next part */
            if (++fw_cnt >= dc.n)
            {
                s_img_prog_write_flg = 1;
                break;
            }

            /* IS there buffer to write ? */
            if (0 == buf_sz_tmp)
            {
                return (FWUP_PROGRESS);
            }
            area_tmp = area_tmp_bak;
        }
    }

    if (1 == s_img_prog_write_flg)
    {
        s_image_size = 0;
        s_initial_rcv_flg = 0;
        s_img_header_write_flg = 0;
        s_prg_list_write_flg = 0;
        s_img_prog_write_flg = 0;
    }
    return (ret_val);
}
/**********************************************************************************************************************
 End of function write_image_prog
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: write_image_offset_prog
 * Description  : write image header from received buffer
 * Arguments    : area             : FWUP_AREA_MAIN/FWUP_AREA_BUFFER
 *                p_buf            : Image program
 *                offset           : offset
 *                buf_size         : buffer size
 * Return Value : FWUP_SUCCESS     : updated image program
 *                FWUP_PROGRESS    : need more image program data
 *                FWUP_ERR_FLASH   : flash write error
 *                FWUP_ERR_FAILURE : invalid parameter
 *********************************************************************************************************************/
static e_fwup_err_t write_image_offset_prog(e_fwup_area_t area, uint8_t *p_buf, uint32_t offset, uint32_t buf_size)
{
    e_fwup_err_t ret_val = FWUP_PROGRESS;
    uint8_t *p_buf_tmp = p_buf;
    uint32_t buf_sz_tmp = buf_size;
    st_fw_desc_t dc;
    e_fwup_area_t area_tmp = area;
    e_fwup_area_t area_tmp_bak;
    static uint8_t fw_cnt = 0;
    uint32_t area_offset;
    uint32_t rsu_offset = 0;
    uint32_t write_offset = 0;
    uint32_t write_address;
    uint32_t write_size = 0;

#if (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK)
    area_tmp = FWUP_AREA_BUFFER;
#else
    area_tmp = area;
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK) */

    /* No buffer */
    if (0 == buf_size)
    {
        return (FWUP_PROGRESS);
    }

    if (FWUP_SUCCESS != check_initial_rcv(area_tmp))
    {
        return (FWUP_ERR_FLASH);
    }

    /* Write descriptor */
    if (0 == s_prg_list_write_flg)
    {
        if (offset != sizeof(st_fw_header_t))
        {
            return (FWUP_ERR_FAILURE);
        }
        area_offset = sizeof(st_fw_header_t);
        write_offset = 0;
        ret_val = write_area_offset(area_tmp, &p_buf_tmp, &buf_sz_tmp, area_offset, sizeof(st_fw_desc_t), &write_offset, &write_size);
        write_offset = 0;

        if (FWUP_SUCCESS != ret_val)
        {
            return (ret_val);
        }
        s_image_size += sizeof(st_fw_desc_t);

        /* get Image size */
        read_area(area_tmp, (uint32_t *)&dc, area_offset, sizeof(st_fw_desc_t));
        /* WAIT_LOOP */
        for (uint32_t i = 0; i < dc.n; i++)
        {
            /* write address in range? */
            if (false == check_addr_range(dc.fw[i].addr))
            {
                erase_area(area_tmp);
                return (FWUP_ERR_FLASH);
            }
            s_image_size += dc.fw[i].size;
        }
        /* Write flag is on */
        s_prg_list_write_flg = 1;

        /* Set rsu_offset to the beginning of the user program. */
        rsu_offset = 0;

        /* IS there more buffer ? */
        if (0 == buf_sz_tmp)
        {
            return (FWUP_PROGRESS);
        }
    }

    /* write user program */
    if (0 == s_img_prog_write_flg)
    {
        /* offset < 0x200 */
        if (offset < sizeof(st_fw_header_t))
        {
            return (FWUP_ERR_FAILURE);
        }
        /* 0x200 < offset < 0x300 */
        if ((offset > sizeof(st_fw_header_t)) && (offset < (sizeof(st_fw_header_t) + sizeof(st_fw_desc_t))))
        {
            return (FWUP_ERR_FAILURE);
        }
        /* The following processing assumes an offset of 0x200 or 0x300 or higher. */
        if (offset != sizeof(st_fw_header_t))
        {
            rsu_offset = offset - sizeof(st_fw_header_t) - sizeof(st_fw_desc_t);
        }
        /* Get N, addr, size */
        area_tmp_bak = area_tmp;
        read_area(area_tmp, (uint32_t *)&dc, sizeof(st_fw_header_t), sizeof(st_fw_desc_t));
        fw_cnt = get_flash_write_addr(area_tmp, buf_sz_tmp, rsu_offset, &write_offset, &write_address, &write_size);
        if (fw_cnt < FWUP_IMAGE_BLOCKS)
        {
            if ((FWUP_CFG_DF_ADDR_L <= write_address) &&
                (write_address < (FWUP_CFG_DF_ADDR_L + FWUP_DF_NUM_BYTES)))
            {
                /* DF*/
                area_offset = write_address - FWUP_CFG_DF_ADDR_L;
                area_tmp = FWUP_AREA_DATA_FLASH;
            }
            else
            {
                /* CF : Flash address -> install area offset */
                area_offset = get_offset_from_install_area(write_address);
#if (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK)
                area_tmp = FWUP_AREA_BUFFER;
#else
                area_tmp = area;
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK) */
            }
            ret_val = write_area_offset(area_tmp, &p_buf_tmp, &buf_sz_tmp, area_offset, dc.fw[fw_cnt].size, &write_offset, &write_size);
            if (FWUP_ERR_FLASH == ret_val)
            {
                return (ret_val);
            }
            if ((FWUP_SUCCESS == ret_val) || (FWUP_PROGRESS == ret_val))
            {
                /* IS there buffer to write ? */
                if (0 == buf_sz_tmp)
                {
                    if (s_image_size <= s_write_current_size)
                    {
                        s_img_prog_write_flg = 1;
                        s_image_size = 0;
                        s_initial_rcv_flg = 0;
                        s_prg_list_write_flg = 0;
                        s_img_prog_write_flg = 0;
                        s_write_current_size = 0;
                        return (FWUP_SUCCESS);
                    }
                    else
                    {
                        return (FWUP_PROGRESS);
                    }
                }
                else
                {
                    /* Next part */
                    if (++fw_cnt >= dc.n)
                    {
                        return (FWUP_ERR_FAILURE);
                    }
                }
                area_tmp = area_tmp_bak;
            }
        }
        else
        {
            return (FWUP_ERR_FAILURE);
        }

        /* WAIT_LOOP */
        while(1)
        {
            /* Get N, addr, size */
            area_tmp_bak = area_tmp;
            read_area(area_tmp, (uint32_t *)&dc, sizeof(st_fw_header_t), sizeof(st_fw_desc_t));
            if ((FWUP_CFG_DF_ADDR_L <= dc.fw[fw_cnt].addr) &&
                (dc.fw[fw_cnt].addr < (FWUP_CFG_DF_ADDR_L + FWUP_DF_NUM_BYTES)))
            {
                /* DF*/
                area_offset = dc.fw[fw_cnt].addr - FWUP_CFG_DF_ADDR_L;
                area_tmp = FWUP_AREA_DATA_FLASH;
            }
            else
            {
                /* CF : Flash address -> install area offset */
                if (dc.fw[fw_cnt].size >= buf_sz_tmp)
                {
                    write_size = buf_sz_tmp;
                }
                else
                {
                    write_size = dc.fw[fw_cnt].size;
                }
                write_address = dc.fw[fw_cnt].addr + write_offset;
                area_offset = get_offset_from_install_area(write_address);
#if (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK)
                area_tmp = FWUP_AREA_BUFFER;
#else
                area_tmp = area;
#endif /* (FWUP_CFG_UPDATE_MODE == FWUP_DUAL_BANK) */
            }
            ret_val = write_area_offset(area_tmp, &p_buf_tmp, &buf_sz_tmp, area_offset, dc.fw[fw_cnt].size, &write_offset, &write_size);
            if ((FWUP_SUCCESS == ret_val) || (FWUP_PROGRESS == ret_val))
            {
                /* IS there buffer to write ? */
                if (0 == buf_sz_tmp)
                {
                    if (s_image_size <= s_write_current_size)
                    {
                        s_img_prog_write_flg = 1;
                        ret_val = FWUP_SUCCESS;
                    }
                    else
                    {
                        ret_val = FWUP_PROGRESS;
                    }
                    break;
                }
                else
                {
                    /* Next part */
                    if (++fw_cnt >= dc.n)
                    {
                        return (FWUP_ERR_FAILURE);
                    }
                }
            }
            else
            {
                return (ret_val);
            }
            area_tmp = area_tmp_bak;
        }
    }

    if (1 == s_img_prog_write_flg)
    {
        s_image_size = 0;
        s_initial_rcv_flg = 0;
        s_prg_list_write_flg = 0;
        s_img_prog_write_flg = 0;
        s_write_current_size = 0;
    }
    return (ret_val);
}
/**********************************************************************************************************************
 End of function write_image_offset_prog
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: erase_area
* Description  : Erase designated install area.
* Arguments    : area           : FWUP_AREA_MAIN/FWUP_AREA_BUFFER/FWUP_AREA_DATA_FLASH
* Return Value : FWUP_SUCCESS   : success
*                FWUP_ERR_FLASH : flash erase error
**********************************************************************************************************************/
static e_fwup_err_t erase_area(e_fwup_area_t area)
{
    e_fwup_err_t (*pfunc)(uint32_t, uint32_t) = r_fwup_wrap_flash_erase;
    uint32_t addr = FWUP_CFG_MAIN_AREA_ADDR_L;
    uint32_t num_blk = (FWUP_CFG_AREA_SIZE / FWUP_CFG_CF_BLK_SIZE);

    switch(area)
    {
        case FWUP_AREA_BUFFER:
            addr = FWUP_BUF_AREA_ADDR_L;
            num_blk = FWUP_BUF_AREA_NUM_BLKS;
            pfunc = FWUP_FUNC_BUFFER_AREA_ERASE;
            break;
        case FWUP_AREA_DATA_FLASH:
            addr = FWUP_CFG_DF_ADDR_L;
            num_blk = FWUP_CFG_DF_NUM_BLKS;
            break;
        default:
            break;
    }
    return (pfunc(addr, num_blk));
}
/**********************************************************************************************************************
 End of function erase_area
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: write_area
 * Description  : Write firmware data to designated area.
 * Arguments    : area           : FWUP_AREA_MAIN/FWUP_AREA_BUFFER/FWUP_AREA_DATA_FLASH
 *                p_buf:(i/o)    : (i)image data's pointer ,  (o)left buffer
 *                p_bufsz:(i/o)  : (i)buffer size,            (o)left buffer size
 *                offset         : write offset from install area. (not flash address!!!)
 *                size           : write size
 * Return Value : FWUP_SUCCESS   : done     (enough buffer)
 *                FWUP_PROGRESS  : progress (not enough buffer)
 *                FWUP_ERR_FLASH : flash write error
 *********************************************************************************************************************/
static e_fwup_err_t write_area(e_fwup_area_t area, uint8_t **p_buf,
                               uint32_t *p_bufsz, uint32_t offset, uint32_t size)
{
    uint32_t start_addr = FWUP_CFG_MAIN_AREA_ADDR_L + offset;
    e_fwup_err_t (*pfunc)(uint32_t, uint32_t, uint32_t) = r_fwup_wrap_flash_write;
    uint32_t write_size_tmp;

    if (FWUP_AREA_BUFFER == area)
    {
        pfunc = FWUP_FUNC_BUFFER_AREA_WRITE;
        start_addr = FWUP_BUF_AREA_ADDR_L + offset;
    }
    else if (FWUP_AREA_DATA_FLASH == area)
    {
        start_addr = FWUP_CFG_DF_ADDR_L + offset;
    }
    else
    {
        ; /* Do nothing */
    }

    /* Is buffer enough? */
    if (size > (s_wrote_counter + (*p_bufsz)))
    {
        /* not enough */
        write_size_tmp = *p_bufsz;
    }
    else
    {
        /* enough */
        write_size_tmp = size - s_wrote_counter;
    }

    /* Write firmware */
    if (FWUP_SUCCESS != pfunc((uint32_t)*p_buf, start_addr + s_wrote_counter, write_size_tmp))
    {
        return (FWUP_ERR_FLASH);
    }
    FWUP_LOG_DBG(MSG_WRITE_OK, start_addr + s_wrote_counter, write_size_tmp);

    s_wrote_counter += write_size_tmp;
    *p_buf += write_size_tmp;
    *p_bufsz -= write_size_tmp;
    if (size > s_wrote_counter)
    {
        return (FWUP_PROGRESS);
    }

    s_wrote_counter = 0;
    return (FWUP_SUCCESS);
}
/**********************************************************************************************************************
 End of function write_area
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: read_area
* Description  : read data from main area / buffer area.
* Arguments    : area        : FWUP_AREA_MAIN/FWUP_AREA_BUFFER/FWUP_AREA_DATA_FLASH
*                p_buf       : buffer pointer
*                area_offset : FWUP_AREA_MAIN/FWUP_AREA_BUFFER : offset in area.
*                            : FWUP_AREA_DATA_FLASH : flash address
*                size        : read size
* Return Value : none
**********************************************************************************************************************/
static void read_area(e_fwup_area_t area, uint32_t * p_buf, uint32_t area_offset, uint32_t size)
{
    uint32_t flash_addr;
    e_fwup_err_t (*pfunc)(uint32_t, uint32_t, uint32_t) = r_fwup_wrap_flash_read;

    if (FWUP_AREA_MAIN == area)
    {
        flash_addr = area_offset + FWUP_CFG_MAIN_AREA_ADDR_L;
    }
    else if (FWUP_AREA_BUFFER == area)
    {
        pfunc = FWUP_FUNC_BUFFER_AREA_READ;
        flash_addr = area_offset + FWUP_BUF_AREA_ADDR_L;
    }
    else
    {
        /* Data flash */
        flash_addr = area_offset;
    }
    pfunc((uint32_t)p_buf, flash_addr, size);
}
/**********************************************************************************************************************
 End of function read_area
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * Function Name: copy_to_main_area
 * Description  : Copy buffer area to main area
 * Arguments    : None
 * Return Value : FWUP_SUCCESS   : success
 *                FWUP_ERR_FLASH : flash error
 *********************************************************************************************************************/
static e_fwup_err_t copy_to_main_area(void)
{
#if (FWUP_CFG_UPDATE_MODE != FWUP_DUAL_BANK) && (FWUP_CFG_UPDATE_MODE != FWUP_SINGLE_BANK_WO_BUFFER)
    uint32_t    read_addr = 0;
    int32_t     block_cnt;
    uint32_t    max_cnt;
    uint32_t    cf_block[FWUP_COPY_BUF_SIZE / sizeof(uint32_t)];

    /* Erase main area */
    if (FWUP_SUCCESS != erase_area(FWUP_AREA_MAIN))
    {
        return (FWUP_ERR_FLASH);
    }

    /* Copy to main area */
    max_cnt = (FWUP_CFG_AREA_SIZE / FWUP_COPY_BUF_SIZE);
    /* WAIT_LOOP */
    for (block_cnt = 0; max_cnt > block_cnt; block_cnt++)
    {
        /* Read firmware from buffer area */
        read_area(FWUP_AREA_BUFFER, cf_block, read_addr, FWUP_COPY_BUF_SIZE);

        /* Write firmware to main area */
        if (FWUP_SUCCESS !=
                r_fwup_wrap_flash_write((uint32_t) cf_block,
                                        (uint32_t) (FWUP_CFG_MAIN_AREA_ADDR_L + (FWUP_COPY_BUF_SIZE * block_cnt)),
                                        FWUP_COPY_BUF_SIZE))
        {
            return (FWUP_ERR_FLASH);
        }
        read_addr += FWUP_COPY_BUF_SIZE;
    }

    /* Erase buffer area */
    return (erase_area(FWUP_AREA_BUFFER));
#else
    return (FWUP_SUCCESS);
#endif /* (FWUP_CFG_UPDATE_MODE != FWUP_DUAL_BANK) */
}
/**********************************************************************************************************************
 End of function copy_to_main_area
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: sha256_update
 * Description  : calculate SHA256 hash of designated install area. (MAIN_AREA or BUFFER_AREA)
 * Arguments    : area        : FWUP_AREA_MAIN/FWUP_AREA_BUFFER/FWUP_AREA_DATA_FLASH
 *                vp_ctx      : crypt library's context
 *                area_offset : offset in area
 *                datalen     : data length
 * Return Value : none.
 *********************************************************************************************************************/
static int32_t sha256_update(e_fwup_area_t area, void * vp_ctx, uint32_t area_offset, uint32_t datalen)
{
    uint32_t rbuf[FWUP_READ_BUF_SIZE / sizeof(uint32_t)];
    uint32_t rbuf_size = FWUP_READ_BUF_SIZE;
    uint32_t area_offset_tmp = area_offset;

    /* WAIT_LOOP */
    while (datalen > 0)
    {
        if (datalen < rbuf_size)
        {
            rbuf_size = datalen;
        }

        /* Read flash */
        read_area(area, (uint32_t *)rbuf, area_offset_tmp, rbuf_size);
        r_fwup_wrap_sha256_update(vp_ctx, (C_U8_FAR *)rbuf, rbuf_size);

        area_offset_tmp += rbuf_size;
        datalen -= rbuf_size;
    }
    return (0);
}
/**********************************************************************************************************************
 End of function sha256_update
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: hash_sha256
 * Description  : calculate SHA256 hash on main area / buffer area.
 * Arguments    : area    : FWUP_AREA_MAIN/FWUP_AREA_BUFFER
 * Return Value : pointer of hash data.
 *********************************************************************************************************************/
static uint8_t * hash_sha256(e_fwup_area_t area)
{
    static uint8_t puc_hash[FWUP_HASH_BYTES];
    uint32_t area_offset;
    st_fw_desc_t dc;
    void * vp_ctx = r_fwup_wrap_get_crypt_context();

    r_fwup_wrap_sha256_init(vp_ctx);

    /* Read N, addr, size from update list */
    area_offset = sizeof(st_fw_header_t);
    read_area(area, (uint32_t *)&dc, area_offset, sizeof(st_fw_desc_t));

    /* update list */
    sha256_update(area, vp_ctx, area_offset, sizeof(st_fw_desc_t));

    /* program code */
    /* WAIT_LOOP */
    for (uint8_t cnt = 0; cnt < dc.n; cnt++)
    {
        if ((FWUP_CFG_DF_ADDR_L <= dc.fw[cnt].addr) && (dc.fw[cnt].addr < (FWUP_CFG_DF_ADDR_L + FWUP_DF_NUM_BYTES)))
        {
            /* Data flash */
            sha256_update(FWUP_AREA_DATA_FLASH, vp_ctx, dc.fw[cnt].addr, dc.fw[cnt].size);
        }
        else
        {
            /* Code flash */
            area_offset =  get_offset_from_install_area(dc.fw[cnt].addr);
            sha256_update(area, vp_ctx, area_offset, dc.fw[cnt].size);
        }
    }
    r_fwup_wrap_sha256_final(puc_hash, vp_ctx);
    return (puc_hash);
}
/**********************************************************************************************************************
 End of function hash_sha256
 *********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: get_flash_write_addr
* Description  : Get flash write start address, offset from write start address, write size, descriptor index from rsu_offset.
* Arguments    : area                : FWUP_AREA_MAIN/FWUP_AREA_BUFFER/FWUP_AREA_DATA_FLASH
*                p_buf:(i/o)         : (i)image data's pointer ,  (o)left buffer
*                buf_sz_tmp          :   buffer size
*                rsu_offset          : Offset from the start address of the user program following Descriptor.
*                p_write_offset:(o)  : Indicates the offset from Start Address[X].
*                p_write_address:(o) : write start address.
*                p_write_size:(o)    : Write size is Data Size[X] minus write_offset.
* Return Value : dc_index            : Descriptor index
**********************************************************************************************************************/
static uint32_t get_flash_write_addr(e_fwup_area_t area, uint32_t buf_sz_tmp, uint32_t rsu_offset, uint32_t *p_write_offset, uint32_t *p_write_address, uint32_t *p_write_size)
{
    uint32_t dc_index = FWUP_IMAGE_BLOCKS;
    uint32_t rsu_index;
    st_fw_desc_t dc;

    rsu_index = 0;

    /* Get N, addr, size */
    read_area(area, (uint32_t *)&dc, sizeof(st_fw_header_t), sizeof(st_fw_desc_t));
    /* WAIT_LOOP */
    for (uint32_t cnt = 0; cnt < dc.n; cnt++)
    {
        if ((rsu_index + dc.fw[cnt].size) >= rsu_offset)
        {
            if ((rsu_index + dc.fw[cnt].size) == rsu_offset)
            {
                *p_write_offset = 0;
                if ((cnt + 1) < dc.n)
                {
                    cnt++;
                }
            }
            else
            {
                *p_write_offset = rsu_offset - rsu_index;
            }
            if (dc.fw[cnt].size > (*p_write_offset + buf_sz_tmp))
            {
                *p_write_size = buf_sz_tmp;
            }
            else
            {
                *p_write_size = dc.fw[cnt].size - *p_write_offset;
            }
            *p_write_address = dc.fw[cnt].addr + *p_write_offset;
            dc_index = cnt;
            break;
        }
        rsu_index += dc.fw[cnt].size;
    }
    return dc_index;

}
/**********************************************************************************************************************
 End of function get_flash_write_addr
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: write_area_offset
 * Description  : Write firmware data to designated area.
 * Arguments    : area               : FWUP_AREA_MAIN/FWUP_AREA_BUFFER/FWUP_AREA_DATA_FLASH
 *                p_buf:(i/o)        : (i)image data's pointer ,  (o)left buffer
 *                p_bufsz:(i/o)      : (i)buffer size,            (o)left buffer size
 *                offset             : write offset from install area. (not flash address!!!)
 *                size               : write size
 *                p_write_offset:(o) : Indicates the offset from Start Address[X].
 *                p_write_size:(o)   : Write size is Data Size[X] minus write_offset.
 * Return Value : FWUP_SUCCESS       : done     (enough buffer)
 *                FWUP_PROGRESS      : progress (not enough buffer)
 *                FWUP_ERR_FLASH     : flash write error
 *********************************************************************************************************************/
static e_fwup_err_t write_area_offset(e_fwup_area_t area, uint8_t **p_buf,
                               uint32_t *p_bufsz, uint32_t offset, uint32_t size, uint32_t *p_write_offset, uint32_t *p_write_size)
{
    uint32_t start_addr = FWUP_CFG_MAIN_AREA_ADDR_L + offset;
    e_fwup_err_t (*pfunc)(uint32_t, uint32_t, uint32_t) = r_fwup_wrap_flash_write;

    if (FWUP_AREA_BUFFER == area)
    {
        pfunc = FWUP_FUNC_BUFFER_AREA_WRITE;
        start_addr = FWUP_BUF_AREA_ADDR_L + offset;
    }
    else if (FWUP_AREA_DATA_FLASH == area)
    {
        start_addr = FWUP_CFG_DF_ADDR_L + offset;
    }
    else
    {
        ; /* Do nothing */
    }

    /* Is buffer enough? */
    if (size > (*p_write_offset + (*p_bufsz)))
    {
        /* not enough */
        *p_write_size = *p_bufsz;
    }
    else
    {
        /* enough */
        *p_write_size = size - *p_write_offset;
    }

    /* Write firmware */
    if (FWUP_SUCCESS != pfunc((uint32_t)*p_buf, start_addr, *p_write_size))
    {
        return (FWUP_ERR_FLASH);
    }
    FWUP_LOG_DBG(MSG_WRITE_OK, start_addr, *p_write_size);
    s_write_current_size += *p_write_size;

    *p_buf += *p_write_size;
    *p_bufsz -= *p_write_size;
    if (size > *p_write_offset + *p_write_size )
    {
        return (FWUP_PROGRESS);
    }
    *p_write_offset = 0;
    return (FWUP_SUCCESS);
}
/**********************************************************************************************************************
 End of function write_area_offset
 *********************************************************************************************************************/

